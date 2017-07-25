#include "StdAfx.h"
#include "Graph.h"
#include <math.h>

#include "../DXF Files/Test_CreationClass.h"

#include "../DXF Files/BaseGraph.h"

CArray<CBaseGraph*> g_ptrGraphArray;
double g_fRate = 1.0;//比例

CGraph::CGraph(void)
{
	m_cxStaticWidth = 0;
	m_cyStaticHeight = 0;
	m_GraphPointMinX = INT_MAX;
	m_GraphPointMinY = INT_MAX;
	m_GraphPointMaxX = INT_MIN;
	m_GraphPointMaxY = INT_MIN;

	InitializeCriticalSection(&m_cs);

#ifdef USE_STL_DEQUE
	m_GraphDataInfoStructDeque.clear();
	m_GraphDataInfoStructDeque.resize(CNC_GRAPH_MAX_POINT_NUM);
#else
	m_pGraphDataInfoStruct = new GraphDataInfoStruct[CNC_GRAPH_MAX_POINT_NUM];
	memset(m_pGraphDataInfoStruct, 0, sizeof(GraphDataInfoStruct) * CNC_GRAPH_MAX_POINT_NUM);
#endif

	m_iGraphPointCurrentNum = 0;
	m_nGraphCompareError = 0.0;
	m_uChGraphPaintFlag = 0;
	m_uChGraphPaintMode = 0;

	m_hGraphWnd = NULL;
	m_hGraphDC = NULL;

	m_hMemDC = NULL;
	m_hBitmap = NULL;
	m_hOldBitmap = NULL;
	m_hBlackBrush = NULL;
	m_hOldBlackBrush = NULL;
	m_hDrawPointPen = NULL;
	m_hOldDrawPointPen = NULL;
}

CGraph::~CGraph(void)
{
	DeleteCriticalSection(&m_cs);
#ifdef USE_STL_DEQUE
	m_GraphDataInfoStructDeque.clear();
#else
	if (m_pGraphDataInfoStruct != NULL)
	{
		delete m_pGraphDataInfoStruct;
		m_pGraphDataInfoStruct = NULL;
	}
#endif

	if(m_hGraphDC != NULL)
	{
		::ReleaseDC(m_hGraphWnd, m_hGraphDC);
		m_hGraphDC = NULL;
	}

	if(m_hGraphWnd != NULL)
	{
		m_hGraphWnd = NULL;
	}

	if(m_hBlackBrush != NULL)
	{
		::DeleteObject(m_hBlackBrush);
		m_hBlackBrush = NULL;
	}

	if(m_hOldBlackBrush != NULL)
	{
		::DeleteObject(m_hOldBlackBrush);
		m_hOldBlackBrush = NULL;
	}

	if(m_hDrawPointPen != NULL)
	{
		::DeleteObject(m_hDrawPointPen);
		m_hDrawPointPen = NULL;
	}

	if(m_hOldDrawPointPen != NULL)
	{
		::DeleteObject(m_hOldDrawPointPen);
		m_hOldDrawPointPen = NULL;
	}

	if(m_hMemDC != NULL)
	{
		::DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
	}

	if(m_hBitmap != NULL)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	if(m_hOldBitmap != NULL)
	{
		::DeleteObject(m_hOldBitmap);
		m_hOldBitmap = NULL;
	}

	if(m_hFramePen != NULL)
	{
		::DeleteObject(m_hFramePen);
		m_hFramePen = NULL;
	}

	if(m_hOldFramePen != NULL)
	{
		::DeleteObject(m_hOldFramePen);
		m_hOldFramePen = NULL;
	}

	if(m_hCurrentToolPointPen != NULL)
	{
		::DeleteObject(m_hCurrentToolPointPen);
		m_hCurrentToolPointPen = NULL;
	}

	if(m_hOldCurrentToolPointPen != NULL)
	{
		::DeleteObject(m_hOldCurrentToolPointPen);
		m_hOldCurrentToolPointPen = NULL;
	}

	if(m_hCurrentToolPointBrush != NULL)
	{
		::DeleteObject(m_hCurrentToolPointBrush);
		m_hCurrentToolPointBrush = NULL;
	}

	if(m_hOldCurrentToolPointBrush != NULL)
	{
		::DeleteObject(m_hOldCurrentToolPointBrush);
		m_hOldCurrentToolPointBrush = NULL;
	}
}

