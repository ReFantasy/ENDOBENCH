#include "stdafx.h"
#include "Auxilib.h"


cv::Rect AdaptTo(cv::Rect SrcRect, cv::Rect DestRect)
{
	cv::Rect restult;
	double scale_dst = DestRect.height / (double)DestRect.width;
	double scale_src = SrcRect.height / (double)SrcRect.width;
	if (scale_src < scale_dst)
	{
		restult.width = DestRect.width;
		restult.height = DestRect.width*SrcRect.height / SrcRect.width;
	}
	else
	{
		restult.height = DestRect.height;
		restult.width = DestRect.height*SrcRect.width / SrcRect.height;
	}
	return restult;
}

cv::Rect MapToRawImage(CRect intercept_rect, CRect video_window_rect)
{
	if (intercept_rect.left < 0 || intercept_rect.top < 0||intercept_rect.Width()<=0||intercept_rect.Height()<=0)
		return cv::Rect(cv::Point2i(0,0), cv::Point2i(10, 10));
	int rw = video_window_rect.right - video_window_rect.left;
	int rh = video_window_rect.bottom - video_window_rect.top;

	// 图像原始大小
	int iw = SingletonCamera::GetInstance()->GetRawImageSize().width;
	int ih = SingletonCamera::GetInstance()->GetRawImageSize().height;

	// 图像适配到控件的位置
	cv::Rect img; img.width = iw; img.height = ih;
	cv::Rect picture; picture.width = video_window_rect.Width(); picture.height = video_window_rect.Height();
	auto adapt_rect = AdaptTo(img, picture);
	iw = adapt_rect.width;     // 适配后的图像大小
	ih = adapt_rect.height;

	CRect adapt_position;
	int tx = (int)(rw - iw) / 2;
	int ty = (int)(rh - ih) / 2;
	SetRect(adapt_position, tx, ty, tx + iw, ty + ih);

	// 计算截图图像在原始图像中的比例位置
	double dx = (intercept_rect.left-adapt_position.left) / (double)adapt_position.Width();
	double dy = (intercept_rect.top-adapt_position.top) / (double)adapt_position.Height();
	double xscale = intercept_rect.Width() / (double)adapt_position.Width();
	double yscale = intercept_rect.Height() / (double)adapt_position.Height();

	auto image_size = SingletonCamera::GetInstance()->GetRawImageSize();
	cv::Rect cv_result;
	cv_result.x = dx * image_size.width;
	cv_result.y = dy*image_size.height;
	cv_result.width = xscale*image_size.width;
	cv_result.height = yscale*image_size.height;
	return cv_result;
}

void ConvertToMatImage(const Image &rawImage, Mat &DstImg)
{
	// convert to rgb
	Image rgbImage;
	rawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage);

	// convert to OpenCV Mat
	unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize() / (double)rgbImage.GetRows();
	cv::Mat image = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(), rowBytes);
	image.copyTo(DstImg);
}


