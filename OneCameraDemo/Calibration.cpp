#include "stdafx.h"
#include "Calibration.h"
#include<math.h>
#include"cv.h"
#include"cxcore.h"
#include"highgui.h"
#include<iomanip>

using namespace cv;
Calibration::Calibration(void)
{
}


Calibration::~Calibration(void)
{
}


static   double  computeCailbError( vector<vector<Point3f>>&objectPoint, vector<vector<Point2f>>&imagePoint,
	                                                vector<Mat>&rvecs,vector<Mat>&tvecs,Mat& cameraMatrix,
													 Mat&distCoeffs/*vector<float>&perViewErrors*/);


//进行单目相机标定
void Calibration::Calib()
{
	int  nx=7, ny=7, frame = 0, n_boards =20, N;
	int count1 = 0,count2 = 0, result1=0;
    int  successes1 = 0,successes2 = 0 ;
   	const int maxScale = 1;
	const float squareSize = 0.025f;	//0.023
	//const int squareSize = 25;//mm
	CvSize imageSize = {0,0};
	CvSize board_sz = cvSize( nx,ny );
	int nPointsPerImage = nx*ny;
	
	int i, j, n = nx*ny, N1 = 0, N2 = 0;
    vector<CvPoint2D32f> points[2];
	vector<int> npoints;
	vector<vector<Point3f>> objectPoints;
	vector<vector<Point2f>> temp;
	vector<vector<cv::Point2f> >	imagePoints1;
	//进行角点初始化
	objectPoints.resize(n_boards,vector<Point3f>(nPointsPerImage,Point3f(0,0,0)));
	imagePoints1.resize(n_boards,vector<Point2f>(nPointsPerImage,Point2f(0,0)));
	CvCapture *capture;
     capture = cvCreateCameraCapture(0);
	Mat cameraMatrix;
	Mat distortionCoefficients;
	vector<Mat> rotations;
	vector<Mat> translations;
	cameraMatrix.create(3,3,CV_64FC1);
	distortionCoefficients.create(5,1,CV_64FC1);
	int flag = 0;
	vector<vector<Point2f>> corners;
	vector<Point2f> corner1;

	//Start webcam
		printf("\nWebcams are starting ...\n");
		cout<<"\nWebcams are starting ...\n"<<endl;
		Mat frame1 = cvQueryFrame(capture);
		imshow("1",frame1);
		waitKey(0);
		Mat gray_fr1;
		imageSize = Size(frame1.cols,frame1.rows);
		cvNamedWindow( "camera1", 1 );
		cvNamedWindow("corners camera1",1);
		
		while(successes1<n_boards)			
	  {
	    	
	//--------Find and Draw chessboard--------------------------------------------------		
		  vector<Point2f>&corner1 = imagePoints1[successes1];
	
			cvtColor(frame1,gray_fr1,CV_BGR2GRAY);
            int flags = CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS;//CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK;
			bool find = findChessboardCorners(frame1,board_sz,corner1,flags);
			if(corner1.size()==n&&find==true)
			{
				   Size regionSize(11,11);
				    cornerSubPix( gray_fr1, corner1, regionSize, cvSize(-1,-1),cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,30, 0.01) );
					drawChessboardCorners( frame1, board_sz, corner1, find);
					imshow("corners camera1",frame1);
					corners.push_back(corner1);
					++successes1;
					cout<<"\nNumber of couple Chessboards were found:  "<<successes1<<endl;
			}
			
			else
			{		
				    imshow( "corners camera1", gray_fr1 );	
			}
		    frame1 = cvQueryFrame(capture);
			if(cvWaitKey(15)==27) break;
     	}		
	
		cvDestroyWindow("camera1");
		cvDestroyWindow("camera2");
		cvDestroyWindow("corners camera1");
		cvDestroyWindow("corners camera2");	
		cout<<"\nDone Capture!"<<endl;
		
	for(int  i1 = 0; i1< n_boards; i1++ )
	{
		int n1 = 0;
		for(int j1 = 0; j1 < ny; j1++ )
			for(int  k = 0; k < nx; k++ )
				objectPoints[i1][n1++] = cv::Point3f(j1*squareSize, k*squareSize, 0);
	}
		    printf("\nRunning stereo calibration ...");                                                                      
		    calibrateCamera(objectPoints,corners,imageSize,cameraMatrix,distortionCoefficients,rotations,translations,0);
		    printf("\nDone Calibration");
			printf("\nSaving matries for later use ...\n");                                               
			cv::FileStorage fs1("C:\\Users\\Li\\Desktop\\cameraMatrix.yml", cv::FileStorage::WRITE);
	        fs1<<"cameraMatrix"<<cameraMatrix;
	        cv::FileStorage fs2("C:\\Users\\Li\\Desktop\\distortionCoefficients.yml", cv::FileStorage::WRITE);
	        fs2<<"distortionCoefficients"<<distortionCoefficients;
	        fs1.release();
	        fs2.release();
			

			Mat resultImage;
	       //进行校正后测试
			undistort(frame1,resultImage,cameraMatrix,distortionCoefficients);
			imshow("result",resultImage);
			waitKey(0);
		    //添加标定测试的代码，需要针对物体中的点CvMat转换为vector<vector<Point3f>>
			double err;
			err = computeCailbError(objectPoints,imagePoints1,rotations,translations,cameraMatrix,distortionCoefficients);
			cout<<err<<endl;
}

