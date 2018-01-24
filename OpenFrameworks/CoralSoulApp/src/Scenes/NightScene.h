/*
 *  NightScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 12/12/17.
 *
 */

#pragma once

#include "ofxScene.h"
#include "ImageVisual.h"
#include "ofxBlur.h"
#include "ofxPostProcessing.h"

#include "ofxShadertoy.h"

class NightScene : public ofxScene {
    
    static const int NUM_CLOUDS;
    
public:
    
    //! Constructor
    NightScene();
    
    //! Destructor
    ~NightScene();
    
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
    
    void setupCloudShader();
    
    void setupMoonShader();
    
    void setupStars();
    
    void updateStars();
    
    void drawStars();
    
    void drawClouds();
    
    void drawMoon();
    
    void drawNight();
    
    ofColor colorTemperatureToRGB(float kelvin);
    
private:
    
    ImageVisual    m_starsImage;
    
    ofColor         m_color;
   
    ofxBlur         m_moonBlur;
    ofxPostProcessing   m_post;
    
    ofFbo               m_fboStars;
    ofFbo               m_fboMoon;
    float               m_starsSpeed;
    vector<float>         m_moonPhases;

    float oldPhase;
    
    ofShader  m_moonShader;
    ofShader  m_cloudsShader;
    
    ofxShadertoy  m_moonShaderToy;
    ofxShadertoy  m_cloudsShaderToy;
    
};




