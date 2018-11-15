// FovDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ENDOBENCH_VS2015.h"
#include "ENDOBENCH_VS2015Dlg.h"
#include "FovDlg.h"
#include "afxdialogex.h"


// FovDlg 对话框

IMPLEMENT_DYNAMIC(FovDlg, CDialogEx)

FovDlg::FovDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FOV, pParent)
	, d1(0)
	, d2(0)
	, rfov(0)
	, a(0)
	, vfov(0)
	, dov(0)
{

}

FovDlg::~FovDlg()
{
}

void FovDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, d1);
	DDX_Text(pDX, IDC_EDIT2, d2);
	DDX_Text(pDX, IDC_EDIT3, rfov);
	DDX_Text(pDX, IDC_EDIT4, a);
	DDX_Text(pDX, IDC_EDIT5, vfov);
	DDX_Text(pDX, IDC_EDIT6, dov);
}


BEGIN_MESSAGE_MAP(FovDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &FovDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// FovDlg 消息处理程序


void FovDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	// 读取D1 D2
	//CString file_path("C:\\Users\\Simple\\Desktop\\ENDOBENCH_VS2015\\ENDOBENCH_VS2015\\x64\\Debug\\config.ini");
	CString strD1('1',256);
	GetPrivateProfileString(_T("FOV"), _T("D1"), _T("25"), strD1.GetBuffer(0), 256, config_path);
	D1 = _wtof(strD1);
	CString strD2('1', 256);
	GetPrivateProfileString(_T("FOV"), _T("D2"), _T("50"), strD2.GetBuffer(0), 256, config_path);
	D2 = _wtof(strD2);

	// 计算其他值
	rfov = 2*atan((D2 - D1) / 2.0 / (d2 - d1));
	a = (D1*d2 - D2*d1) / (D2 - D1);
	vfov = 2 * atan((100 + 2 * a)*tan(rfov / 2) / 100);
	UpdateData(FALSE);
	UpdateTreeControl();
}

void FovDlg::UpdateTreeControl()
{
	auto pmain_window = (CENDOBENCH_VS2015Dlg*)GetParent();
	auto &fov_ref = pmain_window->fov_dlg;
	auto& tree_control = pmain_window->tree_control;
	//查找FOV的第一个节点
	HTREEITEM hRoot = tree_control.GetRootItem();
	hRoot = tree_control.GetChildItem(hRoot);
	while (tree_control.GetItemData(hRoot) != FOV)
	{
		hRoot = tree_control.GetNextSiblingItem(hRoot);
	}
	HTREEITEM hChild = tree_control.GetChildItem(hRoot); //hChild 为FOV 的第一个节点

														  //设置FOV的节点值
	CString  tempValue;
	CString  text;
	tempValue.Format(_T("%f"), 0);

	//d1
	text.Append(_T("Real Fov d1: "));
	tempValue.Format(_T("%f"), fov_ref.d1);
	text += tempValue;
	text.Append(_T(" mm"));
	tree_control.SetItemText(hChild, text);

	//d2
	text = (_T(""));
	hChild = tree_control.GetNextSiblingItem(hChild);
	text.Append(_T("Real Fov d2: "));
	tempValue.Format(_T("%f"), fov_ref.d2);
	text += tempValue;
	text.Append(_T(" mm"));
	tree_control.SetItemText(hChild, text);


	//RFOV
	text = (_T(""));
	hChild = tree_control.GetNextSiblingItem(hChild);
	text.Append(_T("Real Fov: "));
	tempValue.Format(_T("%f"), fov_ref.rfov);
	text += tempValue;
	text.Append(_T(" deg"));
	tree_control.SetItemText(hChild, text);

	//a
	text = (_T(""));
	hChild = tree_control.GetNextSiblingItem(hChild);
	text.Append(_T("Distance between P&V: "));
	tempValue.Format(_T("%f"), fov_ref.a);
	text += tempValue;
	text.Append(_T(" mm"));
	tree_control.SetItemText(hChild, text);

	//VFOV
	text = (_T(""));
	hChild = tree_control.GetNextSiblingItem(hChild);
	text.Append(_T("Vertex FOV: "));
	tempValue.Format(_T("%f"), fov_ref.vfov);
	text += tempValue;
	text.Append(_T(" deg"));
	tree_control.SetItemText(hChild, text);

	//DOV
	text = (_T(""));
	hChild = tree_control.GetNextSiblingItem(hChild);
	text.Append(_T("Direction Of View: "));
	tempValue.Format(_T("%f"), fov_ref.dov);
	text += tempValue;
	text.Append(_T(" deg"));
	tree_control.SetItemText(hChild, text);
}
