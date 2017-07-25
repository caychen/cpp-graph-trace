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

	void GraphSetPaintFlag(unsigned char uChPaintFlag);	//���û�ͼ���ر�ʶ
	void GraphSetPaintMode(unsigned char uChPaintMode);	//���û�ͼģʽ
	void GraphOptimize(void);	//��ͼ�Ż�����

	void SetGraphWnd(HWND hWnd);
	void LoadDxfFile(CString);

private:
	//���߿�
	void DrawRectangleFrame(CRect);
	//����
	void DrawPoint();
	//�������
	void DrawToolPoint(int, int);

	double* TransformPoint(double*);
	void RefreshPointMinAndMax(double*);

	//Ԥ��
	void PreviewDxfGraph();

	//Draw׼��
	void PrepareDraw();
	//Draw����
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
	unsigned char m_uChGraphPaintFlag;	//ͼ�λ�ͼ����
	unsigned char m_uChGraphPaintMode;	//ͼ�λ�ͼģʽ


private:
	CRITICAL_SECTION m_cs;
	int m_cxStaticWidth;
	int m_cyStaticHeight;
	int m_cxExcludeFrameStaticWidth;//��ȥ�߿����Ŀ�
	int m_cyExcludeFrameStaticHeight;//��ȥ�߿����ĸ�

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
