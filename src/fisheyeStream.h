#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "IPVideoGrabber.h"
#include "ofxCv.h"

//using namespace ofx;

class fisheyeStream {
    public:
        fisheyeStream(int cameraWidth = 1920, int cameraHeight = 1080, char* url1 = "", char* name = "Unnamed");
        void update();
        void draw();
		bool isFrameNew();
        
        ofParameterGroup parameters;

        ofFbo input;
        ofFbo texturePlane;
        ofFbo output;

        int cameraWidth;
        int cameraHeight;
        char* name;
        char* url;

        ofParameter<int> outputOpacity;

    private:
        ofShader shader;
        ofMesh mesh;
        ofImage image;
		std::shared_ptr<ofx::Video::IPVideoGrabber> grabber;

        void setupGui();
        ofParameter<int> cameraDeviceId;
        ofParameter<float> visibleRadius;
        ofParameter<bool> rectify;
        ofParameter<bool> calibrate;
        ofParameter<float> rectifyFovFactor;
        ofParameter<int> rectifyOverflow;
        ofParameter<int> rectifyWidth;
        ofParameter<int> rectifyHeight;
        ofParameter<float> inputOffsetX;
        ofParameter<float> inputOffsetY;
        ofParameter<bool> rotate;
        ofParameter<bool> mirrorY;
        ofParameter<bool> mirrorX;
        ofParameter<bool> useCanny;
        ofParameter<int> cannyThreshold1;
        ofParameter<int> cannyThreshold2;
};
