/////////////////////////////////////////////////////////////////
//    eventfunction.cpp                                        //
//    Created by Mikaela Smit on 07/11/2014.				   //
//    Copyright (c) 2014 Mikaela Smit. All rights reserved.    //
//    These event are added to the EventQ.					   //
/////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <math.h>
#include <vector>
#include <random>
#include <cmath>
#include "eventfunctions.h"
#include "event.h"
#include "eventQ.h"
#include "person.h"
#include "errorcoutmacro.h"
#include "CParamReader.hpp"
#include "LoadParams.h"
#include "CountryParams.hpp"
#include "Intervention.hpp"
#include "HPVInfection.hpp"
#include "NCD.hpp"

using namespace std;


//// --- OUTSIDE INFORMATION --- ////
extern double *p_GT;
extern int EndYear;
extern priority_queue<event*, vector<event*>, timeComparison> *p_PQ;
extern int total_population;
extern person** MyArrayOfPointersToPeople;
extern int *p_PY;
extern vector<event*> Events;
extern int ART_start_yr;
extern int country;

extern double*** CD4_startarray;
extern double**  CD4_prog_rates;
extern double*** Death_CD4_rates;
extern double*** ART_CD4_rates;
extern int* ARTKids;
extern int** ARTMen;
extern int* ARTMen_sum;
extern int** ARTWomen;
extern int* ARTWomen_sum;

extern double**  NCDArray;
extern int*      NCDAgeArrayMin;
extern int*      NCDAgeArrayMax;

extern double** DeathArray_Women;
extern double** DeathArray_Men;

extern double   HPV_Prevalence;
extern double   HPV_Screening_coverage;
extern int      age_tostart_CCscreening;
extern int CC_Screening_Count;
extern int CC_ScreenOutcome;
extern int CC_CryoOutcome;
extern double Re_ScreenOn;

extern int      age_atrisk_hpv;
extern double** HPVarray;



//// --- Important Internal informtaion --- ////
extern int RandomMinMax_2(int min, int max);

extern double randfrom(double min, double max);


//// --- NCD PARAMETERS HERE --- ////
//// NCD INTERACTION PARAMETERS ////
extern double Risk_DiabHC;                                        // Having high cholesterol given diabtes etc ...
extern double Risk_DiabHT;
extern double Risk_DiabCKD;
extern double Risk_DiabCVD;

extern double Risk_HCHT;
extern double Risk_HCCVD;

extern double Risk_HTCKD;
extern double Risk_HTCVD;


/// Mortality for NCDs Parameters ///
extern double MortRisk[6]; //{0.087, 0, 1.4, 670.87, 12.23, 5};         // Original values from Smith et al Factors associated with : 1.52 (HT), 1.77 (diabetes)
extern double MortRisk_Cancer[5];                   //{0.087, 0, 1.4, 670.87, 12.23};   // Both this and above needs to be fitted
extern double MortAdj;
extern double ARTbuffer;


/// HIV Increased risk for NCD Parameter ///
double Risk_HIVHT=1.49;                                       // Increased risk of HT given HIV (and CKD given HIV below) - from Schouten et al CID 2016
double Risk_HIVCKD=2;

double Risk_NCDHIV[2]={Risk_HIVHT, Risk_HIVCKD};
int relatedNCD_HIV[2]={0, 5};
int nr_NCD_HIV=sizeof(relatedNCD_HIV)/sizeof(relatedNCD_HIV[0]);
extern int ageAdult;
extern double ARTbuffer;


//// Tidy up
extern double Risk_NCD_Diabetes[5];
extern int relatedNCDs_Diab[5];
extern int nr_NCD_Diab;


extern double Risk_NCD_HT[3];
extern int relatedNCDs_HT[3];
extern int nr_NCD_HT;

extern double Risk_NCD_HC[3];
extern int relatedNCDs_HC[3];
extern int nr_NCD_HC;


// Count to compare to sum and CD4-specific numbers
int count_ARTMen[7]={0, 0, 0, 0, 0, 0, 0};        // Count by CD4 count category
int count_ARTAdult_Women[7]={0, 0, 0, 0, 0, 0, 0};      // Count by CD4 count category
int count_ARTMen_sum=0;                                 // Count the sum for the year - men
int count_ARTWomen_sum=0;                               // Count the sum for the year - women
int count_ARTKids=0;
int count_AdultsART=0;


// Index for the ART year
int ART_index=0;


// To get random index and only chekc HIV+
int HIV_Ref_PersonID[500000]={};
int countHIVRef=0;

int KIDS_HIV_Ref_PersonID[70000]={};
int countKIDSHIVRef=0;

// Count real available patients for error message
int Elig_Men=0;
int Elig_Women=0;
int Elig_Kids=0;

// Vaccination variables
extern int age_HPVvaccination;
extern int int_HPVvaccination;
extern int yearintervention_start;



//////////////////////////////////////
//// --- FUNCTIONS FOR EVENTS --- ////
//////////////////////////////////////

