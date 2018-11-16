
// ENDOBENCH_VS2015Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ENDOBENCH_VS2015.h"
#include "ENDOBENCH_VS2015Dlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "Auxilib.h"
#include <thread>
#include "camera.h"
#include "ComCorrect.h"

CString config_path("C:\\Users\\Simple\\Desktop\\ENDOBENCH_VS2015\\ENDOBENCH_VS2015\\x64\\Debug\\config.ini");

itas109::CSerialPort serial_port;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CENDOBENCH_VS2015Dlg �Ի���



CENDOBENCH_VS2015Dlg::CENDOBENCH_VS2015Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENDOBENCH_VS2015_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CENDOBENCH_VS2015Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, tree_control);
	DDX_Control(pDX, IDC_STATIC_CURVE, m_StcCurve);
	DDX_Control(pDX, IDC_BUTTON1, btn_SelPortOpenCloseCtrl);
	DDX_Control(pDX, IDC_EDIT1, edit_SelPortNO);
	DDX_Control(pDX, IDC_EDIT2, m_ReceiveCtrl);
}

BEGIN_MESSAGE_MAP(CENDOBENCH_VS2015Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnTest, &CENDOBENCH_VS2015Dlg::OnBnClickedBtntest)
	ON_BN_CLICKED(IDCANCEL, &CENDOBENCH_VS2015Dlg::OnBnClickedCancel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CENDOBENCH_VS2015Dlg::OnTvnSelchangedTree1)
	ON_WM_MOUSEMOVE()
	ON_STN_CLICKED(IDC_STATIC_CURVE, &CENDOBENCH_VS2015Dlg::OnStnClickedStaticCurve)
	ON_BN_CLICKED(IDC_BUTTON1, &CENDOBENCH_VS2015Dlg::OnBnClickedButton1)
	ON_MESSAGE(WM_COMM_RXSTR, &CENDOBENCH_VS2015Dlg::OnReceiveStr)
END_MESSAGE_MAP()


// CENDOBENCH_VS2015Dlg ��Ϣ�������

