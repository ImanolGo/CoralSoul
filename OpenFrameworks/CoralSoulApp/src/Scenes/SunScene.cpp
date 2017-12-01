/*
 *  SunScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 10/11/17.
 *
 */


#include "SunScene.h"
#include "AppManager.h"

SunScene::SunScene(): ofxScene("SUN")
{
    //Intentionally left empty
}

SunScene::~SunScene()
{
    //Intentionally left empty
}


void SunScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupImage();
    this->setupFbo();
}

void SunScene::setupImage()
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture("RockTexture");
}

void SunScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void SunScene::update()
{
    this->updateSun();
    this->updateFbo();
}

void SunScene::updateSun()
{
    auto time = ofGetElapsedTimef();
    float kelvin =  ofMap(sin(time), -1, 1, 2000, 10000);
    m_color = colorTemperatureToRGB(kelvin);
    
    float angle =  ofMap(sin(time), -1, 1, -180, 180);
    AppManager::getInstance().getModelManager().onLightZChange(angle);
}

void SunScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawSun();
    m_fbo.end();
}

void SunScene::draw()
{
    ofClear(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
}

void SunScene::drawSun()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();

    ofPushStyle();
    ofSetColor(m_color);
        m_texture->draw(0,0, width, height);
    ofPopStyle();
}

void SunScene::willFadeIn() {
    ofLogNotice("SunScene::willFadeIn");
    //AppManager::getInstance().getModelManager().onLightColorChange(ofColor(0, 191, 255));
    
}

void SunScene::willDraw() {
    ofLogNotice("SunScene::willDraw");
}

void SunScene::willFadeOut() {
    ofLogNotice("SunScene::willFadeOut");
}

void SunScene::willExit() {
     ofLogNotice("SunScene::willExit");
}


ofColor SunScene::colorTemperatureToRGB(float kelvin)
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

