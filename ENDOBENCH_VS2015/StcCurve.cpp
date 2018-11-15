// StcCurve.cpp : implementation file
//

#include "stdafx.h"
#include "StcCurve.h"


// CStcCurve

IMPLEMENT_DYNAMIC(CStcCurve, CStatic)
/*================================================================
*
* 函 数 名：CStcCurve
*
* 参　　数：
*
*
* 功能描述:
*
*　　　　 构造函数，数据初始化
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
CStcCurve::CStcCurve()
{
	//坐标轴初始化
	m_nX = 0;
	m_nY = 0;
	m_pdX = NULL;
	m_pdY = NULL;
	m_pImg = NULL;
	//位图文件头初始化
	m_lpBmpInfo = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFO) + 1024];
	m_lpBmpInfo->bmiHeader.biBitCount = 24;
	m_lpBmpInfo->bmiHeader.biClrImportant = 256;
	m_lpBmpInfo->bmiHeader.biClrUsed = 256;
	m_lpBmpInfo->bmiHeader.biCompression = 0;
	m_lpBmpInfo->bmiHeader.biHeight = 0;
	m_lpBmpInfo->bmiHeader.biPlanes = 1;
	m_lpBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_lpBmpInfo->bmiHeader.biSizeImage = 0;
	m_lpBmpInfo->bmiHeader.biWidth = 0;
	m_lpBmpInfo->bmiHeader.biXPelsPerMeter = 2835;
	m_lpBmpInfo->bmiHeader.biYPelsPerMeter = 2835;
	int i;
	for (i = 0; i < 256; i++)
	{
		m_lpBmpInfo->bmiColors[i].rgbRed = i;
		m_lpBmpInfo->bmiColors[i].rgbGreen = i;
		m_lpBmpInfo->bmiColors[i].rgbBlue = i;
		m_lpBmpInfo->bmiColors[i].rgbReserved = 0;
	}
	m_bFlag = false;
}
/*================================================================
*
* 函 数 名：~CStcCurve
*
* 参　　数：
*
*
* 功能描述:
*
*　　　　 析构函数，内存释放
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
CStcCurve::~CStcCurve()
{
	if (m_pImg)
	{
		cvReleaseImage(&m_pImg);
	}
	if (m_pdX)
	{
		delete[] m_pdX;
	}
	if (m_pdY)
	{
		delete[] m_pdY;
	}
}


BEGIN_MESSAGE_MAP(CStcCurve, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CStcCurve message handlers
//添加曲线节点
/*================================================================
*
* 函 数 名：AddInput
*
* 参　　数：
*
*　　　　  x	: x坐标
*		   y	: y坐标
*		   size	: 坐标点个数
*
* 功能描述:
*
*　　　　 添加x、y坐标，绘制曲线
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
void CStcCurve::AddInput(double* x,		//x坐标
	double* y,		//y坐标
	int size,		//坐标数量
	bool bFlag,	//是否拟合
	bool bFlag2)	//
{
	m_bIsNihe = bFlag;
	m_bFlag = bFlag2;
	STRUCT_CURVE sc;
	sc.x = new double[size];
	sc.y = new double[size];
	memcpy(sc.x, x, sizeof(double) * size);
	memcpy(sc.y, y, sizeof(double) * size);
	sc.size = size;
	int i;
	for (i = 1; i < size - 1; i++)
	{
		//sc.y[i] = Middle(sc.y[i - 1], sc.y[i], sc.y[i + 1]);
	}
	m_CurveArray.Add(sc);
	this->Invalidate(TRUE);
}
/*================================================================
*
* 函 数 名：SetCoordinate

*
* 参　　数：
*
*　　　　  x	: x轴刻度值
*		   y	: y轴刻度值
*		   nx	: x轴刻度个数
*　　　　  ny	: y轴刻度个数
*
* 功能描述:
*
*　　　　 设置坐标轴刻度
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
//
void CStcCurve::SetCoordinate(double* x, double* y, int nx, int ny)
{
	m_nX = nx;
	m_nY = ny;
	if (m_pdX)
	{
		delete[] m_pdX;
	}
	m_pdX = new double[nx];
	memcpy(m_pdX, x, sizeof(double) * nx);
	if (m_pdY)
	{
		delete[] m_pdY;
	}
	m_pdY = new double[ny];
	memcpy(m_pdY, y, sizeof(double) * ny);
	InitData();
}
/*================================================================
*
* 函 数 名：InitData
*
* 参　　数：
*
*　　　　  无
*
* 功能描述:
*
*　　　　 数据初始化，根据客户区大小创建图像，设置位图文件信息
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
//数据初始化
void CStcCurve::InitData(void)
{
	RECT rect;
	this->GetClientRect(&rect);
	if (m_pImg)
	{
		cvReleaseImage(&m_pImg);
	}
	//创建图像
	m_pImg = cvCreateImage(cvSize(rect.right - rect.left, rect.bottom - rect.top), 8, 3);
	m_lpBmpInfo->bmiHeader.biWidth = m_pImg->width;
	m_lpBmpInfo->bmiHeader.biHeight = m_pImg->height;
	m_lpBmpInfo->bmiHeader.biSizeImage = m_pImg->imageSize;
	this->Invalidate(TRUE);
}

/*================================================================
*
* 函 数 名：OnPaint
*
* 参　　数：
*
*　　　　  无
*
* 功能描述:
*
*　　　　 重绘
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
void CStcCurve::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CStatic::OnPaint() for painting messages
	Draw(&dc);
}

/*================================================================
*
* 函 数 名：Draw
*
* 参　　数：
*
*　　　　  pDC	: CDC
*
* 功能描述:
*
*　　　　 将图像显示在pDC
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
void CStcCurve::Draw(CDC* pDC)
{
	if (m_pImg)
	{
		//绘制坐标轴
		DrawCoordinate();
		///< 设置显示模式
		SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
		StretchDIBits(pDC->m_hDC,
			0, m_pImg->height, m_pImg->width, -m_pImg->height,
			0, 0, m_pImg->width, m_pImg->height,
			(void*)(m_pImg->imageData),
			m_lpBmpInfo,
			DIB_RGB_COLORS, SRCCOPY);
	}
}
/*================================================================
*
* 函 数 名：DrawCoordinate
*
* 参　　数：
*
*　　　　  无
*
* 功能描述:
*
*　　　　 画坐标轴和曲线
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
void CStcCurve::DrawCoordinate(void)
{
	//初始化为白色背景
	memset(m_pImg->imageData, 255, m_pImg->imageSize);
	//绘图区域
	RECT rect;
	this->GetClientRect(&rect);
	//绘图范围
	int x0, y0, x1, y1;
	x0 = WIDE + 10;
	y0 = WIDE / 2;
	x1 = rect.right - WIDE / 2;
	y1 = rect.bottom - WIDE;
	//坐标轴
	cvLine(m_pImg, cvPoint(x0, y1), cvPoint(x0, y0), CV_RGB(69, 84, 153), 1);
	cvLine(m_pImg, cvPoint(x0, y1), cvPoint(x1, y1), CV_RGB(69, 84, 153), 1);
	//坐标轴虚线
	int x, y, i, j;
	CString str;
	if (m_nX > 0 && m_nY > 0)
	{
		int nStepX, nStepY;
		double d;
		if (m_bFlag)
		{
			d = .5;
		}
		else
		{
			d = 1.;
		}
		nStepX = (x1 - x0) / (m_nX - 1);
		nStepY = (y1 - y0) / (m_nY - 1);
		y = y1 - (m_nY - 1) * nStepY;


		auto Lf = [&](std::string &str) // string只保留一位小数
		{
			int point_index = -1;
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == '.')
				{
					point_index = i;
					break;
				}
			}
			if (point_index != -1)
			{
				std::string stmp(str, 0, point_index + 2);
				str = stmp;
			}
			
		};
		//str.Format(CString("%.1f"), m_pdX[0] * d);
		std::string str = to_string(m_pdX[0] * d);
		Lf(str);
		//////////////////////////////////////////////////////////////////////////
		//X轴刻度及文字
		cvPutText(m_pImg, str.c_str(), cvPoint(x0 - 12, y1 + 12), &cvFont(.8), CV_RGB(69, 84, 153));
		for (i = 1; i < m_nX; i++)
		{
			x = x0 + i * nStepX;
			DotLine(m_pImg, cvPoint(x, y), cvPoint(x, y1), CV_RGB(69, 84, 153));
			//X轴刻度
			//str.Format("%.1f", m_pdX[i] * d);
			str = to_string(m_pdX[i] * d);
			Lf(str);
			cvPutText(m_pImg, str.c_str(), cvPoint(x - 12, y1 + 12), &cvFont(.8), CV_RGB(69, 84, 153));
		}
		x = x0 + (m_nX - 1) * nStepX;
		//str.Format(CString("%.1f"), m_pdY[0]);
		str = to_string(m_pdY[0]);
		Lf(str);
		//////////////////////////////////////////////////////////////////////////
		//Y轴刻度及文字
		cvPutText(m_pImg, str.c_str(), cvPoint(x0 - 30, y1), &cvFont(.8), CV_RGB(69, 84, 153));
		for (j = 1; j < m_nY; j++)
		{
			y = y1 - j * nStepY;
			DotLine(m_pImg, cvPoint(x0, y), cvPoint(x, y), CV_RGB(69, 84, 153), 1);
			//Y轴刻度
			//str.Format("%.1f", m_pdY[j]);
			str = to_string(m_pdY[j]);
			Lf(str);
			cvPutText(m_pImg, str.c_str(), cvPoint(x0 - 30, y), &cvFont(.8), CV_RGB(69, 84, 153));
		}

		//str.Format(CString("%.1f"), m_pdX[0] * d);
		////////////////////////////////////////////////////////////////////////////
		////X轴刻度及文字
		//cvPutText(m_pImg, str, cvPoint(x0 - 12, y1 + 12), &cvFont(.8), CV_RGB(69, 84, 153));
		//for (i = 1; i < m_nX; i++)
		//{
		//	x = x0 + i * nStepX;
		//	DotLine(m_pImg, cvPoint(x, y), cvPoint(x, y1), CV_RGB(69, 84, 153));
		//	//X轴刻度
		//	str.Format("%.1f", m_pdX[i] * d);
		//	cvPutText(m_pImg, str, cvPoint(x - 12, y1 + 12), &cvFont(.8), CV_RGB(69, 84, 153));
		//}
		//x = x0 + (m_nX - 1) * nStepX;
		//str.Format(CString("%.1f"), m_pdY[0]);
		////////////////////////////////////////////////////////////////////////////
		////Y轴刻度及文字
		//cvPutText(m_pImg, str, cvPoint(x0 - 30, y1), &cvFont(.8), CV_RGB(69, 84, 153));
		//for (j = 1; j < m_nY; j++)
		//{
		//	y = y1 - j * nStepY;
		//	DotLine(m_pImg, cvPoint(x0, y), cvPoint(x, y), CV_RGB(69, 84, 153), 1);
		//	//Y轴刻度
		//	str.Format("%.1f", m_pdY[j]);
		//	cvPutText(m_pImg, str, cvPoint(x0 - 30, y), &cvFont(.8), CV_RGB(69, 84, 153));
		//}
		//画曲线
		int nNum = m_CurveArray.GetSize();


		if (m_bIsNihe)
		{
			//拟合曲线
			if (nNum > 0)
			{
				for (i = 0; i < nNum; i++)
				{
					STRUCT_CURVE sc = m_CurveArray.GetAt(i);
					CvPoint pt1, pt2;
					double a[3];
					CalculateCurveParameter(sc.x, sc.y, 3, sc.size, a);
					//修正在0处传函为1
					a[0] = 1;
					for (j = 0; j < sc.size; j++)
					{
						sc.y[j] = a[2] * sc.x[j] * sc.x[j] + a[1] * sc.x[j] + a[0];
					}
					pt1.x = sc.x[0] * (m_nX - 1) * nStepX / m_pdX[m_nX - 1] + x0;
					pt1.y = y1 - sc.y[0] * (m_nY - 1) * nStepY / m_pdY[m_nY - 1];
					/*for(j = 1; j < sc.size; j ++)
					{
					pt2.x = sc.x[j] * (m_nX - 1) * nStepX / m_pdX[m_nX - 1] + x0;
					pt2.y = y1 - sc.y[j] * (m_nY - 1) * nStepY / m_pdY[m_nY - 1];
					cvLine(m_pImg, pt1, pt2, CV_RGB(0, 255, 0), 1);
					pt1 = pt2;
					}*/
					double s = (sc.x[sc.size - 1] - sc.x[0]) / 100.;
					double ax[100], ay[100];
					for (j = 0; j < 100; j++)
					{
						ax[j] = sc.x[0] + j * s;
						ay[j] = a[2] * ax[j] * ax[j] + a[1] * ax[j] + a[0];
					}
					for (j = 0; j < 100; j++)
					{
						pt2.x = ax[j] * (m_nX - 1) * nStepX / m_pdX[m_nX - 1] + x0;
						pt2.y = y1 - ay[j] * (m_nY - 1) * nStepY / m_pdY[m_nY - 1];
						cvLine(m_pImg, pt1, pt2, CV_RGB(0, 255, 0), 1);
						pt1 = pt2;
					}
				}
			}
		}
		else
		{
			if (nNum > 0)
			{
				for (i = 0; i < nNum; i++)
				{
					STRUCT_CURVE sc = m_CurveArray.GetAt(i);
					CvPoint pt1, pt2;
					pt1.x = sc.x[0] * (m_nX - 1) * nStepX / m_pdX[m_nX - 1] + x0;
					pt1.y = y1 - sc.y[0] * (m_nY - 1) * nStepY / m_pdY[m_nY - 1];
					for (j = 1; j < sc.size; j++)
					{
						pt2.x = sc.x[j] * (m_nX - 1) * nStepX / m_pdX[m_nX - 1] + x0;
						pt2.y = y1 - sc.y[j] * (m_nY - 1) * nStepY / m_pdY[m_nY - 1];
						cvLine(m_pImg, pt1, pt2, CV_RGB(0, 255, 0), 1);
						pt1 = pt2;
					}
				}
			}
		}

	}
}
/*================================================================
*
* 函 数 名：DotLine
*
* 参　　数：
*
*　　　　  src			: 图像
*		   pt1			: 虚线端点1
*		   pt2			: 虚线端点2
*		   color		: 虚线颜色
*		   thinkness	: 虚线宽度
*
* 功能描述:
*
*　　　　 在图像src上，点pt1,pt2之间绘制虚线，虚线颜色为color宽度为thinkness
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
//
void CStcCurve::DotLine(IplImage* src, CvPoint pt1, CvPoint pt2, CvScalar color, int thinkness)
{
	int x, y;
	if (pt1.x == pt2.x)
	{
		//竖直线
		for (y = min(pt1.y, pt2.y); y + 3 <= max(pt1.y, pt2.y); y += 5)
		{
			cvLine(src, cvPoint(pt1.x, y), cvPoint(pt2.x, y + 3), color, 1);
		}
	}
	else if (pt1.y = pt2.y)
	{
		//水平线
		for (x = min(pt1.x, pt2.x); x + 3 <= max(pt1.x, pt2.x); x += 5)
		{
			cvLine(src, cvPoint(x, pt1.y), cvPoint(x + 3, pt2.y), color, 1);
		}
	}
}
/*================================================================
*
* 函 数 名：Remove
*
* 参　　数：
*
*　　　　   无
*
* 功能描述:
*
*　　　　 删除所有坐标点
*
* 返 回 值： 无
*
* 抛出异常： 无
*
* 作　　者： swl		2015-01-04
*
================================================================*/
void CStcCurve::Remove(void)
{
	m_CurveArray.RemoveAll();
	this->Invalidate(TRUE);
}
/*================================================================
*
* 函 数 名：
*
* 参　　数：
*
*　　　　   :
*		   :
*
* 功能描述:
*
*　 最小二乘法2次曲线拟合应用已有的采样时间点，再现这些点
所描述的2次曲线的变化，即求出一个二次曲线方程y=ax2+bx+c (这
个算法的主要问题也就是如何用给定的数据求方程系数abc)

最小二乘法二次曲线拟合算法,Xval时间数据，Yval每个时间点上的
值数据，M代表几次曲线(如：2次的话就是3)，N数据的个数，A二次
曲线方程的系数(A[2]代表a,A[1]代表b,A[0]代表c)
*
* 返 回 值：
*
* 抛出异常：
*
* 作　　者： swl		2015-01-04
*
================================================================*/

