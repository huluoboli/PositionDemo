// OneCameraDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Calibration.h"
#include<math.h>
#include"cv.h"
#include"cxcore.h"
#include"highgui.h"
#include<iomanip>
#include <iostream>
#include "Mark.h"
#include "ORBTrack.h"
#include "CompareNum.h"

using namespace cv;
using namespace std;


//将空间点绕着Z轴旋转
void RotateZ(double x,double y,double thetaz,double &outx,double &outy)
{
	double x1=x;//将变量拷贝一次，确保&x==&outx这种情况下也能计算正确
	double y1=y;
	double rz = thetaz*CV_PI/180;
	outx = cos(rz)*x1-sin(rz)*y1;
	outy = sin(rz)*x1+cos(rz)*y1;
}

//围绕Y轴进行旋转
void RotateY(double x,double z,double thetay,double &outx,double &outz)
{
	double x1=x;
	double z1=z;
	double ry = thetay*CV_PI/180;
	outx=cos(ry)*x1+sin(ry)*z1;
	outz=cos(ry)*z1-sin(ry)*x1;
}

//围绕X轴进行旋转
void RotateX(double y,double z,double thetax,double &outy,double &outz)
{
	double y1=y;
	double z1=z;
	double rx = thetax*CV_PI/180;
	outy = cos(rx)*y1-sin(rx)*z1;
	outz = cos(rx)*z1+sin(rx)*y1;
}



