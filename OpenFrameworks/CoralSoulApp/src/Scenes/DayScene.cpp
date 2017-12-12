/*
 *  DayScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 10/11/17.
 *
 */


#include "DayScene.h"
#include "AppManager.h"

DayScene::DayScene(): ofxScene("DAY"), m_starsSpeed(0.0005)
{
    //Intentionally left empty
}

DayScene::~DayScene()
{
    //Intentionally left empty
}


void DayScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupImage();
    this->setupFbo();
    this->setupPlane();
}

void DayScene::setupImage()
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture("RockTexture");
}

void DayScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void DayScene::setupPlane()
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



void DayScene::update()
{
    auto isDayTime = AppManager::getInstance().getApiManager().isDayTime();
    if(isDayTime){
        this->updateSun();
    }
    else{
        this->updatePlane();
    }
    
    
   
    this->updateFbo();
}

void DayScene::updatePlane()
{
    m_nightPosition.x+=m_starsSpeed;
    m_nightPosition.y-=(m_starsSpeed/2);
    m_plane.mapTexCoords(m_nightPosition.x,  m_nightPosition.y, m_nightPosition.x+1,  m_nightPosition.y+1);
    
}

void DayScene::updateSun()
{
    auto time = ofGetElapsedTimef();
    float kelvin =  ofMap(sin(time), -1, 1, 2000, 10000);
    m_color = colorTemperatureToRGB(kelvin);
    
    float angle =  ofMap(sin(time), -1, 1, -180, 180);
    //AppManager::getInstance().getModelManager().onLightZChange(angle);
}

void DayScene::updateFbo()
{
    auto isDayTime = AppManager::getInstance().getApiManager().isDayTime();
    ofEnableAlphaBlending();
    m_fbo.begin();
        ofClear(0);
        if(isDayTime){
            this->drawDay();
        }
        else{
            this->drawNight();
        }
    
    m_fbo.end();
    ofDisableAlphaBlending();
}

void DayScene::draw()
{
    ofClear(0);
	ofBackground(0);
    //AppManager::getInstance().getModelManager().drawModel(m_fbo);
    m_fbo.draw(0,0);
}

void DayScene::drawDay()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();

    ofPushStyle();
    ofSetColor(m_color);
        m_texture->draw(0,0, width, height);
    ofPopStyle();
}

void DayScene::drawNight()
{
     this->drawPlane();
    AppManager::getInstance().getResourceManager().getTexture("ForegroundStars")->draw(0,0);
}

void DayScene::drawPlane()
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


void DayScene::willFadeIn() {
    ofLogNotice("DayScene::willFadeIn");
    ofColor color(0);
    AppManager::getInstance().getModelManager().setDirLightColorAnimation(color, 0.5);
	color = ofColor(255);
	AppManager::getInstance().getModelManager().setSpotLightColorAnimation(color, 0.5);
    
    m_nightPosition = ofVec2f(0.0);
    
}

void DayScene::willDraw() {
    ofLogNotice("DayScene::willDraw");
}

void DayScene::willFadeOut() {
    ofLogNotice("DayScene::willFadeOut");
}

void DayScene::willExit() {
     ofLogNotice("DayScene::willExit");
     ofColor color(255);
     AppManager::getInstance().getModelManager().setDirLightColorAnimation(color, 0.5);
	 color = ofColor(0);
	 AppManager::getInstance().getModelManager().setSpotLightColorAnimation(color, 0.5);
}


ofColor DayScene::colorTemperatureToRGB(float kelvin)
{
 
    float temp = kelvin / 100.0;
    
    ofColor color;
    float red, green, blue;
    
    if( temp <= 66 ){
        
        red = 255;
        
        green = temp;
        green = 99.4708025861 * log(green) - 161.1195681661;
        
        
        if( temp <= 19){
            
            blue = 0;
            
        } else {
            
            blue = temp-10;
            blue = 138.5177312231 * log(blue) - 305.0447927307;
            
        }
        
    } else {
        
        red  = temp - 60;
        red  = 329.698727446 * pow(red, -0.1332047592);
        
        green = temp - 60;
        green = 288.1221695283 * pow(green, -0.0755148492 );
        
        blue  = 255;
        
    }
    
    color.r = ofClamp(red, 0,255);
    color.g = ofClamp(green, 0,255);
    color.b = ofClamp(blue, 0,255);
    
    return color;

}

