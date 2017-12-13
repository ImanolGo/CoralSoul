/*
 *  NightScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 12/12/17.
 *
 */


#include "NightScene.h"
#include "AppManager.h"

const int NightScene::NUM_CLOUDS = 4;

NightScene::NightScene(): ofxScene("NIGHT"), m_starsSpeed(0.05)
{
    //Intentionally left empty
}

NightScene::~NightScene()
{
    //Intentionally left empty
}


void NightScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupStars();
    this->setupCloudShader();
    this->setupMoonShader();
}

void NightScene::setupCloudShader()
{
    ofDisableArbTex();
    
    if(ofIsGLProgrammableRenderer()){
        m_cloudsShader.load("shaders/shadersGL3/Clouds");
    }
    else{
        m_cloudsShader.load("shaders/shadersGL2/Clouds");
    }
    
     ofEnableArbTex();
}

void NightScene::setupMoonShader()
{
    ofDisableArbTex();
    
    if(ofIsGLProgrammableRenderer()){
        m_moonShader.load("shaders/shadersGL3/Moon");
    }
    else{
        m_moonShader.load("shaders/shadersGL2/Moon");
    }
    
    ofEnableArbTex();
    
    auto resources = AppManager::getInstance().getSettingsManager().getTextureResourcesPath();
    string name = resources["ColorNoise"];
    m_color_noise.load(name);
}

void NightScene::setupStars()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_starsImage.setResource("StarryNight");
    m_starsImage.setCentred(true);
    m_starsImage.setPosition(ofPoint(width*0.5,height*0.5));
    m_starsImage.setHeight(width,true);
    
    m_fboStars.allocate(width, height);
    m_fboStars.begin(); ofClear(0); m_fboStars.end();
}



void NightScene::update()
{
   this->updateStars();
}

void NightScene::updateStars()
{
    auto rotation = m_starsImage.getRotation();
    rotation.z += m_starsSpeed;
    m_starsImage.setRotation(rotation);
}

void NightScene::draw()
{
    ofEnableAlphaBlending();
    ofClear(0);
    this->drawNight();
    ofDisableAlphaBlending();
   
}

void NightScene::drawNight()
{
    ofClear(0, 0, 0);
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    
        this->drawStars();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
        this->drawClouds();
        this->drawMoon();
    
    ofPopStyle();
    
    
    
     AppManager::getInstance().getResourceManager().getTexture("ForegroundStars")->draw(0,0);
}

void NightScene::drawStars()
{
//    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
//    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
//
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    m_fboStars.begin();
//    ofPushStyle();
//        ofSetColor(0, 0, 0, 10);
//        ofDrawRectangle(0, 0, width, height);
//        ofSetColor(255);
//            m_starsImage.draw();
//    ofPopStyle();
//    m_fboStars.end();
//    ofDisableBlendMode();
//
//    m_fboStars.draw(0,0);
    
     m_starsImage.draw();
}

void NightScene::drawClouds()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    float cloudcover = AppManager::getInstance().getApiManager().getCurrentWeather().clouds;
    cloudcover  = ofMap(cloudcover, 0,100,0.0,1.0,true);
    
    float speed = AppManager::getInstance().getApiManager().getCurrentWeather().windSpeed;
    speed  = ofMap(speed,0,100,0.005,0.4,true);
    
    
    m_cloudsShader.begin();
    m_cloudsShader.setUniform3f("iResolution", width, height, 0.0);
    m_cloudsShader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
    m_cloudsShader.setUniform1f("cloudcover", cloudcover);
    m_cloudsShader.setUniform1f("speed", speed);
        ofDrawRectangle(0, 0, width, height);
    m_cloudsShader.end();
}

void NightScene::drawMoon()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth()*0.5;
    float height = AppManager::getInstance().getSettingsManager().getAppHeight()*0.5;
    float moonPhase = AppManager::getInstance().getApiManager().getCurrentWeather().moonPhase;
    moonPhase  = ofMap(moonPhase, 0,1.0,0.0,1.0,true);

    m_moonShader.begin();
    m_moonShader.setUniform3f("iResolution", width, height, 0.0);
    m_moonShader.setUniform1f("iTime", ofGetElapsedTimef());
    m_moonShader.setUniformTexture("iChannel0", m_color_noise.getTexture(), 1);
    ofDrawRectangle(0,0, width, height);
    m_moonShader.end();
}

void NightScene::willFadeIn() {
    ofLogNotice("NightScene::willFadeIn");    
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

