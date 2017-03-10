#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "source.h"

class sourceDevice: public source {
    public:
        sourceDevice(int videoWidth, int videoHeight, char* name, int deviceId = 0);

        void update();
        void draw();
        void draw(int x, int y);
        void draw(int x, int y, int w, int h);
		bool isFrameNew();

    private:
        int deviceId;
        ofVideoGrabber video;

        void setupGui();
        ofParameter<int> cameraDeviceId;
	    void setDeviceId(int& _value) {
            video.setDeviceID(_value);
            cout << "************** camera device IN USE: " << deviceId << endl;
            video.initGrabber(videoWidth, videoHeight);
        }
};
