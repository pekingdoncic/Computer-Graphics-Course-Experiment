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

//Cohen��Sutherland�㷨��Ҫ�ı��뺯��
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
//	if(p2.x!=p1.x)//��б�����б��m
//	{
//		m=(float)(p2.y-p1.y)/(p2.x-p1.x);
//		notVertical = 1;
//	}
//
//	//��ڶ����˵�ı���
//	code2=myEncode(p2);
//	while(!done)
//	{
//	//���һ���˵�ı���
//		code1=myEncode(p1);
//	
//		if(ACCEPT(code1,code2))	//�����˵㶼�ڴ����ڣ��߶���Ȼ�ɼ����ü�����		
//			done=true; 
//		else if(REJECT(code1,code2)) 
//		{//�������밴λ�벻Ϊ0�� �߶���Ȼ���ɼ����ü�����
//			done=true;
//			p1.x=0; p2.x=0;
//			p1.y=0; p2.y=0;
//		}
//		else 
//		{
//			if(INSIDE(code1)) //p1�ڴ����ڣ��򽻻�p1,p2,��֤p1Ϊ������ĵ�
//			{
//				swapPts(p1,p2);
//				swapCode(code1,code2);
//			}
//
//			if ( notVertical && code1 & LEFT_EDGE)//�����������࣬���������߽��㣬������p1
//			{
//				p1.y+=m *  (m_window[0].x-p1.x) ;
//				p1.x=m_window[0].x;
//			}
//			else if( notVertical &&  code1 & RIGHT_EDGE) //�����ұ�����࣬������ұ��߽��㣬������p1
//			{
//				p1.y+=m * (m_window[1].x-p1.x);
//				p1.x=m_window[1].x;
//			}
//			else if(code1 & BOTT_EDGE)//�����±�����࣬������±��߽��㣬������p1
//			{
//				if(p2.x!=p1.x) //���ڴ�ֱ��X���ֱ�ߣ�б��Ϊ����󣬽���x������p1�����
//					p1.x+=(float)(m_window[0].y-p1.y)/m;
//				p1.y=m_window[0].y;
//			}
//			else if(code1 & TOP_EDGE) //�����ϱ�����࣬������ϱ��߽��㣬������p1
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
	//	// ���ڻ�ֱ��δ���壬���ܽ��вü�
	//	return;
	//}

	//// ��ȡ�ü��ߵĶ˵�����
	//CPoint p1 = m_ClippingLine[0];
	//CPoint p2 = m_ClippingLine[1];

	//// ��ȡ�ߵĶ˵��x��y����
	//int x0 = p1.x;
	//int y0 = p1.y;
	//int x1 = p2.x;
	//int y1 = p2.y;

	//// ��ȡ���ڽǵ�����
	//int xmin = m_window[0].x;
	//int ymin = m_window[0].y;
	//int xmax = m_window[1].x;
	//int ymax = m_window[1].y;

	//// ��ʼ���ߵĲ�����ʾ�Ĳ���
	//float t0 = 0.0, t1 = 1.0;

	//// ����˵�����֮��Ĳ��죬��ȷ���ߵ�����
	//float dx = x1 - x0;
	//float dy = y1 - y0;

	//// ͨ���ĸ����ڱ�ִ��Cohen-Sutherland�߲ü�
	//for (int i = 0; i < 4; ++i)
	//{
	//	int p, q;

	//	// ���ݴ��ڵĵ�ǰ�߶������
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
	//	// ������Ƿ��봰�ڱ�ƽ�����ڴ�����
	//	if (p == 0 && q < 0)
	//	{
	//		// ֱ�߶�ƽ���ڴ��ڱ߽����ڴ�����
	//		return;
	//	}

	//	// �����봰�ڱ߽���Ĳ��� 'r'
	//	float r = static_cast<float>(q) / p;

	//	// �����ߵķ���ͽ�����²���ֵ
	//	if (p < 0)
	//	{
	//		t0 = max(t0, r);
	//	}
	//	else if (p > 0)
	//	{
	//		t1 = min(t1, r);
	//	}
	//	// ������Ƿ���ȫ�ڴ�����
	//	if (t0 > t1)
	//	{
	//		// ֱ�߶��ڴ�����
	//		return;
	//	}
	//}

	//// ʹ�ò���ֵ����ü��������
	//int x0_clipped = static_cast<int>(x0 + t0 * dx);
	//int y0_clipped = static_cast<int>(y0 + t0 * dy);
	//int x1_clipped = static_cast<int>(x0 + t1 * dx);
	//int y1_clipped = static_cast<int>(y0 + t1 * dy);

	//// �洢�ü�����ߵ�����
	//m_ClippedLine[0] = CPoint(x0_clipped, y0_clipped);
	//m_ClippedLine[1] = CPoint(x1_clipped, y1_clipped);

	//// ����ָʾ�ü�����ɵı�־
	//myClipDone = true;


	if (!myWindowDefined || !myLineDefined)
	{
		// ���ڻ�ֱ��δ���壬�޷����вü�
		return;
	}

	// ��ʼ������
	double t0 = 0.0, t1 = 1.0; // ���������߶ζ˵�
	int dx = m_ClippingLine[1].x - m_ClippingLine[0].x;
	int dy = m_ClippingLine[1].y - m_ClippingLine[0].y;

	// Liang-Barsky��������
	// ѭ���������ڵ��ĸ��߽�
	for (int i = 0; i < 4; i++)
	{
		int p, q;
		// ����p��q��ֵ�����ڼ������r
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

		// ����r������ʾ�߶��뵱ǰ�߽�Ľ����������߶��ϵ����λ��
		double r = static_cast<double>(q) / p;

		// ��������������߶��뵱ǰ�߽�ƽ�����ڱ߽�֮��
		if (p == 0 && q < 0)
		{
			// �߶�ƽ���ڲü��߽����ڲü��߽�֮�⣬���ü���
			return;
		}

		// ���²���t0��t1
		if (p < 0)
		{
			if (r > t1)
			{
				// �߶��ڴ���֮��
				return;
			}
			else if (r > t0)
			{
				// ����t0
				t0 = r;
			}
		}
		else if (p > 0)
		{
			if (r < t0)
			{
				// �߶��ڴ���֮��
				return;
			}
			else if (r < t1)
			{
				// ����t1
				t1 = r;
			}
		}
	}

	// ����ü�����߶ζ˵�
	int x0 = m_ClippingLine[0].x + static_cast<int>(t0 * dx);
	int y0 = m_ClippingLine[0].y + static_cast<int>(t0 * dy);
	int x1 = m_ClippingLine[0].x + static_cast<int>(t1 * dx);
	int y1 = m_ClippingLine[0].y + static_cast<int>(t1 * dy);

	// ���²ü�����߶�
	m_ClippedLine[0] = CPoint(x0, y0);
	m_ClippedLine[1] = CPoint(x1, y1);
	myClipDone = true;
}

