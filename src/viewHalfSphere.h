#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMathMesh.h"
#include "ofxSphereCam.h"
#include "view.h"


class Sphere:public ofxParametricSurface {
public:
    ofPoint valueForPoint(float u, float v){
        float x = cos(u) * sin(v);
        float y = sin(u) * sin(v);
        float z = cos(v);
        return ofPoint(x, y, z);
    }

    ofFloatColor colorForPoint(float u, float v, ofPoint value){
        return ofFloatColor::white;
    }

    ofFloatColor backColorForPoint(float u, float v, ofPoint value){
        return ofFloatColor::white;
    }

    ofPoint textureSize;
    void setTextureSize(ofPoint size) {
        textureSize = size;
        this->reload();
    }

    // inside sphere
    ofVec2f texCoordForPoint(float u, float v, ofPoint value){
        return ofVec2f(textureSize.x/M_PI *u, textureSize.y/M_PI*v);
    }

    // outside sphere
    ofVec2f backTexCoordForPoint(float u, float v, ofPoint value){
        return ofVec2f(textureSize.x/M_PI *u, textureSize.y/M_PI*v);
    }
};

class viewHalfSphere: public view {
    public:
        viewHalfSphere(int videoWidth, int videoHeight, char* name, int textureSideSize);

        void update();
        void draw();
        void keyPressed(int key);

    private:
        void setupGui();

        Sphere sphereDome;
        ofBoxPrimitive box;
        ofxSphereCam sCam;
        int textureSideSize;

        ofParameter<bool> calibrate;
        ofParameter<float> sphereStep;
        ofParameter<float> sphereRadius;
        ofParameter<float> sphereDistance;

	    void reload(float& _value) {
            sphereDome = Sphere();
            sphereDome.setup(0, M_PI, 0, M_PI, _value, _value);
            sphereDome.setTextureSize(ofPoint(textureSideSize));
        }
};
