
// ENDOBENCH_VS2015Dlg.h : 头文件
//

#pragma once


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
};
