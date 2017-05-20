#pragma once
#include "AboutStatic.h"

const UINT MIN_LOAD_CNT = 4;
const UINT MAX_LOAD_CNT = 25;
// CdlgSetting dialog

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	UINT m_nLoadCnt;
	CString m_strSavePath;
	CStatic m_ctlCnt;
	CStatic m_staWarn;
	CAboutStatic *m_staAbout;

	afx_msg void OnBnAddOne();
	afx_msg void OnBnMinusOne();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnSetfocusEditLoadcnt();
	afx_msg void OnEnUpdateEditLoadcnt();
	afx_msg void OnEnChangeEditLoadcnt();
	afx_msg void OnBnSaveSetting();
	afx_msg void OnBnLoadDefault();
	BOOL GetCurrentConfig();
	afx_msg void OnBnChgPath();
	afx_msg void OnEnKillfocusEditLoadcnt();
};
