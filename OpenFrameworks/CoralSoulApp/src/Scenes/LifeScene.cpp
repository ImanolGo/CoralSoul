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
    m_gradient.addColor( ofColor::orange);
    m_gradient.addColor( ofColor::red);
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
    this->updateBreathing();
    
//    auto tempNorm = AppManager::getInstance().getApiManager().getCurrentWeather().getTemperatureNorm();
//    tempNorm = ofMap(tempNorm, 0.0, 1.0, 0.0, 1.0,true);
//    ofColor color = m_gradient.getColorAtPercent(tempNorm);
//    AppManager::getInstance().getModelManager().setSpotLightColorAnimation(color, 0.5);
}

void LifeScene::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        this->drawImage();
    m_fbo.end();
}

void LifeScene::updateBreathing()
{
    float value = ofMap(1.4*cos(ofGetElapsedTimef()*1.0f), -1, 1, -200, 200, true);
    AppManager::getInstance().getModelManager().setSpotLightZ(value);
    
    value =  ofMap(value, -200, 200, 0.0, 1.0, true);
    this->sendOscBreath(value);
}

void LifeScene::sendOscBreath(float value)
{
    string address = "/activeclip/video/effect5/value";
    
    ofxOscMessage m;
    m.setAddress(address);
    m.addFloatArg(value);
    
    AppManager::getInstance().getOscManager().sendMessage(m);
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
    
    ofLogNotice() << "LifeScene::willFadeIn -> Temp Norm: " << tempNorm;
//    temp = ofMap(temp, 0, 35,5000, 2000,true);
//    color =  colorTemperatureToRGB(temp);
    //tempNorm = ofMap(tempNorm, 0.0, 1.0, 0.0, 1.0,true);
    //color = m_gradient.getColorAtPercent(tempNorm);
    color = getCurrentTempColor();
    
    ofLogNotice() << "LifeScene::willFadeIn -> Color: " << color;
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


ofColor LifeScene::getCurrentTempColor()
{
    auto tempNorm = AppManager::getInstance().getApiManager().getCurrentWeather().getTemperatureNorm();
    auto heatMap =  AppManager::getInstance().getResourceManager().getTexture("HeatMap");
    ofPixels pixels;
    heatMap->readToPixels(pixels);
    int index = (int) ofMap(tempNorm, 0.0, 1.0, 0, heatMap->getWidth(), true);
    ofColor c = pixels.getColor(index, 0.0);
    return c;
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

void LifeScene::activateEffect()
{
	string address = "/layer8/video/effect5/bypassed";
	int value = 0;

	ofxOscMessage m;
	m.setAddress(address);
	m.addIntArg(value);

	AppManager::getInstance().getOscManager().sendMessage(m);
}

void LifeScene::deactivateEffect()
{
	string address = "/layer8/video/effect5/bypassed";
	int value = 1;

	ofxOscMessage m;
	m.setAddress(address);
	m.addIntArg(value);

	AppManager::getInstance().getOscManager().sendMessage(m);
}
