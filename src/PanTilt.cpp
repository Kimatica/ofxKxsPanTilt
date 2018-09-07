//
//  panTilt.cpp
//  pan-tilt
//
//  Created by Nestor Rubio Garcia on 01/12/2017.
//
//

#include "PanTilt.h"


PanTilt::PanTilt() {
    ofAddListener(parameters.parameterChangedE(), this, &PanTilt::onParameterChange);
    
    parameters.setName("pan tilt");
    parameters.add(anglePan.set("pan_angle", 0, -180, 180));
    parameters.add(angleTilt.set("tilt_angle", 0, -180, 180));
    
    setRadius(6);
    setPanRange(-180, 180, 0);
    setPanOffset(0, 6, 0);
    setTiltRange(-180, 180, 0);
    setTiltOffset(0, 0, 0);
}

void PanTilt::setTransform(ofVec3f position, float angle, ofVec3f rotAxis) {
    ofMatrix4x4 translation = ofMatrix4x4::newTranslationMatrix(position);
    ofMatrix4x4 rotation = ofMatrix4x4::newRotationMatrix(angle, rotAxis);
    transform = rotation * translation; // (oF premult) transformations happen from left to right
}

void PanTilt::setTransform(ofMatrix4x4 transform) {
    this->transform = transform;
}

const ofMatrix4x4& PanTilt::getTransform() {
    return transform;
}

bool PanTilt::lookAtPoint(ofVec3f target) {
    // ----
    // pan
    // ----
    // calculate pan angle using XZ plane
    
    // brings the target from world space into this object's local space
    ofVec3f targetLocal = target * transform.getInverse();
    
    // angleA
    ofVec2f heading (0, 1); // looking down
    ofVec2f offset2D (offsetPan.x, offsetPan.z);
    ofVec2f target2D (targetLocal.x, targetLocal.z);
    ofVec2f centerToTarget = target2D - offset2D;
    float angleA = heading.angle(centerToTarget);
    
    // angleB
    heading.rotate(angleA);
// TODO: we need to use tiltOffset here!
// actually, we only need offsetTilt.x component
    offset2D.rotate(angleA);
    ofVec2f offsetToTarget = target2D - offset2D;
////
    float angleB = heading.angle(offsetToTarget);
    
    // final pan angle
    float angleAB = angleA + angleB;
    
    // negate
    anglePan = -angleAB;
    
    // check out of range
    if (anglePan > panMax_angle || anglePan < panMin_angle) {
        anglePan = ofClamp(anglePan, panMin_angle, panMax_angle);
        return false;
    }
    
    // ----
    // tilt
    // ----
    // calculate tilt angle using ZY plane
    
    // we need to apply the reverse pan tranformation to align the point
    // with the the ZY plane of the tilt
    ofMatrix4x4 translation = ofMatrix4x4::newTranslationMatrix (offsetPan);
    ofMatrix4x4 rotation = ofMatrix4x4::newRotationMatrix (anglePan, ofVec3f(0,1,0));
    ofMatrix4x4 panMatrix = rotation * translation;
    
    // align the point with tilt's plane
    // and then bring the point to this object's local space
    targetLocal = target * panMatrix.getInverse() * transform.getInverse();

    // angleC
    heading.set (1, 0); // looking right
    offset2D.set (offsetTilt.z, offsetTilt.y); // to calculate this angle only offsetTilt.y is relevant
    target2D.set (targetLocal.z, targetLocal.y);
    centerToTarget = target2D - offset2D;
    float angleC = heading.angle(centerToTarget);

    // angleB
    heading.rotate(angleC);
    offset2D.rotate(angleC);
    offsetToTarget = target2D - offset2D;
    float angleD = heading.angle(offsetToTarget);

    // final angle
    float angleCD = angleC + angleD;

    // negate
    angleTilt = -angleCD;
    
    // check out of range
    if (angleTilt > tiltMax_angle || angleTilt < tiltMin_angle) {
        angleTilt = ofClamp(angleTilt, tiltMin_angle, tiltMax_angle);
        return false;
    }
    
    return true;
}

