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
    this->setupDayShader();
}

void DayScene::setupImage()
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture("RockTexture");
    
    ofImage img;
    img.load("images/textures/noiseSun2.png");
    ///*
    m_noise.allocate(img.getWidth(), img.getHeight(), GL_RGBA, false); // fourth parameter is false to avoid generation of a GL_TEXTURE_2D_RECTANGLE texture - we don't want this.
    m_noise.loadData(img.getPixels());
    m_noise.generateMipmap();
    m_noise.setTextureWrap(GL_REPEAT, GL_REPEAT);
    //shadertoy.setTexture(0, tex);
    
    //m_noise.load("images/textures/noiseSun2.png");
}

void DayScene::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width, height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void DayScene::setupDayShader()
{
//    ofImage img;
//    img.load("images/textures/noiseSun2.png");
//    ///*
//    ofTexture tex;
//    tex.allocate(img.getWidth(), img.getHeight(), GL_RGBA, false); // fourth parameter is false to avoid generation of a GL_TEXTURE_2D_RECTANGLE texture - we don't want this.
//    tex.loadData(img.getPixels());
//    tex.generateMipmap();
//    tex.setTextureWrap(GL_REPEAT, GL_REPEAT);
//    m_shadertoy.setTexture(0, tex);
    
    ofDisableArbTex();
    
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.load("shaders/shadersGL3/Sun.frag");
       // m_shadertoy.setTexture(0, m_noise.getTexture());
        m_shadertoy.setTexture(0, m_noise);
    }
    else{
        m_dayShader.load("shaders/shadersGL2/Sun");
    }
    
    ofEnableArbTex();
    
    m_shadertoy.setAdvanceTime(true);
}

void DayScene::update()
{
    //this->updateSun();
   // this->updateFbo();
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
            this->drawDay();
    m_fbo.end();
    ofDisableAlphaBlending();
}

void DayScene::draw()
{
    ofClear(0);
	ofBackground(0);
    this->drawDay();
    //AppManager::getInstance().getModelManager().drawModel(m_fbo);
}

void DayScene::drawDay()
{
	float width = AppManager::getInstance().getSettingsManager().getAppWidth();
	float height = AppManager::getInstance().getSettingsManager().getAppHeight();
	float sunPosition = AppManager::getInstance().getApiManager().getCurrentWeather().m_sunPosition;
	sunPosition = ofMap(sunPosition, 0.0, 1.0, PI / 2 - PI / 10, 3 * PI / 2 + PI / 10, true);

	float speed = AppManager::getInstance().getApiManager().getCurrentWeather().getWindSpeedNorm();
	speed = ofMap(speed, 0.0, 1.0, 0.1, 2.0, true);
    
    
    auto tex = AppManager::getInstance().getResourceManager().getTexture("NoiseSun");

    float cloudcover = AppManager::getInstance().getApiManager().getCurrentWeather().getCloudinessNorm();
    
    if(ofIsGLProgrammableRenderer()){
        m_shadertoy.begin();
        m_shadertoy.setUniform3f("iResolution", width, height, 0.0);
        m_shadertoy.setUniform1f("sunPosition", sunPosition);
        m_shadertoy.setUniform1f("speed", speed);
        m_shadertoy.setUniform1f("cloudcover", cloudcover);
            ofDrawRectangle(0, 0, width, height);
        m_shadertoy.end();
        
        //m_shadertoy.draw(0, 0, width, height);
    }
    else{
            m_dayShader.begin();
            m_dayShader.setUniform3f("iResolution", width, height, 0.0);
            m_dayShader.setUniform1f("iTime", ofGetElapsedTimef());
            m_dayShader.setUniformTexture("iChannel0", m_noise, 1);
            m_dayShader.setUniform1f("sunPosition", sunPosition);
            m_dayShader.setUniform1f("speed", speed);
            m_dayShader.setUniform1f("cloudcover", cloudcover);
                ofDrawRectangle(0, 0, width, height);
            m_dayShader.end();
    }
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

