
// MainAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MainApp.h"
#include "MainAppDlg.h"
#include "afxdialogex.h"
#include "./UI/AddDialog/CAddStudentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>


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
    afx_msg void OnBnClickedButtonAdd();  // Thêm dòng này

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


// CMainAppDlg dialog



CMainAppDlg::CMainAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAINAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CMainAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMainAppDlg::OnBnClickedButton2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &CMainAppDlg::OnLvnItemchangedList4)

END_MESSAGE_MAP()


// CMainAppDlg message handlers
BOOL CMainAppDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ========== 1. THIẾT LẬP STYLE NÂNG CAO ==========
    m_ListCtrl.SetExtendedStyle(
        m_ListCtrl.GetExtendedStyle()
        | LVS_EX_GRIDLINES
        | LVS_EX_FULLROWSELECT
        | LVS_EX_HEADERDRAGDROP
        | LVS_EX_CHECKBOXES
    );


    // ========== 2. TẠO CÁC CỘT (PHẢI LÀM TRƯỚC) ==========
    m_ListCtrl.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 50);
    m_ListCtrl.InsertColumn(1, _T("Model"), LVCFMT_CENTER, 100);
    m_ListCtrl.InsertColumn(2, _T("Brand"), LVCFMT_LEFT, 120);
    m_ListCtrl.InsertColumn(3, _T("Type"), LVCFMT_CENTER, 100);
    m_ListCtrl.InsertColumn(4, _T("Price ($)"), LVCFMT_CENTER, 90);
    m_ListCtrl.InsertColumn(5, _T("Status"), LVCFMT_CENTER, 100);

    // ========== 3. THÊM DỮ LIỆU ==========
    // Printer 1
    int nRow = m_ListCtrl.InsertItem(0, _T("1"));
    m_ListCtrl.SetItemText(nRow, 1, _T("LaserJet Pro M15w"));
    m_ListCtrl.SetItemText(nRow, 2, _T("HP"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Laser"));
    m_ListCtrl.SetItemText(nRow, 4, _T("129.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("In Stock"));

    // Printer 2
    nRow = m_ListCtrl.InsertItem(1, _T("2"));
    m_ListCtrl.SetItemText(nRow, 1, _T("EcoTank L3110"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Epson"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Ink Tank"));
    m_ListCtrl.SetItemText(nRow, 4, _T("249.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("In Stock"));

    // Printer 3
    nRow = m_ListCtrl.InsertItem(2, _T("3"));
    m_ListCtrl.SetItemText(nRow, 1, _T("Pixma G6020"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Canon"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Ink Tank"));
    m_ListCtrl.SetItemText(nRow, 4, _T("279.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("Low Stock"));

    // Printer 4
    nRow = m_ListCtrl.InsertItem(3, _T("4"));
    m_ListCtrl.SetItemText(nRow, 1, _T("WorkCentre 6515"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Xerox"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Multifunction"));
    m_ListCtrl.SetItemText(nRow, 4, _T("399.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("In Stock"));

    // Printer 5
    nRow = m_ListCtrl.InsertItem(4, _T("5"));
    m_ListCtrl.SetItemText(nRow, 1, _T("Colorio PX-S160T"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Brother"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Inkjet"));
    m_ListCtrl.SetItemText(nRow, 4, _T("189.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("Out of Stock"));

    // Printer 6
    nRow = m_ListCtrl.InsertItem(5, _T("6"));
    m_ListCtrl.SetItemText(nRow, 1, _T("DesignJet T210"));
    m_ListCtrl.SetItemText(nRow, 2, _T("HP"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Plotter"));
    m_ListCtrl.SetItemText(nRow, 4, _T("1199.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("In Stock"));

    // Printer 7
    nRow = m_ListCtrl.InsertItem(6, _T("7"));
    m_ListCtrl.SetItemText(nRow, 1, _T("SureColor P700"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Epson"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Photo Printer"));
    m_ListCtrl.SetItemText(nRow, 4, _T("899.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("In Stock"));

    // Printer 8
    nRow = m_ListCtrl.InsertItem(7, _T("8"));
    m_ListCtrl.SetItemText(nRow, 1, _T("LBP622dw"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Canon"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Laser"));
    m_ListCtrl.SetItemText(nRow, 4, _T("159.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("Low Stock"));

    // Printer 9
    nRow = m_ListCtrl.InsertItem(8, _T("9"));
    m_ListCtrl.SetItemText(nRow, 1, _T("HL-L2350DW"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Brother"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Laser"));
    m_ListCtrl.SetItemText(nRow, 4, _T("139.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("In Stock"));

    // Printer 10
    nRow = m_ListCtrl.InsertItem(9, _T("10"));
    m_ListCtrl.SetItemText(nRow, 1, _T("VersaLink C405"));
    m_ListCtrl.SetItemText(nRow, 2, _T("Xerox"));
    m_ListCtrl.SetItemText(nRow, 3, _T("Multifunction"));
    m_ListCtrl.SetItemText(nRow, 4, _T("649.99"));
    m_ListCtrl.SetItemText(nRow, 5, _T("Out of Stock"));

    // ========== PHẦN MENU VÀ ICON (GIỮ NGUYÊN) ==========
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
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

    // Set the icon for this dialog.
    SetIcon(m_hIcon, TRUE);   // Set big icon
    SetIcon(m_hIcon, FALSE);  // Set small icon

    return TRUE;
}
void CMainAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMainAppDlg::OnPaint()
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
HCURSOR CMainAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMainAppDlg::OnBnClickedButton2()
{
    // Tạo đối tượng Dialog con
    CAddStudentDlg dlg;

    // Hiển thị Dialog và chờ người dùng nhập
    if (dlg.DoModal() == IDOK)  // Nếu người dùng bấm OK
    {
        // Lấy dữ liệu từ Dialog con
        CString strMaSo = dlg.m_strMaSo;
        CString strHoTen = dlg.m_strHoTen;
        CString strNgaySinh = dlg.m_strNgaySinh;
        double dDiem = dlg.m_dDiemTB;
        CString strXepLoai = dlg.m_strXepLoai;

        // Tính số thứ tự mới
        int nNewSTT = m_ListCtrl.GetItemCount() + 1;
        CString strSTT;
        strSTT.Format(_T("%d"), nNewSTT);

        // Thêm vào List Control
        int nRow = m_ListCtrl.InsertItem(nNewSTT - 1, strSTT);
        m_ListCtrl.SetItemText(nRow, 1, strMaSo);
        m_ListCtrl.SetItemText(nRow, 2, strHoTen);
        m_ListCtrl.SetItemText(nRow, 3, strNgaySinh);

        CString strDiem;
        strDiem.Format(_T("%.1f"), dDiem);
        m_ListCtrl.SetItemText(nRow, 4, strDiem);
        m_ListCtrl.SetItemText(nRow, 5, strXepLoai);

        // Thông báo thành công
        CString msg;
        msg.Format(_T("Đã thêm sinh viên: %s - %s"), strMaSo, strHoTen);
        AfxMessageBox(msg);
    }
    else  // Người dùng bấm Cancel
    {
        AfxMessageBox(_T("Đã hủy thêm sinh viên!"));
    }
}
void CMainAppDlg::OnLvnItemchangedList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
