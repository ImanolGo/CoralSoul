/*
 *  WireFrameScene.cpp
 *  Primavera Sound Stage
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */


#include "WireFrameScene.h"
#include "AppManager.h"

WireFrameScene::WireFrameScene(): ofxScene("WIREFRAME")
{
    //Intentionally left empty
}

WireFrameScene::~WireFrameScene()
{
    //Intentionally left empty
}


void WireFrameScene::setup() {
    ofLogNotice(getName() + "::setup");
}



void WireFrameScene::update()
{
    this->updateNoise();
}


void WireFrameScene::updateNoise()
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


void WireFrameScene::draw()
{
    ofClear(0);
    
    auto tex = AppManager::getInstance().getResourceManager().getTexture("Coral");
    AppManager::getInstance().getModelManager().drawWireframe(*tex.get());
}



void WireFrameScene::willFadeIn() {
    ofLogNotice("WireFrameScene::willFadeIn");
    
    float amp = 0.2;
    float speed = 0.2;
    ofColor coral(255,127,80);

    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
    AppManager::getInstance().getModelManager().onWireFrameColorChange(coral);
    
}

void WireFrameScene::willDraw() {
    ofLogNotice("WireFrameScene::willDraw");
}

void WireFrameScene::willFadeOut() {
    ofLogNotice("WireFrameScene::willFadeOut");
}

void WireFrameScene::willExit() {
    ofLogNotice("WireFrameScene::willExit");
    
    float amp = 0.0;
    float speed = 0.0;
    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
}