void PanTilt::update() {
    if (isDirty) {
        updateValues();
        updateDrawing();
        isDirty = false;
    }
}

void PanTilt::setPanRange(float minAngle, float maxAngle, float zeroAngle) {
    panMin_angle = minAngle;
    panMax_angle = maxAngle;
    panZero_angle = zeroAngle;
    anglePan.setMin(minAngle);
    anglePan.setMax(maxAngle);
    isDirty = true;
}

void PanTilt::setPanOffset(float x, float y, float z) {
    offsetPan = ofVec3f(x,y,z);
    isDirty = true;
}

void PanTilt::setTiltRange(float minAngle, float maxAngle, float zeroAngle) {
    tiltMin_angle = minAngle;
    tiltMax_angle = maxAngle;
    tiltZero_angle = zeroAngle;
    angleTilt.setMin(minAngle);
    angleTilt.setMax(maxAngle);
    isDirty = true;
}

void PanTilt::setTiltOffset(float x, float y, float z) {
    offsetTilt = ofVec3f(x,y,z);
    isDirty = true;
}

void PanTilt::setRadius(float r) {
    radius = r;
    isDirty = true;
}

ofVec3f PanTilt::getHeadPosition() {
    return headPosition;
}

ofVec3f PanTilt::getHeadPositionLocal() {
    return headPositionLocal;
}

ofVec3f PanTilt::getHeadDirection() {
    return headDirection;
}

float PanTilt::getPanPct() {
    return panPct_angle;
}

float PanTilt::getTiltPct() {
    return tiltPct_angle;
}

float PanTilt::getRadius() {
    return radius;
}

float PanTilt::getPanMin() {
    return panMin_angle;
}

float PanTilt::getPanMax() {
    return panMax_angle;
}

float PanTilt::getPanZero() {
    return panZero_angle;
}

float PanTilt::getTiltMin() {
    return tiltMin_angle;
}

float PanTilt::getTiltMax() {
    return tiltMax_angle;
}

float PanTilt::getTiltZero() {
    return tiltZero_angle;
}

void PanTilt::updateValues() {
    // note that we are using oF's row vectors here,
    // order of transformations is left to right
    
    // default orientation is looking down the positive z-axis.
    ofVec3f tail(0,0,0);
    ofVec3f head(0,0,radius);
    
    ofMatrix4x4 panMatrix;
    ofMatrix4x4 tiltMatrix;
    ofMatrix4x4 translation;
    ofMatrix4x4 rotation;
    
    translation.makeTranslationMatrix(offsetPan);
    rotation.makeRotationMatrix(anglePan, ofVec3f(0,1,0));
    panMatrix = rotation * translation;
    
    translation.makeTranslationMatrix(offsetTilt);
    rotation.makeRotationMatrix(angleTilt, ofVec3f(1,0,0));
    tiltMatrix = rotation * translation;
    
    // local space
    ofMatrix4x4 localTransform(tiltMatrix * panMatrix);
    headPositionLocal = head * localTransform;
    
    // combine all tranformations (world space)
    ofMatrix4x4 t(tiltMatrix * panMatrix * transform);
    
    head = head * t;
    tail = tail * t;
    
    headPosition = head;
    headDirection = (head - tail).normalize();
    
    panPct_angle =  anglePan / (panMax_angle - panMin_angle);
    tiltPct_angle =  angleTilt / (tiltMax_angle - tiltMin_angle);
}

void PanTilt::onParameterChange(ofAbstractParameter &e) {
    updateValues();
}

// --------------------
// drawing
// --------------------

void PanTilt::draw() {
    ofPushStyle();
    ofSetColor(255);
    ofPushMatrix();
    {
        // apply object transform
        ofMultMatrix(transform);
        
        drawAxis();
        
        // pan
        ofTranslate(offsetPan);
        drawPanRange();
        
        ofRotateY(anglePan);
        drawPanRotation();
        
        // tilt
        ofTranslate(offsetTilt);
        drawTiltRange();
        ofRotateX(angleTilt);
        drawTiltRotation();
        
        // arrow
        ofSetLineWidth(2);
        ofSetColor(ofColor::yellow);
        ofDrawBox(0.5); // tail
        ofDrawArrow(ofVec3f(), ofVec3f(0,0,radius), radius/10);
    }
    ofPopMatrix();
    ofPopStyle();
}

