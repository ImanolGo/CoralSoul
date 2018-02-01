/*
 *  SceneManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 28/10/17.
 *
 */

#include "ofMain.h"


#include "SceneManager.h"
#include "scenes.h"
#include "AppManager.h"

SceneManager::SceneManager(): Manager(), m_alpha(-1), m_transitionTime(1.0)
{
	//Intentionally left empty
}


SceneManager::~SceneManager()
{
    ofLogNotice() <<"SceneManager::Destructor";
}


void SceneManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();

    this->createScenes();
    this->setupFbo();
    this->setupTimer();
    this->initializeSceneList();

    ofLogNotice() <<"SceneManager::initialized";

}


void SceneManager::createScenes()
{
    m_mySceneManager.setTransitionFade();
	//m_mySceneManager.setTransitionDissolve();
    
    ofPtr<ofxScene> scene;
    
    //Create Video Scene
    scene = ofPtr<ofxScene> (new VideoScene("TEST_VIDEO"));
    //m_mySceneManager.addScene(scene);
    
    //Create Image Scene
    scene = ofPtr<ofxScene> (new ImageScene("RAINBOW"));
    //m_mySceneManager.addScene(scene);
    
    //Create Sea Scene
    scene = ofPtr<ofxScene> (new SeaScene());
    m_mySceneManager.addScene(scene);
    
    //Create Life Scene
    scene = ofPtr<ofxScene> (new LifeScene());
    m_mySceneManager.addScene(scene);
    
    //Create Day Scene
    scene = ofPtr<ofxScene> (new DayScene());
    m_mySceneManager.addScene(scene);
    
    //Create Day Scene
    scene = ofPtr<ofxScene> (new NightScene());
    m_mySceneManager.addScene(scene);
    
    //Create Wind Scene
    scene = ofPtr<ofxScene> (new WindScene());
    m_mySceneManager.addScene(scene);
    
    //Create Rain Scene
    scene = ofPtr<ofxScene> (new RainScene());
    m_mySceneManager.addScene(scene);
    
    //Create Flow Scene
    scene = ofPtr<ofxScene> (new FlowScene());
    //m_mySceneManager.addScene(scene);
    
    //Create Universe Scene
    scene = ofPtr<ofxScene> (new UniverseScene());
    //m_mySceneManager.addScene(scene);
    
    //Create Calibration Scene
    scene = ofPtr<ofxScene> (new CalibrationScene());
    m_mySceneManager.addScene(scene);
    
    //Create Blank Scene
    scene = ofPtr<ofxScene> (new BlankScene());
    m_mySceneManager.addScene(scene);
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();

    m_mySceneManager.run(width, height);
    this->onTransitionTimeChange(m_transitionTime);
}


void SceneManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    //float width = ofGetWidth();
    //float height = ofGetHeight();

    m_fbo.allocate(width, height, GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void SceneManager::setupTimer()
{
    auto time = AppManager::getInstance().getSettingsManager().getSceneTimer();
    
    m_sceneTimer.setup( time*1000 );
    m_sceneTimer.start( false ) ;
    ofAddListener( m_sceneTimer.TIMER_COMPLETE , this, &SceneManager::sceneTimerCompleteHandler ) ;
    
    ofLogNotice() <<"SceneManager::setupTimer << Time = : " << time << "s";
}

void SceneManager::onChangeSceneDuration(float& value)
{
    m_sceneTimer.setup( value*1000*60 );
    m_sceneTimer.start( false ) ;
    ofLogNotice() <<"SceneManager::setupTimer << Time = : " << time << "s";
}

void SceneManager::initializeSceneList()
{
    m_sceneList.clear();
    //m_sceneList  = { "LIFE", "SEA","WIND"};
    
    vector<string> initSceneList = { "LIFE", "SEA","WIND"};
    for(auto& sceneName: initSceneList){
        auto isSceneActive = AppManager::getInstance().getGuiManager().isSceneActive(sceneName);
        if(isSceneActive){
            m_sceneList.push_back(sceneName);
        }
    }
    
    auto isRaining = AppManager::getInstance().getApiManager().getCurrentWeather().m_precipitationValue > 0;
    string sceneName = "RAIN";
    auto isSceneActive = AppManager::getInstance().getGuiManager().isSceneActive(sceneName);
    if(isRaining && isSceneActive){m_sceneList.push_back(sceneName);}
    
    auto isDayTime = AppManager::getInstance().getApiManager().isDayTime();
    if(isDayTime){sceneName = "DAY";}
    else{sceneName = "NIGHT";}
    
    isSceneActive = AppManager::getInstance().getGuiManager().isSceneActive(sceneName);
    if(isSceneActive){m_sceneList.push_back(sceneName);}


	//m_sceneList.clear();
	//m_sceneList = { "LIFE", "WIND", "NIGHT" };

	//auto isRaining = AppManager::getInstance().getApiManager().getCurrentWeather().m_precipitationValue > 0;
	//if (isRaining) { m_sceneList.push_back("RAIN"); }

}


void SceneManager::update()
{
    this->updateScenes();
    this->updateFbo();
    this->updateTimer();
    //this->updateAlpha();
}

void SceneManager::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        ofPushStyle();
        ofSetColor(255);
        ofEnableAlphaBlending();
            m_mySceneManager.draw();
        ofDisableAlphaBlending();
        ofPopStyle();
    m_fbo.end();
}

