#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "transformer.h"

class debuggerFisheye: public transformer {
    public:
        debuggerFisheye(int videoWidth, int videoHeight, char* name);

        void draw();

    private:
        void setupGui();

        ofImage image;
        ofParameter<bool> calibrate;
        ofParameter<float> radius;
};
