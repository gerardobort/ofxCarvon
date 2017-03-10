/*
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "fisheyeStream.h"
#include "sphereView.h"
#include "IPVideoGrabber.h"

//using namespace ofx;

class sbsView {
    public:
        sbsView(sphereView* sphereL, sphereView* sphereR);
        void update();
        void draw();
		bool isFrameNew();
        
        ofParameterGroup parameters;

        sphereView sphereL;
        sphereView sphereR;
        ofFbo output;

    private:
        void setupGui();
        ofParameter<bool> displayVideoSource;
        ofParameter<float> sphereRadius;
        ofParameter<float> sphereDistance;
        ofParameter<float> sphereOffsetX;
        ofEasyCam cam;
};
*/
