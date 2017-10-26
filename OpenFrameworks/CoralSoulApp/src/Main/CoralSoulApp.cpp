/*
 *  CoralSoulApp.cpp
 *  CoralSoulApp
 *
 *  Created by Imanol Gomez on 26/10/17.
 *
 */



#include "AppManager.h"

#include "CoralSoulApp.h"

//--------------------------------------------------------------
void CoralSoulApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void CoralSoulApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void CoralSoulApp::draw(){
    AppManager::getInstance().draw();
}

void CoralSoulApp::exit()
{
    ofLogNotice() <<"CoralSoulApp::exit";

}

//--------------------------------------------------------------
void CoralSoulApp::keyPressed(int key){

}

//--------------------------------------------------------------
void CoralSoulApp::keyReleased(int key){

}

//--------------------------------------------------------------
void CoralSoulApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void CoralSoulApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void CoralSoulApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void CoralSoulApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void CoralSoulApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);


}

//--------------------------------------------------------------
void CoralSoulApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void CoralSoulApp::dragEvent(ofDragInfo dragInfo){

}
