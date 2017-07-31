#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\calib3d.hpp"
#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

class Calibration
{
public:
	Calibration(void);
	~Calibration(void);
	void Calib();

	void calRealPoint(vector<vector<Point3f>>&obj,int boardWidth,int boardHeight,int imgNumber,float squaresize);
	int saveCameraParams(Mat cameraMatrix,Mat distortionCoefficients, const char* filename);
};