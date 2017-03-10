#pragma once

#include "ofMain.h"

class processor {
    public:
        processor(int videoWidth = 640, int videoHeight = 480, char* name = "Unnamed");

        void update();
        void draw();
		bool isFrameNew();

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
