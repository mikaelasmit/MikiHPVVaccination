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


extern int UN_Pop;
extern int init_pop;
extern int total_population;
extern int ART_start_yr;
extern int factor;
extern int ageAdult;
extern int age_atrisk_hpv;
extern int age_tostart_CCscreening;
extern double Sex_ratio;

extern string ParamDirectory1;
extern string ParamDirectory2;
extern string ParamDirectory3;
extern string ParamDirectory4;

extern double HPV_Prevalence;
//extern double CIN1_Rates[2];
//extern double CIN2_3_Rates[2];
//extern double CIS_Rates[2];
//extern double ICC_Rates[2];
extern int HPV_Status_HPV;
extern int HPV_Status_CIN1;
extern int HPV_Status_CIN2_3;
extern int HPV_Status_CIS;
extern int HPV_Status_ICC;
extern int HPV_Status_Recovered;
extern double hpv_date_after_death;
extern double no_hpv_infection;
extern double HPV_Screening_coverage;

extern double ARTbuffer;
extern double MortAdj;
extern double background_d;
extern double HIV_d;
extern double IHD_d;
extern double Depression_d;
extern double Asthma_d;
extern double Stroke_d;
extern double Diabetes_d;
extern double CKD_d;
extern double Colo_d;
extern double Liver_d;
extern double Oeso_d;
extern double Prostate_d;
extern double OtherCan_d;
extern string ParamDirectory;

extern double MortRisk[6];                      // Mortality related
extern double MortRisk_Cancer[5];
extern double MortAdj;

extern double Risk_DiabHC;                      // NCD common causal pathways
extern double Risk_DiabHT;
extern double Risk_DiabCKD;
extern double Risk_DiabCVD;
extern double Risk_HCHT;
extern double Risk_HCCVD;
extern double Risk_HTCKD;
extern double Risk_HTCVD;


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
        
        // NCD parameters
        Risk_DiabHC=1.12;                                        // Having high cholesterol given diabtes etc ...
        Risk_DiabHT=1.4;
        Risk_DiabCKD=1.5;
        Risk_DiabCVD=2.31;
        
        Risk_HCHT=1.277;
        Risk_HCCVD=1.41;
        
        Risk_HTCKD=1.69;
        Risk_HTCVD=1.26;
        
        double Risk_NCD_Diabetes[5]={Risk_DiabHT, Risk_DiabCVD, Risk_DiabCKD, Risk_DiabCVD, Risk_DiabHC};
        int relatedNCDs_Diab[5]={0, 3, 5, 6, 7};
        int nr_NCD_Diab=sizeof(relatedNCDs_Diab)/sizeof(relatedNCDs_Diab[0]);
        
        
        double Risk_NCD_HT[3]={Risk_HTCVD, Risk_HTCKD, Risk_HTCVD};
        int relatedNCDs_HT[3]={3, 5, 6};
        int nr_NCD_HT=sizeof(relatedNCDs_HT)/sizeof(relatedNCDs_HT[0]);
        
        double Risk_NCD_HC[3]={Risk_HCHT, Risk_HCCVD, Risk_HCCVD};
        int relatedNCDs_HC[3]={0, 3, 6};
        int nr_NCD_HC=sizeof(relatedNCDs_HC)/sizeof(relatedNCDs_HC[0]);

        
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
        
        double MortRisk[6]= {0, 0, 0.85, 1.3, 1.1, 0.8}; //{0.087, 0, 1.4, 670.87, 12.23, 5};         // Original values from Smith et al Factors associated with : 1.52 (HT), 1.77 (diabetes)
        double MortRisk_Cancer[5]= {1, 1, 1, 1, 1.05};                   //{0.087, 0, 1.4, 670.87, 12.23};   // Both this and above needs to be fitted
    }
    
    else if (x == 2){                                          // ZIMBABWE
        cout << "You have selected Zimbabwe" << endl;
        UN_Pop=2565000;
        init_pop=UN_Pop/factor;
        total_population=init_pop;
        ART_start_yr=2004;
        ageAdult=15;
        age_atrisk_hpv=17;
        Sex_ratio=0.4986;
        ARTbuffer=1;
        MortAdj=1;
        // Mortality percentages from GBD 2013
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
        UN_Pop=2565000;
        init_pop=UN_Pop/factor;
        total_population=init_pop;
        ART_start_yr=2004;
        ageAdult=15;
        age_atrisk_hpv=17;
        Sex_ratio=0.4986;
        ARTbuffer=1;
        MortAdj=1;
        // Mortality percentages from GBD 2013
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
        UN_Pop=99075;  //99075
        init_pop=UN_Pop/factor;
        total_population=init_pop;
        ART_start_yr=2004;
        ageAdult=15;
        age_atrisk_hpv=17;
        Sex_ratio=0.50237783022;
        HPV_Prevalence=0.388; //normally 0.388
        //CIN1_Rates[2]={0.2,0.8};
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
        ARTbuffer=1.01;
        MortAdj=1;
        // Mortality percentages from GBD 2016
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
        ParamDirectory=ParamDirectory1;
    }
    
    else if (x == 2){
        ParamDirectory=ParamDirectory2;
    }
    else if (x == 3){
        ParamDirectory=ParamDirectory3;
    }
    
    else if (x == 4){
        ParamDirectory=ParamDirectory4;
    }
    
    
}

