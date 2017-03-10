#pragma once

#include "processor.h"

class view: public processor {
    public:
        view(int videoWidth = 640, int videoHeight = 480, char* name = "Unnamed View")
        : processor(videoWidth, videoHeight, name) { }

};
