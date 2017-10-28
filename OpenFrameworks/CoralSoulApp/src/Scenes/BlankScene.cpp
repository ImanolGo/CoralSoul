/*
 *  BlankScene.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 28/10/17.
 *
 */


#include "BlankScene.h"
#include "RectangleVisual.h"

BlankScene::BlankScene(): ofxScene("BLANK"){}

void BlankScene::setup() {
    ofLogNotice("BlankScene::setup");
}

void BlankScene::update() {
    
}

void BlankScene::draw() {
    ofBackground(0,0,0);
    
}

void BlankScene::willFadeIn() {
     ofLogNotice("BlankScene::willFadeIn");
}

void BlankScene::willDraw() {
    ofLogNotice("BlankScene::willDraw");
}

void BlankScene::willFadeOut() {
    ofLogNotice("BlankScene::willFadeOut");
}

void BlankScene::willExit() {
    ofLogNotice("BlankScene::willExit");
}
