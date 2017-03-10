#include "sourceFileVideo.h"

//--------------------------------------------------------------
sourceFileVideo::sourceFileVideo(int videoWidth, int videoHeight, char* name, char* path)
    : source(videoWidth, videoHeight, name), path(path) {

    video.load(path);
    video.play();
    
    setupGui();
}

//--------------------------------------------------------------
void sourceFileVideo::update(){
    source::update();

    video.update();
}

//--------------------------------------------------------------
void sourceFileVideo::draw(){
    video.getTexture().draw(0, 0, videoWidth, videoHeight);
}

//--------------------------------------------------------------
void sourceFileVideo::draw(int x, int y){
    video.getTexture().draw(x, y);
}

//--------------------------------------------------------------
void sourceFileVideo::draw(int x, int y, int w, int h){
    video.getTexture().draw(x, y, w, h);
}

//--------------------------------------------------------------
bool sourceFileVideo::isFrameNew() {
	return video.isFrameNew();
}

//--------------------------------------------------------------
void sourceFileVideo::setupGui() {
	parameters.add(paused.set("paused", false));
	paused.addListener(this, &sourceFileVideo::setPaused);

	parameters.add(position.set("position", 0, 0, 1));
	position.addListener(this, &sourceFileVideo::setPosition);

	parameters.add(speed.set("speed", 1, 0, 1));
	speed.addListener(this, &sourceFileVideo::setSpeed);

	parameters.add(volume.set("volume", 1, 0, 1));
	volume.addListener(this, &sourceFileVideo::setVolume);
}
