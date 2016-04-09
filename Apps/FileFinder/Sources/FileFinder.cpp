/*
Copyright (C) 2003 Hotsprings Inc.
For conditions of distribution and use, see copyright notice

Location: 
	www.HotspringsInc.com 

History:
	2003Sep03-GiuseppeG: code write
	
Requirements:
	1) 	Must show current folder
	2) 	Must be able to jump to any of the parent folders + up button
	3) 	Must have most recent used folders + back,forward buttons
	4)	Must have favorite folders + add to favorites
	5)	Must show content of current folder + view as selection 
	6)	Must be able to type the filename
	7)	Must be able to type the full pathname
	8)  Must be able to type the search pattern 
	9)  Must show the deep search result with full path
    10) Must be able to execute (ask the system to open) file

Description:
    class FileDialogStorage is the persistent storage for the data dialog.
    To instantiate a FileFindDialog you need an instance of this class 
    as a parameter. The dialog will use this storage space for its public 
    member data. When a dialog closes the instance of this class can be held 
    alive if you hold another reference to it. Next time you open the dialog 
    you can use this same (Saved) instance of the storage and therefore make 
    the dialog resume from the place it last closed. This way you can easily 
    keep track of all the visited folders (history), the typed in filters, 
    search patterns and so on. Additionally you can initialize this object 
    before the first use of the dialog to make it act as if it already knows 
    some "favorite places". 
    
	class FileFindDialog is the main functional object in this file. It is
	derived of a dialog window. It uses the FileDialogStorage described above 
	to store it's persistent data. Also it uses 2 lists (the back and forward 
	history) which we decided not to make persistent. 
	This class is a good example for how to use a couple of controls together 
	to implement a more complex state machine with various components having 
	direct user control.
	The "DoLayout" method overrides the virtual in dialog window and allows the 
	dialog to reposition/resize it's component views when resized.
	The "PopulateDialog" method is used to create and initialize the views 
	associated with this dialog.
	The "PopulateFields" adds to each view the appropriate data to render.
	The "RepopulateContent" has the same purpose as "PopulateFields" but only 
	deals with those views that may change their content during the operation 
	of the dialog. For example the "folderContent" list is repopulated every 
	time the user enters a new folder.
	"DialogEventBack" is called when the user pushes the "back" button. If the 
	back history list is not empty the function goes to the previous folder in 
	the history list.
	"DialogEventForward" called when the user pushed the "forward button", otherwise 
	identical to the "back" function.
	DialogEventUpFolder is called when the user wants to go to the parent directory.
	If there is such a parent folder the function will go there and remember the current 
	folder in the back history list.
	DialogEventSelection is a callback function called every time the user clicks on one 
	of the files/folders listed in the content. If it is a folder we use that event to 
	dive into it. If it is a file we simply launch that file using the OS.
	DialogEventFolder is a callback function called every time the user selects a new 
	folder from the folders combo box. 
	DialogEventFilter is called every time the user makes a new choice for the search 
	pattern. As a result the current folder and all its children are searched for that 
	specific pattern and the matching filenames are loaded into the content browser.

	class FilePickButtonListener is a helper object used to listen to the 
	buttons we added to the dialog. The Apply/Ok/Cancel buttons are handled 
	separately by the dialog window base class, we don't need to do anything 
	on those. However for the buttons that we did add, inside the listener we 
	do redirect the notifications to member functions inside the dialog.
	
	class FilePickSelectionListener is a helper object used to listen to the 
	selection of an entry in the list of files. It has proxy semantics, all 
	it does is to redirect the notifications to member functions inside the dialog.
	
	class MySimpleApp  is similar to "DemoApplication" in "DemoHello.cpp", 
	see the description in that file.
*/

#include "XSP_Core.h"
#include "XSP_File.h"
#include "XSP_GUI.h"
#include "XSP_Codecs.h"
using namespace XSP;
#include "CreateSkin.h"

#if 0
#pragma mark class FileDialogStorage
#endif

