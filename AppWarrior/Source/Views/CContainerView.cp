/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory *//****************************************************************IntSetEnable:Ideally, when the super view is disabled, the sub views getdisabled too.  When the super view is next enabled, the subviews get enabled only if they were enabled when disabled.Also ideally, if the programmer enables a view that is disabledbecause the super view is, then the view does not get enabled.However, when the super view is next enabled, the view doesget enabled.One possible way to implement this is to store a mEnableOverrideflag with every view.  If is mIsEnabled is true, the view is notreally enabled unless mEnableOverride is too.  Thus, theimplementation of IsEnabled() would change to:	return mIsEnabled && mEnableOverride;It might be useful to do the same thing with visibility - mVisibleOverride.  An override for active status is NOT necessaryas this is not something that needs to be changed by the programmer.It is not necessary for focus either, as only one view is focussedat a time, so CContainerView can store the old state itself.****************************************************************//*CContainerView is a view that draws other views as its content (known ascontained views).  Contained views are drawn according to their boundingrectangle, which is interpreted in coordinates local to the CContainerView.For example, a contained view at 0,0 is located at the top left corner of theCContainerView.*/#include "CContainerView.h"// begin structures that must be the same across platforms#pragma options align=packedtypedef struct {	Uint32 selectedView, defaultView, cancelView;	Uint32 rsvd[4];	Uint16 isTabKeyNav			: 1;} SContainerView;// end structures that must be the same across platforms#pragma options align=resettypedef struct {	Int16 keyCode;	CView *view;} SViewKeyDownEntry;typedef struct {	Int16 button;	CView *view;} SViewMouseDownEntry;/* -------------------------------------------------------------------------- */CContainerView::CContainerView(CViewHandler *inHandler, const SRect& inBounds)	: CView(inHandler, inBounds){	mMouseCaptureView = mFocusView = mDefaultView = mCancelView = mMouseWithinView = mDragWithinView = nil;	mKeyData = mMouseData = nil;	mKeyDataCount = mMouseDataCount = 0;	mIsTabKeyNav = true;	mIsFocusViaTab = false;	mCanFocus = true;		// container view can select by default}CContainerView::~CContainerView(){	// dispose key and mouse data	UMemory::Dispose(mKeyData);	UMemory::Dispose(mMouseData);}/* -------------------------------------------------------------------------- */#pragma mark -/* * Use GetNextTabView() when the tab key has been pressed, and it will return * which view the selection should be switched to (or nil if none). */CView *CContainerView::GetNextTabView() const{	Uint32 n, i;	CView *v;		n = mFocusView ? mViewList.GetItemIndex(mFocusView) : 1;	i = 0;		// cycle through next views until we find a suitable one	while (mViewList.GetNext(v, n))	{		if (v->CanFocus() && v->IsEnabled() && v->IsActive() && v->IsVisible())			return v;	}	// keep looking from the start	while (mViewList.GetNext(v, i))	{		if (i >= n) break;				// don't check the same views again		if (v->CanFocus() && v->IsEnabled() && v->IsActive() && v->IsVisible())			return v;	}		// no next tab view	return nil;	}/* * Use GetPrevTabView when the tab key has been pressed with the shift key, and it * will return which view the selection should be switched to (or nil if none). */CView *CContainerView::GetPrevTabView() const{	Uint32 n, i;	CView *v;		n = mFocusView ? mViewList.GetItemIndex(mFocusView) : 1;	i = 0;	// cycle through previous views until we find a suitable one	while (mViewList.GetPrev(v, n))	{		if (v->CanFocus() && v->IsEnabled() && v->IsActive() && v->IsVisible())			return v;	}		// keep looking from the end	while (mViewList.GetPrev(v, i))	{		if (i <= n) break;				// don't check the same views again		if (v->CanFocus() && v->IsEnabled() && v->IsActive() && v->IsVisible())			return v;	}		// no prev tab view	return nil;}/* -------------------------------------------------------------------------- */#pragma mark -/* * Given a point local coordinates (0,0 specifies the top left of the * CContainerView), PointToView() returns which contained view the point  * is in (or nil if none). */CView *CContainerView::PointToView(const SPoint& inLocalPt) const{	//CView *v;	//Uint32 i = 0;	//		// check each view	//while (mViewList.GetPrev(v, i))						// front to back	//{	//	if (v->IsVisible())								// ignore invisible views	//	{	//		if (v->IsPointWithin(inLocalPt))	//			return v;	//	}	//}	//	//return nil;		Uint32 n;	CView **vp;	CView *v;	n = mViewList.GetItemCount();	if (n)	{		vp = mViewList.GetArrayPtr();				vp += n;		n++;				while (--n)		{			v = *--vp;						if (v->IsVisible() && v->IsPointWithin(inLocalPt))				return v;		}	}		return nil;}void CContainerView::Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth){	CView *v;	SRect localUpdateRect, r, sect;		// bring update rect into local coords	localUpdateRect = inUpdateRect;	localUpdateRect.top -= mBounds.top;	localUpdateRect.left -= mBounds.left;	localUpdateRect.bottom -= mBounds.top;	localUpdateRect.right -= mBounds.left;		// move existing clip so that it remains in the same place after the origin change	UGraphics::MoveClip(inImage, -mBounds.left, -mBounds.top);		// save origin and shift so that contained views draw according to our top left	StOriginSaver originSave(inImage);	UGraphics::AddOrigin(inImage, mBounds.left, mBounds.top);			// draw each view	Uint32 n = mViewList.GetItemCount();	if (n)	{		CView **vp = mViewList.GetArrayPtr();				StRegion savedClip;		inImage->GetClip(savedClip);		#if USE_PRE_INCREMENT		vp--;#endif		while (n--)		{#if USE_PRE_INCREMENT			v = *++vp;#else			v = *vp++;#endif						if (v->IsVisible())			{				v->GetBounds(r);							if (r.GetIntersection(localUpdateRect, sect))				{					inImage->IntersectClip(savedClip, r);										try					{						v->Draw(inImage, sect, inDepth);					}				#if DEBUG					catch(SError& err)					{						Uint8 errStr[256];						errStr[0] = UError::GetDetailMessage(err, errStr+1, sizeof(errStr)-1);						DebugBreak("Error drawing view (%#s)", errStr);						// don't throw					}				#else					catch(...)					{						// ignore to avoid possible never-ending errors when the view tries to draw again					}				#endif				}			}		}	}}void CContainerView::MouseDown(const SMouseMsgData& inInfo){	CView::MouseDown(inInfo);		SMouseMsgData info = inInfo;	SPoint localMouseLoc;	CView *foundView, *v;		if (!mIsEnabled || !IsActive())		return;		// bring mouse location into local coords	localMouseLoc = inInfo.loc;	localMouseLoc.v -= mBounds.top;	localMouseLoc.h -= mBounds.left;	info.loc = localMouseLoc;		if (mMouseCaptureView)		foundView = mMouseCaptureView;	else	// find which contained view the mouse is in		foundView = PointToView(localMouseLoc);		// if view mouse is in has changed, update mouse within view	if (foundView != mMouseWithinView)	{		// leave current		v = mMouseWithinView;		if (v)		{			mMouseWithinView = nil;			v->MouseLeave(info);		}			// enter new		if (foundView)		{			foundView->MouseEnter(info);			mMouseWithinView = foundView;		}	}		// act upon the view we found	if (foundView && foundView->IsEnabled())	{		// if applicable, change the focus to the view that was clicked in		if (mState == viewState_Focus && foundView->CanFocus() && mFocusView != foundView)		{			if (mFocusView)			{				mFocusView->ChangeState(viewState_Active);				mFocusView = nil;			}						foundView->ChangeState(viewState_Focus);			mFocusView = foundView;		}				// resize mouse data to accomodate new data		if (mMouseData == nil)		{			mMouseData = UMemory::NewHandle(sizeof(SViewMouseDownEntry));			mMouseDataCount = 0;		}		else if ((mMouseDataCount+1) * sizeof(SViewMouseDownEntry) > UMemory::GetSize(mMouseData))		{			UMemory::SetSize(mMouseData, (mMouseDataCount+1) * sizeof(SViewMouseDownEntry));		}				// add entry to mouse data		{			void *p;			StHandleLocker(mMouseData, p);			SViewMouseDownEntry& entry = ((SViewMouseDownEntry *)p)[mMouseDataCount];			entry.button = info.button;			entry.view = foundView;			mMouseDataCount++;		}				// let the view handle the mouse down		try		{			foundView->MouseDown(info);		}		catch(...)		{			mMouseDataCount--;			throw;		}	}}void CContainerView::MouseUp(const SMouseMsgData& inInfo){	CView::MouseUp(inInfo);		if (mMouseData == nil) return;		SViewMouseDownEntry *mouseData;	StHandleLocker locker(mMouseData, (void*&)mouseData);	CView *v = nil;	Int16 button = inInfo.button;	SMouseMsgData info = inInfo;	info.loc.v -= mBounds.top;	info.loc.h -= mBounds.left;	// determine which view we gave the corresponding mouse down to	for (Uint32 i=0; i<mMouseDataCount; i++)	{		if (mouseData[i].button == button)		{			// delete entry from mouse data			v = mouseData[i].view;			UMemory::Copy(mouseData + i, mouseData + i + 1, (mMouseDataCount - i - 1) * sizeof(SViewMouseDownEntry));			mMouseDataCount--;			i--;						v->MouseUp(info);			// if the mouse appears to be down in more than 1 view, release it for all...			//break;		}	}#if 0	// tell view that the mouse is up	if (v)	{		v->MouseUp(info);	}#endif}void CContainerView::MouseEnter(const SMouseMsgData& inInfo){	// in this case, mouse enter is the same as move	CView::MouseEnter(inInfo);	CContainerView::MouseMove(inInfo);		// don't use the virtual dispatch}void CContainerView::MouseMove(const SMouseMsgData& inInfo){	SPoint localMouseLoc;	SMouseMsgData info = inInfo;	CView *foundView, *v;	// bring mouse location into local coords	localMouseLoc = inInfo.loc;	localMouseLoc.v -= mBounds.top;	localMouseLoc.h -= mBounds.left;	info.loc = localMouseLoc;	if (mMouseCaptureView)		foundView = mMouseCaptureView;	else	// find which contained view the mouse is in		foundView = PointToView(localMouseLoc);		if (foundView == mMouseWithinView)	{		// move within current		if (foundView)			foundView->MouseMove(info);	}	else	{		// leave current		v = mMouseWithinView;		if (v)		{			mMouseWithinView = nil;			v->MouseLeave(info);		}			// enter new		if (foundView)		{			mMouseWithinView = foundView;			foundView->MouseEnter(info);		}	}}void CContainerView::MouseLeave(const SMouseMsgData& inInfo){	CView::MouseLeave(inInfo);		CView *v;		// if currently in a view	v = mMouseWithinView;	if (v)	{		// leave that view		mMouseWithinView = nil;				// bring mouse location into local coords		SMouseMsgData info = inInfo;		info.loc.v -= mBounds.top;		info.loc.h -= mBounds.left;				// tell view that the mouse is no longer in it		v->MouseLeave(info);	}}bool CContainerView::KeyDown(const SKeyMsgData& inInfo){	CView *targetView = nil;	bool isRepeat = (inInfo.mods & 0x8000) != 0;		if(!mIsEnabled || mState != viewState_Focus)		return false;		// try tabbed views before focus view	if(mIsTabKeyNav && inInfo.keyChar == '\t')	{				if(inInfo.mods & modKey_Shift)		{			if(!TabFocusPrev())				if(!TabFocusPrev())					return false;		}		else		{			if(!TabFocusNext())				if(!TabFocusNext())					return false;		}				return true;	}	// first try the focus view	if(mFocusView)	{		if(mFocusView->IsVisible() && mFocusView->IsEnabled())		{			if(isRepeat)			{				if(mFocusView->KeyRepeat(inInfo))					return true;			}			else if(mFocusView->KeyDown(inInfo))			{				targetView = mFocusView;				goto storeKeyData;			}		}	}		// focused view could not accept the KeyDown.	// check if EnterKey and defaultView	if(mDefaultView && (inInfo.keyCode == key_nEnter || inInfo.keyCode == key_Return))	{		// default view automagically gets the keydown if enter or return key pressed		if(mDefaultView->IsVisible() && mDefaultView->IsEnabled())		{			if(isRepeat)	// enterkey shouldn't repeat, but keep this here anyway			{				if(mDefaultView->KeyRepeat(inInfo))					return true;			}			else if(mDefaultView->KeyDown(inInfo))			{				targetView = mDefaultView;				goto storeKeyData;			}		}	}		if(mCancelView && (inInfo.keyCode == key_Escape || (inInfo.keyChar == '.' && (inInfo.mods & modKey_Command))))	{		// cancel view automagically gets the keydown if escape or command-period pressed		if (mCancelView->IsVisible() && mCancelView->IsEnabled())		{			if(isRepeat)	// escape key shouldn't repeat, but keep this here anyway			{				if(mDefaultView->KeyRepeat(inInfo))					return true;			}			else if(mCancelView->KeyDown(inInfo))			{				targetView = mCancelView;				goto storeKeyData;			}		}	}		// no view would accept the key down	return false;				storeKeyData:	// expand key data for new entry	if (mKeyData == nil)	{		mKeyData = UMemory::NewHandle(sizeof(SViewKeyDownEntry));		mKeyDataCount = 0;	}	else if ((mKeyDataCount+1) * sizeof(SViewKeyDownEntry) > UMemory::GetSize(mKeyData))	{		UMemory::SetSize(mKeyData, (mKeyDataCount+1) * sizeof(SViewKeyDownEntry));	}		// use key data to remember that we gave the keydown to this view	{		void *p;		StHandleLocker lock(mKeyData, p);		SViewKeyDownEntry& entry = ((SViewKeyDownEntry *)p)[mKeyDataCount];		entry.keyCode = inInfo.keyCode;		entry.view = targetView;		mKeyDataCount++;	}		return true;}void CContainerView::KeyUp(const SKeyMsgData& inInfo){	if (mKeyData == nil) return;		SViewKeyDownEntry *keyData;	StHandleLocker locker(mKeyData, (void*&)keyData);	CView *v = nil;	Int16 keyCode = inInfo.keyCode;	// determine which view we gave the corresponding key down to	for (Uint32 i=0; i<mKeyDataCount; i++)	{		if (keyData[i].keyCode == keyCode)		{			// delete entry from key data			v = keyData[i].view;			UMemory::Copy(keyData + i, keyData + i + 1, (mKeyDataCount - i - 1) * sizeof(SViewKeyDownEntry));			mKeyDataCount--;			break;		}	}	// tell view that the key is up	if (v) v->KeyUp(inInfo);}bool CContainerView::KeyRepeat(const SKeyMsgData& inInfo){	SKeyMsgData info = inInfo;	info.mods |= 0x8000;		// indicate to IntKeyDown() that this is a repeat, not down	return CContainerView::KeyDown(info);}/* -------------------------------------------------------------------------- */#pragma mark -#if 0bool CContainerView::CanAcceptDrop(TDrag /* inDrag */) const{	// because we can always not accept a drop, we will assume we can!!!	return true;}#endifvoid CContainerView::DragEnter(const SDragMsgData& inInfo){	// in this case, drag enter is the same as move	CView::DragEnter(inInfo);	CContainerView::DragMove(inInfo);			// don't use the virtual dispatch}void CContainerView::DragMove(const SDragMsgData& inInfo){	CView::DragMove(inInfo);		SPoint localMouseLoc;	SDragMsgData info = inInfo;	CView *foundView, *v;	// bring mouse location into local coords	localMouseLoc = inInfo.loc;	localMouseLoc.v -= mBounds.top;	localMouseLoc.h -= mBounds.left;	info.loc = localMouseLoc;	// find which view the mouse is in	foundView = PointToView(localMouseLoc);		// if view mouse is in has changed, update drag within view	if (foundView == mDragWithinView)	{		// move within current		if (mDragWithinView)// && mDragWithinViewCanAccept)			mDragWithinView->DragMove(info);	}	else	{		// leave current		v = mDragWithinView;		if (v)		{			mDragWithinView = nil;		//	if (mDragWithinViewCanAccept)				v->DragLeave(info);		}				// enter new		if (foundView)		{//			mDragWithinViewCanAccept = (foundView->CanAcceptDrop(info.drag) != 0);//			if (mDragWithinViewCanAccept)	// don't enter the view if it can't accept the drop				foundView->DragEnter(info);						mDragWithinView = foundView;		}	}}void CContainerView::DragLeave(const SDragMsgData& inInfo){	CView::DragLeave(inInfo);		CView *v;			// if currently in a view	v = mDragWithinView;	if (v)	{		// leave that view		mDragWithinView = nil;		//		if (mDragWithinViewCanAccept)		{			// bring mouse location into local coords			SDragMsgData info = inInfo;			info.loc.v -= mBounds.top;  			info.loc.h -= mBounds.left; 						// tell view that the mouse is no longer in it			v->DragLeave(info);		}	}}bool CContainerView::Drop(const SDragMsgData& inInfo){	if (mDragWithinView)// && mDragWithinViewCanAccept)		return mDragWithinView->Drop(inInfo);	return false;}/* -------------------------------------------------------------------------- */#pragma mark -void CContainerView::UpdateQuickTime(){	Uint32 i = 0;	CView *pView;			while (mViewList.GetNext(pView, i))	{		if (pView->IsVisible())								// ignore invisible views			pView->UpdateQuickTime();	}}void CContainerView::SendToQuickTime(const EventRecord& inInfo){	Uint32 i = 0;	CView *pView;			while (mViewList.GetNext(pView, i))	{		if (pView->IsVisible())								// ignore invisible views			pView->SendToQuickTime(inInfo);	}}/* -------------------------------------------------------------------------- */#pragma mark -// returns true if changedbool CContainerView::ChangeState(Uint16 inState){	if (mState != inState)	// if change	{		Uint32 i, n;		CView **vp;		CView *v;				if (inState == viewState_Focus)		// focus needs special treatment because only one of the contained views should be focused (the rest should be Active)		{			if (mState == viewState_Active)	// if going from Active to Focus			{				// only the focus-view should be set to Focus, the rest should be Active, and since we're going from Active to Focus, the rest are already Active				mState = viewState_Focus;				if (mFocusView) mFocusView->ChangeState(viewState_Focus);			}			else							// going from anything-but-Active to Focus			{				mState = viewState_Focus;				n = mViewList.GetItemCount();				vp = mViewList.GetArrayPtr();				if (mFocusView)				{					// set state for focus-view to Focus and set everything else to Active					v = mFocusView;					for (i=0; i!=n; i++)					{						if (vp[i] != v)							vp[i]->ChangeState(viewState_Active);					}										v->ChangeState(viewState_Focus);				}				else				{					// no focus view, just set everything to Active					for (i=0; i!=n; i++)						vp[i]->ChangeState(viewState_Active);				}			}		}		else if (mState == viewState_Focus && inState == viewState_Active)	// if going from Focus to Active		{			// only the focus-view was set to Focus (the rest are Active), so only that view has to be set to Active			mState = viewState_Active;			if (mFocusView) mFocusView->ChangeState(viewState_Active);		}		else		{			mState = inState;						n = mViewList.GetItemCount();			vp = mViewList.GetArrayPtr();						// just set everything to the new state			for (i=0; i!=n; i++)				vp[i]->ChangeState(inState);		}				// state successfully changed		return true;	}		// state didn't change	return false;}// returns true if change// does nothing and returns false if <inView> can't be focused (inView->CanFocus() returns false)// to remove the focus, use nil for <inView>bool CContainerView::SetFocusView(CView *inView){	if (mFocusView != inView)	// if change	{		// check for trying to set the focus to a view that doesn't belong to us#if DEBUG		if (inView && !mViewList.IsInList(inView))		{			DebugBreak("CContainerView::SetFocusView - specified view is not a subview");			Fail(errorType_Misc, error_Param);		}#endif				// check for trying to set the focus to a view that doesn't want to be focused		if (inView && !inView->CanFocus()) return false;				// ok set the focus		if (mState == viewState_Focus)	// if this container view is focused		{			if (mFocusView)			{				mFocusView->ChangeState(viewState_Active);				mFocusView = nil;		// essential for just in case the next ChangeState() fails			}						if (inView)			{				inView->ChangeState(viewState_Focus);				mFocusView = inView;			}		}		else		{			// we're not focused currently but we'll remember the view for when we are			mFocusView = inView;		}		// successfully changed		return true;	}		// no change	return false;}// returns true if something was focused, false if unfocusedbool CContainerView::TabFocusNext(){	if(!(mIsTabKeyNav && mCanFocus && mIsVisible && mIsEnabled))		return false;		Uint32 n;	CView *v;		if(mFocusView)	{		n = mViewList.GetItemIndex(mFocusView);		v = mFocusView;	}	else	{		v = mViewList.GetItem(1);		if(!v)			return false;				n = 0;	}		for(;;)	{		if(v->TabFocusNext())		{			if(v != mFocusView)			{				if(mFocusView)				{					try					{						mFocusView->ChangeState(viewState_Active);					}					catch(...)					{						mFocusView = nil;						mIsFocusViaTab = false;						throw;					}				}								mFocusView = v;			}									mIsFocusViaTab = true;						mState = viewState_Focus;			return true;		}				if(!mViewList.GetNext(v, n))			break;	}		if(mFocusView)	{		try		{			mFocusView->ChangeState(viewState_Active);			mFocusView = nil;		}		catch(...)		{			mFocusView = nil;			mIsFocusViaTab = false;			throw;		}	}		mState = viewState_Active;	return false;}// returns true if something was focused, false if unfocusedbool CContainerView::TabFocusPrev(){	if(!(mIsTabKeyNav && mCanFocus && mIsVisible && mIsEnabled))		return false;		Uint32 n;	CView *v;		if(mFocusView)	{		n = mViewList.GetItemIndex(mFocusView);		v = mFocusView;	}	else	{		v = mViewList.GetItem(mViewList.GetItemCount());		if(!v)			return false;				n = 0;	}		for(;;)	{		if(v->TabFocusPrev())		{			if(v != mFocusView)			{				if(mFocusView)				{					try					{						mFocusView->ChangeState(viewState_Active);					}					catch(...)					{						mFocusView = nil;						mIsFocusViaTab = false;						throw;					}				}								mFocusView = v;			}						mIsFocusViaTab = true;				mState = viewState_Focus;			return true;		}				if(!mViewList.GetPrev(v, n))			break;	}		if(mFocusView)	{		try		{			mFocusView->ChangeState(viewState_Active);			mFocusView = nil;		}		catch(...)		{			mFocusView = nil;			mIsFocusViaTab = false;			throw;		}	}		mState = viewState_Active;	return false;}bool CContainerView::SetBounds(const SRect& inBounds){	Int32 oldHeight = mBounds.GetHeight();	Int32 oldWidth = mBounds.GetWidth();	if (CView::SetBounds(inBounds))	{		SRect r;		CView *v;		Uint32 i = 0;		Uint16 sizing;				Int32 newHeight = inBounds.GetHeight();		Int32 newWidth = inBounds.GetWidth();						if (oldHeight != newHeight || oldWidth != newWidth)		{			while (mViewList.GetNext(v, i))			{				sizing = v->GetSizing();				if (sizing)				{					v->GetBounds(r);										if (sizing & sizing_LeftSticky)						r.left = newWidth - (oldWidth - r.left);										if (sizing & sizing_RightSticky)						r.right = newWidth - (oldWidth - r.right);										if (sizing & sizing_TopSticky)						r.top = newHeight - (oldHeight - r.top);											if (sizing & sizing_BottomSticky)						r.bottom = newHeight - (oldHeight - r.bottom);					v->SetBounds(r);				}								// if is a QuickTimeView we need to update the movie bounds				CQuickTimeView *pQuickTimeView = dynamic_cast<CQuickTimeView *>(v);				if (pQuickTimeView)					pQuickTimeView->UpdateQuickTimeBounds();			}		}			return true;	}		return false;}void CContainerView::MouseCaptureReleased(){	CView::MouseCaptureReleased();		if (mMouseCaptureView)	{		mMouseCaptureView->MouseCaptureReleased();		mMouseCaptureView = nil;	}}/* -------------------------------------------------------------------------- */#pragma mark -void CContainerView::HandleInstall(CView *inView){	ASSERT(inView);	inView->Hide();	inView->ChangeState(mState == viewState_Focus ? viewState_Active : mState);	CMultiViewContainer::HandleInstall(inView);}void CContainerView::HandleRemove(CView *inView){	// ***** should refresh rect if view was visible ?		if (!inView || mIsDestructing) return;	CMultiViewContainer::HandleRemove(inView);		Uint32 i;	// remove from data members	if (mFocusView == inView)			mFocusView = nil;	if (mDefaultView == inView)			mDefaultView = nil;	if (mCancelView == inView)			mCancelView = nil;	if (mMouseWithinView == inView)		mMouseWithinView = nil;	if (mDragWithinView == inView)		mDragWithinView = nil;	if (mMouseCaptureView == inView)	mMouseCaptureView = nil;				// remove from key data	if (mKeyDataCount && mKeyData)	{		SViewKeyDownEntry *keyData;		StHandleLocker locker(mKeyData, (void*&)keyData);				for (i=0; i<mKeyDataCount; i++)		{			if (keyData[i].view == inView)			{				UMemory::Copy(keyData + i, keyData + i + 1, (mKeyDataCount - i - 1) * sizeof(SViewKeyDownEntry));				mKeyDataCount--;				break;			}		}	}		// remove from mouse data	if (mMouseDataCount && mMouseData)	{		SViewMouseDownEntry *mouseData;		StHandleLocker locker(mMouseData, (void*&)mouseData);				for (i=0; i<mMouseDataCount; i++)		{			if (mouseData[i].view == inView)			{				UMemory::Copy(mouseData + i, mouseData + i + 1, (mMouseDataCount - i - 1) * sizeof(SViewMouseDownEntry));				mMouseDataCount--;				break;			}		}	}}void CContainerView::HandleRefresh(CView */* inView */, const SRect& inUpdateRect){	SRect r = inUpdateRect;	r.Move(mBounds.left, mBounds.top);	Refresh(r);}void CContainerView::HandleHit(CView *inView, const SHitMsgData& inInfo){	if (mHandler) mHandler->HandleHit(inView, inInfo);}void CContainerView::HandleGetScreenDelta(CView */* inView */, Int32& outHoriz, Int32& outVert){	GetScreenDelta(outHoriz, outVert);		outHoriz += mBounds.left;	outVert += mBounds.top;}void CContainerView::HandleGetVisibleRect(CView */* inView */, SRect& outRect){	GetVisibleRect(outRect);		outRect.Move(-mBounds.left, -mBounds.top);}void CContainerView::HandleCaptureMouse(CView *inView){	mMouseCaptureView = inView;	if (mHandler) mHandler->HandleCaptureMouse(this);}void CContainerView::HandleReleaseMouse(CView *inView){	if (mMouseCaptureView == inView)	{	if (mHandler) mHandler->HandleReleaseMouse(this);	}#if DEBUG && DEBUGGING_MOUSECAPTURE	else		DebugBreak("CContainerView HandleReleaseMouse: inView != mMouseCaptureview...mMouseCaptureView = %lu, inView = %lu", (Uint32)mMouseCaptureView, (Uint32)inView);#endif}void CContainerView::HandleGetOrigin(SPoint& outOrigin){	if (mHandler) mHandler->HandleGetOrigin(outOrigin);	outOrigin.x += mBounds.left;	outOrigin.y += mBounds.top;}CView *CContainerView::HandleGetCaptureMouseView(){	return mMouseCaptureView;}CWindow *CContainerView::HandleGetParentWindow(){	return GetParentWindow();}