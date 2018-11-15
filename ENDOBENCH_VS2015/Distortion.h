#pragma once


// Distortion 对话框

class DistortionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DistortionDlg)

public:
	DistortionDlg(CWnd* pParent = NULL);   // 标准构造函数
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
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Distortion };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	double ComputeDiameter(const cv::Mat img, int dic); // dic 1 竖向 2 横向
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
