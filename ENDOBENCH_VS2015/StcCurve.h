#pragma once
#include <opencv.hpp>
#include <iostream>
#include <algorithm>
using namespace std;

// CStcCurve
#define WIDE 30//绘图边距
/************************************************************************
*
*　 文件名：StcCurve.h
*
*　 文件描述：曲线绘制类，可以设置坐标轴刻度、坐标曲线
*   使用方法：
*		1.Remove			//若需要清除前次绘制记录
*		2.AddInput			//添加坐标点
*		3.SetCoordinate		//设置坐标轴显示值
*
*
*　 创建人：  swl
*
*　 版本号：1.0
*
*　 修改记录：
*		2017年12月28日整理完毕
*
************************************************************************/
typedef struct tag_STRUCT_CURVE
{
	double* x;
	double* y;
	int size;
}STRUCT_CURVE;

typedef CArray<STRUCT_CURVE, STRUCT_CURVE> CCurveArray;

class CStcCurve : public CStatic
{
	DECLARE_DYNAMIC(CStcCurve)

public:
	CStcCurve();
	virtual ~CStcCurve();
	//数据初始化，根据客户区大小创建图像，设置位图文件信息
	void InitData(void);
protected:
	DECLARE_MESSAGE_MAP()
	//在图像src上，点pt1,pt2之间绘制虚线，虚线颜色为color宽度为thinkness
	void DotLine(IplImage* src, CvPoint pt1, CvPoint pt2, CvScalar color, int thinkness = 1);
	//拟合
	BOOL CalculateCurveParameter(double *Xval, double *Yval, long M, long N, double *A);
	//将图像显示在pDC
	void Draw(CDC* pDC);
	//画坐标轴和曲线
	void DrawCoordinate(void);
	
	//图像
	IplImage* m_pImg;
	//图像像素数据
	//	LPVOID m_lpDib;
	//图像信息头
	LPBITMAPINFO m_lpBmpInfo;
	//坐标轴刻度
	int m_nX;//X轴坐标数量
	int m_nY;//Y轴坐标数量
	double* m_pdX;//x轴坐标刻度值
	double* m_pdY;//
				  //
	bool m_bIsNihe;//是否拟合

	bool m_bFlag;

	CCurveArray m_CurveArray;//曲线参数
public:
	//删除所有坐标点
	void Remove(void);
	//添加x、y坐标，绘制曲线
	void AddInput(double* x, double* y, int size, bool bFlag = FALSE, bool bFlag2 = FALSE);
	//设置坐标轴刻度
	void SetCoordinate(double* x, double* y, int nx, int ny);
	//重绘
	afx_msg void OnPaint();
	double Middle(double a, double b, double c);
private:
};


