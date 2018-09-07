#include "ofApp.h"


void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAntiAliasing();
    ofBackground(0);
    
    gui.setup();
    gui.add(panTilt.parameters);
    
    camera.setDistance(20);
}

void ofApp::update(){
    
}


void ofApp::draw(){
    ofEnableDepthTest();
    camera.begin();
    {
        drawGroundPlane();
        panTilt.draw();
        
        // draw the head
//        ofTranslate(panTilt.getHeadPosition());
//        ofDrawSphere(0,0, 0.5);
    }
    camera.end();
    ofDisableDepthTest();
    
    gui.draw();
}


void ofApp::drawGroundPlane() {
    ofPushMatrix();
    {
        ofTranslate(0, -0.01, 0);
        ofRotateZ(90);
        
        ofPushStyle();
        ofSetColor(50);
        ofDrawGridPlane();
        ofPopStyle();
    }
    ofPopMatrix();
}


void ofApp::keyPressed(int key){

}
