/*
 *  VectorFieldVisual.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */


#include "AppManager.h"
#include "VectorFieldVisual.h"

const int VectorFieldVisual::NUM_PARTICLES = 500;


VectorFieldVisual::VectorFieldVisual():m_speed(0.02), m_spacing(10)
{
    //Intentionaly left empty
}


VectorFieldVisual::~VectorFieldVisual()
{
    //Intentionaly left empty
}


void VectorFieldVisual::setup()
{
    this->setupVectorField();
    this->setupParticles();
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

void VectorFieldVisual::draw()
{
    this->drawVectorField();
    this->drawParticles();
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


