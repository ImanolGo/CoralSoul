/*
 *  ModelManager.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#pragma once

#include "Manager.h"
#include "ofxAssimpModelLoader.h"

//========================== class ModelManager ==============================
//============================================================================
/** \class ModelManager ModelManager.h
 *	\brief Class managing the 3D models
 *	\details It reads and shows the model with its textures
 */

class ModelManager: public Manager
{
public:
    //! Constructor
    ModelManager();

    //! Destructor
    ~ModelManager();

    //! Set-up the layout
    void setup();
    
    //! Update the layout
    void update();
    
    //! Draw the layout
    void draw();
    
    const ofFbo & getMask() const {return m_fboMask;}
    
    const ofFbo & getModel() const {return m_fboModel;}
    
    const ofFbo & getWireframe() const {return m_fboWireframe;}
    
    void onCameraDistanceChange(float& value);
    
    void onCameraXChange(float& value);
    
    void onCameraYChange(float& value);
    
    void onCameraZChange(float& value);
    
    void onCameraFovChange(float& value);


private:

    void setupCamera();
    
    void setupFbos();
    
    void loadModel();
    
    void updateModel();
    
    void updateFbos();
    
    void drawModel();
    
    void drawMask();
    
    void drawWireframe();
    
    void bindTexture();
    
    void unbindTexture();
    

private:

    ofxAssimpModelLoader m_model;
    ofMesh               m_mesh;
    ofLight              m_light;
    ofEasyCam            m_cam;
    ofFbo                m_fboMask;
    ofFbo                m_fboModel;
    ofFbo                m_fboWireframe;

    ofImage img;
};

//==========================================================================


