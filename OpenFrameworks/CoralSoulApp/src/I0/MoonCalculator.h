/*
 *  MoonCalculator.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 12/12/17.
 *
 */



#pragma once

#include "ofMain.h"
#include "Manager.h"

//========================== class MoonCalculator =======================================
//==============================================================================
/** \class MoonCalculator MoonCalculator.h
 *	\brief class calculating the moon phase
 *	\details It reads all the keyboard events and create the proper application events
 */


class MoonCalculator
{

public:
    //! Constructor
    MoonCalculator();

    //! Destructor
    virtual ~MoonCalculator();

    double  getCurrentMoonPhase();
    
    int     getIp();
    
private:
    
    double moon_position(double j, double ls);
    
    double sun_position(double j);
    
    double Julian(int year,int month,double day);

private:
    
    int m_ip;

};

