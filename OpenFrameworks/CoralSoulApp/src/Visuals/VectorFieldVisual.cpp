/*
 *  VectorFieldVisual.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */


#include "AppManager.h"
#include "VectorFieldVisual.h"

const int VectorFieldVisual::NUM_PARTICLES = 1000;


VectorFieldVisual::VectorFieldVisual():m_speed(0.01), m_spacing(20), m_skipFrames(0), m_fadeTime(10)
{
    //Intentionaly left empty
}


VectorFieldVisual::~VectorFieldVisual()
{
    //Intentionaly left empty
}


void VectorFieldVisual::setup()
{
    this->setupFbo();
    this->setupVectorField();
    this->setupParticles();
}


void VectorFieldVisual::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
   
    m_fbo.allocate(width,height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void VectorFieldVisual::setupVectorField()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_vectorField.setup(width,height, m_spacing);
    
    // create the vector field using perlin noise
    m_vectorField.randomize();
    
    // adjust the vector field by normalizing, scaling, biasing & blurring (to make it look nice)
    m_vectorField.normalize();
    m_vectorField.scale(20);
    m_vectorField.bias(1, 0);
    m_vectorField.blur();
}

void VectorFieldVisual::setupParticles()
{
    for( int i=0; i<NUM_PARTICLES; i++)
    {
        m_particles.push_back(VectorFieldParticle());
    }
}

void VectorFieldVisual::update()
{
    this->updateVectorField();
    this->updateParticles();
    this->updateFbo();
}

void VectorFieldVisual::updateVectorField()
{
    m_vectorField.animate(m_speed);
}

void VectorFieldVisual::updateParticles()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    for( int i=0; i<m_particles.size(); i++)
    {
        auto force = m_vectorField.getVectorInterpolated(m_particles[i].getPos().x, m_particles[i].getPos().y, width, height);
        m_particles[i].addForce(force);
        m_particles[i].update();
    }
}

void VectorFieldVisual::updateFbo()
{
    float decrease = 3.0;
    float framesToDie = 255.0/decrease;
    float dt = ofGetLastFrameTime();
    int numSkipFrames = m_fadeTime/(framesToDie*dt);
    m_skipFrames++;
    
    ofEnableAlphaBlending();
    m_fbo.begin();
    ofPushStyle();
    if(m_skipFrames>=numSkipFrames){
        ofSetColor(0,0,0,decrease);
        ofDrawRectangle(0,0,m_fbo.getWidth(),m_fbo.getHeight());
        m_skipFrames = 0;
    }
    
    ofSetColor(255);
    
        this->drawParticles();
    
    ofPopStyle();
    m_fbo.end();
    ofDisableAlphaBlending();
}

void VectorFieldVisual::draw()
{
//    this->drawVectorField();
//    this->drawParticles();
    
    m_fbo.draw(0,0);
}


void VectorFieldVisual::drawVectorField()
{
    m_vectorField.draw();
}

void VectorFieldVisual::drawParticles()
{
    for( int i=0; i<m_particles.size(); i++){
        m_particles[i].draw();
    }
}



void VectorFieldVisual::addForce(const ofVec2f& force)
{
    for( int i=0; i<m_particles.size(); i++){
        m_particles[i].addForce(force);
    }
}

void VectorFieldVisual::setSpeed(float value)
{
    for( int i=0; i<m_particles.size(); i++){
        m_particles[i].setMaxSpeed(value);
    }
}
