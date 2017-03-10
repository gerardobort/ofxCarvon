#include "sourceDevice.h"

//--------------------------------------------------------------
sourceDevice::sourceDevice(int videoWidth, int videoHeight, char* name, int deviceId)
    : source(videoWidth, videoHeight, name), deviceId(deviceId) {

    video.setVerbose(true);
    video.listDevices();
    setDeviceId(deviceId);
    
    setupGui();
}

//--------------------------------------------------------------
void sourceDevice::update(){
    source::update();
    video.update();
}

//--------------------------------------------------------------
void sourceDevice::draw(){
    video.draw(0, 0, videoWidth, videoHeight);
}

//--------------------------------------------------------------
void sourceDevice::draw(int x, int y){
    video.draw(x, y, videoWidth, videoHeight);
}

//--------------------------------------------------------------
void sourceDevice::draw(int x, int y, int w, int h){
    video.draw(x, y, w, h);
}

//--------------------------------------------------------------
bool sourceDevice::isFrameNew() {
	return video.isFrameNew();
}

//--------------------------------------------------------------
void sourceDevice::setupGui() {
    ofVideoGrabber tvid;
    vector<ofVideoDevice> allCameras;
    allCameras = tvid.listDevices();
	parameters.add(cameraDeviceId.set("device", cameraDeviceId, 0, allCameras.size()-1));
	cameraDeviceId.addListener(this, &sourceDevice::setDeviceId);
}
