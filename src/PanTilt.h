//
//  panTilt.h
//  pan-tilt
//
//  Created by Nestor Rubio Garcia on 01/12/2017.
//
//

#pragma once

#include "ofMain.h"
#include "ofNode.h"


class PanTilt {

public:
    PanTilt();
    
    ofParameterGroup parameters;
    ofParameter<float> anglePan;
    ofParameter<float> angleTilt;
    
    void setPanRange(float minAngle, float maxAngle, float zeroAngle);
    void setPanOffset(float x, float y, float z);
    void setTiltRange(float minAngle, float maxAngle, float zeroAngle);
    void setTiltOffset(float x, float y, float z);
    void setRadius(float radius);
    
    void setTransform(ofVec3f position, float angle, ofVec3f rotAxis);
    void setTransform(ofMatrix4x4 transform);
    const ofMatrix4x4& getTransform();
    
    bool lookAtPoint(ofVec3f target);
    
    ofVec3f getHeadPosition();      // global space
    ofVec3f getHeadPositionLocal(); // local space
    ofVec3f getHeadDirection();     // unit vector
    
    // percentage of the angles [0..1]
    float getPanPct();
    float getTiltPct();
    
    // ranges (angles in degrees)
    float getRadius();
    float getPanMin();
    float getPanMax();
    float getPanZero();
    float getTiltMin();
    float getTiltMax();
    float getTiltZero();
    
    virtual void update();
    virtual void draw();
    
protected:
    float radius;
    ofVec3f offsetPan;
    float panMin_angle;
    float panMax_angle;
    float panZero_angle;
    ofVec3f offsetTilt;
    float tiltMin_angle;
    float tiltMax_angle;
    float tiltZero_angle;
    
    // position and direction of the head in world (aka global) reference frame
    // default orientation is looking down the positive z-axis.
    ofVec3f headPosition;
    ofVec3f headDirection;
    ofVec3f headPositionLocal;
    
    float panPct_angle;
    float tiltPct_angle;
    
    void onParameterChange(ofAbstractParameter& e);
    void updateValues();
    
    bool isDirty;
    
    ofMatrix4x4 transform; // model transform
    
    // --------------------
    // drawign
    // --------------------
    
    void updateDrawing();
    
    void drawAxis();
    void drawPanRotation();
    void drawTiltRotation();
    void drawPanRange();
    void drawTiltRange();
   
    ofPath arcPanRange;
    ofPath arcTiltRange;
    ofPath arcPanAngle;
    ofPath arcTiltAngle;
};