class FileDialogStorage : public ref_obj
{
public:
	FileName selectedFile; // selected on open, stored here on close
	FileName parentFolder; // the folder where the selected file is
	std::list<FileName> favoriteFolders; // init to home, app, desktop, ...
	std::list<String>   filters; // each string can contain an extension or *
public:
	FileDialogStorage();
protected:
	virtual ~FileDialogStorage();
}; // class FileDialogStorage

#if 0
#pragma mark class FileFindDialog
#endif

class FileFindDialog : public DialogWindow
{
public:
	refc<FileDialogStorage> dlgStorage;
	std::list<FileName> 	backHistory;
	std::list<FileName> 	forwHistory;

	refc<LabelView>			labelFolder;
	refc<ComboListView>		curFolder;	  // with favorites
	refc<LabelView>			labelFilter;
	refc<ComboListView>		curFilter;	  // with history of filters
    refc<ScrollPage>		scrollPage;	  // scroll page for the content
	refc<ListView>			folderContent;// the content of current folder
	refc<ButtonView>		backButton;
	refc<ButtonView>		forwButton;
	refc<ButtonView>		upButton;
public:
	FileFindDialog();
	void SetDlgStorage(	const refc<FileDialogStorage>& dlgStorage ); 
	void PopulateDialog(); // store the views into the dialog
	void PopulateFields(); // stores data into the views
	void RepopulateContent();
protected:
	virtual ~FileFindDialog();

	virtual void DialogEventOk();

	virtual void DoLayout();
public:
	virtual void DialogEventBack();
	virtual void DialogEventForward();
	virtual void DialogEventUpFolder();
	virtual void DialogEventSelection(const String& text, bool on);
	virtual void DialogEventFolder(const String& text);
	virtual void DialogEventFilter(const String& text);

}; // class FileFindDialog


#if 0
#pragma mark class FilePickButtonListener
#endif

class FilePickButtonListener : public ButtonView::ActionListener
{
	FileFindDialog* dw;
public:
	FilePickButtonListener(FileFindDialog* dwnd) : dw(dwnd)	{ }
	~FilePickButtonListener() { CancelListener(); }
	void CancelListener()   { dw=0; super::CancelListener(); }
protected:
	virtual void ActionPerform(ButtonView* button)
	{
		if (dw == 0)
			return;
		if (dw->backButton == button)
			dw->DialogEventBack();
		else if (dw->forwButton == button)
			dw->DialogEventForward();
		else if (dw->upButton == button)
			dw->DialogEventUpFolder();
	}
}; // class DialogButtonListener

#if 0
#pragma mark class FilePickSelectionListener
#endif

class FilePickSelectionListener : public ListView::SelectionListener
{
	FileFindDialog* dw;
public:
	FilePickSelectionListener(FileFindDialog* v): dw(v) {}
	~FilePickSelectionListener()   	   	{ CancelListener(); }
	void CancelListener() {	dw = 0;	super::CancelListener(); }
protected:
	virtual void SelectionChanged(const String& text, bool on, ListView* lbox)
	{
		if (dw == 0)
			return;
		if (dw->folderContent == lbox)
			dw->DialogEventSelection(text, on);
		else if (dw->curFolder->GetDropDown() == lbox)
			dw->DialogEventFolder(text);
		else if (dw->curFilter->GetDropDown() == lbox)
			dw->DialogEventFilter(text);
	}
}; // ComboSelectionListener

#if 0
#pragma mark -
#endif


