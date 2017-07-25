/*
* @file test_creationclass.cpp
*/

/*****************************************************************************
**  $Id: test_creationclass.cpp 163 2003-07-01 15:51:48Z andrew $
**
**  This is part of the dxflib library
**  Copyright (C) 2001 Andrew Mustun
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU Library General Public License as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Library General Public License for more details.
**
**  You should have received a copy of the GNU Library General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/
#include "stdafx.h"
#include "Test_CreationClass.h"
#include "DL_Dxf.h"

double DxfToolSafeHeight = 10;

extern CArray<CBaseGraph*> g_ptrGraphArray;

void DxfReading(CString DxfFileName) 
{
	// Load DXF file into memory
	//CString SaveFileName;
	//SaveFileName = DxfFileName;
	//int nPointPos = SaveFileName.ReverseFind('.');

	//if(nPointPos != -1)
	//{
	//	SaveFileName = SaveFileName.Left(nPointPos);
	//}

	//SaveFileName += _T(".txt");

	Test_CreationClass* creationClass = new Test_CreationClass();
	DL_Dxf* dxf = new DL_Dxf();
	if (!dxf->in(DxfFileName, creationClass)) 
	{ // if file open failed
		return ;
	}
	//creationClass->OutputPathCreate();
	delete dxf;
	delete creationClass;
	
}

Test_CreationClass::Test_CreationClass()
{
	
}

Test_CreationClass::~Test_CreationClass(void)
{
	
}

/**
* Sample implementation of the method which handles layers.
*/
void Test_CreationClass::addLayer(const DL_LayerData& data) 
{
}

/**
* Sample implementation of the method which handles point entities.
*/
void Test_CreationClass::addPoint(const DL_PointData& data) 
{
	CPointGraph* pPointGraph = new CPointGraph();

	//点坐标
	pPointGraph->m_fCenterPoint.m_cx = data.x;
	pPointGraph->m_fCenterPoint.m_cy = data.y;
	pPointGraph->m_fCenterPoint.m_cz = data.z;
	g_ptrGraphArray.Add(pPointGraph);
}

/**
* Sample implementation of the method which handles line entities.
*/
void Test_CreationClass::addLine(const DL_LineData& data) 
{
	CLineGraph* pLineGraph = new CLineGraph();
	//起点坐标
	pLineGraph->m_fStartPoint.m_cx = data.x1;
	pLineGraph->m_fStartPoint.m_cy = data.y1;
	pLineGraph->m_fStartPoint.m_cz = data.z1;

	//终点坐标
	pLineGraph->m_fEndPoint.m_cx = data.x2;
	pLineGraph->m_fEndPoint.m_cy = data.y2;
	pLineGraph->m_fEndPoint.m_cz = data.z2;

	g_ptrGraphArray.Add(pLineGraph);
}

/**
* Sample implementation of the method which handles arc entities.
*/
void Test_CreationClass::addArc(const DL_ArcData& data)
{
	CArcGraph* pArcGraph = new CArcGraph();
	//圆心坐标
	pArcGraph->m_fCenterPoint.m_cx = data.cx;
	pArcGraph->m_fCenterPoint.m_cy = data.cy;
	pArcGraph->m_fCenterPoint.m_cz = data.cz;

	//起始角和终点角
	pArcGraph->m_fStartAngle = data.angle1;
	pArcGraph->m_fEndAngle = data.angle2;
	//经过的角度
	pArcGraph->m_fSweepAngle = data.angle2 - data.angle1;

	//半径
	pArcGraph->m_fRadius = data.radius;

	//起点坐标
	double tempAngle = data.angle1 * M_PI / 180;
	pArcGraph->m_fStartPoint.m_cx = (data.cx + data.radius * cos(tempAngle));
	pArcGraph->m_fStartPoint.m_cy = (data.cy + data.radius * sin(tempAngle));
	pArcGraph->m_fStartPoint.m_cz = data.cz ;

	g_ptrGraphArray.Add(pArcGraph);
}

