/*
 *  ApiManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */


#include "ApiManager.h"
#include "AppManager.h"
#include "ofxJSON.h"


ApiManager::ApiManager(): Manager()
{
    //Intentionally left empty
}

ApiManager::~ApiManager()
{
    ofLogNotice() << "ApiManager::destructor";
    ofUnregisterURLNotification(this);
}


//--------------------------------------------------------------

void ApiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    this->setupApis();
    this->setupTimers();
    
    ofRegisterURLNotification(this);
    
    ofLogNotice() <<"ApiManager::initialized" ;
}

void ApiManager::setupApis()
{
    this->setupWeatherApi();
    this->setupNasaApi();
}

void ApiManager::setupTimers()
{
    this->setupWeatherTimer();
    this->setupNasaTimer();
   
}

void ApiManager::setupWeatherTimer()
{
    auto weatherSettings = AppManager::getInstance().getSettingsManager().getWeatherSettings();
    
    m_weatherTimer.setup( weatherSettings.request_time*1000 );
    
    m_weatherTimer.start( false ) ;
    ofAddListener( m_weatherTimer.TIMER_COMPLETE , this, &ApiManager::weatherTimerCompleteHandler ) ;
}

void ApiManager::setupNasaTimer()
{
    auto nasaSettings = AppManager::getInstance().getSettingsManager().getNasaSettings();
    
    m_nasaTimer.setup( nasaSettings.request_time*1000 );
    
    m_nasaTimer.start( false ) ;
    ofAddListener( m_nasaTimer.TIMER_COMPLETE , this, &ApiManager::nasaTimerCompleteHandler ) ;
}


void ApiManager::setupWeatherApi()
{
    auto weatherSettings = AppManager::getInstance().getSettingsManager().getWeatherSettings();
    
    //m_weatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=";
    m_weatherUrl = weatherSettings.url;
    m_weatherUrl+="lat=";
    m_weatherUrl+=ofToString(weatherSettings.lat);
    m_weatherUrl+="&lon=";
    m_weatherUrl+=ofToString(weatherSettings.lon);
    //m_weatherUrl+=weatherSettings.city;
    m_weatherUrl+="&units=";
    m_weatherUrl+=weatherSettings.units;
    m_weatherUrl+="&appid=";
    m_weatherUrl+=weatherSettings.key;
    m_weatherUrl+="&mode=xml";
    
    ofLogNotice() <<"ApiManager::setupWeatherApi << weather url = " <<  m_weatherUrl;
    
    ofLoadURLAsync(m_weatherUrl, "weather");
    
    this->initializeNasaImage();
}

void ApiManager::initializeNasaImage()
{
    auto texture = AppManager::getInstance().getResourceManager().getTexture("Apod");
    ofPixels pixels;
    texture->readToPixels(pixels);
    
    m_defaultImage.setFromPixels(pixels);
    m_nasaImage = m_defaultImage;
}

void ApiManager::setupNasaApi()
{
    auto nasaSettings = AppManager::getInstance().getSettingsManager().getNasaSettings();
    
    m_nasaUrl = nasaSettings.url;
    m_nasaUrl += nasaSettings.key;
    
    ofLogNotice() <<"ApiManager::setupNasaApi << weather url = " <<  m_weatherUrl;
    
    ofLoadURLAsync(m_nasaUrl, "nasa");
}


void ApiManager::update()
{
    this->updateTimers();
}

void ApiManager::updateTimers()
{
    m_weatherTimer.update();
}

void ApiManager::urlResponse(ofHttpResponse & response)
{
   // ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
    
    if(response.status==200)
    {
        if(response.request.name == "weather")
        {
            this->parseWeather(response.data);
            AppManager::getInstance().getGuiManager().onWeatherChange(m_weatherConditions);
        }
        
        else if(response.request.name == "nasa")
        {
            this->parseNasa(response.data);
        }
        
        else if(response.request.name == "nasa_image")
        {
            ofLogNotice() <<"ApiManager::urlResponse -> NASA IMAGE ";
            m_nasaImage.load(response.data);
        }
    }
}

void ApiManager::parseNasa(string response)
{
    //std::cout<< file << std::endl;
    
    //ofLogNotice() <<"ApiManager::parseNasa << file: \n" << file;
    
    ofxJSONElement json(response);
    
    m_nasaImage.clear();
    string url = json["url"].asString();
    
    ofFile file(url);
    string ext = file.getExtension();
    ofLogNotice() <<"ApiManager::parseNasa << url = " << json["url"] << ", extension -> " <<  ext;
   
    if(ext == "jpg" || ext == "png" ){
         ofLoadURLAsync(url,"nasa_image");
    }
    else{
        m_nasaImage = m_defaultImage;
    }
    
   
    
}

void ApiManager::parseWeather(string xml)
{
    ofXml weatherXml;
    weatherXml.loadFromBuffer( xml ); // now get the buffer as a string and make XML
    
    weatherXml.setTo("//");
    string path = "//current/temperature";
    weatherXml.setTo(path);
    auto attributes = weatherXml.getAttributes();
    m_weatherConditions.temp = ofToFloat(attributes["value"]);
    
    path = "//current/humidity";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.humidity = ofToFloat(attributes["value"]);
    
    path = "//current/wind/speed";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.windSpeed = ofToFloat(attributes["value"]);
    
    path = "//current/wind/direction";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.windDirection = ofToFloat(attributes["value"]);
    
    path = "//current/clouds";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.clouds = ofToFloat(attributes["value"]);
    
    path = "//current/precipitation";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.precipitationValue = ofToFloat(attributes["value"]);
    m_weatherConditions.precipitationMode = attributes["mode"];
    if(attributes["mode"] == "no"){
        m_weatherConditions.precipitationValue = 0;
    }
    
    path = "//current/city/sun";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.sunrise = this->parseTime(attributes["rise"]);
    m_weatherConditions.sunset = this->parseTime(attributes["set"]);
    
    path = "//current/city";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.city = attributes["name"];
    
    ofLogNotice() <<"ApiManager::parseWeather << parseWeather -> city = " << m_weatherConditions.city <<", temp = " <<  m_weatherConditions.temp
    << ", humidity = " << m_weatherConditions.humidity
    << ", wind speed = " << m_weatherConditions.windSpeed << ", wind direction = " << m_weatherConditions.windDirection
    << ", clouds = " << m_weatherConditions.clouds
    << ", precipitation mode = " << m_weatherConditions.precipitationMode  << ", precipitation value = " << m_weatherConditions.precipitationValue
    << ", sunrise = " << m_weatherConditions.sunrise  << ", sunset = " << m_weatherConditions.sunset ;
    
}

void ApiManager::weatherTimerCompleteHandler( int &args )
{
    //ofLogNotice() <<"ApiManager::weatherTimerCompleteHandler";
    m_weatherTimer.start(false);
    ofLoadURLAsync(m_weatherUrl, "weather");
}

void ApiManager::nasaTimerCompleteHandler( int &args )
{
    m_nasaTimer.start(false);
    ofLoadURLAsync(m_nasaUrl, "nasa");
}

float ApiManager::parseTime(string timeString)
{
    auto split_string = ofSplitString(timeString, "T");
    
    if(split_string.size()>1){
        split_string = ofSplitString(split_string[1], ":");
    }
    
    float time = 0;
    
    if(split_string.size()>0){
        time = time + 10000* ofToFloat(split_string[0]);
    }
    
    if(split_string.size()>1){
        time = time + 100*ofToFloat(split_string[2]);
    }
    
    if(split_string.size()>2){
        time = time +  ofToFloat(split_string[1]);
    }
    
    return time;
}








