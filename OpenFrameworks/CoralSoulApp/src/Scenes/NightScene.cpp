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
   this->updatePlane();
   this->updateClouds();
}


void NightScene::updateClouds()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    for(auto& cloud: m_clouds){
        auto pos = cloud.getPosition();
        pos.x+= m_starsSpeed*1000;
        if(pos.x>(width + cloud.getWidth()*0.5 )){
            pos.x = -cloud.getWidth()*0.5;
        }
        cloud.setPosition(pos);
    }
}


void NightScene::updatePlane()
{
    m_nightPosition.x+=m_starsSpeed;
    m_nightPosition.y-=(m_starsSpeed);
    m_plane.mapTexCoords(m_nightPosition.x,  m_nightPosition.y, m_nightPosition.x+1,  m_nightPosition.y+1);
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
     this->drawPlane();
     this->drawClouds();
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

void NightScene::drawClouds()
{
    m_shader.begin();        //Enable the shader
    
    m_shader.setUniform1f("time", ofGetElapsedTimef());
    // m_shader.setUniformTexture("tex", m_fbo.getTextureReference(), 0);
    m_shader.setUniform1f("frequency", 0.6);
    m_shader.setUniform1f("amplitude", 0.2);
    m_shader.setUniform1f("speed", 0.4);
    
    
    for(auto& cloud: m_clouds){
        cloud.draw();
    }
    
    m_shader.end();        //Disable the shader
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

