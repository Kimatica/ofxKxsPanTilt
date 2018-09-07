#pragma once

#include "ofMain.h"
#include "PanTiltLaser.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    
    PanTiltLaser panTilt;
    
    ofEasyCam camera;
    
    ofxPanel gui;
    
    void drawGroundPlane();
};
