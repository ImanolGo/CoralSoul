/*
 *  AppManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#include "ofMain.h"


#include "AppManager.h"

AppManager& AppManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static AppManager    m_instance;
    return m_instance;

}

AppManager::AppManager(): Manager(), m_debugMode(false), m_initialized(false)
{
   //Intentionally left empty
}


AppManager::~AppManager()
{
    ofLogNotice() <<"AppManager::Destructor";
}


void AppManager::setup()
{
	if(m_initialized)
		return;

    //ofSetDataPathRoot("../Resources/data/");
    
    ofLogNotice() << "AppManager::initialized";

	Manager::setup();
    
    this->setupOF();
	this->setupManagers();
    
    m_initialized = true;
    
    //setDebugMode(m_debugMode);
}

void AppManager::setupOF()
{
    ofSetLogLevel(OF_LOG_NOTICE);
    //ofSetFrameRate(25);
    ofSetVerticalSync(true);
    ofSetEscapeQuitsApp(true);
    ofSetBackgroundAuto(true);
    //ofDisableArbTex();
}


void AppManager::setupManagers()
{
    m_settingsManager.setup();
    m_resourceManager.setup();
    m_viewManager.setup();
    m_visualEffectsManager.setup();
    m_maskManager.setup();
    m_layoutManager.setup();
    m_apiManager.setup();
    m_keyboardManager.setup();
    m_sceneManager.setup();
    m_modelManager.setup();
    
    m_guiManager.setup();
}

void AppManager::update()
{
    if(!m_initialized)
        return;
    
    m_visualEffectsManager.update();
    m_viewManager.update();
    m_apiManager.update();
    m_modelManager.update();
    m_sceneManager.update();
    m_guiManager.update();
    m_layoutManager.update();
}


void AppManager::draw()
{
    
    if(!m_initialized)
        return;
    
    
    //ofLogNotice()<<"AppManager::ofGetLastFrameTime()-> " << ofGetLastFrameTime();

    //ofBackgroundGradient( ofColor(80), ofColor(55), OF_GRADIENT_CIRCULAR );
    ofBackground(55,55,55);
   // m_viewManager.draw();
    //m_modelManager.draw();
    m_layoutManager.draw();
    m_guiManager.draw();
    //m_noiseManager.draw();
    
}

void AppManager::toggleDebugMode()
{
    m_debugMode = !m_debugMode;
    setDebugMode(m_debugMode);
}


void AppManager::setDebugMode(bool showDebug)
{
    m_debugMode = showDebug;
    
    ofLogNotice()<<"AppManager::setDebugMode-> " << m_debugMode;
    
    if(m_debugMode){
        //ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else{
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    

    m_guiManager.showGui(m_debugMode);
    
}



