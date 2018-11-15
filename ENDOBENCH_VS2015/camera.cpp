#include "stdafx.h"
#include "camera.h"
#include <string>

std::mutex mutex_thread;
std::mutex mutex_buffer_image;
SingletonCamera* SingletonCamera::m_instance = nullptr;

SingletonCamera* SingletonCamera::GetInstance()
{
	std::lock_guard<std::mutex> lock(mutex_thread);
	if (!m_instance)
	{
		m_instance = new SingletonCamera();
	}

	return m_instance;
}

bool SingletonCamera::StartCamera()
{
	if (cam.IsConnected())
		return true;

	Error error;
	BusManager busMgr;
	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);
	PGRGuid guid;
	error = busMgr.GetCameraFromIndex(0, &guid);
	const int k_numImages = 10;



	// Connect to a camera
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		AfxMessageBox(_T("����ʧ��"));
		return false;
	}


	// Get the camera configuration
	FC2Config config;
	error = cam.GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		AfxMessageBox(_T("��ȡ����ʧ��"));
		return false;
	}

	// Set the number of driver buffers used to 10.
	config.numBuffers = 30;

	// Set the camera configuration
	error = cam.SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		AfxMessageBox(_T("��������ʧ��"));
		return false;
	}

	// Start capturing images
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		AfxMessageBox(_T("�ɼ�ʧ��"));
		return false;
	}
	return true;
}

bool SingletonCamera::StopCamera()
{
	if (!cam.IsConnected())
		return true;
	Error error = cam.Disconnect();
	return (error != PGRERROR_OK) ? false : true;
}

bool SingletonCamera::IsConnected()
{
	return cam.IsConnected();
}

ImageSize SingletonCamera::GetRawImageSize()
{
	CameraInfo caminfo;
	cam.GetCameraInfo(&caminfo);
	std::string sensorResolution(caminfo.sensorResolution);

	// ���ַ�������ȡ��͸�
	int height = 0;
	int width = 0;
	int px_index = 0;
	for (int i = 0; i < sensorResolution.size(); i++)
	{ // ��ȡheight
		if ((sensorResolution[i] != 'x') && (sensorResolution[i] != 'X'))
		{
			width *= 10;
			width += (sensorResolution[i] - '0');
		}
		else
		{
			px_index = i;
			break;;
		}
	}
	for (int i = px_index + 1; i < sensorResolution.size(); i++)
	{// ��ȡwidth
		height *= 10;
		height += (sensorResolution[i] - '0');
	}
	ImageSize image_sz;
	image_sz.height = height;
	image_sz.width = width;
	return image_sz;
}

Error SingletonCamera::RetrieveBuffer(image_type *buffer_image)
{
	std::lock_guard<std::mutex> lock(mutex_buffer_image);
	return cam.RetrieveBuffer(buffer_image);//���ڴ�ȡ��ͼ��浽rawImage����
}