BOOL CStcCurve::CalculateCurveParameter(double *Xval, double *Yval, long M, long N, double *A)
{
	//X,Y --  X,Y两轴的坐标
	//M   --  次数，表示几次曲线
	//N   --  采样数目
	//A   --  结果参数

	register long i, j, k;
	double Z, D1, D2, C, P, G, Q;
	//CDoubleArray B,T,S;
	//B.SetSize(N);
	//T.SetSize(N);
	//S.SetSize(N);

	double* B = new double[N];
	double* T = new double[N];
	double* S = new double[N];
	if (M>N)M = N;
	for (i = 0; i<M; i++)
		A[i] = 0;
	Z = 0;
	B[0] = 1;
	D1 = N;
	P = 0;
	C = 0;
	for (i = 0; i<N; i++)
	{
		P = P + Xval[i] - Z;
		C = C + Yval[i];
	}
	C = C / D1;
	P = P / D1;
	A[0] = C*B[0];
	if (M>1)
	{
		T[1] = 1;
		T[0] = -P;
		D2 = 0;
		C = 0;
		G = 0;
		for (i = 0; i<N; i++)
		{
			Q = Xval[i] - Z - P;
			D2 = D2 + Q*Q;
			C = Yval[i] * Q + C;
			G = (Xval[i] - Z)*Q*Q + G;
		}
		C = C / D2;
		P = G / D2;
		Q = D2 / D1;
		D1 = D2;
		A[1] = C*T[1];
		A[0] = C*T[0] + A[0];
	}
	for (j = 2; j<M; j++)
	{
		S[j] = T[j - 1];
		S[j - 1] = -P*T[j - 1] + T[j - 2];
		if (j >= 3)
		{
			for (k = j - 2; k >= 1; k--)
				S[k] = -P*T[k] + T[k - 1] - Q*B[k];
		}
		S[0] = -P*T[0] - Q*B[0];
		D2 = 0;
		C = 0;
		G = 0;
		for (i = 0; i<N; i++)
		{
			Q = S[j];
			for (k = j - 1; k >= 0; k--)
				Q = Q*(Xval[i] - Z) + S[k];
			D2 = D2 + Q*Q;
			C = Yval[i] * Q + C;
			G = (Xval[i] - Z)*Q*Q + G;
		}
		C = C / D2;
		P = G / D2;
		Q = D2 / D1;
		D1 = D2;
		A[j] = C*S[j];
		T[j] = S[j];
		for (k = j - 1; k >= 0; k--)
		{
			A[k] = C*S[k] + A[k];
			B[k] = T[k];
			T[k] = S[k];
		}
	}
	return TRUE;
}

double CStcCurve::Middle(double a, double b, double c)
{
	if (a >= b && a <= c ||
		a >= c && a <= b)
	{
		return a;
	}
	else if (b >= a && b <= c ||
		b >= c && b <= a)
	{
		return b;
	}
	else
	{
		return c;
	}
}
