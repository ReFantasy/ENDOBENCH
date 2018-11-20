#include "StdAfx.h"
#include "Mtf.h"
CMtf::CMtf(cv::Mat src_mat,
	int n,			//����
	int x,			//����Ȥ�������ĵ�����x
	int y,			//����Ȥ�������ĵ�����y
	double dPixel,	//��Ԫ��С
	double dZool)	//���ű���
{
	m_nNum = n;
	uchar *pData = src_mat.data;
	int width = src_mat.cols;
	int height = src_mat.rows;

	CString str;

	double* plfDataX = new double[n];
	double* plfDataY = new double[n];
	int i, j;
	double* plfData = new double[width];
	//////////////////////////////////////////////////////////////////////////
	//y-2��y-1,y,y+1,y+2������ȡ��ֵ,��¼��plfData
	for (i = 0; i < width; i++)
	{
		plfData[i] = .0;
		y = y > height - 3 ? height - 3 : y;
		for (j = -2; j <= 2; j++)
		{
			plfData[i] += pData[(y + j) * width + i];
		}
		plfData[i] /= 5.;
	}
	//////////////////////////////////////////////////////////////////////////
	//ˮƽ������
	for (i = 0; i < width - 1; i++)
	{
		plfData[i] = abs(plfData[i + 1] - plfData[i]);
	}
	//���һ����Ϊ0
	plfData[width - 1] = 0;
	//////////////////////////////////////////////////////////////////////////
	//ѡȡ�㸽�������ֵdMax������¼���ֵλ��t
	double dMax = plfData[x];
	int t = x;
	for (i = 0; i < 50; i++)
	{
		if (dMax < plfData[x - i])
		{
			dMax = plfData[x - i];
			t = x - i;
		}
		if (dMax < plfData[x + i])
		{
			dMax = plfData[x + i];
			t = x + i;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	i = t - n / 2;
	if (i < 0)
	{
		i = 0;
	}
	if (i + n > width)
	{
		i = width - n;
	}
	for (j = 0; j < n; j++)
	{
		//plfDataX[j] = plfData[j + i] > 1500 ? plfData[j + i] : 0;
		//plfDataX[j] = plfData[j + i] > 10 ? plfData[j + i] : 0;
		plfDataX[j] = plfData[j + i];
	}

	int w = 1;
	int wp = 0;
	while (w * 2 <= n)
	{
		w *= 2;
		wp++;
	}
	complex<double> *TD = new complex<double>[w];
	complex<double> *FD = new complex<double>[w];
	double D = 0;
	double C = 0;
	double S = 0;
	for (i = 0; i < w; i++)
	{
		TD[i] = complex<double>(plfDataX[i], 0);
		//���㵶�ں���������X����Χ���
		D += plfDataX[i];
	}
	//����Ҷ�任
	FFT(TD, FD, wp);

	m_pdMtfX = new double[n / 2];
	m_pdMtfY = new double[n / 2];
	for (i = 0; i < n / 2; i++)
	{
		//Ƶ����ȡģ
		m_pdMtfY[i] = sqrtf(FD[i].real() * FD[i].real() + FD[i].imag() * FD[i].imag()) / D;
	}
	for (i = 0; i < n / 2; i++)
	{
		m_pdMtfX[i] = i * 2 * dZool / n / dPixel;
	}


	delete[] plfData;
	delete[] TD;
	delete[] FD;
}




CMtf::~CMtf(void)
{
}


void CMtf::FFT(complex<double> * TD, complex<double> * FD, int r)
{
	LONG	count;				// ����Ҷ�任����	
	int		i, j, k;				// ѭ������
	int		bfsize, p;
	double	angle;				// �Ƕ�	
	complex<double> *W, *X1, *X2, *X;

	count = 1 << r;				// ���㸶��Ҷ�任����

								// ������������洢��
	W = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];

	// �����Ȩϵ��
	for (i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	// ��ʱ���д��X1
	memcpy(X1, TD, sizeof(complex<double>) * count);

	// ���õ����㷨���п��ٸ���Ҷ�任
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r - k);
			for (i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2])
					* W[i * (1 << k)];
			}
		}
		X = X1;
		X1 = X2;
		X2 = X;
	}

	// ��������
	for (j = 0; j < count; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j&(1 << i))
			{
				p += 1 << (r - i - 1);
			}
		}
		FD[j] = X1[p];
	}

	delete W;
	delete X1;
	delete X2;
}


void CMtf::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	LONG	count;					// ����Ҷ�任����	
	int		i;						// ѭ������	
	complex<double> *X;

	count = 1 << r;					// ���㸶��Ҷ�任����
	X = new complex<double>[count];	// ������������洢��
	memcpy(X, FD, sizeof(complex<double>) * count);	// ��Ƶ���д��X

													// ����
	for (i = 0; i < count; i++)
	{
		X[i] = complex<double>(X[i].real(), -X[i].imag());
	}

	FFT(X, TD, r);					// ���ÿ��ٸ���Ҷ�任

									// ��ʱ���Ĺ���
	for (i = 0; i < count; i++)
	{
		TD[i] = complex<double>(TD[i].real() / count, -TD[i].imag() / count);
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