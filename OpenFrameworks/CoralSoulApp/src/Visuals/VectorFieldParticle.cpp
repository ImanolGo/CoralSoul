/*
 *  VectorFieldParticle.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */

#include "AppManager.h"

#include "VectorFieldParticle.h"

VectorFieldParticle::VectorFieldParticle(): m_maxSpeed(4)
{
    this->setup();
}


VectorFieldParticle::~VectorFieldParticle()
{
    //Intentionaly left empty
}


void VectorFieldParticle::setup(){
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_pos.x = ofRandom(0,width);
    m_pos.y = ofRandom(0,height);
    m_prevPos = m_pos;
}


void VectorFieldParticle::addForce(ofVec2f dir)
{
    m_acc += dir;
}


void VectorFieldParticle::update()
{
    m_vel+=m_acc;
    m_vel.limit(m_maxSpeed);
    m_prevPos = m_pos;
    m_pos+=m_vel;
    m_acc = ofVec2f(0);
    
    if(this->isOffScreen()){
        this->stayOnScreen();
        m_prevPos = m_pos;
    }
}

void VectorFieldParticle::draw(){
    ofPushStyle();
        ofSetColor(m_color);
        ofSetLineWidth(2.0);
        ofDrawLine(m_prevPos,m_pos);
    ofPopStyle();
}

void VectorFieldParticle::stayOnScreen()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    if( m_pos.x < 0 ) m_pos.x += width;
    if( m_pos.x >= width) m_pos.x -= width;
    if( m_pos.y < 0 ) m_pos.y += height;
    if( m_pos.y >= ofGetHeight() ) m_pos.y -= height;
}

bool VectorFieldParticle::isOffScreen(){
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    if( m_pos.x < 0 || m_pos.x >= width|| m_pos.y < 0 || m_pos.y >= height ){
        
        return true;
    }
    
    return false;
}

