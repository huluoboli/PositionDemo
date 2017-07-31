#pragma once
#include<core\core.hpp>
#include<highgui\highgui.hpp>
#include<features2d\features2d.hpp>
#include <iostream>
#include <cv.h> 
#include "Mark.h"

using namespace cv;
using namespace std;



class ORBTrack
{
public:
	ORBTrack(void);
	~ORBTrack(void);
	int findMarkerCandidates(const vector<vector<Point>>&contours,Mark& marker/*vector<Mark>&delectedMarkers*/,int &flag);
	void findNewContours(const Mat& thresholdImg, std::vector<std::vector<Point>>& contours, int minContourPointsAllowed);
	
};

