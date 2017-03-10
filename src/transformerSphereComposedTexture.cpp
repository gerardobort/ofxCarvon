#include "transformerSphereComposedTexture.h"

//--------------------------------------------------------------
transformerSphereComposedTexture::transformerSphereComposedTexture(int videoWidth, int videoHeight, char* name)
    : transformer(videoWidth, videoHeight, name) {
    setupGui();
}

//--------------------------------------------------------------
void transformerSphereComposedTexture::update(){
    transformer::update();
}

//--------------------------------------------------------------
void transformerSphereComposedTexture::draw(){
    int w = input1.getWidth();
    int h = input1.getHeight();

    ofClear(0);
    ofBackground(0, 0, 0);
    if (displayInput1) {
        // center half of composed texture
        input1.draw(w * 0.5, 0, w, h);
    }
    if (displayInput2) {
        // left quarter of composed texture
        input2.getTextureReference()
            .drawSubsection(0, 0, videoWidth * 0.25, videoHeight, w * 0.5, 0, -w * 0.5, h);
        // right quarter of composed texture
        input2.getTextureReference()
            .drawSubsection(videoWidth, 0, -videoWidth * 0.25, videoHeight, w * 0.5, 0, w * 0.5, h);
    }
}

//--------------------------------------------------------------
void transformerSphereComposedTexture::setupGui() {
    parameters.add(displayInput1.set("display input 1", true));
    parameters.add(displayInput2.set("display input 2", true));
}
