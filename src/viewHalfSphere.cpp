#include "viewHalfSphere.h"

//--------------------------------------------------------------
viewHalfSphere::viewHalfSphere(int videoWidth, int videoHeight, char* name, int textureSideSize)
    : view(videoWidth, videoHeight, name), textureSideSize(textureSideSize) {
    setupGui();

    sphereDome.setup(0, M_PI, 0, M_PI, sphereStep, sphereStep);
    sphereDome.setTextureSize(ofPoint(textureSideSize));
}

//--------------------------------------------------------------
void viewHalfSphere::update(){
    view::update();

    if (calibrate) {
        box.setResolution(1);
        box.set(sphereRadius, sphereRadius, sphereRadius);
        for (int i = 0; i < 5; i++)
            box.setSideColor(i, ofColor(0, 0, 0, 10));
        box.setSideColor(4, ofColor(255, 0, 0, 150));
        box.setSideColor(5, ofColor(0, 255, 0, 150));
    }
}

//--------------------------------------------------------------
void viewHalfSphere::draw(){
    ofClear(0);
    ofBackground(0);
    ofSetColor(255, 255, 255);

    sCam.begin();
        ofEnableDepthTest();
        ofPushMatrix();
            ofScale(-sphereRadius, -sphereRadius, -sphereRadius);
            ofRotateX(90);
            sphereDome.drawBackFaces(true, true);
            if (calibrate) {
                sphereDome.drawBackFaces(true, false);
                sphereDome.drawWireFrame(true);
                sphereDome.drawFaceNormals(0.05);
            }
        ofPopMatrix();

        if (calibrate) {
            box.drawWireframe();
            box.drawVertices();
            box.drawFaces();
        }
        ofDisableDepthTest();
    sCam.end();
}

//--------------------------------------------------------------
void viewHalfSphere::keyPressed(int key){
}

//--------------------------------------------------------------
void viewHalfSphere::setupGui() {
    parameters.add(calibrate.set("calibrate", true));
    parameters.add(sphereStep.set("sphere step", 0.1, 0, 1));
    sphereStep.addListener(this, &viewHalfSphere::reload);
    parameters.add(sphereRadius.set("sphere Radius", 1.0*videoWidth, 0, 3.0*videoHeight));
    parameters.add(sphereDistance.set("sphere distance", 1.0*videoWidth, 0, 2.0*6.0*videoHeight));
}
