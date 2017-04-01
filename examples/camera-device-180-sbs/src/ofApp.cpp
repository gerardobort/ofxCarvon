#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();

    videoSourceLeft = new sourceDevice(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "source", 0);
    //videoSourceLeft = new sourceNetworkMjpeg(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "source", "http://192.168.0.12:5000/video_feed0");
    transformerStandardLeft = new transformerStandard(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "adjust video");

    videoSourceRight = new sourceDevice(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "source", 2);
    //videoSourceRight = new sourceNetworkMjpeg(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "source", "http://192.168.0.12:5000/video_feed1");
    transformerStandardRight = new transformerStandard(RECORD_VIDEO_WIDTH, RECORD_VIDEO_HEIGHT, "adjust video");


    _debuggerFisheye = new debuggerFisheye(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, "debugger fisheye");
    _transformerSphereTexture = new transformerSphereTexture(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, "sphere texture");
    _viewHalfSphere = new viewHalfSphere(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "viewport", RECORD_VIDEO_HEIGHT);

    indexSample = 0;
    stereobm = new ofxCv::Stereo(16*10, 7);
    leftCamera = new ofxCv::Camera(NUM_CALIBRATION_SAMPLES);
    rightCamera = new ofxCv::Camera(NUM_CALIBRATION_SAMPLES);

	/*
    leftCamera->distortionCoefficients = (cv::Mat_<double>(5, 1) << 2.714082172979768e-21, 1.281436674111915e-15, 5.13633191351897e-24, 9.435497980549972e-24, -3.029111060774951e-21);
	leftCamera->cameraMatrix = (cv::Mat_<double>(3, 3) << 1, 0, 5.328585253723798e-30, 0, 1, 2.914335235674356e-30, 0, 0, 1);
	leftCamera->cameraMatrixRefined = (cv::Mat_<double>(3, 3) << 0.9695592522621155, 0, 4.743571390577924e-18, 0, 0.9689696431159973, 8.419465037473303e-18, 0, 0, 1);

    rightCamera->distortionCoefficients = (cv::Mat_<double>(5, 1) << -1.53305244739114e-20, -5.45402493264424e-15, -3.903892344180897e-23, -5.768942330575815e-23, 1.091696845466223e-20);
	rightCamera->cameraMatrix = (cv::Mat_<double>(3, 3) << 1, 0, -4.3499539671517e-29, 0, 1, -2.960090426442004e-29, 0, 0, 1);
	rightCamera->cameraMatrixRefined = (cv::Mat_<double>(3, 3) << 1.026843905448914, 0, -4.466723570661491e-29, 0, 0.9986660480499268, -2.95614190956143e-29, 0, 0, 1);
	*/
    
    string settingsFile = "settings.xml";
    gui.setup("settings", settingsFile);
    gui.setDefaultBackgroundColor(ofColor(40, 100, 40));
    gui.setDefaultFillColor(ofColor(0, 160, 0));
    gui.setDefaultBorderColor(ofColor(0, 0, 0));
    gui.add(calibrate.set("calibrate", true));

    paramsLeft.setName("left");
    paramsLeft.add(*videoSourceLeft->getParametersReference());
    paramsLeft.add(*transformerStandardLeft->getParametersReference());

    paramsRight.setName("right");
    paramsRight.add(*videoSourceRight->getParametersReference());
    paramsRight.add(*transformerStandardRight->getParametersReference());

    paramsView.setName("view");
    paramsView.add(*_debuggerFisheye->getParametersReference());
    paramsView.add(*_transformerSphereTexture->getParametersReference());
    paramsView.add(*_viewHalfSphere->getParametersReference());

    paramsStereo.setName("stereo");
    paramsStereo.add(swapCameras.set("swapCameras", false));
    paramsStereo.add(indexSample.set("indexSample", 0, 0, NUM_CALIBRATION_SAMPLES -1));
    paramsStereo.add(calibrateStereo.set("calibrateStereo", true));
    paramsStereo.add(showStereoRectification.set("showStereoRectification", true));
    paramsStereo.add(viewStereo.set("viewStereo", false));
    paramsStereo.add(nDisparities.set("nDisparities", 1, 1, 40));
    nDisparities.addListener(this, &ofApp::reloadStereoN);
    paramsStereo.add(windowSize.set("windowSize", 1, 1, 100));
    windowSize.addListener(this, &ofApp::reloadStereoS);

    paramsStereo.add(P1.set("P1", 0, 0, 100));
    P1.addListener(this, &ofApp::reloadStereoP1);

    paramsStereo.add(P2.set("P2", 0, 0, 100));
    P2.addListener(this, &ofApp::reloadStereoP2);

    paramsStereo.add(disp12MaxDiff.set("disp12MaxDiff", 0, 0, 100));
    disp12MaxDiff.addListener(this, &ofApp::reloadDisp12MaxDiff);

    paramsStereo.add(preFilterCap.set("preFilterCap", 0, 0, 100));
    preFilterCap.addListener(this, &ofApp::reloadStereoPrefilterCap);

    paramsStereo.add(uniquenessRatio.set("uniquenessRatio", 0, 0, 100));
    uniquenessRatio.addListener(this, &ofApp::reloadStereoUniquenessRatio);

    paramsStereo.add(speckleWindowSize.set("speckleWindowSize", 0, 0, 100));
    speckleWindowSize.addListener(this, &ofApp::reloadSpeckleWindowSize);

    paramsStereo.add(speckleRange.set("speckleRange", 0, 0, 100));
    speckleRange.addListener(this, &ofApp::reloadSpeckleRange);

    gui.add(paramsLeft);
    gui.add(paramsRight);
    gui.add(paramsView);
    gui.add(paramsStereo);

    gui.minimizeAll();

    gui.loadFromFile(settingsFile);
    gui.setPosition(ofGetWindowWidth() - gui.getWidth() - 10, 10);
    shouldShowSettings = true;
    indexSample = 0;

    // sourceCanvas will contain the trimmed spherical video frames, so the image size should be squared.
    sourceCanvas.allocate(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, GL_RGBA);
    // sphericalCanvas will contain the texture that will cover the sphere-dome, in this case, due that the sphere-dome is a half-sphere, the texture must be squared.
    sphericalCanvas.allocate(RECORD_VIDEO_HEIGHT, RECORD_VIDEO_HEIGHT, GL_RGBA);
    // viewportCanvas helps to positionate the resulting output into the screen.
    viewportCanvas.allocate(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, GL_RGBA);

    leftImage.load("left.png");
    rightImage.load("right.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    videoSourceLeft->update();
    videoSourceRight->update();
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

    int w = 300, h = 200;

    // sourceCanvas purpose is to have the raw video input with some basic transformations applied on it
    // in this example, transformerStandard let's you adjust displacement, scale, rotation or even mirroring issues caused by the
    // installation nature or even manufacturer differences on the input devices
    sourceCanvas.begin();
        ofClear(0);
        ofBackground(0);
        transformerStandardLeft->begin();
            videoSourceLeft->draw();
        transformerStandardLeft->end();
        if (calibrate) {
            _debuggerFisheye->draw(); // just draws a ruler on top of the transformed input, this is helpful to calibrate the installation
        }
    sourceCanvas.end();

    sphericalCanvas.begin();
        sourceCanvas.getTexture().bind(); // bind rolled image
        _transformerSphereTexture->draw(); // generate unrolled squared image, uses last binding
    sphericalCanvas.end();

    viewportCanvas.begin();
        sphericalCanvas.getTextureReference().bind();
        _viewHalfSphere->draw(); // uses last binding
    viewportCanvas.end();

    ofPixels pixels;
    viewportCanvas.readToPixels(pixels);
    if (swapCameras) {
        rightImage.setFromPixels(pixels);
    } else {
        leftImage.setFromPixels(pixels);
    }

    if (calibrate) {
        videoSourceLeft->draw(0, 0, w, h);
        sourceCanvas.draw(0, h, w, w);
        sphericalCanvas.draw(0, h+w, w, w);
        viewportCanvas.draw(2*w, 0, 0.5*VIEWPORT_WIDTH, 0.5*VIEWPORT_HEIGHT);
    } else {
        viewportCanvas.draw(0, 0, ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    }

    // sourceCanvas purpose is to have the raw video input with some basic transformations applied on it
    // in this example, transformerStandard let's you adjust displacement, scale, rotation or even mirroring issues caused by the
    // installation nature or even manufacturer differences on the input devices
    sourceCanvas.begin();
        ofClear(0);
        ofBackground(0);
        transformerStandardRight->begin();
            videoSourceRight->draw();
        transformerStandardRight->end();
        if (calibrate) {
            _debuggerFisheye->draw(); // just draws a ruler on top of the transformed input, this is helpful to calibrate the installation
        }
    sourceCanvas.end();

    sphericalCanvas.begin();
        sourceCanvas.getTexture().bind(); // bind rolled image
        _transformerSphereTexture->draw(); // generate unrolled squared image, uses last binding
    sphericalCanvas.end();

    viewportCanvas.begin();
        sphericalCanvas.getTextureReference().bind();
        _viewHalfSphere->draw(); // uses last binding
    viewportCanvas.end();

    viewportCanvas.readToPixels(pixels);
    if (swapCameras) {
        leftImage.setFromPixels(pixels);
    } else {
        rightImage.setFromPixels(pixels);
    }

    if (calibrate) {
        videoSourceRight->draw(w, 0, w, h);
        sourceCanvas.draw(w, h, w, w);
        sphericalCanvas.draw(w, h+w, w, w);
        viewportCanvas.draw(2*w, 0.5*VIEWPORT_HEIGHT, 0.5*VIEWPORT_WIDTH, 0.5*VIEWPORT_HEIGHT);
    } else {
        viewportCanvas.draw(ofGetWindowWidth()/2, 0, ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    }


    if (!calibrate) {
        if (calibrateStereo && ofGetElapsedTimeMillis() > 1000) {
            if (
                leftCamera->findChessboardCorners(leftImage, indexSample, leftPoints)
                && rightCamera->findChessboardCorners(rightImage, indexSample, rightPoints)
            ) {
                indexSample++;
                ofResetElapsedTimeCounter();
            }
        }

        if (indexSample == NUM_CALIBRATION_SAMPLES && calibrateStereo) {
            leftCamera->calibrate();
            rightCamera->calibrate();
            stereobm->calibrate(*leftCamera, *rightCamera);
            calibrateStereo = false;
        }

        if (stereobm->isReady && showStereoRectification) {
            // two alternatives, both doesn't work as expected
            stereobm->rectify(leftImage, rightImage);
            //stereobm->rectifyLeft(leftImage);
        } else if (indexSample == NUM_CALIBRATION_SAMPLES) {
            leftCamera->rectify(leftImage, leftImage);
            rightCamera->rectify(rightImage, rightImage);
        }
    }

    if (!calibrate) {
        ofEnableAlphaBlending();
        if (viewStereo) {
            stereobm->compute(leftImage, rightImage);
            viewportCanvas.begin();
                stereobm->draw();
            viewportCanvas.end();
            viewportCanvas.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        } else {
            viewportCanvas.begin();
                ofClear(0);
                ofBackground(255);

                ofSetColor(0, 255, 0, 255);
                leftImage.draw(0, 0);

                if (leftCamera->isReady) {
                    ofSetColor(255, 0, 0, 255);
                    if (stereobm->isReady) {
                        ofSetColor(255, 255, 255, 255);
                        leftImage.draw(0, 0);
                    } else
                        leftImage.draw(0, 0);
                }

                ofSetColor(0, 255, 0, 255);
                ofSetLineWidth(10);
                leftPoints.draw();
            viewportCanvas.end();
            viewportCanvas.draw(0, ofGetWindowHeight()/2, ofGetWindowWidth()/2, ofGetWindowHeight()/2);

            viewportCanvas.begin();
                ofEnableAlphaBlending();
                ofClear(0);
                ofBackground(255);

                ofSetColor(0, 255, 0, 255);
                rightImage.draw(0, 0);

                if (rightCamera->isReady) {
                    ofSetColor(255, 0, 0, 255);
                    if (stereobm->isReady) {
                        ofSetColor(255, 255, 255, 255);
                        rightImage.draw(0, 0);
                    } else
                        rightImage.draw(0, 0);
                }

                ofSetColor(0, 255, 0, 255);
                ofSetLineWidth(10);
                rightPoints.draw();

            viewportCanvas.end();
            viewportCanvas.draw(ofGetWindowWidth()/2, ofGetWindowHeight()/2, ofGetWindowWidth()/2, ofGetWindowHeight()/2);
        }
        ofDisableAlphaBlending();
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
