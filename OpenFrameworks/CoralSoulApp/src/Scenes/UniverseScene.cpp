/*
 *  UniverseScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 01/12/17.
 *
 */

#include "UniverseScene.h"
#include "AppManager.h"

UniverseScene::UniverseScene(): ofxScene("UNIVERSE")
{
    //Intentionally left empty
}

UniverseScene::~UniverseScene()
{
    //Intentionally left empty
}


void UniverseScene::setup() {
    ofLogNotice("UniverseScene::setup");
    this->setupFbo();
}

void UniverseScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void UniverseScene::update()
{
    this->updateFbo();
}

void UniverseScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawImage();
    m_fbo.end();
}

void UniverseScene::draw()
{
    ofClear(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
    //this->drawImage();
    //m_fbo.draw(0,0);
}

void UniverseScene::drawImage()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    auto& image =  AppManager::getInstance().getApiManager().getNasaImage();
    if(image.isAllocated())
    {
        image.draw(0,0,width,height);
    }
    
}

void UniverseScene::willFadeIn() {
    ofLogNotice("UniverseScene::willFadeIn");
    
}

void UniverseScene::willDraw() {
    ofLogNotice("UniverseScene::willDraw");
}

void UniverseScene::willFadeOut() {
    ofLogNotice("UniverseScene::willFadeOut");
}

void UniverseScene::willExit() {
     ofLogNotice("UniverseScene::willExit");
}

