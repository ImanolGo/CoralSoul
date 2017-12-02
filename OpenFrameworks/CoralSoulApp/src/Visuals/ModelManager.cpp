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

ModelManager::ModelManager(): Manager(), m_noiseAmplitude(0.0), m_noiseSpeed(0.2)
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

   // ofEnableDepthTest();
    ofDisableArbTex();  //load textures with normalized texcoords (0..1)
    ofSetSmoothLighting(true); // turn on smooth lighting //
    
    this->setupCamera();
    this->setupLight();
	this->loadModel();
    this->setupFbos();
    this->setupShaders();
    
    //img.load("images/model/sonic_bricks_d.png");
    
     ofEnableArbTex();
}


void ModelManager::setupFbos()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fboMask.allocate(width, height);
    m_fboMask.begin(); ofClear(0); m_fboMask.end();
    
    m_fboModel.allocate(width, height);
    m_fboModel.begin(); ofClear(0); m_fboModel.end();
    
    m_fboWireframe.allocate(width, height);
    m_fboWireframe.begin(); ofClear(0); m_fboWireframe.end();
    
    m_fboTexture.allocate(width, height);
    m_fboTexture.begin(); ofClear(0); m_fboTexture.end();
}

void ModelManager::setupCamera()
{
    //m_cam.setDistance(600);
    m_cam.disableMouseInput();
    m_cam.setTarget(ofVec3f(0));
}

void ModelManager::setupLight()
{
    m_light.setDiffuseColor(ofColor(255.0f, 255.0f, 255.0f));
    m_light.setSpecularColor(ofColor(255, 255, 255));
    m_light.setAmbientColor(ofColor(255));
    
    m_light_pos.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);
    m_light.setPosition(m_light_pos.x, m_light_pos.y, 0);
    
    m_material.setAmbientColor(255);
    m_material.setSpecularColor(255);
    m_material.setShininess(244);
    //m_material.setSpecularColor(0);
    
    m_light.setDirectional();

    //m_light.setAmbientColor(ofColor(200));
    //m_light.setAttenuation();
    m_light_rot = ofVec3f(0, -90, 0);
    this->setLightOri(m_light, m_light_rot);
}

void ModelManager::setupShaders()
{
    m_thickShader.setGeometryInputType(GL_LINES);
    m_thickShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    m_thickShader.setGeometryOutputCount(4);
    
    if(ofIsGLProgrammableRenderer())
    {
        m_thickShader.load("shaders/shadersGL3/ThickLineShaderVert.glsl", "shaders/shadersGL3/ThickLineShaderFrag.glsl", "shaders/shadersGL3/ThickLineShaderGeom.glsl");
        m_displacementShader.load("shaders/shadersGL3/DisplacementMap");
    }
    else{
        m_thickShader.load("shaders/shadersGL2/ThickLineShaderVert.glsl", "shaders/shadersGL2/ThickLineShaderFrag.glsl", "shaders/shadersGL2/ThickLineShaderGeom.glsl");
            m_displacementShader.load("shaders/shadersGL2/DisplacementMap");
    }
   

}

void ModelManager::loadModel()
{
    //ofEnableDepthTest();
    
    auto modelResources = AppManager::getInstance().getSettingsManager().getModelResourcesPath();
    
    string path = "images/model/SimplifiedWall.obj";
    string name = "Full";
    
    if(modelResources.find(name)!=modelResources.end()){
        path = modelResources[name];
    }
    
    m_model.loadModel(path);
    
    name = "Simplified";
    if(modelResources.find(name)!=modelResources.end()){
        path = modelResources[name];
    }
    
    //load the model - the 3ds and the texture file need to be in the same folder
    m_simpleModel.loadModel(path);
    //m_model.loadModel("images/model/TexturedWall.obj");
   
    
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    //m_model.setRotation(0, 90, 1, 0, 0);
   // m_model.setRotation(1, 1, 90, 0, 1);
    m_model.setScale(-1, -1, 1);
    m_simpleModel.setScale(-1, -1, 1);
    //m_model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.5, 0);
    
     //ofDisableDepthTest();
}

void ModelManager::update()
{
   this->updateModel();
   this->updateNoise();
   this->updateFbos();
}

void ModelManager::updateModel()
{
    m_model.update();
    m_simpleModel.update();
    m_mesh = m_simpleModel.getMesh(0);
}

void ModelManager::updateFbos()
{
    m_fboMask.begin();
        ofClear(0, 0, 0);
        m_cam.begin();
            m_model.drawFaces();
       m_cam.end();
    m_fboMask.end();
    
    m_fboWireframe.begin();
    ofClear(0, 0, 0);
    m_cam.begin();
        //ofSetLineWidth(2);
        //m_simpleModel.drawWireframe();
        this->drawWireframe();
    m_cam.end();
    m_fboWireframe.end();
    
    m_fboModel.begin();
        ofClear(0, 0, 0);
        m_cam.begin();
        this->drawModel();
        m_cam.end();
    m_fboModel.end();
    
 
}

void ModelManager::draw()
{
    this->drawModel();
}

void ModelManager::drawMask()
{
    m_fboMask.draw(0,0);
}

void ModelManager::drawModel()
{
    m_fboTexture.begin();
    AppManager::getInstance().getSceneManager().draw();
    m_fboTexture.end();
    
    ofEnableDepthTest();
    
    // enable lighting //
    ofEnableLighting();
    
    m_light.enable();
    m_material.begin();
    
    ofPushStyle();
    ofSetColor(255);
    
    m_fboTexture.getTexture().bind();
        m_model.drawFaces();
    m_fboTexture.getTexture().unbind();
    
    ofPopStyle();
    
    m_material.end();
    
    // turn off lighting //
    ofDisableLighting();
    
    ofDisableDepthTest();

}

