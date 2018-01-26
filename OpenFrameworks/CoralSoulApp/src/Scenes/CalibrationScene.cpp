/*
 *  CalibrationScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 08/12/17.
 *
 */


#include "CalibrationScene.h"
#include "AppManager.h"

CalibrationScene::CalibrationScene(): ofxScene("CALIBRATION")
{
    //Intentionally left empty
}

CalibrationScene::~CalibrationScene()
{
    //Intentionally left empty
}


void CalibrationScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupImage();
}

void CalibrationScene::setupImage()
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture("Layout");
}


void CalibrationScene::update()
{
   //
}

void CalibrationScene::draw()
{
     float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    ofClear(0);;
	ofBackground(0);
    ofPushStyle();
    ofPushMatrix();
    ofScale(1,-1, 1); // Invert Y axis so increasing Y goes down
    ofTranslate(0, -height, 0); // shift origin up to upper left corner
        this->drawImage();
    
        ofSetColor(255, 255, 255,150);
        AppManager::getInstance().getModelManager().getModel().draw(0,0);
    
    ofPopMatrix();
    ofPopStyle();
}

void CalibrationScene::drawImage()
{
    if(m_texture)
    {
        float width = AppManager::getInstance().getSettingsManager().getAppWidth();
        float height = AppManager::getInstance().getSettingsManager().getAppHeight();
        
        m_texture->draw(0,0,width,height);

    }
    
}

void CalibrationScene::willFadeIn() {
    ofLogNotice("CalibrationScene::willFadeIn");
    AppManager::getInstance().getLayoutManager().onMaskChange(false);
    
}

void CalibrationScene::willDraw() {
    ofLogNotice("CalibrationScene::willDraw");
}

void CalibrationScene::willFadeOut() {
    ofLogNotice("CalibrationScene::willFadeOut");
}

void CalibrationScene::willExit() {
    ofLogNotice("CalibrationScene::willFadeOut");
     AppManager::getInstance().getLayoutManager().onMaskChange(true);
}

