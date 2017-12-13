/*
 *  LifeScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 13/12/17.
 *
 */
#pragma once

#include "ofxScene.h"
#include "ofxColorGradient.h"

class LifeScene : public ofxScene {
    
public:
    
    //! Constructor
    LifeScene();
    
    //! Destructor
    ~LifeScene();
    
    //! Set up the scene
    void setup();
    
    //! Update the scene
    void update();
    
    //! Draw the scene
    void draw();
    
    //! Called when fading in
    void willFadeIn();
    
    //! Called when to start drawing
    void willDraw();
    
    //! Called fading out
    void willFadeOut();
    
    //! Called when exit
    void willExit();
    
private:
    
    void setupFbo();
    
    void setupColorGradient();
    
    void setupImage();
    
    void updateFbo();
    
    void drawImage();
    
    ofColor colorTemperatureToRGB(float kelvin);
    
private:
    
    ofPtr<ofTexture> m_texture;
    ofFbo      m_fbo;
    ofxColorGradient<ofColor>       m_gradient;
};




