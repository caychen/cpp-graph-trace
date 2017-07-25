#pragma once


enum Global_Graph_Type
{
	NC_NONE = 0,
	NC_LINE,
	NC_CIRCLE,
	NC_ARC,
	NC_ELLIPSE,
	NC_POLYLINE,
	NC_SPLINE,
	NC_POINT
};

class CBaseGraph
{
public:
	CBaseGraph(void)
	{
		m_GraphType = NC_NONE;
	}

	virtual ~CBaseGraph(void){}

public:
	Global_Graph_Type m_GraphType;

	virtual void Draw(HDC, CRect, int) = 0;
};
