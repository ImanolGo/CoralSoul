/*
 *  RainScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 05/11/17.
 *
 */


#include "RainScene.h"
#include "AppManager.h"

RainScene::RainScene(): ofxScene("RAIN")
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
    this->setupFbo();
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

void RainScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void RainScene::update()
{
    this->updateRipples();
    this->updateFbo();
}

void RainScene::updateRipples()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
   // auto tex =  AppManager::getInstance().getModelManager().getModel().getTexture();
    
    //m_bounce.setTexture(tex, 1);
    float precMM = AppManager::getInstance().getApiManager().getCurrentWeather().precipitationValue;
    int skip = (int) ofMap(precMM, 0.0, 10.0, 10, 1, true);
    
    m_bounce.setTexture(*m_texture.get(), 1);
    
    if(precMM > 0 && ofGetFrameNum()%skip == 0){
        m_ripples.begin();
        
        ofFill();
        ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
        int dropsPerCycle = 2;
        for(int i=0; i<dropsPerCycle; i++){
             ofDrawEllipse(ofRandom(width),ofRandom(height), 10,10);
        }
       
        
       
        m_ripples.end();
    }
    
   
   
    m_ripples.update();
    
    m_bounce << m_ripples;
}

void RainScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawRipples();
    m_fbo.end();
}

void RainScene::draw()
{
    ofClear(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
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
    
}

void RainScene::willDraw() {
    ofLogNotice("RainScene::willDraw");
}

void RainScene::willFadeOut() {
    ofLogNotice("RainScene::willFadeOut");
}

void RainScene::willExit() {
}

