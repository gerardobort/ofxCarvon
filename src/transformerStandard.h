#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "transformer.h"

class transformerStandard: public transformer {
    public:
        transformerStandard(int videoWidth, int videoHeight, char* name);

        void begin();
        void end();

    private:
        void setupGui();

        ofParameter<ofVec2f> translate;
        ofParameter<int> rotate;
        ofParameter<ofVec2f> scale;
};
