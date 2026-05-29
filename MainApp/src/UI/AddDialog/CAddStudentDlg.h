#pragma once
#include "afxdialogex.h"


// CAddStudentDlg dialog

class CAddStudentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddStudentDlg)

public:
	CAddStudentDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddStudentDlg();

	// Biến lưu dữ liệu nhập từ Dialog con
	CString m_strMaSo;
	CString m_strHoTen;
	CString m_strNgaySinh;
	double  m_dDiemTB;
	CString m_strXepLoai;  // Có thể tự tính hoặc cho nhập

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
