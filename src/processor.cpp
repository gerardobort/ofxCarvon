#include "processor.h"

//--------------------------------------------------------------
processor::processor(int videoWidth, int videoHeight, char* name)
    : videoWidth(videoWidth), videoHeight(videoHeight), name(name) {
    
    setupGui();
}

//--------------------------------------------------------------
void processor::update(){
}

//--------------------------------------------------------------
void processor::draw(){
}

//--------------------------------------------------------------
bool processor::isFrameNew() {
}

//--------------------------------------------------------------
void processor::setupGui() {
    parameters.setName(name);
}
