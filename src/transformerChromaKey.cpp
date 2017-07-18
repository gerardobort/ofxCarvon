#include "transformerChromaKey.h"

//--------------------------------------------------------------
transformerChromaKey::transformerChromaKey(int videoWidth, int videoHeight, char* name)
    : transformer(videoWidth, videoHeight, name) {
    setupGui();

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
void transformerChromaKey::draw(){
    ofClear(0);
    ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
    ofPushMatrix();
        //ofTranslate(ofPoint(-videoWidth/2.0, 0));
        //ofScale(ofPoint(2, 1));
        shader.begin();
            shader.setUniform3f("u_colorKey", colorKey);
            shader.setUniform3f("u_colorReplacement", colorReplacement);
            shader.setUniform1f("u_threshold", threshold);
            shader.setUniform2i("u_videoSize", videoWidth, videoHeight);
            mesh.draw();
        shader.end();
    ofPopMatrix();
}

//--------------------------------------------------------------
void transformerChromaKey::setupGui() {
    parameters.add(colorKey.set("colorKey", ofVec3f(0, 255, 0), ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));
    parameters.add(colorReplacement.set("colorReplacement", ofVec3f(0, 0, 255), ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));
    parameters.add(threshold.set("threshold", 0.5, 0, 1));
}
