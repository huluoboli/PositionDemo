#pragma once
#include<stdlib.h>
#include <opencv2\opencv.hpp>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv.hpp>
#include<cv.h>

using namespace cv;
class Mark
{
public:
	Mark(void);
	~Mark(void);
	  friend bool operator<(const Mark&M1,const Mark&M2);
  friend std::ostream & operator<<(std::ostream &str,const Mark&M);
  //void drawContour(cv::Mat& image, cv::Scalar color = CV_RGB(0,250,0)) const;

  static cv::Mat rotate(cv::Mat  in);
  static int hammDistMarker(cv::Mat bits);
  static int mat2id(const cv::Mat &bits);
  static int getMarkerId(cv::Mat &in,int &nRotations);
    //id of  the marker
  int id;
  
  //marker transformation wrt to the camera
  //Transformation transformation;
  
  std::vector<cv::Point2f> points;
  void drawContour(cv::Mat& image, cv::Scalar color = CV_RGB(0,250,0)) const;
};

