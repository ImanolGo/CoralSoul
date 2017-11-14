/*
 *  FlowScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 13/11/17.
 *
 */


#include "FlowScene.h"
#include "AppManager.h"

FlowScene::FlowScene(): ofxScene("FLOW")
{
    //Intentionally left empty
}

FlowScene::~FlowScene()
{
    //Intentionally left empty
}


void FlowScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupFluid();
}

void FlowScene::setupFluid()
{
    m_fluid.setup("xmls/FLOW_GUI.xml");
}

void FlowScene::update()
{
    this->updateFluid();
}

void FlowScene::updateFluid()
{
    auto source = AppManager::getInstance().getNoiseManager().getFbo();
    
    m_fluid.setSource(source);
    m_fluid.update();
}


void FlowScene::draw()
{
    ofClear(0);
    this->drawFluid();
   
}

void FlowScene::drawFluid()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    ofRectangle rect(0,0,width,height);
    m_fluid.draw(rect);
}

void FlowScene::willFadeIn() {
    ofLogNotice("FlowScene::willFadeIn");
    m_fluid.reset();
}

void FlowScene::willDraw() {
    ofLogNotice("FlowScene::willDraw");
}

void FlowScene::willFadeOut() {
    ofLogNotice("FlowScene::willFadeOut");
}

void FlowScene::willExit() {
     ofLogNotice("FlowScene::willExit");
     m_fluid.reset();
}
