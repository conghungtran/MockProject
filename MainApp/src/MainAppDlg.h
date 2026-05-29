
// MainAppDlg.h : header file
//

#pragma once


// CMainAppDlg dialog
class CMainAppDlg : public CDialogEx
{
// Construction
public:
	CMainAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINAPP_DIALOG };
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
	afx_msg void OnBnClickedButton2();
	CListCtrl m_ListCtrl;
	afx_msg void OnLvnItemchangedList4(NMHDR* pNMHDR, LRESULT* pResult);
};