FileDialogStorage::FileDialogStorage()
{
	const SpecialFileNames& sfnm = FileModule::_instance->specialFileNames;
	if (sfnm.HasFileName(SpecialFileNames::SystemDir))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::SystemDir]);
	if (sfnm.HasFileName(SpecialFileNames::MachineRoot))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::MachineRoot]);
	if (sfnm.HasFileName(SpecialFileNames::TempDir))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::TempDir]);
	if (sfnm.HasFileName(SpecialFileNames::UserPreferencesDir))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::UserPreferencesDir]);
	if (sfnm.HasFileName(SpecialFileNames::UserHomeDir))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::UserHomeDir]);
	if (sfnm.HasFileName(SpecialFileNames::AppDir))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::AppDir]);
	if (sfnm.HasFileName(SpecialFileNames::UserDesktopDir))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::UserDesktopDir]);
	if (sfnm.HasFileName(SpecialFileNames::ProcessWorkingDir))
	    favoriteFolders.push_front(sfnm[SpecialFileNames::ProcessWorkingDir]);
	parentFolder = favoriteFolders.front();
	
	filters.push_front(String::From_c_str("*"));// everything
	filters.push_front(String::kEmpty); // nothing
}

FileDialogStorage::~FileDialogStorage()
{
}

FileFindDialog::FileFindDialog()
{
}

FileFindDialog::~FileFindDialog()
{
}

void FileFindDialog::DialogEventOk()
{
	if (curFolder->IsStateSet(View::StateFocused))
	{  // we typed something new in the curFolder, jump to that folder
		String text = curFolder->GetSelectedText();
		DialogEventFolder(text);
	}
	else if (folderContent->IsStateSet(View::StateFocused))
	{
		folderContent->SelectFocused(true);
	}
	else
	{
		String text = curFilter->GetSelectedText();
		DialogEventFilter(text);
	}
}


void FileFindDialog::DialogEventBack()
{
	dlgStorage->selectedFile = FileName();
	if (backHistory.empty())
		return;

	forwHistory.push_front(dlgStorage->parentFolder);
	dlgStorage->parentFolder = backHistory.back();
	backHistory.pop_back();
	RepopulateContent();
	dialogW.Refresh();
}

void FileFindDialog::DialogEventForward()
{
	dlgStorage->selectedFile = FileName();
	if (forwHistory.empty())
		return;

	backHistory.push_back(dlgStorage->parentFolder);
	dlgStorage->parentFolder = forwHistory.front();
	forwHistory.pop_front();
	RepopulateContent();
	dialogW.Refresh();
}

void FileFindDialog::DialogEventUpFolder()
{
	dlgStorage->selectedFile = FileName();
	if (!dlgStorage->parentFolder.HasParent())
		return;
		
	forwHistory.clear();
	backHistory.push_back(dlgStorage->parentFolder);

	dlgStorage->parentFolder = dlgStorage->parentFolder.GetParent();	
	RepopulateContent();
	dialogW.Refresh();
}

void FileFindDialog::DialogEventSelection(const String& text, bool on)
{
	if (text.isEmpty())
		return;

    FileName newfnm = FileName::FromPathNameOS(text);
    if (!on && (dlgStorage->selectedFile == newfnm))
    {
    	if (newfnm.IsDir())
	    {   // we can handle this ourselves = dive in
			forwHistory.clear();
			backHistory.push_back(dlgStorage->parentFolder);
			dlgStorage->parentFolder = newfnm;
			dlgStorage->selectedFile = FileName();
			RepopulateContent();
			dialogW.Refresh();
	    }
	    else
	    {	// activate 
	    	try
	    	{
				dlgStorage->selectedFile.OpenWithOS();
			}
			catch(...)
			{}
	    }
	}
    else
    {
    	if (on)
			dlgStorage->selectedFile = newfnm;    
		else
			dlgStorage->selectedFile = FileName();    
	}
}

void FileFindDialog::DialogEventFolder(const String& text)
{
	if (text == dlgStorage->parentFolder.GetFullPathNameOS())
		return;

    FileName newfnm = FileName::FromPathNameOS(text);
	if (!newfnm.Exists())
		return; // complain about folder not beeing there anymore

	forwHistory.clear();
	backHistory.push_back(dlgStorage->parentFolder);
	dlgStorage->parentFolder = newfnm;
	dlgStorage->selectedFile = FileName();
	RepopulateContent();
	dialogW.Refresh();

	if (newfnm.IsDir())
		return;
	// else we should take this as the final file
}

