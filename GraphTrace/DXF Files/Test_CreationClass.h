#pragma once


/**
* @file test_creationclass.h
*/

/*****************************************************************************
**  $Id: test_creationclass.h 219 2004-01-07 23:08:40Z andrew $
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

#ifndef TEST_CREATIONCLASS_H
#define TEST_CREATIONCLASS_H

#include "Windows.h"
#include "DL_CreationAdapter.h"
#include "Vec.h"
#include <vector>
#include "BaseGraph.h"
#include "DetailGraph.h"

void  DxfReading(CString DxfFileName); 

/**
* This class takes care of the entities read from the file.
* Usually such a class would probably store the entities.
* this one just prints some information about them to stdout.
*
* @author Andrew Mustun
*/

class Test_CreationClass : public DL_CreationAdapter
{
public:
	Test_CreationClass();
	virtual ~Test_CreationClass(void);

	virtual void addLayer(const DL_LayerData& data);

	//��
	virtual void addPoint(const DL_PointData& data);

	//ֱ��
	virtual void addLine(const DL_LineData& data);

	//Բ��
	virtual void addArc(const DL_ArcData& data);

	//��Բ
	virtual void addCircle(const DL_CircleData& data);

	//�����
	virtual void addPolyline(const DL_PolylineData& data);
	virtual void addVertex(const DL_VertexData& data);

	//��Բ
	virtual void addEllipse(const DL_EllipseData& data);

	//��������
	virtual void addSpline(const DL_SplineData& data);
	virtual void addControlPoint(const DL_ControlPointData& data);
	virtual void addKnot(const DL_KnotData& data);

	//�ı�
	virtual void addText(const DL_TextData& data);
	virtual void addMText(const DL_MTextData& data);

	void printAttributes();

private:
	
};

#endif
