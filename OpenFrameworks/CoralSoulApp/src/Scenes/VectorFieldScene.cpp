/*
 *  VectorFieldScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */


#include "VectorFieldScene.h"
#include "AppManager.h"

VectorFieldScene::VectorFieldScene(): ofxScene("VECTORFIELD")
{
    //Intentionally left empty
}

VectorFieldScene::~VectorFieldScene()
{
    //Intentionally left empty
}


void VectorFieldScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupVectorField();
    this->setupFbo();
}

void VectorFieldScene::setupVectorField()
{
    m_vectorField.setup();
}

void VectorFieldScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void VectorFieldScene::update()
{
    this->updateVectorField();
    this->updateFbo();
}

void VectorFieldScene::updateVectorField()
{
    auto source = AppManager::getInstance().getNoiseManager().getFbo();
    m_vectorField.update();
}


void VectorFieldScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawVectorField();
    m_fbo.end();
}
void VectorFieldScene::draw()
{
    ofClear(0);
    //AppManager::getInstance().getModelManager().drawModel(m_fbo);
    m_vectorField.draw();
    //this->drawVectorField();
}

void VectorFieldScene::drawVectorField()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_vectorField.draw();
}

void VectorFieldScene::willFadeIn() {
    ofLogNotice("VectorFieldScene::willFadeIn");

}

void VectorFieldScene::willDraw() {
    ofLogNotice("VectorFieldScene::willDraw");
}

void VectorFieldScene::willFadeOut() {
    ofLogNotice("VectorFieldScene::willFadeOut");
}

void VectorFieldScene::willExit() {
     ofLogNotice("VectorFieldScene::willExit");
}