void FileFindDialog::DialogEventFilter(const String& text)
{
	if (text != dlgStorage->filters.front())
	{
		dlgStorage->filters.push_front(text);
		curFilter->AddText(text);
		curFilter->SelectText(text);
    }
	RepopulateContent();
	dialogW.Refresh();
}

void FileFindDialog::SetDlgStorage(	const refc<FileDialogStorage>& dlgSt )
{
	dlgStorage = dlgSt;
	PopulateDialog();
}

void FileFindDialog::PopulateDialog()
{
	labelFolder = new LabelView();
    dialogW.AddView(labelFolder.operator->());
	labelFolder->SetFont(dlgFont); 
	labelFolder->SetText(String::From_c_str("Folder:")); 

	labelFilter = new LabelView();
    dialogW.AddView(labelFilter.operator->());
	labelFilter->SetFont(dlgFont); 
	labelFilter->SetText(String::From_c_str("Pattern:")); 
	
	ListView::SelectionListener::owner selListener(new FilePickSelectionListener(this));
	
	curFolder = new ComboListView();
    dialogW.AddView(curFolder.operator->());
	refc<ViewSkin> cskin(dlgSkin->GetPart(String::From_c_str("Combo")));
	curFolder->SetBounds(Rect2D(Size2D(100,10))); 
	curFolder->SetSkin(cskin); 
	curFolder->SetFont(dlgFont); 
	curFolder->AddSelectionListener(selListener);
	
	curFilter = new ComboListView();
    dialogW.AddView(curFilter.operator->());
	curFilter->SetBounds(Rect2D(Size2D(100,10))); 
	curFilter->SetSkin(cskin); 
	curFilter->SetFont(dlgFont); 
	curFilter->AddSelectionListener(selListener);

	scrollPage = new ScrollPage();
	dialogW.AddView(scrollPage.operator->());
	scrollPage->SetSkin(dlgSkin->GetPart(String::From_c_str("ScrollPage"))); // for the scrollbars
	
	folderContent = new ListView();
	scrollPage->SetContent(folderContent.operator->());
	folderContent->SetSkin(dlgSkin->GetPart(String::From_c_str("List"))); 
	folderContent->SetFont(dlgFont); 
	folderContent->AddSelectionListener(selListener);

	refc<ButtonView::ActionListener> fbl(new FilePickButtonListener(this));
	backButton = new ButtonView();
	AddToolButton(backButton, fbl, String::From_c_str("Back-Button"));

	forwButton = new ButtonView();
	AddToolButton(forwButton, fbl, String::From_c_str("Forward-Button"));

	upButton = new ButtonView();
	AddToolButton(upButton, fbl, String::From_c_str("Up-Button"));

	DialogWindow::Populate(DialogWindow::DialogOk);
	if (buttonOk != 0)
		buttonOk->SetText(String::From_c_str("Go"));
	
	PopulateFields();

	if (!dialogW.GetBounds().isEmpty())
		DoLayout();
}


