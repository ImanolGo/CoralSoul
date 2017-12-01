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


struct weather_conditions
{
    float temp{0.0};
    float humidity{0.0};
    float windSpeed{0.0};
    float windDirection{0.0};
    float sunrise{0.0};
    float sunset{0.0};
    float clouds{0.0};
    string  precipitationMode{"no"};
    float  precipitationValue{0.0};
    string city{""};
};

//========================== class ApiManager =======================================
//===================================================================================
/** \class ApiManager ApiManager.h
 *	\brief class for managing several APIs
 *	\details It reads from weather APIs, tide APIs, parses the informations and provides the
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
    
    const ofImage& getNasaImage() {return m_nasaImage;}
    
private:
    
    void setupApis();
    
    void setupWeatherApi();
    
    void setupNasaApi();
    
    void initializeNasaImage();
    
    void setupTimers();
    
    void setupWeatherTimer();
    
    void setupNasaTimer();
    
    void updateTimers();
    
    void parseWeather(string xml);
    
    void parseNasa(string file);
    
    float parseTime(string timeString);
    

private:
    
    ofxSimpleTimer          m_weatherTimer;
    ofxSimpleTimer          m_nasaTimer;
    string                  m_weatherUrl;
    string                  m_nasaUrl;
    weather_conditions      m_weatherConditions;
    ofImage                 m_nasaImage;

};

