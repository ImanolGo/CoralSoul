/*
 *  RainScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 05/11/17.
 *
 */


#include "RainScene.h"
#include "AppManager.h"

RainScene::RainScene(): ofxScene("WATER")
{
    //Intentionally left empty
}

RainScene::~RainScene()
{
    //Intentionally left empty
}


void RainScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupImage();
    this->setupRipples();
}

void RainScene::setupImage()
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture(getName());
}

void RainScene::setupRipples()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    //m_texture = AppManager::getInstance().getResourceManager().getTexture(getName());
    
    //auto tex =  AppManager::getInstance().getModelManager().getModel().getTexture();
    
   // m_ripples.allocate(width, height);
   // m_bounce.allocate(width, height);
    //m_bounce.setTexture(tex, 1);
    
    m_ripples.allocate(m_texture->getWidth(), m_texture->getHeight());
    m_bounce.allocate(m_texture->getWidth(),  m_texture->getHeight());
    m_bounce.setTexture(*m_texture.get(), 1);
   
}


void RainScene::update()
{
    this->updateRipples();
}

void RainScene::updateRipples()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
   // auto tex =  AppManager::getInstance().getModelManager().getModel().getTexture();
    
    //m_bounce.setTexture(tex, 1);
    
    m_bounce.setTexture(*m_texture.get(), 1);
    
    m_ripples.begin();
    ofFill();
        ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
        ofEllipse(ofRandom(width),ofRandom(height), 10,10);
    m_ripples.end();
    m_ripples.update();
    
    m_bounce << m_ripples;
}


void RainScene::draw()
{
    ofClear(0);
    this->drawRipples();
    
   
}

void RainScene::drawRipples()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();

     m_bounce.draw(0,0, width, height);
    //m_ripples.draw(0,0, width, height);
    
    // m_texture->draw(0,0, width, height);
}

void RainScene::willFadeIn() {
    ofLogNotice("RainScene::willFadeIn");
    //AppManager::getInstance().getModelManager().onLightColorChange(ofColor(0, 191, 255));
    
}

void RainScene::willDraw() {
    ofLogNotice("RainScene::willDraw");
}

void RainScene::willFadeOut() {
    ofLogNotice("RainScene::willFadeOut");
}

void RainScene::willExit() {
}

