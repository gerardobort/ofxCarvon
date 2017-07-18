#pragma once

#define ASSETS_PATH "../../../../assets/"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCarvon.h"
#include "ofxCv.h"

#define RECORD_VIDEO_WIDTH 1920
#define RECORD_VIDEO_HEIGHT 1080

#define VIEWPORT_WIDTH 1280
#define VIEWPORT_HEIGHT 800

#define NUM_CALIBRATION_SAMPLES 15

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
        ofParameterGroup paramsSource;
        sourceDevice* videoSource;
        transformerStandard* _transformerStandard;

        ofParameterGroup paramsView;
        debuggerFisheye* _debuggerFisheye;
        transformerSphereTexture* _transformerSphereTexture;
        transformerChromaKey* _transformerChromaKey;
        viewHalfSphere* _viewHalfSphere;

        ofFbo sourceCanvas;
        ofFbo chromaCanvas;
        ofFbo sphericalCanvas;
        ofFbo viewportCanvas;

        bool shouldShowSettings;

        ofxPanel gui;
        ofParameter<bool> testRaw;
        ofParameter<bool> calibrate;
};
