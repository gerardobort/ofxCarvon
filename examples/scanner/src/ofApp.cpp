#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();

    videoSource = new sourceDevice(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "source", 0);
    _transformerStandard = new transformerStandard(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "adjust video");

    _debuggerFisheye = new debuggerFisheye(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, "debugger fisheye");
    _transformerSphereTexture = new transformerSphereTexture(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, "sphere texture");
    _transformerChromaKey = new transformerChromaKey(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, "chroma key");
    _viewHalfSphere = new viewHalfSphere(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "viewport", RECORD_VIDEO_HEIGHT);

    string settingsFile = "settings.xml";
    gui.setup("settings", settingsFile);
    gui.setDefaultBackgroundColor(ofColor(40, 100, 40));
    gui.setDefaultFillColor(ofColor(0, 160, 0));
    gui.setDefaultBorderColor(ofColor(0, 0, 0));
    gui.add(testRaw.set("test raw", false));
    gui.add(calibrate.set("calibrate", true));

    paramsSource.setName("video source");
    paramsSource.add(*videoSource->getParametersReference());
    paramsSource.add(*_transformerStandard->getParametersReference());
    paramsSource.add(*_transformerChromaKey->getParametersReference());

    paramsView.setName("view");
    paramsView.add(*_debuggerFisheye->getParametersReference());
    paramsView.add(*_transformerSphereTexture->getParametersReference());
    paramsView.add(*_viewHalfSphere->getParametersReference());

    gui.add(paramsSource);
    gui.add(paramsView);

    gui.minimizeAll();

    gui.loadFromFile(settingsFile);
    gui.setPosition(ofGetWindowWidth() - gui.getWidth() - 10, 10);
    shouldShowSettings = true;

    // sourceCanvas will contain the trimmed spherical video frames, so the image size should be squared.
    sourceCanvas.allocate(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, GL_RGBA);
    chromaCanvas.allocate(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, GL_RGBA);
    // sphericalCanvas will contain the texture that will cover the sphere-dome, in this case, due that the sphere-dome is a half-sphere, the texture must be squared.
    sphericalCanvas.allocate(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, GL_RGBA);
    // viewportCanvas helps to positionate the resulting output into the screen.
    viewportCanvas.allocate(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, GL_RGBA);

	ofEnableSmoothing();
}

//--------------------------------------------------------------
void ofApp::update(){
    videoSource->update();
    if (calibrate) {
        _debuggerFisheye->update();
    }
    _viewHalfSphere->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    ofBackground(0);
    ofSetColor(255, 255, 255);

    int w = ofGetWindowWidth()/2, h = ofGetWindowHeight()/3;

    // sourceCanvas purpose is to have the raw video input with some basic transformations applied on it
    // in this example, _transformerStandard let's you adjust displacement, scale, rotation or even mirroring issues caused by the
    // installation nature or even manufacturer differences on the input devices
    sourceCanvas.begin();
        ofClear(0);
        ofBackground(0);
        _transformerStandard->begin();
            videoSource->draw();
        _transformerStandard->end();
        if (calibrate) {
            _debuggerFisheye->draw(); // just draws a ruler on top of the transformed input, this is helpful to calibrate the installation
        }
    sourceCanvas.end();

	chromaCanvas.begin();
		sourceCanvas.getTexture().bind(); // bind source
		_transformerChromaKey->draw(); // generate unrolled squared image, uses last binding
	chromaCanvas.end();

	sphericalCanvas.begin();
		chromaCanvas.getTexture().bind(); // bind rolled image
		_transformerSphereTexture->draw(); // generate unrolled squared image, uses last binding
	sphericalCanvas.end();

	viewportCanvas.begin();
		sphericalCanvas.getTextureReference().bind();
		_viewHalfSphere->draw(); // uses last binding
	viewportCanvas.end();

    ofPixels pixels;
    viewportCanvas.readToPixels(pixels);

    if (calibrate) {
		if (testRaw) {
			videoSource->draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		} else {
			videoSource->draw(0, 0, w, h);
			sourceCanvas.draw(0, h, w, h);
			sphericalCanvas.draw(0, 2*h, w, h);
			viewportCanvas.draw(w, 0, w, h);
		}
    } else {
        viewportCanvas.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    }

    if (shouldShowSettings) {
        gui.draw();
    }

    string fpsInfo = "fps: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapStringHighlight(fpsInfo, 10, ofGetWindowHeight()-20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    _viewHalfSphere->keyPressed(key);
    switch (key) {
        case '.':
            shouldShowSettings = !shouldShowSettings;
            break;
        case ' ':
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


//--------------------------------------------------------------
void ofApp::exit(){
}
