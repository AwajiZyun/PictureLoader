// CdlgSetting.cpp : implementation file
//

#include "stdafx.h"
#include "PictureLoader.h"
#include "CdlgSetting.h"
#include "afxdialogex.h"
#include "ConfigOptions.h"


// CdlgSetting dialog

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING_DIALOG, pParent)
	, m_strSavePath(_T("C:\\"))
	, m_nLoadCnt(DEFAULT_MAX_LOAD_CNT)
{

}

CSettingDlg::~CSettingDlg()
{
	delete m_staAbout;
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOADCNT, m_ctlCnt);
	DDX_Control(pDX, IDC_STATIC_WARNING, m_staWarn);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strSavePath);
	DDX_Text(pDX, IDC_EDIT_LOADCNT, m_nLoadCnt);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CSettingDlg::OnBnAddOne)
	ON_BN_CLICKED(IDC_BUTTON3, &CSettingDlg::OnBnMinusOne)
	ON_WM_CREATE()
	ON_EN_SETFOCUS(IDC_EDIT_LOADCNT, &CSettingDlg::OnEnSetfocusEditLoadcnt)
	ON_EN_UPDATE(IDC_EDIT_LOADCNT, &CSettingDlg::OnEnUpdateEditLoadcnt)
	ON_EN_CHANGE(IDC_EDIT_LOADCNT, &CSettingDlg::OnEnChangeEditLoadcnt)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnSaveSetting)
	ON_BN_CLICKED(IDC_BUTTON1, &CSettingDlg::OnBnLoadDefault)
	ON_BN_CLICKED(IDC_BUTTON4, &CSettingDlg::OnBnChgPath)
	ON_EN_KILLFOCUS(IDC_EDIT_LOADCNT, &CSettingDlg::OnEnKillfocusEditLoadcnt)
END_MESSAGE_MAP()


// CdlgSetting message handlers


void CSettingDlg::OnBnAddOne()
{
	UpdateData(TRUE);
	m_nLoadCnt++;
	if ((MIN_LOAD_CNT > m_nLoadCnt) ){
		m_nLoadCnt = MIN_LOAD_CNT;
	}
	else if (m_nLoadCnt > MAX_LOAD_CNT) {
		m_nLoadCnt = MAX_LOAD_CNT;
	}
	UpdateData(FALSE);
}


void CSettingDlg::OnBnMinusOne()
{
	UpdateData(TRUE);
	m_nLoadCnt--;
	if ((MIN_LOAD_CNT > m_nLoadCnt)) {
		m_nLoadCnt = MIN_LOAD_CNT;
	}
	else if (m_nLoadCnt > MAX_LOAD_CNT) {
		m_nLoadCnt = MAX_LOAD_CNT;
	}
	UpdateData(FALSE);
}


int CSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_staAbout = new CAboutStatic;
	CRect rect(0, 0, 100, 20);
	m_staAbout->Create(_T("关于..."), WS_CHILD | WS_VISIBLE, rect, this);
	m_staAbout->ModifyStyle(0, SS_NOTIFY);

	// 读取配置文件
	GetCurrentConfig();


	return 0;
}


void CSettingDlg::OnEnSetfocusEditLoadcnt()
{
	CString warnMsg;
	warnMsg.Format(_T("设定范围:%d~%d"), MIN_LOAD_CNT, MAX_LOAD_CNT);
	m_staWarn.SetWindowText(warnMsg.GetString());
}


void CSettingDlg::OnEnUpdateEditLoadcnt()
{
	//UpdateData(TRUE);
	//if ((MIN_LOAD_CNT > m_nLoadCnt) || (m_nLoadCnt > MAX_LOAD_CNT)) {
	//	CString warnMsg;
	//	warnMsg.Format(_T("设定范围超过！设定范围:%d~%d"), MIN_LOAD_CNT, MAX_LOAD_CNT);
	//	m_staWarn.SetWindowText(warnMsg.GetString());
	//}
}


