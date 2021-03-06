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
    float angleRadiands = degree2radian(weatherConditions.m_windDirection);
    float mag = 1.0;
    
    float speed = ofMap(weatherConditions.getWindSpeedNorm(), 0, 1.0, 0.3, 2, true);
    
    ofVec2f force;
    force.x = -mag*sin(angleRadiands);
    force.y = mag*cos(angleRadiands);
    
    m_vectorField.addForce(force);
    m_vectorField.setSpeed(speed);
    
    auto size =  AppManager::getInstance().getLayoutManager().getSizeWindParticles();
    auto num =  AppManager::getInstance().getLayoutManager().getNumWindParticles();
    
    m_vectorField.setSize(size);
    m_vectorField.setNumber(num);
    
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
	ofBackground(0);
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
    this->activateParticles();
	m_vectorField.resetParticles();
}

void WindScene::willDraw() {
    ofLogNotice("WindScene::willDraw");
}

void WindScene::willFadeOut() {
    ofLogNotice("WindScene::willFadeOut");
}

void WindScene::willExit() {
     ofLogNotice("WindScene::willExit");
    this->deactivateParticles();
}

void WindScene::activateParticles()
{
    string address = "/layer8/video/effect4/bypassed";
    int value = 0;
    
    ofxOscMessage m;
    m.setAddress(address);
    m.addFloatArg(value);
    
    AppManager::getInstance().getOscManager().sendMessage(m);
}

void WindScene::deactivateParticles()
{
    string address = "/layer8/video/effect4/bypassed";
    int value = 1;
    
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(value);
    
    AppManager::getInstance().getOscManager().sendMessage(m);
}

