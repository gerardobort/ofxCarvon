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
	USER_CFLAGS = -I/usr/local/include/opencv -I/usr/local/include/opencv2
	USER_LDFLAGS = /usr/local/lib/libopencv_calib3d.dylib /usr/local/lib/libopencv_contrib.dylib /usr/local/lib/libopencv_core.dylib /usr/local/lib/libopencv_features2d.dylib /usr/local/lib/libopencv_flann.dylib /usr/local/lib/libopencv_gpu.dylib /usr/local/lib/libopencv_highgui.dylib /usr/local/lib/libopencv_imgproc.dylib /usr/local/lib/libopencv_legacy.dylib /usr/local/lib/libopencv_ml.dylib /usr/local/lib/libopencv_nonfree.dylib /usr/local/lib/libopencv_objdetect.dylib /usr/local/lib/libopencv_ocl.dylib /usr/local/lib/libopencv_photo.dylib /usr/local/lib/libopencv_stitching.dylib /usr/local/lib/libopencv_superres.dylib /usr/local/lib/libopencv_video.dylib /usr/local/lib/libopencv_videostab.dylib 

    # avoid ofxCv libs to be loaded, give place to brew's includes
    ADDON_LIBS_EXCLUDE = libs/opencv
    ADDON_LIBS_EXCLUDE += libs/opencv/%
    ADDON_INCLUDES_EXCLUDE = libs/opencv
    ADDON_INCLUDES_EXCLUDE += libs/opencv/%
