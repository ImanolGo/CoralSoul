/*
 *  ModelManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#include "ofMain.h"

#include "AppManager.h"
#include "SettingsManager.h"
#include "ViewManager.h"


#include "ModelManager.h"

ModelManager::ModelManager(): Manager()
{
	//Intentionally left empty
}


ModelManager::~ModelManager()
{
    ofLogNotice() <<"ModelManager::Destructor";
}


void ModelManager::setup()
{
	if(m_initialized)
		return;
    
    Manager::setup();

    ofLogNotice() <<"ModelManager::initialized";

    this->setupCamera();
	this->loadModel();
    this->setupFbos();
    
    img.load("images/model/ROCA_MODELO_VFX_38000_centred.jpg");
}


void ModelManager::setupFbos()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    
    m_fboMask.allocate(width, height);
    m_fboMask.begin(); ofClear(0); m_fboMask.end();
}

void ModelManager::setupCamera()
{
    m_cam.setDistance(600);
    m_cam.disableMouseInput();
}

void ModelManager::loadModel()
{
    //load the model - the 3ds and the texture file need to be in the same folder
    m_model.loadModel("images/model/texturemappedwall.3ds");
    
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    //m_model.setRotation(0, 90, 1, 0, 0);
   // m_model.setRotation(1, 1, 90, 0, 1);
    m_model.setScale(-1, -1, 1);
    //m_model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.5, 0);

}

void ModelManager::update()
{
   this->updateModel();
   this->updateFbos();
}

void ModelManager::updateModel()
{
    m_model.update();
    m_mesh = m_model.getMesh(0);
}

void ModelManager::updateFbos()
{
    m_fboMask.begin();
        ofClear(0, 0, 0);
        m_cam.begin();
            m_model.drawFaces();
        m_cam.end();
    m_fboMask.end();
}

void ModelManager::draw()
{
  //this->drawModel();
  this->drawMask();
}

void ModelManager::drawMask()
{
    m_fboMask.draw(0,0);
}

void ModelManager::drawModel()
{
    ofDisableArbTex();

    ofSetColor(255);
    ofTexture tex = AppManager::getInstance().getLayoutManager().getCurrentFbo().getTexture();

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    ofEnableDepthTest();
#ifndef TARGET_PROGRAMMABLE_GL
    glShadeModel(GL_SMOOTH); //some model / light stuff
#endif
    m_light.enable();
    ofEnableSeparateSpecularLight();

#ifndef TARGET_PROGRAMMABLE_GL
    glEnable(GL_NORMALIZE);
#endif
    ofPushMatrix();
    ofTranslate(m_model.getPosition().x+100, m_model.getPosition().y, 0);
    ofRotate(-ofGetMouseX(), 0, 1, 0);
    ofTranslate(-m_model.getPosition().x, -m_model.getPosition().y, 0);

    ofxAssimpMeshHelper & meshHelper = m_model.getMeshHelper(0);

    //ofMultMatrix(m_model.getModelMatrix());
    //ofMultMatrix(meshHelper.matrix);

    //ofMaterial & material = meshHelper.material;
   // ofDisableArbTex();
    img.bind();
    //bindTexture();
    //material.begin();
    //m_mesh.drawWireframe();
    //m_mesh.drawFaces();
    m_model.drawFaces();
    //material.end();

     //unbindTexture();
    img.unbind();
    ofPopMatrix();

    ofDisableDepthTest();
    m_light.disable();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();
   
}


void ModelManager::bindTexture() {
    
    auto tex = AppManager::getInstance().getLayoutManager().getCurrentFbo().getTexture();
    
    tex.bind();
    
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    
    ofTextureData texData = tex.getTextureData();
    if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB) {
        glScalef(tex.getWidth(), tex.getHeight(), 1.0f);
    } else {
        glScalef(tex.getWidth() / texData.tex_w, tex.getHeight() / texData.tex_h, 1.0f);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

void ModelManager::unbindTexture() {
    
    auto tex  = AppManager::getInstance().getLayoutManager().getCurrentFbo().getTexture();
    tex.unbind();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void ModelManager::onCameraDistanceChange(float& value)
{
    m_cam.setDistance(value);
}

void ModelManager::onCameraXChange(float& value)
{
    auto pos = m_cam.getPosition();
    pos.x = value;
    m_cam.setPosition(pos);
}


void ModelManager::onCameraYChange(float& value)
{
    auto pos = m_cam.getPosition();
    pos.y = value;
    m_cam.setPosition(pos);
}


void ModelManager::onCameraZChange(float& value)
{
    auto pos = m_cam.getPosition();
    pos.z = value;
    m_cam.setPosition(pos);
}
void ModelManager::onCameraFovChange(float& value)
{
    m_cam.setFov(value);
}

