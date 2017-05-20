#include "stdafx.h"
#include "AboutStatic.h"


CAboutStatic::CAboutStatic()
{
}


CAboutStatic::~CAboutStatic()
{
}
BEGIN_MESSAGE_MAP(CAboutStatic, CStatic)
	ON_WM_MOUSEHOVER()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CAboutStatic::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	AfxMessageBox(_T("Catch"));

	CStatic::OnMouseHover(nFlags, point);
}




void CAboutStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	//
	AfxMessageBox(_T("About"));

	CStatic::OnLButtonUp(nFlags, point);
}
