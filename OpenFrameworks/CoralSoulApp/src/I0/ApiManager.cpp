/*
 *  ApiManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */


#include "ApiManager.h"
#include "AppManager.h"


ApiManager::ApiManager(): Manager()
{
    //Intentionally left empty
}

ApiManager::~ApiManager()
{
   ofLogNotice() << "ApiManager::destructor";
}


//--------------------------------------------------------------

void ApiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    ofLogNotice() <<"ApiManager::initialized" ;
}




void ApiManager::update()
{
 
}









