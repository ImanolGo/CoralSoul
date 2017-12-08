/*
 *  WindScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 03/12/17.
 *
 */


#include "WindScene.h"
#include "AppManager.h"

#define radian2degree(a) (a * 57.295779513082)
#define degree2radian(a) (a * 0.017453292519)

WindScene::WindScene(): ofxScene("WIND")
{
    //Intentionally left empty
}

WindScene::~WindScene()
{
    //Intentionally left empty
}


void WindScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupVectorField();
    this->setupFbo();
}

void WindScene::setupVectorField()
{
    m_vectorField.setup();
}

void WindScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void WindScene::update()
{
    this->updateVectorField();
    this->updateFbo();
}

void WindScene::updateVectorField()
{
    auto weatherConditions = AppManager::getInstance().getApiManager().getCurrentWeather();
    float angleRadiands = degree2radian(weatherConditions.windDirection);
    float mag = 1.0;
    float speed = ofMap(weatherConditions.windSpeed, 0, 100, 0.5, 10, true);
    
    ofVec2f force;
    force.x = mag*sin(angleRadiands);
    force.y = -mag*cos(angleRadiands);
    
    m_vectorField.addForce(force);
    m_vectorField.setSpeed(speed);
    
    m_vectorField.update();
}


void WindScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawVectorField();
    m_fbo.end();
}
void WindScene::draw()
{
    ofClear(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
    //m_vectorField.draw();
    //this->drawVectorField();
}

void WindScene::drawVectorField()
{
    m_vectorField.draw();
}

void WindScene::willFadeIn() {
    ofLogNotice("WindScene::willFadeIn");

}

void WindScene::willDraw() {
    ofLogNotice("WindScene::willDraw");
}

void WindScene::willFadeOut() {
    ofLogNotice("WindScene::willFadeOut");
}

void WindScene::willExit() {
     ofLogNotice("WindScene::willExit");
}