void SceneManager::updateAlpha()
{    
    if(m_alpha!=m_mySceneManager.getCurrentAlpha())
    {
        m_alpha=m_mySceneManager.getCurrentAlpha();
        //ofLogNotice() <<"SceneManager::updateAlpha << Alpha = " << m_alpha;
        
        string address = "/CoralSoul/Ableton/Fade";
        float value = m_alpha/255.0;
        
        ofxOscMessage m;
        m.setAddress(address);
        m.addFloatArg(value);
        
        AppManager::getInstance().getOscManager().sendMessage(m);
    }
}

void SceneManager::updateScenes()
{
    m_mySceneManager.update();
}

void SceneManager::updateTimer()
{
    m_sceneTimer.update();
}


void SceneManager::draw()
{
	ofEnableAlphaBlending();
    m_fbo.draw(0,0);
	ofDisableAlphaBlending();
}

void SceneManager::draw(const ofRectangle& rect)
{
    m_fbo.draw(rect.x,rect.y,rect.width,rect.height);
}


void SceneManager::changeScene(string sceneName)
{
    m_mySceneManager.changeScene(sceneName);
    m_sceneTimer.start(false,true);
    m_currentSceneName = sceneName;
    this->sendSceneChange();
    
    
}

void SceneManager::changeScene(int sceneIndex)
{
    
     m_mySceneManager.changeScene(sceneIndex);
     m_sceneTimer.start(false,true);
     m_currentSceneName = this->getSceneName(sceneIndex);
     this->sendSceneChange();
}


void SceneManager::onTransitionTimeChange(float value)
{
   m_mySceneManager.setSceneDuration(value,value);
   m_sceneTimer.start(false,true);
}

string SceneManager::getSceneName(int sceneIndex)
{
    string name = "";
    if(sceneIndex < 0 || sceneIndex >= m_mySceneManager.scenes.size()){
        return name;
    }
    
    return m_mySceneManager.scenes[sceneIndex]->getName();
   
}

int SceneManager::getIndex(string& sceneName)
{
    auto isDayTime = AppManager::getInstance().getApiManager().isDayTime();
    
    if(sceneName=="DAYCYCLE"){
        if(isDayTime) { sceneName="DAY";}
        else{sceneName="NIGHT";}
    }
    
    for(int i = 0; i< m_mySceneManager.scenes.size(); i++){
        if(m_mySceneManager.scenes[i]->getName() == sceneName){
            return i;
        }
    }
    
    return -1;
}

void SceneManager::sceneTimerCompleteHandler( int &args )
{
    m_sceneTimer.start(false);
    
    if(m_sceneList.empty()){
        this->initializeSceneList();
    }
    
    string sceneName = m_sceneList.back();  m_sceneList.pop_back();
    AppManager::getInstance().getGuiManager().onSceneChange(sceneName);
    
    ofLogNotice() <<"SceneManager::sceneTimerCompleteHandler << Chnage Scene: " << sceneName;
}

void SceneManager::sendSceneChange()
{
    int sceneIndex = this->getIndex(m_currentSceneName);
       
    string address = "/CoralSoul/Ableton/Clip";
    
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(sceneIndex+1);
    AppManager::getInstance().getOscManager().sendMessage(m);

	auto isDayTime = AppManager::getInstance().getApiManager().isDayTime();

	if (isDayTime) {
		address = "/layer7/clip3" + ofToString(sceneIndex) + "/connect";
		m.setAddress(address);
		m.addIntArg(1);
		AppManager::getInstance().getOscManager().sendMessage(m);

		address = "/layer6/clip3" + ofToString(sceneIndex) + "/connect";
		m.setAddress(address);
		m.addIntArg(1);
		AppManager::getInstance().getOscManager().sendMessage(m);

		address = "/layer7/clip2" + ofToString(sceneIndex + 4) + "/connect";
		m.setAddress(address);
		m.addIntArg(0);
		AppManager::getInstance().getOscManager().sendMessage(m);

		address = "/layer6/clip2" + ofToString(sceneIndex + 4) + "/connect";
		m.setAddress(address);
		m.addIntArg(0);
		AppManager::getInstance().getOscManager().sendMessage(m);
	}
	else {

		address = "/layer7/clip2" + ofToString(sceneIndex+4) + "/connect";
		m.setAddress(address);
		m.addIntArg(1);
		AppManager::getInstance().getOscManager().sendMessage(m);

		address = "/layer6/clip2" + ofToString(sceneIndex+4) + "/connect";
		m.setAddress(address);
		m.addIntArg(1);
		AppManager::getInstance().getOscManager().sendMessage(m);

		address = "/layer7/clip3" + ofToString(sceneIndex) + "/connect";
		m.setAddress(address);
		m.addIntArg(0);
		AppManager::getInstance().getOscManager().sendMessage(m);

		address = "/layer6/clip3" + ofToString(sceneIndex) + "/connect";
		m.setAddress(address);
		m.addIntArg(0);
		AppManager::getInstance().getOscManager().sendMessage(m);

	}


	address = "/layer2/clip3" + ofToString(sceneIndex) + "/connect";
	m.setAddress(address);
	m.addIntArg(1);

	AppManager::getInstance().getOscManager().sendMessage(m);
    
	int effectValue = 1;
	if (isDayTime) 
	{
		effectValue = 0;
	}
	
	address = "/composition/video/effect1/bypassed";
	m.setAddress(address);
	m.addIntArg(effectValue);

	AppManager::getInstance().getOscManager().sendMessage(m);

	address = "/composition/video/effect2/bypassed";
	m.setAddress(address);
	m.addIntArg(effectValue);

	AppManager::getInstance().getOscManager().sendMessage(m);

	address = "/composition/video/effect3/bypassed";
	m.setAddress(address);
	m.addIntArg(effectValue);

	AppManager::getInstance().getOscManager().sendMessage(m);

}





