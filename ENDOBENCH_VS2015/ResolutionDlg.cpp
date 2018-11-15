// ResolutionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ENDOBENCH_VS2015.h"
#include "ENDOBENCH_VS2015Dlg.h"
#include "ResolutionDlg.h"
#include "afxdialogex.h"


// ResolutionDlg 对话框

IMPLEMENT_DYNAMIC(ResolutionDlg, CDialogEx)

ResolutionDlg::ResolutionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Resolution, pParent)
	, or_center(0)
	, or_first(0)
	, or_second(0)
	, or_third(0)
	, or_fourth(0)
	, or_avg(0)
	, or_min(0)
	, or_stddev(0)
	, or_d(0)

	, ar_center(0)
	, ar_first(0)
	, ar_second(0)
	, ar_third(0)
	, ar_fourth(0)
	, ar_avg(0)
	, ar_min(0)
	, ar_stddev(0)
	, ar_d(0)
{

}

ResolutionDlg::~ResolutionDlg()
{
}

void ResolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, or_center);
	DDX_Text(pDX, IDC_EDIT2, or_first);
	DDX_Text(pDX, IDC_EDIT3, or_second);
	DDX_Text(pDX, IDC_EDIT4, or_third);
	DDX_Text(pDX, IDC_EDIT5, or_fourth);
	DDX_Text(pDX, IDC_EDIT6, or_avg);
	DDX_Text(pDX, IDC_EDIT7, or_min);
	DDX_Text(pDX, IDC_EDIT8, or_stddev);
	DDX_Text(pDX, IDC_EDIT9, or_d);

	DDX_Text(pDX, IDC_EDIT10, ar_center);
	DDX_Text(pDX, IDC_EDIT11, ar_first);
	DDX_Text(pDX, IDC_EDIT12, ar_second);
	DDX_Text(pDX, IDC_EDIT13, ar_third);
	DDX_Text(pDX, IDC_EDIT14, ar_fourth);
	DDX_Text(pDX, IDC_EDIT15, ar_avg);
	DDX_Text(pDX, IDC_EDIT16, ar_min);
	DDX_Text(pDX, IDC_EDIT17, ar_stddev);
	DDX_Text(pDX, IDC_EDIT18, ar_d);
}


BEGIN_MESSAGE_MAP(ResolutionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &ResolutionDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// ResolutionDlg 消息处理程序


void ResolutionDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	auto pmain = (CENDOBENCH_VS2015Dlg*)GetParent();
	double x = pmain->fov_dlg.a;
	 
}
