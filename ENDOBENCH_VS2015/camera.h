/**
 * ����ӿ���
 * ���ģʽ������ģʽ
 * 2018/11/10 ̷����
 *
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "stdafx.h"
#include <FlyCapture2.h>
#include <FlyCapture2GUI.h>
#include <mutex>
using namespace FlyCapture2;

extern std::mutex mutex_thread; 
extern std::mutex mutex_buffer_image;
typedef cv::Rect ImageSize;
class SingletonCamera
{
public:
	using image_type = FlyCapture2::Image;
	static SingletonCamera* GetInstance();
	static SingletonCamera* m_instance;
	bool StartCamera();
	bool StopCamera();
	bool IsConnected();
	ImageSize GetRawImageSize();
	Error RetrieveBuffer(image_type *buffer_image);  // ��ȡ���ͼ��
	
private:
	
private:
	SingletonCamera() = default;
	SingletonCamera(const SingletonCamera&);
	GigECamera cam;
};

#endif // __CAMERA_H__

