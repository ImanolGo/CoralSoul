/*
 *  SettingsManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#include "ofMain.h"

#include "SettingsManager.h"


const string SettingsManager::APPLICATION_SETTINGS_FILE_NAME = "xmls/ApplicationSettings.xml";


SettingsManager::SettingsManager(): Manager(), m_appHeight(0.0), m_appWidth(0.0)
{
    //Intentionally left empty
}


SettingsManager::~SettingsManager()
{
    ofLogNotice() <<"SettingsManager::Destructor" ;
}


void SettingsManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"SettingsManager::initialized" ;
    
    Manager::setup();
    
    if(this->loadSettingsFile()){
        this->loadAllSettings();
    }
}

void SettingsManager::loadAllSettings()
{
    this->setWindowProperties();
    this->setDebugProperties();
    this->setNetworkProperties();
    this->setApiProperties();
    this->loadTextureSettings();
    this->loadVideoSettings();
    this->loadColors();
    this->loadModelSettings();
}

bool SettingsManager::loadSettingsFile()
{
    
    if(!m_xml.load(APPLICATION_SETTINGS_FILE_NAME)){
        ofLogNotice() <<"SettingsManager::loadSettingsFile-> unable to load file: " << APPLICATION_SETTINGS_FILE_NAME ;
        return false;
    }
    
    ofLogNotice() <<"SettingsManager::loadSettingsFile->  successfully loaded " << APPLICATION_SETTINGS_FILE_NAME ;
    return true;
}

void SettingsManager::setDebugProperties()
{
    
    //m_xml.setTo("//");
    
    string path = "//of_settings/debug";
    auto xml = m_xml.findFirst(path);
    if(xml) {
        typedef   std::map<string, string>   AttributesMap;
      //  AttributesMap attributes = m_xml.getAttributes();
        
        bool showCursor = xml.getAttribute("showCursor").getBoolValue();
        
        if(showCursor){
            ofShowCursor();
        }
        else{
            ofHideCursor();
        }
        
        
        
        bool setVerbose = xml.getAttribute("setVerbose").getBoolValue();
        if(setVerbose){
            ofSetLogLevel(OF_LOG_VERBOSE);
        }
        else{
            ofSetLogLevel(OF_LOG_NOTICE);
        }
        
        
        m_sceneTimer =  xml.getAttribute("sceneTimer").getFloatValue();
        
        ofLogNotice() <<"SettingsManager::setDebugProperties->  successfully loaded the OF general settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setOFProperties->  path not found: " << path ;
}

void SettingsManager::setWindowProperties()
{
   // m_xml.setTo("//");
   
    string path = "//of_settings/window";
    auto xml = m_xml.findFirst(path);
    if(xml)
    {
        //m_xml.setTo(windowPath);
       // typedef   std::map<string, string>   AttributesMap;
       // AttributesMap attributes = m_xml.getAttributes();
        string title = xml.getAttribute("title").getValue();
        m_appWidth = xml.getAttribute("width").getIntValue();
        m_appHeight = xml.getAttribute("height").getIntValue();
       
        
        int x = xml.getAttribute("x").getIntValue();
        int y = xml.getAttribute("y").getIntValue();
        bool fullscreen = xml.getAttribute("fullscreen").getBoolValue();
        
        ofSetFullscreen(fullscreen);
        ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
        if(!fullscreen){
            ofSetWindowPosition(x,y);
        }
        ofSetWindowTitle(title);
        
        ofLogNotice() <<"SettingsManager::setWindowProperties->  successfully loaded the window settings" ;
        ofLogNotice() <<"SettingsManager::setWindowProperties->  title = "<< title<<", width = " << m_appWidth <<", height = "
        <<m_appHeight <<", x = "<<x<<", y = "<<y;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setWindowProperties->  path not found: " << path ;
}

void SettingsManager::setApiProperties()
{
    string path = "//api/weather";
    auto xml = m_xml.findFirst(path);
    if(xml) {
        
        m_weatherSettings.url = xml.getAttribute("url").getValue();
        m_weatherSettings.key = xml.getAttribute("key").getValue();
        m_weatherSettings.city = xml.getAttribute("city").getValue();
        m_weatherSettings.units = xml.getAttribute("units").getValue();
        m_weatherSettings.lat = xml.getAttribute("lat").getFloatValue();
        m_weatherSettings.lon = xml.getAttribute("lon").getFloatValue();
        m_weatherSettings.request_time = xml.getAttribute("request_time").getFloatValue();
        
        ofLogNotice() <<"SettingsManager::setApiProperties->  successfully loaded the weather settings" ;
        ofLogNotice() <<"SettingsManager::setApiProperties->  url = " << m_weatherSettings.url <<", city = "<< m_weatherSettings.city <<", units = " << m_weatherSettings.units <<", lat = "
        <<m_weatherSettings.lat <<", lon = "<<m_weatherSettings.lon<<", request time = "<<m_weatherSettings.request_time<<", key = "<<m_weatherSettings.key;
    }
    else{
        ofLogNotice() <<"SettingsManager::setApiProperties->  path not found: " << path ;
    }
    
    
    
    path = "//api/nasa";
    xml = m_xml.findFirst(path);
    if(xml) {
       
        m_nasaSettings.url = xml.getAttribute("url").getValue();
        m_nasaSettings.key = xml.getAttribute("key").getValue();
        m_nasaSettings.request_time = xml.getAttribute("request_time").getFloatValue();
        
        ofLogNotice() <<"SettingsManager::setApiProperties->  successfully loaded the nasa settings" ;
        ofLogNotice() <<"SettingsManager::setApiProperties->  url = " << m_nasaSettings.url <<", request time = "<<m_weatherSettings.request_time<<", key = "<<m_weatherSettings.key;
    }
    else{
        ofLogNotice() <<"SettingsManager::setApiProperties->  path not found: " << path ;
    }
    
    
    
    path = "//api/surf";
    xml = m_xml.findFirst(path);
    if(xml) {
      
        m_surfSettings.city = xml.getAttribute("ciry").getValue();
        m_surfSettings.key = xml.getAttribute("key").getValue();
        m_surfSettings.units = xml.getAttribute("units").getValue();
        m_surfSettings.url = xml.getAttribute("url").getValue();
        m_surfSettings.id =xml.getAttribute("id").getValue();
        m_surfSettings.request_time = xml.getAttribute("request_time").getFloatValue();
        
        ofLogNotice() <<"SettingsManager::setApiProperties->  successfully loaded the surf settings" ;
        ofLogNotice() <<"SettingsManager::setApiProperties->  name = " <<  m_surfSettings.city  <<", url = " << m_surfSettings.url <<", request time = "<<m_surfSettings.request_time<<", spotId = "<<m_surfSettings.id;
    }
    else{
        ofLogNotice() <<"SettingsManager::setApiProperties->  path not found: " << path ;
    }
    
    
}

void SettingsManager::setNetworkProperties()
{
    string path = "//of_settings/network";
    auto xml = m_xml.findFirst(path);
    if(xml) {

        m_portOscReceive  =  xml.getAttribute("portOscReceive").getIntValue();
        m_portOscSend  = xml.getAttribute("portOscSend").getIntValue();
        m_ipAddress  =  xml.getAttribute("ipAddress").getValue();
        m_spoutName = xml.getAttribute("spout").getValue();
        
        ofLogNotice() <<"SettingsManager::setNetworkProperties->  successfully loaded the network settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setNetworkProperties->  path not found: " << path ;
}

void SettingsManager::loadColors()
{
    string path = "//colors/color";
    auto colorsXml = m_xml.find(path);
    if(!colorsXml.empty()) {
        
        for(auto & colorXml: colorsXml)
        {
            int r = colorXml.getAttribute("r").getIntValue();
            int g = colorXml.getAttribute("g").getIntValue();
            int b = colorXml.getAttribute("b").getIntValue();
            int a = colorXml.getAttribute("a").getIntValue();
            string name =  colorXml.getAttribute("name").getValue();;
            
            auto color = ofPtr<ofColor> (new ofColor(r,g,b,a));
            m_colors[name] = color;
            
            ofLogNotice() <<"SettingsManager::loadColors->  color = " << name <<", r = " << r
            <<", g = "<< g << ", b = " << b << ", a = " << a ;
        }
                
        
        ofLogNotice() <<"SettingsManager::loadColors->  successfully loaded the applications colors" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadColors->  path not found: " << path ;
}

void SettingsManager::loadTextureSettings()
{
    string path = "//textures/texture";
    auto texturesXml = m_xml.find(path);
    if(!texturesXml.empty()) {
        
        for(auto & textureXml: texturesXml)
        {
            string path =  textureXml.getAttribute("path").getValue();
            string name =  textureXml.getAttribute("name").getValue();
            
            m_texturesPath[name] = path;
            
        
            ofLogNotice() <<"SettingsManager::loadTextureSettings->  texture = " << name
            <<", path = "<< path;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadTextureSettings->  successfully loaded the resource settings" ;
        return;
    }
    
    
    ofLogNotice() <<"SettingsManager::loadTextureSettings->  path not found: " << path ;
}

ofColor SettingsManager::getColor(const string& colorName)
{
    ofColor color;
    if(m_colors.find(colorName)!= m_colors.end()){
        color.setHex(m_colors[colorName]->getHex());
    }
    
    return color;
}



void SettingsManager::loadVideoSettings()
{
    string path = "//videos/video";
    auto videosXml = m_xml.find(path);
    if(!videosXml.empty()) {
        
        for(auto & videoXml: videosXml)
        {
            string path =  videoXml.getAttribute("path").getValue();
            string name =  videoXml.getAttribute("name").getValue();
            
            m_videoResourcesPath[name] = path;
            
            
            ofLogNotice() <<"SettingsManager::loadVideoSettings->  video = " << name
            <<", path = "<< path;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadVideoSettings->  successfully loaded the video settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadVideoSettings->  path not found: " << path ;
}

void SettingsManager::loadModelSettings()
{
    
    string path = "//models/model";
    auto modelsXml = m_xml.find(path);
    if(!modelsXml.empty()) {
        
        for(auto & modelXml: modelsXml)
        {
            string path =  modelXml.getAttribute("path").getValue();
            string name =  modelXml.getAttribute("name").getValue();
            
            m_modelResourcesPath[name] = path;
            
            
            ofLogNotice() <<"SettingsManager::loadModelSettings->  video = " << name
            <<", path = "<< path;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadModelSettings->  successfully loaded the model settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadModelSettings->  path not found: " << path ;
}





