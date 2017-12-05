/*
 *  SettingsManager.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */


#pragma once

#include "Manager.h"
#include "ApiManager.h"


//========================== class SettingsManager ==============================
//============================================================================
/** \class SettingsManager SettingsManager.h
 *	\brief Class managing the whole settings of the application
 *	\details it reads from an xml settings file and provides access to the information
 */

typedef             map<string,string>               ResourcesPathMap;       ///< defines a map of path attached to the resources name

class SettingsManager: public Manager
{
    
    static const string APPLICATION_SETTINGS_FILE_NAME;
    
public:
    
    //! Destructor
    ~SettingsManager();
    
    //! Constructor
    SettingsManager();
    
    //! Compares two transition objects
    void setup();
    
    const ResourcesPathMap& getTextureResourcesPath() const {return m_texturesPath;}
    
    const ResourcesPathMap& getSvgResourcesPath() const {return m_svgResourcesPath;}
    
    const ResourcesPathMap& getVideoResourcesPath() const {return m_videoResourcesPath;}
    
    const ResourcesPathMap& getModelResourcesPath() const {return m_modelResourcesPath;}
    
    ofColor getColor(const string& colorName);
    
    float getAppWidth() const {return m_appWidth;}
    
    float getAppHeight() const {return m_appHeight;}
    
    string getIpAddress() const {return m_ipAddress;}
    
    string getSpoutName() const {return m_spoutName;}
    
    int getOscPortReceive() const {return m_portOscReceive;}
    
    int getOscPortSend() const {return m_portOscSend;}
    
    api_settings getWeatherSettings() const {return m_weatherSettings;}
    
    api_settings getNasaSettings() const {return m_nasaSettings;}
    
    api_settings getsurfSettings() const {return m_surfSettings;}
    
    
private:
    
    //! Loads the settings file
    bool loadSettingsFile();
    
    //! Loads all the settings
    void loadAllSettings();
    
    //! Sets all the debug properties
    void setDebugProperties();
    
    //! Sets all the network properties
    void setNetworkProperties();
    
    //! Sets all the api properties
    void setApiProperties();
    
    //! Sets all the window properties
    void setWindowProperties();
    
    //! Loads all the app colors
    void loadColors();
    
    //! Loads all the textures settings
    void loadTextureSettings();
    
    //! Loads all the svg images settings
    void loadSvgSettings();
    
    //! Loads all the video  settings
    void loadVideoSettings();
    
    //! Loads all the model names and paths
    void loadModelSettings();
    
private:
    
    typedef             map< string, ofPtr<ofColor> >    ColorMap;               ///< Defines a map of colors attached to a name
    
    
    ofXml		            m_xml;                  ///< instance of the xml parser
    ResourcesPathMap        m_texturesPath;         ///< stores the texture paths
    ResourcesPathMap        m_svgResourcesPath;     ///< stores the resources paths
    ResourcesPathMap        m_videoResourcesPath;   ///< stores the video paths
    ResourcesPathMap        m_modelResourcesPath;   ///< stores the model paths
    ColorMap                m_colors;               ///< stores all the application's colors
    float                   m_appWidth;             ///< stores the applications width
    float                   m_appHeight;            ///< stores the applications height
    int                     m_portOscReceive;       ///< stores the  port to receive OSC messages from
    int                     m_portOscSend;          ///< stores the  port to send OSC messages to
    string                  m_ipAddress;            ///< stores the Ip Address used for the Network communications
    string                  m_spoutName;			///<stores the name of the spout pipe name
    api_settings            m_weatherSettings;      ///<stores the weather api's settings
    api_settings            m_nasaSettings;         ///<stores the nasa's settings
    api_settings            m_surfSettings;         ///<stores the surf's settings
    
};