void FileFindDialog::DoLayout()
{
	if (dialogW == 0) return;
 	Rect2D cb(dialogW.GetContentSize());
	if (cb.isEmpty()) 	return;

	DialogWindow::DoLayout(); // let the base do it's thing first

	Size2D space(4,4);
	sint32 lineh=22;
	sint32 btnw=22;
	sint32 labelw=48;

	Rect2D bd(cb);
	bd.y += space.h;
    bd.x += space.w;
    bd.w -= space.w+space.w;
    bd.h -= space.h+space.h;

	{ // line 1
		Rect2D kb(bd);
		kb.x += kb.w;
		kb.w  = btnw;
		kb.h  = lineh;
		
		if (upButton != 0)
		{
			kb.x -= btnw;
			upButton->SetBounds(kb);
			kb.x -= space.w;
		}
		if (forwButton != 0)
		{
			kb.x -= btnw;
			forwButton->SetBounds(kb);
			kb.x -= space.w;
		}
		if (backButton != 0)
		{
			kb.x -= btnw;
			backButton->SetBounds(kb);
			kb.x -= space.w;
		}
		kb.w = kb.x - bd.x;
		kb.x = bd.x;

		Rect2D lb(bd);
		lb.h = lineh;
		lb.w = labelw;
		if (labelFolder != 0)
		{
			labelFolder->SetBounds(lb);
			kb.x += lb.w;
			kb.w -= lb.w;
	    }

		if (curFolder != 0)
			curFolder->SetBounds(kb);

		bd.y += lineh + space.h;
		bd.h -= lineh + space.h;
    }
    {  // line 2
		Rect2D kb(bd);
		kb.x += kb.w;
		kb.w  = btnw;
		kb.h  = lineh;

		if (buttonOk != 0)
		{
			kb.x -= btnw;
			buttonOk->SetBounds(kb);
			kb.x -= space.w;
		}
		kb.w = kb.x - bd.x;
		kb.x = bd.x;


		Rect2D lb(bd);
		lb.h = lineh;
		lb.w = labelw;
		if (labelFilter != 0)
		{
			labelFilter->SetBounds(lb);
			kb.x += lb.w;
			kb.w -= lb.w;
	    }

		if (curFilter != 0)
			curFilter->SetBounds(kb);

		bd.y += lineh + space.h;
		bd.h -= lineh + space.h;
	}
	{  // line 3
		if (scrollPage != 0)
			scrollPage->SetBounds(bd);
	}
}

void FileFindDialog::PopulateFields()
{
	{
		std::list<FileName>::iterator b = dlgStorage->favoriteFolders.begin();
		std::list<FileName>::iterator e = dlgStorage->favoriteFolders.end();
		for(; b!=e; ++b)
			curFolder->AddText(b->GetFullPathNameOS());
	}

	{
		std::list<String>::iterator b = dlgStorage->filters.begin();
		std::list<String>::iterator e = dlgStorage->filters.end();
		for(; b!=e; ++b)
			curFilter->AddText(*b);
		curFilter->SelectText(dlgStorage->filters.front());
	}

	RepopulateContent();
}

void FileFindDialog::RepopulateContent()
{
	FileName  folderName= dlgStorage->parentFolder;
	String	  fileName;
	if (dlgStorage->selectedFile != 0)
		fileName = dlgStorage->selectedFile.GetName(); 

	curFolder->SelectText(folderName.GetFullPathNameOS());

	folderContent->RemoveAllText();
	try
	{
		const String& filter = dlgStorage->filters.front();

		FileList flist;
		if (filter.isEmpty())
		{
			flist = FileList(folderName);
		}
		else
		{
			FileFilterPredicate fp(filter);
			FileFind(folderName, true, fp, flist); 
		}

	    FileList::size_type z = flist.size();
	    FileList::iterator bl = flist.begin();
	    FileList::iterator el = flist.end();
	    
		for(; bl!=el; ++bl)
		{   // I want folders first
			const FileProperties& fp = *bl;
			if (fp.HasProperty(FileProperties::IsDir))
				folderContent->AddText(fp.GetFileName().GetFullPathNameOS());
		}	
		for(bl = flist.begin(); bl!=el; ++bl)
		{   // ordinary files next, with filter 
			const FileProperties& fp = *bl;
			if (!fp.HasProperty(FileProperties::IsDir))
					folderContent->AddText(fp.GetFileName().GetFullPathNameOS()); // GetName
		}	
	}
	catch(...) // if we cannot look into the folder the content stays unpopulated
	{}
	backButton->StateAssign(View::StateDisabled, backHistory.empty());
	forwButton->StateAssign(View::StateDisabled, forwHistory.empty());
	upButton->StateAssign(View::StateDisabled, !folderName.HasParent());
}


#if 0
#pragma mark class MySimpleApp
#endif

class MySimpleApp : public Event_Abstract
{
	refc<SkinBuilder> skinBuilder;
	WinMgr winMgr;

public:
	MySimpleApp();
	~MySimpleApp();
	
