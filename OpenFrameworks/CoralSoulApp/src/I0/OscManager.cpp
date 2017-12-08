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


const string OscManager::OSC_PARENT_ADDRESS = "/CoralSoul";

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
    
    ofLogNotice() <<"OscManager::initialized" ;
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
    m_oscSender.sendMessage(m);
}

void OscManager::sendMessage(ofxOscMessage& message)
{
    m_oscSender.sendMessage(message);
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


void OscManager::sendOscWeather(const weather_conditions& weather)
{
    string message = "temperature";
    this->sendFloatMessage(weather.temp, message);
    
    message = "humidity";
    this->sendFloatMessage(weather.humidity, message);
    
    message = "windSpeed";
    this->sendFloatMessage(weather.windSpeed, message);
    
    message = "windDirection";
    this->sendFloatMessage(weather.windDirection, message);
    
    message = "clouds";
    this->sendFloatMessage(weather.clouds, message);
    
    message = "precipitation";
    this->sendFloatMessage(weather.precipitationValue, message);
    
    message = "swellHeight";
    this->sendFloatMessage(weather.swellHeight, message);
    
    message = "swellPeriod";
    this->sendFloatMessage(weather.swellPeriod, message);
}






