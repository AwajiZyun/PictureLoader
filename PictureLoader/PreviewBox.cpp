#include "stdafx.h"
#include "PreviewBox.h"


CPreviewBox::CPreviewBox()
	: m_pCanvas(NULL)
	, m_nCurShowPage(0)
	, m_nLoadCnt(0)
{
	//m_arrImage.SetSize(20, 10);
}


CPreviewBox::~CPreviewBox()
{
	if (m_pCanvas) {
		if (m_hCanvasDC != NULL) {
			m_pCanvas->ReleaseDC();
		}
		delete m_pCanvas;
	}
	m_arrImage.RemoveAll();
}

BOOL CPreviewBox::Create(int x, int y, int width, int height, CWnd * pParent)
{
	if (width < THUMBNAIL_BOX_GAP + THUMBNAIL_BOX_WIDTH * 2) {
		width = THUMBNAIL_BOX_GAP + THUMBNAIL_BOX_WIDTH * 2;
	}
	CRect rect(x, y, x + width, y + height);
	if (!CWnd::Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL, rect, pParent, 1024)) {
		return FALSE;
	}
	if (m_pCanvas == NULL) {
		m_pCanvas = new CImage;
		m_pCanvas->Create(width, height, 32, CImage::createAlphaChannel);
		m_hCanvasDC = m_pCanvas->GetDC();
	}
	return TRUE;
}

BOOL CPreviewBox::AddImage(LPCTSTR  imgPath)
{
	CImage img;
	if (S_OK != img.Load(imgPath)) {
		return FALSE;
	}

	// Éè¶¨ËõÂÔÍ¼³ß´ç£¬Ïà¶ÔÎ»ÖÃ
	int iImgWidth = img.GetWidth();
	int iImgHeight = img.GetHeight();
	float iRatio = 1.0f;
	if (iImgWidth >= iImgHeight) {
		iRatio = THUMBNAIL_BOX_WIDTH / static_cast<float>(iImgWidth);
	}
	else {
		iRatio = THUMBNAIL_BOX_HEIGHT / static_cast<float>(iImgHeight);
	}
	int iThumbWidth = iImgWidth * iRatio;
	int iThumbHeight = iImgHeight * iRatio;
	int iThumbX = (THUMBNAIL_BOX_WIDTH - iThumbWidth) / 2;
	int iThumbY = (THUMBNAIL_BOX_HEIGHT - iThumbHeight) / 2;

	// »æÖÆËõÂÔÍ¼±³¾°
	CImage *thumbBack = new CImage;
	thumbBack->Create(THUMBNAIL_BOX_WIDTH, THUMBNAIL_BOX_HEIGHT, 32, CImage::createAlphaChannel);
	HDC hBackDC = thumbBack->GetDC();
	::SetStretchBltMode(hBackDC, COLORONCOLOR);
	HPEN hPen = ::CreatePen(PS_SOLID, 1, THUMBNAIL_BOX_BACKGROUND);
	HBRUSH hBrush = ::CreateSolidBrush(THUMBNAIL_BOX_BACKGROUND);
	HPEN hOldPen = (HPEN)::SelectObject(hBackDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hBackDC, hBrush);
	::Rectangle(hBackDC, 0, 0, THUMBNAIL_BOX_WIDTH, THUMBNAIL_BOX_HEIGHT);
	::SelectObject(hBackDC, hOldPen);
	::SelectObject(hBackDC, hOldBrush);
	// »æÖÆËõÂÔÍ¼
	img.StretchBlt(hBackDC, iThumbX, iThumbY, iThumbWidth, iThumbHeight, SRCCOPY);
	thumbBack->ReleaseDC();

	m_arrImage.Add(thumbBack);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CPreviewBox, CWnd)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CPreviewBox::OnPaint()
{
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(rcClient);
	// Clear canvas off screen image.
	if (m_pCanvas) {
		if (m_hCanvasDC) {
			m_pCanvas->ReleaseDC();
		}
		delete m_pCanvas;
		m_pCanvas = new CImage;
		m_pCanvas->Create(rcClient.Width(), rcClient.Height(), 32, CImage::createAlphaChannel);
		m_hCanvasDC = m_pCanvas->GetDC();
	}
	if (m_arrImage.GetSize()) {
		HPEN hPen = ::CreatePen(PS_SOLID, 1, THUMBNAIL_CANVAS_BACKGROUND);
		HBRUSH hBrush = ::CreateSolidBrush(THUMBNAIL_CANVAS_BACKGROUND);
		HPEN hOldPen = (HPEN)::SelectObject(m_hCanvasDC, hPen);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hCanvasDC, hBrush);
		::Rectangle(m_hCanvasDC, 0, 0, rcClient.Width(), rcClient.Height());
		::SelectObject(m_hCanvasDC, hOldPen);
		::SelectObject(m_hCanvasDC, hOldBrush);

		// Draw thumbnail box.
		SCROLLINFO vsi;
		GetScrollInfo(SB_VERT, &vsi);

		int iColumn = rcClient.Width() / THUMBNAIL_BOX_WIDTH;
		int iThumbnailMargin = (rcClient.Width() % THUMBNAIL_BOX_WIDTH - THUMBNAIL_BOX_GAP * 2) / (iColumn - 1);
		if (iThumbnailMargin <= 0)
			iThumbnailMargin = 1;
		// µ±Ç°Ò³Í¼Æ¬ÖØ»æ
		int totalCnt = GetSize();
		int loadMaxCnt = totalCnt < ((m_nCurShowPage + 1) * m_nLoadCnt) ? totalCnt : ((m_nCurShowPage + 1)* m_nLoadCnt);
		for (int i = m_nCurShowPage * m_nLoadCnt; i < loadMaxCnt; i++)
		{
			int iThumbnailX = (i % m_nLoadCnt % iColumn) * (THUMBNAIL_BOX_WIDTH + iThumbnailMargin) + THUMBNAIL_BOX_GAP;
			int iThumbnailY = -vsi.nPos + (i % m_nLoadCnt / iColumn) * (THUMBNAIL_BOX_HEIGHT + THUMBNAIL_BOX_GAP) + THUMBNAIL_BOX_GAP;
			CImage* pThumbnail = GetImageAt(i);
			pThumbnail->BitBlt(m_hCanvasDC, iThumbnailX, iThumbnailY, SRCCOPY);
		}

		m_pCanvas->BitBlt((HDC)dc, 0, 0, SRCCOPY);
	}
	else {
		// Ô¤ÀÀÇø³õÊ¼±³¾°
		CImage imgBg;
		imgBg.LoadFromResource(AfxGetInstanceHandle(), IDB_BG);
		imgBg.Draw(GetDC()->m_hDC, rcClient);
	}
}


