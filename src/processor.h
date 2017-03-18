#pragma once

#include "ofMain.h"

class processor {
    public:
        processor(int videoWidth = 640, int videoHeight = 480, char* name = "Unnamed Processor");

        void update();
        void draw();
		bool isFrameNew();

        ofParameterGroup* getParametersReference(void) {
            return &parameters;
        }

        ofParameterGroup getParameters(void) {
            return parameters;
        }

    protected:
        int videoWidth;
        int videoHeight;
        char* name;

        ofParameterGroup parameters;

        void setupGui();
};
