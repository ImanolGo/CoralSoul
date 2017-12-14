/*
 *  LifeScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 13/12/17.
 *
 */


#include "LifeScene.h"
#include "AppManager.h"

LifeScene::LifeScene(): ofxScene("LIFE")
{
    //Intentionally left empty
}

LifeScene::~LifeScene()
{
    //Intentionally left empty
}


void LifeScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupImage();
    this->setupFbo();
    this->setupColorGradient();
}

void LifeScene::setupImage()
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture("Coal");
}

void LifeScene::setupColorGradient()
{
    //first, fill in the gradient with colors, left to right
    m_gradient.addColor( ofColor::blue );
    m_gradient.addColor( ofColor::cyan);
    m_gradient.addColor( ofColor::yellow);
    m_gradient.addColor( ofColor::orange);
    m_gradient.addColor( ofColor::red);
}

void LifeScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void LifeScene::update()
{
    this->updateFbo();
}

void LifeScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawImage();
    m_fbo.end();
}

void LifeScene::draw()
{
    ofClear(0);
    AppManager::getInstance().getModelManager().drawModel(m_fbo);
}

void LifeScene::drawImage()
{
    if(m_texture)
    {
        float width = AppManager::getInstance().getSettingsManager().getAppWidth();
        float height = AppManager::getInstance().getSettingsManager().getAppHeight();
        
        m_texture->draw(0,0,width,height);

    }
    
}

void LifeScene::willFadeIn() {
    ofLogNotice("LifeScene::willFadeIn");
    
    ofColor color(100);
    auto tempNorm = AppManager::getInstance().getApiManager().getCurrentWeather().getTemperatureNorm();
//    temp = ofMap(temp, 0, 35,5000, 2000,true);
//    color =  colorTemperatureToRGB(temp);
    tempNorm = ofMap(tempNorm, 0.0, 1.0, 0.0, 1.0,true);
    color = m_gradient.getColorAtPercent(tempNorm);
    AppManager::getInstance().getModelManager().setSpotLightColorAnimation(color, 0.5);
    
    color = ofColor(200);
   
    AppManager::getInstance().getModelManager().setDirLightColorAnimation(color, 0.5);
    
}

void LifeScene::willDraw() {
    ofLogNotice("LifeScene::willDraw");
}

void LifeScene::willFadeOut() {
    ofLogNotice("LifeScene::willFadeOut");
}

void LifeScene::willExit() {
    ofLogNotice("LifeScene::willFadeOut");
    
    ofLogNotice("DayScene::willExit");
    ofColor color(255);
    AppManager::getInstance().getModelManager().setDirLightColorAnimation(color, 0.5);
    color = ofColor(0);
    AppManager::getInstance().getModelManager().setSpotLightColorAnimation(color, 0.5);
}


ofColor LifeScene::colorTemperatureToRGB(float kelvin)
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


