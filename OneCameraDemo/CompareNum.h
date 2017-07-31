#pragma once
#include "core\core.hpp"
#include "core\core_c.h"
#include <math.h>
#include "opencv.hpp"


using namespace cv;
class CompareNum
{
public:
	CompareNum(void);
	~CompareNum(void);
	int Comparable(Point p1,Point p2,Point p3,Point p4,Point &pb1,Point &pb2,Point &pb3,Point &pb4);

};

