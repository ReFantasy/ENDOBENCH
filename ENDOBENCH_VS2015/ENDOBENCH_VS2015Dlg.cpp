
// ENDOBENCH_VS2015Dlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CENDOBENCH_VS2015Dlg 对话框



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


// CENDOBENCH_VS2015Dlg 消息处理程序

BOOL CENDOBENCH_VS2015Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CFont font; // 设置串口数据显示字体
	font.CreateFont(16, 6, 0, 0, 600,
		TRUE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
	m_ReceiveCtrl.SetFont(&font);
	
	// 初始化树形控件
	InitTreeControl();

	// 初始化对话框 IDD_ENDOBENCH_VS2015_DIALOG IDC_Pic_Dlg
	CRect rect;
	GetDlgItem(IDC_Pic_Dlg)->GetWindowRect(&rect);//获取控件的屏幕坐标
	ScreenToClient(&rect);//转换为对话框上的客户坐标

	fov_dlg.Create(IDD_DIALOG_FOV, this);
	fov_dlg.ShowWindow(SW_SHOW);
	fov_dlg.MoveWindow(rect,TRUE);

	resolution_dlg.Create(IDD_Resolution, this);
	resolution_dlg.ShowWindow(SW_HIDE);
	resolution_dlg.MoveWindow(rect, TRUE);
	
	distortion_dlg.Create(IDD_DIALOG_Distortion, this);
	distortion_dlg.ShowWindow(SW_HIDE);
	distortion_dlg.MoveWindow(rect, TRUE);

	// 初始化刻度表
	double cx[11], cy[11];
	for (int i = 0; i < 11; i++) {cx[i] = (double)i * 5;cy[i] = (double)i / 10.;}
	m_StcCurve.Remove();
	m_StcCurve.SetCoordinate(cx, cy, 11, 11);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CENDOBENCH_VS2015Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

	// 图像原始大小
	int iw = image->width;
	int ih = image->height;

	// 图像适配到控件
	cv::Rect img; img.width = iw; img.height = ih;
	cv::Rect picture; picture.width = rect.Width(); picture.height = rect.Height();
	auto adapt_rect = AdaptTo(img, picture);
	iw = adapt_rect.width;     // 适配后的图像大小
	ih = adapt_rect.height;

	int tx = (int)(rw - iw) / 2;
	int ty = (int)(rh - ih) / 2;
	SetRect(rect, tx, ty, tx + iw, ty + ih);

	CvvImage cimg;
	cimg.CopyOf(image);
	cimg.DrawToHDC(hDC, &rect);

	// 在这里加入额外的选择框
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
	{// 检测状态画矩形
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));  // 透明画刷
		pDC->SelectObject(pBrush);
		pDC->Rectangle(intercept_rect);
	}

	if (status == Status::SDistortion)
	{// 检测圆的变形误差
		// 画圆函数
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));  // 透明画刷
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
		// 如果相机没有连接则退出线程
		if (!pcam->IsConnected())
			return;

		// 读取数据
		SingletonCamera::image_type buf_image;
		Error error = pcam->RetrieveBuffer(&buf_image);//从内存取回图像存到rawImage里面
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
	// TODO: 在此添加控件通知处理程序代码

	// 相机线程
	//pthread = new std::thread(VideoThreadFunc,this);
	
	
}


// 关闭应用程序
void CENDOBENCH_VS2015Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	if (pthread)
	{
		// 先关闭相机
		auto pcam = SingletonCamera::GetInstance();
		while (!pcam->StopCamera())
			pcam->StopCamera();
		// 关闭线程
		pthread->join();
		delete pthread;
	}
		
}


