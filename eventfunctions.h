//////////////////////////////////////////////////////////////
//    eventfunction.h                                       //
//    Created by Mikaela Smit on 07/11/2014.                //
//    Copyright (c) 2014 Mikaela Smit. All rights reserved. //
//    These event are added to the EventQ.                  //
//////////////////////////////////////////////////////////////

#include <stdio.h> 
#include "person.h"

using namespace std;


//// ---- NOTE: Think about having different types of events ----

	//// --- Demographic Events ---
	void EventMyDeathDate(person *MyPointerToPerson);			// Function to be executed upon death
	void EventBirth(person *MyPointerToPerson);					// Function to be executed when someone gives birth
	
	//// --- Calendar Events ---
	void EventTellNewYear(person *MyPointerToPerson);			// Tell us when a new year start an update calendar-related items
	
	
    //// --- Cervical cancer interventions ---
    void EventMyFirst_VIA_Screening(person *MyPointerToPerson);
    void EventMy_VIA_FollowUp(person *MyPointerToPerson);
    void EventMyFirst_Pap_Screening(person *MyPointerToPerson);
    void EventMyCIN1_FollowUp(person *MyPointerToPerson);
    void EventMyCIN2_Cryo_Intervention(person *MyPointerToPerson);
    void EventMyCIN2_LEEP_Intervention(person *MyPointerToPerson);

   
  
