/*
 *  VectorFieldParticle.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */

#pragma once

#include "ofMain.h"

//======================== class VectorFieldParticle =========================
//============================================================================
/** \class VectorFieldParticle VectorFieldParticle.h
 *    \brief Class definding a vector field particle
 *    \details It describes the behaviour and rendering of a vector field particle
 */

class VectorFieldParticle
{
public:
    
    //! Constructor
    VectorFieldParticle();
    
    //! Destructor
    ~VectorFieldParticle();
    
    //! Update the Vector Field Particle
    void update();
    
    //! Draw the Vector Field Particle
    void draw();
    
    void addForce(ofVec2f dir);
    
    void setMaxSpeed(float speed) {m_maxSpeed = speed;}
    
    ofVec2f getPos() const {return m_pos;}
    
private:
    
    //! Setup the Vector Field Particle
    void setup();
    
    bool isOffScreen();
    
    void stayOnScreen();
    
private:
    
    ofVec2f     m_pos;
    ofVec2f     m_prevPos;
    ofVec2f     m_vel;
    ofVec2f     m_acc;
    float       m_maxSpeed;
    
    ofColor     m_color;
    
    
    int duration;
    
};