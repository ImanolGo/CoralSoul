/*
 *  ImageScene.cpp
 *  Primavera Sound Stage
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */


#include "ImageScene.h"
#include "AppManager.h"

ImageScene::ImageScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

ImageScene::~ImageScene()
{
    //Intentionally left empty
}


void ImageScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupImage();
}

void ImageScene::setupImage()
{
    m_texture = AppManager::getInstance().getResourceManager().getTexture(getName());
}


void ImageScene::update()
{
    //Empty
}


void ImageScene::draw()
{
    this->drawImage();
}

void ImageScene::drawImage()
{
    if(m_texture)
    {
        float width = AppManager::getInstance().getSettingsManager().getAppWidth();
        float height = AppManager::getInstance().getSettingsManager().getAppHeight();
        
        m_texture->draw(0,0,width,height);

    }
    
}

void ImageScene::willFadeIn() {
    ofLogNotice("ImageScene::willFadeIn");
    
}

void ImageScene::willDraw() {
    ofLogNotice("ImageScene::willDraw");
}

void ImageScene::willFadeOut() {
    ofLogNotice("ImageScene::willFadeOut");
}

void ImageScene::willExit() {
}

