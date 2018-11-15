/*
 *  辅助函数头文件
 *
 *
 */

#ifndef __AUXILIB_H__
#define __AUXILIB_H__
#include <windows.h>
#include <opencv.hpp>
#include "camera.h"

using namespace cv;
// 将读取到的图像适配到显示窗口
cv::Rect AdaptTo(cv::Rect SrcRect, cv::Rect DestRect);

// 将截取到的区域映射回实际的图像区域
cv::Rect MapToRawImage(CRect intercept_rect, CRect video_window_rect);

//相机图像转为Opencv图像
void ConvertToMatImage(const Image &rawImage, Mat &DstImg);


#endif // __AUXILIB_H__
