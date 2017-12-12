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
    
    void setupClouds();
    
    void setupShader();
    
    void setupCloudShader();
    
    void setupStars();
    
    void updateStars();
    
    void updateClouds();
    
    void drawStars();
    
    void drawClouds();
    
    void drawNight();
    
    ofColor colorTemperatureToRGB(float kelvin);
    
private:
    
    typedef vector<ImageVisual>  ImageVector;
    
    ImageVisual    m_starsImage;
    
    ofColor         m_color;
    ofShader        m_shader;
    ofShader        m_cloudsShader;
    
    
    ImageVector         m_clouds;
    
    ofFbo               m_fboStars;
    float               m_starsSpeed;
        
};




