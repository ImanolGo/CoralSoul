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
            m_weatherTimer.start(false);
            this->parseWeather(response.data);
            AppManager::getInstance().getGuiManager().onWeatherChange();
            AppManager::getInstance().getOscManager ().sendOscWeather();
        }
        
//        else if(response.request.name == "nasa")
//        {
//            this->parseNasa(response.data);
//        }
//
//        else if(response.request.name == "nasa_image")
//        {
//            ofLogNotice() <<"ApiManager::urlResponse -> NASA IMAGE ";
//            m_nasaImage.load(response.data);
//        }
        
        else if(response.request.name == "surf")
        {
            m_surfTimer.start(false);
            this->parsesurf(response.data);
            AppManager::getInstance().getGuiManager().onWeatherChange();
            AppManager::getInstance().getOscManager().sendOscWeather();
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

    m_weatherConditions.m_swellHeight = json[0]["swell"]["components"]["combined"]["height"].asFloat();
    m_weatherConditions.m_swellPeriod = json[0]["swell"]["components"]["combined"]["period"].asFloat();

    ofLogNotice() <<"ApiManager::parsesurf << swell height = " <<  m_weatherConditions.m_swellHeight << ", swell period -> " <<m_weatherConditions.m_swellPeriod;
}


void ApiManager::parseWeather(string xml)
{
    ofXml weatherXml;
    
    if(!weatherXml.parse( xml )){
        ofLogNotice() <<"ApiManager::parseWeather << Unable to parse weather: " << xml;
        return;
    }
    
     //ofLogNotice() <<"ApiManager::parseWeather << Parsing weather " << xml;
    
   // weatherXml.parse( xml ); // now get the buffer as a string and make XML
    
    string path = "//current/temperature";
    auto xmlChild = weatherXml.findFirst(path);
    m_weatherConditions.m_temp = xmlChild.getAttribute("value").getFloatValue();
    
    path = "//current/humidity";
    xmlChild =weatherXml.findFirst(path);
    m_weatherConditions.m_humidity = xmlChild.getAttribute("value").getFloatValue();
    
    path = "//current/wind/speed";
    xmlChild =weatherXml.findFirst(path);
    m_weatherConditions.m_windSpeed =  xmlChild.getAttribute("value").getFloatValue();
    
    path = "//current/wind/direction";
    xmlChild =weatherXml.findFirst(path);
    m_weatherConditions.m_windDirection =  xmlChild.getAttribute("value").getFloatValue();
    
    path = "//current/clouds";
    xmlChild =weatherXml.findFirst(path);
    m_weatherConditions.m_clouds =  xmlChild.getAttribute("value").getFloatValue();
    
    path = "//current/precipitation";
    xmlChild = weatherXml.findFirst(path);
    m_weatherConditions.m_precipitationValue =  xmlChild.getAttribute("value").getFloatValue();
    m_weatherConditions.m_precipitationMode =  xmlChild.getAttribute("mode").getValue();
    if(m_weatherConditions.m_precipitationMode  == "no"){
        m_weatherConditions.m_precipitationValue = 0;
    }
    
    path = "//current/city/sun";
    xmlChild =weatherXml.findFirst(path);
    m_weatherConditions.m_sunrise = m_weatherConditions.getFormatTime(xmlChild.getAttribute("rise").getValue());
    m_weatherConditions.m_sunset = m_weatherConditions.getFormatTime(xmlChild.getAttribute("set").getValue());
    
    path = "//current/city";
    xmlChild = weatherXml.findFirst(path);
    m_weatherConditions.m_city = xmlChild.getAttribute("name").getValue();
    
    m_weatherConditions.m_moonPhase = m_weatherConditions.getCurrentMoonPhase();
    
    m_weatherConditions.m_sunPosition = m_weatherConditions.calculatePosition();
    
    ofLogNotice() <<"ApiManager::parseWeather << parseWeather -> city = " << m_weatherConditions.m_city <<", temp = " <<  m_weatherConditions.m_temp
    << ", humidity = " << m_weatherConditions.m_humidity
    << ", wind speed = " << m_weatherConditions.m_windSpeed << ", wind direction = " << m_weatherConditions.m_windDirection
    << ", clouds = " << m_weatherConditions.m_clouds
    << ", precipitation mode = " << m_weatherConditions.m_precipitationMode  << ", precipitation value = " << m_weatherConditions.m_precipitationValue
    << ", sunrise = " << m_weatherConditions.m_sunrise  << ", sunset = " << m_weatherConditions.m_sunset
    << ", moon phase  = " << m_weatherConditions.getMoonPhaseInt() << ", sun position = " << m_weatherConditions.m_sunPosition;
    
}

void ApiManager::weatherTimerCompleteHandler( int &args )
{
    //ofLogNotice() <<"ApiManager::weatherTimerCompleteHandler";
    //m_weatherTimer.start(false);
    ofLoadURLAsync(m_weatherUrl, "weather");
}

void ApiManager::nasaTimerCompleteHandler( int &args )
{
   // m_nasaTimer.start(false);
    ofLoadURLAsync(m_nasaUrl, "nasa");
}

void ApiManager::surfTimerCompleteHandler( int &args )
{
   // m_surfTimer.start(false);
    ofLoadURLAsync(m_surfUrl, "surf");
}







