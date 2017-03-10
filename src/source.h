#pragma once

#include "processor.h"

class source: public processor {
    public:
        source(int videoWidth = 640, int videoHeight = 480, char* name = "Unnamed Video Source")
        : processor(videoWidth, videoHeight, name) { }

        void draw();
        void draw(int x, int y);
        void draw(int x, int y, int w, int h);

};
