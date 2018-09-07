//
//  PanTiltLaser.h
//  pan-tilt
//
//  Created by Nestor Rubio Garcia on 02/09/2017.
//
//

/*
 Reporesents a pan and tilt rig with a laser.
 */

#pragma once

#include "PanTilt.h"

class PanTiltLaser : public PanTilt{

public:
    PanTiltLaser();
    
    void draw(float rayLength = 10000);
    
    ofParameter<bool> bLaserOn;
};
