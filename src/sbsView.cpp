#include "sbsView.h"
/*
//--------------------------------------------------------------
sbsView::sbsView(sphereView* sphereL, sphereView* sphereR)
    : sphereL(*sphereL), sphereR(*sphereR) {
    
    output.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
    
    setupGui();

    cam.setDistance(sphereDistance);
    cam.setNearClip(1.0);
    cam.setFarClip(2.0 * sphereRadius);
    cam.lookAt(ofVec3f(0.0, 0.0, sphereRadius));
}

//--------------------------------------------------------------
void sbsView::update(){
	sphereL.update();
	sphereR.update();

	output.begin();
		ofClear(0);
		ofBackground(0, 255, 0);
		ofSetColor(255, 255, 255);

		sphereL.draw();
		sphereL.output.draw(0, 0, output.getWidth() / 2.0, output.getHeight());
		sphereR.output.draw(output.getWidth() / 2.0, 0, output.getWidth() / 2.0, output.getHeight());
	output.end();
}

//--------------------------------------------------------------
void sbsView::draw(){
}

//--------------------------------------------------------------
bool sbsView::isFrameNew() {
	return false;
}

//--------------------------------------------------------------
void sbsView::setupGui() {
    // params
    parameters.add(displayVideoSource.set("displayVideoSource", false));
    parameters.add(sphereRadius.set("sphereRadius", 2.0*output.getWidth(), 0, 4.0*output.getHeight()));
    parameters.add(sphereDistance.set("sphereDistance", 2.0*output.getWidth(), 0, 2.0*4.0*output.getHeight()));
    parameters.add(sphereOffsetX.set("sphereOffsetX", 0, -100.0, 100.0));
}
*/