//// --- NEW YEAR FUNCTION --- ////                                     // includes count of adults/kids on ART for error/infinite loop problems
void EventTellNewYear(person *MyPointerToPerson){
    
    cout << "A new year has started, it is now " << *p_GT << endl;      // Tells us a new year has started
    
    
    // Lets set the first people on ART
    if (*p_GT>=2004){
        
        // Add people on ART until we hit our aim
        while (count_ARTKids<ARTKids[ART_index] || ARTbuffer*count_ARTMen_sum<ARTMen_sum[ART_index] || ARTbuffer*count_ARTWomen_sum<ARTWomen_sum[ART_index]){
            
            //cout << endl << "Its " << *p_GT << " KIDS: There have " << Elig_Kids << " eligible kids and we need to reach " << ARTKids[ART_index] << " and only have " << count_ARTKids   << endl;
            //cout << "Men: There have " << Elig_Men << " eligible men and we need to reach " << ARTMen_sum[ART_index] << " and only have " << count_ARTMen_sum << " times 10% " << ARTbuffer*count_ARTMen_sum << endl;
            //cout << "Women: There have " << Elig_Women << " eligible women and we need to reach " << ARTWomen_sum[ART_index] << " and only have " << count_ARTWomen_sum << " times 10% " << ARTbuffer*count_ARTWomen_sum<< endl;
            
            
            if (count_ARTKids<ARTKids[ART_index] ){
                
                int a=(RandomMinMax_2(1,countKIDSHIVRef-1));    // Get a random person and update age
                int i=KIDS_HIV_Ref_PersonID[a]-1;
                //cout << "totalpopulation " << total_population << " a " << a <<  " Kids ref " << KIDS_HIV_Ref_PersonID[a] << endl;
                //cout << "I: " << i << " Age " << MyArrayOfPointersToPeople[i]->Age << " GT " << *p_GT << endl;
                MyArrayOfPointersToPeople[i]->Age=(*p_GT - MyArrayOfPointersToPeople[i]->DoB);
                
                
                // Lets get some kids onto ART
                if (((MyArrayOfPointersToPeople[i]->Age<ageAdult && *p_GT<2012)|| (MyArrayOfPointersToPeople[i]->Age<ageAdult && *p_GT>=2012)) && MyArrayOfPointersToPeople[i]->HIV>0 && MyArrayOfPointersToPeople[i]->HIV<=*p_GT && MyArrayOfPointersToPeople[i]->ART==-999 && MyArrayOfPointersToPeople[i]->Alive==1 && count_ARTKids<ARTKids[ART_index] ){
                    
                    MyArrayOfPointersToPeople[i]->ART=*p_GT;            // Lets set ART date
                    MyArrayOfPointersToPeople[i]->CD4_cat_ARTstart=MyArrayOfPointersToPeople[i]->CD4_cat;       // Lets set CD4 cat at ART start
                    count_ARTKids++;                                    // Update our counter
                    Elig_Kids=Elig_Kids-1;
                }
            }
            
            
            // Lets see if adults start ART
            if (ARTbuffer*count_ARTMen_sum<ARTMen_sum[ART_index] || ARTbuffer*count_ARTWomen_sum<ARTWomen_sum[ART_index]){
                
                int a=(RandomMinMax_2(1,countHIVRef-1));
                //cout << "Random number: " << a << " counter: " << countHIVRef << endl;
                int i=HIV_Ref_PersonID[a]-1;
                MyArrayOfPointersToPeople[i]->Age=(*p_GT - MyArrayOfPointersToPeople[i]->DoB);
                
                // Lets let men start ART
                if (MyArrayOfPointersToPeople[i]->Sex==1 && MyArrayOfPointersToPeople[i]->Age>=ageAdult && MyArrayOfPointersToPeople[i]->HIV>0 && MyArrayOfPointersToPeople[i]->HIV<*p_GT && MyArrayOfPointersToPeople[i]->ART==-999 &&MyArrayOfPointersToPeople[i]->Alive==1 && count_ARTMen_sum<ARTMen_sum[ART_index] ){
                    
                    
                    
                    if (ARTbuffer*ARTMen[ART_index][MyArrayOfPointersToPeople[i]->CD4_cat] > count_ARTMen[MyArrayOfPointersToPeople[i]->CD4_cat]){
                        
                        MyArrayOfPointersToPeople[i]->ART=*p_GT;                        // Lets set ART date
                        MyArrayOfPointersToPeople[i]->CD4_cat_ARTstart=MyArrayOfPointersToPeople[i]->CD4_cat;       // Lets set CD4 cat at ART start
                        count_ARTMen[MyArrayOfPointersToPeople[i]->CD4_cat]++;    // Update our counter CD4/ART array
                        count_ARTMen_sum++;                                             // Update the sum counter
                        count_AdultsART++;
                        Elig_Men=Elig_Men-1;
                    }
                    
                }
                
                
                // Lets start women on ART
                if (MyArrayOfPointersToPeople[i]->Sex==2 && MyArrayOfPointersToPeople[i]->Age>=ageAdult && MyArrayOfPointersToPeople[i]->HIV>0 && MyArrayOfPointersToPeople[i]->HIV<*p_GT && MyArrayOfPointersToPeople[i]->ART==-999 &&MyArrayOfPointersToPeople[i]->Alive==1 && count_ARTWomen_sum<ARTWomen_sum[ART_index]){
                    
                    
                    if (ARTbuffer*ARTWomen[ART_index][MyArrayOfPointersToPeople[i]->CD4_cat] > count_ARTAdult_Women[MyArrayOfPointersToPeople[i]->CD4_cat]){
                        
                        
                        MyArrayOfPointersToPeople[i]->ART=*p_GT;            // Lets set ART date
                        MyArrayOfPointersToPeople[i]->CD4_cat_ARTstart=MyArrayOfPointersToPeople[i]->CD4_cat;       // Lets set CD4 cat at ART start
                        count_ARTAdult_Women[MyArrayOfPointersToPeople[i]->CD4_cat]++;  // Update our counter CD4/ART array
                        count_ARTWomen_sum++;                               // Update the sum counter
                        count_AdultsART++;
                        Elig_Women=Elig_Women-1;
                        
                        //cout << MyArrayOfPointersToPeople[i]->PersonID << " has an ART start date of " << MyArrayOfPointersToPeople[i]->ART << endl;
                        
                    }
                    
                }
            }
            
        }
        
        //        cout << endl << " we have finished assigning ART KIDS: There have " << Elig_Kids << " eligible kids and we need to reach " << ARTKids[ART_index] << " and only have " << count_ARTKids   << endl;
        //        cout << "Men: There have " << Elig_Men << " eligible men and we need to reach " << ARTMen_sum[ART_index] << " and only have " << count_ARTMen_sum << " times 20% " << 1.2*count_ARTMen_sum << endl;
        //        cout << "Women: There have " << Elig_Women << " eligible women and we need to reach " << ARTWomen_sum[ART_index] << " and only have " << count_ARTWomen_sum << " times 20% " << 1.2*count_ARTWomen_sum<< endl;
        
        // Lets update the ART index
        ART_index++;
        if (ART_index>12){ART_index=12;}
        
    }
    
    
    
    // Lets get the pointer to the right year range
    if (*p_GT<1955){*p_PY = 0; };
    if (*p_GT >= 1955 && *p_GT<1960){*p_PY = 1; };
    if (*p_GT >= 1960 && *p_GT<1965){*p_PY = 2; };
    if (*p_GT >= 1965 && *p_GT<1970){*p_PY = 3; };
    if (*p_GT >= 1970 && *p_GT<1975){*p_PY = 4; };
    if (*p_GT >= 1975 && *p_GT<1980){*p_PY = 5; };
    if (*p_GT >= 1980 && *p_GT<1985){*p_PY = 6; };
    if (*p_GT >= 1985 && *p_GT<1990){*p_PY = 7; };
    if (*p_GT >= 1990 && *p_GT<1995){*p_PY = 8; };
    if (*p_GT >= 1995 && *p_GT<2000){*p_PY = 9; };
    if (*p_GT >= 2000 && *p_GT<2005){*p_PY = 10;};
    if (*p_GT >= 2005){*p_PY = 11; };
    E(cout << "Let's check relevant things have been updated... *p_PY: " << *p_PY << " and Global Time: " << *p_GT << endl;)
    
    // Schedule event for next year
    event * RecurrentTellNewYear = new event;
    Events.push_back(RecurrentTellNewYear);
    RecurrentTellNewYear->time = *p_GT + 1;
    RecurrentTellNewYear->p_fun = &EventTellNewYear;
    p_PQ->push(RecurrentTellNewYear);
    
    E(cout << "We have finished telling you the new year and setting fertility variables for the year." << endl;)	// Error message - can be switched on/off
}

