#pragma once
#include <opencv.hpp>
#include <iostream>
#include <algorithm>
using namespace std;

// CStcCurve
#define WIDE 30//��ͼ�߾�
/************************************************************************
*
*�� �ļ�����StcCurve.h
*
*�� �ļ����������߻����࣬��������������̶ȡ���������
*   ʹ�÷�����
*		1.Remove			//����Ҫ���ǰ�λ��Ƽ�¼
*		2.AddInput			//��������
*		3.SetCoordinate		//������������ʾֵ
*
*
*�� �����ˣ�  swl
*
*�� �汾�ţ�1.0
*
*�� �޸ļ�¼��
*		2017��12��28���������
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
	//���ݳ�ʼ�������ݿͻ�����С����ͼ������λͼ�ļ���Ϣ
	void InitData(void);
protected:
	DECLARE_MESSAGE_MAP()
	//��ͼ��src�ϣ���pt1,pt2֮��������ߣ�������ɫΪcolor���Ϊthinkness
	void DotLine(IplImage* src, CvPoint pt1, CvPoint pt2, CvScalar color, int thinkness = 1);
	//���
	BOOL CalculateCurveParameter(double *Xval, double *Yval, long M, long N, double *A);
	//��ͼ����ʾ��pDC
	void Draw(CDC* pDC);
	//�������������
	void DrawCoordinate(void);
	
	//ͼ��
	IplImage* m_pImg;
	//ͼ����������
	//	LPVOID m_lpDib;
	//ͼ����Ϣͷ
	LPBITMAPINFO m_lpBmpInfo;
	//������̶�
	int m_nX;//X����������
	int m_nY;//Y����������
	double* m_pdX;//x������̶�ֵ
	double* m_pdY;//
				  //
	bool m_bIsNihe;//�Ƿ����

	bool m_bFlag;

	CCurveArray m_CurveArray;//���߲���
public:
	//ɾ�����������
	void Remove(void);
	//���x��y���꣬��������
	void AddInput(double* x, double* y, int size, bool bFlag = FALSE, bool bFlag2 = FALSE);
	//����������̶�
	void SetCoordinate(double* x, double* y, int nx, int ny);
	//�ػ�
	afx_msg void OnPaint();
	double Middle(double a, double b, double c);
private:
};


