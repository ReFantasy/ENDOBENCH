#include "StdAfx.h"
#include "Mtf.h"

CMtf::CMtf(IplImage* src, int n, int x, int y, double dPixel, double dZool)
{
	m_nNum = n;
	int width = src->width;
	int height = src->height;
	int widthStep = src->widthStep;
	char* pData = src->imageData;
	double* plfDataX = new double[n];
	double* plfDataY = new double[n];
	int i, j, k, l,m;
	double* plfData = new double[width];
	for(i = 0; i < width; i ++)
	{
		plfData[i] = .0;
		y = y > height - 3 ? height - 3 : y;
		for(j = -2; j <=2; j ++)
		{
			plfData[i] += pData[(y + j) * widthStep + i];
		}
		plfData[i] /= 5.;
	}
	//水平方向求导
	for(i = 0; i < width - 1; i ++)
	{
		plfData[i] = abs(plfData[i + 1] - plfData[i]);
	}
	plfData[width - 1] = 0;
	//选取点附近找最大值
	double dMax = plfData[x];
	if(x + 50 > width)
	{
		x = width - 50;
	}
	else if(x < 50)
	{
		x = 50;
	}
	int t = x;
	for(i = 0; i < 50; i ++)
	{
		if(dMax < plfData[x - i])
		{
			dMax = plfData[x - i];
			t = x - i;
		}
		if(dMax < plfData[x + i])
		{
			dMax = plfData[x + i];
			t = x + i;
		}
	}
	i = t - n / 2;
	if( i < 0)
	{
		i = 0;
	}
	if(i + n > width)
	{
		i = width - n;
	}
	for(j = 0;j < n; j ++)
	{
		plfDataX[j] = plfData[j + i] > 1500 ? plfData[j + i] : 0;
	}

	int w = 1;
	int wp = 0;
	while(w * 2 <= n)
	{
		w *= 2;
		wp ++;
	}
	complex<double> *TD = new complex<double>[w];
	complex<double> *FD = new complex<double>[w];
	double D = 0;
	double C = 0;
	double S = 0;
	for(i = 0; i < w; i ++)
	{
		TD[i] = complex<double>(plfDataX[i], 0);
		//计算刀口函数曲线与X轴所围面积
		D += plfDataX[i];
	}
	//傅里叶变换
	FFT(TD, FD, wp);
	
	m_pdMtfX = new double[n/2];
	m_pdMtfY = new double[n/2];
	for(i = 0; i < n / 2; i ++)
	{
		//频率域取模
		m_pdMtfY[i] = sqrtf(FD[i].real() * FD[i].real() + FD[i].imag() * FD[i].imag()) / D;
		m_pdMtfY[i] = m_pdMtfY[i] * m_pdMtfY[i] * m_pdMtfY[i];
	}
	for(i = 0; i < n / 2; i ++)
	{
		m_pdMtfX[i] = i * 2 * dZool / n / dPixel;
	}

	//////////////////////////////////////////////////////////////////////////
	delete [] plfDataX;
	delete [] plfDataY;
	delete [] plfData;
	delete [] TD;
	delete [] FD;
}

