
// PictureLoaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PictureLoader.h"
#include "PictureLoaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPictureLoaderDlg dialog



CPictureLoaderDlg::CPictureLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PICTURELOADER_DIALOG, pParent)
	, m_pPreviewBox(NULL)
	, m_nLoadCnt(DEFAULT_MAX_LOAD_CNT)
	, m_strDownloadPath(_T(""))
	, m_nCurShowPage(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPictureLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PREVIOUS, m_btnPrevious);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_COMBO, m_comboSite);
}

BEGIN_MESSAGE_MAP(CPictureLoaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CPictureLoaderDlg::OnBnSearch)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CPictureLoaderDlg::OnBnConfig)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CPictureLoaderDlg::OnBnNextPage)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &CPictureLoaderDlg::OnBnPreviousPage)
END_MESSAGE_MAP()


// CPictureLoaderDlg message handlers

BOOL CPictureLoaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// 读入配置文件
	if (!GetSettings()) {
		return TRUE;
	}

	// 初始化选择条
	InitComboBox();
	m_comboSite.SetFocus();

	// 预览区初始化
	CRect rcClient;
	GetClientRect(rcClient);
	m_pPreviewBox = new CPreviewBox;
	m_pPreviewBox->Create(0, 0, rcClient.Width() - FUNCTION_AREA_WIDTH, rcClient.Height() - STATUS_AREA_HEIGHT, this);
	m_pPreviewBox->m_nLoadCnt = m_nLoadCnt;
	m_pPreviewBox->m_nCurShowPage = m_nCurShowPage;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPictureLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPictureLoaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPictureLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPictureLoaderDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// 限定对话框最小缩放尺寸
	POINT point{ MIN_DIALOG_WIDTH, MIN_DIALOG_HEIGHT };
	lpMMI->ptMinTrackSize = point;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CPictureLoaderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	// 调整预览区尺寸
	if (m_pPreviewBox) {
		//m_pPreviewBox->MoveWindow(0, 0, cx - FUNCTION_AREA_WIDTH, cy - STATUS_AREA_HEIGHT);
		m_pPreviewBox->SetWindowPos(NULL, 0, 0, cx - FUNCTION_AREA_WIDTH, cy - STATUS_AREA_HEIGHT, SWP_NOMOVE);
	}
	// 调整前后页按钮
	if (m_btnPrevious && m_btnNext) {
		CRect btnRect;
		m_btnPrevious.GetWindowRect(&btnRect);
		m_btnPrevious.SetWindowPos(NULL, 0, cy - STATUS_AREA_HEIGHT, cx, cy, SWP_NOZORDER | SWP_NOSIZE);
		m_btnNext.GetWindowRect(&btnRect);
		m_btnNext.SetWindowPos(NULL, cx - btnRect.Width() - FUNCTION_AREA_WIDTH, cy - STATUS_AREA_HEIGHT, cx, cy, SWP_NOZORDER | SWP_NOSIZE);
	}
}


void CPictureLoaderDlg::OnBnSearch()
{
	// TODO: Add your control notification handler code here
	m_nCurShowPage = 0;
	m_btnNext.EnableWindow(TRUE);
	m_btnPrevious.EnableWindow(TRUE);

	const UINT MAX_OPEN_FILE = 1024;
	CFileDialog dlgFileOpen(TRUE);

	dlgFileOpen.m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY;
	dlgFileOpen.m_ofn.lpstrFilter = _T("图片\0*.jpeg; *.jpg; *.png; *.bmp\0所有文件\0*.*\0");
	dlgFileOpen.m_ofn.nMaxFile = MAX_OPEN_FILE;
	TCHAR *buff = new TCHAR[MAX_OPEN_FILE * (MAX_PATH + 1)];
	memset(buff, 0, sizeof(TCHAR) * (MAX_OPEN_FILE * (MAX_PATH + 1)));
	dlgFileOpen.m_ofn.lpstrFile = buff;
	//dlgFileOpen.GetOFN().lpstrFile = buff;
	//dlgFileOpen.GetOFN().lpstrFile[0] = NULL;
	//std::array<CString, MAX_OPEN_FILE> filePath;

	if (IDOK == dlgFileOpen.DoModal()) {
		m_pPreviewBox->RemoveAllImg();
		POSITION curPos = dlgFileOpen.GetStartPosition();
		//UINT index = 0;
		while (curPos) {
			CString curPath = dlgFileOpen.GetNextPathName(curPos);
			//filePath[index] =curPath;
			//index++;
			//TCHAR *tmp = new TCHAR[256];
			//tmp = curPath.GetBuffer(curPath.GetLength() * sizeof(TCHAR));
			//MessageBox(curPath);
			m_pPreviewBox->AddImage(curPath);
		}
	}
	delete[] buff;

	// TODO:触发预览区，滚动条重绘
	CRect rect;
	m_pPreviewBox->GetWindowRect(&rect);
	m_pPreviewBox->MoveWindow( 0, 0, rect.Width() + 1, rect.Height());
	m_pPreviewBox->MoveWindow( 0, 0, rect.Width(), rect.Height());
	Invalidate(FALSE);
	UpdateWindow();
	//CRect rect;
	//m_pPreviewBox->GetClientRect(&rect);
	//m_pPreviewBox->SetWindowPos(NULL, 0, 0, rect.Width() + 1, rect.Height(), SWP_NOMOVE);

}


