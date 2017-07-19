#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "transformer.h"

#define GLSL120(shader)  "#version 120 \n #extension GL_ARB_texture_rectangle : enable \n" #shader
#define GLSL120PI(shader)  "#version 120 \n #extension GL_ARB_texture_rectangle : enable \n #define PI 3.1415926535897932 \n #define HALF_PI 1.5707963267948966 \n" #shader
#define GLSL120GEO(shader)  "#version 120 \n #extension GL_ARB_texture_rectangle : enable \n #extension GL_EXT_geometry_shader4: enable \n" #shader
#define GLSL150(shader)  "#version 150 \n" #shader
#define GLSL150PI(shader)  "#version 150 \n #define PI 3.1415926535897932 \n #define HALF_PI 1.5707963267948966 \n" #shader

class transformerChromaKey: public transformer {
    public:
        transformerChromaKey(int videoWidth, int videoHeight, char* name);

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
                uniform vec3 u_colorKey;
                uniform vec3 u_colorReplacement;
                uniform float u_threshold;
                uniform ivec2 u_videoSize;

                out vec4 outputColor;
                 
                void main()
                {
                    vec4 videoColor = texture(u_sampler2d, v_texCoord0);
					if (length(videoColor.rgb - u_colorKey) < u_threshold) {
						videoColor = vec4(u_colorReplacement, 1.0);
					}

                    outputColor = videoColor;
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

        ofParameter<ofVec3f> colorKey;
        ofParameter<ofVec3f> colorReplacement;
        ofParameter<float> threshold;

        bool bInitialized;
        string fragmentShader;
        string vertexShader;
};
