/*
 *  SeaScene.h
 *  Primavera Sound Stage
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */

#pragma once

#include "ofxScene.h"
#include "ofxPostProcessing.h"

class SeaScene : public ofxScene {
    
public:
    
    //! Constructor
    SeaScene();
    
    //! Destructor
    ~SeaScene();
    
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
    
    void setupPostProcessing();
    
    void updateNoise();
    
private:
    
    ofxPostProcessing m_post;
    
};




