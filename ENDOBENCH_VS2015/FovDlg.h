#pragma once


// FovDlg �Ի���

class FovDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FovDlg)

public:
	FovDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FovDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FOV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
