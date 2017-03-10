#include "sourceNetworkMjpeg.h"

//--------------------------------------------------------------
sourceNetworkMjpeg::sourceNetworkMjpeg(int videoWidth, int videoHeight, char* name, char* url)
    : source(videoWidth, videoHeight, name), url(url) {

	grabber = std::make_shared<ofx::Video::IPVideoGrabber>();
	grabber->setCameraName(name);
	setUrl(url);
    
    setupGui();
}

//--------------------------------------------------------------
void sourceNetworkMjpeg::update(){
    source::update();
    grabber->update();

}

//--------------------------------------------------------------
void sourceNetworkMjpeg::draw(){
    grabber->draw(0, 0, videoWidth, videoHeight);
}

//--------------------------------------------------------------
void sourceNetworkMjpeg::draw(int x, int y){
    grabber->draw(x, y, videoWidth, videoHeight);
}

//--------------------------------------------------------------
void sourceNetworkMjpeg::draw(int x, int y, int w, int h){
    grabber->draw(x, y, w, h);
}

//--------------------------------------------------------------
bool sourceNetworkMjpeg::isFrameNew() {
	return true;
}

//--------------------------------------------------------------
void sourceNetworkMjpeg::setupGui() {
	parameters.add(sourceUrl.set("url", url));
	//sourceUrl.addListener(this, &sourceNetworkMjpeg::setUrl);
}
