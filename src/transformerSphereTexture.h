#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "transformer.h"

#define GLSL120(shader)  "#version 120 \n #extension GL_ARB_texture_rectangle : enable \n" #shader
#define GLSL120PI(shader)  "#version 120 \n #extension GL_ARB_texture_rectangle : enable \n #define PI 3.1415926535897932 \n #define HALF_PI 1.5707963267948966 \n" #shader
#define GLSL120GEO(shader)  "#version 120 \n #extension GL_ARB_texture_rectangle : enable \n #extension GL_EXT_geometry_shader4: enable \n" #shader
#define GLSL150(shader)  "#version 150 \n" #shader
#define GLSL150PI(shader)  "#version 150 \n #define PI 3.1415926535897932 \n #define HALF_PI 1.5707963267948966 \n" #shader

class transformerSphereTexture: public transformer {
    public:
        transformerSphereTexture(int videoWidth, int videoHeight, char* name);

        void draw();

        void setupShader() {
            bInitialized = false;
            
            if (ofIsGLProgrammableRenderer())
                glThree();
            else
                glTwo();
        };

        void glTwo()  {
        };

        void glThree()  {
            vertexShader = GLSL150(
                in vec4 a_position;
                in vec4 a_color;
                in vec2 a_texCoord0;

                uniform mat4 modelViewProjectionMatrix;
                 
                out vec4 v_color;
                out vec4 v_position;
                out vec2 v_texCoord0;

                void main(){
                    gl_Position = modelViewProjectionMatrix * a_position;
                    v_color = a_color;
                    v_position = gl_Position;
                    v_texCoord0 = a_position.xy;
                }
            );

            fragmentShader = GLSL150PI(
                in vec4 v_color;
                in vec4 v_position;
                in vec2 v_texCoord0;

                uniform sampler2DRect u_sampler2d;
                uniform sampler2DRect u_sampler2dRuler;
                uniform bool u_calibrate;
                uniform float u_fovFactor;
                uniform int u_rectifyOverflow;
                uniform ivec2 u_videoSize;
                uniform ivec2 u_rulerSize;

                out vec4 outputColor;

                vec2 fish2sphere(vec2 pfish, vec2 textureSize, float fovFactor);
                 
                void main()
                {
                    vec4 videoColor;

                    float rulerRatio = u_rulerSize.x/float(u_videoSize.x);
                    vec2 coords = v_texCoord0; // TODO u_rectifyOverflow -> input.draw(-rectifyOverflow, rectifyOverflow, videoWidth + 2.0 * rectifyOverflow, videoHeight -2.0 * rectifyOverflow);
                    videoColor = texture(u_sampler2d, fish2sphere(coords, u_videoSize, u_fovFactor));
                    if (u_calibrate) {
                        videoColor += 0.7* texture(u_sampler2dRuler, fish2sphere(rulerRatio*coords, u_rulerSize, 1.0));
                    }

                    outputColor = videoColor;
                }

                // http://paulbourke.net/dome/fish2/
                vec2 fish2sphere(vec2 pfish, vec2 textureSize, float fovFactor)
                {
                    vec2 ret;
                    float theta;
                    float phi;
                    float r;
                    vec3 psph;
                    
                    // float FOV = 3.141592654*1.0; // FOV of the fisheye, eg: 180 degrees
                    float FOV = PI*fovFactor;

                    // Polar angles
                    theta = 2.0 * PI * (pfish.x / textureSize.x - 0.5); // -pi to pi
                    phi = PI * (pfish.y / textureSize.y - 0.5);  // -pi/2 to pi/2

                    // Vector in 3D space
                    psph.x = cos(phi) * sin(theta);
                    psph.y = cos(phi) * cos(theta);
                    psph.z = sin(phi);
                    
                    // Calculate fisheye angle and radius
                    theta = atan(psph.z, psph.x);
                    phi = atan(sqrt(psph.x*psph.x + psph.z*psph.z), psph.y);
                    r = textureSize.y * phi / FOV; 

                    // Pixel in fisheye space
                    ret.x = 0.5 * textureSize.x + r * cos(theta);
                    ret.y = 0.5 * textureSize.y + r * sin(theta);
                    
                    return ret;
                }

                vec2 fish2pano(vec2 pfish, vec2 textureSize, float fovFactor)
                {
                    vec2 ret;
                    float theta;
                    float phi;
                    float r;
                    vec3 psph;
                    
                    // float FOV = 3.141592654*1.0; // FOV of the fisheye, eg: 180 degrees
                    float FOV = PI*fovFactor;

                    // Polar angles
                    theta = 2.0 * PI * (pfish.x / textureSize.x - 0.5); // -pi to pi
                    phi = PI * (pfish.y / textureSize.y - 0.5);  // -pi/2 to pi/2

                    // Vector in 3D space
                    psph.x = cos(phi) * sin(theta);
                    psph.y = cos(phi) * cos(theta);
                    psph.z = sin(phi);
                    
                    // Pixel in pano space
                    r = textureSize.y * phi / FOV; 
                    ret.x = 0.5 * textureSize.x + r * cos(theta);
                    ret.y = 0.5 * textureSize.y + r * sin(theta);
                    
                    return ret;
                }
            );

            bInitialized *= shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
            bInitialized *= shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
            bInitialized *= shader.bindDefaults();
            bInitialized *= shader.linkProgram();
        }

    private:
        void setupGui();

        ofShader shader;
        ofMesh mesh;
        ofImage image;

        ofParameter<bool> calibrate;
        ofParameter<float> rectifyFovFactor;
        ofParameter<int> rectifyOverflow;

        bool bInitialized;
        string fragmentShader;
        string vertexShader;
};
