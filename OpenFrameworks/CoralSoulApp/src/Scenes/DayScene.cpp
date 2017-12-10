/*
 *  DayScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 10/11/17.
 *
 */


#include "DayScene.h"
#include "AppManager.h"

DayScene::DayScene(): ofxScene("DAY")
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


void DayScene::update()
{
    this->updateSun();
    this->updateFbo();
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
    m_fbo.begin();
        ofClear(0);
        this->drawSun();
    m_fbo.end();
}

void DayScene::draw()
{
    ofClear(0);
	ofBackground(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
}

void DayScene::drawSun()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();

    ofPushStyle();
    ofSetColor(m_color);
        m_texture->draw(0,0, width, height);
    ofPopStyle();
}

void DayScene::willFadeIn() {
    ofLogNotice("DayScene::willFadeIn");
    ofColor color(0);
    AppManager::getInstance().getModelManager().setDirLightColorAnimation(color, 0.5);
	color = ofColor(255);
	AppManager::getInstance().getModelManager().setSpotLightColorAnimation(color, 0.5);
    
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

