#ifndef __POINT3D_H__
#define __POINT3D_H__

typedef struct CPoint3D
{
	CPoint3D()
	{
		this->m_cx = this->m_cy = this->m_cz = 0.0;
	}
	CPoint3D(double x, double y, double z)
		: m_cx(x), m_cy(y), m_cz(z){}

	double m_cx;
	double m_cy;
	double m_cz;

}CPoint3D;

#endif