/**
* Sample implementation of the method which handles circle entities.
*/
void Test_CreationClass::addCircle(const DL_CircleData& data) 
{
	CCircelGraph* pCircleGraph = new CCircelGraph();
	//圆心坐标
	pCircleGraph->m_fCenterPoint.m_cx = data.cx;
	pCircleGraph->m_fCenterPoint.m_cy = data.cy;
	pCircleGraph->m_fCenterPoint.m_cz = data.cz;

	//半径
	pCircleGraph->m_fRadius = data.radius;

	g_ptrGraphArray.Add(pCircleGraph);
}


/**
* Sample implementation of the method which handles spline entities.
*/
CSplineGraph* pSplineGraph = NULL;
void Test_CreationClass::addSpline(const DL_SplineData& data)
{
	pSplineGraph = new CSplineGraph();

	//控制点数
	pSplineGraph->m_nControlNodeNumber = data.nControl;
}

void Test_CreationClass::addControlPoint(const DL_ControlPointData& data)
{
	//控制点坐标
	CPoint3D controlNode(data.x, data.y, data.z);
	//控制点集合
	pSplineGraph->m_VectorControlNode.push_back(controlNode);

	if(pSplineGraph->m_VectorControlNode.size() == pSplineGraph->m_nControlNodeNumber)
	{
		g_ptrGraphArray.Add(pSplineGraph);
	}
}

void Test_CreationClass::addKnot(const DL_KnotData& data)
{

}


void Test_CreationClass::addEllipse(const DL_EllipseData& data)
{
	CEllipseGraph* pEllipseGraph = new CEllipseGraph();

	//椭圆中心坐标
	pEllipseGraph->m_fCenterPoint.m_cx = data.cx;
	pEllipseGraph->m_fCenterPoint.m_cy = data.cy;
	pEllipseGraph->m_fCenterPoint.m_cz = data.cz;

	//相对于中心的长轴坐标
	pEllipseGraph->m_fMajorAsixPoint.m_cx = data.mx;
	pEllipseGraph->m_fMajorAsixPoint.m_cy = data.my;
	pEllipseGraph->m_fMajorAsixPoint.m_cz = data.mz;

	//开始参数，闭合椭圆为0
	pEllipseGraph->m_fStartAngle = data.angle1;
	//结束参数，闭合椭圆为2 * PI
	pEllipseGraph->m_fEndAngle = data.angle2;

	//短轴与长轴的比例 
	pEllipseGraph->m_fRatio = data.ratio;

	g_ptrGraphArray.Add(pEllipseGraph);
}

/**
* Sample implementation of the method which handles polyline entities.
*/
CPolylineGraph* pPolylineGraph = NULL;
void Test_CreationClass::addPolyline(const DL_PolylineData& data) 
{
	pPolylineGraph = new CPolylineGraph();
	//多段线顶点个数
	pPolylineGraph->m_nPolylineNumber = data.number;

	//多段线闭合标记，默认为0，闭合为1
	pPolylineGraph->m_bFlags = data.flags;
}

/**
* Sample implementation of the method which handles vertices.
*/
void Test_CreationClass::addVertex(const DL_VertexData& data) 
{
	//顶点坐标，根据bulge判断是直线还是圆弧，0为直线，非0为圆弧
	CPoint3D vertex(data.x, data.y, data.bulge);
	//顶点集合
	pPolylineGraph->m_VertexArray.push_back(vertex);

	if(pPolylineGraph->m_nPolylineNumber == pPolylineGraph->m_VertexArray.size())
	{
		if(pPolylineGraph->m_bFlags != 1)
		{
			if(fabs(pPolylineGraph->m_VertexArray.at(0).m_cx - data.x) < 10e-3 
				&& fabs(pPolylineGraph->m_VertexArray.at(0).m_cy - data.y) < 10e-3)
			{
				pPolylineGraph->m_bFlags = 1;
				pPolylineGraph->m_VertexArray.pop_back();
				pPolylineGraph->m_nPolylineNumber -= 1;
			}
			else
			{
				pPolylineGraph->m_bFlags = 0;
			}
		}

		g_ptrGraphArray.Add(pPolylineGraph);
	}
	
}

void Test_CreationClass::addText(const DL_TextData& data)
{
	
}

void Test_CreationClass::addMText(const DL_MTextData& data)
{
	
}

void Test_CreationClass::printAttributes() 
{

}