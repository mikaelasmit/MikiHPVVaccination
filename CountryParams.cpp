//////////////////////////////////////////////////////////
//                                                      //
//                  CountryParams.cpp                   //
//            Created by Pablo on 20/10/2017.           //
//  Copyright © 2017 Mikaela Smit. All rights reserved. //
//                                                      //
//////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "errorcoutmacro.h"
#include "CountryParams.hpp"
#include "LoadParams.h"

using namespace std;

// clean up all hpv related items

// General parameter
int UN_Pop;
int init_pop;
int total_population;
double Sex_ratio;
extern int factor;
extern int ageAdult;
int minAgeBirth;
int maxAgeBirth;


// HIV parameters
int ART_start_yr;
double ARTbuffer;

// HPV parameters
extern int age_atrisk_hpv;
extern int age_tostart_CCscreening;
extern double HPV_Prevalence;
extern int HPV_Status_HPV;
extern int HPV_Status_CIN1;
extern int HPV_Status_CIN2_3;
extern int HPV_Status_CIS;
extern int HPV_Status_ICC;
extern int HPV_Status_Recovered;
extern double hpv_date_after_death;
extern double no_hpv_infection;
extern double HPV_Screening_coverage;

// Directory parameters
string ParamDirectory;
extern string InputFileDirectory;

// Mortality parameters
double background_d;
double HIV_d;
double IHD_d;
double Depression_d;
double Asthma_d;
double Stroke_d;
double Diabetes_d;
double CKD_d;
double Colo_d;
double Liver_d;
double Oeso_d;
double Prostate_d;
double OtherCan_d;
extern double MortRisk[6];
extern double MortRisk_Cancer[5];
double MortAdj;

// NCD parameters
double Risk_DiabHC;
double Risk_DiabHT;
double Risk_DiabCKD;
double Risk_DiabCVD;
double Risk_HCHT;
double Risk_HCCVD;
double Risk_HTCKD;
double Risk_HTCVD;

double Risk_NCD_Diabetes[5];
double relatedNCDs_Diab[5];
int nr_NCD_Diab;

double Risk_NCD_HT[3];
int relatedNCDs_HT[3];
int nr_NCD_HT;

double Risk_NCD_HC[3];
int relatedNCDs_HC[3];
int nr_NCD_HC;


