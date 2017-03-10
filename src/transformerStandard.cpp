#include "transformerStandard.h"

//--------------------------------------------------------------
transformerStandard::transformerStandard(int videoWidth, int videoHeight, char* name)
    : transformer(videoWidth, videoHeight, name) {
    setupGui();
}

//--------------------------------------------------------------
void transformerStandard::begin(){
    ofPushMatrix();
        // center pivot
        ofTranslate(ofPoint(videoWidth/2.0, videoHeight/2.0));
            // apply basic transformations
            ofTranslate(ofPoint(translate->x, translate->y));
            ofRotate(rotate);
            ofScale(ofPoint(scale->x, scale->y));
        ofTranslate(ofPoint(-videoWidth/2.0, -videoHeight/2.0));
        // then .... flush buffer
}

//--------------------------------------------------------------
void transformerStandard::end(){
    ofPopMatrix();
}

//--------------------------------------------------------------
void transformerStandard::setupGui() {
    // TODO replace with: https://braitsch.github.io/ofxDatGui/
	parameters.add(translate.set("translate", ofVec2f(0, 0), ofVec2f(-videoWidth, -videoHeight), ofVec2f(videoWidth, videoHeight)));
	parameters.add(rotate.set("rotate", 0, -180, 180));
	parameters.add(scale.set("scale", ofVec2f(1, 1), ofVec2f(-4, -4), ofVec2f(4, 4)));
}
