
// ENDOBENCH_VS2015Dlg.h : 头文件
//

#pragma once
#include <thread>
#include "Auxilib.h"
#include "afxcmn.h"
#include "StcCurve.h"

#include "FovDlg.h"
#include "ResolutionDlg.h"
#include "Distortion.h"
#include "afxwin.h"
#include "SerialPort.h"




extern CString config_path;

#define FOV 1
#define Resolution 2
#define Distortion 3
#define Detection 4

enum class Status { SFOV, SResolution, SDistortion, SDetection };
enum class DistortionSelected { top, bottom, left, right, center, None };

// 串口对象
extern itas109::CSerialPort serial_port;

// CENDOBENCH_VS2015Dlg 对话框
class CENDOBENCH_VS2015Dlg : public CDialogEx
{
// 构造
public:
	CENDOBENCH_VS2015Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENDOBENCH_VS2015_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnReceiveStr(WPARAM str, LPARAM commInfo);
public:
	// show image to Picture Control whose id is ID
	void ShowImage(IplImage* image, UINT ID);
	void ShowImage(const Mat &image, UINT ID);
	void DrawInPictureControl(Status status, UINT ID);
	std::thread *pthread = nullptr;  // 线程：读取相机图像
	CRect intercept_rect;  // 截取Picture控件区域
	
public:
	afx_msg void OnBnClickedBtntest();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	// 树形控件
	CTreeCtrl tree_control;
	void InitTreeControl();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);

	// 对话框资源
	Status status = Status::SFOV;
	DistortionSelected distortion_selected = DistortionSelected::None;
	FovDlg fov_dlg;
	ResolutionDlg resolution_dlg;
	DistortionDlg distortion_dlg;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CStcCurve m_StcCurve;
	afx_msg void OnStnClickedStaticCurve();

	
	CButton btn_SelPortOpenCloseCtrl;
	afx_msg void OnBnClickedButton1();
	CEdit edit_SelPortNO;
	CEdit m_ReceiveCtrl;
	
	afx_msg void OnBnClickedButton2();
	// 显色性编辑框控制变量
	CEdit color_coef;
};
// 视频线程回调函数
void VideoThreadFunc(CENDOBENCH_VS2015Dlg *pMainthread);