	void Run();	
	void SkinBuildFailed(const Exception& err);
	void SkinBuildComplete();
	void CreateMainWindow();
}; // class MySimpleApp

#if 0
#pragma mark class MySkinBuildListener
#endif

class MySkinBuildListener : public SkinBuilder::Listener
{
	MySimpleApp* app;
public:
	MySkinBuildListener(MySimpleApp* t)
	:app(t)
	{}
protected:
	virtual void SkinBuildComplete()
	{
		app->SkinBuildComplete();
	}
	virtual void SkinBuildFailed(const Exception& err)
	{
		app->SkinBuildFailed(err);
	}
}; // class ImageLoader

#if 0
#pragma mark -
#endif


MySimpleApp::MySimpleApp()
{
}

MySimpleApp::~MySimpleApp()
{
}

void MySimpleApp::Run()
{
	skinBuilder = new SkinBuilder();
	skinBuilder->AddListener( new MySkinBuildListener(this) );
	skinBuilder->LoadImages();
	skinBuilder->appSkin->JoinPartsFrom(winMgr.GetSysSkin()); // Sys.Button, Sys.Edit
}

void MySimpleApp::SkinBuildFailed(const Exception& err)
{
	refc<EventLoop> el = CoreModule::_instance->mainLoop;
	el->ReportError(err);
	el->PostEvent(el->CreateQuitEvent());
}

void MySimpleApp::SkinBuildComplete()
{
	CreateMainWindow();
}

void MySimpleApp::CreateMainWindow()
{
	Rect2D lr; lr.x=160; lr.y=100; lr.h=348; lr.w=520; 
	refc<FileFindDialog> dw(new FileFindDialog());
	dw->CreateFrame(winMgr);
	dw->dialogW.SetWindowClosesApp();
    dw->dialogW.SetBgStyle(BackgroundView::Dialog);
	dw->SetTitle(String::From_c_str("Find files"));
	dw->SetContentBoundsOnScreen(lr);
	dw->SetSkin(skinBuilder->appSkin->GetPart(String::From_c_str("FilePick")), 
				skinBuilder->tahoma8Font);
	refc<FileDialogStorage> dlgStor(new FileDialogStorage());
	dw->SetDlgStorage(dlgStor);
	dw->Show();
}


#if 0
#pragma mark -
#endif

namespace XSP
{
	void MainEntryPoint()
	{
	    CoreModule _coreModule; // initialize the core module
	    _coreModule.mainLoop = new EventLoop();
	    _coreModule.mainLoop->SetErrorSink(new ErrorsToConsole());

	    FileModule _fileModule; // initialize the file module
		
		GUI_Module _gui;
		_gui.codecFactory = new SmartCodecFactory();

		#if DEBUG
		{
			bool& result = UnitTest::TestResult();
			result = true;
			try
			{ 	// add your test functions here
				String::RunUnitTest();
//				#ifdef BOOST_RE_REGEX_HPP
					RegExp::RunUnitTest();
//				#endif
				LocalTime::RunUnitTest();
				FileName::RunUnitTest();
				WinMgr::RunUnitTest();
			}
			catch(const Exception& /*err*/)
			{
				UnitTest::Failed(" exception caught while running the test chain");
			}
			catch(...)
			{
				UnitTest::Failed(" unknown exception caught while running the test chain");
			}
			// we only run the application if the test 
			// chain completes successfully
			if (!result)
				return;
		}		
		#endif

		printf("%s \n", CoreModule::_instance->runtimeVersion.FullName().c_str());
		
		refc<Event_Abstract> myApp(new MySimpleApp());
		_coreModule.mainLoop->PostEvent(myApp);
		// make sure a quit is at the end of the queue
		//_coreModule.mainLoop->PostEvent(_coreModule.mainLoop->CreateQuitEvent());
		_coreModule.mainLoop->Run();
	}
} // namespace XSP

