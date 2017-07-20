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

	mask.allocate(videoWidth, videoHeight, GL_RGBA);
	float i = 0;
	while (i < TWO_PI) { // make a heart
		float r = (2-2*sin(i) + sin(i)*sqrt(abs(cos(i))) / (sin(i)+1.4)) * -80;
		float x = videoWidth/2 + cos(i) * r;
		float y = videoHeight/2 + sin(i) * r;
		line.addVertex(ofVec2f(x, y));
		i += 0.005*HALF_PI*0.5;
	}
	line.close(); // close the shape
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

	// draw polyline shape
	mask.begin();
		ofSetColor(ofColor::white);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofFill();

		ofBeginShape();
			ofVertex(0, 0);
			ofVertex(videoWidth, 0);
			ofVertex(videoWidth, videoHeight);
			ofVertex(0, videoHeight);
		ofEndShape();

		ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
		ofBeginShape();
			vector<ofPoint>& vertices = line.getVertices();
			for(int j = 0; j < vertices.size(); j++) {
				ofVertex(vertices[j]);
			}
		ofEndShape();

		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	mask.end();

	ofSetColor(255*colorReplacement->x, 255*colorReplacement->y, 255*colorReplacement->z);
	mask.draw(0, 0, videoWidth, videoHeight);
}

//--------------------------------------------------------------
void transformerChromaKey::setupGui() {
    parameters.add(colorKey.set("colorKey", ofVec3f(0, 1, 0), ofVec3f(0, 0, 0), ofVec3f(1, 1, 1)));
    parameters.add(colorReplacement.set("colorReplacement", ofVec3f(0, 0, 1), ofVec3f(0, 0, 0), ofVec3f(1, 1, 1)));
    parameters.add(threshold.set("threshold", 0.5, 0, 1));
}
