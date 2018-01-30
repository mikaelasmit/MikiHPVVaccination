

///////////////////////////////////////////////////////////
//  Intervention.cpp                                     //
//  hivmodelzimbabwe                                     //
//                                                       //
//  Created by Mikaela Smit on 12/01/2018.               //
//  Copyright © 2018 Mikaela Smit. All rights reserved.  //
//  File for executing interventions                     //
//                                                       //
///////////////////////////////////////////////////////////


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

using namespace std;

// clean HPV intervention

//// --- OUTSIDE INFORMATION --- ////
extern double *p_GT;
extern int int_HPVvaccination;
extern person** MyArrayOfPointersToPeople;
extern priority_queue<event*, vector<event*>, timeComparison> *p_PQ;
extern vector<event*> Events;

extern double** HPVarray;
extern double   Re_ScreenOn;
extern int CC_Screening_Count;
extern int CC_ScreenOutcome;
extern int CC_CryoOutcome;
extern int age_atrisk_hpv;
extern int count_AdultsART;
extern int HIV_Ref_PersonID[500000];


// Function that contains all interventions to be rolled out - adjust as needed
void EventStartIntervention(person *MyPointerToPerson){
    
    // Rolling out HPV vaccination
    if (int_HPVvaccination==1)
    {
        cout << "We are rolling out vaccination for HPV and it is the year: " << *p_GT << endl;
    }
}


// Event that executes vaccination
void EventMyHPVVaccination(person *MyPointerToPerson){
    
    if (MyPointerToPerson->Alive == 1){
        
        MyPointerToPerson->HPVvaccination_status=1;
        MyPointerToPerson->HPVvaccination_date=*p_GT;
        
    }
}


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







