
// PictureLoaderDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "PreviewBox.h"
#include "CDlgSetting.h"
#include "afxtempl.h"
#include "ConfigOptions.h"

const UINT MIN_DIALOG_WIDTH = 900;		// 主界面最小缩放尺寸
const UINT MIN_DIALOG_HEIGHT = 533;
const UINT MAX_WEBSITE = 3;				// ComboBox icon数目

// CPictureLoaderDlg dialog
class CPictureLoaderDlg : public CDialogEx
{
// Construction
public:
	CPictureLoaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICTURELOADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CPreviewBox *m_pPreviewBox;
	CButton m_btnPrevious;
	CButton m_btnNext;
	afx_msg void OnBnSearch();
	afx_msg void OnBnConfig();
	BOOL InitComboBox();
	CComboBoxEx m_comboSite;
protected:
	HICON m_arrWebsiteIcon[MAX_WEBSITE];
	CImageList m_lstIcon;
public:
	int m_nLoadCnt;
	BOOL GetSettings();
	CString m_strDownloadPath;
	int m_nCurShowPage;
	afx_msg void OnBnNextPage();
	afx_msg void OnBnPreviousPage();
};
