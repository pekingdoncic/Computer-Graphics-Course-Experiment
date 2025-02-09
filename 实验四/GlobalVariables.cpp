#define _MY_GLOBAL_VARIABLE_DEFINITION_FILE_

#include "stdafx.h"
#include "GlobalVariables.h"

// --------------------- Global variables ---------------------

int G_iDrawState;
int G_iMouseState;
COLORREF G_cLineColor;

void MyClip::setLine(CPoint point1, CPoint point2)
{
		m_ClippingLine[0] = point1;
		m_ClippingLine[1] = point2;
		myLineDefined = true; 
}
void MyClip::clearWindow()
{
	myWindowDefined = false; 	
}
void MyClip::clearLine()
{
	myLineDefined = false;
}
void MyClip::setWindow(CPoint point1, CPoint point2)
{
	if(point1.x < point2.x)
	{
		m_window[0].x = point1.x;
		m_window[1].x = point2.x;
	}
	else	
	{
		m_window[0].x = point2.x;
		m_window[1].x = point1.x;
	}

	if(point1.y < point2.y)
	{
		m_window[0].y = point1.y;
		m_window[1].y = point2.y;
	}
	else		
	{
		m_window[0].y = point2.y;
		m_window[1].y = point1.y;
	}
	myWindowDefined = true; 
}

//Cohen—Sutherland算法需要的编码函数
unsigned char MyClip::myEncode(CPoint thePoint)
{
	unsigned char code=0x00;
	if(thePoint.x < m_window[0].x)
		code=code|LEFT_EDGE;
	if(thePoint.x>m_window[1].x)
		code=code|RIGHT_EDGE;
	if(thePoint.y<m_window[0].y)
		code=code|BOTT_EDGE;
	if(thePoint.y>m_window[1].y)
		code=code|TOP_EDGE;
	return code;
}



void MyClip::swapCode(unsigned char &c1,unsigned char &c2)
{
	unsigned char tmp;
	tmp=c1;
	c1=c2;
	c2=tmp;
}

void MyClip::swapPts(CPoint &p1,CPoint &p2)
{
	CPoint tmp;
	tmp=p1;
	p1=p2;
	p2=tmp;
}