//进行标定测试，通过计算真实坐标系中的点与标定系数的关系，重新把真实坐标系的点投射到屏幕上，与拍摄到的图片中的焦点对比，求误差
static   double  computeCailbError( vector<vector<Point3f>>&objectPoint, vector<vector<Point2f>>&imagePoint,
	                                                 vector<Mat>&rvecs,vector<Mat>&tvecs,Mat& cameraMatrix,
													 Mat&distCoeffs/*vector<float>&perViewErrors*/)
{
	cv::Mat imagePoints2;
	int totalPoints = 0;
	double totalErr = 0;

	size_t nImages = objectPoint.size();

	for (int i = 0; i < nImages; i++)
	{
		// 提取当前棋盘对应的角点坐标子矩阵
		vector<cv::Point3f>& objectPoints = objectPoint[i];
		vector<cv::Point2f>& imagePoints = imagePoint[i];
		totalPoints += objectPoints.size();

		// 计算重投影点的坐标
		projectPoints(
			Mat(objectPoint[i])/*objectPoints*/, 
			rvecs[i], 
			tvecs[i],
			cameraMatrix, 
			distCoeffs,
			imagePoints2);

		// 计算重投影误差
		double erri = norm(Mat(imagePoint[i])/*imagePoints*/, imagePoints2, CV_L2);
		totalErr += erri * erri;
	}

	// 平均的重投影误差
	double err = std::sqrt(totalErr / totalPoints);

	return err;

}


void Calibration::calRealPoint(vector<vector<Point3f>>&obj,int boardWidth,int boardHeight,int imgNumber,float squaresize)
{
	vector<Point3f>imgPoint;
	for (int row = 0; row < boardHeight; row++)
	{
		for (int col = 0; col < boardWidth; col++)
		{
			imgPoint.push_back(Point3f(row*squaresize,col*squaresize,0));
		}
	}
	for (int imgd = 0; imgd < imgNumber; imgd++)
	{
		obj.push_back(imgPoint);
	}
}

//保存相机内参与畸变系数
int Calibration::saveCameraParams(Mat cameraMatrix,Mat distortionCoefficients, const char* filename )
{
	std::string filename_ = filename;

	//写入数据
	//cv::FileStorage fs(filename_.c_str(), cv::FileStorage::WRITE);
	cv::FileStorage fs1("cameraMatrix.yml", cv::FileStorage::WRITE);
	fs1<<"cameraMatrix"<<cameraMatrix;
	cv::FileStorage fs2("distortionCoefficients.yml", cv::FileStorage::WRITE);
	fs2<<"distortionCoefficients"<<distortionCoefficients;
	fs1.release();
	fs2.release();
	return 0;
}