UINT CPreviewBox::GetSize() const
{
	return m_arrImage.GetSize();
}


CImage* CPreviewBox::GetImageAt(int iIndex) const
{
	if (!m_arrImage.GetSize()) {
		return nullptr;
	}
	if (0 > iIndex || iIndex >= m_arrImage.GetSize()) {
		return nullptr;
	}
	return m_arrImage.GetAt(iIndex);
}


BOOL CPreviewBox::RemoveAllImg()
{
	for (int i = 0; i < m_arrImage.GetSize(); i++)
	{
		CImage* pImage = m_arrImage.GetAt(i);
		pImage->Destroy();
		delete pImage;
	}
	m_arrImage.RemoveAll();
	return 0;
}


void CPreviewBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int iMin, iMax;
	static int iCurrentScrollPos = 0;
	GetScrollRange(SB_VERT, &iMin, &iMax);
	
	//iCurrentScrollPos = GetScrollPos(SB_VERT);
	switch (nSBCode)
	{
	case SB_TOP:
		iCurrentScrollPos = iMin;
		break;
	case SB_BOTTOM:
		iCurrentScrollPos = iMax;
	case SB_LINEDOWN:
		if (iCurrentScrollPos < iMax)
		{
			iCurrentScrollPos += 10;
		}
		break;
	case SB_LINEUP:
		if (iCurrentScrollPos > iMin)
		{
			iCurrentScrollPos -= 10;
		}
		break;
	case SB_PAGEDOWN:
	{
		SCROLLINFO si;
		GetScrollInfo(SB_VERT, &si, SIF_ALL);
		if (iCurrentScrollPos < iMax)
			iCurrentScrollPos = min(iMax, iCurrentScrollPos + (int)si.nPage);
	}
	break;
	case SB_PAGEUP:
	{
		SCROLLINFO si;
		GetScrollInfo(SB_VERT, &si, SIF_ALL);
		if (iCurrentScrollPos > iMin)
			iCurrentScrollPos = max(iMin, iCurrentScrollPos - (int)si.nPage);
	}
	break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		iCurrentScrollPos = nPos;
		break;
	case SB_ENDSCROLL:
		break;
	}

	if (iCurrentScrollPos < 0)
	{
		iCurrentScrollPos = 0;
	}
	if (iCurrentScrollPos > iMax)
	{
		iCurrentScrollPos = iMax;
	}

	SetScrollPos(SB_VERT, iCurrentScrollPos);
	//m_iLastScrollPos = iCurrentScrollPos;
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate(FALSE);
	UpdateWindow();
}


void CPreviewBox::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// µ÷Õû¹ö¶¯Ìõ
	int iColumn = cx / THUMBNAIL_BOX_WIDTH;
	int iRow = 0;
	if (m_nLoadCnt) {
		if ((m_nCurShowPage + 1) * m_nLoadCnt >= GetSize()) {
			iRow = (m_nCurShowPage + 1) * m_nLoadCnt - GetSize() / iColumn + 1;
		}
		else {
			iRow = m_nLoadCnt / iColumn + 1;
		}
	}
	else {
		iRow = GetSize() / iColumn + 1;
	}
	int iScrollMax = max(iRow * (THUMBNAIL_BOX_HEIGHT + THUMBNAIL_BOX_GAP), cy - 1);
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = iScrollMax;
	si.nPage = cy;
	SetScrollInfo(SB_VERT, &si, TRUE);
	
	Invalidate(FALSE);
	UpdateWindow();
	//static int iPreYPos = 0;
	//CRect rect;
	//GetClientRect(&rect);

	//SCROLLINFO vertiScroInfo;
	//vertiScroInfo.cbSize = sizeof(SCROLLINFO);
	//vertiScroInfo.fMask = SIF_ALL;
	//vertiScroInfo.nMin = 0;
	//vertiScroInfo.nMax = rect.Height();
	//vertiScroInfo.nPage = rect.Height() / cy;
	//SetScrollInfo(SB_VERT, &vertiScroInfo, TRUE);

	//int iCurYPos = GetScrollPos(SB_VERT);
	//
	//if (iCurYPos < iPreYPos)
	//{
	//	ScrollWindow(0, iPreYPos - iCurYPos);
	//}
	//iPreYPos = iCurYPos;

	//Invalidate(FALSE);
	//UpdateWindow();
}


int CPreviewBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
