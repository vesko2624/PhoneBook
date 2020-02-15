#pragma once
#include "afxtempl.h"

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CIterableMap : public CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>
{
// Constants
// ----------------
	typedef CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> MAP;
	typedef INT_PTR INDEX_TYPE;

// Constructor / Destructor
// ----------------
public:
	CIterableMap()
	{

	}

	virtual ~CIterableMap()
	{
		ClearMap();
	}

	void ClearMap()
	{
		while (oCurrentPos = MAP::GetStartPosition())
		{

			KEY oKey;
			VALUE oValue;
			MAP::GetNextAssoc(oCurrentPos, oKey, oValue);
			if (oValue != NULL)
			{
				delete oValue;
			}
			MAP::RemoveKey(oKey);
		}
	}
	bool GetAtIndex(INDEX_TYPE nIndex, VALUE& oValue)
	{
		if (nCurrentPos == -1 || nIndex > nCurrentPos)
		{
			if (nIndex > nCurrentPos)
			{
				nCurrentPos = -1;
			}
			oCurrentPos = MAP::GetStartPosition();
		}
		
		KEY oKey;
		for (; nCurrentPos < nIndex; ++nCurrentPos)
		{
			MAP::GetNextAssoc(oCurrentPos, oKey, oValue);

			if (oCurrentPos == NULL)
				break;
		}
		
		return (oCurrentPos != NULL);
	}

	bool RemoveAtKey(ARG_KEY oKey)
	{
		VALUE oValue;
		
		MAP::Lookup(oKey, oValue);
		if (oValue == NULL)
		{
			return false;
		}

		delete oValue;
		MAP::RemoveKey(oKey);

		return true;
	}


	//	<summary>
	//		Set value at given key if key already exists then
	//		copy new value into old one, delete oNewValue and return false
	//		else the value is directly set, the nCurrentPos is reset to -1
	//		(because map positions have changed) and we return true;
	//	</summary>
	void SetAt(ARG_KEY oKey, ARG_VALUE oNewValue)
	{
		VALUE oOldValue = NULL;
		bool bFound = MAP::Lookup(oKey, oOldValue);
		
		if (bFound && oOldValue != NULL)
		{
			delete oOldValue;
		}

		MAP::SetAt(oKey, oNewValue);
		nCurrentPos = -1;
	}

// Methods
// ----------------



// Overrides
// ----------------



// Members
// ----------------
private:
	int nCurrentPos = -1;
	POSITION oCurrentPos = NULL;
};