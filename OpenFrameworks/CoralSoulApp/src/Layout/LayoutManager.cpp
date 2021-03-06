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

LayoutManager::LayoutManager(): Manager(), m_previewMode(0), m_drawMode(0), m_useMask(true), m_moonSize(0), m_seaOpacity(1.0),m_dayOpacity(1.0), m_numWindParticles(800), m_sizeWindParticles(6.0), m_cloudsOpacity(1.0), m_blurValue(0.0), m_fadeTimeWindParticles(8)
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
    //m_blur.setup(width, height);
    
    
    ofFbo::Settings s;
    s.width = width;
    s.height = height;
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;
    
    m_blur.setup(s, false);
    m_blur.blurPasses = 3;
    m_blur.blurOffset = 0;
    m_blur.numBlurOverlays = 1;
    m_blur.blurOverlayGain = 255;
  
}

void LayoutManager::setupMask()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    string name = "output";
    AppManager::getInstance().getMaskManager().allocate(name, width, height);
}


void LayoutManager::setupFbo()
{
    int margin = MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    
    m_fbo.allocate(width, height, GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    m_previewFbo.allocate(width, height, GL_RGBA);
    m_previewFbo.begin(); ofClear(0); m_previewFbo.end();
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
        
        m_previewWindowRect.width = 2*frame_width/5 - 2*MARGIN;
        m_previewWindowRect.height = m_previewWindowRect.width / ratio;
        
        m_windowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*MARGIN;
        m_windowRect.y = ofGetHeight()*0.5 - m_windowRect.height/2;
        
        m_previewWindowRect.x = m_windowRect.x + 2*MARGIN + m_windowRect.width;
        m_previewWindowRect.y =  ofGetHeight()*0.5 - m_previewWindowRect.height/2;
    }
    else
    {
        m_windowRect.width = frame_width - 2*MARGIN;
        m_windowRect.height =  m_windowRect.width / ratio;
        
        m_previewWindowRect.width = 3*m_windowRect.width/4;
        m_previewWindowRect.height = m_previewWindowRect.width / ratio;
        
        m_windowRect.x = AppManager::getInstance().getGuiManager().getWidth()  + 3*MARGIN;

        m_previewWindowRect.x = m_windowRect.x;
        m_previewWindowRect.y = m_windowRect.y + m_windowRect.height + 2*MARGIN  + m_textVisuals["Preview"]->getHeight();
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
    m_previewWindowFrame.setColor(color);
}

void LayoutManager::resetWindowFrames()
{
    m_windowFrame.setPosition(ofPoint( m_windowRect.x - FRAME_MARGIN, m_windowRect.y - FRAME_MARGIN, 0));
    m_windowFrame.setWidth(m_windowRect.width + 2*FRAME_MARGIN); m_windowFrame.setHeight(m_windowRect.height + 2*FRAME_MARGIN);
    
    m_previewWindowFrame.setPosition(ofPoint( m_previewWindowRect.x - FRAME_MARGIN, m_previewWindowRect.y - FRAME_MARGIN, 0));
    m_previewWindowFrame.setWidth(m_previewWindowRect.width + 2*FRAME_MARGIN); m_previewWindowFrame.setHeight(m_previewWindowRect.height + 2*FRAME_MARGIN);
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
   // m_blur.beginDrawScene();
       // ofClear(0);
        AppManager::getInstance().getModelManager().getMask().draw(0,0);
    //m_blur.endDrawScene();
    //m_blur.performBlur();
    //m_blur.drawBlurFbo();
    
    AppManager::getInstance().getMaskManager().endMask(name);
   
    AppManager::getInstance().getMaskManager().begin(name);
        ofClear(0);
        AppManager::getInstance().getSceneManager().draw();
    AppManager::getInstance().getMaskManager().end(name);
}

void LayoutManager::updateFbos()
{
    this->updateOutputFbo();
    this->updatePreviewFbo();
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
    ofClear(0, 0, 0);
        if(m_useMask){
            AppManager::getInstance().getMaskManager().draw(name);
        }
        else{
            AppManager::getInstance().getSceneManager().draw();
        }

    m_fbo.end();
    ofDisableAlphaBlending();
    
}

void LayoutManager::updatePreviewFbo()
{
    ofEnableAlphaBlending();
    m_previewFbo.begin();
    ofClear(0, 0, 0);

        if(m_previewMode == MASK){
            string name = "output";
            AppManager::getInstance().getMaskManager().drawMask(name);
            //AppManager::getInstance().getModelManager().getMask().draw(0,0);
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
    
        else if(m_previewMode == NOISE){
            //AppManager::getInstance().getNoiseManager().draw(m_fbo.getWidth(), m_fbo.getHeight());
            AppManager::getInstance().getNoiseManager().getFbo().draw(0,0);
        }
    
    m_previewFbo.end();
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
    
    
    x =  m_previewWindowRect.x + m_previewWindowRect.getWidth()*0.5;
    y =  m_previewWindowRect.y - h - 2*MARGIN;
    text = "Preview";
    textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h,true));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_textVisuals["Preview"] = textVisual;
    
}


void LayoutManager::resetWindowTitles()
{
    float x =  m_windowRect.x + m_windowRect.getWidth()*0.5;
    float y =  m_windowRect.y -  m_textVisuals["Output"]->getHeight()*0.5 + MARGIN;
    ofPoint pos = ofPoint(x, y);
    m_textVisuals["Output"]->setPosition(pos);
    
    
    pos.x =  m_previewWindowRect.x + m_previewWindowRect.getWidth()*0.5;
    pos.y =  m_previewWindowRect.y - m_textVisuals["Preview"]->getHeight()*0.5  + MARGIN;
    m_textVisuals["Preview"]->setPosition(pos);
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
    
    switch (m_drawMode)
    {
        case DRAW_NORMAL:  this->drawNormal(); break;
        case DRAW_OUTPUT:  this->drawOutput(); break;
        case DRAW_PREVIEW:  this->drawPreview(); break;
        default: this->drawNormal(); break;
    }
   
}

void LayoutManager::drawOutput()
{	
    m_fbo.draw(0,0, ofGetWidth(), ofGetHeight());
}

void LayoutManager::drawPreview()
{
    m_previewFbo.draw(0,0, ofGetWidth(), ofGetHeight());
}

void LayoutManager::drawNormal()
{
    this->drawFbos();
    this->drawText();
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
    this->drawPreviewFbo();
}


void LayoutManager::drawOutputFbo()
{
    m_windowFrame.draw();
    m_fbo.draw(m_windowRect.x,m_windowRect.y,m_windowRect.width,m_windowRect.height);
}

void LayoutManager::drawPreviewFbo()
{
    
    m_previewWindowFrame.draw();
    m_previewFbo.draw(m_previewWindowRect.x,m_previewWindowRect.y,m_previewWindowRect.width,m_previewWindowRect.height);
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
    m_blurValue = 4*value;
    m_blur.blurOffset = m_blurValue;
}

void LayoutManager::toggleDrawMode()
{
    if(m_drawMode == 0)
    {
        m_drawMode = 1;
    }
    else{
        m_drawMode = 0;
    }
    
}


