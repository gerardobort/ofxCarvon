#include "transformerSphereTexture.h"
// FIXME ofApp.h may be named differently depending on the project
#include "ofApp.h"

//--------------------------------------------------------------
transformerSphereTexture::transformerSphereTexture(int videoWidth, int videoHeight, char* name)
    : transformer(videoWidth, videoHeight, name) {
    setupGui();

    ofDirectory dir = ofDirectory(ASSETS_PATH);
    string path = dir.getAbsolutePath(); 
    ofLogNotice("ASSETS_PATH=", path);
    ofLogNotice("Loading Asset:", path + "/cube_orig.jpg");
    image.load(path + "/cube_orig.jpg");

    setupShader();

	int w = 2*videoWidth;
	int h = videoHeight;
	
	mesh.clear();

	mesh.addVertex(ofVec3f(-w, -h, 0));
	mesh.addVertex(ofVec3f(-w,  h, 0));
	mesh.addVertex(ofVec3f( w,  h, 0));
	mesh.addVertex(ofVec3f( w, -h, 0));
	
	mesh.addColor(ofColor(0, 100, 0));
	mesh.addColor(ofColor(0, 100, 0));
	mesh.addColor(ofColor(0, 100, 0));
	mesh.addColor(ofColor(0, 100, 0));

	mesh.addTexCoord(ofVec2f(-w, -h));
	mesh.addTexCoord(ofVec2f(-w,  h));
	mesh.addTexCoord(ofVec2f( w,  h));
	mesh.addTexCoord(ofVec2f( w, -h));

	mesh.addIndex(0);
	mesh.addIndex(1);
	mesh.addIndex(2);
	mesh.addIndex(0);
	mesh.addIndex(2);
	mesh.addIndex(3);
}

//--------------------------------------------------------------
void transformerSphereTexture::draw(){
    ofClear(0);
    ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
    ofPushMatrix();
        ofTranslate(ofPoint(-videoWidth/2.0, 0));
        ofScale(ofPoint(2, 1));
        shader.begin();
            shader.setUniform1i("u_calibrate", calibrate ? 1 : 0);
            shader.setUniform1f("u_fovFactor", rectifyFovFactor);
            shader.setUniform1i("u_rectifyOverflow", rectifyOverflow);
            shader.setUniform2i("u_videoSize", videoWidth, videoHeight);
            shader.setUniform2i("u_rulerSize", image.getWidth(), image.getHeight());
            shader.setUniformTexture("u_sampler2dRuler", image.getTexture(), 1);
            mesh.draw();
        shader.end();
    ofPopMatrix();
}

//--------------------------------------------------------------
void transformerSphereTexture::setupGui() {
    parameters.add(calibrate.set("calibrate", true));
    parameters.add(rectifyFovFactor.set("rectifyFovFactor", 1.0, 0, 4));
    parameters.add(rectifyOverflow.set("rectifyOverflow", 0, 0, 400));
}
