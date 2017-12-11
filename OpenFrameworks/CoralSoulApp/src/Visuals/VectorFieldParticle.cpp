/*
 *  VectorFieldParticle.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */

#include "AppManager.h"

#include "VectorFieldParticle.h"

VectorFieldParticle::VectorFieldParticle(): m_maxSpeed(2), height(8)
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
    
    this->setupBrush();
}

void VectorFieldParticle::setupBrush()
{
    m_brush.setResource("Brush");
    m_brush.setWidth(20,true);
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
    m_pos+= (m_vel + ofVec2f(0.5*ofRandomf(),0.5*ofRandomf()));
    m_acc = ofVec2f(0);
    
    float adj = ofMap(m_pos.y, 0, height, 255, 0);
    m_color = ofColor(40, adj, 255);
    
    if(this->isOffScreen()){
        this->stayOnScreen();
        m_prevPos = m_pos;
    }
    
    m_brush.setPosition(m_pos);
    m_brush.setColor(m_color);
}

void VectorFieldParticle::draw(){
    
    ofPushMatrix();
    ofPushStyle();
        ofSetColor(m_color);
        //ofScale(0.5, 0.5);
        ofSetLineWidth(height);
    //m_brush.draw();
        ofDrawLine(m_prevPos,m_pos);
        //ofDrawRectRounded(-width*0.5,-height*0.5,m_fbo.getWidth(),height, 0.1);
    ofPopStyle();
    ofPopMatrix();
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

