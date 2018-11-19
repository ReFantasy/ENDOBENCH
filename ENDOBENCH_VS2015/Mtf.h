#ifndef __MTF_H__
#define __MTF_H__
#include <opencv.hpp>
#include <math.h>
#include <complex>
using namespace std;


#define PI 3.1415926

//传函计算
class CMtf
{
public:
	/*************************************************************************
	* 函数名称：
	*   CMtf()
	* 参数:
	*   IplImage* src		- 图像
	*   int n				- 存在配置文件中
	*	int x				- 选择框的中心x
	*	int y				- 选择框的中心y
	*	double dPixel		- 在配置文件中
	*	double dZool		- 在配置文件中
	* 返回值:
	*   无。
	* 说明:
	*   实现光学传递函数计算。
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

#endif // __MTF_H__ 头文件
