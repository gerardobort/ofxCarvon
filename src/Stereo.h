#pragma once

#include "ofxCv.h"

namespace ofxCv {

    /**
     * Reference:
     *  @see https://github.com/opencv/opencv/blob/master/samples/cpp/tutorial_code/calib3d/stereoBM/SBM_Sample.cpp
     *  @see http://docs.opencv.org/2.4.1/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
     *  @author Gerardo Bort <gerardobort@gmail.com>
     */



    // --------------
	class Camera {
	public:
		Camera(int numSamples);
		virtual ~Camera();

        // chessboard calibration
	    bool findChessboardCorners(ofImage& srcImage, int indexSample, ofPolyline& corners);
        vector<cv::Point3f> Create3DChessboardCorners(cv::Size boardSize, float squareSize);
	    void calibrate();

        void rectify(ofImage srcImage, ofImage& dstImage);
        
        // after successful camera calibration the following properties should be all set
        cv::Size imageSize;
        cv::Size boardSize;
        std::vector<std::vector<cv::Point3f> > objectPoints;
        std::vector<std::vector<cv::Point2f> > imagePoints;
        std::vector<cv::Mat> rotationVectors;
        std::vector<cv::Mat> translationVectors;
        cv::Mat distortionCoefficients;
        cv::Mat cameraMatrix;
        cv::Mat cameraMatrixRefined;

        cv::Mat map1, map2;

        int numSamples;
        bool isReady;
        
    private:
    protected:
	};






	
    // --------------
	class Stereo {
	public:
		Stereo(int ndisparities, int SADWindowSize);
		virtual ~Stereo();
		
        template <class T>
		void compute(T& leftImage, T& rightImage) {
            // StereoBM requires CV_8UC1 for both input images, StereoSGBM accepts color inputs
            // leftImage.setImageType(OF_IMAGE_GRAYSCALE);
            // rightImage.setImageType(OF_IMAGE_GRAYSCALE);

            compute(toCv(leftImage), toCv(rightImage));
        }
		void compute(cv::Mat leftImage, cv::Mat rightImage);
		
        void setNDisparities(int n);
        void setSADWindowSize(int s);
        void reload();
		void draw();


        // chessboard calibration
        template <class T>
		ofPolyline calibrate(T& image, bool& success) {
            return toOf(calibrate(toCv(image), success));
        }
	    vector<cv::Point2f> calibrate(cv::Mat image, bool& success);
        vector<cv::Point3f> Create3DChessboardCorners(cv::Size boardSize, float squareSize);

        /*
        R – Output rotation matrix between the 1st and the 2nd camera coordinate systems.
        T – Output translation vector between the coordinate systems of the cameras.
        E – Output essential matrix.
        F – Output fundamental matrix.
        */
        //cv::Mat_<double> cameraMatrix1(3,3); // 3x3 matrix
        //cv::Mat_<double> distCoeffs1(5,1);   // 5x1 matrix for five distortion coefficients
        //cv::Mat_<double> cameraMatrix2(3,3); // 3x3 matrix
        //cv::Mat_<double> distCoeffs2(5,1);   // 5x1 matrix
        //cv::Mat_<double> R(3,3);             // 3x3 matrix, rotation left to right camera
        //cv::Mat_<double> T(3,1);             // * 3 * x1 matrix, translation left to right proj. center
        cv::Mat R, T, E, F;
        cv::Mat R1, R2, P1, P2, Q;
        cv::Mat map1x, map1y, map2x, map2y;
        void calibrate(Camera& leftCamera, Camera& rightCamera);
        void rectifyLeft(ofImage& leftImage);
        void rectify(ofImage& leftImage, ofImage& rightImage);
        bool isReady;

		void setP1(int v) { p1 = v; };
		void setP2(int v) { p2 = v; };
		void setDisp12MaxDiff(int v) { disp12MaxDiff = v; };
		void setPreFilterCap(int v) { preFilterCap = v; };
		void setUniquenessRatio(int v) { uniquenessRatio = v; };
		void setSpeckleWindowSize(int v) { speckleWindowSize = v; };
		void setSpeckleRange(int v) { speckleRange = v; };

    private:
        int ndisparities = 16*5; /**< Range of disparity */
        int SADWindowSize = 21; /**< Size of the block window. Must be odd */
		int p1;
		int p2;
		int disp12MaxDiff;
		int preFilterCap;
		int uniquenessRatio;
		int speckleWindowSize;
		int speckleRange;


        cv::Mat imgDisparity16S;
        cv::Mat imgDisparity8U;
        //cv::Ptr<cv::StereoBM> sbm;
        cv::Ptr<cv::StereoSGBM> sbm;
        
    protected:
	};

	
}
