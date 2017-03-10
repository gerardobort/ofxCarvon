#pragma once

#define ASSETS_PATH "../../../../assets/"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCarvon.h"

#define RECORD_VIDEO_WIDTH 1920
#define RECORD_VIDEO_HEIGHT 1080

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
        sourceDevice* videoSource;
        transformerStandard* transformerStandard1;
        debuggerFisheye* debuggerFisheye1;
        transformerSphereTexture* transformerSphereTexture1;
        viewHalfSphere* viewHalfSphere1;

        ofFbo sourceCanvas;
        ofFbo sphericalCanvas;
        ofFbo viewportCanvas;

        bool shouldShowSettings;

        ofxPanel gui;
        ofParameter<bool> calibrate;
};