void CSettingDlg::OnEnChangeEditLoadcnt()
{
	//UpdateData(TRUE);
	//if ((MIN_LOAD_CNT > m_nLoadCnt) || (m_nLoadCnt > MAX_LOAD_CNT)) {
	//	CString warnMsg;
	//	warnMsg.Format(_T("设定范围超过！设定范围:%d~%d"), MIN_LOAD_CNT, MAX_LOAD_CNT);
	//	m_staWarn.SetWindowText(warnMsg.GetString());
	//}
	//else {
	//	CString warnMsg;
	//	warnMsg.Format(_T("设定范围:%d~%d"), MIN_LOAD_CNT, MAX_LOAD_CNT);
	//	m_staWarn.SetWindowText(warnMsg.GetString());
	//}
}

// 保存当前设置
void CSettingDlg::OnBnSaveSetting()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString  strPathName;
	GetModuleFileName(NULL, strPathName.GetBuffer(256), 256);
	strPathName.ReleaseBuffer(256);
	int nPos = strPathName.ReverseFind('\\');
	strPathName = strPathName.Left(nPos + 1) + CONFIG_FILE_PATH;
	CString loadFileCnt;
	loadFileCnt.Format(_T("%d"), m_nLoadCnt);
	WritePrivateProfileString(SECTION_DISPLAY, KEY_MAX_LOAD_CNT, loadFileCnt, strPathName);
	WritePrivateProfileString(SECTION_MISC, KEY_DOWNLOAD_PATH, m_strSavePath, strPathName);

	CDialogEx::OnOK();
}

// 恢复默认设置
void CSettingDlg::OnBnLoadDefault()
{
	CString  strCurPath;
	GetModuleFileName(NULL, strCurPath.GetBuffer(256), 256);
	strCurPath.ReleaseBuffer(256);
	int nPos = strCurPath.ReverseFind('\\');
	strCurPath = strCurPath.Left(nPos + 1) ;
	//CString loadFileCnt;
	//loadFileCnt.Format(_T("%d"), DEFAULT_MAX_LOAD_CNT);

	//WritePrivateProfileString(SECTION_DISPLAY, KEY_MAX_LOAD_CNT, loadFileCnt, strCurPath + CONFIG_FILE_PATH);
	//WritePrivateProfileString(SECTION_MISC, KEY_DOWNLOAD_PATH, strCurPath + DEFAULT_DOWNLOAD_PATH, strCurPath + CONFIG_FILE_PATH);

	m_strSavePath = strCurPath + DEFAULT_DOWNLOAD_PATH;
	m_nLoadCnt = DEFAULT_MAX_LOAD_CNT;
	UpdateData(FALSE);
}

// 读取设置文件
BOOL CSettingDlg::GetCurrentConfig()
{
	CString  strConfigPath;
	GetModuleFileName(NULL, strConfigPath.GetBuffer(256), 256);
	strConfigPath.ReleaseBuffer(256);
	int nPos = strConfigPath.ReverseFind('\\');
	strConfigPath = strConfigPath.Left(nPos + 1) + CONFIG_FILE_PATH;

	TCHAR buf[1024];
	GetPrivateProfileString(SECTION_MISC, KEY_DOWNLOAD_PATH, _T("C:\\") + DEFAULT_DOWNLOAD_PATH, buf, 1024, strConfigPath);
	m_strSavePath.Format(_T("%s"), buf);
	m_nLoadCnt = GetPrivateProfileInt(SECTION_DISPLAY, KEY_MAX_LOAD_CNT, DEFAULT_MAX_LOAD_CNT,strConfigPath);
	//UpdateData(FALSE);

	return 0;
}


void CSettingDlg::OnBnChgPath()
{
	CFolderPickerDialog chgPath(NULL, 0, this, 0);
	if (IDOK == chgPath.DoModal()) {
		m_strSavePath = chgPath.GetPathName();
		UpdateData(FALSE);
	}
}


void CSettingDlg::OnEnKillfocusEditLoadcnt()
{
	UpdateData(TRUE);
}
