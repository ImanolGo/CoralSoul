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
    
    string path = "//networks/network";
    auto xmlNetworks = xml.find(path);
    if(!xmlNetworks.empty()) {
        
        for(auto & xmlNetwork: xmlNetworks)
        {
            ofxOscSender  oscSender;
            auto ipAddress =  xmlNetwork.getAttribute("ipAddress").getValue();
            auto port = xmlNetwork.getAttribute("port").getIntValue();
            auto name =  xmlNetwork.getAttribute("name").getValue();
            oscSender.setup(ipAddress, port);
            m_oscSenders[name] = oscSender ;
           
            ofLogNotice() <<"OscManager::readSenderInformation->  name = " << name
            <<", ipAddress = "<< ipAddress  << ", port = " <<port;
        }
        
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
        m_oscReceiver.getNextMessage(&m);
        
        if(m.getAddress() == OSC_PARENT_ADDRESS + "/Scene")
        {
            string sceneName = m.getArgAsString(0);
            AppManager::getInstance().getGuiManager().onSceneChange(sceneName);
        }
        
        if(m.getAddress() == OSC_PARENT_ADDRESS + "/EnableScene")
        {
            string sceneName = m.getArgAsString(0);
            bool sceneEnable = m.getArgAsInt(1)>0;
            AppManager::getInstance().getGuiManager().onEnableSceneChange(sceneName,sceneEnable);
        }
        
        
        ofLogNotice() <<"OscManager::received -> " << this->getMessageAsString(m);
    }

	this->sendOscAll();
    
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

void OscManager::sendOscAll()
{
	this->sendOscLive();
	this->sendOscIpad();
	this->sendOscResolume();
}

void OscManager::sendOscLive()
{
	auto weather = AppManager::getInstance().getApiManager().getCurrentWeather();

	string message = "Weather/TemperatureNorm";
	this->sendFloatMessage(weather.getTemperatureNorm(), message);

	message = "Weather/HumidityNorm";
	this->sendFloatMessage(weather.getHumidityNorm(), message);

	message = "Weather/WindSpeedNorm";
	this->sendFloatMessage(weather.getWindSpeedNorm(), message);

	message = "Weather/WindDirectionNorm";
	this->sendFloatMessage(weather.getWindDirectionNorm(), message);

	message = "Weather/CloudinessNorm";
	this->sendFloatMessage(weather.getCloudinessNorm(), message);

	message = "Weather/PrecipitationNorm";
	this->sendFloatMessage(weather.getPrecipitationNorm(), message);

	message = "Weather/SwellHeightNorm";
	this->sendFloatMessage(weather.getSwellHeightNorm(), message);

	message = "Weather/SwellPeriodNorm";
	this->sendFloatMessage(weather.getSwellPeriodNorm(), message);

	message = "Weather/MoonPhaseNorm";
	this->sendFloatMessage(weather.getMoonPhaseNorm(), message);

	message = "Weather/SunPositionNorm";
	this->sendFloatMessage(weather.getSunPositionNorm(), message);

}

void OscManager::sendOscResolume()
{
	auto fValue = AppManager::getInstance().getApiManager().getCurrentWeather().getSwellHeightNorm();
	fValue = ofMap(fValue, 0.0, 1.0, 0.02, 0.18, true);
	string address = "/layer3/video/effect3/param4/values";

	ofxOscMessage m;
	m.setAddress(address);
	m.addFloatArg(fValue);
	AppManager::getInstance().getOscManager().sendMessage(m);


	fValue = AppManager::getInstance().getApiManager().getCurrentWeather().getSwellPeriodNorm();
	fValue = ofMap(fValue, 0.0, 1.0, 0.00, 0.5, true);
	address = "/layer3/video/effect3/param5/values";
	m.setAddress(address);
	m.addFloatArg(fValue);
	AppManager::getInstance().getOscManager().sendMessage(m);


}

void OscManager::sendOscIpad()
{
	auto weather = AppManager::getInstance().getApiManager().getCurrentWeather();

	string message = "Weather/Temperature";
	this->sendStringMessage(weather.getTemperature(), message);

	message = "Weather/Humidity";
	this->sendStringMessage(weather.getHumidity(), message);

	message = "Weather/WindSpeed";
	this->sendStringMessage(weather.getWindSpeed(), message);

	message = "Weather/WindDirection";
	this->sendStringMessage(weather.getWindDirection(), message);

	message = "Weather/Cloudiness";
	this->sendStringMessage(weather.getCloudiness(), message);

	message = "Weather/Precipitation";
	this->sendStringMessage(weather.getPrecipitation(), message);

	message = "Weather/SwellHeight";
	this->sendStringMessage(weather.getSwellHeight(), message);

	message = "Weather/SwellPeriod";
	this->sendStringMessage(weather.getSwellPeriod(), message);

	message = "Weather/MoonPhase";
	this->sendStringMessage(weather.getMoonPhase(), message);

	message = "Weather/SunPosition";
	this->sendStringMessage(weather.getSunPosition(), message);

	message = "Weather/Sunrise";
	this->sendStringMessage(weather.getSunrise(), message);

	message = "Weather/Sunset";
	this->sendStringMessage(weather.getSunset(), message);
}