void CGraph::GraphMain(void)
{
	//绘图开启标识
	if (m_uChGraphPaintFlag)
	{
		//绘制图形
		if(m_hGraphWnd == NULL)
			return;

		PrepareDraw();

		DrawPoint();

		::BitBlt(m_hGraphDC, 0, 0, m_cxStaticWidth, m_cyStaticHeight, m_hMemDC, 0, 0, SRCCOPY);

		//::StretchBlt(m_hGraphDC, 0, 0, m_cxStaticWidth, m_cyStaticHeight, m_hMemDC, 
		//	m_GraphPointMinX, m_GraphPointMinY, m_GraphPointMaxX, m_GraphPointMaxY, SRCCOPY);

		OverDraw();

	}
}

void CGraph::GraphInit(void)
{
#ifdef USE_STL_DEQUE
	m_GraphDataInfoStructDeque.resize(0);
	m_GraphDataInfoStructDeque.resize(CNC_GRAPH_MAX_POINT_NUM);
#else
	memset(m_pGraphDataInfoStruct, 0, sizeof(GraphDataInfoStruct) * CNC_GRAPH_MAX_POINT_NUM);
#endif
	m_iGraphPointCurrentNum = 0;
	m_GraphPointMinX = INT_MAX;
	m_GraphPointMinY = INT_MAX;
	m_GraphPointMaxX = INT_MIN;
	m_GraphPointMaxY = INT_MIN;
	g_fRate = 1.0;

	if(g_ptrGraphArray.GetSize() != 0)
		g_ptrGraphArray.RemoveAll();

	if(m_hGraphWnd != NULL)
	{
		CRect rt;
		GetWindowRect(m_hGraphWnd, &rt);
		m_cxStaticWidth = rt.Width();
		m_cyStaticHeight = rt.Height();

		m_cxExcludeFrameStaticWidth = m_cxStaticWidth - FRAME_EDAGE_OFFSET * 2;
		m_cyExcludeFrameStaticHeight = m_cyStaticHeight - FRAME_EDAGE_OFFSET * 2;

		/*m_pNullBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		m_Pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));*/

		m_hBlackBrush = ::CreateSolidBrush(RGB(0, 0, 0));
		m_hDrawPointPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

		m_hMemDC = ::CreateCompatibleDC(m_hGraphDC);
		m_hBitmap = ::CreateCompatibleBitmap(m_hGraphDC, m_cxStaticWidth, m_cyStaticHeight);

		m_hFramePen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

		m_hCurrentToolPointPen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		m_hCurrentToolPointBrush = ::CreateSolidBrush(RGB(255, 0, 0));
	}
}