//// --- ART Category switch from Kids to Adult --- ////
void EventARTCatSwitch(person *MyPointerToPerson){
    
    E(cout << "We are switching our kids to adult ART count: " << endl;)       // Error message - can be switched on/off
    
    if(MyPointerToPerson->Alive == 1) {
        
        
        // If they are not on ART
        if (MyPointerToPerson->ART==-999){
            
            // First we need to add them to eligible adults for selection
            HIV_Ref_PersonID[countHIVRef]=MyPointerToPerson->PersonID;         // And the patientID reference
            countHIVRef++;                                                     // Then we need to updated the adult counter
            Elig_Kids=Elig_Kids-1;
            if (MyPointerToPerson->Sex==1){Elig_Men++;}
            if (MyPointerToPerson->Sex==2){Elig_Women++;}
        }
        
        // If they are on ART
        if (MyPointerToPerson->ART>0){
            
            if (count_ARTKids>0) {count_ARTKids=count_ARTKids-1;}              // This count how many children are still on ART
            
            
            if (MyPointerToPerson->Sex==1){
                count_ARTMen_sum++;                                             // Lets add a count to the adult category
                count_ARTMen[4]++;                                        // Lets update the ART CD4 array counter
            }
            
            if (MyPointerToPerson->Sex==2){
                count_ARTWomen_sum++;                                           // Lets add a count to the adult category
                count_ARTAdult_Women[4]++;                                      // Lets update the ART CD4 array counter
            }
        }
    }
}


