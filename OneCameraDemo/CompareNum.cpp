#include "stdafx.h"
#include "CompareNum.h"
#include "core\core.hpp"
#include "core\core_c.h"
#include <math.h>
#include "opencv.hpp"


using namespace cv;


CompareNum::CompareNum(void)
{
}


CompareNum::~CompareNum(void)
{
}

//对传入的四个特征点进行比较，按照逆时针进行排序
int  CompareNum::Comparable(Point p1,Point p2,Point p3,Point p4,Point &pb1,Point &pb2,Point &pb3,Point &pb4)
{
     	//////////////////////////////////////////ymin1////////////////////////////////////////////////////////////////
	if (min(min(min(p1.y,p2.y),p3.y),p4.y) == p1.y)//1
	{
		if (min(min(p2.y,p3.y),p4.y)==p2.y)//2
		{
			//对0，1的x值进行判断
			if (p1.x<=p2.x)
			{
				if (p3.x>=p4.x)//1234
				{
					pb1 = p1;
					pb2 = p2;
					pb3 = p3;
					pb4 = p4;
					return 0;
				}
				else//1243
				{
					pb1 = p1;
					pb2 = p2;
					pb3 = p4;
					pb4 = p3;
					return 0;
				}
			}
			else
			{
				if (p3.x>=p4.x)//2134
				{
					pb1 = p2;
					pb2 = p1;
					pb3 = p3;
					pb4 = p4;
					return 0;
				}
				else//2143
				{
					pb1 = p2;
					pb2 = p1;
					pb3 = p4;
					pb4 = p3;
					return 0;
				}
			}
		}
		
	///////////////////////////////////////////////13//////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (min(min(p2.y,p3.y),p4.y)==p3.y)//3
		{
				
			//对0，1的x值进行判断
			if (p1.x<=p3.x)
			{
				if (p2.x>=p4.x)//1324
				{
					pb1 = p1;
					pb2 = p3;
					pb3 = p2;
					pb4 = p4;
					return 0;
				}
				else//1342
				{
					pb1 = p1;
					pb2 = p3;
					pb3 = p4;
					pb4 = p2;
					return 0;
				}
			}
			else
			{
				if (p2.x>=p4.x)//3124
				{
					pb1 = p3;
					pb2 = p1;
					pb3 = p2;
					pb4 = p4;
					return 0;
				}
				else//3142
				{
					pb1 = p3;
					pb2 = p1;
					pb3 = p4;
					pb4 = p2;
					return 0;
				}
			}
		}
	///////////////////////////////////////////////14//////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (min(min(p2.y,p3.y),p4.y)==p4.y)//4
		{
			//对0，2的x值进行判断
		if (p1.x<=p4.x)
			{
				if (p2.x>=p3.x)//1423
				{
					pb1 = p1;
					pb2 = p4;
					pb3 = p2;
					pb4 = p3;
					return 0;
				}
				else//1432
				{
					pb1 = p1;
					pb2 = p4;
					pb3 = p3;
					pb4 = p2;
					return 0;
				}
			}
			else
			{
				if (p2.x>=p3.x)//4123
				{
					pb1 = p4;
					pb2 = p1;
					pb3 = p2;
					pb4 = p3;
					return 0;
				}
				else//4132
				{
					pb1 = p4;
					pb2 = p1;
					pb3 = p3;
					pb4 = p2;
					return 0;
				}
			}
		}
		}
		///////////////////////////////////////////ymin2////////////////////////////////////////////////////////////////
		else if (min(min(min(p1.y,p2.y),p3.y),p4.y)==p2.y)
		{
			if ((min(min(p1.y,p3.y),p4.y)==p1.y))
			{
			//对0，1的x值进行判断
			if (p2.x<=p1.x)//2134
			{
				if (p3.x>=p4.x)
				{
					pb1 = p2;
					pb2 = p1;
					pb3 = p3;
					pb4 = p4;
					return 0;
				}
				else//2143
				{
					pb1 = p2;
					pb2 = p1;
					pb3 = p4;
					pb4 = p3;
					return 0;
				}
			}
			else
			{
				if (p3.x>=p4.x)
				{
					pb1 = p1;
					pb2 = p2;
					pb3 = p3;
					pb4 = p4;
					return 0;
				}
				else//2143
				{
					pb1 = p1;
					pb2 = p2;
					pb3 = p4;
					pb4 = p3;
					return 0;
				}
			}
			}
			else if ((min(min(p1.y,p3.y),p4.y)==p3.y))
			{
			//对0，1的x值进行判断
			if (p2.x<=p3.x)//2314
			{
				if (p1.x>=p4.x)
				{
					pb1 = p2;
					pb2 = p3;
					pb3 = p1;
					pb4 = p4;
					return 0;
				}
				else//2341
				{
					pb1 = p2;
					pb2 = p3;
					pb3 = p4;
					pb4 = p1;
					return 0;
				}
			}
			else
			{
				if (p1.x>=p4.x)
				{
					pb1 = p3;
					pb2 = p2;
					pb3 = p1;
					pb4 = p4;
					return 0;
				}
				else
				{
					pb1 = p3;
					pb2 = p2;
					pb3 = p4;
					pb4 = p1;
					return 0;
				}
			}
			}
			else if((min(min(p1.y,p3.y),p4.y)==p4.y))
			{
				if (p2.x<=p4.y)//2413
				{
					if (p1.x>=p3.x)
				{
					pb1 = p2;
					pb2 = p4;
					pb3 = p1;
					pb4 = p3;
					return 0;
				}
				
				else//2431
				{
					pb1 = p2;
					pb2 = p4;
					pb3 = p3;
					pb4 = p1;
					return 0;
				}
			}
				else
				{
				if (p1.x>=p3.x)
				{
					pb1 = p4;
					pb2 = p2;
					pb3 = p1;
					pb4 = p3;
					return 0;
				}
				
				else//2431
				{
					pb1 = p4;
					pb2 = p2;
					pb3 = p3;
					pb4 = p1;
					return 0;
				}
				}
			}
		}

		/////////////////////////////////////////ymin3///////////////////////////////////////////////////////////////
		else if (min(min(min(p1.y,p2.y),p3.y),p4.y)==p3.y)
		{
			if ((min(min(p1.y,p3.y),p4.y)==p1.y))
			{
			//对0，1的x值进行判断
			if (p1.x<=p3.x)//2314
			{
				if (p2.x>=p4.x)
				{
					pb1 = p1;
					pb2 = p3;
					pb3 = p2;
					pb4 = p4;
					return 0;
				}
				else//2341
				{
					pb1 = p1;
					pb2 = p3;
					pb3 = p4;
					pb4 = p2;
					return 0;
				}
			}
			else
			{
				if (p2.x>=p4.x)
				{
					pb1 = p3;
					pb2 = p1;
					pb3 = p2;
					pb4 = p4;
					return 0;
				}
				else//2341
				{
					pb1 = p3;
					pb2 = p1;
					pb3 = p4;
					pb4 = p2;
					return 0;
				}
			}
			}
			else if ((min(min(p1.y,p2.y),p4.y)==p2.y))//3214
			{
			if (p3.x<=p2.x)
			{
				if (p1.x>=p4.x)
				{
					pb1 = p3;
					pb2 = p2;
					pb3 = p1;
					pb4 = p4;
					return 0;
				}
				else//3241
				{
					pb1 = p3;
					pb2 = p2;
					pb3 = p4;
					pb4 = p1;
					return 0;
				}
			}
			else
			{
				if (p1.x>=p4.x)
				{
					pb1 = p2;
					pb2 = p3;
					pb3 = p1;
					pb4 = p4;
					return 0;
				}
				else//3241
				{
					pb1 = p2;
					pb2 = p3;
					pb3 = p4;
					pb4 = p1;
					return 0;
				}
			}
			}
			else if((min(min(p1.y,p3.y),p4.y)==p4.y))
			{
				if (p3.x<=p4.y)//3412
				{
					if (p1.x>=p2.x)
				{
					pb1 = p3;
					pb2 = p4;
					pb3 = p1;
					pb4 = p2;
					return 0;
				}
				
				else//3421
				{
					pb1 = p3;
					pb2 = p4;
					pb3 = p2;
					pb4 = p1;
					return 0;
				}
			}
				else
				{
				if (p1.x>=p2.x)
				{
					pb1 = p4;
					pb2 = p3;
					pb3 = p1;
					pb4 = p2;
					return 0;
				}
				
				else//3421
				{
					pb1 = p4;
					pb2 = p3;
					pb3 = p2;
					pb4 = p1;
					return 0;
				}
				}
			}
		}

		/////////////////////////////////////////ymin3///////////////////////////////////////////////////////////////
		else if (min(min(min(p1.y,p2.y),p3.y),p4.y)==p4.y)
		{
			if ((min(min(p1.y,p3.y),p4.y)==p1.y))
			{
			//对0，1的x值进行判断
			if (p4.x<=p1.x)//2314
			{
				if (p2.x>=p3.x)
				{
					pb1 = p4;
					pb2 = p1;
					pb3 = p2;
					pb4 = p3;
					return 0;
				}
				else//2341
				{
					pb1 = p4;
					pb2 = p1;
					pb3 = p3;
					pb4 = p2;
					return 0;
				}
			}
			else 
			{
				if (p2.x>=p3.x)
				{
					pb1 = p1;
					pb2 = p4;
					pb3 = p2;
					pb4 = p3;
					return 0;
				}
				else//2341
				{
					pb1 = p1;
					pb2 = p4;
					pb3 = p3;
					pb4 = p2;
					return 0;
				}
			}
			}
			else if ((min(min(p1.y,p2.y),p4.y)==p2.y))//4231
			{
			if (p4.x<=p2.x)
			{
				if (p3.x>=p1.x)
				{
					pb1 = p4;
					pb2 = p2;
					pb3 = p3;
					pb4 = p1;
					return 0;
				}
				else//4213
				{
					pb1 = p4;
					pb2 = p2;
					pb3 = p1;
					pb4 = p3;
					return 0;
				}
			}
			else
			{
				if (p3.x>=p1.x)
				{
					pb1 = p2;
					pb2 = p4;
					pb3 = p3;
					pb4 = p1;
					return 0;
				}
				else//4213
				{
					pb1 = p2;
					pb2 = p4;
					pb3 = p1;
					pb4 = p3;
					return 0;
				}
			}
			}
			else if((min(min(p1.y,p3.y),p4.y)==p3.y))
			{
				if (p4.x<=p3.y)//4312
				{
					if (p1.x>=p2.x)
				{
					pb1 = p4;
					pb2 = p3;
					pb3 = p1;
					pb4 = p2;
					return 0;
				}
				
				else//4321
				{
					pb1 = p4;
					pb2 = p3;
					pb3 = p2;
					pb4 = p1;
					return 0;
				}
			}
				else
				{
					if (p1.x>=p2.x)
				{
					pb1 = p3;
					pb2 = p4;
					pb3 = p1;
					pb4 = p2;
					return 0;
				}
				
				else//4321
				{
					pb1 = p3;
					pb2 = p4;
					pb3 = p2;
					pb4 = p1;
					return 0;
				}
				}
			}
		}
		}