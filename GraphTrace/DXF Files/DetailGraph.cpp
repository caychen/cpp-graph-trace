#include "stdafx.h"
#include "DetailGraph.h"

extern double g_fRate;

void CLineGraph::Draw(HDC hDC, CRect rt, int cFrameOffset)
{
//	this->m_fStartPoint = CPoint3D(0, 0, 0);
//	this->m_fEndPoint = CPoint3D(100, 150, 0);
	::MoveToEx(hDC, 
		this->m_fStartPoint.m_cx / g_fRate, 
		this->m_fStartPoint.m_cy / g_fRate, NULL);
	::LineTo(hDC, 
		this->m_fEndPoint.m_cx / g_fRate, 
		this->m_fEndPoint.m_cy / g_fRate);
}

void CCircelGraph::Draw(HDC hDC, CRect rt, int cFrameOffset)
{
	/*this->m_fCenterPoint = CPoint3D(50,50,0);
	this->m_fRadius = 50;*/
	::Ellipse(hDC, 
		this->m_fCenterPoint.m_cx - this->m_fRadius, 
		this->m_fCenterPoint.m_cy - this->m_fRadius, 
		this->m_fCenterPoint.m_cx + this->m_fRadius, 
		this->m_fCenterPoint.m_cy + this->m_fRadius);
}

void CArcGraph::Draw(HDC hDC, CRect rt, int cFrameOffset)
{
	/*::MoveToEx(hDC, 
		this->m_fStartPoint.m_cx - (this->m_fCenterPoint.m_cx - 50.0), 
		this->m_fStartPoint.m_cy - (this->m_fCenterPoint.m_cy - 50.0),
		NULL);

	this->m_fCenterPoint = CPoint3D(50.0, 50.0, 0.0);
	this->m_fRadius = 20.0;*/

	::AngleArc(hDC, 
		this->m_fCenterPoint.m_cx, 
		this->m_fCenterPoint.m_cy,
		this->m_fRadius,
		this->m_fStartAngle,
		this->m_fSweepAngle);
}

void CEllipseGraph::Draw(HDC, CRect rt, int cFrameOffset)
{
}

void CPolylineGraph::Draw(HDC, CRect rt, int cFrameOffset)
{
}

void CSplineGraph::Draw(HDC, CRect rt, int cFrameOffset)
{
}

void CPointGraph::Draw(HDC, CRect rt, int cFrameOffset)
{
}