//// --- DEATH EVENT --- ////
void EventMyDeathDate(person *MyPointerToPerson){
    
    
    // Lets kills people
    if (MyPointerToPerson->Alive==1){
        
        MyPointerToPerson->Alive=0;
        
        if (MyPointerToPerson->CauseOfDeath==-999)
        {
            MyPointerToPerson->CauseOfDeath=1;
        }
        
        
        // Lets also update ART information as people die
        // 1. If they are not on ART
        if (MyPointerToPerson->HIV>0 && MyPointerToPerson->HIV<*p_GT && MyPointerToPerson->ART==-999){
            
            // First we need to update age
            MyPointerToPerson->Age=(*p_GT - MyPointerToPerson->DoB);  // First we update age
            
            if (MyPointerToPerson->Sex==1 && MyPointerToPerson->Age>=ageAdult){Elig_Men=Elig_Men-1;}
            if (MyPointerToPerson->Sex==2 && MyPointerToPerson->Age>=ageAdult){Elig_Women=Elig_Women-1;}
            if (MyPointerToPerson->Age<ageAdult){Elig_Kids=Elig_Kids-1;}
        }
        
        
        // 2. If they are on ART
        if (MyPointerToPerson->HIV>0 && MyPointerToPerson->HIV<*p_GT && MyPointerToPerson->ART>0){
            
            // First we need to update age
            MyPointerToPerson->Age=(*p_GT - MyPointerToPerson->DoB);  // First we update age
            
            // If it is a child
            if (MyPointerToPerson->Age<ageAdult){
                if (count_ARTKids>0) {count_ARTKids=count_ARTKids-1;}
            }
            
            // If it is a man
            if (MyPointerToPerson->Sex==1 && MyPointerToPerson->Age>=ageAdult){
                count_ARTMen[MyPointerToPerson->CD4_cat_start]=count_ARTMen[MyPointerToPerson->CD4_cat_start]-1;
                count_ARTMen_sum=count_ARTMen_sum-1;
            }
            
            // If it is a woman
            if (MyPointerToPerson->Sex==2 && MyPointerToPerson->Age>=ageAdult){
                count_ARTAdult_Women[MyPointerToPerson->CD4_cat_start]=count_ARTAdult_Women[MyPointerToPerson->CD4_cat_start]-1;
                count_ARTWomen_sum=count_ARTWomen_sum-1;
            }
        }
    }
    
    
    E(cout << "Person " << MyPointerToPerson->PersonID << " just died. Their life status now is: " << MyPointerToPerson->Alive << endl;)
}


//// --- BIRTH EVENT AND MAKING NEW PERSON --- ////
void EventBirth(person *MyPointerToPerson){
    
    E(cout << "A birth is about to happen and my life status: " << endl;)		// Error message - can be switched on/off
    
    if(MyPointerToPerson->Alive == 1) {											// Only let woman give birth if she is still alive
        
        total_population=total_population+1;									// Update total population for output and for next new entry
        MyPointerToPerson->Age= (*p_GT - MyPointerToPerson->DoB);				// Update age to get age at birth for output
        
        
        // Creating a new person
        MyArrayOfPointersToPeople[total_population-1]=new person();
        (MyArrayOfPointersToPeople[total_population-1])->PersonIDAssign(total_population-1);
        (MyArrayOfPointersToPeople[total_population-1])->Alive=1;
        (MyArrayOfPointersToPeople[total_population-1])->GenderDistribution();
        (MyArrayOfPointersToPeople[total_population-1])->GetMyDoBNewEntry();
        (MyArrayOfPointersToPeople[total_population-1])->GetDateOfDeath();
        (MyArrayOfPointersToPeople[total_population-1])->GetDateOfBaby();
        (MyArrayOfPointersToPeople[total_population-1])->GetMyDateNCD();
        (MyArrayOfPointersToPeople[total_population-1])->GetMyDateCancers();
        (MyArrayOfPointersToPeople[total_population-1])->GetMyDateOfHPVInfection();
        (MyArrayOfPointersToPeople[total_population-1])->GetMyDateOfHIVInfection();
        
        
        
        // Link Mother and Child
        (MyArrayOfPointersToPeople[total_population-1])->MotherID=MyPointerToPerson->PersonID;			// Give child their mothers ID
        MyPointerToPerson->ChildIDVector.push_back((MyArrayOfPointersToPeople[total_population-1]));	// Give mothers their child's ID
        
        
        // This code is for the HOV vaccination Intervention
        if (*p_GT>=(yearintervention_start-age_HPVvaccination) && int_HPVvaccination==1)
        {
            
            event * HPV_VaccinationEvent = new event;
            Events.push_back(HPV_VaccinationEvent);
            HPV_VaccinationEvent->time = MyArrayOfPointersToPeople[total_population-1]->DoB + age_HPVvaccination;
            HPV_VaccinationEvent->p_fun = &EventMyHPVVaccination;
            HPV_VaccinationEvent->person_ID = MyArrayOfPointersToPeople[total_population-1];
            p_PQ->push(HPV_VaccinationEvent);
            
            
        }
        
        E(cout << "We have finished giving birth " << endl;)					// Error message - can be switched on/off
        
    }
}





