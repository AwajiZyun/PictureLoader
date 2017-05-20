// refs http://download.csdn.net/download/pcradio/8950871
#pragma once
#include "afxwin.h"
#include "resource.h"

#define THUMBNAIL_BOX_BACKGROUND	RGB(128, 128, 128)
#define THUMBNAIL_CANVAS_BACKGROUND	RGB(200, 200, 200)

const UINT FUNCTION_AREA_WIDTH = 140;
const UINT STATUS_AREA_HEIGHT = 25 + 10;
const UINT THUMBNAIL_BOX_WIDTH = 200;
const UINT THUMBNAIL_BOX_HEIGHT = 200;
const UINT THUMBNAIL_BOX_GAP = 5;

class CPreviewBox :
	public CWnd
{
public:
	CPreviewBox();
	virtual ~CPreviewBox();

	BOOL Create(int x, int y, int width, int height, CWnd* pParent);	// ´´½¨Ô¤ÀÀÇø´°¿Ú
	BOOL AddImage(LPCTSTR  imgPath);

protected:
	CImage* m_pCanvas;							// Ô¤ÀÀÇø±³¾°
	HDC m_hCanvasDC;
	CArray<CImage*, CImage*> m_arrImage;		// Ô¤ÀÀÇøËõÂÔÍ¼ÈÝÆ÷
public:
	UINT GetSize() const;
	CImage* GetImageAt(int iIndex) const;
	BOOL RemoveAllImg();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	int m_nCurShowPage;
	int m_nLoadCnt;
};

