/*
 *  CalibrationScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 08/12/17.
 *
 */

#pragma once

#include "ofxScene.h"

class CalibrationScene : public ofxScene {
    
public:
    
    //! Constructor
    CalibrationScene();
    
    //! Destructor
    ~CalibrationScene();
    
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
    
    void setupImage();
    
    void updateFbo();
    
    void drawImage();
    
private:
    
    ofPtr<ofTexture> m_texture;
    
};




