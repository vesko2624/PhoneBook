#pragma once
#include "CListSort.h"
#include "afxcmn.h"

static int BinarySearchListColumn(SortParams& oSortParams, CString strDesiredString)
{
    bool bIsAscending = (oSortParams.nSortOrder == AscendingOrder || oSortParams.nSortOrder == NoneOrder);

	CListCtrl& oListCtrl = oSortParams.oListCtrl;
	int nLeft = 0, nRight = oListCtrl.GetItemCount() - 1;
    while (nLeft <= nRight) {
        bool bCompareCondition;
        int nMiddle = nLeft + (nRight - nLeft) / 2;
        CString strItemColumn = oListCtrl.GetItemText(nMiddle, oSortParams.nSortColumn);

        bCompareCondition = (bIsAscending) ? (strItemColumn < strDesiredString) : (strItemColumn > strDesiredString);
            
        if (strItemColumn == strDesiredString)
        {
            return nMiddle;
        }
        if (bCompareCondition)
        {
            nLeft = nMiddle + 1;
        }
        else
        {
            nRight = nMiddle - 1;
        }
    }

    return -1;
}