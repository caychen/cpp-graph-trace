#pragma once

#define USE_STL_DEQUE
#define FRAME_EDAGE_OFFSET		10

#ifdef USE_STL_DEQUE
#include <deque>
#endif

typedef struct tagGraphDataInfoStruct 
{
	double nAxisRealPos[CNC_AXIS_NUM];
	char ChGCode;
	TCHAR tChCodeContent[CNC_FILE_CONTENT_LENGTH];
}GraphDataInfoStruct;

class CGraph
{
public:
	CGraph(void);
	virtual ~CGraph(void);
	void GraphMain(void);
	void GraphInit(void);

#ifdef USE_STL_DEQUE
	void GraphAddPoint(GraphDataInfoStruct graphDataInfoStruct);
#else
	void GraphAddPoint(GraphDataInfoStruct* pGraphDataInfoStruct);
#endif

	void GraphSetPaintFlag(unsigned char uChPaintFlag);	//设置绘图开关标识
	void GraphSetPaintMode(unsigned char uChPaintMode);	//设置绘图模式
	void GraphOptimize(void);	//绘图优化函数

	void SetGraphWnd(HWND hWnd);
	void LoadDxfFile(CString);

private:
	//画边框
	void DrawRectangleFrame(CRect);
	//画点
	void DrawPoint();
	//画刀尖点
	void DrawToolPoint(int, int);

	double* TransformPoint(double*);
	void RefreshPointMinAndMax(double*);

	//预览
	void PreviewDxfGraph();

	//Draw准备
	void PrepareDraw();
	//Draw结束
	void OverDraw();
	

protected:
#ifdef USE_STL_DEQUE
	std::deque<GraphDataInfoStruct> m_GraphDataInfoStructDeque;
#else
	GraphDataInfoStruct* m_pGraphDataInfoStruct;
#endif
	int m_iGraphPointCurrentNum;
	HWND m_hGraphWnd;
	HDC m_hGraphDC;
	double m_nGraphCompareError;
	unsigned char m_uChGraphPaintFlag;	//图形绘图开关
	unsigned char m_uChGraphPaintMode;	//图形绘图模式


private:
	CRITICAL_SECTION m_cs;
	int m_cxStaticWidth;
	int m_cyStaticHeight;
	int m_cxExcludeFrameStaticWidth;//除去边框外后的宽
	int m_cyExcludeFrameStaticHeight;//除去边框外后的高

	double m_GraphPointMinX;
	double m_GraphPointMinY;
	double m_GraphPointMaxX;
	double m_GraphPointMaxY;

	HBRUSH m_hBlackBrush, m_hOldBlackBrush;
	HPEN m_hDrawPointPen, m_hOldDrawPointPen;
	HDC m_hMemDC;
	HBITMAP m_hBitmap, m_hOldBitmap;
	HPEN m_hFramePen, m_hOldFramePen;
	HPEN m_hCurrentToolPointPen, m_hOldCurrentToolPointPen;
	HBRUSH m_hCurrentToolPointBrush, m_hOldCurrentToolPointBrush;
};