//
//CMtf::CMtf(IplImage* src, int n, int x, int y, double dPixel, double dZool)
//{
//	m_nNum = n;
//	IplImage* pGrayImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
//	//转灰度图
//	if(src->nChannels == 1)
//	{
//		cvCopyImage(src, pGrayImg);
//	}
//	else
//	{
//		cvCvtColor(src, pGrayImg, CV_BGR2GRAY);
//	}
//	//中值滤波
//	cvSmooth(pGrayImg, pGrayImg, CV_MEDIAN);
//	BYTE* p = (BYTE*)(pGrayImg->imageData);
//	double* plfData = new double[pGrayImg->width];
//	int width = pGrayImg->width;
//	int height = pGrayImg->height;
//	int i, j, k;
//	for(i = 0; i <width; i ++)
//	{
//		plfData[i] = .0;
//
//		///< 取中间5行,取均值/*归一*/
//		y = y < 2 ? 2 : y;
//		y = y > height - 3 ? height - 3 : y;
//		for(j = -2; j <= 2; j ++)
//		{
//			plfData[i] += p[(y + j)* pGrayImg->widthStep + i];
//		}
//		plfData[i] /= 5.0;
//	}
//	//水平方向求导
//	for(i = 0; i <width - 1; i ++)
//	{
//		plfData[i] = abs(plfData[i + 1] - plfData[i]);
//	}
//	plfData[width - 1] = 0;
//	//选取点附近找最大值
//	double dMax = plfData[x];
//	int t = x;
//	for(i = 0; i < 50; i ++)
//	{
//		if(dMax < plfData[x - i])
//		{
//			dMax = plfData[x - i];
//			t = x - i;
//		}
//		if(dMax < plfData[x + i])
//		{
//			dMax = plfData[x + i];
//			t = x + i;
//		}
//	}
//	m_pdDaokou = new double[m_nNum];
//	memcpy(m_pdDaokou, plfData + t - n / 2, sizeof(double) * n);
//	int w = 1;
//	int wp = 0;
//	while(w * 2 <= n)
//	{
//		w *= 2;
//		wp ++;
//	}
//	complex<double> *TD = new complex<double>[w];
//	complex<double> *FD = new complex<double>[w];
//	double D = 0;
//	double C = 0;
//	double S = 0;
//	for(i = 0; i < w; i ++)
//	{
//		TD[i] = complex<double>(m_pdDaokou[i], 0);
//		//计算刀口函数曲线与X轴所围面积
//		D += m_pdDaokou[i];
//	}
//	//傅里叶变换
//	FFT(TD, FD, wp);
//	///< 传函曲线的纵横坐标
//	m_pdMtfX = new double[n/2];
//	m_pdMtfY = new double[n/2];
//	for(i = 0; i < n / 2; i ++)
//	{
//		//频率域取模
//		m_pdMtfY[i] = sqrtf(FD[i].real() * FD[i].real() + FD[i].imag() * FD[i].imag()) / D;
//	}
//	m_pdMtfX[0] = dZool / (double)n / dPixel;
//	for(i = 1; i < n / 2; i ++)
//	{
//		///< X坐标计算
//		m_pdMtfX[i] = dZool / (double)(n - i * 2) / dPixel - m_pdMtfX[0];
//	}
//	m_pdMtfX[0] = 0;
//	//////////////////////////////////////////////////////////////////////////
//	//保存MTF测试数据
//	CFile f("MTF.txt", CFile::modeCreate | CFile::modeReadWrite);
//	CString str;
//	str.Format("序号\t    X\t\t    Y\n");
//	f.Write(str.GetBuffer(0), str.GetLength());
//	for(i = 0; i < n / 2; i ++)
//	{
//		str.Format("%d\t%.6lf\t%.6lf\n", i, m_pdMtfX[i], m_pdMtfY[i]);
//		f.Write(str.GetBuffer(0), str.GetLength());
//	}
//	f.Close();
//	//////////////////////////////////////////////////////////////////////////
//	delete [] plfData;
//	delete [] TD;
//	delete [] FD;
//}

CMtf::~CMtf(void)
{
	delete [] m_pdMtfX;
	delete [] m_pdMtfY;
}
//////////////////////////////////////////////////////////////////////
//  图像正交变换函数
//////////////////////////////////////////////////////////////////////
/*************************************************************************
 * 函数名称：
 *   FFT()
 * 参数:
 *   complex<double> * TD	- 指向时域数组的指针
 *   complex<double> * FD	- 指向频域数组的指针
 *   r						－2的幂数，即迭代次数
 * 返回值:
 *   无。
 * 说明:
 *   该函数用来实现快速付立叶变换。
 ************************************************************************/
void CMtf::FFT(complex<double> * TD, complex<double> * FD, int r)
{	
	LONG	count;				// 付立叶变换点数	
	int		i,j,k;				// 循环变量
	int		bfsize,p;	
	double	angle;				// 角度	
	complex<double> *W,*X1,*X2,*X;
	
	count = 1 << r;				// 计算付立叶变换点数
	
	// 分配运算所需存储器
	W  = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// 计算加权系数
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// 将时域点写入X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) 
					* W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// 重新排序
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
	}
	
	delete W;
	delete X1;
	delete X2;
}

/*************************************************************************
 * 函数名称：
 *   IFFT()
 * 参数:
 *   complex<double> * FD	- 指向频域值的指针
 *   complex<double> * TD	- 指向时域值的指针
 *   r						－2的幂数
 * 返回值:
 *   无。
 * 说明:
 *   该函数用来实现快速付立叶反变换。
 ************************************************************************/
void CMtf::IFFT(complex<double> * FD, complex<double> * TD, int r)
{	
	LONG	count;					// 付立叶变换点数	
	int		i;						// 循环变量	
	complex<double> *X;	
	
	count = 1 << r;					// 计算付立叶变换点数
	X = new complex<double>[count];	// 分配运算所需存储器
	memcpy(X, FD, sizeof(complex<double>) * count);	// 将频域点写入X
	
	// 求共轭
	for(i = 0; i < count; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	FFT(X, TD, r);					// 调用快速付立叶变换
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
	{
		TD[i] = complex<double> (TD[i].real() / count, -TD[i].imag() / count);
	}
	delete X;
}

double* CMtf::GetMtfX(int& nNum)
{
	nNum = m_nNum / 2;
	return m_pdMtfX;
}

double* CMtf::GetMtfY(int& nNum)
{
	nNum = m_nNum / 2;
	return m_pdMtfY;
}
