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
		// Approximate to a poligon, ͨ���㼯���ƶ���Σ�����������Ϊepsilon������Ƴ̶ȣ���ԭʼ���������ƶ����֮��ľ��룬���ĸ�������ʾ������Ǳպϵ�
		approxPolyDP(contours[i], approxCurve, double(contours[i].size())*0.05 , true);

		if (approxCurve.size() != 4)
			continue;

		// ���������Ƿ�Ϊ͹�����
		if (!isContourConvex(approxCurve))
			continue;

		//ȷ��������֮��ľ������㹻���
		float minDist = 1e10;
		for (int i=0;i<4;i++)
		{
			//��ǰ�ı��θ�����֮����̾���
			Point vec = approxCurve[i] - approxCurve[(i+1)%4];            
			float squaredDistance = vec.dot(vec);
			minDist = min(minDist,squaredDistance);
		}

		// ���ı��δ�С���ʣ��򽫸��ı���maker����possibleMarkers������
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

		//����ʽ�ļ���������ʲô�أ����������ͣ�һ������������ʽ��������ʽ�е��л������������ɵĳ�ƽ�ж��������������������������һ�������Ǿ���A������ʽdetA�������Ա任A�µ�ͼ�������������������ӡ�
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



