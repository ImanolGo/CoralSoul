/*
 *  GuiManager.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */

#pragma once

#include "Manager.h"
#include "ofxDatGui.h"

//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application´s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const int GUI_WIDTH;
    
public:

    //! Constructor
    GuiManager();

    //! Destructor
    ~GuiManager();

    //! Set-up the gui
    void setup();
    
    //! update the gui
    void update();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    void onSetVideoMode(bool& value);
    
    void onSetNoiseMode(bool& value);
    
    void onSetAudioMode(bool& value);
    
    int getWidth() {return GUI_WIDTH;}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    void onToggleEvent(ofxDatGuiToggleEvent e);
    
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    
private:
    
    void setupGuiParameters();
    
    void setupLayoutGui();
    
    void drawRectangle();
    
    void setupGuiEvents();

private:
    
    // Fluid GUI
    ofxDatGui			m_gui;

    ofParameter<float>	m_guiFPS;
    

    ofParameterGroup      m_parameters;

   
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