void PanTilt::updateDrawing() {
    arcPanRange.setFilled(false);
    arcPanRange.setStrokeWidth(1);
    arcPanRange.setStrokeColor(ofColor::darkRed);
    arcPanRange.arc(ofPoint(), radius, radius, panMin_angle, panMax_angle);
    arcPanRange.close();
    
    arcPanAngle.setFillColor(ofColor::darkRed);
    
    arcTiltRange.setFilled(false);
    arcTiltRange.setStrokeWidth(1);
    arcTiltRange.setStrokeColor(ofColor::darkGreen);
    arcTiltRange.arc(ofPoint(), radius, radius, tiltMin_angle, tiltMax_angle);
    arcTiltRange.close();
    
    arcTiltAngle.setFillColor(ofColor::darkGreen);
}

void PanTilt::drawPanRange() {
    ofPushMatrix();
    {
        ofRotateY(-90);
        ofRotateX(90);
        arcPanRange.draw();
    }
    ofPopMatrix();
}

void PanTilt::drawPanRotation() {
    arcPanAngle.clear();
    if (anglePan > 0)
        arcPanAngle.arc(ofPoint(), radius, radius, 0, anglePan);
    else if (anglePan < 0)
        arcPanAngle.arcNegative(ofPoint(), radius, radius, 0, anglePan);
    arcPanAngle.close();
    ofPushMatrix();
    {
        ofRotateY(-90);
        ofRotateX(90);
        arcPanAngle.draw();
    }
    ofPopMatrix();
    
    ofPushStyle();
    ofSetColor(ofColor::red);
    ofDrawLine(ofVec3f(-2,0,0), ofVec3f(2,0,0));
    ofDrawLine(ofVec3f(0,0,-2), ofVec3f(0,0,2));
    ofPopStyle();
}

void PanTilt::drawTiltRange() {
    ofPushMatrix();
    {
        ofRotateY(-90);
        ofRotateZ(90);
        arcTiltRange.draw();
    }
    ofPopMatrix();
}

void PanTilt::drawTiltRotation() {
    arcTiltAngle.clear();
    if (angleTilt > 0)
        arcTiltAngle.arc(ofPoint(), radius, radius, 0, angleTilt);
    else if (angleTilt < 0)
        arcTiltAngle.arcNegative(ofPoint(), radius, radius, 0, angleTilt);
    arcTiltAngle.close();
    ofPushMatrix();
    {
        ofRotateY(-90);
        arcTiltAngle.draw();
    }
    ofPopMatrix();
    
    ofPushStyle();
    ofSetColor(ofColor::green); // darkGray
    ofDrawLine(ofVec3f(0,-2,0), ofVec3f(0,2,0));
    ofDrawLine(ofVec3f(0,0,-2), ofVec3f(0,0,2));
    ofPopStyle();
}

void PanTilt::drawAxis() {
    float size = 3;
    float scale = radius / 8.0f;
    
    // origin
    ofDrawAxis(size);
    
    ofPushStyle();
    ofSetColor(ofColor::blue);
    ofPushMatrix();
    {
        ofTranslate(0, 0, size);
        ofRotateX(-90);
        ofDrawCone(0, 0, 0, scale*0.5, scale);
    }
    ofPopMatrix();
    ofSetColor(ofColor::red);
    ofPushMatrix();
    {
        ofTranslate(size, 0, 0);
        ofRotateZ(90);
        ofDrawCone(0, 0, 0, scale*0.5, scale);
    }
    ofPopMatrix();
    ofSetColor(ofColor::green);
    ofPushMatrix();
    {
        ofTranslate(0, size, 0);
        ofRotateX(180);
        ofDrawCone(0, 0, 0, scale*0.5, scale);
    }
    ofPopMatrix();
    ofPopStyle();
}
