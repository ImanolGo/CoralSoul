/*
 *  NightScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 12/12/17.
 *
 */


#include "NightScene.h"
#include "AppManager.h"

NightScene::NightScene(): ofxScene("NIGHT"), m_starsSpeed(0.0005)
{
    //Intentionally left empty
}

NightScene::~NightScene()
{
    //Intentionally left empty
}


void NightScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupPlane();
}


void NightScene::setupPlane()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    auto resourcePath = AppManager::getInstance().getSettingsManager().getTextureResourcesPath();
    
    ofDisableArbTex();
    string path = resourcePath["StarryNight"];
    ofImage img(path);
    m_starryNightTex = img.getTexture();
    m_starryNightTex.setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    m_plane.set(width, height);
    m_plane.setPosition(width*0.5,height*0.5,0);
    m_plane.setResolution(2, 2);
    ofEnableArbTex();
}



void NightScene::update()
{
   this->updatePlane();
}

void NightScene::updatePlane()
{
    m_nightPosition.x+=m_starsSpeed;
    m_nightPosition.y-=(m_starsSpeed/2);
    m_plane.mapTexCoords(m_nightPosition.x,  m_nightPosition.y, m_nightPosition.x+1,  m_nightPosition.y+1);
    
}

void NightScene::draw()
{
    ofClear(0);
    this->drawNight();
}

void NightScene::drawNight()
{
     this->drawPlane();
    AppManager::getInstance().getResourceManager().getTexture("ForegroundStars")->draw(0,0);
}

void NightScene::drawPlane()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    ofDisableArbTex();
    auto tex = AppManager::getInstance().getResourceManager().getTexture("StarryNight");
    m_starryNightTex.bind();
        m_plane.draw();
    m_starryNightTex.unbind();
     ofEnableArbTex();
}


void NightScene::willFadeIn() {
    ofLogNotice("NightScene::willFadeIn");
    m_nightPosition = ofVec2f(0.0);
    
}

void NightScene::willDraw() {
    ofLogNotice("NightScene::willDraw");
}

void NightScene::willFadeOut() {
    ofLogNotice("NightScene::willFadeOut");
}

void NightScene::willExit() {
     ofLogNotice("NightScene::willExit");
}

