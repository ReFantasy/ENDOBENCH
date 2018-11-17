// Distortion.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ENDOBENCH_VS2015.h"
#include "ENDOBENCH_VS2015Dlg.h"
#include "Distortion.h"
#include "afxdialogex.h"
#include "camera.h"
#include "CirclrDistortion.h"
#include "Auxilib.h"


// Distortion �Ի���

IMPLEMENT_DYNAMIC(DistortionDlg, CDialogEx)

DistortionDlg::DistortionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Distortion, pParent)
	, Top(0)
	, Right(0)
	, Bottom(0)
	, Left(0)
	, Avg(0)
	, Max(0)
	, StdDev(0)
{
	// ��ʼ�����Բ������
	ReadCircleCrd();
}

DistortionDlg::~DistortionDlg()
{
}

void DistortionDlg::ReadCircleCrd()
{
	
	//CString file_path("C:\\Users\\Simple\\Desktop\\ENDOBENCH_VS2015\\ENDOBENCH_VS2015\\x64\\Debug\\config.ini");
	CString str_value('1', 256);

	GetPrivateProfileString(_T("Distortion"), _T("top_x"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	top_x = atof(str_value);
	GetPrivateProfileString(_T("Distortion"), _T("top_y"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	top_y = atof(str_value);

	GetPrivateProfileString(_T("Distortion"), _T("bottom_x"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	bottom_x = atof(str_value);
	GetPrivateProfileString(_T("Distortion"), _T("bottom_y"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	bottom_y = atof(str_value);

	GetPrivateProfileString(_T("Distortion"), _T("left_x"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	left_x = atof(str_value);
	GetPrivateProfileString(_T("Distortion"), _T("left_y"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	left_y = atof(str_value);

	GetPrivateProfileString(_T("Distortion"), _T("right_x"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	right_x = atof(str_value);
	GetPrivateProfileString(_T("Distortion"), _T("right_y"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	right_y = atof(str_value);

	GetPrivateProfileString(_T("Distortion"), _T("center_x"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	center_x = atof(str_value);
	GetPrivateProfileString(_T("Distortion"), _T("center_y"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	center_y = atof(str_value);

	GetPrivateProfileString(_T("Distortion"), _T("circle_radius"), _T("25"), str_value.GetBuffer(0), 256, config_path);
	circle_radius = atof(str_value);

}

void DistortionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Top);
	DDX_Text(pDX, IDC_EDIT2, Right);
	DDX_Text(pDX, IDC_EDIT3, Bottom);
	DDX_Text(pDX, IDC_EDIT4, Left);
	DDX_Text(pDX, IDC_EDIT5, Avg);
	DDX_Text(pDX, IDC_EDIT6, Max);
	DDX_Text(pDX, IDC_EDIT7, StdDev);
}


double DistortionDlg::ComputeDiameter(const cv::Mat _img, int dic)
{
	// �趨��ֵ
	Mat img;
	_img.copyTo(img);
	int yz = 100;
	cvtColor(img, img, CV_RGB2GRAY);
	int Diameter = 0;
	
	if (dic == 1)
	{
		int low = img.rows-1, high = 0;

		for (int i = 0; i < img.rows; i++)
		{
			
			for (int j = 0; j < img.cols; j++)
			{
				int tmp = img.at<uchar>(i, j);
				if (tmp < yz)
				{
					if (low > i)low = i;
					if (high < i)high = i;
				}
			}
		}

		Diameter = high - low;
	}

	if (dic == 2)
	{
		int low = img.cols - 1, high = 0;

		for (int i = 0; i < img.rows; i++)
		{
			
			for (int j = 0; j < img.cols; j++)
			{
				int tmp = img.at<uchar>(i, j);
				if (tmp < yz)
				{
					/*low = (low < j ? low : j);
					high = (high > j ? high : j);*/
					if (low > j)low = j;
					if (high < j)high = j;
				}
			}
		}

		Diameter = high - low;
	}
	
	return Diameter;
}

void DistortionDlg::UpdateTreeControl()
{
	auto pmainaindow = (CENDOBENCH_VS2015Dlg*)GetParent();
	auto &distortion_ref = pmainaindow->distortion_dlg;
	auto& tree_control = pmainaindow->tree_control;
	//����Resolution�ĵ�һ���ڵ�
	HTREEITEM hRoot = tree_control.GetRootItem();
	hRoot = tree_control.GetChildItem(hRoot);
	while (tree_control.GetItemData(hRoot) != Distortion)
	{
		hRoot = tree_control.GetNextSiblingItem(hRoot);
	}
	HTREEITEM hChild = tree_control.GetChildItem(hRoot); //hChild ΪResolution �ĵ�һ���ڵ�

														  //����Distortion�Ľڵ�ֵ
	CString  Value;
	Value.Format(_T("%.2f"), 0);

	//Top
	Value.Format(_T("%.2f"), Top * 100);
	tree_control.SetItemText(hChild, CString("Distortion Top: ") + Value + "%");

	//Right
	hChild = tree_control.GetNextSiblingItem(hChild);
	Value.Format(_T("%.2f"), Right * 100);
	tree_control.SetItemText(hChild, CString("Distortion Right: ") + Value + "%");

	//Bottom
	hChild = tree_control.GetNextSiblingItem(hChild);
	Value.Format(_T("%.2f"), Bottom * 100);
	tree_control.SetItemText(hChild, CString("Distortion Bottom: ") + Value + "%");

	//Left
	hChild = tree_control.GetNextSiblingItem(hChild);
	Value.Format(_T("%.2f"), Left * 100);
	tree_control.SetItemText(hChild, CString("Distortion Left: ") + Value + "%");

	//Avg
	hChild = tree_control.GetNextSiblingItem(hChild);
	Value.Format(_T("%.2f"), Avg * 100);
	tree_control.SetItemText(hChild, CString("Distortion Avg: ") + Value + "%");

	//Max
	hChild = tree_control.GetNextSiblingItem(hChild);
	Value.Format(_T("%.2f"), Max * 100);
	tree_control.SetItemText(hChild, CString("Distortion Max: ") + Value + "%");

	//Std Dev
	hChild = tree_control.GetNextSiblingItem(hChild);
	Value.Format(_T("%.2f"), StdDev * 100);
	tree_control.SetItemText(hChild, CString("Distortion Std Dev: ") + Value + "%");
}

BEGIN_MESSAGE_MAP(DistortionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &DistortionDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Distortion ��Ϣ�������

void DistortionDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!SingletonCamera::GetInstance()->IsConnected())
		return;

	CDC *pDC = this->GetParent()->GetDlgItem(IDC_PicControl)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	this->GetParent()->GetDlgItem(IDC_PicControl)->GetClientRect(&rect);
	CRect VideoRect(0, 0, rect.Width(), rect.Height());

	// ��ȡ�����Ƭ
	SingletonCamera::image_type raw_image;
	SingletonCamera::GetInstance()->RetrieveBuffer(&raw_image);
	cv::Mat cv_image;
	ConvertToMatImage(raw_image, cv_image);

	
	//// ��ȡ���������Բ
	auto GetDiameter = [&](int x, int y, double radius, int dic)
	{ 
		auto rect = MapToRawImage(CRect(x - radius, y - radius, x + radius, y + radius)
			, VideoRect);
		auto roi_image = cv_image(rect);
		auto diameter = ComputeDiameter(roi_image, dic);
		return diameter;
	};

	auto diameter_top = GetDiameter(top_x, top_y, circle_radius, 1);
	auto diameter_bottom = GetDiameter(bottom_x, bottom_y, circle_radius, 1);
	auto diameter_left = GetDiameter(left_x, left_y, circle_radius, 2);
	auto diameter_right = GetDiameter(right_x, right_y, circle_radius, 2);

	auto diameter_1 = GetDiameter(center_x, center_y, circle_radius, 1);
	auto diameter_2 = GetDiameter(center_x, center_y, circle_radius, 2);

	// ����������
	Top = abs(diameter_top - diameter_1) / diameter_1;
	Bottom = abs(diameter_bottom - diameter_1) / diameter_1;
	Left = abs(diameter_left - diameter_2) / diameter_2;
	Right = abs(diameter_right - diameter_2) / diameter_2;
	Avg = (Top + Bottom + Left + Right) / 4;
	Max = max(Top, max(Bottom, max(Left, Right)));
	StdDev = sqrt((pow((Top - Avg), 2) + pow((Bottom - Avg), 2) + pow((Left - Avg), 2) + pow((Right - Avg), 2)) / 4);
	UpdateData(FALSE);
	UpdateTreeControl();
}