BOOL CENDOBENCH_VS2015Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CFont font; // ���ô���������ʾ����
	font.CreateFont(16, 6, 0, 0, 600,
		TRUE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
	m_ReceiveCtrl.SetFont(&font);
	
	// ��ʼ�����οؼ�
	InitTreeControl();

	// ��ʼ���Ի��� IDD_ENDOBENCH_VS2015_DIALOG IDC_Pic_Dlg
	CRect rect;
	GetDlgItem(IDC_Pic_Dlg)->GetWindowRect(&rect);//��ȡ�ؼ�����Ļ����
	ScreenToClient(&rect);//ת��Ϊ�Ի����ϵĿͻ�����

	fov_dlg.Create(IDD_DIALOG_FOV, this);
	fov_dlg.ShowWindow(SW_SHOW);
	fov_dlg.MoveWindow(rect,TRUE);

	resolution_dlg.Create(IDD_Resolution, this);
	resolution_dlg.ShowWindow(SW_HIDE);
	resolution_dlg.MoveWindow(rect, TRUE);
	
	distortion_dlg.Create(IDD_DIALOG_Distortion, this);
	distortion_dlg.ShowWindow(SW_HIDE);
	distortion_dlg.MoveWindow(rect, TRUE);

	// ��ʼ���̶ȱ�
	double cx[11], cy[11];
	for (int i = 0; i < 11; i++) {cx[i] = (double)i * 5;cy[i] = (double)i / 10.;}
	m_StcCurve.Remove();
	m_StcCurve.SetCoordinate(cx, cy, 11, 11);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CENDOBENCH_VS2015Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CENDOBENCH_VS2015Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CENDOBENCH_VS2015Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CENDOBENCH_VS2015Dlg::ShowImage(IplImage* image, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;
	int rh = rect.bottom - rect.top;

	// ͼ��ԭʼ��С
	int iw = image->width;
	int ih = image->height;

	// ͼ�����䵽�ؼ�
	cv::Rect img; img.width = iw; img.height = ih;
	cv::Rect picture; picture.width = rect.Width(); picture.height = rect.Height();
	auto adapt_rect = AdaptTo(img, picture);
	iw = adapt_rect.width;     // ������ͼ���С
	ih = adapt_rect.height;

	int tx = (int)(rw - iw) / 2;
	int ty = (int)(rh - ih) / 2;
	SetRect(rect, tx, ty, tx + iw, ty + ih);

	CvvImage cimg;
	cimg.CopyOf(image);
	cimg.DrawToHDC(hDC, &rect);

	// �������������ѡ���
	DrawInPictureControl(status, ID);
	
	ReleaseDC(pDC);
}

void CENDOBENCH_VS2015Dlg::ShowImage(const Mat &image, UINT ID)
{
	IplImage ipl_image = image;
	ShowImage(&ipl_image, ID);
}



void CENDOBENCH_VS2015Dlg::DrawInPictureControl(Status status, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	if (status == Status::SDetection)
	{// ���״̬������
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));  // ͸����ˢ
		pDC->SelectObject(pBrush);
		pDC->Rectangle(intercept_rect);
	}

	if (status == Status::SDistortion)
	{// ���Բ�ı������
		// ��Բ����
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));  // ͸����ˢ
		pDC->SelectObject(pBrush);
		/*CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		pDC->SelectObject(pen);*/

		auto Drawcircle = [&](double x,double y, double radius)
		{	
			pDC->Ellipse(x- radius,y- radius, x+radius,y+radius);
		};

		Drawcircle(distortion_dlg.top_x, distortion_dlg.top_y, distortion_dlg.circle_radius);
		Drawcircle(distortion_dlg.bottom_x, distortion_dlg.bottom_y, distortion_dlg.circle_radius);
		Drawcircle(distortion_dlg.left_x, distortion_dlg.left_y, distortion_dlg.circle_radius);
		Drawcircle(distortion_dlg.right_x, distortion_dlg.right_y, distortion_dlg.circle_radius);
		Drawcircle(distortion_dlg.center_x, distortion_dlg.center_y, distortion_dlg.circle_radius);
	}
}

void VideoThreadFunc(CENDOBENCH_VS2015Dlg *pMainthread)
{
	auto pcam = SingletonCamera::GetInstance();
	bool is_connected = pcam->StartCamera();
	if (!is_connected)
		return;
	while (1)
	{
		// ������û���������˳��߳�
		if (!pcam->IsConnected())
			return;

		// ��ȡ����
		SingletonCamera::image_type buf_image;
		Error error = pcam->RetrieveBuffer(&buf_image);//���ڴ�ȡ��ͼ��浽rawImage����
		if (error != FlyCapture2::PGRERROR_OK)
		{
			continue;
		}
		Mat tmp;
		ConvertToMatImage(buf_image, tmp);
		
		pMainthread->ShowImage(tmp, IDC_PicControl);
		Sleep(10);
	}
}

void CENDOBENCH_VS2015Dlg::OnBnClickedBtntest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ����߳�
	//pthread = new std::thread(VideoThreadFunc,this);
	
	// ����
	int SelPortNO, SelBaudRate;
	bool is_sp_open = serial_port.InitPort(this->GetSafeHwnd(), 1);
	if (is_sp_open)
	{
		AfxMessageBox(_T("�򿪳ɹ�"));
	}
	else
	{
		AfxMessageBox(_T("��ʧ��"));
	}
}


// �ر�Ӧ�ó���
void CENDOBENCH_VS2015Dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
	if (pthread)
	{
		// �ȹر����
		auto pcam = SingletonCamera::GetInstance();
		while (!pcam->StopCamera())
			pcam->StopCamera();
		// �ر��߳�
		pthread->join();
		delete pthread;
	}
		
}


