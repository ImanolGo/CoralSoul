/*
 *  UniverseScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 01/12/17.
 *
 */

#pragma once

#include "ofxScene.h"

class UniverseScene : public ofxScene {
    
public:
    
    //! Constructor
    UniverseScene();
    
    //! Destructor
    ~UniverseScene();
    
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
    
    void updateFbo();
    
    void drawImage();
    
private:
    
    ofFbo      m_fbo;
    
};



