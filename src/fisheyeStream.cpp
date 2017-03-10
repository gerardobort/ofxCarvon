#include "fisheyeStream.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
fisheyeStream::fisheyeStream(int cameraWidth, int cameraHeight, char* url, char* name)
    : cameraWidth(cameraWidth), cameraHeight(cameraHeight), url(url), name(name) {
    
    input.allocate(cameraWidth, cameraHeight, GL_RGBA);
    texturePlane.allocate(cameraWidth, cameraHeight, GL_RGBA);
    output.allocate(cameraWidth, cameraHeight, GL_RGBA);
    
    shader.load("shaders/rectify");
    image.load("cube_orig.jpg");

    setupGui();

	grabber = std::make_shared<ofx::Video::IPVideoGrabber>();
	grabber->setCameraName(name);
	grabber->setURI(url);
	grabber->connect(); // connect immediately
}

//--------------------------------------------------------------
void fisheyeStream::update(){
	grabber->update();

	int w = cameraWidth;
	int h = cameraHeight;
	
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

	input.begin();
		ofClear(0);
		ofBackground(0, 0, 0);
		ofSetColor(255, 255, 255);

		ofImage i1, i2;
		i1.setFromPixels(grabber->getPixels());
		if (useCanny) { 
			convertColor(i1, i2, CV_RGB2GRAY);
			Canny(i2, i1, cannyThreshold1, cannyThreshold2, 3);
		}
		if (rotate) {
			i1.rotate90(1);
		}

		int _x, _y, _w, _h;
		if (mirrorX) {
			_y = h - inputOffsetY;
			_h = -h + 2*inputOffsetY;
		} else {
			_y = inputOffsetY;
			_h = h - 2*inputOffsetY;
		}
		if (mirrorY) {
			_x = w - inputOffsetX;
			_w = -w + 2*inputOffsetX;
		} else {
			_x = inputOffsetX;
			_w = w - 2*inputOffsetX;
		}
		i1.draw(_x, _y, _w, _h);
	input.end();

	texturePlane.begin();
		ofClear(0);
		ofSetColor(255, 255, 255);
		input.draw(-rectifyOverflow, rectifyOverflow, cameraWidth +2*rectifyOverflow, cameraHeight -2*rectifyOverflow);
		if (calibrate) {
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			image.draw(0, 0, cameraWidth, cameraHeight);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		}
	texturePlane.end();

	shader.begin();
		shader.setUniform1i("u_rectify", rectify);
		shader.setUniform1f("u_fovFactor", rectifyFovFactor);
		shader.setUniform1i("u_width", rectifyWidth);
		shader.setUniform1i("u_height", rectifyHeight);
	shader.end();

	output.begin();
		ofClear(0);
		ofBackground(0, 0, 0);
		ofSetColor(255, 255, 255);
		ofPushMatrix();
			ofTranslate((1.0-visibleRadius)*w/2.0, (1.0-visibleRadius)*h/2.0, 0);
			ofScale(visibleRadius, visibleRadius, 1.0);
			shader.begin();
				shader.setUniformTexture("u_sampler2dVideo", texturePlane.getTextureReference(0), 0);
				mesh.draw();
			shader.end();
		ofPopMatrix();
	output.end();
}

//--------------------------------------------------------------
void fisheyeStream::draw(){
}

//--------------------------------------------------------------
bool fisheyeStream::isFrameNew() {
	return false;
}

//--------------------------------------------------------------
void fisheyeStream::setupGui() {
    parameters.setName(name);

    // params
    parameters.add(visibleRadius.set("visibleRadius", 1, 1, 3));
    parameters.add(rectify.set("rectify", true));
    parameters.add(calibrate.set("calibrate", true));
    parameters.add(rectifyFovFactor.set("rectifyFovFactor", 1.0, 0, 4));
    parameters.add(rectifyOverflow.set("rectifyOverflow", 0, 0, 400));
    parameters.add(rectifyWidth.set("rectifyWidth", cameraWidth, 0, 4*cameraWidth));
    parameters.add(rectifyHeight.set("rectifyHeight", cameraHeight, 0, 4*cameraHeight));
    parameters.add(inputOffsetX.set("inputOffsetX", 0, -cameraWidth, cameraWidth));
    parameters.add(inputOffsetY.set("inputOffsetY", 0, -cameraHeight, cameraHeight));
    parameters.add(rotate.set("rotate", 0, 0, 1));
    parameters.add(mirrorX.set("mirrorX", 0, 0, 1));
    parameters.add(mirrorY.set("mirrorY", 0, 0, 1));
    parameters.add(outputOpacity.set("outputOpacity", 100, 0, 255));

    parameters.add(useCanny.set("useCanny", true));
    parameters.add(cannyThreshold1.set("cannyThreshold1", 400, 0, 5000));
    parameters.add(cannyThreshold2.set("cannyThreshold2", 100, 0, 5000));
}
