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


GuiManager::GuiManager(): Manager(), m_showGui(true), m_cityLabel(NULL)
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
    this->setupPreviewGui();
    this->setupLayoutGui();
    this->setupLightGui();
    this->setupWeatherGui();
    this->setupCameraGui();
    this->setupModelGui();
    this->setupNoiseGui();
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
    
    m_gui.addFRM(0.1);
    
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
    menu->expand(); //let's have it open by default
    menu->setStripeColor(ofColor::pink);
    for (int i=0; i<menu->size(); i++) menu->getChildAt(i)->setStripeColor(ofColor::pink);
    m_gui.addBreak();
    
}

void GuiManager::setupPreviewGui()
{
    vector<string> opts;
    opts.push_back("MASK");
    opts.push_back("MODEL");
    opts.push_back("WIREFRAME");
    opts.push_back("SCENE");
    opts.push_back("NOISE");
    
    string label = "PREVIEW";
    
    m_gui.addDropdown(label, opts);
    auto menu = m_gui.getDropdown(label);
    menu->expand(); //let's have it open by default
    menu->setStripeColor(ofColor::yellow);
    for (int i=0; i<menu->size(); i++) menu->getChildAt(i)->setStripeColor(ofColor::yellow);
    m_gui.addBreak();
}

void GuiManager::setupLayoutGui()
{
    auto layoutManager = &AppManager::getInstance().getLayoutManager();
    
    m_layourBlurScale.set("Blur", 1.0, 0.0, 2.0);
    m_layourBlurScale.addListener(layoutManager, &LayoutManager::onBlurScaleChange);
    m_parameters.add(m_layourBlurScale);
    
    ofxDatGuiFolder* folder = m_gui.addFolder("LAYOUT", ofColor::purple);
    folder->addSlider(m_layourBlurScale);
    //folder->expand();
    m_gui.addBreak();
}

void GuiManager::setupLightGui()
{
    auto modelManager = &AppManager::getInstance().getModelManager();
    
    m_lightX.set("LightX", 0.0, -180.0, 180.0);
    m_lightX.addListener(modelManager, &ModelManager::onLightXChange);
    m_parameters.add(m_lightX);
    
    m_lightY.set("LightY", 0.0, -180.0, 180.0);
    m_lightY.addListener(modelManager, &ModelManager::onLightYChange);
    m_parameters.add(m_lightY);
    
    m_lightZ.set("LightZ", 0.0, -180.0, 180.0);
    m_lightZ.addListener(modelManager, &ModelManager::onLightZChange);
    m_parameters.add(m_lightZ);
    
    ofxDatGuiFolder* folder = m_gui.addFolder("LIGHT", ofColor::white);
    folder->addSlider(m_lightX);
    folder->addSlider(m_lightY);
    folder->addSlider(m_lightZ);
    //auto colorPicker = folder->addColorPicker("LIGHT");
    //colorPicker->setColor(ofColor(255));
    //folder->expand();
    m_gui.addBreak();

}

void GuiManager::setupNoiseGui()
{
    auto noiseManager = &AppManager::getInstance().getNoiseManager();
    
    m_noiseResolution.set("Noise Resolution",  64, 2, 256 );
    m_noiseResolution.addListener(noiseManager, &NoiseManager::onNoiseResolutionChange);
    m_parameters.add(m_noiseResolution);
    
    m_noiseFrequency.set("Noise Frequency",  0.4, 0.0, 4.0);
    m_noiseFrequency.addListener(noiseManager, &NoiseManager::onNoiseFrequencyChange);
    m_parameters.add(m_noiseFrequency);
    
    m_noiseSpeed.set("Noise Speed",  1.0, 0.001, 3);
    m_noiseSpeed.addListener(noiseManager, &NoiseManager::onNoiseSpeedChange);
    m_parameters.add(m_noiseSpeed);
    
    ofxDatGuiFolder* folder = m_gui.addFolder("NOISE", ofColor::white);
    folder->addSlider(m_noiseResolution);
    folder->addSlider(m_noiseFrequency);
    folder->addSlider(m_noiseSpeed);
    //folder->expand();
    m_gui.addBreak();
    
}



void GuiManager::setupCameraGui()
{
    auto modelManager = &AppManager::getInstance().getModelManager();
    
    m_cameraDistance.set("Distance", 600, 0.0, 1000.0);
    m_cameraDistance.addListener(modelManager, &ModelManager::onCameraDistanceChange);
    m_parameters.add(m_cameraDistance);
    
    m_cameraX.set("X", 0, -1000.0, 1000.0);
    m_cameraX.addListener(modelManager, &ModelManager::onCameraXChange);
    m_parameters.add(m_cameraX);
    
    m_cameraY.set("Y", 0, -1000.0, 1000.0);
    m_cameraY.addListener(modelManager, &ModelManager::onCameraYChange);
    m_parameters.add(m_cameraY);
    
    m_cameraZ.set("Z", 600, 0, 3000.0);
    m_cameraZ.addListener(modelManager, &ModelManager::onCameraZChange);
    m_parameters.add(m_cameraZ);
    
    m_cameraFov.set("FOV", 60, 0, 180);
    m_cameraFov.addListener(modelManager, &ModelManager::onCameraFovChange);
    m_parameters.add(m_cameraFov);
    
    ofxDatGuiFolder* folder = m_gui.addFolder("CAMERA", ofColor::yellow);
    //folder->addSlider(m_cameraDistance);
    folder->addSlider(m_cameraX);
    folder->addSlider(m_cameraY);
    folder->addSlider(m_cameraZ);
    //folder->addSlider(m_cameraFov);
    //folder->expand();
    
    m_gui.addBreak();
}

