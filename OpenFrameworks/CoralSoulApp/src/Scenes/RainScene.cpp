/*
 *  RainScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 05/11/17.
 *
 */


#include "RainScene.h"
#include "AppManager.h"

RainScene::RainScene(): ofxScene("RAIN")
{
    //Intentionally left empty
}

RainScene::~RainScene()
{
    //Intentionally left empty
}


void RainScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupImage();
    this->setupRipples();
    this->setupFbo();
    this->setupShader();
}


void RainScene::setupShader()
{
    
    if(ofIsGLProgrammableRenderer()){
        m_shader.load("shaders/shadersGL3/LiquifyShader");
    }
    else{
        m_shader.load("shaders/shadersGL2/LiquifyShader");
        
    }
    
    m_frequency = 0.4;
    m_amplitude = 0.2;
    m_speed = 0.3;
}


void RainScene::setupImage()
{
    m_texture = this->getCurrentTexture();
}

void RainScene::setupRipples()
{
    m_texture = this->getCurrentTexture();
    m_ripples.clear();
    m_bounce.clear();
    m_ripples.allocate(m_texture->getWidth(), m_texture->getHeight());
    m_bounce.allocate(m_texture->getWidth(),  m_texture->getHeight());
    m_bounce.setTexture(*m_texture.get(), 1);
	m_ripples.damping = 0.97;
   
}

void RainScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void RainScene::update()
{
    this->updateRipples();
    this->updateFbo();
}

void RainScene::updateRipples()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
   // auto tex =  AppManager::getInstance().getModelManager().getModel().getTexture();
    
    //m_bounce.setTexture(tex, 1);
    //float precMM = AppManager::getInstance().getApiManager().getCurrentWeather().getPrecipitationNorm();
    float precMM = AppManager::getInstance().getApiManager().getCurrentWeather().m_precipitationValue;
    int skip = (int) ofMap(precMM, 0.0, 20.0, 6, 1, true);
    
    m_bounce.setTexture(*m_texture.get(), 1);
    
    if(precMM > 0 && ofGetFrameNum()%skip == 0){
        m_ripples.begin();
        
        ofFill();
        ofSetColor(ofNoise( ofGetFrameNum() ) * 255*2, 255);
        int dropsPerCycle = 5;
        for(int i=0; i<dropsPerCycle; i++){
             ofDrawEllipse(ofRandom(width),ofRandom(height), 3,3);
        }
       
        m_ripples.end();
    }
    
   
   
    m_ripples.update();
    
    m_bounce << m_ripples;
    
    //m_amplitude  = ofMap(precMM, 0.0, 10.0, 0, 0.4, true);
    //m_amplitude  = 0.1;
}

ofPtr<ofTexture> RainScene::getCurrentTexture()
{
    if(!AppManager::getInstance().getApiManager().getCurrentWeather().isDayTime()){
         m_amplitude  = 0.15;
        return  AppManager::getInstance().getResourceManager().getTexture("RAIN_DAY");
    }
    else{
         m_amplitude  = 0.11;
         return AppManager::getInstance().getResourceManager().getTexture("RAIN_NIGHT");
    }
}

void RainScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        //Drawing to screen through the shader
        m_shader.begin();        //Enable the shader
    
        m_shader.setUniform1f("time", ofGetElapsedTimef());
        // m_shader.setUniformTexture("tex", m_fbo.getTextureReference(), 0);
        m_shader.setUniform1f("frequency", m_frequency);
        m_shader.setUniform1f("amplitude", m_amplitude);
        m_shader.setUniform1f("speed", m_speed);
            this->drawRipples();
        m_shader.end();        //Disable the shader
    m_fbo.end();
}

void RainScene::draw()
{
    ofClear(0);
	ofBackground(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
}

void RainScene::drawRipples()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();

     m_bounce.draw(0,0, width, height);
    //m_ripples.draw(0,0, width, height);
    
    // m_texture->draw(0,0, width, height);
}

void RainScene::willFadeIn() {
    this->setupRipples();
    ofLogNotice("RainScene::willFadeIn");
    
}

void RainScene::willDraw() {
    ofLogNotice("RainScene::willDraw");
}

void RainScene::willFadeOut() {
    ofLogNotice("RainScene::willFadeOut");
}

void RainScene::willExit() {
}

