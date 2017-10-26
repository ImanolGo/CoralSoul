/*
 *  ModelManager.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#pragma once

#include "Manager.h"
#include "ofxAssimpModelLoader.h"

//========================== class ModelManager ==============================
//============================================================================
/** \class ModelManager ModelManager.h
 *	\brief Class managing the 3D models
 *	\details It reads and shows the model with its textures
 */

class ModelManager: public Manager
{
public:
    //! Constructor
    ModelManager();

    //! Destructor
    ~ModelManager();

    //! Set-up the layout
    void setup();
    
    //! Update the layout
    void update();
    
    //! Draw the layout
    void draw();


private:


    

private:


};

//==========================================================================


