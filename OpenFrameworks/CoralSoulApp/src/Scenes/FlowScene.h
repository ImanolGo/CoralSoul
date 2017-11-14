/*
 *  FlowScene.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 13/11/17.
 *
 */

#pragma once

#include "ofxScene.h"
#include "FluidVisual.h"

class FlowScene : public ofxScene {
    
public:
    
    //! Constructor
    FlowScene();
    
    //! Destructor
    ~FlowScene();
    
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
    
    void setupFluid();
    
    void updateFluid();
    
    void drawFluid();
    
private:
    
    FluidVisual             m_fluid;
    
};




