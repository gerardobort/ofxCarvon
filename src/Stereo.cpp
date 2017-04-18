#include "Stereo.h"
#include "ofGraphics.h"

namespace ofxCv {
	
	using namespace cv;


    // --------------
	Camera::Camera(int numSamples): numSamples(numSamples){
        //cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        //distortionCoefficients = cv::Mat::zeros(4, 1, CV_64F); // There are 4 distortion coefficients in fisheye model (instead of 8)
        //cameraMatrixRefined = cv::Mat::eye(3, 3, CV_64F);
        //isReady = false;
        cv::Matx33d cameraMatrix(443.9760827244972, 0, 647.8958956682515,
          0, 259.9864732380117, 388.1746347022338,
          0, 0, 1);
        cv::Vec4d distortionCoefficients (-0.04959317625347989,
          0.005313175245151469,
          -0.004160818224412319,
          0.001018235747365488);
        cv::Matx33d cameraMatrixRefined(368.5408978148645, 0, 639.5,
          0, 215.8126349484327, 399.5,
          0, 0, 1);
        isReady = true;
        imageSize = cv::Size(1280, 800);
        cv::fisheye::initUndistortRectifyMap(cameraMatrix, distortionCoefficients, Mat::eye(3, 3, CV_64F), cameraMatrixRefined, imageSize, CV_32F, mapx, mapy);
/*
RMS: 0.539341
Distortion coefficients: [-0.04959317625347989;
  0.005313175245151469;
  -0.004160818224412319;
  0.001018235747365488]
Camera matrix: [443.9760827244972, 0, 647.8958956682515;
  0, 259.9864732380117, 388.1746347022338;
  0, 0, 1]
Camera matrix refined: [368.5408978148645, 0, 639.5;
  0, 215.8126349484327, 399.5;
  0, 0, 1]
*/

        objectPoints = std::vector<std::vector<cv::Point3d> >(numSamples);
        imagePoints = std::vector<std::vector<cv::Point2d> >(numSamples);
	}
	
	Camera::~Camera(){
	}

	bool Camera::findChessboardCorners(ofImage& srcImage, int indexSample, ofPolyline& corners){
        ofImage tmpImage;
        tmpImage.clone(srcImage);
        tmpImage.setImageType(OF_IMAGE_GRAYSCALE);
        cv::Mat grayscaleImage = toCv(tmpImage);

        boardSize = cv::Size(9, 7);
        imageSize = grayscaleImage.size();

        // CALIB_CB_FAST_CHECK saves a lot of time on images
        // that do not contain any chessboard corners
        bool success = cv::findChessboardCorners(grayscaleImage, boardSize, imagePoints[indexSample], CV_CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FILTER_QUADS | CALIB_CB_FAST_CHECK);

        if (success) {
            // call-again for better corner detection
			// requires imagePoints in Point2f
            // cv::cornerSubPix(grayscaleImage, imagePoints[indexSample], cv::Size(11, 11), cv::Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 0.1));

            float squareSize = 21.0f; // This is "1 arbitrary unit"
            objectPoints[indexSample] = Create3DChessboardCorners(boardSize, squareSize);

            //drawChessboardCorners(grayscaleImage, boardSize, Mat(imagePoints[indexSample]), success);
            // http://programmingexamples.net/wiki/OpenCV/CheckerboardCalibration

            corners = toOf(imagePoints[indexSample]);
            if (indexSample == numSamples -1) {
                isReady = true;
            }
        }

        return success;
    }

