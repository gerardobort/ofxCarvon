#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "source.h"
#include "IPVideoGrabber.h"

class sourceNetworkMjpeg: public source {
    public:
        sourceNetworkMjpeg(int videoWidth, int videoHeight, char* name, char* url = "");

        void update();
        void draw();
        void draw(int x, int y);
        void draw(int x, int y, int w, int h);
		bool isFrameNew();

    private:
        char* url;
		std::shared_ptr<ofx::Video::IPVideoGrabber> grabber;

        void setupGui();
        ofParameter<string> sourceUrl;
	    void setUrl(char* _value) {
            grabber->setURI(_value);
            grabber->connect(); // connect immediately
        }
};
