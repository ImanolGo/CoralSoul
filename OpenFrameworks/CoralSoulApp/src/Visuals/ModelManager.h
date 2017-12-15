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
#include "BasicVisual.h"

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
    
    void drawModel(const ofFbo& tex);
    
    void drawWireframe(const ofTexture& texture);
    
    const ofFbo & getMask() const {return m_fboMask;}
    
    const ofFbo & getModel() const {return m_fboModel;}
    
    const ofFbo & getWireframe() const {return m_fboWireframe;}
    
    void onCameraDistanceChange(float& value);
    
    void onCameraXChange(float& value);
    
    void onCameraYChange(float& value);
    
    void onCameraZChange(float& value);
    
    void onCameraFovChange(float& value);
    
    void onPositionXChange(float& value);
    
    void onPositionYChange(float& value);
    
    void onPositionZChange(float& value);
    
    void onRotationXChange(float& value);
    
    void onRotationYChange(float& value);
    
    void onRotationZChange(float& value);

    void onLightXChange(float& value);
    
    void onLightYChange(float& value);
    
    void onLightZChange(float& value);
    
    void onLightColorChange(ofColor color);
    
    void onNoiseAmplitudeChange(float& value) {m_noiseAmplitude = value;}
    
    void onNoiseSpeedChange(float& value) {m_noiseSpeed = value;}
    
    void onWireFrameColorChange(ofColor& color) {m_wireFrameColor = color;}
    
    void setDirLightColorAnimation(ofColor& color, float duration);

	void setSpotLightColorAnimation(ofColor& color, float duration);
    
    void setDirLightFadeAnimation(float fadeAmount, float duration);
    
    void setSpotLightZ(float posz);

private:

    void setupCamera();
    
    void setupLight();
    
    void setupDirectionalLight();
    
    void setupSpotLight();
    
    void setupShaders();
    
    void setupFbos();
    
    void loadModel();
    
    void updateModel();
    
    void updateFbos();

    void updateNoise();
    
    void updateLight();
    
    void drawModel();
    
    void drawMask();
    
    void drawWireframe();
    
    void bindTexture();
    
    void unbindTexture();
    
    void setLightOri(ofLight &light, ofVec3f rot);

private:

    ofxAssimpModelLoader m_model;
    ofxAssimpModelLoader m_simpleModel;
    ofMesh               m_mesh;
    ofLight              m_dirLight;
    ofLight              m_spotLight;
    ofEasyCam            m_cam;
    ofFbo                m_fboMask;
    ofFbo                m_fboModel;
    ofFbo                m_fboWireframe;
    ofFbo                m_fboTexture;
    ofFbo                m_fboTextureWireFrame;
    
    
    ofPtr<BasicVisual>   m_dirLightVisual;
    ofPtr<BasicVisual>   m_spotLightVisual;
    ofVec3f              m_light_rot;
    ofVec3f              m_light_pos;
    ofVec3f              m_modelPos;
    ofVec3f              m_modelRot;
    
    ofMaterial          m_material;
    
    ofShader            m_thickShader;
    ofShader            m_displacementShader;

    ofImage img;
    
    float              m_noiseAmplitude;
    float              m_noiseSpeed;
    ofColor            m_wireFrameColor;
};

//==========================================================================


