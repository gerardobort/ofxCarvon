#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();

    videoSource = new sourceDevice(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "video", 0);
    transformerStandard1 = new transformerStandard(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "adjust video");
    debuggerFisheye1 = new debuggerFisheye(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, "debugger fisheye");
    transformerSphereTexture1 = new transformerSphereTexture(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, "sphere texture");
    viewHalfSphere1 = new viewHalfSphere(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "viewport", RECORD_VIDEO_HEIGHT);
    
    string settingsFile = "settings.xml";
    gui.setup("settings", settingsFile);
    gui.setDefaultBackgroundColor(ofColor(40, 100, 40));
    gui.setDefaultFillColor(ofColor(0, 160, 0));
    gui.setDefaultBorderColor(ofColor(0, 0, 0));
    gui.add(calibrate.set("calibrate", true));
    gui.add(videoSource->getParameters());
    gui.add(transformerStandard1->getParameters());
    gui.add(debuggerFisheye1->getParameters());
    gui.add(transformerSphereTexture1->getParameters());
    gui.add(viewHalfSphere1->getParameters());
    gui.minimizeAll();

    gui.loadFromFile(settingsFile);
    gui.setPosition(ofGetWindowWidth() - gui.getWidth() - 10, 10);
    shouldShowSettings = true;

    // sourceCanvas will contain the trimmed spherical video frames, so the image size should be squared.
    sourceCanvas.allocate(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, GL_RGBA);
    // sphericalCanvas will contain the texture that will cover the sphere-dome, in this case, due that the sphere-dome is a half-sphere, the texture must be squared.
    sphericalCanvas.allocate(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, GL_RGBA);
    // viewportCanvas helps to positionate the resulting output into the screen.
    viewportCanvas.allocate(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::update(){
    videoSource->update();
    if (calibrate) {
        debuggerFisheye1->update();
    }
    viewHalfSphere1->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    ofBackground(0);
    ofSetColor(255, 255, 255);

    // sourceCanvas purpose is to have the raw video input with some basic transformations applied on it
    // in this example, transformerStandard let's you adjust displacement, scale, rotation or even mirroring issues caused by the
    // installation nature or even manufacturer differences on the input devices
    sourceCanvas.begin();
        ofClear(0);
        ofBackground(0);
        transformerStandard1->begin();
            videoSource->draw();
        transformerStandard1->end();
        if (calibrate) {
            debuggerFisheye1->draw(); // just draws a ruler on top of the transformed input, this is helpful to calibrate the installation
        }
    sourceCanvas.end();

    sphericalCanvas.begin();
        sourceCanvas.getTexture().bind(); // bind rolled image
        transformerSphereTexture1->draw(); // generate unrolled squared image, uses last binding
    sphericalCanvas.end();

    viewportCanvas.begin();
        sphericalCanvas.getTextureReference().bind();
        viewHalfSphere1->draw(); // uses last binding
    viewportCanvas.end();

    if (calibrate) {
        int w = 300, h = 200;
        videoSource->draw(0, 0, w, h);
        sourceCanvas.draw(0, h, w, h);
        sphericalCanvas.draw(0, 2*h, w, h);
        viewportCanvas.draw(w, 0, 0.5*VIEWPORT_WIDTH, 0.5*VIEWPORT_HEIGHT);
    } else {
        viewportCanvas.draw(0.5*(ofGetWindowWidth() - VIEWPORT_WIDTH), 0.5*(ofGetWindowHeight() - VIEWPORT_HEIGHT));
    }

    if (shouldShowSettings) {
        gui.draw();
    }

    string fpsInfo = "fps: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapStringHighlight(fpsInfo, 10, ofGetWindowHeight()-20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    viewHalfSphere1->keyPressed(key);
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
