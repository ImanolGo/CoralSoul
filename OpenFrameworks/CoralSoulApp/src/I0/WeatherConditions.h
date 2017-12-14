/*
 *  WeatherConditions.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 13/12/17.
 *
 */



#pragma once

#include "ofMain.h"
#include "MoonCalculator.h"

//===================+==== class WeatherConditions =============================
//==============================================================================
/** \class WeatherConditions WeatherConditions.h
 *	\brief class holding the weather conditions
 *	\details It reads weather conditions and provides serveral conbinient functions to retrieve the data
 */


class WeatherConditions
{

public:
    //! Constructor
    WeatherConditions();

    //! Destructor
    virtual ~WeatherConditions();

    string getTemperature() const { return ofToString(m_temp) + "°C";}
    string getHumidity() const { return ofToString(m_humidity) + "%";}
    string getWindSpeed()  const {return ofToString(m_windSpeed) + " kmh";}
    string getWindDirection() const { return ofToString(m_windDirection) + "°";}
    string getCloudiness() const { return ofToString(m_clouds);}
    string getSunrise() const {return m_sunrise;}
    string getSunset()  const {return m_sunset;}
    string getSunPosition() const { return ofToString(100*m_sunPosition) + "%";}
    string getMoonPhase() const { return ofToString(100*m_moonPhase) + "%";}
    string getCity() const { return m_city;}
    string getSwellHeight() const { return ofToString(m_swellHeight) + " m";}
    string getSwellPeriod()  const{ return ofToString(m_swellPeriod) + " s";}
    string getPrecipitation()  const{ return ofToString(m_precipitationValue) + " mm";}
    
    
    float getTemperatureNorm();
    float getCloudinessNorm();
    float getPrecipitationNorm();
    float getHumidityNorm();
    float getWindSpeedNorm();
    float getWindDirectionNorm();
    float getSunPositionNorm();
    float getMoonPhaseNorm();
    float getSwellHeightNorm();
    float getSwellPeriodNorm();
    
    int getMoonPhaseInt(){ return m_moonCalculator.getMoonPhaseInt();}
    
    bool isDayTime();
    
    string getFormatTime(string timeString);
    
    float calculatePosition();
    
    float getCurrentMoonPhase(){return (float) m_moonCalculator.getCurrentMoonPhase();}
    
private:
    
    void setupPatameters();
    float parseTime(string timeString);
    
   
    
public:
    
    ofParameter<float>  m_temp;
    ofParameter<float>  m_humidity;
    ofParameter<float>  m_windSpeed;
    ofParameter<float>  m_windDirection;
    string m_sunrise{"07:00:00"};
    string m_sunset{"20:00:00"};
    ofParameter<float>  m_sunPosition;
    ofParameter<float>  m_moonPhase;
    ofParameter<float>  m_clouds;
    string  m_precipitationMode;
    ofParameter<float>   m_precipitationValue;
    string m_city;
    
    ofParameter<float>  m_swellHeight;
    ofParameter<float>  m_swellPeriod;
    
    
private:
    
    
    MoonCalculator  m_moonCalculator;

};

