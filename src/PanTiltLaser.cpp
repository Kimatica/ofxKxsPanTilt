//
//  PanTiltLaser.cpp
//  pan-tilt
//
//  Created by Nestor Rubio Garcia on 02/09/2017.
//
//

#include "PanTiltLaser.h"

PanTiltLaser::PanTiltLaser() {
    parameters.add(bLaserOn.set("laser on", false));
}


void PanTiltLaser::draw(float rayLength) {
    PanTilt::draw();
    
    if(bLaserOn) {
        ofVec3f tail(getHeadPosition());
        ofVec3f head(tail + getHeadDirection() * rayLength);
        
        ofPushStyle();
        ofSetColor(ofColor::green);
        ofDrawLine(tail, head);
        ofPopStyle();
    }
}
