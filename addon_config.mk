meta:
    ADDON_NAME = ofxCarvon
    ADDON_DESCRIPTION = A toolkit for manipulating fisheye cameras, for VR, streamming, and surveillance, UHD performance tunned
    ADDON_AUTHOR = Gerardo Bort
    ADDON_TAGS = computer vision, cemrera, fisheye
    ADDON_URL = http://github.com/gerardobort/ofxCarvon


common:
	# requires opencv with opencl support installed
	USER_CFLAGS = -I/usr/local/include/opencv -I/usr/local/include 
    USER_LDFLAGS = -L/usr/local/opt/opencv3/lib
    USER_CPPFLAGS = -I/usr/local/opt/opencv3/include
    PKG_CONFIG_PATH = /usr/local/opt/opencv3/lib/pkgconfig

    ADDON_INCLUDES = libs
    ADDON_INCLUDES += src
    ADDON_SOURCES = $(OF_ROOT)/addons/ofxCarvon/src/ofxCarvon.cpp
    ADDON_SOURCES += $(OF_ROOT)/addons/ofxCarvon/src/ofxCarvon.h
    ADDON_LIBS_EXCLUDE = libs/opencv
    ADDON_LIBS_EXCLUDE += libs/opencv/%
    ADDON_INCLUDES_EXCLUDE = libs/opencv
    ADDON_INCLUDES_EXCLUDE += libs/opencv/%
