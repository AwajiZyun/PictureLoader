#pragma once
#include "afxwin.h"

class CAboutStatic :
	public CStatic
{
public:
	CAboutStatic();
	~CAboutStatic();
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

