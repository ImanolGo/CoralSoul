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
    this->setupClouds();
    this->setupShader();
}


void NightScene::setupClouds()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    for(int i = 0; i<NUM_CLOUDS; i++)
    {
        string resourceName = "Cloud" + ofToString(i);
        ofPoint pos(ofRandom(width*0.3, width*0.7), ofRandom(height*0.2, height*0.3));
        ImageVisual cloud(pos,resourceName,true);
        m_clouds.push_back(cloud);
    }
    
    this->setupCloudShader();
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

void NightScene::setupStars()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_starsImage.setResource("StarryNight");
    m_starsImage.setCentred(true);
    m_starsImage.setPosition(ofPoint(width*0.5,height*0.5));
    
    m_fboStars.allocate(width, height);
    m_fboStars.begin(); ofClear(0); m_fboStars.end();
}

void NightScene::setupShader()
{
    if(ofIsGLProgrammableRenderer()){
        m_shader.load("shaders/shadersGL3/LiquifyShader");
    }
    else{
        m_shader.load("shaders/shadersGL2/LiquifyShader");
        
    }
}

void NightScene::update()
{
   this->updateStars();
   this->updateClouds();
}


void NightScene::updateClouds()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    for(auto& cloud: m_clouds){
        auto pos = cloud.getPosition();
        pos.x+= m_starsSpeed*10;
        if(pos.x>(width + cloud.getWidth()*0.5 )){
            pos.x = -cloud.getWidth()*0.5;
        }
        cloud.setPosition(pos);
    }
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
     this->drawStars();
     this->drawClouds();
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
//    m_shader.begin();        //Enable the shader
//
//    m_shader.setUniform1f("time", ofGetElapsedTimef());
//    // m_shader.setUniformTexture("tex", m_fbo.getTextureReference(), 0);
//    m_shader.setUniform1f("frequency", 0.6);
//    m_shader.setUniform1f("amplitude", 0.2);
//    m_shader.setUniform1f("speed", 0.4);
//
//
//    for(auto& cloud: m_clouds){
//        cloud.draw();x
//    }
//
//    m_shader.end();        //Disable the shader
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    
    m_cloudsShader.begin();
    m_cloudsShader.setUniform3f("iResolution", width, height, 0.0);
    m_cloudsShader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
        ofDrawRectangle(0, 0, width, height);
    m_cloudsShader.end();
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

