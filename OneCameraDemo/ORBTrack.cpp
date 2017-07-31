#include "stdafx.h"
#include "ORBTrack.h"
#include<core\core.hpp>
#include<highgui\highgui.hpp>
#include<features2d\features2d.hpp>
#include <iostream>
#include <cv.h> 
#include "Mark.h"

using namespace cv;
using namespace std;

ORBTrack::ORBTrack(void)
{
}


ORBTrack::~ORBTrack(void)
{
}


int ORBTrack::findMarkerCandidates(const vector<vector<Point>>&contours,Mark& marker/*vector<Mark>&delectedMarkers*/,int &flag)
{
	vector<Point>  approxCurve;	
	vector<Mark> possibleMarkers;
	float m_minContourLengthAllowed = 200.0;

	for (size_t i=0; i<contours.size(); i++)
	{
		// Approximate to a poligon, 通过点集近似多边形，第三个参数为epsilon代表近似程度，即原始轮廓及近似多边形之间的距离，第四个参数表示多边形是闭合的
		approxPolyDP(contours[i], approxCurve, double(contours[i].size())*0.05 , true);

		if (approxCurve.size() != 4)
			continue;

		// 检验轮廓是否为凸多边形
		if (!isContourConvex(approxCurve))
			continue;

		//确保连续点之间的距离是足够大的
		float minDist = 1e10;
		for (int i=0;i<4;i++)
		{
			//求当前四边形各顶点之间最短距离
			Point vec = approxCurve[i] - approxCurve[(i+1)%4];            
			float squaredDistance = vec.dot(vec);
			minDist = min(minDist,squaredDistance);
		}

		// 当四边形大小合适，则将该四边形maker放入possibleMarkers容器内
		if (minDist > m_minContourLengthAllowed)
		{
			Mark m;
			for(int i=0;i<4;i++)
			{
				m.points.push_back( Point2f(approxCurve[i].x,approxCurve[i].y) );
			}
			possibleMarkers.push_back(m);
		}

	}
	for (size_t i=0; i<possibleMarkers.size(); i++)
	{
		marker = possibleMarkers[i];

		Point v1 = marker.points[1] - marker.points[0];
		Point v2 = marker.points[2] - marker.points[0];

		//行列式的几何意义是什么呢？有两个解释：一个解释是行列式就是行列式中的行或列向量所构成的超平行多面体的有向面积或有向体积；另一个解释是矩阵A的行列式detA就是线性变换A下的图形面积或体积的伸缩因子。
		double o = (v1.x * v2.y) - (v1.y * v2.x);
   		if (o < 0.0)         
		{
			swap(marker.points[1],marker.points[3]);
		}
	}
	  flag = 0;
		if (possibleMarkers.size()==NULL)
	{
		flag =-1;
		return 0;
	}
}


void ORBTrack::findNewContours(const Mat& thresholdImg, std::vector<std::vector<Point>>& contours, int minContourPointsAllowed)
{
	std::vector< std::vector<Point> > allContours;
	findContours(thresholdImg, allContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	contours.clear();
	for (size_t i=0; i<allContours.size(); i++)
	{
		int contourSize = allContours[i].size();
		if (contourSize > minContourPointsAllowed)
		{
			contours.push_back(allContours[i]);
		}
	}
}



