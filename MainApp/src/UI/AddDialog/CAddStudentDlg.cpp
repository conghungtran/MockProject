// CAddStudentDlg.cpp : implementation file
//

#include "pch.h"
#include "MainApp.h"
#include "afxdialogex.h"
#include "CAddStudentDlg.h"


// CAddStudentDlg dialog

IMPLEMENT_DYNAMIC(CAddStudentDlg, CDialogEx)

CAddStudentDlg::CAddStudentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
{

}

CAddStudentDlg::~CAddStudentDlg()
{
}

void CAddStudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddStudentDlg, CDialogEx)

	ON_BN_CLICKED(IDOK, &CAddStudentDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddStudentDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddStudentDlg message handlers

void CAddStudentDlg::OnBnClickedOk()
{
    // Cập nhật dữ liệu từ controls vào biến
    UpdateData(TRUE);

    // Kiểm tra dữ liệu nhập (validation)
    if (m_strMaSo.IsEmpty())
    {
        AfxMessageBox(_T("Vui lòng nhập Mã số!"));
        return;
    }
    if (m_strHoTen.IsEmpty())
    {
        AfxMessageBox(_T("Vui lòng nhập Họ tên!"));
        return;
    }
    if (m_dDiemTB < 0 || m_dDiemTB > 10)
    {
        AfxMessageBox(_T("Điểm phải từ 0 đến 10!"));
        return;
    }

    // Tự động tính xếp loại nếu chưa nhập
    if (m_strXepLoai.IsEmpty())
    {
        if (m_dDiemTB >= 9.0)
            m_strXepLoai = _T("Xuất sắc");
        else if (m_dDiemTB >= 8.0)
            m_strXepLoai = _T("Giỏi");
        else if (m_dDiemTB >= 6.5)
            m_strXepLoai = _T("Khá");
        else if (m_dDiemTB >= 5.0)
            m_strXepLoai = _T("Trung bình");
        else
            m_strXepLoai = _T("Yếu");
    }

    // Đóng dialog với kết quả OK
    CDialogEx::OnOK();
}

void CAddStudentDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
