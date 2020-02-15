#pragma once

#define CPTRARRAY_CLEAR_MEMORY(name)         \
    for (int i = 0; i < name.GetSize(); ++i) \
        delete name.GetAt(i);                \
    name.RemoveAll();

/////////////////////////////////////////////////////////////////////////////
// CPtrArrayManager

//  <summary>
//  Class meant to delete each element in
//  CTypedPtrArray on scope exit
//  </sumamry>
template<class TYPE>
class CPtrArrayManager : public CTypedPtrArray<CPtrArray, TYPE>
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
    ~CPtrArrayManager()
    {
        ClearArray();
    }


// Methods
// ----------------
public:
    void ClearArray()
    {
        CPTRARRAY_CLEAR_MEMORY((*this))
    }


// Overrides
// ----------------


// Members
// ----------------


};