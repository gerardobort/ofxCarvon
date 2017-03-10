#include "viewSphere.h"

//--------------------------------------------------------------
viewSphere::viewSphere(int videoWidth, int videoHeight, char* name)
    : view(videoWidth, videoHeight, name) {
    setupGui();

    cam.setDistance(sphereDistance);
    cam.setNearClip(1.0);
    cam.setFarClip(2.0 * sphereRadius);
    cam.lookAt(ofVec3f(0.0, 0.0, sphereRadius));

	sphere.setMode(OF_PRIMITIVE_TRIANGLES);
}

//--------------------------------------------------------------
void viewSphere::update(){
    view::update();

    // TODO move these to ofxGui events
    sphere.setRadius(sphereRadius);
    sphere.setResolution(sphereResolution);
    cam.setDistance(sphereDistance);

    if (calibrate) {
        box.setResolution(1);
        box.set(sphereRadius, sphereRadius, sphereRadius);
        for (int i = 0; i < 5; i++)
            box.setSideColor(i, ofColor(0, 0, 0, 10));
        box.setSideColor(4, ofColor(0, 255, 0, 150));
        box.setSideColor(5, ofColor(255, 0, 0, 150));
    }

}

//--------------------------------------------------------------
void viewSphere::draw(){
    ofClear(0);
    ofBackground(0, 255, 0);
    ofSetColor(255, 255, 255);

    ofPushMatrix();
        cam.begin();
        ofScale(-1.0, -1.0, 1.0);
        sphere.drawFaces();
        if (calibrate) {
            glLineWidth(6.0);
            box.drawWireframe();
            glPointSize(10.0);
            box.drawVertices();
            box.drawFaces();
        }
        cam.end();
    ofPopMatrix();
}

//--------------------------------------------------------------
void viewSphere::setupGui() {
    parameters.add(calibrate.set("calibrate", true));
    parameters.add(sphereResolution.set("sphere resolution", 32, 0, 64));
    parameters.add(sphereRadius.set("sphere radius", 2.0*videoWidth, 0, 4.0*videoHeight));
    parameters.add(sphereDistance.set("sphere distance", 2.0*videoWidth, 0, 2.0*4.0*videoHeight));}
