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
    m_post.init(width, height);
    m_post.createPass<FxaaPass>()->setEnabled(true);
    m_post.createPass<BloomPass>()->setEnabled(false);
}



void SeaScene::update()
{
    this->updateNoise();
}


void SeaScene::updateNoise()
{
    auto weatherConditions = AppManager::getInstance().getApiManager().getCurrentWeather();
    float speed = 0.2;
    float f = 1.0/weatherConditions.swellPeriod;
    float sine = sin(TWO_PI*f*ofGetElapsedTimef());
    float amp = ofMap(sine, -1, 1, 0.07,  weatherConditions.swellHeight/9);
    //float speed = ofMap(sine, -1, 1, 0.1,  0.2);
    
    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
}


void SeaScene::draw()
{
    ofClear(0);
    
    // m_post.begin();
    
    auto tex = AppManager::getInstance().getResourceManager().getTexture("Coral");
    AppManager::getInstance().getModelManager().drawWireframe(*tex.get());
    
    //m_post.end();
}



void SeaScene::willFadeIn() {
    ofLogNotice("SeaScene::willFadeIn");
    
    float amp = 0.2;
    float speed = 0.2;
    ofColor coral(255,127,80);

    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
    AppManager::getInstance().getModelManager().onWireFrameColorChange(coral);
    
}

void SeaScene::willDraw() {
    ofLogNotice("SeaScene::willDraw");
}

void SeaScene::willFadeOut() {
    ofLogNotice("SeaScene::willFadeOut");
}

void SeaScene::willExit() {
    ofLogNotice("SeaScene::willExit");
    
    float amp = 0.0;
    float speed = 0.0;
    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
}

