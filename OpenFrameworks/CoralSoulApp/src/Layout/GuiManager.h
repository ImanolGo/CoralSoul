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
#include "ApiManager.h"
#include "WeatherConditions.h"

//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *    \brief Class managing the application´s grapical user interface (GUI)
 *    \details It creates the gui and the callback functions that will be affected
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
    
    int getWidth() {return m_gui.getWidth();}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    void onToggleEvent(ofxDatGuiToggleEvent e);
    
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    void onWeatherChange();
    
    void onSceneChange(string& sceneName);
    
    void onEnableSceneChange(string& sceneName, bool enable);
    
    void onSceneChange(int sceneIndex);
    
    bool isSceneActive(string name);
    
private:
    
    void setupGuiParameters();
    
    void setupLayoutGui();
    
    void setupScenesGui();
    
    void setupScenesToggleGui();
    
    void setupPreviewGui();
    
    void setupCameraGui();
    
    void setupModelGui();
    
    void setupLightGui();
    
    void setupWeatherGui();
    
    void setupNoiseGui();
    
    void drawRectangle();
    
    void drawGui();
    
    void setupGuiEvents();
    
    void setSceneToggle(string name, bool value);
    
    void loadSceneToggles();
    
private:
    
    ofxDatGui            m_gui;
    
    ofParameter<float>    m_guiFPS;
    
    
    ofParameterGroup      m_parameters;
    
    ofParameter<bool>      m_lifeScene;
    ofParameter<bool>      m_seaScene;
    ofParameter<bool>      m_windScene;
    ofParameter<bool>      m_rainScene;
    ofParameter<bool>      m_dayScene;
    ofParameter<bool>      m_nightScene;
    
    ofParameter<float>      m_cameraDistance;
    ofParameter<float>      m_cameraX;
    ofParameter<float>      m_cameraY;
    ofParameter<float>      m_cameraZ;
    ofParameter<float>      m_cameraFov;
    
    ofParameter<float>      m_modelPosX;
    ofParameter<float>      m_modelPosY;
    ofParameter<float>      m_modelPosZ;
    ofParameter<float>      m_modelRotX;
    ofParameter<float>      m_modelRotY;
    ofParameter<float>      m_modelRotZ;
    
    ofParameter<float>      m_layourBlurScale;
    ofParameter<float>      m_sceneDuration;
	ofParameter<float>      m_moonPositionX;
	ofParameter<float>      m_moonPositionY;
	ofParameter<float>      m_moonSize;
    ofParameter<float>      m_seaOpacity;
    ofParameter<float>      m_dayOpacity;
    ofParameter<float>      m_cloudsOpacity;
    ofParameter<int>        m_numWindParticles;
    ofParameter<float>      m_sizeWindParticles;
    ofParameter<float>      m_windFadeTime;
    
    ofParameter<float>      m_weatherTemperature;
    ofParameter<float>      m_weatherHumidity;
    ofParameter<float>      m_weatherWindSpeed;
    ofParameter<float>      m_weatherWindDirection;
    ofParameter<float>      m_weatherPrecipitation;
    ofParameter<float>      m_weatherClouds;
    ofParameter<float>      m_weatherMoon;
    ofParameter<float>      m_weatherSun;
    
    ofParameter<float>      m_swellHeight;
    ofParameter<float>      m_swellPeriod;
    
    ofParameter<float>      m_lightX;
    ofParameter<float>      m_lightY;
    ofParameter<float>      m_lightZ;
    
    ofParameter<int>        m_noiseResolution;
    ofParameter<float>      m_noiseFrequency;
    ofParameter<float>      m_noiseSpeed;
    
    ofxDatGuiLabel*        m_cityLabel;
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