//// --- HIV EVENT --- ////
void EventMyHIVInfection(person *MyPointerToPerson){
    
    E(cout << "Somebody is about to get infected with HIV: " << endl;)			// Error message - can be switched on/off
    
    if(MyPointerToPerson->Alive == 1) {											// Only execute this is patient is still alove
        
        if (MyPointerToPerson->DateOfDeath<MyPointerToPerson->HIV){
            cout << "Error! GT " << *p_GT << " PersonID: " << MyPointerToPerson->PersonID << " Death: " << MyPointerToPerson->DateOfDeath << " HIV: " << MyPointerToPerson->HIV<< endl;
        }
        
        MyPointerToPerson->Age= (*p_GT - MyPointerToPerson->DoB);				// Update age to get correct parameter below
        
        //// Update key parameters for ART
        if (MyPointerToPerson->Age>ageAdult){
            HIV_Ref_PersonID[countHIVRef]=MyPointerToPerson->PersonID;
            countHIVRef++;
            if (MyPointerToPerson->Sex==1){Elig_Men++;}
            if (MyPointerToPerson->Sex==2){Elig_Women++;}
        }
        
        if (MyPointerToPerson->Age<ageAdult) {
            KIDS_HIV_Ref_PersonID[countKIDSHIVRef]=MyPointerToPerson->PersonID;
            countKIDSHIVRef++;
            Elig_Kids++;
            
            // Now we need to schedule an event where we move them to adult category
            double DateCD4CatSwitch=(MyPointerToPerson->DoB+ageAdult);
            
            // Schedule event for moving from kids ART to adult ART
            event * ARTCatMove = new event;
            Events.push_back(ARTCatMove);
            ARTCatMove->time = DateCD4CatSwitch;
            ARTCatMove->p_fun = &EventARTCatSwitch;
            ARTCatMove->person_ID = MyPointerToPerson;
            p_PQ->push(ARTCatMove);
        }
        
        //// --- Get my CD4 count at start --- ////
        double	h = ((double)rand() / (RAND_MAX));								// Gets a random number between 0 and 1.
        int i=0;
        int j=0;
        int a=25;
        
        while (MyPointerToPerson->Age>=a && a<46){a=a+10; i++;};				// To get the right age-specific row in the above sex-specific arrays
        while (h>CD4_startarray[MyPointerToPerson->Sex-1][i][j] && j<3){j++;}	// To get the corresponding CD4 count for correct age and sex from random 'h' generated above
        
        MyPointerToPerson->CD4_cat=0+j;											// CD4 count cat (variable over time)
        MyPointerToPerson->CD4_cat_start=0+j;									// CD4 count cat at start (to keep in records)
        MyPointerToPerson->CD4_change.at(MyPointerToPerson->CD4_cat)=*p_GT;
        
        //// --- Let's see what will happen next (Death, CD4 count progression or ART initiation) ---- ////
        double FindART_CD4_rate = ART_CD4_rates[MyPointerToPerson->Sex-1][i][MyPointerToPerson->CD4_cat];
        double FindCD4_rate = CD4_prog_rates[MyPointerToPerson->Sex-1][MyPointerToPerson->CD4_cat];
        double FindDeath_CD4_rate = Death_CD4_rates[MyPointerToPerson->Sex-1][i][MyPointerToPerson->CD4_cat];
        
        // Lets see when CD4 count progression would start
        double CD4_test = 0;
        double cd4 = ((double)rand() / (RAND_MAX));
        CD4_test = (-1/FindCD4_rate) * log(cd4);
        
        // Lets see when death would happen
        double death_test = 0;
        double dd = ((double)rand() / (RAND_MAX));
        death_test = (-1/FindDeath_CD4_rate) * log(dd);
        
        
        //  If they do not start ART then we need to evaluate if they die or decrease in CD4 count
        if (MyPointerToPerson->ART<0){
            
            if (CD4_test<death_test){
                event * CD4change = new event;
                Events.push_back(CD4change);
                CD4change->time = *p_GT+CD4_test;
                CD4change->p_fun = &EventCD4change;
                CD4change->person_ID = MyPointerToPerson;
                p_PQ->push(CD4change);
                
            }
            
            if (death_test<=CD4_test){
                
                double death_test_date = *p_GT +death_test;                  // Get the actual date, not just time until death
                
                if (death_test_date<MyPointerToPerson->DateOfDeath && death_test_date>*p_GT){   // Check HIV deaths happens before natural death
                    
                    MyPointerToPerson->DateOfDeath=death_test_date;
                    
                    event * DeathEvent = new event;						    // In that case we need to add the new death date to the EventQ
                    Events.push_back(DeathEvent);
                    DeathEvent->time = MyPointerToPerson->DateOfDeath;
                    DeathEvent->p_fun = &EventMyDeathDate;
                    DeathEvent->person_ID = MyPointerToPerson;
                    p_PQ->push(DeathEvent);
                    
                    MyPointerToPerson->CauseOfDeath=2;                      // and we will need to update Cause of Death
                }
            }
        }
        
        // Now we need to update NCD risks for HT and CKD
        E(cout << endl << endl << "We are assigning NCDs for HIV+ upon infection!" << endl;)
        
        double age_at_death=MyPointerToPerson->DateOfDeath-MyPointerToPerson->DoB;
        
        if (age_at_death>18)
        {
            
            // Some basic code and finding index for not getting NCDs
            int ncd_nr=0;                                                  // Assisgn all the possible NCDs in this code
            double DateNCD=-997;
            
            
            // Re-evaluate NCD related to HIV
            while (ncd_nr<nr_NCD_HIV){
                double r = ((double) rand() / (RAND_MAX));                 // Get a random number for each NCD
                
                
                // If we are getting an NCD lets get age and date
                if (r<=NCDArray[relatedNCD_HIV[ncd_nr]][120]*Risk_NCDHIV[ncd_nr])             // If they will get and NCD lets get the age and date and also update mortality
                {
                    
                    // Lets get the index for age at NCD
                    int i=0;
                    while (r>NCDArray[relatedNCD_HIV[ncd_nr]][i]*Risk_NCDHIV[ncd_nr]){i++;}
                    
                    // Lets get the age and date they will have the NCD
                    double YearFraction=(RandomMinMax_2(1,12))/12.1;                          // This gets month of birth as a fraction of a year
                    DateNCD=MyPointerToPerson->DoB+i+YearFraction;
                    
                    // Lets see if this pushed NCD date forward
                    if (DateNCD>=*p_GT && DateNCD<MyPointerToPerson->NCD_DatesVector.at(relatedNCD_HIV[ncd_nr]))
                    {
                        // Lets update the Date everywhere and add to the queue
                        MyPointerToPerson->NCD_DatesVector.at(relatedNCD_HIV[ncd_nr])=DateNCD;
                        
                        
                        if (ncd_nr==0)
                        {
                            MyPointerToPerson->HT=DateNCD;
                            //// --- Lets feed Hypertension into the eventQ --- ////
                            int p=MyPointerToPerson->PersonID-1;
                            event * HTEvent = new event;
                            Events.push_back(HTEvent);
                            HTEvent->time = MyPointerToPerson->HT;
                            HTEvent->p_fun = &EventMyHyptenDate;
                            HTEvent->person_ID = MyArrayOfPointersToPeople[p];
                            p_PQ->push(HTEvent);
                        }
                        
                        
                        else if (ncd_nr==1)
                        {
                            MyPointerToPerson->CKD=DateNCD;
                            //// --- Lets feed MI into the eventQ --- ////
                            int p=MyPointerToPerson->PersonID-1;
                            event * CKDEvent = new event;
                            Events.push_back(CKDEvent);
                            CKDEvent->time = MyPointerToPerson->CKD;
                            CKDEvent->p_fun = &EventMyCKDDate;
                            CKDEvent->person_ID = MyArrayOfPointersToPeople[p];
                            p_PQ->push(CKDEvent);
                        }
                        
                        
                    }
                    
                    ncd_nr++;                                                    // Lets do the next NCD
                    
                }
            }
        }
        E(cout << "We finished assigning NCDs for HIV+!" << endl;)
        
        // Now we need to re-evaluate the risk of HPV infection for women with HIV
        
        E(cout << endl << endl << "We're re-evaluating the probability of HPV for HIV+ women upon infection!" << endl);
        
        if (age_at_death>age_atrisk_hpv && MyPointerToPerson->Sex==2)
            //cout << "Age at risk" << age_atrisk_hpv << endl;
        {
            double ReTestHPVDate=0;
            double r = ((double) rand() / (RAND_MAX));
            
            if (r<=HPVarray[1][65])                     // They will get HPV infection!
            {
                int i=0;
                while (r>HPVarray[1][i]){i++;
                }
                double YearFraction=(RandomMinMax_2(1,12))/ 12.1;
                ReTestHPVDate=MyPointerToPerson->DoB+i+YearFraction;
                
                
                
                if (ReTestHPVDate>*p_GT && ReTestHPVDate<MyPointerToPerson->HPV_DateofInfection)
                {
                    MyPointerToPerson->HPV_DateofInfection=ReTestHPVDate;    // The newly assigned date of infection replaces the old one
                    
                    int p=MyPointerToPerson->PersonID-1;
                    event * HPV_DateofInfectionEvent = new event;
                    Events.push_back(HPV_DateofInfectionEvent);
                    HPV_DateofInfectionEvent->time = MyPointerToPerson->HPV_DateofInfection;
                    HPV_DateofInfectionEvent->p_fun = &EventMyHPVInfection;
                    HPV_DateofInfectionEvent->person_ID = MyArrayOfPointersToPeople[p];
                    p_PQ->push(HPV_DateofInfectionEvent);
                }
            }
        }
    }
    
    E(cout << "Somebody has just been infected with HIV!" << endl;)				// Error message - can be switched on/off
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventMyFirst_VIA_Screening(person *MyPointerToPerson){
    
    // Let's find all women on ART each year and screen them for cervical cancer with VIA
    for(int n=0; n<count_AdultsART; n++){
        
        // --- Set elegibility criteria for first screening with VIA
        if (MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->Alive==1 && MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->Sex==2 && MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_Screening_Count==0 && MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->ART<=*p_GT && MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->ART != -999){
            
            // --- Update age to get correct parameter below
            MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->Age=(*p_GT - MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->DoB);
            
            // --- A woman with no HPV infection or that has recovered from a previous infection is about to be screened
            if(MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->HPV_Status<0 || MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->HPV_Status==6)
            {
                MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_Screening_Count++;
                // Let's book her for a second screening in 6 months
                MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->Re_ScreenOn = *p_GT+0.5;
                int p=MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]]->PersonID;
                event * DateOfSecondScreening = new event;
                Events.push_back(DateOfSecondScreening);
                DateOfSecondScreening->time = MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]]->Re_ScreenOn;
                DateOfSecondScreening->p_fun = &EventMy_VIA_FollowUp;
                DateOfSecondScreening->person_ID = MyArrayOfPointersToPeople[p];
                p_PQ->push(DateOfSecondScreening);
            }
            
            // --- A woman with HPV infection, but no cervical abnormalities is about to be screened
            if (MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->HPV_Status==1)
            {
                MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_Screening_Count++;
                // Let's book her for a second screening in 6 months
                MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->Re_ScreenOn = *p_GT+0.5;
                int p=MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]]->PersonID;
                event * DateOfSecondScreening = new event;
                Events.push_back(DateOfSecondScreening);
                DateOfSecondScreening->time = MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]]->Re_ScreenOn;
                DateOfSecondScreening->p_fun = &EventMy_VIA_FollowUp;
                DateOfSecondScreening->person_ID = MyArrayOfPointersToPeople[p];
                p_PQ->push(DateOfSecondScreening);
                
            }
            
            // --- A woman with CIN1 is about to be screened
            if (MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->HPV_Status==2)
            {
                MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_Screening_Count++;
                double h = ((double) rand() / (RAND_MAX));  // Let's see if she's diagnosed
                if(h>0.672){    // No luck =(
                    MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_ScreenOutcome=2;   // 2 = False negative
                    // Let's book her for a second screening in 6 months, and hope she's lucky next time
                    MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->Re_ScreenOn = *p_GT+0.5;
                    int p=MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]]->PersonID;
                    event * DateOfSecondScreening = new event;
                    Events.push_back(DateOfSecondScreening);
                    DateOfSecondScreening->time = MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]]->Re_ScreenOn;
                    DateOfSecondScreening->p_fun = &EventMy_VIA_FollowUp;
                    DateOfSecondScreening->person_ID = MyArrayOfPointersToPeople[p];
                    p_PQ->push(DateOfSecondScreening);
                }
                if(h<=0.672) {  // She was lucky!
                    MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_ScreenOutcome=1;   // 1 = True positive
                    // Let's give her cryotherapy right now and see if she's cured
                    double j = ((double) rand() / (RAND_MAX));
                    if(j>0.875){    // Not cured... she'll need follow-up
                        MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_CryoOutcome=2;   // 2 = Cryo not successful
                    }
                    if(j<=0.875){   // She'll be cured with cryo!
                        MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_CryoOutcome=1;   // 1 = Cryo successful
                    }
                }
            }
            
            // --- A woman with CIN2/3 is about to be screened
            if (MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->HPV_Status==3)
            {
                MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_Screening_Count++;
            }
            
            // --- A woman with CIS is about to be screened
            if (MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->HPV_Status==4)
            {
                MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->CC_Screening_Count++;
                
            }
            
            // --- A woman with ICC is about to be screened
            if (MyArrayOfPointersToPeople[HIV_Ref_PersonID[n]-1]->HPV_Status==5)
            {
            }
            
        }
        
    }
    
    // Schedule event for next year
    event * RecurrentCC_FirstScreen = new event;
    Events.push_back(RecurrentCC_FirstScreen);
    RecurrentCC_FirstScreen->time = *p_GT + 1;
    RecurrentCC_FirstScreen->p_fun = &EventMyFirst_VIA_Screening;
    p_PQ->push(RecurrentCC_FirstScreen);
    
}

