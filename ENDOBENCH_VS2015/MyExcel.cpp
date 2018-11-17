// MyExcel.cpp: implementation of the CMyExcel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "DSCAMDEMO.h"
#include "MyExcel.h"
#include "excel.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace myexcel;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMyExcel::CMyExcel()
{
}

CMyExcel::~CMyExcel()
{

}
void CMyExcel::SetInput(char* fileName)
{
	// 打开新文件
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	CString sValue = "";
	_Application objApp;
	_Workbook objBook;
	Workbooks objBooks;
	Worksheets objSheets;
	_Worksheet objSheet;
	myexcel::Range objRange;

	objApp.SetAlertBeforeOverwriting(TRUE);
	objApp.SetDisplayAlerts(TRUE);

	objApp.CreateDispatch("Excel.Application");
	objBooks = objApp.GetWorkbooks();
	try
	{
		objBook = objBooks.Open(fileName, VOptional, VOptional, VOptional, VOptional,
			VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional,
			VOptional, VOptional, VOptional);
	}
	catch (CMemoryException* e)
	{

		AfxMessageBox("1");
	}
	catch (CFileException* e)
	{
		AfxMessageBox("2");
	}
	catch (CException* e)
	{
		AfxMessageBox("3");
	}
	objSheets = objBook.GetWorksheets();
	objSheet = objSheets.GetItem(COleVariant((short)1));
	//读取待测数据
	for (int i = 0; i <= 80; i++)
	{
		objRange.AttachDispatch(objSheet.GetCells());
		objRange.AttachDispatch(objRange.GetItem(COleVariant((long)(i + 2)), COleVariant((long)2)).pdispVal);
		m_VarLED4[i] = objRange.GetValue2();
	}
	objRange.ReleaseDispatch();
	// 关闭文件
	objRange.Clear();
	objBook.ReleaseDispatch();
	objBook.Close(COleVariant((short)FALSE), VOptional, VOptional);
	objBooks.ReleaseDispatch();
	objBooks.Close();
	objApp.Quit();
	objApp.ReleaseDispatch();
}
void CMyExcel::GetOutput(double& ctt, double& cier)
{
	// 取模板计算器
	char criPath[MAX_PATH];
	//获取应用程序路径
	if (!GetModuleFileName(NULL, criPath, sizeof(criPath)))
	{
		return;
	}
	char *pDest = strrchr(criPath, '\\');
	if (!pDest)
	{
		return;
	}
	pDest[0] = '\0';

	if (criPath[strlen(criPath) - 1] != '\\')
	{
		strcat(criPath, "\\CRI1.xls");
	}
	else
	{
		strcat(criPath, "CRI1.xls");
	}
	criPath[sizeof(criPath) - 1] = '\0';

	// 打开计算器
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	CString sValue = "";
	_Application objApp;
	_Workbook objBook;
	Workbooks objBooks;
	Worksheets objSheets;
	_Worksheet objSheet;
	myexcel::Range objRange;
	VARIANT vRet;

	objApp.SetAlertBeforeOverwriting(TRUE);
	objApp.SetDisplayAlerts(TRUE);

	objApp.CreateDispatch("Excel.Application");
	objBooks = objApp.GetWorkbooks();
	try
	{
		objBook = objBooks.Open(criPath, VOptional, VOptional, VOptional, VOptional,
			VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional,
			VOptional, VOptional, VOptional);
	}
	catch (CMemoryException* e)
	{

		AfxMessageBox("1");
	}
	catch (CFileException* e)
	{
		AfxMessageBox("2");
	}
	catch (CException* e)
	{
		AfxMessageBox("3");
	}
	objSheets = objBook.GetWorksheets();
	objSheet = objSheets.GetItem(COleVariant((short)1));
	objRange.AttachDispatch(objSheet.GetCells(), true);
	for (int i = 0; i <= 80; i++)
	{
		objRange.SetItem(COleVariant((long)(i + 5)), COleVariant((long)4), m_VarLED4[i]);
	}
	//获取待测数据色温
	objRange.AttachDispatch(objSheet.GetCells());
	objRange.AttachDispatch(objRange.GetItem(COleVariant((long)4), COleVariant((long)9)).pdispVal);
	ctt = objRange.GetValue2().dblVal;
	//获取待测数据显色指数
	objRange.AttachDispatch(objSheet.GetCells());
	objRange.AttachDispatch(objRange.GetItem(COleVariant((long)7), COleVariant((long)9)).pdispVal);
	cier = objRange.GetValue2().dblVal;

	objRange.ReleaseDispatch();
	// 关闭文件
	objRange.Clear();
	objBook.Save();
	objBook.ReleaseDispatch();
	objBook.Close(COleVariant((short)FALSE), VOptional, VOptional);
	objBooks.ReleaseDispatch();
	objBooks.Close();
	objApp.Quit();
	objApp.ReleaseDispatch();
}
