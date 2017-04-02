meta:
    ADDON_NAME = ofxCarvon
    ADDON_DESCRIPTION = A toolkit for manipulating fisheye cameras, for VR, streamming, and surveillance, UHD performance tunned
    ADDON_AUTHOR = Gerardo Bort
    ADDON_TAGS = computer vision, cemrera, fisheye
    ADDON_URL = http://github.com/gerardobort/ofxCarvon


common:
	# requires opencv with opencl support installed
	USER_CFLAGS = -I/usr/local/include/opencv -I/usr/local/include 
	USER_LDFLAGS = /usr/local/lib/libopencv_ocl.dylib