void EventMy_VIA_FollowUp(person *MyPointerToPerson){
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void EventCD4change(person *MyPointerToPerson){
    
    E(cout << "Somebody is about to experience a drop in CD4 count: " << endl;)	// Error message - can be switched on/off
    
    if(MyPointerToPerson->Alive == 1) {											// Add any additional things to progression of CD4 count
        
        //// --- Let's get the right index for all relevant arrays used here and update important info ---- ////
        int i=0;
        int a=25;
        
        while (MyPointerToPerson->Age>=a && a<46){a=a+10; i++;};				// To get the right age-specific row in the above sex-specific arrays
        MyPointerToPerson->Age= (*p_GT - MyPointerToPerson->DoB);				// Update age to get correct parameter below
        MyPointerToPerson->CD4_cat=MyPointerToPerson->CD4_cat+1;                // Update CD4 count
        MyPointerToPerson->CD4_change.at(MyPointerToPerson->CD4_cat)=*p_GT;
        
        
        //// --- When CD4 count hits the lowest possible value and no ART is started --- ////
        if (MyPointerToPerson->CD4_cat==6 && MyPointerToPerson->ART<0){
            
            double FindDeath_CD4_rate = Death_CD4_rates[MyPointerToPerson->Sex-1][i][MyPointerToPerson->CD4_cat];
            
            // Lets see when death would happen
            double death_test = 0;
            double dd = ((double)rand() / (RAND_MAX));
            death_test = (-1/FindDeath_CD4_rate) * log(dd);
            //cout <<  death_test << endl;
            
            double death_test_date = *p_GT +death_test;                      // Get the actual date, not just time until death
            
            if (death_test_date<MyPointerToPerson->DateOfDeath && death_test_date>*p_GT){            // Check HIV deaths happens before natural death
                
                MyPointerToPerson->DateOfDeath=death_test_date;
                
                event * DeathEvent = new event;								// In that case we need to add the new death date to the EventQ
                Events.push_back(DeathEvent);
                DeathEvent->time = MyPointerToPerson->DateOfDeath;
                DeathEvent->p_fun = &EventMyDeathDate;
                DeathEvent->person_ID = MyPointerToPerson;
                p_PQ->push(DeathEvent);
                
                MyPointerToPerson->CauseOfDeath=2;                          // and we will need to update Cause of Death
            }
        }
        
        //// --- In case CD4 count is higher than minimum possible category.  They can either die OR Progress AND they still have a 1 year risk of death if on ART --- ///
        if (MyPointerToPerson->CD4_cat<6){
            
            //// --- Let's see what will happen next (Death, CD4 count progression or ART initiation) ---- ////
            double FindART_CD4_rate = ART_CD4_rates[MyPointerToPerson->Sex-1][i][MyPointerToPerson->CD4_cat];
            double FindCD4_rate = CD4_prog_rates[MyPointerToPerson->Sex-1][MyPointerToPerson->CD4_cat];
            double FindDeath_CD4_rate = Death_CD4_rates[MyPointerToPerson->Sex-1][i][MyPointerToPerson->CD4_cat];
            
            // Lets see when CD4 count progression would start
            double CD4_test = 0;
            double cd4 = ((double)rand() / (RAND_MAX));
            CD4_test = (-1/FindCD4_rate) * log(cd4);
            
            
            // Lets see when death would happen
            double death_test = 0;
            double dd = ((double)rand() / (RAND_MAX));
            death_test = (-1/FindDeath_CD4_rate) * log(dd);
            
            
            // If patient hasn't started ART yet
            if (MyPointerToPerson->ART<0){
                
                if (CD4_test<death_test){
                    event * CD4change = new event;
                    Events.push_back(CD4change);
                    CD4change->time = *p_GT+CD4_test;
                    CD4change->p_fun = &EventCD4change;
                    CD4change->person_ID = MyPointerToPerson;
                    p_PQ->push(CD4change);
                }
                
                if (death_test<=CD4_test){
                    
                    double death_test_date = *p_GT +death_test;                      // Get the actual date, not just time until death
                    
                    if (death_test_date<MyPointerToPerson->DateOfDeath && death_test_date>*p_GT){            // Check HIV deaths happens before natural death
                        MyPointerToPerson->DateOfDeath=death_test_date;
                        event * DeathEvent = new event;								// In that case we need to add the new death date to the EventQ
                        Events.push_back(DeathEvent);
                        DeathEvent->time = MyPointerToPerson->DateOfDeath;
                        DeathEvent->p_fun = &EventMyDeathDate;
                        DeathEvent->person_ID = MyPointerToPerson;
                        p_PQ->push(DeathEvent);
                        
                        MyPointerToPerson->CauseOfDeath=2;                          // and we will need to update Cause of Death
                    }
                    
                }
                
            }
            
        }
        
    }
    
    E(cout << "Somebody has just experiences a drop in CD4 count!" << endl;)	// Error message - can be switched on/off
}


