#pragma once
#include "ErrorHandler.h"
#include "framework.h"

enum SortOrder
{
	AscendingOrder,
	DescendingOrder,
	NoneOrder
};

struct SortParams
{
	CListCtrl& oListCtrl;
	int nSortColumn;
	SortOrder nSortOrder;
	SortParams(CListCtrl& oListCtrl, int nSortColumn, SortOrder nSortOrder)
		: oListCtrl(oListCtrl)
	{
		this->nSortColumn = nSortColumn;
		this->nSortOrder = nSortOrder;
	}
};

static int CALLBACK ListCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SortParams* pSortParams = (SortParams*)lParamSort;
	if (pSortParams == NULL)
	{
		NullPointerCaught(__LINE__, _T(__FILE__));
		return 0;
	}

	CListCtrl& oListCtrl = pSortParams->oListCtrl;
	if (pSortParams->nSortOrder == NoneOrder)
	{
		long nId1 = static_cast<long>(oListCtrl.GetItemData(static_cast<int>(lParam1)));
		long nId2 = static_cast<long>(oListCtrl.GetItemData(static_cast<int>(lParam2)));

		if (nId1 < nId2)
			return -1;
		return 1;
	}

	CString strItem1 = oListCtrl.GetItemText(static_cast<int>(lParam1), (int)pSortParams->nSortColumn);
	CString strItem2 = oListCtrl.GetItemText(static_cast<int>(lParam2), (int)pSortParams->nSortColumn);

	int nResult = wcscmp(strItem1, strItem2);
	if (pSortParams->nSortOrder == DescendingOrder) nResult *= -1;

	return nResult;
}