#ifdef USE_STL_DEQUE
void CGraph::GraphAddPoint(GraphDataInfoStruct graphDataInfoStruct)	
{
	EnterCriticalSection(&m_cs);
	if(m_iGraphPointCurrentNum == 0)
	{
		//第一个绘图点
		//memcpy(m_pGraphDataInfoStruct[m_iGraphPointCurrentNum].nAxisRealPos, pGraphDataInfoStruct->nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
		m_GraphDataInfoStructDeque[m_iGraphPointCurrentNum] = (graphDataInfoStruct);
		m_iGraphPointCurrentNum++;
	}
	else
	{
		if (fabs(m_GraphDataInfoStructDeque.at(m_iGraphPointCurrentNum - 1).nAxisRealPos[0] - graphDataInfoStruct.nAxisRealPos[0]) < m_nGraphCompareError 
			&& fabs(m_GraphDataInfoStructDeque.at(m_iGraphPointCurrentNum - 1).nAxisRealPos[1] - graphDataInfoStruct.nAxisRealPos[1]) < m_nGraphCompareError)
		{
			//与上一个绘图点基本重合时
		}
		else
		{
			//与上一个绘图点不一致时
			if (m_iGraphPointCurrentNum == CNC_GRAPH_MAX_POINT_NUM)
			{
				//绘图点填充已满时，需要先删除第一个，再加入新数据
				m_GraphDataInfoStructDeque.pop_front();
				//m_GraphDataInfoStructDeque[m_iGraphPointCurrentNum] = (graphDataInfoStruct);
				m_GraphDataInfoStructDeque.push_back(graphDataInfoStruct);

				m_GraphPointMinX = INT_MAX;
				m_GraphPointMinY = INT_MAX;
				m_GraphPointMaxX = INT_MIN;
				m_GraphPointMaxY = INT_MIN;
				g_fRate = 0.0;
			}
			else
			{
				//绘图点填充未满时
				//memcpy(m_pGraphDataInfoStruct[m_iGraphPointCurrentNum].nAxisRealPos, pGraphDataInfoStruct->nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
				m_GraphDataInfoStructDeque[m_iGraphPointCurrentNum] = (graphDataInfoStruct);
				//m_GraphDataInfoStructDeque.push_back(graphDataInfoStruct);
				m_iGraphPointCurrentNum++;
			}
		}
	}
	LeaveCriticalSection(&m_cs);
}
#else
void CGraph::GraphAddPoint(GraphDataInfoStruct* pGraphDataInfoStruct)
{
	EnterCriticalSection(&m_cs);
	if (m_iGraphPointCurrentNum == 0)
	{
		//第一个绘图点
		memcpy(m_pGraphDataInfoStruct[m_iGraphPointCurrentNum].nAxisRealPos, pGraphDataInfoStruct->nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
		m_iGraphPointCurrentNum++;
	} 
	else
	{
		if (fabs(m_pGraphDataInfoStruct[m_iGraphPointCurrentNum - 1].nAxisRealPos[0] - pGraphDataInfoStruct->nAxisRealPos[0]) < m_nGraphCompareError 
			&& fabs(m_pGraphDataInfoStruct[m_iGraphPointCurrentNum - 1].nAxisRealPos[1] - pGraphDataInfoStruct->nAxisRealPos[1]) < m_nGraphCompareError)
		{
			//与上一个绘图点基本重合时
		}
		else
		{
			//与上一个绘图点不一致时
			if (m_iGraphPointCurrentNum == CNC_GRAPH_MAX_POINT_NUM - 1)
			{
				//绘图点填充已满时
				for (int i = 0; i < CNC_GRAPH_MAX_POINT_NUM - 1; i++)
				{
					memcpy(m_pGraphDataInfoStruct[i].nAxisRealPos, m_pGraphDataInfoStruct[i + 1].nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
				}
				memcpy(m_pGraphDataInfoStruct[m_iGraphPointCurrentNum].nAxisRealPos, pGraphDataInfoStruct->nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
			} 
			else
			{
				//绘图点填充未满时
				memcpy(m_pGraphDataInfoStruct[m_iGraphPointCurrentNum].nAxisRealPos, pGraphDataInfoStruct->nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
				m_iGraphPointCurrentNum++;
			}
		}
	}
	LeaveCriticalSection(&m_cs);
}
#endif

void CGraph::SetGraphWnd(HWND hGraphWnd)
{
	EnterCriticalSection(&m_cs);
	this->m_hGraphWnd = hGraphWnd;
	this->m_hGraphDC = ::GetDC(m_hGraphWnd);
	LeaveCriticalSection(&m_cs);
}

void CGraph::GraphSetPaintFlag(unsigned char uChPaintFlag)
{
	EnterCriticalSection(&m_cs);
	m_uChGraphPaintFlag = uChPaintFlag;
	LeaveCriticalSection(&m_cs);
}

void CGraph::GraphSetPaintMode(unsigned char uChPaintMode)
{
	EnterCriticalSection(&m_cs);
	m_uChGraphPaintMode = uChPaintMode;
	LeaveCriticalSection(&m_cs);
}

//优化
void CGraph::GraphOptimize(void)
{

}

void CGraph::DrawPoint()
{
	EnterCriticalSection(&m_cs);
	if(m_iGraphPointCurrentNum != 0)
	{
		m_hOldDrawPointPen = (HPEN)::SelectObject(m_hMemDC, m_hDrawPointPen);

		//计算比例
		for(std::deque<GraphDataInfoStruct>::size_type i = 0; i < m_iGraphPointCurrentNum; ++i)
		{
			GraphDataInfoStruct gd = m_GraphDataInfoStructDeque[i];
			double gdPoint[CNC_AXIS_NUM];
			memcpy(gdPoint, gd.nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);

			RefreshPointMinAndMax(gdPoint);

			double xRate = (m_GraphPointMaxX - m_GraphPointMinX) / m_cxExcludeFrameStaticWidth;
			double yRate = (m_GraphPointMaxY - m_GraphPointMinY) / m_cyExcludeFrameStaticHeight;

			//fTempRate = xRate >= yRate ? xRate : yRate;
			//g_fRate = g_fRate > fTempRate ? g_fRate : fTempRate;
			g_fRate = xRate >= yRate ? xRate : yRate;
		}

		for(std::deque<GraphDataInfoStruct>::size_type i = 0; i < m_iGraphPointCurrentNum - 1; ++i)
		{
			GraphDataInfoStruct gd1 = m_GraphDataInfoStructDeque[i];
			GraphDataInfoStruct gd2 = m_GraphDataInfoStructDeque[i + 1];

			double BeforeTransformGd1Point[CNC_AXIS_NUM], BeforeTransformGd2Point[CNC_AXIS_NUM];
			memcpy(BeforeTransformGd1Point, gd1.nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
			memcpy(BeforeTransformGd2Point, gd2.nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);

			////获取区域大小
			//RefreshPointMinAndMax(BeforeTransformGd1Point);
			//RefreshPointMinAndMax(BeforeTransformGd2Point);

			double *AfterTransformGd1Point = TransformPoint(BeforeTransformGd1Point);
			double *AfterTransformGd2Point = TransformPoint(BeforeTransformGd2Point);

			//为防止像素点为同一点，过滤
			if(fabs(int(AfterTransformGd2Point[0]) * 1.0 - int(AfterTransformGd1Point[0]) * 1.0) >= 1.0 || 
				fabs(int(AfterTransformGd2Point[1]) * 1.0 - int(AfterTransformGd1Point[1]) * 1.0) >= 1.0)
			{
				::MoveToEx(m_hMemDC, AfterTransformGd1Point[0], AfterTransformGd1Point[1], NULL);
				::LineTo(m_hMemDC, AfterTransformGd2Point[0], AfterTransformGd2Point[1]);
			}

			delete[] AfterTransformGd1Point;
			delete[] AfterTransformGd2Point;
		}
		::SelectObject(m_hMemDC, m_hOldDrawPointPen);

		double BeforeTransformLastGd[CNC_AXIS_NUM];
		memcpy(BeforeTransformLastGd, m_GraphDataInfoStructDeque[m_iGraphPointCurrentNum - 1].nAxisRealPos, sizeof(double) * CNC_AXIS_NUM);
		double* AfterTransformLastGd = TransformPoint(BeforeTransformLastGd);
		DrawToolPoint(AfterTransformLastGd[0], AfterTransformLastGd[1]);
	}
	LeaveCriticalSection(&m_cs);
}

double* CGraph::TransformPoint(double* formerPoint)
{
	double *afterPoint = new double[CNC_AXIS_NUM];
	memcpy(afterPoint, formerPoint, sizeof(double) * CNC_AXIS_NUM);

	if(g_fRate != 0.0)
	{
		afterPoint[0] /= g_fRate;
		afterPoint[1] /= g_fRate;
	}
	//memcpy(afterPoint, formerPoint, sizeof(double) * CNC_AXIS_NUM);

	//修改坐标
	afterPoint[0] = afterPoint[0] + FRAME_EDAGE_OFFSET + (-m_GraphPointMinX / g_fRate);
	afterPoint[1] = m_cyStaticHeight - FRAME_EDAGE_OFFSET - afterPoint[1] + (m_GraphPointMinY / g_fRate);

	return afterPoint;
}

void CGraph::DrawRectangleFrame(CRect rt)
{
	m_hOldFramePen = (HPEN)::SelectObject(m_hMemDC, m_hFramePen);
	::MoveToEx(m_hMemDC, rt.left, rt.top, NULL);
	::LineTo(m_hMemDC, rt.left, rt.bottom);
	::LineTo(m_hMemDC, rt.right, rt.bottom);
	::LineTo(m_hMemDC, rt.right, rt.top);
	::LineTo(m_hMemDC, rt.left, rt.top);

	::SelectObject(m_hMemDC, m_hOldFramePen);
}

void CGraph::DrawToolPoint(int x, int y)
{
	POINT point[3];
	point[0].x = x;
	point[0].y = y;
	point[1].x = x - 3;
	point[1].y = y - 8;
	point[2].x = x + 3;
	point[2].y = y - 8;

	m_hOldCurrentToolPointBrush = (HBRUSH)::SelectObject(m_hMemDC, m_hCurrentToolPointBrush);
	m_hOldCurrentToolPointPen = (HPEN)::SelectObject(m_hMemDC, m_hCurrentToolPointPen);

	::Polygon(m_hMemDC, point, 3);

	::SelectObject(m_hMemDC, m_hOldCurrentToolPointBrush);
	::SelectObject(m_hMemDC, m_hOldCurrentToolPointPen);

}

void CGraph::RefreshPointMinAndMax(double *currentPoint)
{
	if(currentPoint[0] < m_GraphPointMinX)
		m_GraphPointMinX = currentPoint[0];
	if(currentPoint[1] < m_GraphPointMinY)
		m_GraphPointMinY = currentPoint[1];

	if(currentPoint[0] > m_GraphPointMaxX)
		m_GraphPointMaxX = currentPoint[0];
	if(currentPoint[1] > m_GraphPointMaxY)
		m_GraphPointMaxY = currentPoint[1];
}

void CGraph::LoadDxfFile(CString dxfFileString)
{
	g_ptrGraphArray.RemoveAll();

	DxfReading(dxfFileString);

	GraphInit();

	PreviewDxfGraph();
}

void CGraph::PreviewDxfGraph()
{
	PrepareDraw();

	m_hOldDrawPointPen = (HPEN)::SelectObject(m_hMemDC, m_hDrawPointPen);
	for(int i = 0; i < g_ptrGraphArray.GetSize(); ++i)
	{
		(g_ptrGraphArray.GetAt(i))->Draw(m_hMemDC, CRect(0, 0, m_cxStaticWidth, m_cxStaticWidth), FRAME_EDAGE_OFFSET);
	}
	::SelectObject(m_hMemDC, m_hOldDrawPointPen);
	::BitBlt(m_hGraphDC, 0, 0, m_cxStaticWidth, m_cyStaticHeight, m_hMemDC, 0, 0, SRCCOPY);

	OverDraw();
}

void CGraph::PrepareDraw()
{
	m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemDC, m_hBitmap);
	m_hOldBlackBrush = (HBRUSH)::SelectObject(m_hMemDC, m_hBlackBrush);

	::FillRect(m_hMemDC,  CRect(0, 0, m_cxStaticWidth, m_cyStaticHeight), m_hBlackBrush);

	CRect rtRectangleFrame;
	rtRectangleFrame.CopyRect(&CRect(0 + FRAME_EDAGE_OFFSET, 0 + FRAME_EDAGE_OFFSET, m_cxExcludeFrameStaticWidth + FRAME_EDAGE_OFFSET, m_cyExcludeFrameStaticHeight + FRAME_EDAGE_OFFSET));
	DrawRectangleFrame(rtRectangleFrame);
}

void CGraph::OverDraw()
{
	::SelectObject(m_hMemDC, m_hOldBlackBrush);
	::SelectObject(m_hMemDC, m_hOldBitmap);
}
