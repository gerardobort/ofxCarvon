#pragma once

#define ASSETS_PATH "../../../../assets/"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCarvon.h"
#include "ofxCv.h"

#define RECORD_VIDEO_WIDTH 1920
#define RECORD_VIDEO_HEIGHT 1080

#define VIEWPORT_WIDTH 640
#define VIEWPORT_HEIGHT 480

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

        void reloadStereoN(int& _value) {
            stereobm->setNDisparities(16*_value);
            stereobm->reload();
        }

        void reloadStereoS(int& _value) {
            stereobm->setSADWindowSize(4 + 2*(_value - 1) + 1);
            stereobm->reload();
        }

        void reloadStereoP1(int& _value) { stereobm->setP1(_value); stereobm->reload(); }
        void reloadStereoP2(int& _value) { stereobm->setP2(_value); stereobm->reload(); }
        void reloadDisp12MaxDiff(int& _value) { stereobm->setDisp12MaxDiff(_value); stereobm->reload(); }
        void reloadStereoPrefilterCap(int& _value) { stereobm->setPreFilterCap(_value); stereobm->reload(); }
        void reloadStereoUniquenessRatio(int& _value) { stereobm->setUniquenessRatio(_value); stereobm->reload(); }
        void reloadSpeckleWindowSize(int& _value) { stereobm->setSpeckleWindowSize(_value); stereobm->reload(); }
        void reloadSpeckleRange(int& _value) { stereobm->setSpeckleRange(_value); stereobm->reload(); }
        
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

        ofParameterGroup paramsStereo;
        ofParameter<int> indexSample;
        ofParameter<bool> calibrateStereo;
        ofParameter<bool> showStereoRectification;
        ofParameter<bool> swapCameras;
        ofParameter<bool> viewStereo;
        ofParameter<int> nDisparities;
        ofParameter<int> windowSize;

        ofParameter<int> P1;
        ofParameter<int> P2;
        ofParameter<int> disp12MaxDiff;
        ofParameter<int> preFilterCap;
        ofParameter<int> uniquenessRatio;
        ofParameter<int> speckleWindowSize;
        ofParameter<int> speckleRange;

        ofFbo sourceCanvas;
        ofFbo sphericalCanvas;
        ofFbo viewportCanvas;

        ofxCv::Stereo* stereobm;
        ofxCv::Camera* leftCamera;
        ofxCv::Camera* rightCamera;
        ofImage leftImage;
        ofImage rightImage;
        ofPolyline leftPoints;
        ofPolyline rightPoints;

        bool shouldShowSettings;

        ofxPanel gui;
        ofParameter<bool> calibrate;
};