void  ModelManager::drawModel(const ofFbo& tex)
{
    m_cam.begin();
    
    m_fboTexture.begin();
        tex.draw(0,0,m_fboTexture.getWidth(), m_fboTexture.getHeight());
    m_fboTexture.end();
    
    ofEnableDepthTest();
    
    // enable lighting //
    ofEnableLighting();
    
    m_light.enable();
    //m_material.begin();
    
    ofPushStyle();
    ofSetColor(255);
    
    m_fboTexture.getTexture().bind();
        m_model.drawFaces();
    m_fboTexture.getTexture().unbind();
    
    ofPopStyle();
    
    //m_material.end();
    
    // turn off lighting //
    ofDisableLighting();
    
    ofDisableDepthTest();
    
     m_cam.end();
}
void ModelManager::updateNoise()
{
    //modify mesh with some noise
    
    if(m_noiseAmplitude<=0.0 || m_noiseSpeed<=0.0){
        return;
    }
    
    float liquidness = 5;
    float amplitude = m_noiseAmplitude;
    float speedDampen = 1.0/m_noiseSpeed;
    vector<ofVec3f>& verts = m_mesh.getVertices();
    for(unsigned int i = 0; i < verts.size(); i++){
        verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        verts[i].z += ofSignedNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
    }
    
    //ofLogNotice() <<"ModelManager::updateNoise << amp = " << amplitude << ", speedDampen = " << speedDampen;
}

void ModelManager::drawWireframe()
{
    //ofEnableSmoothing();
    //ofEnableAntiAliasing();
    
    //this->updateNoise();
    
    ofSetLineWidth(3.0);
    
    ofEnableDepthTest();
    
    // enable lighting //
    ofEnableLighting();
    
    m_light.enable();
    
    
    ofxAssimpMeshHelper & meshHelper = m_simpleModel.getMeshHelper(0);
    ofMaterial material = m_simpleModel.getMaterialForMesh(0);
    ofMultMatrix(m_simpleModel.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
    
    material.begin();
    
    m_mesh.drawWireframe();
    
    material.end();
    // turn off lighting //
    ofDisableLighting();
    
    ofDisableDepthTest();
    
//    ofDisableSmoothing();
//    ofDisableAntiAliasing();
}


void ModelManager::bindTexture() {
    
    auto tex = AppManager::getInstance().getLayoutManager().getCurrentFbo().getTexture();
    
    //img.getTexture().bind();
    
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
   // img.getTexture().unbind();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void ModelManager::setLightOri(ofLight &light, ofVec3f rot)
{
    ofVec3f xax(1, 0, 0);
    ofVec3f yax(0, 1, 0);
    ofVec3f zax(0, 0, 1);
    ofQuaternion q;
    q.makeRotate(rot.x, xax, rot.y, yax, rot.z, zax);
    light.setOrientation(q);
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
     m_cam.setTarget(ofVec3f(0));
}


void ModelManager::onCameraYChange(float& value)
{
    auto pos = m_cam.getPosition();
    pos.y = value;
    m_cam.setPosition(pos);
     m_cam.setTarget(ofVec3f(0));
}


void ModelManager::onCameraZChange(float& value)
{
    auto pos = m_cam.getPosition();
    pos.z = value;
    m_cam.setPosition(pos);
     m_cam.setTarget(ofVec3f(0));
}
void ModelManager::onCameraFovChange(float& value)
{
    m_cam.setFov(value);
}


void ModelManager::onLightXChange(float& value)
{
    m_light_rot.x = value;
    setLightOri(m_light, m_light_rot);
}

void ModelManager::onLightYChange(float& value)
{
    m_light_rot.y = value;
    setLightOri(m_light, m_light_rot);
}

void ModelManager::onLightZChange(float& value)
{
    m_light_rot.z = value;
    setLightOri(m_light, m_light_rot);
}

void ModelManager::onPositionXChange(float& value)
{
    m_modelPos.x = value;
    m_model.setPosition(m_modelPos.x , m_modelPos.y , m_modelPos.z);
    m_simpleModel.setPosition(m_modelPos.x , m_modelPos.y , m_modelPos.z);
}

void ModelManager::onPositionYChange(float& value)
{
    m_modelPos.y = value;
    m_model.setPosition(m_modelPos.x , m_modelPos.y , m_modelPos.z);
    m_simpleModel.setPosition(m_modelPos.x , m_modelPos.y , m_modelPos.z);
}

void ModelManager::onPositionZChange(float& value)
{
    m_modelPos.z = value;
    m_model.setPosition(m_modelPos.x , m_modelPos.y , m_modelPos.z);
    m_simpleModel.setPosition(m_modelPos.x , m_modelPos.y , m_modelPos.z);
}

void ModelManager::onRotationXChange(float& value)
{
    m_modelRot.x = value;
    m_model.setRotation(0, m_modelRot.x, 1, 0, 0);
    m_simpleModel.setRotation(0, m_modelRot.x, 1, 0, 0);
}

void ModelManager::onRotationYChange(float& value)
{
    m_modelRot.y = value;
    m_model.setRotation(1, m_modelRot.y, 0, 1, 0);
    m_simpleModel.setRotation(1, m_modelRot.y, 0, 1, 0);
}

void ModelManager::onRotationZChange(float& value)
{
    m_modelRot.z = value;
    m_model.setRotation(2, m_modelRot.z, 0, 0, 1);
    m_simpleModel.setRotation(2, m_modelRot.z, 0, 0, 1);
}


void ModelManager::onLightColorChange(ofColor color)
{
    m_light.setDiffuseColor(color);
    m_light.setSpecularColor(color);
}


