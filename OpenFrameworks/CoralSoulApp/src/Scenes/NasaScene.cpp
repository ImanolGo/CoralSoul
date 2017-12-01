/*
 *  NasaScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 01/12/17.
 *
 */

#include "NasaScene.h"
#include "AppManager.h"

NasaScene::NasaScene(): ofxScene("NASA")
{
    //Intentionally left empty
}

NasaScene::~NasaScene()
{
    //Intentionally left empty
}


void NasaScene::setup() {
    ofLogNotice("NasaScene::setup");
    this->setupFbo();
}

void NasaScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void NasaScene::update()
{
    this->updateFbo();
}

void NasaScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawImage();
    m_fbo.end();
}

void NasaScene::draw()
{
    ofClear(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
    //this->drawImage();
    //m_fbo.draw(0,0);
}

void NasaScene::drawImage()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    auto& image =  AppManager::getInstance().getApiManager().getNasaImage();
    if(image.isAllocated())
    {
        image.draw(0,0,width,height);
    }
    
}

void NasaScene::willFadeIn() {
    ofLogNotice("NasaScene::willFadeIn");
    
}

void NasaScene::willDraw() {
    ofLogNotice("NasaScene::willDraw");
}

void NasaScene::willFadeOut() {
    ofLogNotice("NasaScene::willFadeOut");
}

void NasaScene::willExit() {
     ofLogNotice("NasaScene::willExit");
}

