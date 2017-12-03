/*
 *  VectorFieldVisual.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */


#pragma once

#include "ofxVectorField.h"
#include "VectorFieldParticle.h"


//========================= class VectorFieldVisual ==========================
//============================================================================
/** \class VectorFieldVisual VectorFieldVisual.h
 *	\brief Class managing the vector field visuals
 *	\details It creates and manages all the parameters and vector field like visualizations
 */


class VectorFieldVisual
{
    static const int NUM_PARTICLES;

    
public:
    
    //! Constructor
    VectorFieldVisual();
    
    //! Destructor
    ~VectorFieldVisual();
    
    //! Setup the Vector Field Visual
    void setup();
    
    //! Update the Vector Field Visual
    void update();
    
    //! Draw the Vector Field Visual
    void draw();
    
   
private:
    
    void setupVectorField();
    
    void setupParticles();
    
    void updateVectorField();
    
    void updateParticles();
    
    void drawVectorField();
    
    void drawParticles();
    
    
private:
    
    ofxVectorField                 m_vectorField;
    vector <VectorFieldParticle>   m_particles;
    
    float       m_spacing;
    float       m_speed;
    
    
};



