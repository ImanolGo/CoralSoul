/*
 *  LayoutManager.h
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#pragma once

#include "Manager.h"
#include "TextVisual.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"
#include "ofxFboBlur.h"

#if defined(TARGET_WIN32)
#include "ofxSpout.h"
#endif




//========================== class LayoutManager ==============================
//============================================================================
/** \class LayoutManager LayoutManager.h
 *	\brief Class managing the layout of the application
 *	\details It creates an places al the text and elements regarding the layout
 */

enum ScreenMode{
    DRAW_NORMAL = 0,
    DRAW_OUTPUT,
    DRAW_PREVIEW
};

class LayoutManager: public Manager
{
    
    enum PreviewMode{
        MASK,
        MODEL,
        WIREFRAME,
        SCENE,
        NOISE
    };

    
public:
    
    static const int MARGIN;
    static const int FRAME_MARGIN;
    static const string LAYOUT_FONT;
    static const string LAYOUT_FONT_LIGHT;

    //! Constructor
    LayoutManager();

    //! Destructor
    ~LayoutManager();

    //! Set-up the layout
    void setup();
    
    //! Update the layout
    void update();
    
    //! Draw the layout
    void draw();
    
    void onFullScreenChange(bool value);
    
    void windowResized(int w, int h);
    
    const ofRectangle& getWindowRect() {return m_windowRect;}
    
    void setFullScreen();
    
    const ofFbo& getCurrentFbo(){return m_fbo;}
    
    void onSetPreviewMode(int mode) {m_previewMode = mode;}
    
    void onBlurScaleChange(float& value);

	void onMoonPositionXChange(float& value) { m_moonPosition.x = value; }

	void onMoonPositionYChange(float& value) { m_moonPosition.y = value; }

	void onMoonSizeChange(float& value) { m_moonSize= value; }
    
    void onNumWindParticlesChange(int& value) { m_numWindParticles= value; }
    
    void onSizeWindParticlesChange(float& value) { m_sizeWindParticles= value; }
    
    void onSeaOpacityChange(float& value) { m_seaOpacity= value; }
    
    void onCloudsOpacityChange(float& value) { m_cloudsOpacity= value; }

	const ofVec2f& getMoonPosition() { return m_moonPosition; }

	const float& getMoonSize() { return m_moonSize; }
    
    const float& getSeaOpacity() { return m_seaOpacity; }
    
    const float& getCloudsOpacity() { return m_cloudsOpacity; }
    
    const float& getSizeWindParticles() { return m_sizeWindParticles; }
    
    const int& getNumWindParticles() { return m_numWindParticles; }
    
    void onMaskChange(bool value) {m_useMask = value;}
    
    void toggleDrawMode(int mode) {m_drawMode = mode;}
    
    void toggleDrawMode();
    
    int getDrawMode() {return m_drawMode;}
    
    float getBlurValue() {return m_blurValue;}

private:


    //! Create the text visuals
    void createTextVisuals();

    //! Create the image visuals
    void createImageVisuals();

    //! Create the background Image
    void createBackground();


    //! Add all visuals as overlays
    void addVisuals();

    void setupMask();
    
    void setupBlur();
    
    void setupFbo();

	void setupSpout();
    
    void updateFbos();
    
    void updateOutputFbo();
    
    void updatePreviewFbo();
    
    void updateMask();

	void updateSpout();
    
    void drawOutput();
    
    void drawNormal();
    
    void drawPreview();
    
    void drawFbos();
    
    void drawOutputFbo();
    
    void drawPreviewFbo();
    
    void drawText();
    
    void resetWindowRects();
    
    void resetWindowFrames();
    
    void resetWindowTitles();
    
    void setupWindowFrames();
private:


    typedef  map<string, ofPtr<TextVisual> >      TextMap;            ///< defines a map of Text attached to an identifier
    typedef  map<string, ofPtr<ImageVisual>  >    ImageMap;           ///< defines a map of ImageVisual Map attached to an identifier
  
    TextMap             m_textVisuals;             ///< map storing the text visuals attached to a name
    ImageMap            m_imageVisuals;            ///< map storing the image visuals attached to a name
    
    ofRectangle         m_windowRect;
    RectangleVisual     m_windowFrame;
    RectangleVisual     m_previewWindowFrame;
    ofRectangle         m_previewWindowRect;
    
    int                 m_drawMode;
    
    ofFbo               m_fbo;
    ofFbo               m_previewFbo;
    ofColor             m_color;
    
    int                 m_previewMode;
    ofxFboBlur          m_blur;
    bool                m_useMask;
	ofVec2f				m_moonPosition;
	float				m_moonSize;
    
    float               m_seaOpacity;
    float               m_cloudsOpacity;
    int                 m_numWindParticles;
    float               m_sizeWindParticles;
    float               m_blurValue;
	
    
    #if defined(TARGET_WIN32)
        ofxSpout::Sender    m_spoutSender;
    #endif
};

//==========================================================================


