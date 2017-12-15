/*
 *  SeaScene.cpp
 *  Primavera Sound Stage
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */


#include "SeaScene.h"
#include "AppManager.h"

SeaScene::SeaScene(): ofxScene("SEA")
{
    //Intentionally left empty
}

SeaScene::~SeaScene()
{
    //Intentionally left empty
}


void SeaScene::setup()
{
    ofLogNotice(getName() + "::setup");
    this->setupPostProcessing();
}


void SeaScene::setupPostProcessing()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    // Setup post-processing chain
   // m_post.init(width, height);
   // m_post.createPass<FxaaPass>()->setEnabled(true);
    //m_post.createPass<BloomPass>()->setEnabled(false);
}



void SeaScene::update()
{
    this->updateNoise();
}


void SeaScene::updateNoise()
{
    auto weatherConditions = AppManager::getInstance().getApiManager().getCurrentWeather();
    float speed = 0.2;
    float f = 1.0/weatherConditions.m_swellPeriod;
    float sine = sin(TWO_PI*f*ofGetElapsedTimef());
    float amp = ofMap(sine, -1, 1, 0.07,  weatherConditions.m_swellHeight/9);
    //float speed = ofMap(sine, -1, 1, 0.1,  0.2);
    
    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
}


void SeaScene::draw()
{
    ofClear(0);
	ofBackground(0);
    
    // m_post.begin();
    
	ofPushStyle();
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    auto tex = AppManager::getInstance().getResourceManager().getTexture("SEA");
	ofSetColor(128);
    tex->draw(0,0,width, height);
    //AppManager::getInstance().getModelManager().drawWireframe(*tex.get());
	ofPopStyle();
    
    //m_post.end();
}



void SeaScene::willFadeIn() {
    ofLogNotice("SeaScene::willFadeIn");
    
//    float amp = 0.2;
//    float speed = 0.2;
//    ofColor coral(255,127,80);
//
//    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
//    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
//    AppManager::getInstance().getModelManager().onWireFrameColorChange(coral);
    
    AppManager::getInstance().getLayoutManager().onMaskChange(false);
    this->activateWaves();
}

void SeaScene::willDraw() {
    ofLogNotice("SeaScene::willDraw");
}

void SeaScene::willFadeOut() {
    ofLogNotice("SeaScene::willFadeOut");
}

void SeaScene::willExit()
{
    ofLogNotice("SeaScene::willExit");
    
     AppManager::getInstance().getLayoutManager().onMaskChange(true);
     this->deactivateWaves();
    
//    float amp = 0.0;
//    float speed = 0.0;
//    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
//    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
}


void SeaScene::activateWaves()
{
    string address = "/layer3/video/effect3/bypassed";
    int value = 0;
    
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(value);
    
    AppManager::getInstance().getOscManager().sendMessage(m);
    
    address = "/layer3/video/effect3/param4/values";
    
    auto fValue = AppManager::getInstance().getApiManager().getCurrentWeather().getSwellHeightNorm();
    fValue = ofMap(fValue, 0.0, 10.0, 0.02, 0.18, true);
    
    m.setAddress(address);
    m.addFloatArg(fValue);
    AppManager::getInstance().getOscManager().sendMessage(m);
    
    
    fValue = AppManager::getInstance().getApiManager().getCurrentWeather().getSwellPeriodNorm();
    
	address = "/layer3/video/effect3/param5/values";
    m.setAddress(address);
    m.addFloatArg(fValue);
    AppManager::getInstance().getOscManager().sendMessage(m);
    
}

void SeaScene::deactivateWaves()
{
    string address = "/layer3/video/effect3/bypassed";
    int value = 1;
    
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(value);
    
    AppManager::getInstance().getOscManager().sendMessage(m);
}


