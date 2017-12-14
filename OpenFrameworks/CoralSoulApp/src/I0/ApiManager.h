/*
 *  ApiManager.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 31/10/17.
 *
 */

#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxSimpleTimer.h"
#include "MoonCalculator.h"
#include "WeatherConditions.h"


struct api_settings
{
    float lat{0.0};
    float lon{0.0};
    float request_time{2.0};
    string city{"berlin"};
    string key{""};
    string units{"metric"};
    string url{""};
    string id{""};
};



//========================== class ApiManager =======================================
//===================================================================================
/** \class ApiManager ApiManager.h
 *    \brief class for managing several APIs
 *    \details It reads from weather APIs, tide APIs, parses the informations and provides the
 *    methods to read it
 */


class ApiManager: public Manager
{
    
public:
    //! Constructor
    ApiManager();
    
    //! Destructor
    virtual ~ApiManager();
    
    //! setups the manager
    void setup();
    
    //! updates the manager
    void update();
    
    void urlResponse(ofHttpResponse & response);
    
    void weatherTimerCompleteHandler( int &args ) ;
    
    void nasaTimerCompleteHandler( int &args ) ;
    
    void surfTimerCompleteHandler( int &args ) ;
    
    const ofImage& getNasaImage() {return m_nasaImage;}
    
    WeatherConditions& getCurrentWeather() {return m_weatherConditions;}
    
    void onWindSpeedChange(float& value){m_weatherConditions.m_windSpeed = value;}
    
    void onWindDirChange(float& value){m_weatherConditions.m_windDirection = value;}
    
    void onTemperatureChange(float& value){m_weatherConditions.m_temp = value;}
    
    void onHumidityChange(float& value){m_weatherConditions.m_humidity = value;}
    
    void onPrecipitationChange(float& value){m_weatherConditions.m_precipitationValue = value;}
    
    void onCloudsChange(float& value){m_weatherConditions.m_clouds = value;}
    
    void onMoonChange(float& value){m_weatherConditions.m_moonPhase = value;}
    
    void onSunChange(float& value){m_weatherConditions.m_sunPosition = value;}
    
    void onSwellHeightChange(float& value){m_weatherConditions.m_swellHeight = value;}
    
    void onSwellPeriodChange(float& value){m_weatherConditions.m_swellPeriod = value;}
    
    bool isDayTime() {return m_weatherConditions.isDayTime();}
    
private:
    
    void setupApis();
    
    void setupWeatherApi();
    
    void setupNasaApi();
    
    void setupsurfApi();
    
    void initializeNasaImage();
    
    void setupTimers();
    
    void setupWeatherTimer();
    
    void setupNasaTimer();
    
    void setupsurfTimer();
    
    void updateTimers();
    
    void parseWeather(string xml);
    
    void parseNasa(string response);
    
    void parsesurf(string response);
    

private:
    
    ofxSimpleTimer          m_weatherTimer;
    ofxSimpleTimer          m_nasaTimer;
    ofxSimpleTimer          m_surfTimer;
    string                  m_weatherUrl;
    string                  m_nasaUrl;
    string                  m_surfUrl;
    WeatherConditions       m_weatherConditions;
    ofImage                 m_nasaImage;
    ofImage                 m_defaultImage;
};

