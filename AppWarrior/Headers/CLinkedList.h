/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"

class CLink
{
	friend class CLinkedList;
	
	public:
		CLink() : mNext(0) {}
		
		CLink *GetNext() const		{	return mNext;	}
	
	protected:
		CLink *mNext;
};

class CLinkedList
{
	public:
		CLinkedList() : mHead(0) {}
	
		bool IsEmpty() const		{	return mHead == nil;	}
		bool IsNotEmpty() const		{	return mHead != nil;	}

		void AddFirst(CLink *ioLink);
		void AddLast(CLink *ioLink);
		
		CLink *RemoveFirst();
		CLink *RemoveLast();
		
		CLink *GetFirst() const		{	return mHead;	}
		CLink *GetLast() const;
		
		bool IsInList(const CLink *inLink) const;
		bool RemoveLink(CLink *ioLink);
		
		CLink *GetIndexedLink(Uint32 inIndex) const;
		Uint32 GetCount() const;
		
		CLink *StealList()			{	CLink *tmp = mHead; mHead = nil; return tmp;	}
		void SetHead(CLink *inHead)	{	mHead = inHead;	}
		
	protected:
		CLink *mHead;
};


