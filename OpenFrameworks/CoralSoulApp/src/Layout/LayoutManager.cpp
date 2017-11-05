/*
 *  LayoutManager.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#include "ofMain.h"

#include "AppManager.h"
#include "SettingsManager.h"
#include "ViewManager.h"


#include "LayoutManager.h"

const int LayoutManager::MARGIN = 20;
const int LayoutManager::FRAME_MARGIN = 2;

const string LayoutManager::LAYOUT_FONT =  "fonts/open-sans/OpenSans-Semibold.ttf";
const string LayoutManager::LAYOUT_FONT_LIGHT =  "fonts/open-sans/OpenSans-Light.ttf";

LayoutManager::LayoutManager(): Manager(), m_previewMode(0)
{
	//Intentionally left empty
}


LayoutManager::~LayoutManager()
{
    ofLogNotice() <<"LayoutManager::Destructor";
}


void LayoutManager::setup()
{
	if(m_initialized)
		return;

    ofLogNotice() <<"LayoutManager::initialized";

	Manager::setup();

    //ofEnableArbTex();
    
    this->createTextVisuals();
    this->createSvgVisuals();
    this->createImageVisuals();
    
    this->setupBlur();
    this->setupMask();
    this->setupFbo();
	this->setupSpout();
    this->setupWindowFrames();

    //this->addVisuals();
   // ofDisableArbTex();
}
void LayoutManager::setupBlur()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    m_blur.setup(width, height);
  
}

void LayoutManager::setupMask()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    //ofEnableArbTex();
    //m_mask.allocate(width, height, ofxMask::LUMINANCE);
    string name = "output";
    AppManager::getInstance().getMaskManager().allocate(name, width, height);
    //ofDisableArbTex();
}


void LayoutManager::setupFbo()
{
    int margin = MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    
    m_fbo.allocate(width, height, GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    m_3dfbo.allocate(width, height, GL_RGBA);
    m_3dfbo.begin(); ofClear(0); m_3dfbo.end();
}

void LayoutManager::setupSpout()
{
    #if defined(TARGET_WIN32)
        string spoutName = AppManager::getInstance().getSettingsManager().getSpoutName();
        m_spoutSender.init(spoutName);
    #endif
}

void LayoutManager::resetWindowRects()
{
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float ratio = width/ height;
    float frame_width = ofGetWidth() - AppManager::getInstance().getGuiManager().getWidth() - 2*MARGIN;
    
    if(frame_width >= ofGetHeight())
    {
        m_windowRect.width = 3*frame_width/5 - 2*MARGIN;
        m_windowRect.height =  m_windowRect.width / ratio;
        
        m_3dWindowRect.width = 2*frame_width/5 - 2*MARGIN;
        m_3dWindowRect.height = m_3dWindowRect.width / ratio;
        
        m_windowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*MARGIN;
        m_windowRect.y = ofGetHeight()*0.5 - m_windowRect.height/2;
        
        m_3dWindowRect.x = m_windowRect.x + 2*MARGIN + m_windowRect.width;
        m_3dWindowRect.y =  ofGetHeight()*0.5 - m_3dWindowRect.height/2;
    }
    else
    {
        m_windowRect.width = frame_width - 2*MARGIN;
        m_windowRect.height =  m_windowRect.width / ratio;
        
        m_3dWindowRect.width = 3*m_windowRect.width/4;
        m_3dWindowRect.height = m_3dWindowRect.width / ratio;
        
        m_windowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*MARGIN;

        m_3dWindowRect.x = m_windowRect.x;
        m_3dWindowRect.y = m_windowRect.y + m_windowRect.height + 2*MARGIN  + m_textVisuals["3D"]->getHeight();
    }

}


void LayoutManager::setupWindowFrames()
{
    this->resetWindowRects();
    this->resetWindowFrames();
    
    float width = ofGetScreenWidth();
    float height = ofGetScreenHeight()/80;
    
    
    ofColor color = AppManager::getInstance().getSettingsManager().getColor("FrameRectangle");
    m_windowFrame.setColor(color);
    m_3dWindowFrame.setColor(color);
}

void LayoutManager::resetWindowFrames()
{
    m_windowFrame.setPosition(ofPoint( m_windowRect.x - FRAME_MARGIN, m_windowRect.y - FRAME_MARGIN, 0));
    m_windowFrame.setWidth(m_windowRect.width + 2*FRAME_MARGIN); m_windowFrame.setHeight(m_windowRect.height + 2*FRAME_MARGIN);
    
    m_3dWindowFrame.setPosition(ofPoint( m_3dWindowRect.x - FRAME_MARGIN, m_3dWindowRect.y - FRAME_MARGIN, 0));
    m_3dWindowFrame.setWidth(m_3dWindowRect.width + 2*FRAME_MARGIN); m_3dWindowFrame.setHeight(m_3dWindowRect.height + 2*FRAME_MARGIN);
}


void LayoutManager::update()
{
     ///ofEnableArbTex();
    
    this->updateFbos();
    this->updateMask();
	this->updateSpout();
    
     //ofDisableArbTex();
}

void LayoutManager::updateMask()
{
    string name = "output";
    AppManager::getInstance().getMaskManager().beginMask(name);
    
   // m_mask.beginMask();
        m_blur.begin();
            AppManager::getInstance().getModelManager().getMask().draw(0,0);
        m_blur.end();
        m_blur.draw();
    //m_mask.endMask();
    AppManager::getInstance().getMaskManager().endMask(name);
    
    AppManager::getInstance().getMaskManager().begin(name);
        ofClear(0);
         AppManager::getInstance().getSceneManager().draw();
        //ofDrawRectangle(100,100,500,500);
    AppManager::getInstance().getMaskManager().end(name);
}

void LayoutManager::updateFbos()
{
    this->updateOutputFbo();
    this->update3dFbo();
}

void LayoutManager::updateSpout()
{
    #if defined(TARGET_WIN32)
        m_spoutSender.send(m_fbo.getTexture());
    #endif
}

void LayoutManager::updateOutputFbo()
{
    string name = "output";
    
    ofEnableAlphaBlending();
    m_fbo.begin();
    ofPushStyle();
    ofClear(0, 0, 0);
    
       // AppManager::getInstance().getSceneManager().draw();
        AppManager::getInstance().getMaskManager().draw(name);
        //AppManager::getInstance().getSceneManager().draw();
       // m_mask.draw();
       // m_blur.draw();
    
    ofPopStyle();
    m_fbo.end();
    ofDisableAlphaBlending();
    
}

void LayoutManager::update3dFbo()
{
    ofEnableAlphaBlending();
    m_3dfbo.begin();
    ofClear(0, 0, 0);
        //ofBackgroundGradient(ofColor::gray, ofColor::black);
        if(m_previewMode == MASK){
            string name = "output";
            AppManager::getInstance().getMaskManager().drawMask(name);
            //m_mask.drawMasker();
            
            //m_blur.draw();
        }
        else if(m_previewMode == MODEL){
            AppManager::getInstance().getModelManager().getModel().draw(0,0);
        }
        else if(m_previewMode == WIREFRAME){
            AppManager::getInstance().getModelManager().getWireframe().draw(0,0);
        }
    
        else if(m_previewMode == SCENE){
            AppManager::getInstance().getSceneManager().draw();
        }
    
    m_3dfbo.end();
    ofDisableAlphaBlending();
    
}


void LayoutManager::createTextVisuals()
{
    float size = 20;
    float w = size*50;
    float h = size;
    float x =  m_windowRect.x + m_windowRect.getWidth()*0.5;
    float y =  m_windowRect.y - h - 2*MARGIN;
    ofPoint pos = ofPoint(x, y);
    string text = "Output";
    string fontName = LAYOUT_FONT_LIGHT;
    
    
    auto textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals[text] = textVisual;
    
    
    x =  m_3dWindowRect.x + m_3dWindowRect.getWidth()*0.5;
    y =  m_3dWindowRect.y - h - 2*MARGIN;
    text = "3D Preview";
    textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["3D"] = textVisual;
    
}


void LayoutManager::resetWindowTitles()
{
    float x =  m_windowRect.x + m_windowRect.getWidth()*0.5;
    float y =  m_windowRect.y -  m_textVisuals["Output"]->getHeight()*0.5 - MARGIN;
    ofPoint pos = ofPoint(x, y);
    m_textVisuals["Output"]->setPosition(pos);
    
    
    pos.x =  m_3dWindowRect.x + m_3dWindowRect.getWidth()*0.5;
    pos.y =  m_3dWindowRect.y - m_textVisuals["3D"]->getHeight()*0.5  - MARGIN;
    m_textVisuals["3D"]->setPosition(pos);
}


void LayoutManager::createSvgVisuals()
{
    ///To implement in case we have text visuals
}


void LayoutManager::createImageVisuals()
{
    //this->createBackground();
}

void LayoutManager::createBackground()
{
}

void LayoutManager::addVisuals()
{
    int depthLevel = -1;
    for(SvgMap::iterator it = m_svgVisuals.begin(); it!= m_svgVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
    
    for(TextMap::iterator it = m_textVisuals.begin(); it!= m_textVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
    
    for(ImageMap::iterator it = m_imageVisuals.begin(); it!= m_imageVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
}

void LayoutManager::onFullScreenChange(bool value)
{
    if(value){
        ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
    }
    else{
        
        float width = 4*MARGIN + 2*AppManager::getInstance().getGuiManager().getWidth();
        float height = AppManager::getInstance().getGuiManager().getHeight() + 2*MARGIN;
        ofSetWindowShape(width,height);
    }
}


void LayoutManager::draw()
{
    if(!m_initialized)
        return;
    
   //  ofEnableArbTex();
    this->drawFbos();
    this->drawText();
    // ofDisableArbTex();
    
}


void LayoutManager::drawText()
{
    for(auto textVisual: m_textVisuals){
        textVisual.second->draw();
    }
}

void LayoutManager::drawFbos()
{
    this->drawOutputFbo();
    this->draw3dFbo();
}


void LayoutManager::drawOutputFbo()
{
    m_windowFrame.draw();
    m_fbo.draw(m_windowRect.x,m_windowRect.y,m_windowRect.width,m_windowRect.height);
}

void LayoutManager::draw3dFbo()
{
    
    m_3dWindowFrame.draw();
    m_3dfbo.draw(m_3dWindowRect.x,m_3dWindowRect.y,m_3dWindowRect.width,m_3dWindowRect.height);
}


void LayoutManager::windowResized(int w, int h)
{
    if(!m_initialized){
        return;
    }
    
    this->resetWindowRects();
    this->resetWindowFrames();
    this->resetWindowTitles();
}

void LayoutManager::setFullScreen()
{
    ofSetWindowPosition(0,0);
    ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
}

void LayoutManager::onBlurScaleChange(float& value)
{
     m_blur.setScale(ofClamp(value, 0, 10));
}

