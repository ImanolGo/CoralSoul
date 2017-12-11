/*
 *  RainScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 05/11/17.
 *
 */

#pragma once

#include "ofxScene.h"

#include "ofxRipples.h"
#include "ofxBounce.h"

class RainScene : public ofxScene {
    
public:
    
    //! Constructor
    RainScene();
    
    //! Destructor
    ~RainScene();
    
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
    
    void setupFbo();
    
    void setupShader();
    
    void setupRipples();
    
    void updateRipples();
    
    void updateFbo();
    
    void drawRipples();
    
private:
    
    ofPtr<ofTexture> m_texture;
    
    ofxRipples  m_ripples;
    ofxBounce   m_bounce;
    
    ofFbo      m_fbo;
    ofShader   m_shader;
    float                  m_frequency, m_amplitude, m_speed; //shader parameters
    
};




