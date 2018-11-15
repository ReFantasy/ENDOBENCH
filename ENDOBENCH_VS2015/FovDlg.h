#pragma once


// FovDlg 对话框

class FovDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FovDlg)

public:
	FovDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FovDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FOV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double D1;
	double D2;
	double d1;
	double d2;
	double rfov;
	double a;
	double vfov;
	double dov;
	afx_msg void OnBnClickedButton1();
	void UpdateTreeControl();
};
