// MyExcel.h: interface for the CMyExcel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEXCEL_H__56833ED1_1A75_42E9_BAFD_21F2771EF18A__INCLUDED_)
#define AFX_MYEXCEL_H__56833ED1_1A75_42E9_BAFD_21F2771EF18A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyExcel  
{
public:
	void SetInput(char* fileName);
	void GetOutput(double& ctt, double& cier);

	CMyExcel();
	virtual ~CMyExcel();
protected:

	//光谱功率分布
	COleVariant		m_VarLED4[81];


public:
};

#endif // !defined(AFX_MYEXCEL_H__56833ED1_1A75_42E9_BAFD_21F2771EF18A__INCLUDED_)
