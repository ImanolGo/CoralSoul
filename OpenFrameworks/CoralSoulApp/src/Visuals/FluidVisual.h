/*
 *  FluidVisual.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 14/11/17.
 *
 */


#pragma once

#include "ofxFlowTools.h"

using namespace flowTools;

//========================== class FluidManager ==============================
//============================================================================
/** \class FluidVisual FluidVisual.h
 *	\brief Class managing the fluid visuals
 *	\details It creates and manages all the parameters and fluid-like visualizations 
 */


class FluidVisual
{
    
public:
    
    //! Constructor
    FluidVisual();
    
    //! Destructor
    ~FluidVisual();
    
    //! Setup the Fluid Visual
    void setup(float downSampling = 1.0f);
    
    //! Update the Fluid Visual
    void update();
    
    //! Draw the Fluid Visual
    //void draw();
    
     //! Draw the Fluid Visual within a given area
    void draw(const ofRectangle& area);
    
    //! Set the source texture use to create the fluid
    void setSource(const ofFbo& source);
    
    //Gui
    
    void    resetDrawForces(bool& _value) { if (_value) {for (int i=0; i<m_numDrawForces; i++) m_flexDrawForces[i].reset();}}
    
    void    addForce(ofVec2f position);
   
    
private:
    
    void drawVisuals();
    
    void setupFluid();
    
    void updateFluid();
    
    void drawFluid();
    
public:
    
    static const int FLUID_WIDTH;
    static const int FLUID_HEIGHT;
    
    ofParameter<bool>	toggleGuiDraw;
    ofParameterGroup	visualisationParameters;
    
    ofParameterGroup	drawForceParameters;
    ofParameter<bool>	doResetDrawForces;
    ofParameterGroup	leftButtonParameters;
    ofParameterGroup	rightButtonParameters;
    ofParameter<bool>	showScalar;
    ofParameter<bool>	showField;
    ofParameter<float>	displayScalarScale;
    ofParameter<float>	velocityFieldArrowScale;
    ofParameter<float>	temperatureFieldBarScale;
    ofParameter<bool>	visualisationLineSmooth;
    
    
    void reset();
    
private:
    
    // FlowTools
    ftOpticalFlow		m_opticalFlow;
    ftVelocityMask		m_velocityMask;
    ftFluidSimulation	m_fluid;
    ftParticleFlow      m_particleFlow;
    
    float               m_downSampling;
    ofFbo               m_source;
    
    // Forces
    int                 m_numDrawForces;
    ftDrawForce*        m_flexDrawForces;
    ofVec2f             m_lastPosition;
  
    
};



