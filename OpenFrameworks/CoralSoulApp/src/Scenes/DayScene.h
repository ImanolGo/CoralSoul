/*
 *  DayScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 10/11/17.
 *
 */

#pragma once

#include "ofxScene.h"
#include "ofxShadertoy.h"

class DayScene : public ofxScene {
    
public:
    
    //! Constructor
    DayScene();
    
    //! Destructor
    ~DayScene();
    
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
    
    void setupImage();
    
    void setupDayShader();
    
    void setupFbo();
    
    void updateSun();
    
    void updateFbo();
    
    void drawDay();
    
    ofColor colorTemperatureToRGB(float kelvin);
    
private:
    
    ofPtr<ofTexture> m_texture;
    
    ofColor         m_color;
    ofFbo           m_fbo;
    
    ofShader       m_dayShader;
    ofTexture     m_noise;
    ofxShadertoy  m_shadertoy;
    
};