int _tmain(int argc, _TCHAR* argv[])
{

	//打开相机
	CvCapture *capture;
	capture = cvCreateCameraCapture(0 );
	if (capture==0)
	{
		cout<<"Camera is not open"<<endl;
		return 0;
	}
	IplImage *frame = NULL;
	CompareNum compara;
	//进行相机标定
	//Calibration cali;
	//cali.Calib();
	//对模板进行特征点检测
	Mat gray,firstgray;
	vector<KeyPoint> keypoints,nextkeypoints,firstkeypoints,goodpoint; 
	Mat descriptors, nextdescriptors,firstdescriptors; 
	BFMatcher matcher(NORM_HAMMING);
	vector<vector<DMatch>> matches;
	vector<DMatch>match,good;
	vector<cv::Point3f>Points3D;
	vector<cv::Point2f>Points2D;
	double distance; 
	ORB orb;
	Mat temp = imread("C:\\Users\\Li\\Desktop\\demo.JPG",1);
	cvtColor(temp,gray,CV_BGR2GRAY);
	orb.detect(gray, keypoints);
	orb.compute(gray, keypoints, descriptors);
	vector<vector<Point>>allContours;
	vector<std::vector<Point>>contours;
	Mat grayscale;
	Mat img;
	frame = cvQueryFrame(capture);
	IplImage *nextgray = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
	int goodNum = 0;
	while(1)
	{
		frame = cvQueryFrame(capture);
		if (!frame)
		{
			cout<<"Frame Wrong"<<endl;
		}
		cvShowImage("frame",frame);
		if( cvWaitKey(10) == 27 ) break;
		////////////////////////////////////////////追踪///////////////////////////////////////////////////////////////
	    cvCvtColor(frame,nextgray,CV_RGB2GRAY);
		orb.detect(nextgray, nextkeypoints);                                                                                                         
	    orb.compute(nextgray, nextkeypoints, nextdescriptors);
		if (nextkeypoints.size()==0)
		{
			continue;
		}
	    matcher.match(descriptors,nextdescriptors,match);
		IplImage *thre = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
		Mark marker;
		for (size_t i = 0; i < match.size(); i++)
		{
			if (match[i].distance<20)
	    {
			good.push_back(match[i]);
		}
		}
		
		if (good.size()>/*20*/goodNum+10)
		{
	////////////////轮廓提取加矩形提取测试//////////////////////////////////////////////////////
	//good.clear();
	goodNum=good.size();
	cvAdaptiveThreshold(nextgray,thre,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY_INV,7,7);
	cvShowImage("threold",thre);
	if( cvWaitKey(10) == 27 ) break;                                                                                                                                 
	ORBTrack track;
	track.findNewContours(thre,contours,200);
	int flag = 0 ;
	track.findMarkerCandidates(contours, marker,flag);
	if (flag == -1)
	{
		continue;
	}
	cout<<"Lp1："<<marker.points[0]<<endl;
	cout<<"Lp2："<<marker.points[1]<<endl;
	cout<<"Lp3："<<marker.points[2]<<endl;
	cout<<"Lp4："<<marker.points[3]<<endl;

	////////////////////////////////////读取参数/////////////////////////////////////////////////////////////////////////////
	Mat cameraMatrix;//相机内参矩阵
	Mat distortionCoefficients;//相机畸变系数
	////////////////////////////////打开fs文件/////////////////////////////////////
	FileStorage fs("C:\\Users\\Li\\Desktop\\cameraMatrix.yml",FileStorage::READ);
	fs["cameraMatrix"]>>cameraMatrix;
	FileStorage fs1("C:\\Users\\Li\\Desktop\\distortionCoefficients.yml",FileStorage::READ);
	fs1["distortionCoefficients"]>>distortionCoefficients;
	
	/////////////////////////////获取相机的位姿////////////////////////////////////////////////////////////////////////////////
    //对像素点进行排序，与世界坐标的特征的值想对应，首先对y值进行判断
	Point p0,p1,p2,p3;
	compara.Comparable(marker.points[0],marker.points[1],marker.points[2],marker.points[3],p0,p1,p2,p3);
	//test
	/*if (p0.x==0&&p0.y==0&&p1.x==0&&p1.y==0&&p2.x==0&&p2.y==0&&p3.x==0&&p3.y==0)
	{
		continue;
	}*/
	cout<<"Lp1："<<p0<<endl;
	cout<<"Lp2："<<p1<<endl;
	cout<<"Lp3："<<p2<<endl;
	cout<<"Lp4："<<p3<<endl;
	Mat img(frame,0);
	line(img,p0,p1,Scalar(0,255,0),1);
	line(img,p1,p2,Scalar(0,255,0),1);
	line(img,p2,p3,Scalar(0,255,0),1);
	line(img,p0,p3,Scalar(0,255,0),1);
    
	 //对相机追踪到的点，传入世界坐标参数，所选取的特征点
	    Points2D.push_back(p0);
		Points2D.push_back(p1);
		Points2D.push_back(p2);
		Points2D.push_back(p3);
		
		Points3D.push_back(cv::Point3f(0,0,0));//此处圆心如何设置后续还需要继续讨论，此处的值需要根据实际的3D坐标进行输入
		Points3D.push_back(cv::Point3f(0,118,0));
		Points3D.push_back(cv::Point3f(115,118,0));
		Points3D.push_back(cv::Point3f(115,0,0));

		//初始化输出矩阵
		cv::Mat rvec = Mat::zeros(3,1,CV_64FC1);
		cv::Mat tvec = Mat::zeros(3,1,CV_64FC1);

		//应用solvepnp进行求解
		//所包含的参数列表
		//Points3D为特征点的世界坐标
		//Points2D为特征点的像素坐标
		//为相机的内参矩阵，大小为3x3
		//为相机的畸变参数，为1x5的矩阵
		//rvec为输出矩阵，输出解得的旋转向量
		//tvec为输出的平移向量
		//最后一个参数有三个可选项
		//1）CV_ITERATIVE，默认值，它通过迭代求出重投影误差最小的解作为问题的最优解
		//2）CV_P3P使用经典的Gao的PP问题求解算法
		//3）CV_EPNP
		solvePnP(Points3D,Points2D,cameraMatrix,distortionCoefficients,rvec,tvec,false,CV_P3P);


		//旋转向量旋转矩阵
		//提取旋转矩阵
		double rm[9];
		cv::Mat rotM(3,3,CV_64FC1,rm);
		Rodrigues(rvec,rotM);
		double r11 = rotM.ptr<double>(0)[0];
		double r12 = rotM.ptr<double>(0)[1];
        double r13 = rotM.ptr<double>(0)[2];
		double r21 = rotM.ptr<double>(1)[0];
		double r22 = rotM.ptr<double>(1)[1];
		double r23 = rotM.ptr<double>(1)[2];
		double r31 = rotM.ptr<double>(2)[0];
		double r32 = rotM.ptr<double>(2)[1];
		double r33 = rotM.ptr<double>(2)[2];

		//计算相机的旋转角
		//旋转顺序为z,y,x
		//有可能产生的奇异解，需要添加判断，此处需要修改，Y(3cm)与Z（3-4cm)轴误差较大
		//double epsilon = 1E-12;
		double thetaz = atan2(r21,r11)/CV_PI*180;          
		double thetay = atan2(-1*r31,sqrt(r32*r32 + r33*r33)) / CV_PI * 180;
		double thetax = atan2(r32, r33) / CV_PI * 180;
		//double thetaz = atan2(r21,r11);
		//double thetay = /*-asin(r32);*/atan2(-1*r31,sqrt(r11*r11+r21*r21));
	    //double thetax = atan2(r32,r33);
		///test
		//double epsion = 1E-12;
		//double thetaz,thetay,thetax;
		//thetay = /*-asin(r32);*/atan2(-1*r31,sqrt(r11*r11+r21*r21));
		//if (fabs(thetay)>(CV_PI/2.0-epsion))
		//{
		//	thetay=atan2(-1*r12,r22);
		//	thetax = 0;
		//}
		//else
		//{
		//	thetax = atan2(r32,r33);
		//	thetaz = atan2(r21,r11);
		//}

		cout<<"相机的三轴旋转角"<<-1*thetax<<","<<-1*thetay<<","<<-1*thetaz<<endl;


		double tx=tvec.ptr<double>(0)[0];
		double ty=tvec.ptr<double>(0)[1];
		double tz=tvec.ptr<double>(0)[2];

		//x,y,z为向量OcOw在相机坐标系下的值
		double x=tx,y=ty,z=tz;
		RotateZ(x,y,-1*thetaz,x,y);
		RotateY(x,z,-1*thetay,x,z);
		RotateX(y,z,-1*thetax,y,z);

		double Cx=x*-1;
		double Cy=y*-1;
		double Cz=z*-1;
		//输出相机在世界坐标系中的位置坐标
		cout<<"相机的世界坐标"<<Cx<<","<<Cy<<","<<Cz<<","<<endl;
		Points3D.clear();
		Points2D.clear();
		//重投影误差检测，通过对已知像素坐标以及世界坐标的点，通过给出该点的世界坐标，验证得出的像素坐标是否正确
		/*vector<cv::Point2f>projectPoints;
		Points3D.push_back(cv::Point3f(0,100,105));
		cv::projectPoints(Points3D,rvec,tvec,cameraMatrix,distortionCoefficients,projectPoints);*///最后一个值存在误差
	}
	cvShowImage("notmatch",frame);
	if( cvWaitKey(10) == 27 ) break;
	else
	{
	continue;
	}
 }
return 0;
}

