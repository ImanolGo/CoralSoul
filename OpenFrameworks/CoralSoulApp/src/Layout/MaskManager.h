/*
 *  MaskManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 04/11/17.
 *
 */


#pragma once

#include "Manager.h"
#include "ImageVisual.h"

//========================== class MaskManager ==============================
//============================================================================
/** \class MaskManager MaskManager.h
 *	\brief Class managing the masking of the visuals
 *	\details It loads all the masks and it provides an interface for visuals to be rendered under the mask
 */

class MaskManager: public Manager
{
public:
    
    //! Constructor
    MaskManager();
    
    //! Destructor
    ~MaskManager();
    
    //! Set up the mask manager
    void setup();
    
    //! Update the mask manager
    void update();
    
    //! start a new mask
    void allocate(string& maskName, int width, int height);
    
    //! Begin fbo for a specific mask
    void begin(string& maskName);
    
    //! End fbo for a specific mask
    void end(string& maskName);
    
    //! Draw the maskee
    void draw(string& maskName);
    
    //! Draw the mask
    void drawMask(string& maskName);
    
    //! Begin mask
    void beginMask(string& maskName);
    
    //! End mask
    void endMask(string& maskName);
    
    void resizeMasks();

    ofTexture& getTexture(string& name);
    

    
private:
    
    //! Set-up the masks
    void setupShader();
    
    
private:
  
    typedef  map<string, shared_ptr<ofFbo> >  FboMap;        ///< defines a map of ofFbos
    typedef  vector<string>  MaskModeList;        ///< defines a vecotr of stings
    
    ofShader        m_maskShader;
    
    FboMap          m_masks;
    FboMap          m_masksees;

    bool            m_showMasks;
    ofTexture       m_defaultTexture;
    
};

//==========================================================================