	void Camera::calibrate(){
        //int flags = fisheye::CALIB_CHECK_COND|fisheye::CALIB_FIX_SKEW; // CV_CALIB_FIX_PRINCIPAL_POINT mess everything when centerPrincipalPoint=false
        int flags = 0 | cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC | cv::fisheye::CALIB_CHECK_COND | cv::fisheye::CALIB_FIX_SKEW;

        // TODO depending on flags some parameters must be initialized
        double rms = cv::fisheye::calibrate(objectPoints, imagePoints, imageSize, cameraMatrix, distortionCoefficients, cv::noArray(), cv::noArray(), flags, cv::TermCriteria(3, 20, 1e-6));

        std::cout << "RMS: " << rms << std::endl;
        std::cout << "Distortion coefficients: " << distortionCoefficients << std::endl;
        std::cout << "Camera matrix: " << cameraMatrix << std::endl;

        cameraMatrixRefined = cv::getOptimalNewCameraMatrix(cameraMatrix, distortionCoefficients, imageSize, 1, imageSize, 0, true);
        std::cout << "Camera matrix refined: " << cameraMatrixRefined << std::endl;

        // helps rectifying faster (expanded method)
        cv::fisheye::initUndistortRectifyMap(cameraMatrix, distortionCoefficients, Mat::eye(3, 3, CV_64F), cameraMatrixRefined, imageSize, CV_32F, mapx, mapy);
        std::cout << "Camera::calibrate() done" << std::endl;
    }

    void Camera::rectify(ofImage srcImage, ofImage& dstImage) {
        cv::Mat srcMat = toCv(srcImage);
        cv::Mat dstMat;

        // method 1
        // unexpanded method (less efficient)
        // cv::undistort(srcMat, dstMat, cameraMatrixRefined, distortionCoefficients);

        // method 2
        // expanded method (more efficient, requires mapx, mapy)
        remap(srcMat, dstMat, mapx, mapy, INTER_LINEAR);

        // copy resultant matrix into dstImage
        toOf(dstMat, dstImage);
        dstImage.setImageType(OF_IMAGE_COLOR);
    }

    std::vector<cv::Point3d> Camera::Create3DChessboardCorners(cv::Size boardSize, float squareSize) {
        // This function creates the 3D points of your chessboard in its own coordinate system
        std::vector<cv::Point3d> corners;
     
        for( int i = 0; i < boardSize.height; i++ )
        {
            for( int j = 0; j < boardSize.width; j++ )
            {
                corners.push_back(cv::Point3d(float(j*squareSize), float(i*squareSize), 0));
            }
        }
     
        return corners;
    }







	
    // --------------
	Stereo::Stereo(int ndisparities, int SADWindowSize)
    :ndisparities(ndisparities)
    ,SADWindowSize(SADWindowSize) {
        isReady = false;
        reload();
	}
	
	Stereo::~Stereo(){
	}

    void Stereo::setNDisparities(int n) {
        if (n % 16 != 0) {
            ofLogWarning() << "ofxCv::Stereo: Invalid nDisparities value " << n << ".  The value must be multiplier of 16.";
            return;
        }
        ndisparities = n;
    }

    void Stereo::setSADWindowSize(int s) {
        if (s < 5 || s > 255 || s % 2 == 0 || s > imgDisparity8U.cols || s > imgDisparity8U.cols) {
            ofLogWarning() << "ofxCv::Stereo: Invalid SADWindowSize value " << s << ".  The value must be odd, be within 5..255 and be not larger than image width or height in function.";
            return;
        }
        SADWindowSize = s;
    }

    void Stereo::reload() {
        /*
            StereoSGBM
                int  minDisparity,
                int  numDisparities,
                int  SADWindowSize
        */
        // slow for real-time, uses cpu
        // sbm = new StereoBM(StereoBM::PREFILTER_XSOBEL, ndisparities, SADWindowSize);
        /*
            StereoSGBM
                int  minDisparity,
                int  numDisparities,
                int  SADWindowSize,
                int  P1 = 0,
                int  P2 = 0,
                int  disp12MaxDiff = 0,
                int  preFilterCap = 0,
                int  uniquenessRatio = 0,
                int  speckleWindowSize = 0,
                int  speckleRange = 0,
                bool fullDP = false 
        // slow for real-time, uses cpu
        */


        sbm = new ocl::StereoBM_OCL(ocl::StereoBM_OCL::BASIC_PRESET, ndisparities, SADWindowSize);

        /*
            int ndisp – Number of disparities.
            int iters – Number of BP iterations on each level.
            int levels – Number of levels.
            [
            float max_data_term – Threshold for data cost truncation.
            float data_weight – Data weight.
            float max_disc_term – Threshold for discontinuity truncation.
            float disc_single_jump – Discontinuity single jump.
            int msg_type – Type for messages. CV_16SC1 and CV_32FC1 types are supported.
            ]
         */
        // too slow for real-time
        //sbm = new ocl::StereoBeliefPropagation(ndisparities, SADWindowSize, 8,    1, 1, 1, 1, CV_16S);

        /*
            int ndisp – Number of disparities.
            int iters – Number of BP iterations on each level.
            int levels – Number of levels.
            int nr_plane – Number of disparity levels on the first level.
            [
            float max_data_term – Truncation of data cost.
            float data_weight – Data weight.
            float max_disc_term – Truncation of discontinuity.
            float disc_single_jump – Discontinuity single jump.
            int min_disp_th – Minimal disparity threshold.
            ]
            int msg_type – Type for messages. CV_16SC1 and CV_32FC1 types are supported.

         */
        // too slow for real-time
        //sbm = new ocl::StereoConstantSpaceBP(ndisparities, 16, 1, 16, CV_16S);
    }
	
