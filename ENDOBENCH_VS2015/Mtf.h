#ifndef __MTF_H__
#define __MTF_H__
#include <opencv.hpp>
#include <math.h>
#include <complex>
using namespace std;


#define PI 3.1415926

//��������
class CMtf
{
public:
	/*************************************************************************
	* �������ƣ�
	*   CMtf()
	* ����:
	*   IplImage* src		- ͼ��
	*   int n				- ���������ļ���
	*	int x				- ѡ��������x
	*	int y				- ѡ��������y
	*	double dPixel		- �������ļ���
	*	double dZool		- �������ļ���
	* ����ֵ:
	*   �ޡ�
	* ˵��:
	*   ʵ�ֹ�ѧ���ݺ������㡣
	************************************************************************/
	CMtf(IplImage* src, int n, int x, int y, double dPixel = 1, double dZool = 1);
	~CMtf(void);
protected:
	void IFFT(complex<double> * FD, complex<double> * TD, int r);
	void FFT(complex<double> * TD, complex<double> * FD, int r);
	int m_nNum;
	double* m_pdDaokou;
	double* m_pdMtfX;
	double* m_pdMtfY;

public:
	double* GetMtfX(int& nNum);
	double* GetMtfY(int& nNum);
};

#endif // __MTF_H__ ͷ�ļ�
