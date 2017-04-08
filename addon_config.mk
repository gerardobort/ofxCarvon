meta:
    ADDON_NAME = ofxCarvon
    ADDON_DESCRIPTION = A toolkit for manipulating fisheye cameras, for VR, streamming, and surveillance, UHD performance tunned
    ADDON_AUTHOR = Gerardo Bort
    ADDON_TAGS = computer vision, cemrera, fisheye
    ADDON_URL = http://github.com/gerardobort/ofxCarvon


common:
    # requires include opencv from external sources on an early stage at openframeworks constants
    # #include "/usr/local/include/opencv2/opencv.hpp"  --->  ~/Applications/openFrameworks/libs/openFrameworks/utils/ofConstants.h

	# requires opencv with opencl support installed
	USER_CFLAGS = -I/usr/local/include/opencv -I/usr/local/include 
	USER_LDFLAGS = /usr/local/lib/libopencv_calib3d.dylib /usr/local/lib/libopencv_ocl.dylib
