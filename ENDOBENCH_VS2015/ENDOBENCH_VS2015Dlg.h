
// ENDOBENCH_VS2015Dlg.h : ͷ�ļ�
//

#pragma once


// CENDOBENCH_VS2015Dlg �Ի���
class CENDOBENCH_VS2015Dlg : public CDialogEx
{
// ����
public:
	CENDOBENCH_VS2015Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENDOBENCH_VS2015_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
