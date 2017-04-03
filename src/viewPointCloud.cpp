#include "viewPointCloud.h"

//--------------------------------------------------------------
viewPointCloud::viewPointCloud(int videoWidth, int videoHeight, char* name)
    : view(videoWidth, videoHeight, name) {
    setupGui();

}

//--------------------------------------------------------------
void viewPointCloud::update(){
    view::update();

    int w = videoWidth;
    int h = videoHeight;

	ofPixels depthPixels;
	depthFbo->readToPixels(depthPixels);

	ofPixels colorPixels = colorImage->getPixels();

	if (drawWireframe) {
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	} else {
		mesh.setMode(OF_PRIMITIVE_POINTS);
	}

	mesh.clear();
	int index;
    float depth;
    float cx = w/2, cy = h/2, dmax = sqrt( cx*cx + cy*cy ), radialZCoeficient;
    for (int y = 0; y+step < h; y += step) {
        for (int x = 0; x+step < w; x += step) {
			index = y*w + x;
			depth = depthPixels[4*index];
            if (depth > 0) {

                radialZCoeficient = -(1- ( (x-cx)*(x-cx) + (y-cy)*(y-cy) )/dmax);

                if (nearThreshold > depth && depth > farThreshold) {
                    mesh.addColor(ofColor(colorPixels[4*index], colorPixels[4*index + 1], colorPixels[4*index + 2]));
                    mesh.addVertex(ofVec3f(-x*dispersion, y*dispersion, (depth + radialZCoeficient)*zScale));
                }
            }
        }
    }
}

//--------------------------------------------------------------
void viewPointCloud::draw(){
    ofClear(0);
    ofBackground(0);
    ofSetColor(255, 255, 255);

    glPointSize(1);
    ofPushMatrix();
        sCam.begin();
		ofRotate(180);
		ofTranslate(pos->x*dispersion, pos->y*dispersion, pos->z*zScale);
		glEnable(GL_DEPTH_TEST);
		if (drawWireframe) {
			mesh.drawWireframe();
		} else {
			mesh.drawVertices();
		}
		glDisable(GL_DEPTH_TEST);
        sCam.end();
    ofPopMatrix();
}

//--------------------------------------------------------------
void viewPointCloud::drawMesh(){
    glPointSize(pointSize);
    ofPushMatrix();
		ofRotate(180);
		ofTranslate(pos->x*dispersion, pos->y*dispersion, pos->z*zScale);
		glEnable(GL_DEPTH_TEST);
		if (drawWireframe) {
			mesh.drawWireframe();
		} else {
			mesh.drawVertices();
		}
		glDisable(GL_DEPTH_TEST);
    ofPopMatrix();
}

//--------------------------------------------------------------
void viewPointCloud::setupGui() {
    parameters.add(step.set("step", 1, 1, 20));
    parameters.add(zScale.set("zScale", 10, 0, 60));
    parameters.add(dispersion.set("dispersion", 1, 1, 20));
    parameters.add(drawWireframe.set("drawWireframe", false));
    parameters.add(nearThreshold.set("nearThreshold", 255, 0, 255));
    parameters.add(farThreshold.set("farThreshold", 0, 0, 255));
    parameters.add(pointSize.set("pointSize", 1, 1, 10));
    parameters.add(pos.set("pos", ofPoint(0,0,0), 4.0*ofPoint(-videoWidth, -videoHeight, -255), 4.0*ofPoint(videoWidth, videoHeight, 255)));
}