void CPictureLoaderDlg::OnBnConfig()
{
	CSettingDlg dlg;
	if (IDOK == dlg.DoModal()) {
		m_nLoadCnt = dlg.m_nLoadCnt;
		m_pPreviewBox->m_nLoadCnt = dlg.m_nLoadCnt;
	}
}


BOOL CPictureLoaderDlg::InitComboBox()
{
	m_arrWebsiteIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_arrWebsiteIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON2);
	m_arrWebsiteIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON3);

	m_lstIcon.Create(16, 16, ILC_COLOR32, 5, 5);
	m_lstIcon.Add(m_arrWebsiteIcon[0]);
	m_lstIcon.Add(m_arrWebsiteIcon[1]);
	m_lstIcon.Add(m_arrWebsiteIcon[2]);

	m_comboSite.SetImageList(&m_lstIcon);

	LPTSTR txt[MAX_WEBSITE] = { _T("Konachan"), _T("Pixiv"), _T("Yandere") };
	COMBOBOXEXITEM ComboListItem;
	ComboListItem.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	for (int i = 0; i < MAX_WEBSITE; i++) {
		ComboListItem.iItem = i;
		ComboListItem.pszText = txt[i];
		ComboListItem.cchTextMax = sizeof(txt[i]);
		ComboListItem.iImage = i;
		ComboListItem.iSelectedImage = i;
		ComboListItem.iIndent = 0;
		m_comboSite.InsertItem(&ComboListItem);
	}
	m_comboSite.SetCurSel(0);

	return 0;
}


BOOL CPictureLoaderDlg::GetSettings()
{
	CString  strConfigPath;
	GetModuleFileName(NULL, strConfigPath.GetBuffer(256), 256);
	strConfigPath.ReleaseBuffer(256);
	int nPos = strConfigPath.ReverseFind('\\');
	strConfigPath = strConfigPath.Left(nPos + 1) + CONFIG_FILE_PATH;

	TCHAR buf[1024];
	GetPrivateProfileString(SECTION_MISC, KEY_DOWNLOAD_PATH, _T("C:\\") + DEFAULT_DOWNLOAD_PATH, buf, 1024, strConfigPath);
	m_strDownloadPath.Format(_T("%s"), buf);
	m_nLoadCnt = GetPrivateProfileInt(SECTION_DISPLAY, KEY_MAX_LOAD_CNT, DEFAULT_MAX_LOAD_CNT, strConfigPath);

	return TRUE;
}


void CPictureLoaderDlg::OnBnNextPage()
{
	if (((m_nCurShowPage + 1) * m_nLoadCnt) < m_pPreviewBox->GetSize()) {
		m_nCurShowPage++;
		m_pPreviewBox->m_nCurShowPage++;

		m_btnPrevious.EnableWindow(TRUE);

		Invalidate(TRUE);
		UpdateWindow();
	}
	else {
		m_btnNext.EnableWindow(FALSE);
	}
}


void CPictureLoaderDlg::OnBnPreviousPage()
{
	if (m_nCurShowPage > 0) {
		m_nCurShowPage--;
		m_pPreviewBox->m_nCurShowPage--;

		m_btnNext.EnableWindow(TRUE);

		Invalidate(TRUE);
		UpdateWindow();
	}
	else {
		m_btnPrevious.EnableWindow(FALSE);
	}
}
