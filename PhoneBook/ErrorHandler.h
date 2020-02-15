#pragma once
#include <afxtempl.h>

//	<summary>
//	Message output when null pointer is caught
//	</summary>
//	<param name="nLine"> Line of Error </param>
//	<param name="strFile"> File Name </param>
static void NullPointerCaught(int nLine, CString strFile)
{
	CString strErrorMessage;
	strErrorMessage.Format(_T("NULL Pointer caught at line %d in %s"), nLine, (LPCTSTR)strFile);
	OutputDebugStringW(strErrorMessage);
}