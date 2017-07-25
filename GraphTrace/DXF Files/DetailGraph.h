#pragma once
#include "BaseGraph.h"
#include "Point3D.h"
#include <vector>

class CLineGraph : public CBaseGraph
{
public:
	CPoint3D m_fStartPoint;
	CPoint3D m_fEndPoint;

public:
	CLineGraph()
	{
		m_GraphType = NC_LINE;
	}

	void Draw(HDC, CRect, int);
};

class CCircelGraph : public CBaseGraph
{
public:
	CPoint3D m_fCenterPoint;
	double m_fRadius;

public:
	CCircelGraph()
	{
		m_GraphType = NC_CIRCLE;
	}

	void Draw(HDC, CRect, int);
};

class CArcGraph : public CCircelGraph
{
public:
	double m_fStartAngle;
	double m_fEndAngle;
	CPoint3D m_fStartPoint;
	double m_fSweepAngle;

public:
	CArcGraph()
	{
		m_GraphType = NC_ARC;
	}

	void Draw(HDC, CRect, int);
};

class CEllipseGraph : public CArcGraph
{
public:
	CPoint3D m_fMajorAsixPoint;
	double m_fRatio;

public:
	CEllipseGraph()
	{
		m_GraphType = NC_ELLIPSE;
	}

	void Draw(HDC, CRect, int);
};

class CPolylineGraph : public CBaseGraph
{
public:
	std::vector<CPoint3D> m_VertexArray;
	unsigned m_nPolylineNumber;
	int m_bFlags;

public:
	CPolylineGraph()
	{
		m_GraphType = NC_POLYLINE;
	}

	void Draw(HDC, CRect, int);
};

class CSplineGraph : public CBaseGraph
{
public:
	unsigned m_nControlNodeNumber;
	std::vector<CPoint3D> m_VectorControlNode;

public:
	CSplineGraph()
	{
		m_GraphType = NC_SPLINE;
	}

	void Draw(HDC, CRect, int);
};

class CPointGraph : public CBaseGraph
{
public:
	CPoint3D m_fCenterPoint;

public:
	CPointGraph()
	{
		m_GraphType = NC_POINT;
	}

	//void SetCenterPoint(double, double, double);

	void Draw(HDC, CRect, int);
};
