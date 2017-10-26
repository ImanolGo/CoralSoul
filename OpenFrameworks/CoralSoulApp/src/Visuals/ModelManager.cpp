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

	this->loadModel();
}



void ModelManager::loadModel()
{
    //load the model - the 3ds and the texture file need to be in the same folder
    m_model.loadModel("images/model/WallSimplified2.obj");
    
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    //m_model.setRotation(0, 90, 1, 0, 0);
   // m_model.setRotation(1, 1, 90, 0, 1);
    m_model.setScale(-0.9, 0.9, 0.9);
    m_model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75, 0);

}

void ModelManager::update()
{
   this->updateModel();
}

void ModelManager::updateModel()
{
    m_model.update();
    m_mesh = m_model.getMesh(0);
}

void ModelManager::draw()
{
  this->drawModel();
}

void ModelManager::drawModel()
{
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofEnableDepthTest();
#ifndef TARGET_PROGRAMMABLE_GL
    glShadeModel(GL_SMOOTH); //some model / light stuff
#endif
    m_light.enable();
    ofEnableSeparateSpecularLight();
    
    ofPushMatrix();
    ofTranslate(m_model.getPosition().x+100, m_model.getPosition().y, 0);
    ofRotate(-ofGetMouseX(), 0, 1, 0);
    ofTranslate(-m_model.getPosition().x, -m_model.getPosition().y, 0);
    //m_model.drawFaces();
    ofPopMatrix();
#ifndef TARGET_PROGRAMMABLE_GL
    glEnable(GL_NORMALIZE);
#endif
    ofPushMatrix();
    ofTranslate(m_model.getPosition().x+100, m_model.getPosition().y, 0);
    ofRotate(-ofGetMouseX(), 0, 1, 0);
    ofTranslate(-m_model.getPosition().x, -m_model.getPosition().y, 0);
    
    ofxAssimpMeshHelper & meshHelper = m_model.getMeshHelper(0);
    
    ofMultMatrix(m_model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
    
    ofMaterial & material = meshHelper.material;
    if(meshHelper.hasTexture()){
        meshHelper.getTextureRef().bind();
    }
    material.begin();
    m_mesh.drawWireframe();
    material.end();
    if(meshHelper.hasTexture()){
        meshHelper.getTextureRef().unbind();
    }
    ofPopMatrix();
    
    ofDisableDepthTest();
    m_light.disable();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();
    
}

