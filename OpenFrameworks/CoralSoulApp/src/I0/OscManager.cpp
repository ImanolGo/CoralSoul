/*
 *  OscManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 30/10/17.
 *
 */


#include "OscManager.h"
#include "SettingsManager.h"
#include "AppManager.h"



const string OscManager::OSC_PARENT_ADDRESS = "/CoralSoul/Generative";

OscManager::OscManager(): Manager()
{
    //Intentionally left empty
}

OscManager::~OscManager()
{
   ofLogNotice() << "OscManager::destructor";
}


//--------------------------------------------------------------

void OscManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupOscReceiver();
    this->setupOscSender();
    this->readSenderInformation();
    
    ofLogNotice() <<"OscManager::initialized" ;
}

void OscManager::readSenderInformation()
{
    ofXml xml;
    string fileName = SettingsManager::APPLICATION_SETTINGS_FILE_NAME;
    if(!xml.load( fileName)){
        ofLogNotice() <<"OscManager::readSenderInformation-> unable to load file: " << fileName;
        return;
    }
    
    ofLogNotice() <<"OscManager::readSenderInformation->  successfully loaded " << fileName ;
    
    
    string path = "//networks";
    if(xml.exists(path)) {
        
        typedef   std::map<string, string>   AttributesMap;
        AttributesMap attributes;
        
        path = "//networks/network[0]";
        xml.setTo(path);
        do {
            
            attributes = xml.getAttributes();
            ofxOscSender  oscSender;
            oscSender.setup(attributes["ipAddress"], ofToInt(attributes["port"]));
            m_oscSenders[attributes["name"]] = oscSender ;
            
            ofLogNotice() <<"OscManager::readSenderInformation->  name = " << attributes["name"]
            <<", ipAddress = "<< attributes["ipAddress"]  << ", port = " << attributes["port"];
        }
        while(xml.setToSibling()); // go to the next texture
        
        return;
    }
    
    ofLogNotice() <<"OscManager::readSenderInformation->  path not found: " << path;
}

void OscManager::setupOscReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getOscPortReceive();
    ofLogNotice() <<"OscManager::setupOscReceiver -> listening for osc messages on port  " << portReceive;
    m_oscReceiver.setup(portReceive);
}

void OscManager::setupOscSender()
{
    int portSend = AppManager::getInstance().getSettingsManager().getOscPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();
    m_oscSender.setup(host, portSend);
    ofLogNotice() <<"OscManager::setupOscSender -> open osc connection " << host << ":" << portSend;
    
}


void OscManager::update()
{
    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(m);
        
        if(m.getAddress() == OSC_PARENT_ADDRESS + "/Scene")
        {
            string sceneName = m.getArgAsString(0);
            AppManager::getInstance().getGuiManager().onSceneChange(sceneName);
        }
        
        
        ofLogNotice() <<"OscManager::received -> " << this->getMessageAsString(m);
    }
}


void OscManager::sendFloatMessage(float value, string& name)
{
    string message = OSC_PARENT_ADDRESS + "/" + name;
    ofxOscMessage m;
    m.setAddress(message);
    m.addFloatArg(value);
    //m_oscSender.sendMessage(m);
    
    for (auto& oscSender : m_oscSenders) {
        oscSender.second.sendMessage(m);
    }
}

void OscManager::sendStringMessage(string value, string& name)
{
    string message = OSC_PARENT_ADDRESS + "/" + name;
    ofxOscMessage m;
    m.setAddress(message);
    m.addStringArg(value);
    //m_oscSender.sendMessage(m);
    
    for (auto& oscSender : m_oscSenders) {
        oscSender.second.sendMessage(m);
    }
    
}

void OscManager::sendMessage(ofxOscMessage& message)
{
    //m_oscSender.sendMessage(message);
    for (auto& oscSender : m_oscSenders) {
        oscSender.second.sendMessage(message);
    }
    message.clear();
}


string OscManager::getMessageAsString(const ofxOscMessage& m) const
{
    string msg_string;
    msg_string = m.getAddress();
    for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += " ";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += m.getArgAsString(i);
        }
        else{
            msg_string += "unknown";
        }
    }
    
    return msg_string;
}


void OscManager::sendOscWeather()
{
    auto weather = AppManager::getInstance().getApiManager().getCurrentWeather();
    
    string message = "Weather/Temperature";
    this->sendStringMessage(weather.getTemperature(), message);
    
    message = "Weather/TemperatureNorm";
    this->sendFloatMessage(weather.getTemperatureNorm(), message);
     
    message = "Weather/Humidity";
    this->sendStringMessage(weather.getHumidity(), message);
    
    message = "Weather/HumidityNorm";
    this->sendFloatMessage(weather.getHumidityNorm(), message);
    
    message = "Weather/WindSpeed";
    this->sendStringMessage(weather.getWindSpeed(), message);
    
    message = "Weather/WindSpeedNorm";
    this->sendFloatMessage(weather.getWindSpeedNorm(), message);
    
    message = "Weather/WindDirection";
    this->sendStringMessage(weather.getWindDirection(), message);
    
    message = "Weather/WindDirectionNorm";
    this->sendFloatMessage(weather.getWindDirectionNorm(), message);
    
    message = "Weather/Cloudiness";
    this->sendStringMessage(weather.getCloudiness(), message);
    
    message = "Weather/CloudinessNorm";
    this->sendFloatMessage(weather.getCloudinessNorm(), message);
    
    message = "Weather/Precipitation";
    this->sendStringMessage(weather.getPrecipitation(), message);
    
    message = "Weather/PrecipitationNorm";
    this->sendFloatMessage(weather.getPrecipitationNorm(), message);
    
    message = "Weather/SwellHeight";
    this->sendStringMessage(weather.getSwellHeight(), message);
    
    message = "Weather/SwellHeightNorm";
    this->sendFloatMessage(weather.getSwellHeightNorm(), message);
    
    message = "Weather/SwellPeriod";
    this->sendStringMessage(weather.getSwellPeriod(), message);
    
    message = "Weather/SwellPeriodNorm";
    this->sendFloatMessage(weather.getSwellPeriodNorm(), message);
    
    message = "Weather/MoonPhase";
    this->sendStringMessage(weather.getMoonPhase(), message);
    
    message = "Weather/MoonPhaseNorm";
    this->sendFloatMessage(weather.getMoonPhaseNorm(), message);
    
    message = "Weather/SunPosition";
    this->sendStringMessage(weather.getSunPosition(), message);
    
    message = "Weather/SunPositionNorm";
    this->sendFloatMessage(weather.getSunPositionNorm(), message);
    
    message = "Weather/Sunrise";
    this->sendStringMessage(weather.getSunrise(), message);
    
    message = "Weather/Sunset";
    this->sendStringMessage(weather.getSunset(), message);
}







