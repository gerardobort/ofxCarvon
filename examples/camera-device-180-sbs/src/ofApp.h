#pragma once

#define ASSETS_PATH "../../../../assets/"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCarvon.h"

#define RECORD_VIDEO_WIDTH 1280
#define RECORD_VIDEO_HEIGHT 720

#define VIEWPORT_WIDTH 1280
#define VIEWPORT_HEIGHT 720

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();
        void exit();
        
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
    private:
        ofParameterGroup paramsLeft;
        sourceDevice* videoSourceLeft;
        transformerStandard* transformerStandardLeft;

        ofParameterGroup paramsRight;
        sourceDevice* videoSourceRight;
        transformerStandard* transformerStandardRight;

        ofParameterGroup paramsView;
        debuggerFisheye* _debuggerFisheye;
        transformerSphereTexture* _transformerSphereTexture;
        viewHalfSphere* _viewHalfSphere;

        ofFbo sourceCanvas;
        ofFbo sphericalCanvas;
        ofFbo viewportCanvas;

        bool shouldShowSettings;

        ofxPanel gui;
        ofParameter<bool> calibrate;
};