void MyClip::doMyClipping()
{
//	if(!myWindowDefined && !myLineDefined)  return;
//// cohen_sutherland clipping algorithm	
//
//	unsigned char code1,code2;
//	CPoint p1, p2;
//	p1 = m_ClippingLine[0];
//	p2 = m_ClippingLine[1];
//
//	bool done=false;
//	float m;
//	bool notVertical=0;
//	if(p2.x!=p1.x)//有斜率算出斜率m
//	{
//		m=(float)(p2.y-p1.y)/(p2.x-p1.x);
//		notVertical = 1;
//	}
//
//	//求第二个端点的编码
//	code2=myEncode(p2);
//	while(!done)
//	{
//	//求第一个端点的编码
//		code1=myEncode(p1);
//	
//		if(ACCEPT(code1,code2))	//两个端点都在窗口内，线段显然可见，裁剪结束		
//			done=true; 
//		else if(REJECT(code1,code2)) 
//		{//两个编码按位与不为0， 线段显然不可见，裁剪结束
//			done=true;
//			p1.x=0; p2.x=0;
//			p1.y=0; p2.y=0;
//		}
//		else 
//		{
//			if(INSIDE(code1)) //p1在窗口内，则交换p1,p2,保证p1为窗口外的点
//			{
//				swapPts(p1,p2);
//				swapCode(code1,code2);
//			}
//
//			if ( notVertical && code1 & LEFT_EDGE)//点在左边线外侧，求出与左边线交点，并赋予p1
//			{
//				p1.y+=m *  (m_window[0].x-p1.x) ;
//				p1.x=m_window[0].x;
//			}
//			else if( notVertical &&  code1 & RIGHT_EDGE) //点在右边线外侧，求出与右边线交点，并赋予p1
//			{
//				p1.y+=m * (m_window[1].x-p1.x);
//				p1.x=m_window[1].x;
//			}
//			else if(code1 & BOTT_EDGE)//点在下边线外侧，求出与下边线交点，并赋予p1
//			{
//				if(p2.x!=p1.x) //对于垂直于X轴的直线，斜率为无穷大，交点x坐标与p1点相等
//					p1.x+=(float)(m_window[0].y-p1.y)/m;
//				p1.y=m_window[0].y;
//			}
//			else if(code1 & TOP_EDGE) //点在上边线外侧，求出与上边线交点，并赋予p1
//			{ 
//				if(p2.x!=p1.x)
//					p1.x+=(float)(m_window[1].y-p1.y)/m;
//				p1.y=m_window[1].y;
//			}
//		}
//	}
//	m_ClippedLine[0].x = p1.x;
//	m_ClippedLine[0].y = p1.y;
//	m_ClippedLine[1].x = p2.x;
//	m_ClippedLine[1].y = p2.y;
//
// 	myClipDone = true;

	////liang-barsky clipping alogrithm
	//if (!myWindowDefined || !myLineDefined)
	//{
	//	// 窗口或直线未定义，不能进行裁剪
	//	return;
	//}

	//// 提取裁剪线的端点坐标
	//CPoint p1 = m_ClippingLine[0];
	//CPoint p2 = m_ClippingLine[1];

	//// 提取线的端点的x和y坐标
	//int x0 = p1.x;
	//int y0 = p1.y;
	//int x1 = p2.x;
	//int y1 = p2.y;

	//// 提取窗口角的坐标
	//int xmin = m_window[0].x;
	//int ymin = m_window[0].y;
	//int xmax = m_window[1].x;
	//int ymax = m_window[1].y;

	//// 初始化线的参数表示的参数
	//float t0 = 0.0, t1 = 1.0;

	//// 计算端点坐标之间的差异，以确定线的增量
	//float dx = x1 - x0;
	//float dy = y1 - y0;

	//// 通过四个窗口边执行Cohen-Sutherland线裁剪
	//for (int i = 0; i < 4; ++i)
	//{
	//	int p, q;

	//	// 根据窗口的当前边定义参数
	//	switch (i)
	//	{
	//	case 0:  // Top
	//		p = -dy;
	//		q = -(ymin - y0);
	//		break;
	//	case 1:  // Bottom
	//		p = dy;
	//		q = ymax - y0;
	//		break;
	//	case 2:  // Right
	//		p = dx;
	//		q = xmax - x0;
	//		break;
	//	case 3:  // Left
	//		p = -dx;
	//		q = -(xmin - x0);
	//		break;
	//	}
	//	// 检查线是否与窗口边平行且在窗口外
	//	if (p == 0 && q < 0)
	//	{
	//		// 直线段平行于窗口边界且在窗口外
	//		return;
	//	}

	//	// 计算与窗口边交点的参数 'r'
	//	float r = static_cast<float>(q) / p;

	//	// 根据线的方向和交点更新参数值
	//	if (p < 0)
	//	{
	//		t0 = max(t0, r);
	//	}
	//	else if (p > 0)
	//	{
	//		t1 = min(t1, r);
	//	}
	//	// 检查线是否完全在窗口外
	//	if (t0 > t1)
	//	{
	//		// 直线段在窗口外
	//		return;
	//	}
	//}

	//// 使用参数值计算裁剪后的坐标
	//int x0_clipped = static_cast<int>(x0 + t0 * dx);
	//int y0_clipped = static_cast<int>(y0 + t0 * dy);
	//int x1_clipped = static_cast<int>(x0 + t1 * dx);
	//int y1_clipped = static_cast<int>(y0 + t1 * dy);

	//// 存储裁剪后的线的坐标
	//m_ClippedLine[0] = CPoint(x0_clipped, y0_clipped);
	//m_ClippedLine[1] = CPoint(x1_clipped, y1_clipped);

	//// 设置指示裁剪已完成的标志
	//myClipDone = true;


	if (!myWindowDefined || !myLineDefined)
	{
		// 窗口或直线未定义，无法进行裁剪
		return;
	}

	// 初始化参数
	double t0 = 0.0, t1 = 1.0; // 参数化的线段端点
	int dx = m_ClippingLine[1].x - m_ClippingLine[0].x;
	int dy = m_ClippingLine[1].y - m_ClippingLine[0].y;

	// Liang-Barsky参数计算
	// 循环遍历窗口的四个边界
	for (int i = 0; i < 4; i++)
	{
		int p, q;
		// 计算p和q的值，用于计算参数r
		if (i == 0)
		{
			p = -dx;
			q = -(m_window[0].x - m_ClippingLine[0].x);
		}
		else if (i == 1)
		{
			p = dx;
			q = m_window[1].x - m_ClippingLine[0].x;
		}
		else if (i == 2)
		{
			p = -dy;
			q = -(m_window[0].y - m_ClippingLine[0].y);
		}
		else
		{
			p = dy;
			q = m_window[1].y - m_ClippingLine[0].y;
		}

		// 计算r，它表示线段与当前边界的交点在整个线段上的相对位置
		double r = static_cast<double>(q) / p;

		// 处理特殊情况：线段与当前边界平行且在边界之外
		if (p == 0 && q < 0)
		{
			// 线段平行于裁剪边界且在裁剪边界之外，被裁剪掉
			return;
		}

		// 更新参数t0和t1
		if (p < 0)
		{
			if (r > t1)
			{
				// 线段在窗口之外
				return;
			}
			else if (r > t0)
			{
				// 更新t0
				t0 = r;
			}
		}
		else if (p > 0)
		{
			if (r < t0)
			{
				// 线段在窗口之外
				return;
			}
			else if (r < t1)
			{
				// 更新t1
				t1 = r;
			}
		}
	}

	// 计算裁剪后的线段端点
	int x0 = m_ClippingLine[0].x + static_cast<int>(t0 * dx);
	int y0 = m_ClippingLine[0].y + static_cast<int>(t0 * dy);
	int x1 = m_ClippingLine[0].x + static_cast<int>(t1 * dx);
	int y1 = m_ClippingLine[0].y + static_cast<int>(t1 * dy);

	// 更新裁剪后的线段
	m_ClippedLine[0] = CPoint(x0, y0);
	m_ClippedLine[1] = CPoint(x1, y1);
	myClipDone = true;
}

