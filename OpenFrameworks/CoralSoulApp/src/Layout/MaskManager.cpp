/*
 *  MaskManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 04/11/17.
 *
 */


#include "ofMain.h"

#include "AppManager.h"
#include "MaskManager.h"



MaskManager::MaskManager(): Manager(), m_showMasks(false)
{
    //Intentionally left empty
}


MaskManager::~MaskManager()
{
    ofLogNotice() <<"MaskManager::Destructor";
}


void MaskManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupShader();
    ofLogNotice() <<"MaskManager::initialized";
    
}


void MaskManager::setupShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_maskShader.load("shaders/shadersGL3/BlackMask");
    }else{
        m_maskShader.load("shaders/shadersGL2/BlackMask");
    }
}


void MaskManager::allocate(string& maskName, int width, int height)
{
    
    shared_ptr<ofFbo> mask =  shared_ptr<ofFbo>(new ofFbo());
    mask->allocate(width, height, GL_RGBA);
    mask->begin();  ofClear(0); mask->end();
    
    m_masks[maskName] = mask;
    
    shared_ptr<ofFbo> maskee =  shared_ptr<ofFbo>(new ofFbo());
    maskee->allocate(width, height, GL_RGBA);
    maskee->begin();  ofClear(0); maskee->end();
    
    m_masksees[maskName] = maskee;
    
    ofLogNotice() <<"MaskManager::allocate -> New Mask: " << maskName << ", w = " << width << ", height = " << height ;
}

void MaskManager::begin(string& maskName)
{
    if(m_masksees.find(maskName) == m_masksees.end()){
        return;
    }
    
    if(m_masks.find(maskName) == m_masks.end()){
        return;
    }
    
    //ofEnableAlphaBlending();
    m_masksees[maskName]->begin();
    m_maskShader.begin();
    m_maskShader.setUniformTexture("imageMask", m_masks[maskName]->getTexture(), 1);
    
    ofClear(0);
    
}

void MaskManager::end(string& maskName)
{
    if(m_masksees.find(maskName) == m_masksees.end()){
        return;
    }
    
    m_maskShader.end();
    m_masksees[maskName]->end();
    
}


void MaskManager::beginMask(string& maskName)
{
    if(m_masks.find(maskName) == m_masks.end()){
        return;
    }
    
    //ofEnableAlphaBlending();
    m_masks[maskName]->begin();
    ofClear(0);
}

void MaskManager::endMask(string& maskName)
{
    if(m_masks.find(maskName) == m_masks.end()){
        return;
    }
    
    m_masks[maskName]->end();
    // ofDisableAlphaBlending();
}


void MaskManager::draw(string& maskName)
{
    if(m_masksees.find(maskName) == m_masksees.end()){
        return;
    }
    
    m_masksees[maskName]->draw(0,0);
}

void MaskManager::drawMask(string& maskName)
{
    if(m_masks.find(maskName) == m_masks.end()){
        return;
    }
    
    m_masks[maskName]->draw(0,0);
}




void MaskManager::update()
{
    //Empty
}




