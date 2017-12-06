/*
 *  VectorFieldParticle.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */

#include "AppManager.h"

#include "VectorFieldParticle.h"

VectorFieldParticle::VectorFieldParticle(): m_maxSpeed(2)
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


void VectorFieldParticle::addForce(const ofVec2f& dir)
{
    m_acc += dir;
}


void VectorFieldParticle::update()
{
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_vel+=m_acc;
    m_vel.limit(m_maxSpeed);
    m_prevPos = m_pos;
    m_pos+=m_vel;
    m_acc = ofVec2f(0);
    
    float adj = ofMap(m_pos.y, 0, height, 255, 0);
    m_color = ofColor(40, adj, 255);
    
    if(this->isOffScreen()){
        this->stayOnScreen();
        m_prevPos = m_pos;
    }
}

void VectorFieldParticle::draw(){
    ofPushStyle();
        ofSetColor(m_color);
        ofSetLineWidth(6.0);
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
    if( m_pos.y >= height ) m_pos.y -= height;
}

bool VectorFieldParticle::isOffScreen(){
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    if( m_pos.x < 0 || m_pos.x >= width|| m_pos.y < 0 || m_pos.y >= height ){
        
        return true;
    }
    
    return false;
}

