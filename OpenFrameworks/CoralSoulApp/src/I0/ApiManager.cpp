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


ApiManager::ApiManager(): Manager(), m_isDayTime(true)
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
    //this->setupNasaApi();
    this->setupsurfApi();
}

void ApiManager::setupTimers()
{
    this->setupWeatherTimer();
    //this->setupNasaTimer();
    this->setupsurfTimer();
    
}

void ApiManager::setupWeatherTimer()
{
    auto weatherSettings = AppManager::getInstance().getSettingsManager().getWeatherSettings();
    
    m_weatherTimer.setup( weatherSettings.request_time*1000 );
    
    m_weatherTimer.start( false ) ;
    ofAddListener( m_weatherTimer.TIMER_COMPLETE , this, &ApiManager::weatherTimerCompleteHandler ) ;
}

void ApiManager::setupsurfTimer()
{
    auto surfSettings = AppManager::getInstance().getSettingsManager().getsurfSettings();
    
    m_surfTimer.setup( surfSettings.request_time*1000 );
    
    m_surfTimer.start( false ) ;
    ofAddListener( m_surfTimer.TIMER_COMPLETE , this, &ApiManager::surfTimerCompleteHandler ) ;
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
    
    ofLogNotice() <<"ApiManager::setupNasaApi << nasa url = " <<  m_nasaUrl;
    
    ofLoadURLAsync(m_nasaUrl, "nasa");
}

void ApiManager::setupsurfApi()
{
    auto surfSettings = AppManager::getInstance().getSettingsManager().getsurfSettings();
    
    m_surfUrl = surfSettings.url;
    m_surfUrl += surfSettings.key;
    m_surfUrl+="/forecast/?spot_id=";
    m_surfUrl+=surfSettings.id;
    m_surfUrl+="&fields=swell.components.combined.height,swell.components.combined.period";
    m_surfUrl+="&units=";
    m_surfUrl+=surfSettings.units;
    
    ofLogNotice() <<"ApiManager::setupsurfApi << surf url = " <<  m_surfUrl;
    
    ofLoadURLAsync(m_surfUrl, "surf");
}


void ApiManager::update()
{
    this->updateTimers();
}

void ApiManager::updateTimers()
{
    m_weatherTimer.update();
    //m_nasaTimer.update();
    m_surfTimer.update();
}

void ApiManager::urlResponse(ofHttpResponse & response)
{
    //ofLogNotice() <<"ApiManager::urlResponse -> " << response.request.name << ", " << response.status;
    

    if(response.status==200)
    {
        if(response.request.name == "weather")
        {
            this->parseWeather(response.data);
            this->checkDayNight();
            AppManager::getInstance().getGuiManager().onWeatherChange(m_weatherConditions);
            AppManager::getInstance().getOscManager ().sendOscWeather(m_weatherConditions);
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
        
        else if(response.request.name == "surf")
        {
            this->parsesurf(response.data);
            AppManager::getInstance().getGuiManager().onWeatherChange(m_weatherConditions);
            AppManager::getInstance().getOscManager().sendOscWeather(m_weatherConditions);
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

void ApiManager::parsesurf(string response)
{
    //ofLogNotice() << response;
    
    ofxJSONElement json(response);

    m_weatherConditions.swellHeight = json[0]["swell"]["components"]["combined"]["height"].asFloat();
    m_weatherConditions.swellPeriod = json[0]["swell"]["components"]["combined"]["period"].asFloat();

    ofLogNotice() <<"ApiManager::parsesurf << swell height = " <<  m_weatherConditions.swellHeight << ", swell period -> " <<m_weatherConditions.swellPeriod;
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
    m_weatherConditions.sunrise = this->getFormatTime(attributes["rise"]);
    m_weatherConditions.sunset = this->getFormatTime(attributes["set"]);
    
    path = "//current/city";
    weatherXml.setTo(path);
    attributes = weatherXml.getAttributes();
    m_weatherConditions.city = attributes["name"];
    
    m_weatherConditions.moonPhase = (float)m_moonCalculator.getCurrentMoonPhase();
    
    m_weatherConditions.sunPosition = this->getSunPosition();
    
    ofLogNotice() <<"ApiManager::parseWeather << parseWeather -> city = " << m_weatherConditions.city <<", temp = " <<  m_weatherConditions.temp
    << ", humidity = " << m_weatherConditions.humidity
    << ", wind speed = " << m_weatherConditions.windSpeed << ", wind direction = " << m_weatherConditions.windDirection
    << ", clouds = " << m_weatherConditions.clouds
    << ", precipitation mode = " << m_weatherConditions.precipitationMode  << ", precipitation value = " << m_weatherConditions.precipitationValue
    << ", sunrise = " << m_weatherConditions.sunrise  << ", sunset = " << m_weatherConditions.sunset
    << ", moon phase = " << m_weatherConditions.moonPhase << ", sun position = " << m_weatherConditions.sunPosition;
    
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

void ApiManager::surfTimerCompleteHandler( int &args )
{
    m_surfTimer.start(false);
    ofLoadURLAsync(m_surfUrl, "surf");
}

string ApiManager::getFormatTime(string timeString)
{
    auto split_string = ofSplitString(timeString, "T");
    
    if(split_string.size()>1){
        return split_string[1];
    }
}

float ApiManager::parseTime(string timeString)
{
    auto split_string = ofSplitString(timeString, ":");
    
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

void ApiManager::checkDayNight()
{
    float currentTime = 10000*ofGetHours() + 100*ofGetMinutes() + ofGetSeconds();
    float sunrise = this->parseTime(m_weatherConditions.sunrise );
    float sunset = this->parseTime(m_weatherConditions.sunset );
    ofLogNotice() <<"ApiManager::checkDayNight -> current time : "<< currentTime
    << ", sunrise = " << sunrise << ", sunset = " << sunset;
    if(currentTime> sunrise && currentTime< sunset){
        ofLogNotice() <<"ApiManager::checkDayNight -> It's day time";
        m_isDayTime = true;
    }
    else{
        ofLogNotice() <<"ApiManager::checkDayNight -> It's night time";
        m_isDayTime = false;
    }
}

float ApiManager::getSunPosition()
{
    float currentTime = 10000*ofGetHours() + 100*ofGetMinutes() + ofGetSeconds();
    float sunrise = this->parseTime(m_weatherConditions.sunrise );
    float sunset = this->parseTime(m_weatherConditions.sunset );
    
    return ofMap(currentTime, sunrise, sunset, 0.0,1.0, true);
}