	//call with two images
	void Stereo::compute(Mat leftImage, Mat rightImage){
        imgDisparity16S = ocl::oclMat(leftImage.rows, leftImage.cols, CV_16S);
        imgDisparity8U = ocl::oclMat(leftImage.rows, leftImage.cols, CV_8UC3);

		ocl::oclMat oclLeftImage = ocl::oclMat(leftImage);
		ocl::oclMat oclRightImage = ocl::oclMat(rightImage);

        //-- 3. Calculate the disparity image
        sbm->operator()(oclLeftImage, oclRightImage, imgDisparity16S);
        std::cout << "Stereo::compute() done" << std::endl;
	}
	
	void Stereo::draw(){
        double minVal, maxVal;

        //-- Check its extreme values
        minMaxLoc(imgDisparity16S, &minVal, &maxVal);

        //-- 4. Display it as a CV_8UC1 image
        imgDisparity16S.convertTo(imgDisparity8U, CV_8UC3, (255)/(maxVal - minVal));
		Mat cvImgDisparity8U;
		imgDisparity8U.download(cvImgDisparity8U);

        ofPixels pix8u;
        toOf(cvImgDisparity8U, pix8u);
        ofImage img;
        img.setFromPixels(pix8u);
        img.draw(0, 0);
	}


    void Stereo::calibrate(Camera& leftCamera, Camera& rightCamera) {
        if (!leftCamera.isReady || !rightCamera.isReady) {
            std::cout << "Error: Cameras not ready yet for setero calibration: missing camera calibration" << std::endl;
            return;
        }
        std::cout << "Stereo::calibrate() stereoCalibrate->starting..." << std::endl;

        // TODO depending on flags some parameters must be initialized
        double res = cv::fisheye::stereoCalibrate(
            leftCamera.objectPoints,
            leftCamera.imagePoints, rightCamera.imagePoints,
            leftCamera.cameraMatrixRefined, leftCamera.distortionCoefficients,
            rightCamera.cameraMatrixRefined, rightCamera.distortionCoefficients,
            leftCamera.imageSize,
            // output matrices
            R, T,
            0 | /*fisheye::CALIB_RECOMPUTE_EXTRINSIC | fisheye::CALIB_CHECK_COND | */cv::fisheye::CALIB_FIX_SKEW,// | cv::fisheye::CALIB_FIX_INTRINSIC,
            cv::TermCriteria(3, 12, 0));

        std::cout << "R: " << R << std::endl;
        std::cout << "T: " << T << std::endl;
        //std::cout << "E: " << E << std::endl;
        //std::cout << "F: " << F << std::endl;

        std::cout << "Stereo::calibrate() stereoCalibrate->done" << std::endl;
        double balance = 0.0, fov_scale = 1.1;

        // Starting Rectification
        cv::fisheye::stereoRectify(
            leftCamera.cameraMatrixRefined, leftCamera.distortionCoefficients,
            rightCamera.cameraMatrixRefined, rightCamera.distortionCoefficients,
            leftCamera.imageSize, R, T,
            R1, R2, P1, P2, Q, 0 | cv::CALIB_ZERO_DISPARITY, leftCamera.imageSize, balance, fov_scale); // see alpha param

        std::cout << "Stereo::calibrate() stereoRectify->done" << std::endl;

        std::cout << "R1" << R1 << std::endl;
        std::cout << "R2" << R2 << std::endl;
        std::cout << "P1" << P1 << std::endl;
        std::cout << "P2" << P2 << std::endl;
        std::cout << "Q" << Q << std::endl;

        // Applying Undistort
        cv::initUndistortRectifyMap(leftCamera.cameraMatrixRefined, leftCamera.distortionCoefficients, R1, P1, leftCamera.imageSize, CV_32F, map1x, map1y);
        cv::initUndistortRectifyMap(rightCamera.cameraMatrixRefined, rightCamera.distortionCoefficients, R2, P2, rightCamera.imageSize, CV_32F, map2x, map2y);

        std::cout << "Stereo::calibrate() initUndistortRectifyMap->done" << std::endl;


        /*
            // This commented out code replaces the exisintg method by making stereoRectify better than individual rectify calls, but in general yields worst results because of unknown results... in theory should be better.
        Mat CM1 = Mat(3, 3, CV_64FC1);
        Mat CM2 = Mat(3, 3, CV_64FC1);
        Mat D1, D2;
        double res = cv::stereoCalibrate(
            leftCamera.objectPoints,
            leftCamera.imagePoints, rightCamera.imagePoints,
            CM1, D1,
            CM2, D2,
            leftCamera.imageSize,
            // output matrices
            R, T, E, F,
            cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5), 
            CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST | CV_CALIB_FIX_PRINCIPAL_POINT);

        // Starting Rectification
        stereoRectify(
            CM1, D1, CM2, D2,
            leftCamera.imageSize, R, T,
            R1, R2, P1, P2, Q, 0); // see alpha param

        // Applying Undistort
        cv::initUndistortRectifyMap(CM1, D1, R1, P1, leftCamera.imageSize, CV_32FC1, map1x, map1y);
        cv::initUndistortRectifyMap(CM2, D2, R2, P2, rightCamera.imageSize, CV_32FC1, map2x, map2y);
        */

        // Done Rectification
        isReady = true;
        std::cout << "Stereo::calibrate() done" << std::endl;
    }

