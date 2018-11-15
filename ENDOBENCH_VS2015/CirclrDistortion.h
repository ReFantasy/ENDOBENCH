
/************************************************************************/
/* ����Բ�ߵĻ���                                                        */
/************************************************************************/

#ifndef __CirclrDistortion_H__
#define __CirclrDistortion_H__

#include <iostream>
#include <opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

typedef struct circle
{
	cv::Point2f coordinate;  //����
	double diameter;  //ֱ��
}mcircle;

typedef vector<mcircle> Circleset;  //Բ�ļ�������


									//������
class CirclrDistortion
{
public:
	//����
	CirclrDistortion(const Mat &m_srcimage, const uchar &m_threshold, const double &m_Radius, const Circleset &m_circle_set, int m_padding = 5)
		:srcimage(m_srcimage), threshold(m_threshold), Radius(m_Radius), circle_set(m_circle_set), padding(m_padding)
	{
		if (!srcimage.data)
			return;
		//תΪ�Ҷ�ͼ
		if (srcimage.channels() != 1)
			cvtColor(srcimage, srcimage, CV_BGR2GRAY);
		//��ֵ��
		Mat_<uchar>::iterator iter = srcimage.begin<uchar>();
		while (iter++ != srcimage.end<uchar>())
			(*iter) = (*iter) > threshold ? 255 : 0;

		Mat grayimg;
		srcimage.copyTo(grayimg);
		srcimage.release();
		grayimg.copyTo(srcimage);
		//imshow("srcimage", srcimage);
		//waitKey(0);
	}

public:
	void computeDistortion()
	{
		//��������Բ�ߵ�ֱ��
		double radius_0 = computeDiameter(circle_set[0], 0);  //����
		double radius_1 = computeDiameter(circle_set[0], 1);  //����
		if (radius_0 == 0 || radius_1 == 0)
			return;
		//imshow("srcimage", srcimage);

		//��������Բ�ߵ�ֱ��
		computeDiameter(circle_set[1], 1);
		computeDiameter(circle_set[3], 1);
		computeDiameter(circle_set[2], 0);
		computeDiameter(circle_set[4], 0);

		//��������С
		Top = (circle_set[1].diameter - radius_1) / radius_1;
		Bottom = (circle_set[3].diameter - radius_1) / radius_1;
		Right = (circle_set[2].diameter - radius_0) / radius_0;
		Left = (circle_set[4].diameter - radius_0) / radius_0;
	}
	int computeDiameter(struct circle & circle, int m_flag) //0������� 1��������
	{
		if (!srcimage.data)
			return 0;
		//��ȡROI
		int flag = m_flag;

		cv::Rect rect = cv::Rect(circle.coordinate.x - Radius - padding, circle.coordinate.y - Radius - padding
			, Radius * 2 + padding * 2, Radius * 2 + padding * 2);

		if ((rect.x + rect.width) > srcimage.cols | (rect.y + rect.height) > srcimage.rows | rect.x < 0 | rect.y < 0)
			return 0.00000001;

		Mat roi = srcimage(rect);

		if (flag == 0)
		{
			int left = 2 * Radius;
			int right = -1;

			for (int row = 0; row < roi.rows; row++)
			{
				for (int col = 0; col < roi.cols; col++)
				{
					if (roi.at<uchar>(row, col) < threshold) //��ɫ
					{
						if (col < left)
							left = col;
						if (col > right)
							right = col;
					}
				}
			}

			circle.diameter = right - left + 1;
			if (circle.diameter <= 0)
				circle.diameter = 0.0000000001;
			return circle.diameter;
		}

		if (flag == 1)
		{
			int top = 2 * Radius;
			int bottom = -1;

			for (int row = 0; row < roi.rows; row++)
			{
				for (int col = 0; col < roi.cols; col++)
				{
					if (roi.at<uchar>(row, col) < threshold) //��ɫ
					{
						//cout<<"flag1  "<<row<<" "<<col<<endl;
						if (row < top)
							top = row;
						if (row > bottom)
							bottom = row;
					}
				}
			}

			circle.diameter = bottom - top + 1;
			if (circle.diameter <= 0)
				circle.diameter = 0.0000000001;
			return circle.diameter;
		}
	}
	void getResult(double &m_Top, double &m_Right, double &m_Bottom, double &m_Left)
	{
		m_Top = Top;
		m_Right = Right;
		m_Bottom = Bottom;
		m_Left = Left;
	}

private:
	Mat srcimage;
	int threshold;
	double Radius;  //�뾶��׼ֵ
	int padding;
	Circleset circle_set;  //Բ�ļ���

	double Top, Right, Bottom, Left;
};

#endif