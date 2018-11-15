/*
 *  ��������ͷ�ļ�
 *
 *
 */

#ifndef __AUXILIB_H__
#define __AUXILIB_H__
#include <windows.h>
#include <opencv.hpp>
#include "camera.h"

using namespace cv;
// ����ȡ����ͼ�����䵽��ʾ����
cv::Rect AdaptTo(cv::Rect SrcRect, cv::Rect DestRect);

// ����ȡ��������ӳ���ʵ�ʵ�ͼ������
cv::Rect MapToRawImage(CRect intercept_rect, CRect video_window_rect);

//���ͼ��תΪOpencvͼ��
void ConvertToMatImage(const Image &rawImage, Mat &DstImg);


#endif // __AUXILIB_H__