void CENDOBENCH_VS2015Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	if (status == Status::SDetection)
	{
		if (!SingletonCamera::GetInstance()->IsConnected())return;
		// ��pointת����Picture����ϵ
		CRect rect;
		GetDlgItem(IDC_PicControl)->GetWindowRect(&rect);
		ScreenToClient(rect);
		point.x = point.x - rect.left;
		point.y = point.y - rect.top;

		intercept_rect.left = point.x;
		intercept_rect.top = point.y;
		intercept_rect.right = point.x;
		intercept_rect.bottom = point.y;
	}
	if (status == Status::SDistortion)
	{
		// ���������ľ���
		auto Get2PointDiatance = [&](int x1,int y1, int x2, int y2)
		{
			return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
		};

		CRect rect;
		GetDlgItem(IDC_PicControl)->GetWindowRect(&rect);
		ScreenToClient(rect);
		point.x = point.x - rect.left;
		point.y = point.y - rect.top;
		if (Get2PointDiatance(point.x, point.y, distortion_dlg.top_x, distortion_dlg.top_y) <= distortion_dlg.circle_radius)
		{
			distortion_selected = DistortionSelected::top;
		}
		else if (Get2PointDiatance(point.x, point.y, distortion_dlg.bottom_x, distortion_dlg.bottom_y) <= distortion_dlg.circle_radius)
		{
			distortion_selected = DistortionSelected::bottom;
		}
		else if (Get2PointDiatance(point.x, point.y, distortion_dlg.left_x, distortion_dlg.left_y) <= distortion_dlg.circle_radius)
		{
			distortion_selected = DistortionSelected::left;
		}
		else if (Get2PointDiatance(point.x, point.y, distortion_dlg.right_x, distortion_dlg.right_y) <= distortion_dlg.circle_radius)
		{
			distortion_selected = DistortionSelected::right;
		}
		else if (Get2PointDiatance(point.x, point.y, distortion_dlg.center_x, distortion_dlg.center_y) <= distortion_dlg.circle_radius)
		{
			distortion_selected = DistortionSelected::center;
		}
		else
		{
			distortion_selected = DistortionSelected::None;
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CENDOBENCH_VS2015Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (status == Status::SDetection)
	{
		if (!SingletonCamera::GetInstance()->IsConnected())return;
		// ��pointת����Picture����ϵ
		CRect rect;
		GetDlgItem(IDC_PicControl)->GetWindowRect(&rect);
		ScreenToClient(rect);
		point.x = point.x - rect.left;
		point.y = point.y - rect.top;

		intercept_rect.right = point.x;
		intercept_rect.bottom = point.y;
		auto rect_roi = MapToRawImage(intercept_rect, CRect(0, 0, rect.Width(), rect.Height()));
	}
	
	

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CENDOBENCH_VS2015Dlg::InitTreeControl()
{
	//���οؼ���ʼ��
	HTREEITEM hRoot;     // ���ĸ��ڵ�ľ��   
	HTREEITEM hCataItem; // �ɱ�ʾ��һ����ڵ�ľ��   
	HTREEITEM hEleItem;  // �ɱ�ʾ��һ���ݽڵ�ľ��
						 // ������ڵ�   
	hRoot = tree_control.InsertItem(_T("Test Result"), 0, 0);

	// �ڸ��ڵ��²���FOV�ӽڵ�   
	hCataItem = tree_control.InsertItem(_T("FOV"), 0, 0, hRoot, TVI_LAST);
	tree_control.SetItemData(hCataItem, FOV);

	hEleItem = tree_control.InsertItem(_T("Real Fov d1:0.0 mm"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, FOV);

	hEleItem = tree_control.InsertItem(_T("Real Fov d2:0.0 mm"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, FOV);

	hEleItem = tree_control.InsertItem(_T("Real Fov: 0.0 deg"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, FOV);

	hEleItem = tree_control.InsertItem(_T("Distance between P&V:0.0 mm"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, FOV);

	hEleItem = tree_control.InsertItem(_T("Vertex FOV: 0.0 deg"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, FOV);

	hEleItem = tree_control.InsertItem(_T("Direction Of View: 0.0 deg"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, FOV);

	// �ڸ��ڵ��²���Resolution�ӽڵ� 
	hCataItem = tree_control.InsertItem(_T("Resolution"), 0, 0, hRoot, TVI_LAST);
	tree_control.SetItemData(hCataItem, Resolution);

	hEleItem = tree_control.InsertItem(_T("Center: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("First: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Second: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Third: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Fourth: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Edge Avg: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Edge Min: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Edge Std Dev: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Measured Distance: 0.0 lp/mm, 0.0C/��"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);

	// �ڸ��ڵ��²���Distortion�ӽڵ� 
	hCataItem = tree_control.InsertItem(_T("Distortion"), 0, 0, hRoot, TVI_LAST);
	tree_control.SetItemData(hCataItem, Distortion);

	hEleItem = tree_control.InsertItem(_T("Distortion Top: 0.0%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Distortion);

	hEleItem = tree_control.InsertItem(_T("Distortion Right: 0.0%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Distortion);

	hEleItem = tree_control.InsertItem(_T("Distortion Bottom: 0.0%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Distortion);

	hEleItem = tree_control.InsertItem(_T("Distortion Left: 0.0%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Distortion);

	hEleItem = tree_control.InsertItem(_T("Distortion Avg: 0.0%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Distortion);

	hEleItem = tree_control.InsertItem(_T("Distortion Max: 0.0%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Distortion);

	hEleItem = tree_control.InsertItem(_T("Distortion Std Dev: 0.0%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Distortion);

	// �ڸ��ڵ��²���oke�ӽڵ� 
	hCataItem = tree_control.InsertItem(_T("Detection"), 0, 0, hRoot, TVI_LAST);
	tree_control.SetItemData(hCataItem, Detection);

	hEleItem = tree_control.InsertItem(_T("????%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Detection);
}


void CENDOBENCH_VS2015Dlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	//���ص� ���еĶԻ���
	fov_dlg.ShowWindow(SW_HIDE);
	resolution_dlg.ShowWindow(SW_HIDE);
	distortion_dlg.ShowWindow(SW_HIDE);

	// ��ȡ��ǰѡ�нڵ�ľ��   
	HTREEITEM hItem = tree_control.GetSelectedItem();
	int choice = tree_control.GetItemData(hItem);

	switch (choice)
	{
	case FOV:
		fov_dlg.ShowWindow(SW_SHOW);
		status = Status::SFOV;
		break;
	case Resolution:
		resolution_dlg.ShowWindow(SW_SHOW);
		status = Status::SResolution;
		break;
	case Distortion:
		distortion_dlg.ReadCircleCrd(); // ���¶�ȡ������Բ�������ļ�
		distortion_dlg.ShowWindow(SW_SHOW);
		status = Status::SDistortion;
		break;
	case Detection:
		status = Status::SDetection;
		break;
	default:
		fov_dlg.ShowWindow(SW_SHOW);
	}
	
}


void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point) { CDialogEx::OnMouseMove(nFlags, point); }


void CENDOBENCH_VS2015Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (status == Status::SDetection)
	{
		if (!SingletonCamera::GetInstance()->IsConnected())return;
		if (nFlags == MK_LBUTTON) // �������
		{
			CRect rect;
			GetDlgItem(IDC_PicControl)->GetWindowRect(&rect);
			ScreenToClient(rect);
			point.x = point.x - rect.left;
			point.y = point.y - rect.top;
			intercept_rect.right = point.x;
			intercept_rect.bottom = point.y;
		}
	}
	else if (status == Status::SDistortion)
	{
		if (nFlags == MK_LBUTTON) // �������
		{
			CRect rect;
			GetDlgItem(IDC_PicControl)->GetWindowRect(&rect);
			ScreenToClient(rect);
			point.x = point.x - rect.left;
			point.y = point.y - rect.top;
			if (distortion_selected == DistortionSelected::top)
			{
				distortion_dlg.top_x = point.x;
				distortion_dlg.top_y = point.y;
			}
			else if (distortion_selected == DistortionSelected::bottom)
			{
				distortion_dlg.bottom_x = point.x;
				distortion_dlg.bottom_y = point.y;
			}
			else if (distortion_selected == DistortionSelected::left)
			{
				distortion_dlg.left_x = point.x;
				distortion_dlg.left_y = point.y;
			}
			else if (distortion_selected == DistortionSelected::right)
			{
				distortion_dlg.right_x = point.x;
				distortion_dlg.right_y = point.y;
			}
			else if (distortion_selected == DistortionSelected::center)
			{
				distortion_dlg.center_x = point.x;
				distortion_dlg.center_y = point.y;
			}
		}
	}
	
	
	CDialogEx::OnMouseMove(nFlags, point);
}


void CENDOBENCH_VS2015Dlg::OnStnClickedStaticCurve()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


// �򿪹رմ��ڰ�ť
void CENDOBENCH_VS2015Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString temp;
	btn_SelPortOpenCloseCtrl.GetWindowTextW(temp);
	if (temp == _T("Close Port"))
	{
		serial_port.ClosePort();
		btn_SelPortOpenCloseCtrl.SetWindowTextW(_T("Open Port"));
		//AfxMessageBox(_T("Close serial port success��"));
	}
	else
	{
		// ��ȡ�˿ں�
		CString SelPortNO;
		edit_SelPortNO.GetWindowTextW(SelPortNO);
		// ������ںŲ���������ʧ��
		if (!_ttoi(SelPortNO))
		{
			AfxMessageBox(_T("����������Ҫ�򿪴��ڱ��(������)"));
			return;
		}
		// �򿪴���
		bool is_open = serial_port.InitPort(this->GetSafeHwnd(), _ttoi(SelPortNO));
		if (is_open)
		{
			serial_port.StartMonitoring();
			btn_SelPortOpenCloseCtrl.SetWindowText(_T("Close Port"));
			// ���ش���У������
			LoadIni();
			//AfxMessageBox(_T("Open serial port success��"));
		}
		else
		{
			AfxMessageBox(_T("Open serial port failure��Mabye the port is absent or occupied!"));
		}
	}
}

LRESULT CENDOBENCH_VS2015Dlg::OnReceiveStr(WPARAM str, LPARAM commInfo)
{
	struct serialPortInfo
	{
		UINT portNr;//���ں�
		DWORD bytesRead;//��ȡ���ֽ���
	}*pCommInfo;
	pCommInfo = (serialPortInfo*)commInfo;

	// ���յ������ݳ���
	int len = pCommInfo->bytesRead;
	// ��ȡ����
	std::string tmp = std::string((char*)str, (char*)str + len);

	// ÿ�ﵽ256�ֽھ����
	static string out_put;
	out_put += tmp;
	if (out_put.size() >= 128)
	{
		// �˴������ݽ����Լ�У���㷨Ϊ��ѧԺ�ṩ
		std::string SA = GetSubA(out_put);
		std::string SB = GetSubB(out_put);
		double A = Convert_To_Real_CH1(SA);
		double B = Convert_To_Real_CH2(SB);
		CString distance(std::to_string(A).c_str());
		CString angle(std::to_string(B).c_str());

		CString received = CString("����: ") + distance + "  " + CString("�Ƕ�: ") + angle;
		m_ReceiveCtrl.SetWindowTextW(received);
		out_put.clear();
		UpdateData(FALSE);
	}
	return TRUE;
}