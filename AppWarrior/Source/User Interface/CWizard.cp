/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CWizard.h"


/* -------------------------------------------------------------------------- */

CWizard::CWizard()
	: CWindow(SRect(0,0,300,30), windowLayer_Modal)
{
	mHeadPane = mCurrentPane = nil;
	
	CContainerView *vc;
	CButtonView *btn;
	SRect r(0, 0, 300, 30);
	
	vc = new CContainerView(this, r);
	vc->Show();
	mContainerView = vc;
	
	// cancel button
	r.Set(10, 3, 90, 25);
	
	btn = new CButtonView(vc, r, 0, "\pCancel");
	btn->SetSizing(sizing_VertSticky);
	btn->SetID(1);
	vc->SetCancelView(btn);
	btn->Show();
	mCancelBtn = btn;
	
	// back button
	r.right = 190;
	r.left = 110;
	
	btn = new CButtonView(vc, r, 0, "\pBack");
	btn->SetSizing(sizing_VertSticky | sizing_HorizSticky);
	btn->SetID(2);
	btn->Show();
	mBackBtn = btn;

	// next button
	r.Set(210, 0, 290, 28);
	
	btn = new CButtonView(vc, r, 0, "\pNext");
	btn->SetSizing(sizing_VertSticky | sizing_HorizSticky);
	btn->SetID(3);
	btn->SetDefault(true);
	vc->SetDefaultView(btn);
	btn->Show();
	mNextBtn = btn;
	
	// finish button
	btn = new CButtonView(vc, r, 0, "\pFinish");
	btn->SetSizing(sizing_VertSticky | sizing_HorizSticky);
	btn->SetID(4);
	btn->SetDefault(true);
	btn->Show();
	mFinishBtn = btn;
}

CWizard::~CWizard()
{
	if(!mHeadPane)
		return;
	
	Finish();
		
	SMyViewLink *pane = mHeadPane;
	SMyViewLink *next;
	
	for(;;)
	{
		next = pane->next;
		delete pane->view;
		delete pane;
	
		if(next == mHeadPane)
			break;
			
		pane = next;
	}
}

/* -------------------------------------------------------------------------- */
#pragma mark -

bool CWizard::Process()
{
	Reset();
	Show();
	
	for(;;)
	{
		ProcessModal();
		
		switch(GetHitID())
		{
			case 1:
				Finish();
				return false;
				break;
			case 2:
				GoBack();
				break;
			case 3:
				GoNext();
				break;
			case 4:
				Finish();
				return true;
				break;
		}
	}
}

void CWizard::AddPane(CView *inPaneView)
{
	Require(inPaneView);

	SMyViewLink *pane = new SMyViewLink;
	pane->view = inPaneView;
	
	if(mHeadPane)
	{
		pane->next = mHeadPane;
		pane->prev = mHeadPane->prev;
		mHeadPane->prev = pane;
		pane->prev->next = pane;
	}
	else
	{
		mHeadPane = pane;
		pane->next = pane;
		pane->prev = pane;
	}
	
	SRect vr, bounds;
	
	inPaneView->GetBounds(vr);
	GetBounds(bounds);
	
	Int32 w = vr.right;
	Int32 h = vr.bottom;
	bool changed = false;
	
	h += 30;	// we want at least 30 pixels at the bottom;
	if(w < 250)
	{
		w = 250;
	}
		
	if(w > bounds.GetWidth())
	{
		bounds.right = bounds.left + w;
		changed = true;
	}
	
	if(h > bounds.GetHeight())
	{
		bounds.bottom = bounds.top + h;
		changed = true;
	}
	
	if(changed)
		SetBounds(bounds);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CWizard::GoNext()
{
	if(!(mCurrentPane && mCurrentPane->next))
		return;
	
	mContainerView->RemoveView(mCurrentPane->view);

	mContainerView->InstallView(mCurrentPane->next->view);
	mContainerView->SetFocusView(mCurrentPane->next->view);
	mCurrentPane->next->view->Show();
	
	if(mCurrentPane == mHeadPane)
	{
		// if we were on the first pane and have now moved forward, enable the back btn
		mBackBtn->SetEnable(true);
	}
	
	mCurrentPane = mCurrentPane->next;
	
	if(mCurrentPane->next == mHeadPane)
	{
		// if we are on the last pane, show Finish
		mNextBtn->Hide();
		mFinishBtn->Show();
		mContainerView->SetDefaultView(mFinishBtn);
	}	
}

void CWizard::GoBack()
{
	if(!(mCurrentPane && mCurrentPane->prev))
		return;
	
	mContainerView->RemoveView(mCurrentPane->view);
		
	mContainerView->InstallView(mCurrentPane->prev->view);
	mContainerView->SetFocusView(mCurrentPane->prev->view);
	mCurrentPane->prev->view->Show();
	
	if(mCurrentPane->next == mHeadPane)
	{
		// if we were on the last pane and moved back, hide Finish
		mFinishBtn->Hide();
		mNextBtn->Show();
		mContainerView->SetDefaultView(mNextBtn);
	}
	
	mCurrentPane = mCurrentPane->prev;
	
	if(mCurrentPane == mHeadPane)
	{
		// if we are now at the first pane, disable Back
		mBackBtn->SetEnable(false);
	}
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CWizard::Finish()
{
	Hide();
	
	// remove the views since otherwise, destructing the window will dispose of it
	// and CWizard already destructs these
	if(mCurrentPane && mCurrentPane->prev)
		mContainerView->RemoveView(mCurrentPane->view);
}

void CWizard::Reset()
{
	if(!mHeadPane)
		return;
		
	SetAutoBounds(windowPos_Center, windowPosOn_MainScreen);

	if(mCurrentPane)
		mContainerView->RemoveView(mCurrentPane->view);

	mCurrentPane = mHeadPane;

	mContainerView->InstallView(mCurrentPane->view);
	mContainerView->SetFocusView(mCurrentPane->view);
	mCurrentPane->view->Show();
		
	mBackBtn->SetEnable(false);
		
	if(mCurrentPane->next == mHeadPane)
	{
		mNextBtn->Hide();
		mFinishBtn->Show();
		mContainerView->SetDefaultView(mFinishBtn);
	}
	else
	{
		mFinishBtn->Hide();
		mNextBtn->Show();
		mContainerView->SetDefaultView(mNextBtn);
	}
}