void CENDOBENCH_VS2015Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (status == Status::SDetection)
	{
		if (!SingletonCamera::GetInstance()->IsConnected())return;
		// 将point转换到Picture坐标系
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
		// 计算两点间的距离
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (status == Status::SDetection)
	{
		if (!SingletonCamera::GetInstance()->IsConnected())return;
		// 将point转换到Picture坐标系
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
	//树形控件初始化
	HTREEITEM hRoot;     // 树的根节点的句柄   
	HTREEITEM hCataItem; // 可表示任一分类节点的句柄   
	HTREEITEM hEleItem;  // 可表示任一数据节点的句柄
						 // 插入根节点   
	hRoot = tree_control.InsertItem(_T("Test Result"), 0, 0);

	// 在根节点下插入FOV子节点   
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

	// 在根节点下插入Resolution子节点 
	hCataItem = tree_control.InsertItem(_T("Resolution"), 0, 0, hRoot, TVI_LAST);
	tree_control.SetItemData(hCataItem, Resolution);

	hEleItem = tree_control.InsertItem(_T("Center: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("First: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Second: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Third: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Fourth: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Edge Avg: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Edge Min: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Edge Std Dev: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);
	hEleItem = tree_control.InsertItem(_T("Measured Distance: 0.0 lp/mm, 0.0C/°"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Resolution);

	// 在根节点下插入Distortion子节点 
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

	// 在根节点下插入oke子节点 
	hCataItem = tree_control.InsertItem(_T("Detection"), 0, 0, hRoot, TVI_LAST);
	tree_control.SetItemData(hCataItem, Detection);

	hEleItem = tree_control.InsertItem(_T("????%"), 0, 0, hCataItem, TVI_LAST);
	tree_control.SetItemData(hEleItem, Detection);
}


void CENDOBENCH_VS2015Dlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//隐藏掉 所有的对话框
	fov_dlg.ShowWindow(SW_HIDE);
	resolution_dlg.ShowWindow(SW_HIDE);
	distortion_dlg.ShowWindow(SW_HIDE);

	// 获取当前选中节点的句柄   
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
		distortion_dlg.ReadCircleCrd(); // 重新读取被检测的圆的配置文件
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (status == Status::SDetection)
	{
		if (!SingletonCamera::GetInstance()->IsConnected())return;
		if (nFlags == MK_LBUTTON) // 左键按下
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
		if (nFlags == MK_LBUTTON) // 左键按下
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
	// TODO: 在此添加控件通知处理程序代码
}


// 打开关闭串口按钮
void CENDOBENCH_VS2015Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	btn_SelPortOpenCloseCtrl.GetWindowTextW(temp);
	if (temp == _T("Close Port"))
	{
		serial_port.ClosePort();
		btn_SelPortOpenCloseCtrl.SetWindowTextW(_T("Open Port"));
		//AfxMessageBox(_T("Close serial port success！"));
	}
	else
	{
		// 获取端口号
		CString SelPortNO;
		edit_SelPortNO.GetWindowTextW(SelPortNO);
		// 如果串口号不是正整数失败
		if (!_ttoi(SelPortNO))
		{
			AfxMessageBox(_T("请先输入需要打开串口编号(正整数)"));
			return;
		}
		// 打开串口
		bool is_open = serial_port.InitPort(this->GetSafeHwnd(), _ttoi(SelPortNO));
		if (is_open)
		{
			serial_port.StartMonitoring();
			btn_SelPortOpenCloseCtrl.SetWindowText(_T("Close Port"));
			// 加载串口校正数据
			LoadIni();
			//AfxMessageBox(_T("Open serial port success！"));
		}
		else
		{
			AfxMessageBox(_T("Open serial port failure！Mabye the port is absent or occupied!"));
		}
	}
}


void CENDOBENCH_VS2015Dlg::example_read_write(const char* from, const char* to)
{
	cout << "read " << from << endl;
	BasicExcel xls(from);

	XLSFormatManager fmt_mgr(xls);
	BasicExcelWorksheet* sheet = xls.GetWorksheet(0);

	CellFormat fmt_general(fmt_mgr);

	fmt_general.set_format_string("0.000");

	for (int y = 0; y < 2; ++y) {
		for (int x = 0; x < 2; ++x) {
			cout << y << "/" << x;

			BasicExcelCell* cell = sheet->Cell(y, x);

			CellFormat fmt(fmt_mgr, cell);

			//			cout << " - xf_idx=" << cell->GetXFormatIdx();

			const Workbook::Font& font = fmt_mgr.get_font(fmt);
			string font_name = stringFromSmallString(font.name_);
			cout << "  font name: " << font_name;

			const wstring& fmt_string = fmt.get_format_string();
			cout << "  format: " << narrow_string(fmt_string);

			cell->SetFormat(fmt_general);

			cout << endl;
		}
	}

	cout << "write: " << from << endl;
	xls.SaveAs(to);
}

LRESULT CENDOBENCH_VS2015Dlg::OnReceiveStr(WPARAM str, LPARAM commInfo)
{
	struct serialPortInfo
	{
		UINT portNr;//串口号
		DWORD bytesRead;//读取的字节数
	}*pCommInfo;
	pCommInfo = (serialPortInfo*)commInfo;

	// 接收到的数据长度
	int len = pCommInfo->bytesRead;
	// 读取数据
	std::string tmp = std::string((char*)str, (char*)str + len);

	// 每达到256字节就输出
	static string out_put;
	out_put += tmp;
	if (out_put.size() >= 128)
	{
		// 此处的数据解析以及校正算法为光学院提供
		std::string SA = GetSubA(out_put);
		std::string SB = GetSubB(out_put);
		double A = Convert_To_Real_CH1(SA);
		double B = Convert_To_Real_CH2(SB);
		CString distance(std::to_string(A).c_str());
		CString angle(std::to_string(B).c_str());

		CString received = CString("距离: ") + distance + "  " + CString("角度: ") + angle;
		m_ReceiveCtrl.SetWindowTextW(received);
		out_put.clear();
		UpdateData(FALSE);
	}
	return TRUE;
}