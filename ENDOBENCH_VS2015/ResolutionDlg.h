#pragma once


// ResolutionDlg �Ի���

class ResolutionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ResolutionDlg)

public:
	ResolutionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ResolutionDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Resolution };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	double or_center;
	double or_first;
	double or_second;
	double or_third;
	double or_fourth;
	double or_avg;
	double or_min;
	double or_stddev;
	double or_d;

	double ar_center;
	double ar_first;
	double ar_second;
	double ar_third;
	double ar_fourth;
	double ar_avg;
	double ar_min;
	double ar_stddev;
	double ar_d;
};
