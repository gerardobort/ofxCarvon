#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "transformer.h"

class transformerSphereComposedTexture: public transformer {
    public:
        transformerSphereComposedTexture(int videoWidth, int videoHeight, char* name);

        void update();
        void draw();

        void setInput(ofFbo* fbo1, ofFbo* fbo2) {
            input1 = *fbo1;
            input2 = *fbo2;
        }

    private:
        void setupGui();

        ofFbo input1;
        ofFbo input2;
        ofFbo texturePlane;
        ofShader shader;
        ofMesh mesh;
        ofImage image;

        ofParameter<bool> displayInput1;
        ofParameter<bool> displayInput2;
};
