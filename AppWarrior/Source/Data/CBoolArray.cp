/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
/*
CBoolArray maintains an ordered list of boolean values (true or false).  Each
item is stored as a single bit, making CBoolArray very efficient memory wise.
For example, a CBoolArray with 1024 items uses only 128 bytes of memory.
*/

#include "CBoolArray.h"
#include "UBitString.h"

/* -------------------------------------------------------------------------- */

CBoolArray::CBoolArray()
{
	mData = nil;
	mCount = 0;
	mDataSize = 0;
}

CBoolArray::~CBoolArray()
{
	UMemory::Dispose(mData);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CBoolArray::ExpandData(Uint32 inExtraItems)
{
	Uint32 s = ((mCount + inExtraItems) / 8) + 1;
	
	if (mData == nil)
	{
		mCount = 0;
		mData = UMemory::NewHandleClear(s);
	}
	else if (s > mDataSize)
	{
		UMemory::SetSize(mData, s);
	}
	
	mCount += inExtraItems;
	mDataSize = s;
}

void CBoolArray::ShrinkData(Uint32 inLessItems)
{
	// wipe everything if there's not going to be be any items left
	if (inLessItems >= mCount)
	{
		Clear();
		return;
	}

	// shrink data
	if (mData == nil)
	{
		mCount = nil;
		mDataSize = 0;
	}
	else
	{
		Uint32 s = ((mCount - inLessItems) / 8) + 1;
		if (s != mDataSize)
		{
			UMemory::SetSize(mData, s);
			mDataSize = s;
		}
		mCount -= inLessItems;
	}
}

void CBoolArray::ShiftItems(Uint32 inStartPos, Uint32 inEndPos, Uint32 inDestPos)
{
	void *p;
	StHandleLocker locker(mData, p);
	UBitString::Copy(p, inDestPos-1, inStartPos-1, inEndPos - (inStartPos-1));
}

/* -------------------------------------------------------------------------- */
#pragma mark -

Uint32 CBoolArray::InsertItems(Uint32 inIndex, Uint32 inCount, bool inData)
{
	Uint32 saveCount = mCount;
	
	// allocate storage for the new items
	ExpandData(inCount);
	
	// adjust index and move existing items as necessary
	if (inIndex > saveCount)		// if inserting at end of array
		inIndex = saveCount + 1;
	else							// inserting at start or middle of array
	{
		if (inIndex < 1)			// if inserting at start
			inIndex = 1;
		
		// move existing items to make to make room for the new one
		if (saveCount > 0)
			ShiftItems(inIndex, saveCount, inIndex + inCount);
	}
	
	// inserted items are all set to the same value
	void *p;
	StHandleLocker locker(mData, p);
	Uint32 i = inIndex;
	do {
		UBitString::Set(p, i-1, inData);
	} while (++i < inIndex + inCount);
	
	return inIndex;
}

Uint32 CBoolArray::InsertItem(Uint32 inIndex, bool inData)
{
	return InsertItems(inIndex, 1, inData);
}

Uint32 CBoolArray::AddItem(bool inData)
{
	return InsertItems(max_Uint32, 1, inData);
}

void CBoolArray::RemoveItems(Uint32 inIndex, Uint32 inCount)
{
	if (inIndex < 1) inIndex = 0;

	if (inIndex + inCount <= mCount)		// if removing items from the middle
	{
		// shift down items that are beyond the ones being removed
		ShiftItems(inIndex + inCount, mCount, inIndex);
	}
	else									// removing items at the end
	{
		// remove from index to the end of the array
		inCount = mCount - inIndex + 1;
	}
	
	// shrink storage
	ShrinkData(inCount);
}

void CBoolArray::RemoveItem(Uint32 inIndex)
{
	RemoveItems(inIndex, 1);
}

void CBoolArray::Clear()
{
	UMemory::Dispose(mData);

	mData = nil;
	mCount = 0;
	mDataSize = 0;
}

Uint32 CBoolArray::GetItemCount() const
{
	return mCount;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

bool CBoolArray::GetItem(Uint32 i) const
{
	if (i < 1 || i > mCount) return false;
	void *p;
	StHandleLocker locker(mData, p);
	return UBitString::Get(p, i-1);
}

void CBoolArray::SetItems(Uint32 inIndex, Uint32 inCount, bool inData)
{
	if (inIndex > 0 && inIndex <= mCount)
	{
		if ((inIndex-1) + inCount > mCount)
			inCount = mCount - (inIndex-1);
	
		void *p;
		StHandleLocker locker(mData, p);
		UBitString::Set(p, inIndex-1, inCount, inData);
	}
}

void CBoolArray::SetItem(Uint32 i, bool b)
{
	if (i > 0 && i <= mCount)
	{
		void *p;
		StHandleLocker locker(mData, p);
		UBitString::Set(p, i-1, b);
	}
}

Uint32 CBoolArray::GetFirstSet() const
{
	if (mData)
	{
		Uint32 i;
		void *p;
		StHandleLocker locker(mData, p);
		if (UBitString::GetFirstSet(p, mCount, i))
			return i+1;
	}
	return 0;
}

Uint32 CBoolArray::GetLastSet() const
{
	if (mData)
	{
		Uint32 i;
		void *p;
		StHandleLocker locker(mData, p);
		if (UBitString::GetLastSet(p, mCount, i))
			return i+1;
	}
	return 0;
}

void CBoolArray::SetDataHandle(THdl inHdl, Uint32 inItemCount)
{
	Require(inHdl);
	
	UMemory::Dispose(mData);
	mData = nil;
	mDataSize = 0;
	mCount = 0;
	
	mData = inHdl;
	mDataSize = UMemory::GetSize(inHdl);
	mCount = inItemCount;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CBoolArray::MoveItems(Uint32 inToIndex, Uint32 inFromIndex, Uint32 inCount)	
{
	// checks
	if (inCount < 1) return;
	if (inFromIndex < 1 || inFromIndex > mCount) return;
	if (inToIndex < 1 || inToIndex > mCount) return;
	if (inFromIndex == inToIndex) return;
	
	if (inCount != 1)
	{
		DebugBreak("CBoolArray - MoveItems() can not yet move multiple items");
		Fail(errorType_Misc, error_Unimplemented);
	}

	void *p;
	StHandleLocker locker(mData, p);

	// store copy of item to move
	bool buf = UBitString::Get(p, inFromIndex-1);
	
	// shift items in between
	if (inFromIndex < inToIndex)								// if moving item down
		UBitString::Copy(p, inFromIndex-1, inFromIndex, inToIndex - inFromIndex);
	else														// moving item up
		UBitString::Copy(p, inToIndex, inToIndex-1, inFromIndex - inToIndex);

	// store the item at new index
	UBitString::Set(p, inToIndex-1, buf);
}

void CBoolArray::MoveItem(Uint32 inToIndex, Uint32 inFromIndex)
{
	MoveItems(inToIndex, inFromIndex, 1);
}

void CBoolArray::SwapItems(Uint32 inIndexA, Uint32 inIndexB, Uint32 inCount)
{
	if (inCount)
	{
		if (inCount != 1)
		{
			DebugBreak("CBoolArray - SwapItems() can not yet swap multiple items");
			return;
		}
		
		bool t = GetItem(inIndexA);				// t = a
		SetItem(inIndexA, GetItem(inIndexB));	// a = b
		SetItem(inIndexB, t);					// b = t
	}
}