void GuiManager::setupModelGui()
{
    auto modelManager = &AppManager::getInstance().getModelManager();
    
    m_modelPosX.set("Pos X", 0, -1000.0, 1000.0);
    m_modelPosX.addListener(modelManager, &ModelManager::onPositionXChange);
    m_parameters.add(m_modelPosX);
    
    m_modelPosY.set("Pos Y", 0, -1000.0, 1000.0);
    m_modelPosY.addListener(modelManager, &ModelManager::onPositionYChange);
    m_parameters.add(m_modelPosY);
    
    m_modelPosZ.set("Pos Z", 0, -1000.0, 1000.0);
    m_modelPosZ.addListener(modelManager, &ModelManager::onPositionZChange);
    m_parameters.add(m_modelPosZ);
    
    m_modelRotX.set("Rot X", 0, -180.0, 180);
    m_modelRotX.addListener(modelManager, &ModelManager::onRotationXChange);
    m_parameters.add(m_modelRotX);
    
    m_modelRotY.set("Rot Y", 0, -180.0, 180);
    m_modelRotY.addListener(modelManager, &ModelManager::onRotationYChange);
    m_parameters.add(m_modelRotY);
    
    m_modelRotZ.set("Rot Z", 0, -180.0, 180);
    m_modelRotZ.addListener(modelManager, &ModelManager::onRotationZChange);
    m_parameters.add(m_modelRotZ);
    
    ofxDatGuiFolder* folder = m_gui.addFolder("MODEL", ofColor::yellow);
    folder->addSlider(m_modelPosX);
    folder->addSlider(m_modelPosY);
    folder->addSlider(m_modelPosZ);
    folder->addSlider(m_modelRotX);
    folder->addSlider(m_modelRotY);
    folder->addSlider(m_modelRotZ);
    //folder->expand();
    
    m_gui.addBreak();
    
    ofParameter<float>      m_modelRotX;
    ofParameter<float>      m_modelRotY;
    ofParameter<float>      m_modelRotZ;
    
}


void GuiManager::setupWeatherGui()
{
    auto apiManager = &AppManager::getInstance().getApiManager();
    
    m_weatherTemperature.set("Temp.", 0.0, -20.0, 50.0);
    m_weatherTemperature.addListener(apiManager, &ApiManager::onTemperatureChange);
    m_parameters.add(m_weatherTemperature);
    
    m_weatherHumidity.set("Humidity", 0.0, 0.0, 100.0);
    m_weatherHumidity.addListener(apiManager, &ApiManager::onHumidityChange);
    m_parameters.add(m_weatherHumidity);
    
    m_weatherWindSpeed.set("Wind Speed", 0.0, 0.0, 100.0);
    m_weatherWindSpeed.addListener(apiManager, &ApiManager::onWindSpeedChange);
    m_parameters.add(m_weatherWindSpeed);
    
    m_weatherWindDirection.set("Wind Dir", 0.0, 0.0, 360.0);
    m_weatherWindDirection.addListener(apiManager, &ApiManager::onWindDirChange);
    m_parameters.add(m_weatherWindDirection);
    
    m_weatherPrecipitation.set("Prec.", 0.0, 0.0, 360.0);
    m_weatherPrecipitation.addListener(apiManager, &ApiManager::onPrecipitationChange);
    m_parameters.add(m_weatherPrecipitation);
    
    m_weatherClouds.set("Clouds.", 0.0, 0.0, 100);
    m_weatherClouds.addListener(apiManager, &ApiManager::onCloudsChange);
    m_parameters.add(m_weatherClouds);
    
    m_swellHeight.set("Swell Height", 0.0, 0.0, 10);
    m_swellHeight.addListener(apiManager, &ApiManager::onSwellHeightChange);
    m_parameters.add(m_swellHeight);
    
    m_swellPeriod.set("Swell Period", 0.0, 0.0, 20);
    m_swellPeriod.addListener(apiManager, &ApiManager::onSwellPeriodChange);
    m_parameters.add(m_swellPeriod);
    
    
    ofxDatGuiFolder* folder = m_gui.addFolder("WEATHER", ofColor::blue);
    m_cityLabel = folder->addLabel("CITY: ");
    folder->addSlider(m_weatherTemperature);
    folder->addSlider(m_weatherHumidity);
    folder->addSlider(m_weatherWindSpeed);
    folder->addSlider(m_weatherWindDirection);
    folder->addSlider(m_weatherPrecipitation);
    folder->addSlider(m_weatherClouds);
    folder->addSlider(m_swellHeight);
    folder->addSlider(m_swellPeriod);
    folder->expand();
    
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
        m_gui.getDropdown(e.target->getName())->expand();
        m_gui.getDropdown(e.target->getName())->setLabel("SCENES:" + e.target->getLabel());
    }
    
    else if(e.target->getName() == "PREVIEW")
    {
        AppManager::getInstance().getLayoutManager().onSetPreviewMode(e.child);
        m_gui.getDropdown(e.target->getName())->expand();
        m_gui.getDropdown(e.target->getName())->setLabel("PREVIEW:" + e.target->getLabel());
    }
}

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "LIGHT")
    {
        //cout << "the picker was set to: " << e.color << endl;
        AppManager::getInstance().getModelManager().onLightColorChange(e.color);
    }
    
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

void GuiManager::onWeatherChange(const weather_conditions& value)
{
    m_weatherTemperature = value.temp;
    m_weatherHumidity = value.humidity;
    m_weatherWindSpeed = value.windSpeed;
    m_weatherWindDirection = value.windDirection;
    m_weatherPrecipitation = value.precipitationValue;
    
    m_swellHeight = value.swellHeight;
    m_swellPeriod = value.swellPeriod;
    
    if(m_cityLabel!=NULL){
        m_cityLabel->setLabel("CITY: " +  value.city);
    }
}
