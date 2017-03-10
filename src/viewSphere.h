#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "view.h"

class viewSphere: public view {
    public:
        viewSphere(int videoWidth, int videoHeight, char* name);

        void update();
        void draw();

        void setInput(ofFbo* fbo) {
            //input = *fbo;
		    sphere.mapTexCoordsFromTexture(fbo->getTexture());
        }

    private:
        void setupGui();

		ofSpherePrimitive sphere;
        ofBoxPrimitive box;
        ofEasyCam cam;

        ofParameter<bool> calibrate;
        ofParameter<int> sphereResolution;
        ofParameter<float> sphereRadius;
        ofParameter<float> sphereDistance;
};
