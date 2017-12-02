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
    //Intentionally left empty
}


void WireFrameScene::draw()
{
    ofClear(0);
    AppManager::getInstance().getModelManager().getWireframe().draw(0,0);
}



void WireFrameScene::willFadeIn() {
    ofLogNotice("WireFrameScene::willFadeIn");
    
    float amp = 0.2;
    float speed = 0.2;
    AppManager::getInstance().getModelManager().onNoiseAmplitudeChange(amp);
    AppManager::getInstance().getModelManager().onNoiseSpeedChange(speed);
    
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

