#pragma once


// Distortion �Ի���

class DistortionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DistortionDlg)

public:
	DistortionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DistortionDlg();

	double top_x = 10;
	double top_y = 10;
	double bottom_x = 10;
	double bottom_y = 10;
	double left_x = 10;
	double left_y = 10;
	double right_x = 10;
	double right_y = 10;
	double center_x = 10;
	double center_y = 10;
	double circle_radius = 10;
	void ReadCircleCrd();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Distortion };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	double ComputeDiameter(const cv::Mat img, int dic); // dic 1 ���� 2 ����
	void UpdateTreeControl();
	DECLARE_MESSAGE_MAP()
public:
	double Top;
	double Right;
	double Bottom;
	double Left;
	double Avg;
	double Max;
	double StdDev;
	afx_msg void OnBnClickedButton2();
};
