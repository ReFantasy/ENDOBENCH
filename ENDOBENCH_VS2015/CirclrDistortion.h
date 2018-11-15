
/************************************************************************/
/* 计算圆斑的畸变                                                        */
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
	cv::Point2f coordinate;  //坐标
	double diameter;  //直径
}mcircle;

typedef vector<mcircle> Circleset;  //圆的集合类型


									//畸变类
class CirclrDistortion
{
public:
	//构造
	CirclrDistortion(const Mat &m_srcimage, const uchar &m_threshold, const double &m_Radius, const Circleset &m_circle_set, int m_padding = 5)
		:srcimage(m_srcimage), threshold(m_threshold), Radius(m_Radius), circle_set(m_circle_set), padding(m_padding)
	{
		if (!srcimage.data)
			return;
		//转为灰度图
		if (srcimage.channels() != 1)
			cvtColor(srcimage, srcimage, CV_BGR2GRAY);
		//二值化
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
		//计算中心圆斑的直径
		double radius_0 = computeDiameter(circle_set[0], 0);  //横向
		double radius_1 = computeDiameter(circle_set[0], 1);  //纵向
		if (radius_0 == 0 || radius_1 == 0)
			return;
		//imshow("srcimage", srcimage);

		//计算其它圆斑的直径
		computeDiameter(circle_set[1], 1);
		computeDiameter(circle_set[3], 1);
		computeDiameter(circle_set[2], 0);
		computeDiameter(circle_set[4], 0);

		//计算畸变大小
		Top = (circle_set[1].diameter - radius_1) / radius_1;
		Bottom = (circle_set[3].diameter - radius_1) / radius_1;
		Right = (circle_set[2].diameter - radius_0) / radius_0;
		Left = (circle_set[4].diameter - radius_0) / radius_0;
	}
	int computeDiameter(struct circle & circle, int m_flag) //0计算横向 1计算纵向
	{
		if (!srcimage.data)
			return 0;
		//获取ROI
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
					if (roi.at<uchar>(row, col) < threshold) //黑色
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
					if (roi.at<uchar>(row, col) < threshold) //黑色
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
	double Radius;  //半径标准值
	int padding;
	Circleset circle_set;  //圆的集合

	double Top, Right, Bottom, Left;
};

#endif