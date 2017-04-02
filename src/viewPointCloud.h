#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "view.h"
#include <cmath>

class viewPointCloud: public view {
    public:
        viewPointCloud(int videoWidth, int videoHeight, char* name);

        void update();
        void draw();

        void setInput(ofFbo* _depthFbo, ofImage* _colorImage) {
            depthFbo = _depthFbo;
            colorImage = _colorImage;
        }

    private:
        void setupGui();

		ofFbo* depthFbo;
		ofImage* colorImage;
        ofMesh mesh;
        ofEasyCam sCam;

        ofParameter<int> step;
        ofParameter<float> zScale;
        ofParameter<float> dispersion;
        ofParameter<bool> drawWireframe;
        ofParameter<int> nearThreshold;
        ofParameter<int> farThreshold;
        ofParameter<ofPoint> pos;
};
