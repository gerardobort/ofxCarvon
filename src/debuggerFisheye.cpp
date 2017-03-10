#include "debuggerFisheye.h"
// FIXME ofApp.h may be named differently depending on the project
#include "ofApp.h"

//--------------------------------------------------------------
debuggerFisheye::debuggerFisheye(int videoWidth, int videoHeight, char* name)
    : transformer(videoWidth, videoHeight, name) {
    setupGui();

    ofDirectory dir = ofDirectory(ASSETS_PATH);
    string path = dir.getAbsolutePath(); 
    ofLogNotice("ASSETS_PATH=", path);
    ofLogNotice("Loading Asset:", path + "/fish2sphere180.jpg");
    image.load(path + "/fish2sphere180.jpg");
}

//--------------------------------------------------------------
void debuggerFisheye::draw(){
    ofPushMatrix();
        if (calibrate) {
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            ofPushMatrix();
                // center pivot
                ofTranslate(ofPoint(videoWidth/2.0, videoHeight/2.0));
                    // apply basic transformations
                    ofScale(ofPoint(radius));
                ofTranslate(ofPoint(-videoWidth/2.0, -videoHeight/2.0));
                // flush buffer
                image.draw(0, 0, videoWidth, videoHeight);
            ofPopMatrix();
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        }
    ofPopMatrix();
}

//--------------------------------------------------------------
void debuggerFisheye::setupGui() {
    parameters.add(calibrate.set("calibrate", true));
    parameters.add(radius.set("radius", 1.0, 0, 2));
}
