/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */


class CMyDLItem;

class CMyDLFldr
{
	public:
		CMyDLFldr(TFSRefObj* inRef, CMyDLFldr *inParent, bool inDBAccess);
		~CMyDLFldr();
		
		Uint32 GetTotalItems();
		Uint32 GetTotalSize();
		
		bool HasItem(TFSRefObj* inRef);
		TFSRefObj* GetFolder()				{	return mRef;		}
		CMyDLFldr *GetParent()			{	return mParent;		}

		TFSRefObj* GetNextItem(Uint8*& ioPathStart, Int16& ioMaxPathSize, Uint16& ioCount, bool *outIsFolder);
		
	protected:
		TFSRefObj* mRef;
		CMyDLFldr *mParent;
		
		Uint32 mCurrentItem;
		CPtrList<CMyDLItem> mItems;
};

class CMyDLItem
{
	public:
		CMyDLItem(SFSListItem *inItem, CMyDLFldr *inParent, bool inDBAccess);
		~CMyDLItem();
	
		Uint32 GetTotalItems()								{	return mFldr ? mFldr->GetTotalItems() : mFile ? 1 : 0;					}
		Uint32 GetTotalSize()								{	return mFldr ? mFldr->GetTotalSize() : mFile ? mFile->GetSize() : 0;	}
		
		TFSRefObj* GetNextItem(Uint8*& ioPathStart, Int16& ioMaxPathSize, Uint16& ioCount, bool *outIsFolder);
		bool HasItem(TFSRefObj* inRef);
		
	protected:
		TFSRefObj* mFile;

		union {
			CMyDLFldr *mFldr;
			bool mIsDone;
		};
};

