/*
 *  WireFrameScene.h
 *  Primavera Sound Stage
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */

#pragma once

#include "ofxScene.h"

class WireFrameScene : public ofxScene {
    
public:
    
    //! Constructor
    WireFrameScene();
    
    //! Destructor
    ~WireFrameScene();
    
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
    
private:
    
    

    
};




