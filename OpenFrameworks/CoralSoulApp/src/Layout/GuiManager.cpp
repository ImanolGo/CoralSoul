/*
 *  GuiManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */

#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
const int GuiManager::GUI_WIDTH = 350;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
	//Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
	if(m_initialized)
		return;
    
    Manager::setup();


    this->setupGuiParameters();
    this->setupScenesGui();
    this->setupGuiEvents();
    this->loadGuiValues();

    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    
    ofxDatGuiLog::quiet();
    
    m_gui.setPosition(ofxDatGuiAnchor::TOP_LEFT);
    //m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath("../Resources/data/fonts/");
    m_gui.setTheme(new GuiTheme());
    
    
    int margin =  LayoutManager::MARGIN;
    m_gui.setAutoDraw(false);
    auto pos = m_gui.getPosition();
    m_gui.setPosition(pos.x + margin, pos.y + margin);
    m_gui.addHeader(GUI_SETTINGS_NAME, false);
    
    m_gui.addFRM();
    
    m_gui.addBreak();
}

void GuiManager::setupScenesGui()
{
    auto sceneManager = &AppManager::getInstance().getSceneManager();
    vector<string> opts;
    
    for(int i = 0; i < sceneManager->getNumberScenes(); i++)
    {
        opts.push_back(sceneManager->getSceneName(i));
    }
    
    string label = "SCENES";
    
    m_gui.addDropdown(label, opts);
    auto menu = m_gui.getDropdown(label);
    //menu->expand(); //let's have it open by default
    menu->setStripeColor(ofColor::pink);
    for (int i=0; i<menu->size(); i++) menu->getChildAt(i)->setStripeColor(ofColor::pink);
    m_gui.addBreak();
    
}


void GuiManager::update()
{
    m_gui.update();
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    this->drawRectangle();
    this->drawGui();
}


void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
    m_gui.draw();
    ofDisableAlphaBlending();
}

void GuiManager::setupGuiEvents()
{
    m_gui.onDropdownEvent(this, &GuiManager::onDropdownEvent);
    m_gui.onColorPickerEvent(this, &GuiManager::onColorPickerEvent);
    m_gui.onButtonEvent(this, &GuiManager::onButtonEvent);
    m_gui.onToggleEvent(this, &GuiManager::onToggleEvent);
    m_gui.onMatrixEvent(this, &GuiManager::onMatrixEvent);
}


void GuiManager::saveGuiValues()
{
    ofXml xml;
    xml.serialize(m_parameters);
    xml.save(GUI_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
    ofXml xml(GUI_SETTINGS_FILE_NAME);
    xml.deserialize(m_parameters);
}


void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( m_gui.getPosition().x - margin, 0, m_gui.getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}



void GuiManager::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "SCENES")
    {
        AppManager::getInstance().getSceneManager().changeScene(e.child);
        //m_gui.getDropdown(e.target->getName())->expand();
        m_gui.getDropdown(e.target->getName())->setLabel("SCENES:" + e.target->getLabel());
    }
}

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
    
    
}

void GuiManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "Autodiscovery")
    {
        //AppManager::getInstance().getUdpManager().sendAutodiscovery();
    }
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "Clear")
    {
        //AppManager::getInstance().getLightSculptureManager().onClearLights(e.target->getChecked());
    }
}

void GuiManager::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
}

