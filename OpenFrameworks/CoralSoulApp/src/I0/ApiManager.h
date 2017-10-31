/*
 *  ApiManager.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */

#pragma once

#include "ofMain.h"
#include "Manager.h"


//========================== class ApiManager =======================================
//===================================================================================
/** \class ApiManager ApiManager.h
 *	\brief class for managing several APIs
 *	\details It reads from weather APIs, tide APIs, parses the informations and provides the
 *    methods to read it
 */


class ApiManager: public Manager
{
    
public:
    //! Constructor
    ApiManager();

    //! Destructor
    virtual ~ApiManager();

    //! setups the manager
    void setup();

    //! updates the manager
    void update();
    
    
private:
    
   


 private:
    

};

