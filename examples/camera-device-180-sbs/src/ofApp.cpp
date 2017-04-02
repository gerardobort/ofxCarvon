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
    _viewPointCloud = new viewPointCloud(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "pointcloud");

    indexSample = 0;
    stereobm = new ofxCv::Stereo(16*10, 7);
    leftCamera = new ofxCv::Camera(NUM_CALIBRATION_SAMPLES);
    rightCamera = new ofxCv::Camera(NUM_CALIBRATION_SAMPLES);
    
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
    paramsView.add(*_viewPointCloud->getParametersReference());

    paramsStereo.setName("stereo");
    paramsStereo.add(useFisheyeSource.set("useFisheyeSource", false));
    paramsStereo.add(swapCameras.set("swapCameras", false));
    paramsStereo.add(indexSample.set("indexSample", 0, 0, NUM_CALIBRATION_SAMPLES -1));
    paramsStereo.add(calibrateStereo.set("calibrateStereo", true));
    paramsStereo.add(showStereoRectification.set("showStereoRectification", true));
    paramsStereo.add(viewStereo.set("viewStereo", false));
    paramsStereo.add(viewMesh.set("viewMesh", false));
    paramsStereo.add(nDisparities.set("nDisparities", 1, 1, 40));
    nDisparities.addListener(this, &ofApp::reloadStereoN);
    paramsStereo.add(windowSize.set("windowSize", 1, 1, 100));
    windowSize.addListener(this, &ofApp::reloadStereoS);

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
    depthMap.allocate(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, GL_RGBA);

    leftImage.load("left.png");
    rightImage.load("right.png");

	ofEnableSmoothing();
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

    if (useFisheyeSource) {
        viewportCanvas.begin();
            sourceCanvas.draw(0, 0, viewportCanvas.getWidth(), viewportCanvas.getHeight());
        viewportCanvas.end();
    } else {
        sphericalCanvas.begin();
            sourceCanvas.getTexture().bind(); // bind rolled image
            _transformerSphereTexture->draw(); // generate unrolled squared image, uses last binding
        sphericalCanvas.end();

        viewportCanvas.begin();
            sphericalCanvas.getTextureReference().bind();
            _viewHalfSphere->draw(); // uses last binding
        viewportCanvas.end();
    }

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

    if (useFisheyeSource) {
        viewportCanvas.begin();
            sourceCanvas.draw(0, 0, viewportCanvas.getWidth(), viewportCanvas.getHeight());
        viewportCanvas.end();
    } else {
        sphericalCanvas.begin();
            sourceCanvas.getTexture().bind(); // bind rolled image
            _transformerSphereTexture->draw(); // generate unrolled squared image, uses last binding
        sphericalCanvas.end();

        viewportCanvas.begin();
            sphericalCanvas.getTextureReference().bind();
            _viewHalfSphere->draw(); // uses last binding
        viewportCanvas.end();
    }

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
			colorMap.clone(leftImage);
            stereobm->compute(leftImage, rightImage); // converts the images into grayscale
			if (viewMesh) {
				depthMap.begin();
					stereobm->draw();
				depthMap.end();
				_viewPointCloud->setInput(&depthMap, &colorMap);
				_viewPointCloud->update();
				viewportCanvas.begin();
					_viewPointCloud->draw();
				viewportCanvas.end();
			} else {
				viewportCanvas.begin();
					stereobm->draw();
				viewportCanvas.end();
			}
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