    void Stereo::rectify(ofImage& leftImage, ofImage& rightImage) {
        Mat img1 = toCv(leftImage);
        Mat img2 = toCv(rightImage);

        Mat imgL, imgR;
        cv::remap(img1, imgL, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
        cv::remap(img2, imgR, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

        ofPixels pixelsL;
        toOf(imgL, pixelsL);
        leftImage.setFromPixels(pixelsL);

        ofPixels pixelsR;
        toOf(imgR, pixelsR);
        rightImage.setFromPixels(pixelsR);
    }


    void Stereo::rectifyLeft(ofImage& leftImage) {
        cv::Mat src = toCv(leftImage);
        cv::Mat dst = Mat::zeros(src.rows, src.cols, src.type());
        // http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/warp_affine/warp_affine.html
        Mat A = (Mat_<double>(3, 2) << 1, 1,   0, 1,   1, 0);
        Mat B(3, 2, CV_32F);
        B = E * A;

        Mat A1(3, 2, CV_32FC1);
        Mat B1(3, 2, CV_32FC1);
        A.assignTo(A1, CV_32FC1);
        B.assignTo(B1, CV_32FC1);

        cv::Mat left2Right = getAffineTransform(A1, B1);
        std::cout << "left2Right: " << left2Right << std::endl;

        //cv::Mat rot_mat = getRotationMatrix2D(Point2f(src.cols/2.0, src.rows/2.0), 90, 1.2);
        //cv::warpAffine(src, dst, rot_mat, dst.size());
        cv::warpAffine(src, dst, left2Right, dst.size());
        
        ofPixels pixels;
        toOf(dst, pixels);
        leftImage.setFromPixels(pixels);
    }
        
}