/////////////////// FUNCTION: IF LOOP FOR COUNTRY-SPECIFIC PARAMETERS //////////////////////
void loadCountryParams(int x){
    if (x == 1){                            // KENYA
        
        cout << "You have selected Kenya" << endl;
        
        // Central parameters
        UN_Pop=5909800;
        init_pop=UN_Pop/factor;
        total_population=init_pop;
        Sex_ratio=0.495639296;
        ageAdult=15;
        minAgeBirth=15;
        maxAgeBirth=50;
        
        // NCD parameters
        Risk_DiabHC=1.12;                                        // Having high cholesterol given diabtes etc ...
        Risk_DiabHT=1.4;
        Risk_DiabCKD=1.5;
        Risk_DiabCVD=2.31;
        Risk_HCHT=1.277;
        Risk_HCCVD=1.41;
        Risk_HTCKD=1.69;
        Risk_HTCVD=1.26;
        
        
        // Diabetes related NCDs
        int size_diabetes=5;
        double Temp1_diab[5]={Risk_DiabHT, Risk_DiabCVD, Risk_DiabCKD, Risk_DiabCVD, Risk_DiabHC};
        double Temp2_diab[5]={0, 3, 5, 6, 7};
        for (int i=0; i<size_diabetes; i++){
            Risk_NCD_Diabetes[i]=Temp1_diab[i];
            relatedNCDs_Diab[i]=Temp2_diab[i];
        }
        nr_NCD_Diab=sizeof(relatedNCDs_Diab)/sizeof(relatedNCDs_Diab[0]);
        
        // HT related NCDs
        int size_HT=3;
        double Temp1_HT[3]={Risk_HTCVD, Risk_HTCKD, Risk_HTCVD};
        double Temp2_HT[3]={3, 5, 6};
        for (int i=0; i<size_HT; i++){
            Risk_NCD_HT[i]=Temp1_HT[i];
            relatedNCDs_HT[i]=Temp2_HT[i];
        }
        nr_NCD_HT=sizeof(relatedNCDs_HT)/sizeof(relatedNCDs_HT[0]);
        
        // HC related NCDs
        int size_HC=3;
        double Temp1_HC[3]={Risk_HCHT, Risk_HCCVD, Risk_HCCVD};
        double Temp2_HC[3]={0, 3, 6};
        for (int i=0; i<size_HC; i++){
            Risk_NCD_HC[i]=Temp1_HC[i];
            relatedNCDs_HC[i]=Temp2_HC[i];
        }
        nr_NCD_HC=sizeof(relatedNCDs_HC)/sizeof(relatedNCDs_HC[0]);
        
        // HIV parameters
        ART_start_yr=2004;
        ARTbuffer=1.01;
        
        // HPV parameters
        age_atrisk_hpv=17;
        age_tostart_CCscreening=25;
        HPV_Prevalence=0.8;
        HPV_Screening_coverage=0.03;
        //CIN1_Rates[2]= {0.2,0.8};
        //CIN2_3_Rates[2]={0.4,0.6};
        //CIS_Rates[2]={0.65,0.35};
        //ICC_Rates[2]={1.0,0.0};
        no_hpv_infection=-988;
        hpv_date_after_death = -977;
        HPV_Status_HPV=1;
        HPV_Status_CIN1=2;
        HPV_Status_CIN2_3=3;
        HPV_Status_CIS=4;
        HPV_Status_ICC=5;
        HPV_Status_Recovered=6;
        
        // Mortality parameters
        MortAdj=1;
        background_d =71.32;                // Mortality percentages from GBD 2016
        HIV_d        =15.56;
        IHD_d         =3.99;
        Depression_d =0.0;
        Asthma_d     =0.45;
        Stroke_d     =3.92;
        Diabetes_d   =1.27;
        CKD_d        =1.5;
        Colo_d       =0.39;
        Liver_d      =0.34;
        Oeso_d       =0.32;
        Prostate_d    =0.31;
        OtherCan_d   =0.71;
    }
    
    else if (x == 2){                                          // ZIMBABWE
        
        cout << "You have selected Zimbabwe" << endl;
        
        // Central parameters
        UN_Pop=2565000;
        init_pop=UN_Pop/factor;
        total_population=init_pop;
        Sex_ratio=0.4986;
        ageAdult=15;
        minAgeBirth=15;
        maxAgeBirth=50;
        
        
        // NCD parameters
        Risk_DiabHC=1.12;
        Risk_DiabHT=1.4;
        Risk_DiabCKD=1.5;
        Risk_DiabCVD=2.31;
        Risk_HCHT=1.277;
        Risk_HCCVD=1.41;
        Risk_HTCKD=1.69;
        Risk_HTCVD=1.26;
        
        // Diabetes related NCDs
        int size_diabetes=5;
        double Temp1_diab[5]={Risk_DiabHT, Risk_DiabCVD, Risk_DiabCKD, Risk_DiabCVD, Risk_DiabHC};
        double Temp2_diab[5]={0, 3, 5, 6, 7};
        for (int i=0; i<size_diabetes; i++){
            Risk_NCD_Diabetes[i]=Temp1_diab[i];
            relatedNCDs_Diab[i]=Temp2_diab[i];
        }
        nr_NCD_Diab=sizeof(relatedNCDs_Diab)/sizeof(relatedNCDs_Diab[0]);
        
        // HT related NCDs
        int size_HT=3;
        double Temp1_HT[3]={Risk_HTCVD, Risk_HTCKD, Risk_HTCVD};
        double Temp2_HT[3]={3, 5, 6};
        for (int i=0; i<size_HT; i++){
            Risk_NCD_HT[i]=Temp1_HT[i];
            relatedNCDs_HT[i]=Temp2_HT[i];
        }
        nr_NCD_HT=sizeof(relatedNCDs_HT)/sizeof(relatedNCDs_HT[0]);
        
        // HC related NCDs
        int size_HC=3;
        double Temp1_HC[3]={Risk_HCHT, Risk_HCCVD, Risk_HCCVD};
        double Temp2_HC[3]={0, 3, 6};
        for (int i=0; i<size_HC; i++){
            Risk_NCD_HC[i]=Temp1_HC[i];
            relatedNCDs_HC[i]=Temp2_HC[i];
        }
        nr_NCD_HC=sizeof(relatedNCDs_HC)/sizeof(relatedNCDs_HC[0]);
        
        // HIV parameters
        ART_start_yr=2004;
        ARTbuffer=1.01;
        
        // HPV parameters
        age_atrisk_hpv=17;
        age_tostart_CCscreening=25;
        HPV_Prevalence=0.8;
        HPV_Screening_coverage=0.03;
        //CIN1_Rates[2]= {0.2,0.8};
        //CIN2_3_Rates[2]={0.4,0.6};
        //CIS_Rates[2]={0.65,0.35};
        //ICC_Rates[2]={1.0,0.0};
        no_hpv_infection=-988;
        hpv_date_after_death = -977;
        HPV_Status_HPV=1;
        HPV_Status_CIN1=2;
        HPV_Status_CIN2_3=3;
        HPV_Status_CIS=4;
        HPV_Status_ICC=5;
        HPV_Status_Recovered=6;
        
        // Mortality percentages from GBD 2013
        MortAdj=1;
        background_d =56.6;
        HIV_d        =29.6;
        IHD_d         =1.00;
        Depression_d =0.0;
        Asthma_d     =0.7;
        Stroke_d     =4.6;
        Diabetes_d   =1.7;
        CKD_d        =1.3;
        Colo_d       =0.3;
        Liver_d      =0.3;
        Oeso_d       =0.6;
        Prostate_d    =0.2;
        OtherCan_d   =3.1;
    }
    
    
    else if (x == 3){                                          // MALAWI need to replace params
        
        cout << "You have selected MALAWI" << endl;
        
        
        // Central Parameters
        UN_Pop=2565000;
        init_pop=UN_Pop/factor;
        total_population=init_pop;
        Sex_ratio=0.4986;
        ageAdult=15;
        minAgeBirth=15;
        maxAgeBirth=50;
        
        
        // NCD parameters
        Risk_DiabHC=1.12;
        Risk_DiabHT=1.4;
        Risk_DiabCKD=1.5;
        Risk_DiabCVD=2.31;
        Risk_HCHT=1.277;
        Risk_HCCVD=1.41;
        Risk_HTCKD=1.69;
        Risk_HTCVD=1.26;
        
        // Diabetes related NCDs
        int size_diabetes=5;
        double Temp1_diab[5]={Risk_DiabHT, Risk_DiabCVD, Risk_DiabCKD, Risk_DiabCVD, Risk_DiabHC};
        double Temp2_diab[5]={0, 3, 5, 6, 7};
        for (int i=0; i<size_diabetes; i++){
            Risk_NCD_Diabetes[i]=Temp1_diab[i];
            relatedNCDs_Diab[i]=Temp2_diab[i];
        }
        nr_NCD_Diab=sizeof(relatedNCDs_Diab)/sizeof(relatedNCDs_Diab[0]);
        
        // HT related NCDs
        int size_HT=3;
        double Temp1_HT[3]={Risk_HTCVD, Risk_HTCKD, Risk_HTCVD};
        double Temp2_HT[3]={3, 5, 6};
        for (int i=0; i<size_HT; i++){
            Risk_NCD_HT[i]=Temp1_HT[i];
            relatedNCDs_HT[i]=Temp2_HT[i];
        }
        nr_NCD_HT=sizeof(relatedNCDs_HT)/sizeof(relatedNCDs_HT[0]);
        
        // HC related NCDs
        int size_HC=3;
        double Temp1_HC[3]={Risk_HCHT, Risk_HCCVD, Risk_HCCVD};
        double Temp2_HC[3]={0, 3, 6};
        for (int i=0; i<size_HC; i++){
            Risk_NCD_HC[i]=Temp1_HC[i];
            relatedNCDs_HC[i]=Temp2_HC[i];
        }
        nr_NCD_HC=sizeof(relatedNCDs_HC)/sizeof(relatedNCDs_HC[0]);
        
        // HIV parameters
        ART_start_yr=2004;
        ARTbuffer=1.01;
        
        // HPV parameters
        age_atrisk_hpv=17;
        age_tostart_CCscreening=25;
        HPV_Prevalence=0.8;
        HPV_Screening_coverage=0.03;
        //CIN1_Rates[2]= {0.2,0.8};
        //CIN2_3_Rates[2]={0.4,0.6};
        //CIS_Rates[2]={0.65,0.35};
        //ICC_Rates[2]={1.0,0.0};
        no_hpv_infection=-988;
        hpv_date_after_death = -977;
        HPV_Status_HPV=1;
        HPV_Status_CIN1=2;
        HPV_Status_CIN2_3=3;
        HPV_Status_CIS=4;
        HPV_Status_ICC=5;
        HPV_Status_Recovered=6;
        
        // Mortality percentages from GBD 2013
        MortAdj=1;
        background_d =56.6;
        HIV_d        =29.6;
        IHD_d         =1.00;
        Depression_d =0.0;
        Asthma_d     =0.7;
        Stroke_d     =4.6;
        Diabetes_d   =1.7;
        CKD_d        =1.3;
        Colo_d       =0.3;
        Liver_d      =0.3;
        Oeso_d       =0.6;
        Prostate_d    =0.2;
        OtherCan_d   =3.1;
    }
    
    else if (x == 4){                                          // Uasin Gishu - KENYA
        
        cout << "You have selected Kenya - Uasin Gishu" << endl;
        
        // Central Parameters
        UN_Pop=99075;
        init_pop=UN_Pop/factor;
        total_population=init_pop;
        Sex_ratio=0.50237783022;
        ageAdult=15;
        minAgeBirth=15;
        maxAgeBirth=50;
        
        
        // NCD parameters
        Risk_DiabHC=1.12;                                        // Having high cholesterol given diabtes etc ...
        Risk_DiabHT=1.4;
        Risk_DiabCKD=1.5;
        Risk_DiabCVD=2.31;
        Risk_HCHT=1.277;
        Risk_HCCVD=1.41;
        Risk_HTCKD=1.69;
        Risk_HTCVD=1.26;
        
        // Diabetes related NCDs
        int size_diabetes=5;
        double Temp1_diab[5]={Risk_DiabHT, Risk_DiabCVD, Risk_DiabCKD, Risk_DiabCVD, Risk_DiabHC};
        double Temp2_diab[5]={0, 3, 5, 6, 7};
        for (int i=0; i<size_diabetes; i++){
            Risk_NCD_Diabetes[i]=Temp1_diab[i];
            relatedNCDs_Diab[i]=Temp2_diab[i];
        }
        nr_NCD_Diab=sizeof(relatedNCDs_Diab)/sizeof(relatedNCDs_Diab[0]);
        
        // HT related NCDs
        int size_HT=3;
        double Temp1_HT[3]={Risk_HTCVD, Risk_HTCKD, Risk_HTCVD};
        double Temp2_HT[3]={3, 5, 6};
        for (int i=0; i<size_HT; i++){
            Risk_NCD_HT[i]=Temp1_HT[i];
            relatedNCDs_HT[i]=Temp2_HT[i];
        }
        nr_NCD_HT=sizeof(relatedNCDs_HT)/sizeof(relatedNCDs_HT[0]);
        
        // HC related NCDs
        int size_HC=3;
        double Temp1_HC[3]={Risk_HCHT, Risk_HCCVD, Risk_HCCVD};
        double Temp2_HC[3]={0, 3, 6};
        for (int i=0; i<size_HC; i++){
            Risk_NCD_HC[i]=Temp1_HC[i];
            relatedNCDs_HC[i]=Temp2_HC[i];
        }
        nr_NCD_HC=sizeof(relatedNCDs_HC)/sizeof(relatedNCDs_HC[0]);
        
        // HIV parameters
        ART_start_yr=2004;
        ARTbuffer=1.01;
        
        // HPV parameters
        age_atrisk_hpv=17;
        age_tostart_CCscreening=25;
        HPV_Prevalence=0.8;
        HPV_Screening_coverage=0.03;
        //CIN1_Rates[2]= {0.2,0.8};
        //CIN2_3_Rates[2]={0.4,0.6};
        //CIS_Rates[2]={0.65,0.35};
        //ICC_Rates[2]={1.0,0.0};
        no_hpv_infection=-988;
        hpv_date_after_death = -977;
        HPV_Status_HPV=1;
        HPV_Status_CIN1=2;
        HPV_Status_CIN2_3=3;
        HPV_Status_CIS=4;
        HPV_Status_ICC=5;
        HPV_Status_Recovered=6;
        
        // Mortality parameters
        MortAdj=1;
        background_d =71.32;
        HIV_d        =15.56;
        IHD_d         =3.99;
        Depression_d =0.0;
        Asthma_d     =0.45;
        Stroke_d     =3.92;
        Diabetes_d   =1.27;
        CKD_d        =1.5;
        Colo_d       =0.39;
        Liver_d      =0.34;
        Oeso_d       =0.32;
        Prostate_d    =0.31;
        OtherCan_d   =0.71;
    }
}


void getParamsString(int x){
    if (x == 1){
        ParamDirectory=InputFileDirectory + "/Kenya/";
    }
    
    else if (x == 2){
        ParamDirectory=InputFileDirectory + "/Zimbabwe/";
    }
    else if (x == 3){
        ParamDirectory=InputFileDirectory + "/Malawi/";
    }
    
    else if (x == 4){
        ParamDirectory=InputFileDirectory + "/Malawi/";
    }
    
}

