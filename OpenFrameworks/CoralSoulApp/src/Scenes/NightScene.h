/*
 *  NightScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 12/12/17.
 *
 */

#pragma once

#include "ofxScene.h"


class NightScene : public ofxScene {
    
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
    
    void setupImage();
    
    void setupPlane();
    
    void setupFbo();
    
    void updateSun();
    
    void updatePlane();
    
    void drawPlane();
    
    void updateFbo();
    
    void drawDay();
    
    void drawNight();
    
    ofColor colorTemperatureToRGB(float kelvin);
    
private:
    
    ofPtr<ofTexture> m_texture;
    ofTexture        m_starryNightTex;
    
    ofColor         m_color;
    ofFbo           m_fbo;
    
    ofPlanePrimitive    m_plane;
    ofVec2f             m_nightPosition;
    float               m_starsSpeed;
        
};




