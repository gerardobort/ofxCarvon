#pragma once

#include "processor.h"

class transformer: public processor {
    public:
        transformer(int videoWidth = 640, int videoHeight = 480, char* name = "Unnamed Transformer")
        : processor(videoWidth, videoHeight, name) { }

};
