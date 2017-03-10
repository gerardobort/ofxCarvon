#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "source.h"

class sourceFileVideo: public source {
    public:
        sourceFileVideo(int videoWidth, int videoHeight, char* name, char* path);

        void update();
        void draw();
        void draw(int x, int y);
        void draw(int x, int y, int w, int h);
		bool isFrameNew();

    private:
        ofVideoPlayer video;
        char* path;

        void setupGui();

        ofParameter<bool> paused;
	    void setPaused(bool& _value) { video.setPaused(_value); }

        ofParameter<float> position;
	    void setPosition(float& _value) { video.setPosition(_value); }

        ofParameter<float> speed;
	    void setSpeed(float& _value) { video.setSpeed(_value); }

        ofParameter<float> volume;
	    void setVolume(float& _value) { video.setVolume(_value); }
};
