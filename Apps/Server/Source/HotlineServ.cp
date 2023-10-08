/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "HotlineServ.h"
#include "HotlineServIDTranslate.h"


#define kServerVersion	"\p1.9.7"

const Uint8 *kDefaultIncorrectLogin = "\pIncorrect login.";
const Uint8 *kDefaultMessageBeforedl = "\pSend a message to the public chat before downloading!";
const Uint8 *kDefaultPostBeforedl = "\pPost a message in news before downloading!";
const Uint8 *kDefaultAcceptPrivate = "\pAccept private messages and private chat please.";
const Uint8 *kDefaultWrongUserName = "\pYou are not allowed to use this name. Please change your name!";
const Uint8 *kDefaultLoginMessageText = "\pHello!";


#if WIN32
const Uint8 *kDefaultTrackerAddr = "\plorbac.net";

#else
const Uint8 *kDefaultTrackerAddr = "\pdmp.fimble.com";
#endif

const Uint8 *kMsgBoardFileName =			"\pMessageBoard.txt";


const Uint8 *kMyAgreementTextFileName =		"\pAgreement.txt";
const Uint8 *kMyErrorLogFileName = 			"\pError Log.txt";
// c'est modifi� plus bas si csv ou pas!
const Uint8 *kMyDownloadLogFileName =		"\pDownload Log.txt";
const Uint8 *kMyUploadLogFileName =			"\pUpload Log.txt";
const Uint8 *kMyChatLogFileName =			"\pChatLog.txt";

const Uint8 *kMyConnectLogFileName =		"\pConnect Log.txt";
const Uint8 *kMyAccountLogFileName =		"\pAccount Log.txt";


#if WIN32
bool _ActivateNextWindow();
bool WinIsNewsCat(const Uint8 *inName, Uint32 s);

#endif

#if MACINTOSH
extern Int16 gMacMenuBarID;
#endif


extern bool _FSAliasUI;
extern Uint32 _gTransactMaxReceiveSize;

CMyApplication *gApp = nil;

void main()

{

	UOperatingSystem::Init();
	
	try
	{
		UTransport::Init();
		
		if (!UTransport::HasTCP())
			Fail(errorType_Transport, transError_NeedTCP);
	
	#if MACINTOSH
		gMacMenuBarID = 128;
	#endif
	
	#if WIN32
		try { URez::AddProgramFileToSearchChain("\pData//hls19.dat"); } catch(...){ }
		//try { URez::AddProgramFileToSearchChain("\phlserver.dat"); } catch(...){ }
	#endif
		
		gApp = new CMyApplication;
		gApp->StartUp();
		gApp->Run();
		delete gApp;
	}
	catch (SError& err)
	{
		UError::Alert(err);
	}
}




/* ������������������������������������������������������������������������� */
#pragma mark -

CMyApplication::CMyApplication()
{	
	const SKeyCmd keyCmds[] = {
		{	viewID_Options,				key_F1,		kIsKeyCode | modKey_Shortcut	},
		{	viewID_Quit,				'q',		modKey_Shortcut					},
		{	viewID_CloseWindow,			'w',		modKey_Shortcut					}
	
	#if WIN32
		,{	viewID_WindowNext,			key_Tab,	kIsKeyCode | modKey_Control		},
		{	viewID_CloseWindow,			key_F4,		kIsKeyCode | modKey_Alt			}
	#endif

	#if !MACINTOSH	// these aren't needed on the mac due to the menubar
		,{	viewID_Broadcast,			'b',		modKey_Shortcut					},
		{	viewID_Connect,				'k',		modKey_Shortcut					},
		{	viewID_Reload,				'r',		modKey_Shortcut					},
		{	viewID_ShowLog,				'l',		modKey_Shortcut					},
		{	viewID_ShowStats,			's',		modKey_Shortcut					}
	#endif
	};

	
	
	ClearStruct(mOptions);

	mMsgData = nil;
	
	mServerAgreement = nil;
	mServerBanner = nil;
	mServerBannerType = 0;
	mServerBannerUrl = nil;
	
	mTrackerTimer = nil;

	mErrorCount = mResetCount = mFirstErrorTime = 0;
	mRefNumCounter = 0;
	mLogWin = nil;
	mStatsWin = nil;
	mToolbarWin = nil;
	mQuittingWin = nil;
	mIsQuitting = false;
	ClearStruct(mStats);
	mUserIDCounter = 0;
	mChatIDCounter = 0;
	
	mCommunityID = 0;
#if NETWORK_SERVER
	mSerialNumber[0] = 0;
	mSimConnections = 0;
#endif

	mUsersFolder = mFilesFolder = mNewsFolder = nil;
	mLastConnectAddress = mLastConnectTime = mLastConnectCount = 0;

	InstallKeyCommands(keyCmds, sizeof(keyCmds) / sizeof(SKeyCmd));
}

void CMyApplication::StartUp()
{

#if MACINTOSH_CARBON
	// reset mouse image
	UApplication::PostMessage(1112);
#endif

	// make log window - must be created first since some functions below might log errors
	mLogWin = new CMyTextWin;
	mLogWin->SetTitle("\pGLoarbLine Server Log");
	
#if !FACELESS
	LogText("\pGLoarbLine Server", kServerVersion);
#endif


	Uint16 type;
	Uint32 winVis;
	SRect winRect[3];
	
	mUsersFolder = UFS::New(kProgramFolder, nil, "\pUsers", fsOption_FailIfNotFolder + fsOption_ResolveAliases, &type);
	if (type == fsItemType_NonExistant)
		mUsersFolder->CreateFolder();
	
	mFilesFolder = UFS::New(kProgramFolder, nil, "\pFiles", fsOption_FailIfNotFolder + fsOption_ResolveAliases, &type);
	if (type == fsItemType_NonExistant)
		mFilesFolder->CreateFolder();
	
	mNewsFolder = UFS::New(kProgramFolder, nil, "\pNews", fsOption_FailIfNotFolder + fsOption_ResolveAliases, &type);
	if (type == fsItemType_NonExistant)
		mNewsFolder->CreateFolder();
	
	LoadMsgBoard();
	
	
	_FSAliasUI = false;					// don't show user interface when resolving an alias
	_gTransactMaxReceiveSize = 524288;	// allow transactions to receive max 512 KB (prevent attacks)

	ReadPrefs(winRect, &winVis);
	ReadPermBanList();
	
	LoadAgreement();
#if NETWORK_SERVER
	DecodeSerialNumber(mSerialNumber, mCommunityID, mSimConnections);
#endif
	
	// make tools window
	mToolbarWin = new CMyToolbarWin;
	UWindow::SetMainWindow(*mToolbarWin);
	if (winRect[0].IsEmpty())
		mToolbarWin->SetAutoBounds(windowPos_TopRight, windowPosOn_WinScreen);
	else
	{
		mToolbarWin->SetLocation(winRect[0].TopLeft());
		#if SHOW_SERVER_INFO_IN_TOOLBAR
		if(winRect[0].GetHeight() <= 50)	mToolbarWin->ToggleState();
		#endif
		mToolbarWin->SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}

	mToolbarWin->Show();
	
	if (winRect[1].IsEmpty())
		mLogWin->SetAutoBounds(windowPos_TopLeft, windowPosOn_WinScreen);
	else
	{
		mLogWin->SetBounds(winRect[1]);
		mLogWin->SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}

	if (winVis & myWinVis_Log) mLogWin->Show();

	// make stats window
	mStatsWin = new CMyStatsWin;
	if (winRect[2].IsEmpty())
		mStatsWin->SetAutoBounds(windowPos_BottomRight, windowPosOn_WinScreen);
	else
	{
		mStatsWin->SetLocation(winRect[2].TopLeft());
		mStatsWin->SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}

	if (winVis & myWinVis_Stats) mStatsWin->Show();
	
	// show options window if option key is down
	if (UKeyboard::IsOptionKeyDown())
		DoOptions();
	
	CheckAdminAccount();
	
	try
	{
		// main tpt
		SInternetAddress addr = { kInternetAddressType, mOptions.nBasePortNum, 0, 0, 0, 0 };
		mListenTpt.New(transport_TCPIP);
		mListenTpt->SetIPAddress(mOptions.stIPAddress);
		mListenTpt->Listen(&addr, sizeof(addr));
		mListenTpt->SetMessageHandler(MessageHandler, this);
		
		// file xfer
		addr.port = mOptions.nBasePortNum + 1;
		mTransferListenTpt.New(transport_TCPIP);
		mTransferListenTpt->SetIPAddress(mOptions.stIPAddress);
		mTransferListenTpt->Listen(&addr, sizeof(addr));
		mTransferListenTpt->SetMessageHandler(MessageHandler, this);
		
		// http
		addr.port = mOptions.nBasePortNum + 2;
		mListenHttpTpt.New(transport_TCPIP, false);
		mListenHttpTpt->SetIPAddress(mOptions.stIPAddress);
		mListenHttpTpt->Listen(&addr, sizeof(addr));
		mListenHttpTpt->SetMessageHandler(MessageHandler, this);
	
		// http file xfer
		addr.port = mOptions.nBasePortNum + 3;
		mTransferListenHttpTpt.New(transport_TCPIP, false);
		mTransferListenHttpTpt->SetIPAddress(mOptions.stIPAddress);
		mTransferListenHttpTpt->Listen(&addr, sizeof(addr));
		mTransferListenHttpTpt->SetMessageHandler(MessageHandler, this);
	}
	catch(...)
	{
	#if !FACELESS
		LogText("\pServer Error", "\pPlease change the \"Server IP\" or \"Server port number\" from \"Options/General\" and restart the server");
	#endif
	
		DisplayStandardMessage("\pServer Error", "\pCannot bind to the specified address because it is incorrect or it is already in use. Please change the \"Server IP\" or \"Server port number\" from \"Options/General\" and restart the server.", icon_Stop, 1);
		return;
	}
	
	mOptions.nCurPortNum = mOptions.nBasePortNum;
	
#if !FACELESS	
	try
	{
		// convert local address to text, and include port number if not the default
		Uint8 psLocalAddr[64];
		psLocalAddr[0] = mListenTpt->GetTransport()->GetLocalAddressText(psLocalAddr + 1, sizeof(psLocalAddr) - 1);
		if (mOptions.nBasePortNum != 5500)
		{
			psLocalAddr[++psLocalAddr[0]] = ':';
			psLocalAddr[0] += UText::IntegerToText(psLocalAddr + psLocalAddr[0] + 1, 10, mOptions.nBasePortNum);
		}
	
		LogText(psLocalAddr, "\pGLoarbLine Server started");
	}
	catch(...)
	{
		// don't throw
	}
#endif
	
	// create tracker transport and timer to regularly update the tracker
	mTrackerTpt.New(transport_UDPIP);
	mTrackerTpt->SetIPAddress(mOptions.stIPAddress);
	mTrackerTimer = NewTimer();
	if (mOptions.nOpts & myOpt_UseTracker)
	{
		mTrackerTimer->Start(300000, kRepeatingTimer);
		mTrackerTimer->Simulate();
	}
	
	
#if SHOW_SERVER_INFO_IN_TOOLBAR
	// update toolbar info
	UApplication::PostMessage(1110);
#endif

	
	// reset mouse image
	UMouse::SetImage(mouseImage_Standard);
}
CMyApplication::~CMyApplication()
{
	if (mServerAgreement)
		UMemory::Dispose(mServerAgreement);

	if (mServerBanner)
		UMemory::Dispose(mServerBanner);
		
	if (mServerBannerUrl)
		UMemory::Dispose(mServerBannerUrl);
		
	ClearPermBanList();
}

/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark �� Application Functions ��

void CMyApplication::HandleMessage(void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize)
{
	switch (inMsg)
	{
		case msg_DataArrived:
		case msg_DataTimeOut:
		case msg_ReadyToSend:
		case msg_ConnectionClosed:
		case msg_ConnectionRequest:
		case msg_ConnectionEstablished:
		case msg_ConnectionRefused:
			ProcessClients();
			ProcessTransferEstab();
			ProcessBannerDownloads();
			ProcessDownloads();
			ProcessFldrDownloads();
			ProcessUploads();
			ProcessFldrUploads();
			ProcessDownloadsWaiting();
			break;

		case msg_Special:
			MsgBox(138);
			break;

		case 999:
			DoMacMenuBar(((Int16 *)inData)[0], ((Int16 *)inData)[1]);
			break;
				
		case 1107:
			DisplayStandardMessage("\pRestart the server", "\pChanges of the Server IP or Port Number will only take effect after the server is restarted.", icon_Stop, 1);
			break;

	
		case 1109:
			CheckAdminAccount();
			break;
		
	#if SHOW_SERVER_INFO_IN_TOOLBAR
		case 1110:
			UpdateToolbarInfo();
			break;
	#endif

	#if NETWORK_SERVER
		case 1111:
			DisplayStandardMessage("\pInvalid Serial Number", "\pThe Serial Number you entered is not valid.", icon_Stop, 1);
			break;
	#endif
	
	#if MACINTOSH_CARBON
		case 1112:
			UMouse::ResetImage();
			break;
	#endif
	
		
		default:
			CApplication::HandleMessage(inObject, inMsg, inData, inDataSize);
			break;
	}
}

void CMyApplication::WindowHit(CWindow *inWindow, const SHitMsgData& inInfo)
{
	switch (inInfo.id)
	{
		case viewID_Broadcast:
			DoServerBroadcast();
			break;
		
		case viewID_Connect:
			DoAdminConnect();
			break;
		
		case viewID_Reload:
			LoadMsgBoard();
			
			LoadAgreement(true);
			break;
		
		case viewID_ShowLog:
			DoShowLog();
			break;
			
		case viewID_ShowStats:
			DoShowStats();
			break;
			
		case viewID_Options:
			DoOptions();
			break;
			
		case viewID_CancelQuit:
			DoCancelQuit();
			break;
			
		case viewID_QuitNow:
			Quit();
			break;
		case viewID_Quit:
			UserQuit();
			break;
				
	#if SHOW_SERVER_INFO_IN_TOOLBAR
		case viewID_ToolbarInfo:
			if (mToolbarWin) 
				mToolbarWin->ToggleState();
			break;
	#endif		
	
		case CWindow::ClassID:
			DoCloseWindow(inWindow);
			break;
	}
}

void CMyApplication::Error(const SError& inInfo)
{
	Uint32 nCurTime = UDateTime::GetSeconds();
	
	if (!mErrorCount || nCurTime - mFirstErrorTime > 300)	// 5 minutes
	{
		mErrorCount = 1;
		mFirstErrorTime = nCurTime;
	}
	else	// the server will be reseted is there are more that 100 errors in less that 5 minutes
	{
		mErrorCount++;
		if (mErrorCount > 100)
		{
			DebugBreak("Too many errors! Resetting...");
			try
			{
				ResetServer();
			}
			catch(...)
			{
				DebugBreak("Reset failed! Terminating...");
				UApplication::Abort();
			}
		}
	}

	StFileSysRef logFile(kProgramFolder, nil, kMyErrorLogFileName);
	UError::Log(logFile, inInfo);
	

	// just in case there was unfinished work to do
	UApplication::PostMessage(msg_DataArrived);
}

void CMyApplication::UserQuit()
{
	if (mClientList.IsNotEmpty() && (mOptions.nOpts & myOpt_ConfirmQuit))
	{
		CMyQuitWin win;
		Uint32 id;
		Uint8 str[256];
		
		USound::Beep();
		win.Show();
		
		for(;;)
		{
			win.ProcessModal();
			id = win.GetHitID();
			
			if (id == 1)	// polite quit
			{
				if (mClientList.IsNotEmpty())
				{
					UMouse::SetImage(mouseImage_Wait);
					mIsQuitting = true;

					win.GetText(str);
					if (str[0] == 0) { Quit(); return; }
					
					StFieldData data;
					data->AddPString(myField_Data, str);
					BroadcastTran(myTran_DisconnectMsg, data);
					
					UMouse::SetImage(mouseImage_Standard);
					ShowQuittingWin();
				}
				else
					Quit();
				break;
			}
			else if (id == 2)	// quit now
			{
				Quit();
				break;
			}
			else if (id == 3)	// cancel
				break;
		}
	}
	else
		Quit();
}



void CMyApplication::Quit()
{
	try {
		WritePrefs();
	} catch(...) {}		// even if we can't write the prefs, we want to let the user quit anyway
	
	CApplication::Quit();
}

void CMyApplication::KeyCommand(Uint32 inCmd, const SKeyMsgData& /* inInfo */)
{
	switch (inCmd)
	{
		case viewID_Broadcast:
			DoServerBroadcast();
			break;
		case viewID_Connect:
			DoAdminConnect();
			break;
		case viewID_Reload:
			LoadMsgBoard();
			
			LoadAgreement(true);
			break;
		case viewID_ShowLog:
			DoShowLog();
			break;
		case viewID_ShowStats:
			DoShowStats();
			break;
		case viewID_Options:
			DoOptions();
			break;
			
	#if WIN32
		case viewID_WindowNext:
			_ActivateNextWindow();
			break;
	#endif
		case viewID_Quit:
			UserQuit();
			break;
		
		case viewID_CloseWindow:
			DoCloseWindow(CWindow::GetFront(windowLayer_Standard));
			break;
	}
}

void CMyApplication::Timer(TTimer inTimer)
{
	if (inTimer == mTrackerTimer)
		UpdateTracker();
}

void CMyApplication::ShowPreferences()
{
	DoOptions();
}

void CMyApplication::DisplayStandardMessage(Uint8 *inTitle, Uint8 *inMsg, Int16 inIcon, Int16 inSound)
{
	DisplayMessage(inTitle, inMsg, inIcon, inSound);
}

bool CMyApplication::DisplayCreateAdmin()
{
	SMsgBox mb;
	ClearStruct(mb);

	Uint8 psMessage[256];
	psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "The Admin account \"%#s\" does not exist. Would you like to create it?", mAdminLogin);

	mb.icon = icon_Stop;
	mb.title = "\pCreate Admin Account";
	mb.messageSize = psMessage[0];
	mb.messageData = psMessage + 1;
	mb.button1 = "\pCreate";
	mb.button2 = "\pCancel";
	
	if (MsgBox(mb) != 1)
		return false;

	return true;
}

bool CMyApplication::DisplayAccountExists(const Uint8 *inLogin)
{
	if (!inLogin || !inLogin[0])
		return false;
	
	SMsgBox mb;
	ClearStruct(mb);

	Uint8 psMessage[256];
	psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "The account \"%#s\" already exists. Would you like to set this account as the head Admin account?", inLogin);

	mb.icon = icon_Stop;
	mb.title = "\pAccount Already Exists";
	mb.messageSize = psMessage[0];
	mb.messageData = psMessage + 1;
	mb.button1 = "\pOK";
	mb.button2 = "\pCancel";
			
	if (MsgBox(mb) != 1)
		return false;

	return true;
}

bool CMyApplication::DisplaySetAdminPrivs(const Uint8 *inLogin)
{
	if (!inLogin || !inLogin[0])
		return false;
	
	CMySetAdminPrivsWin win(inLogin);
	Uint32 id;

	win.Show();
	
	for(;;)
	{
		win.ProcessModal();
		id = win.GetHitID();
		
		if (id == 1)
			return true;
		else if (id == 2)
			return false;
	}
	
	return false;
}

bool CMyApplication::DisplayDownloadClient()
{
	CMyDownloadClientWin win;
	win.Show();
	
	for(;;)
	{
		win.ProcessModal();
		Uint32 id = win.GetHitID();
		
		if (id == 1)
			return true;
		else if (id == 2)
			return false;
	}
	
	return false;
}

/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark ���GUI Hits ��

void CMyApplication::DoShowLog()
{
	mLogWin->BringToFront();
	mLogWin->Show();
}

void CMyApplication::DoShowStats()
{
	mStatsWin->BringToFront();
	mStatsWin->Show();
}

bool CMyApplication::DoOptions()
{
	// search for options window
	if (SearchWin<CMyOptionsWin>())
	{
		USound::Beep();
		return false;
	}
	
	CMyOptionsWin win;
	Uint32 id, i;
	bool hasTrackers = false;
	Uint8 str[256];
		
	
	
	CPtrList<BanRecord> stPermBanList;
	ReadPermBanInfo(&stPermBanList);
	
	// setup the just-created options window
#if BANNER_SERVER & NETWORK_SERVER
	win.SetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mSerialNumber, mAdminLogin, 
				mAgreementBanner, mAgreementBannerUrl, mAgreementUrl, 
				&stPermBanList, mOptions);
#elif BANNER_SERVER
	win.SetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mAdminLogin, mAgreementBanner, 
				mAgreementBannerUrl, mAgreementUrl, &stPermBanList, 
				mOptions);
#elif NETWORK_SERVER
	win.SetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mSerialNumber, mAdminLogin, 
				&stPermBanList, mOptions);
#else
	win.SetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mAdminLogin, &stPermBanList, 
				mOptions);
#endif

	
	
	// setup window with tracker data
	for (i = 0; i < 10; i++)
	{
		win.SetTrackerAddress(i, mTrackInfo[i].addr.name);
	#if NEW_TRACKER
		win.SetTrackerLogin(i, mTrackInfo[i].login);
	#endif
		win.SetTrackerPassword(i, mTrackInfo[i].passwd);
	}

	// delete all the bans in the list
	{
		i = 0;
		BanRecord *banRecord;
		while (stPermBanList.GetNext(banRecord, i))
			delete banRecord;
		stPermBanList.Clear();
	}
	// show the options window
	win.Show();
	
	// go into modal loop to process modal options window
	for(;;)
	{
		win.ProcessModal();
		id = win.GetHitID();
		
		if (id == 1 || id == 3)					// Save/Unlock
		{
		#if NETWORK_SERVER
			// validate serial number
			Uint8 psSerialNumber[32];
			win.GetSerialNumber(psSerialNumber);
			if (psSerialNumber[0] && !ValidateSerialNumber(psSerialNumber))
			{
				win.FocusSerialNumber();
				UApplication::PostMessage(1111);
				continue;
			}
	
			// save the old Serial Number
			UMemory::Copy(psSerialNumber, mSerialNumber, mSerialNumber[0] + 1);
		#endif
			
			// validate admin login
			Uint8 psAdminLogin[32];
			win.GetAdminInfo(psAdminLogin);
			if (ValidateAdminAccount(psAdminLogin))
				win.SetAdminInfo(psAdminLogin);

			// save admin login
			UMemory::Copy(psAdminLogin, mAdminLogin, mAdminLogin[0] + 1);
		
		#if BANNER_SERVER
			// save agreement info
			bool bAgreementBannerUrl = (mOptions.nOpts & myOpt_AgreementImageUrl) != 0;
			
			Uint8 psAgreementBanner[128];
			UMemory::Copy(psAgreementBanner, mAgreementBanner, mAgreementBanner[0] + 1);
			
			Uint8 psAgreementBannerUrl[256];
			UMemory::Copy(psAgreementBannerUrl, mAgreementBannerUrl, mAgreementBannerUrl[0] + 1);

			Uint8 psAgreementUrl[256];
			UMemory::Copy(psAgreementUrl, mAgreementUrl, mAgreementUrl[0] + 1);
		#endif
			
			// extract general info from the options window
			SMyOptions stOptions = mOptions;

		#if BANNER_SERVER & NETWORK_SERVER
			win.GetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mSerialNumber, mAdminLogin, mAgreementBanner, mAgreementBannerUrl, mAgreementUrl, &stPermBanList, stOptions);
		#elif BANNER_SERVER
			win.GetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mAdminLogin, mAgreementBanner, mAgreementBannerUrl, mAgreementUrl, &stPermBanList, stOptions);
		#elif NETWORK_SERVER
			win.GetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mSerialNumber, mAdminLogin, &stPermBanList, stOptions);
		#else
			win.GetInfo(mServerName, mIncorrectlogin, mMessageBeforedl, mPostBeforedl, mWrongUserName, mAcceptPrivate, mLoginMessageText, mServerDesc, mAdminLogin, &stPermBanList, stOptions);
		#endif
						
			bool bRestartServer = false;
			if (stOptions.stIPAddress.un_IP.stDW_IP.nDW_IP != mOptions.stIPAddress.un_IP.stDW_IP.nDW_IP || stOptions.nBasePortNum != mOptions.nBasePortNum)
			{
				bRestartServer = true;
				
				mOptions.stIPAddress.un_IP.stDW_IP.nDW_IP = stOptions.stIPAddress.un_IP.stDW_IP.nDW_IP;
				mOptions.nBasePortNum = stOptions.nBasePortNum;
			}
			
			mOptions.nMaxDownloads = stOptions.nMaxDownloads;
			mOptions.nawaydelay = stOptions.nawaydelay;
			mOptions.nbandelay = stOptions.nbandelay;
			mOptions.nMaxDownloadsClient = stOptions.nMaxDownloadsClient;
			mOptions.nMaxConnectionsIP = stOptions.nMaxConnectionsIP;
			mOptions.nOpts = stOptions.nOpts;

		#if BANNER_SERVER
			if (bAgreementBannerUrl != ((mOptions.nOpts & myOpt_AgreementImageUrl) != 0) || 
			   (bAgreementBannerUrl && (UMemory::Compare(psAgreementBanner + 1, psAgreementBanner[0], mAgreementBanner + 1, mAgreementBanner[0]) || 
			    UMemory::Compare(psAgreementBannerUrl + 1, psAgreementBannerUrl[0], mAgreementBannerUrl + 1, mAgreementBannerUrl[0]))) ||
			   (!bAgreementBannerUrl && UMemory::Compare(psAgreementUrl + 1, psAgreementUrl[0], mAgreementUrl + 1, mAgreementUrl[0])))
				LoadAgreement();
		#endif
			
			// extract tracker data from the options window
			for (i = 0; i < 10; i++)
			{
				win.GetTrackerAddress(i, str);
			#if NEW_TRACKER
				win.GetTrackerLogin(i, mTrackInfo[i].login);
			#endif
				win.GetTrackerPassword(i, mTrackInfo[i].passwd);
				
				mTrackInfo[i].addr.name[0] = UTransport::CleanUpAddressText(kInternetNameAddressType, str+1, str[0], mTrackInfo[i].addr.name+1, 255);
				if (mTrackInfo[i].addr.name[0]) hasTrackers = true;
				mTrackInfo[i].addr.port = UMemory::SearchByte(':', mTrackInfo[i].addr.name+1, mTrackInfo[i].addr.name[0]) ? 0 : 5499;
			}
			
			if (!hasTrackers || !mServerName[0]) 
				mOptions.nOpts &= ~myOpt_UseTracker;

			
			// update the tracker
			if (mOptions.nOpts & myOpt_UseTracker)
			{
			
				mTrackerTimer->Start(300000, kRepeatingTimer);
				mTrackerTimer->Simulate();
			}
			else
				mTrackerTimer->Stop();
			
		#if NETWORK_SERVER
			// check serial number
			if (UMemory::Compare(psSerialNumber + 1, psSerialNumber[0], mSerialNumber + 1, mSerialNumber[0]))
				DecodeSerialNumber(mSerialNumber, mCommunityID, mSimConnections);
		#endif
			
			// write the just-modified prefs out to disk
			WritePrefs();
			WritePermBanInfo(&stPermBanList);
			DisconnectPermBan();
			
			// check admin account
			if (UMemory::Compare(psAdminLogin + 1, psAdminLogin[0], mAdminLogin + 1, mAdminLogin[0]))
				UApplication::PostMessage(1109);
			
			// restart server
			if (bRestartServer)	
				UApplication::PostMessage(1107);
			
			
		#if SHOW_SERVER_INFO_IN_TOOLBAR
			// options changed successfully
			UpdateToolbarInfo();
		#endif			
		
			return true;
		}
		else if (id == 2)						// Cancel
			break;
	#if BANNER_SERVER
		else if (id == 4 || id == 5)			// Banner
			win.UpdateBannerTab();
	#endif
		else if (id == 6)						// Add perm ban
			win.AddPermBan();
		else if (id == 7)						// Delete perm ban
			win.DeletePermBan();
		else if (id == 8)						// Select admin
		{
			win.GetAdminInfo(str);
			if (SelectAdminAccount(str))
				win.SetAdminInfo(str);
		}
		else if (id == 9)						// Reset admin
		{
			win.GetAdminInfo(str);
			if (ValidateAdminAccount(str))
				win.SetAdminInfo(str);
			
			ResetAdminAccount(str);
		}
	}
	
	return false;
}

void CMyApplication::DoServerBroadcast()
{
	// check the transport
	if (mListenTpt.IsInvalid() || !mOptions.nCurPortNum)
	{
		USound::Beep();
		return;
	}

	// search for broadcast window
	if (SearchWin<CMyBroadcastWin>())
	{
		USound::Beep();
		return;
	}
	
	CMyBroadcastWin win;
	Uint32 id;
		
	win.Show();
	
	for(;;)
	{
		win.ProcessModal();
		id = win.GetHitID();
		
		if (id == 1)
		{
			THdl h = win.GetTextHandle();
			if (h->GetSize() == 0)
			{
				USound::Beep();
				continue;
			}
			
			void *p;
			StHandleLocker locker(h, p);
			
			StFieldData data;
			data->AddField(myField_Data, p, UMemory::GetSize(h));
			
			BroadcastTran(myTran_ServerMsg, data);
			break;
		}
		else if (id == 2)
			break;
	}
}

void CMyApplication::DoAdminConnect()
{
	if (mListenTpt.IsInvalid() || !mOptions.nCurPortNum)
	{
		USound::Beep();
		return;
	}

	Uint8 psPassword[32];
	SMyUserDataFile stUserInfo;
			
	try
	{
		// get admin account
		GetUser(mAdminLogin, stUserInfo);
	
		// unscramble the password
		psPassword[0] = UMemory::Copy(psPassword + 1, stUserInfo.passwordData, FB(stUserInfo.passwordSize) > sizeof(psPassword) - 1 ? sizeof(psPassword) - 1 : FB(stUserInfo.passwordSize));
		Uint32 nSize = psPassword[0];
		Uint8 *pPassword = psPassword + 1;
		while (nSize--) { *pPassword = ~(*pPassword); pPassword++; }
	}
	catch(...)
	{
		// create admin account
		if (!DisplayCreateAdmin() || !CreatePasswordAdminAccount(psPassword))
			return;
	}

	// check if the Hotline protocol is registered
	if (!UTransport::IsRegisteredProtocol("\photline"))
	{
		if (DisplayDownloadClient())
		{
			const Uint8 *psUrl = "\phttp://www.lorbac.net/";
			UTransport::LaunchURL(psUrl + 1, psUrl[0]);
		}	
		
		return;
	}
		
	// compose Hotline URL
	Uint8 psAddress[32];
	psAddress[0] = mListenTpt->GetTransport()->GetLocalAddressText(psAddress + 1, sizeof(psAddress) - 1);
	
	Uint8 psLoginPassword[64];
	UMemory::Copy(psLoginPassword, mAdminLogin, mAdminLogin[0] + 1);
	if (psPassword[0])
	{
		psLoginPassword[0] += UMemory::Copy(psLoginPassword + psLoginPassword[0] + 1, ":", 1);
		psLoginPassword[0] += UMemory::Copy(psLoginPassword + psLoginPassword[0] + 1, psPassword + 1, psPassword[0]);
	}
		
	Uint8 psHotlineUrl[256];
	psHotlineUrl[0] = UText::Format(psHotlineUrl + 1, sizeof(psHotlineUrl) - 1, "hotline://%#s@%#s:%lu", psLoginPassword, psAddress, mOptions.nCurPortNum);
	
	// launch Hotline Client
	if (!UTransport::LaunchURL(psHotlineUrl + 1, psHotlineUrl[0]))
	{
		if (DisplayDownloadClient())
		{
			const Uint8 *psUrl = "\phttp://www.lorbac.net";
			UTransport::LaunchURL(psUrl + 1, psUrl[0]);
		}	
	}
}

void CMyApplication::DoCancelQuit()
{
	HideQuittingWin();
	mIsQuitting = false;
}

void CMyApplication::DoCloseWindow(CWindow *inWin)
{
	if (inWin == nil) 
		return;
	
	if (inWin == mToolbarWin)
		UserQuit();
	if (inWin == mStatsWin)
		mStatsWin->Hide();
	else if (inWin == mLogWin)
		mLogWin->Hide();
}

#if MACINTOSH
void _DoAboutComputer();
#endif

void CMyApplication::DoMacMenuBar(Int16 inMenu, Int16 inItem)
{
#if WIN32
	if (inItem < 200)
	{
		inMenu = 129;
		inItem -= 100;
	}
	else if (inItem < 300)
	{
		inMenu = 130;
		inItem -= 200;
	}
	else if (inItem < 400)
	{
		inMenu = 131;
		inItem -= 300;
	}
#endif
	
	if (inMenu == 128)			// Apple
	{
	#if MACINTOSH
		if (inItem == 1)
			_DoAboutComputer();
	#endif
	}
	else if (inMenu == 129)		// File
	{
		if (inItem == 1 && !UWindow::InModal())	// don't quit if there is a modal window opened
			UserQuit();
	}
	else if (inMenu == 130)		// Edit
	{
		SKeyMsgData ki;
		ClearStruct(ki);
		ki.mods = modKey_Command;
		
		switch (inItem)
		{
			case 1:
				ki.keyChar = 'z';
				break;
			case 3:
				ki.keyChar = 'x';
				break;
			case 4:
				ki.keyChar = 'c';
				break;
			case 5:
				ki.keyChar = 'v';
				break;
			case 6:
				ki.keyCode = key_nClear;
				ki.mods = 0;
				break;
			case 8:
				ki.keyChar = 'a';
				break;
		}
		
		TWindow win = UWindow::GetFocus();
		if (win) UWindow::PostMessage(win, msg_KeyDown, &ki, sizeof(ki), priority_KeyDown);
	}
	else if (inMenu == 131)		// Hotline
	{
		switch (inItem)
		{
			case 1:				// Broadcast
				DoServerBroadcast();
				break;
			case 2:				// Connect as Admin
				DoAdminConnect();
				break;
			case 3:				// Reload
				LoadMsgBoard();
				
				LoadAgreement(true);
				break;
			case 5:				// Show Log
				DoShowLog();
				break;
			case 6:				// Show Statistics
				DoShowStats();
				break;
			case 8:				// Options
				DoOptions();
				break;
		}
	}
}


/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark �� Utility Functions ��

#if NETWORK_SERVER
extern Uint8 ValidateBase36(Uint8 inData);

bool CMyApplication::ValidateSerialNumber(Uint8 *inSerialNumber)
{
	if (!inSerialNumber || !inSerialNumber[0])
		return false;

	Uint8 psSerialNumber[32];
	psSerialNumber[0] = 0;
	
	// eliminate invalid characters
	for (Uint32 i = 1; i <= inSerialNumber[0]; i++)
	{
		Uint8 nValue = ValidateBase36(inSerialNumber[i]);
		if (nValue)	// ignore invalid characters
			*(psSerialNumber + psSerialNumber[0]++ + 1) = nValue;
	}
	
	// check the size and the version number
	if (psSerialNumber[0] != 15 || psSerialNumber[7] != 'A')
	{
		inSerialNumber[0] = 0;
		return false;
	}

	// calculate CRC sum
	Uint32 nCrcSum = TB(UMemory::CRC(psSerialNumber + 1, 9));
	
	// copy data, CRC sum and "HTLS" in a temporar buffer
	Uint8 psTempData[32];
	psTempData[0] = UMemory::Copy(psTempData + 1, psSerialNumber + 1, 9);
	psTempData[0] += UMemory::Copy(psTempData + psTempData[0] + 1, &nCrcSum, sizeof(nCrcSum));
	psTempData[0] += UMemory::Copy(psTempData + psTempData[0] + 1, "HTLS", 4);
	
	// calculate Adler sum
	Uint32 nAdlerSum = UMemory::AdlerSum(psTempData + 1, psTempData[0]);
	nAdlerSum &= 0x7FFFFFFF;	// limit this value

	// check Adler sum
	if (nAdlerSum != TranslateFromBase36(psSerialNumber + 10, 6))
	{
		inSerialNumber[0] = 0;
		return false;
	}
	
	// format the serial number
	inSerialNumber[0] = UText::Format(inSerialNumber + 1, 31, "%-6.6s-%-4.4s-%-5.5s", psSerialNumber + 1, psSerialNumber + 7, psSerialNumber + 11);

	return true;
}

bool CMyApplication::DecodeSerialNumber(Uint8 *inSerialNumber, Uint32& outCommunityID, Uint16& outSimConnections)
{
	outCommunityID = 0;
	outSimConnections = 0;
		
	// check serial number
	if (!inSerialNumber || !inSerialNumber[0])
		return false;

	// validate the serial number
	if (!ValidateSerialNumber(inSerialNumber))
	{
		UApplication::PostMessage(1111);
		return false;
	}
	
	// translate CommunityID and outSimConnections
	outCommunityID = HLCommID_CharToInt((Int8 *)(inSerialNumber + 1));
	outSimConnections = TranslateFromBase36(inSerialNumber + 9, 2);
	
	return true;
}
#endif

void CMyApplication::LoadAgreement(bool inReload)
{
	Uint32 nTypeCode;
	LoadAgreement(kMyAgreementTextFileName, inReload, mServerAgreement, nTypeCode);

#if BANNER_SERVER
	mServerBannerType = 0;
	bool bServerBannerUrl = (mOptions.nOpts & myOpt_AgreementImageUrl) != 0;

	if (bServerBannerUrl)
	{
		if (mServerBannerUrl)
		{
			UMemory::Dispose(mServerBannerUrl);
			mServerBannerUrl = nil;
		}

		if (!LoadAgreement(mAgreementBanner, inReload, mServerBanner, nTypeCode))
			return;

		Uint8 bufExtension[4] = {0,0,0,0};
		if (mAgreementBanner[0] > 4)
		{
			UMemory::Copy(bufExtension, mAgreementBanner + mAgreementBanner[0] - 3, 4);
			UText::MakeLowercase(bufExtension, 4);
		}

		if (!UMemory::Compare(bufExtension, ".jpg", 4) || nTypeCode == TB((Uint32)'JPEG') || nTypeCode == TB((Uint32)'JFIF'))
			mServerBannerType = TB('JPEG');
		else if (!UMemory::Compare(bufExtension, ".gif", 4) || nTypeCode == TB((Uint32)'GIFf'))
			mServerBannerType = TB('GIFf');
		else if (!UMemory::Compare(bufExtension, ".pct", 4) || nTypeCode == TB((Uint32)'PICT'))
			mServerBannerType = TB('PICT');
		else if (!UMemory::Compare(bufExtension, ".bmp", 4) || !UMemory::Compare(bufExtension, ".dib", 4) || nTypeCode == TB((Uint32)'BMP '))
			mServerBannerType = TB('BMP ');
			
		if (!mServerBannerType)		// if is not an image we assume that is a QuickTime file
			mServerBannerType = nTypeCode;
		
		LoadUrl(mAgreementBannerUrl, mServerBannerUrl);
	}
	else
	{
		if (mServerBanner)
		{
			UMemory::Dispose(mServerBanner);
			mServerBanner = nil;
		}

		if (LoadUrl(mAgreementUrl, mServerBannerUrl))
			mServerBannerType = TB((Uint32)'URL ');
	}
#endif
}

bool CMyApplication::LoadAgreement(const Uint8 *inAgreementFileName, bool inReload, THdl& outAgreement, Uint32& outTypeCode)
{
	outTypeCode = 0;
	if (outAgreement)
	{
		UMemory::Dispose(outAgreement);
		outAgreement = nil;
	}
	
	if (!inAgreementFileName || !inAgreementFileName[0])
		return false;
	
	TFSRefObj* fp = UFS::New(kProgramFolder, nil, inAgreementFileName, fsOption_PreferExistingFile);
	if (!fp)
	{		
	#if !FACELESS
	
		Log("Could not find file �%#s�", inAgreementFileName);

	#endif
	
		return false;
	}
		
	scopekill(TFSRefObj, fp);	
	try
	{
		outTypeCode = fp->GetTypeCode();
		
		Uint32 nMaxSize = max_Uint16;	// max 64K for text and max 256K for images and QuickTime files
		if (outTypeCode != TB('TEXT'))
			nMaxSize *= 4;
		
		fp->Open(perm_Read);
		Uint32 nSize = fp->GetSize();
			
		if (nSize <= nMaxSize)
		{
			outAgreement = UMemory::NewHandle(nSize);		
			{
				void *pAgreement;
				StHandleLocker stLock(outAgreement, pAgreement); 
				fp->Read(0, pAgreement, nSize);
			}
			
			// replace CRLFs with CR in case we've loaded a pc text file 
			if (outTypeCode == TB('TEXT'))
				UMemory::SearchAndReplaceAll(outAgreement, 0, "\x0D\x0A", 2, "\x0D", 1);	
		}
		else
		{
			Uint8 psMsg[128];
			psMsg[0] = UText::Format(psMsg + 1, sizeof(psMsg) - 1, "<%#s> is larger that %lu bytes and cannot be used as agreement.", inAgreementFileName, nMaxSize);
		
			DisplayStandardMessage("\pAttention", psMsg, icon_Note, 132);			
			return false;
		}
	}
	catch(...)
	{
		if (outAgreement)
		{
			UMemory::Dispose(outAgreement);
			outAgreement = nil;
		}

	#if !FACELESS
	
		Log("Could not load file �%#s�", inAgreementFileName);

	#endif
	
		throw;
	}
	
#if !FACELESS	

	if (inReload)
		Log("Reloaded file �%#s�", inAgreementFileName);
	else
		Log("Loaded file �%#s�", inAgreementFileName);
	
#else

	#pragma unused(inReload)
	
#endif
	
	return true;
}

bool CMyApplication::LoadUrl(const Uint8 *inAgreementUrl, THdl& outUrl)
{
	if (outUrl)
	{
		UMemory::Dispose(outUrl);
		outUrl = nil;
	}

	if (!inAgreementUrl || !inAgreementUrl[0])
		return false;
	
	outUrl = UMemory::NewHandle(inAgreementUrl[0]);
	{
		void *pUrl;
		StHandleLocker stLock(outUrl, pUrl);
		UMemory::Copy(pUrl, inAgreementUrl + 1, inAgreementUrl[0]);
	}
	
	return true;
}



TFSRefObj* CMyApplication::GetMsgBoardFileSpec()
{
	TFSRefObj* ref;
	Uint16 type;
	
	ref = UFS::New(kProgramFolder, nil, kMsgBoardFileName, fsOption_FailIfNotFile + fsOption_ResolveAliases, &type);
	
	try
	{
		if (type == fsItemType_NonExistant)
			ref->CreateFile('TEXT', 'ttxt');
	}
	catch(...)
	{
		delete ref;
		throw;
	}
	
	return ref;
}

void CMyApplication::LoadMsgBoard()
{
	TFSRefObj* fp;
	Uint32 s;
	
	fp = GetMsgBoardFileSpec();
	scopekill(TFSRefObj, fp);
	
	try
	{
		fp->Open(perm_Read);
		s = fp->GetSize();
		
		if (mMsgData == nil)
			mMsgData = UMemory::NewHandle(s);
		else
			UMemory::Reallocate(mMsgData, s);
		
		void *p;
		StHandleLocker lock(mMsgData, p); 
		fp->Read(0, p, s);
	}
	catch(...)
	{
		UMemory::Dispose(mMsgData);
		mMsgData = nil;
		throw;
	}

#if !FACELESS
	Log("Loaded MessageBoard.txt");
#endif
}

Uint32 CMyApplication::AddToMsgBoard(TFieldData inData, const Uint8 inUserName[])
{
	Require(mMsgData);

	const Int8 kHeaderFormat[] = "From %#s (%s%2.2hu %2.2hu:%2.2hu):\r\r";
	const Uint8 kFooterText[] = "\p\r__________________________________________________________\r";

	Uint8 hdr[256];
	Int8 *month;
	Uint32 hdrSize, msgSize, s;
	TFSRefObj* fp;
	SCalendarDate cal;
	
	msgSize = inData->GetFieldSize(myField_Data);
	
	UDateTime::GetCalendarDate(calendar_Gregorian, cal);
	switch (cal.month)
	{
		case 1:		month = "Jan"; break;
		case 2:		month = "Feb"; break;
		case 3:		month = "Mar"; break;
		case 4:		month = "Apr"; break;
		case 5:		month = "May"; break;
		case 6:		month = "Jun"; break;
		case 7:		month = "Jul"; break;
		case 8:		month = "Aug"; break;
		case 9:		month = "Sep"; break;
		case 10:	month = "Oct"; break;
		case 11:	month = "Nov"; break;
		case 12:	month = "Dec"; break;
		default:	month = "???"; break;
	}
	
	hdrSize = UText::Format(hdr, sizeof(hdr), kHeaderFormat, inUserName, month, (Uint16)cal.day, (Uint16)cal.hour, (Uint16)cal.minute);
	
	UMemory::Insert(mMsgData, 0, nil, hdrSize + msgSize + kFooterText[0]);
	void *p;
	StHandleLocker locker(mMsgData, p);
	
	UMemory::Copy(p, hdr, hdrSize);
	inData->GetField(myField_Data, BPTR(p)+hdrSize, msgSize);
	UMemory::Copy(BPTR(p)+hdrSize+msgSize, kFooterText+1, kFooterText[0]);
	
	// write entire news to file
	fp = GetMsgBoardFileSpec();
	scopekill(TFSRefObj, fp);
	fp->Open(perm_ReadWrite);
	s = UMemory::GetSize(mMsgData);
	fp->SetSize(s);
	fp->Write(0, p, s);
	
	return (hdrSize + msgSize + kFooterText[0]);
}

#if !FACELESS
void CMyApplication::UpdateDisplay()
{
	Uint32 downloading = mDownloadList.GetItemCount() + mDownloadFldrList.GetItemCount();
	Uint32 uploading = mUploadList.GetItemCount() + mUploadFldrList.GetItemCount();
	Uint32 waiting = mDownloadWaitingList.GetItemCount();
	
	if (mStats.connCount > mStats.connPeak)
		mStats.connPeak = mStats.connCount;

	if (mStatsWin) 
		mStatsWin->SetStats(mStats.connCount, mStats.invisible, downloading, uploading, waiting, mStats.connPeak, mStats.connCounter, mStats.dlCounter, mStats.ulCounter);
}
#endif

#if SHOW_SERVER_INFO_IN_TOOLBAR
void CMyApplication::UpdateToolbarInfo()
{
	if (!mToolbarWin || mListenTpt.IsInvalid() || !mOptions.nBasePortNum || !mOptions.nCurPortNum)
		return;
	
	#define infosize	(p - info - 1)
	#define maxsize		(sizeof(info) - 1 - infosize)
	
	Uint8 info[256];
	Uint8 *p = info + 1;
	
	p += mListenTpt->GetTransport()->GetLocalAddressText(p, maxsize);
	
	if (mOptions.nBasePortNum != 5500)
	{
		if(maxsize)
			*p++ = ':';
	
		p+= UText::IntegerToText(p, maxsize, mOptions.nBasePortNum);
	}
	
	if (maxsize)
		*p++ = '\r';
	
	p += UMemory::Copy(p, mServerName + 1, min((Uint32)maxsize, (Uint32)mServerName[0]));
	
	info[0] = infosize;
	mToolbarWin->SetInfoText(info);
}
#endif

Uint32 CMyApplication::NewRefNum()
{
	Uint32 ref;
	
#if CONVERT_INTS
	((Uint16 *)&ref)[0] = UMath::GetRandom();										// to make it difficult to guess a ref
	((Uint16 *)&ref)[1] = ++mRefNumCounter ? mRefNumCounter : ++mRefNumCounter;		// so we get unique refs
#else
	((Uint16 *)&ref)[0] = ++mRefNumCounter ? mRefNumCounter : ++mRefNumCounter;		// so we get unique refs
	((Uint16 *)&ref)[1] = UMath::GetRandom();										// to make it difficult to guess a ref
#endif

	return ref;
}

SMyClient *CMyApplication::GetClientByID(Uint16 inID)
{
	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	
	while (client)
	{
	
	
		if (client->userID == inID)
		
			return client;
			
		
		client = client->next;
	}
	
	return nil;
}

SMyChat *CMyApplication::GetChatByID(Uint32 inID)
{
	SMyChat *chat = (SMyChat *)mChatList.GetFirst();
	
	while (chat)
	{
		if (chat->id == inID)
			return chat;
		
		chat = chat->next;
	}
	
	return nil;
}

void CMyApplication::SendTextToChat(Uint32 inChatID, const void *inText, Uint32 inTextSize)
{
	SMyChat *chatChan;
	SMyClient *client;
	Uint32 i, n;
	
	chatChan = GetChatByID(inChatID);

	if (chatChan)
	{
		StFieldData data;
		data->AddInteger(myField_ChatID, inChatID);
		data->AddField(myField_Data, inText, inTextSize);
		//gloarb
			
		SMyClient **clientList = chatChan->clientList.GetArrayPtr();
		n = chatChan->clientList.GetItemCount();

		for (i=0; i!=n; i++)
		{
			client = clientList[i];
			
			ASSERT(IsValidClient(client));
			
			if (client->tpt->IsEstablished() && HasGeneralPriv(client, myAcc_ReadChat))
				client->tpt->SendTransaction(myTran_ChatMsg, data);
				
		}
	}
}

void CMyApplication::BroadcastToChat(SMyChat *inChan, Uint16 inType, TFieldData inData)
{
	SMyClient **clientList = inChan->clientList.GetArrayPtr();
	Uint32 n = inChan->clientList.GetItemCount();
	Uint32 i;
	SMyClient *client;
	
	for (i=0; i!=n; i++)
	{
		client = clientList[i];
		
		ASSERT(IsValidClient(client));
		
		if (client->tpt->IsEstablished())
			client->tpt->SendTransaction(inType, inData);
	}
}

void CMyApplication::KillClient(SMyClient *inClient)
{
	if (!inClient)
		return;

	Uint16 userID = inClient->userID;

	mClientList.RemoveLink((CLink *)inClient);
	TTransact tpt = inClient->tpt;
	if (inClient->isVisible){
	mStats.invisible--;
	}
	delete inClient;
	delete tpt;
		
	mStats.connCount--;
	
		
	// remove client from private chats
	{
		SMyChat *chat, *nextChat;
		try
		{
			StFieldData notifData;
				
			chat = (SMyChat *)mChatList.GetFirst();
			while (chat)
			{
				if (chat->clientList.RemoveItem(inClient))
				{
					notifData->DeleteAllFields();
					notifData->AddInteger(myField_ChatID, chat->id);
					notifData->AddInteger(myField_UserID, userID);
					BroadcastToChat(chat, myTran_NotifyChatDeleteUser, notifData);
				}
					
				if (chat->clientList.GetItemCount() == 0)
				{
					nextChat = chat->next;
					mChatList.RemoveLink((CLink *)chat);
					UMemory::Dispose((TPtr)chat->subject);
					delete chat;
					chat = nextChat;
					continue;
				}
					
				chat = chat->next;
			}
		}
		catch(...)
		{
			// must remove client from chats anyway (can't leave invalid/deleted ptrs hanging around)
			chat = (SMyChat *)mChatList.GetFirst();
			while (chat)
			{
				chat->clientList.RemoveItem(inClient);
				chat = chat->next;
			}
			throw;
		}
	}
		
	StFieldData data;
	data->AddInteger(myField_UserID, userID);
	
	BroadcastTran(myTran_NotifyDeleteUser, data);
		
	

#if !FACELESS
	UpdateDisplay();
#endif
}

void CMyApplication::SetClientUserInfo(SMyClient *inClient, const Uint8 *inName, const SMyUserDataFile& inUserInfo)
{
	if (!inClient || !inName || !inName[0])
		return;

	// set access
	inClient->access = inUserInfo.access;
	
	// set and clean up real name
	Uint16 nSize = inName[0];
	if (nSize > 31) nSize = 31;
    if (nSize == 0)
    {
		inClient->realName[0] = UMemory::Copy(inClient->realName + 1, "***", 3);
    }
    else
    {
		inClient->realName[0] = UMemory::Copy(inClient->realName + 1, inName + 1, nSize);
		UText::ReplaceNonPrinting(inClient->realName + 1, inClient->realName[0], 0, '*', 1);
    }
	// set user name
	if (inClient->access.HasPriv(myAcc_AnyName))
		UMemory::Copy(inClient->userName, inClient->realName, inClient->realName[0] + 1);	
	else
	{
		nSize = FB(inUserInfo.nameSize);
		if (nSize == 0)
			inClient->userName[0] = UMemory::Copy(inClient->userName + 1, "***", 3);
		else
			inClient->userName[0] = UMemory::Copy(inClient->userName + 1, inUserInfo.nameData, nSize);
	}
	
	
	BroadcastClientUserInfo(inClient);
	
	
}

void CMyApplication::SetClientUserInfo(SMyClient *inClient, TFieldData inData, const SMyUserDataFile *inUserInfo)
{
bool allowName;
allowName = true;
	if (!inClient)
		return;

	// set access
	if (inUserInfo)
		inClient->access = inUserInfo->access;

	// set and clean up real name
	inData->GetPString(myField_UserName, inClient->realName, sizeof(inClient->realName));
	if (inClient->realName[0] == 0)
		inClient->realName[0] = UMemory::Copy(inClient->realName + 1, "***", 3);
	else
		UText::ReplaceNonPrinting(inClient->realName + 1, inClient->realName[0], 0, '*', 1);
	
	// set user name
	if (inClient->access.HasPriv(myAcc_AnyName))
		UMemory::Copy(inClient->userName, inClient->realName, inClient->realName[0] + 1);	
	else if (inUserInfo)
	{
		Uint16 nSize = FB(inUserInfo->nameSize);
		if (nSize > 31) nSize = 31;
		if (nSize == 0)
			inClient->userName[0] = UMemory::Copy(inClient->userName + 1, "***", 3);
		else
			inClient->userName[0] = UMemory::Copy(inClient->userName + 1, inUserInfo->nameData, nSize);
	}
	
	// set icon ID
	inClient->iconID = inData->GetInteger(myField_UserIconID);
		
			
	//set login time
	
	
	// set options
	Uint8 nOptions = inData->GetInteger(myField_Options);
	inClient->bRefusePrivateMsg = (nOptions & 0x01) ? true : false;
	inClient->bRefusePrivateChat = (nOptions & 0x02) ? true : false;
	inClient->bAutomaticResponse = (nOptions & 0x04) ? true : false;

	// kill if he refuse pv chat and pv msg
	//if (mOptions.nOpts & myOpt_kickdontchat)
	//{
	if(inClient->bRefusePrivateMsg || inClient->bRefusePrivateChat){
		if (!inClient->access.HasPriv(myAcc_RefuseChat)){
		StFieldData data;
		
		
		//SendErrorMsg(inTsn, mPostBeforedl);
	
		
		//data->AddPString(myField_Data, "\pAccept private messages and private chats please.");
		data->AddPString(myField_Data, mAcceptPrivate);
		
		inClient->tpt->SendTransaction(myTran_ServerMsg, data);
		inClient->tpt->StartDisconnect();
		}
	 // }	
	}
	
	
	
	
	
	
	// set automatic response
	inClient->psAutomaticResponseText[0] = 0;
	if (inClient->bAutomaticResponse)
	{
		if (inData->GetFieldSize(myField_AutomaticResponse))
			inData->GetPString(myField_AutomaticResponse, inClient->psAutomaticResponseText, sizeof(inClient->psAutomaticResponseText));
		else
			inClient->psAutomaticResponseText[0] = UText::Format(inClient->psAutomaticResponseText + 1, sizeof(inClient->psAutomaticResponseText) - 1, "I'm not able to respond right now. Please try later!");
	}
	
	BroadcastClientUserInfo(inClient);
	
	
	//test here
}

void CMyApplication::BroadcastClientUserInfo(SMyClient *inClient)
{

	if (!inClient)
		return;

	StFieldData data;
	data->AddInteger(myField_UserID, inClient->userID);
	data->AddInteger(myField_UserIconID, inClient->iconID);
	data->AddInteger(myField_UserFlags, GetClientFlags(inClient));
	data->AddPString(myField_UserName, inClient->userName);
	
	
	
	
	
		if (!inClient->isVisible)	
		{
		BroadcastTran(myTran_NotifyChangeUser, data);
			
		}
		else{
		 // si il est invisible
		BroadcastTranForVisible(myTran_NotifyChangeUser, data); //envoi juste a l'admin
		}
		
	
}

Uint16 CMyApplication::GetClientFlags(SMyClient *inClient)
{
	if (!inClient)
		return 0;
		
	Uint16 nRetVal = 
		((inClient->isAway) 							? 0x01 : 0)	|
		((inClient->isVisible) 							? 0x40 : 0)	|
		((inClient->access.HasPriv(myAcc_DisconUser))	? 0x02 : 0)	|
		((inClient->bRefusePrivateMsg) 					? 0x04 : 0)	|
		((inClient->bRefusePrivateChat)					? 0x08 : 0)	|
		((inClient->isDownloading)						? 0x20 : 0)	|
		((inClient->access.HasAllPriv())				? 0x10 : 0)	|
		((inClient->isFakeRed)							? 0x02 : 0)	|
		((inClient->access.HasNoPriv())					? 0x20 : 0)	;

	return nRetVal;
}




void CMyApplication::SetClientChatting(SMyClient *inClient, bool inChatting)
{
	if (!inClient)
		return;

	if (inClient->isChatting != (inChatting!=0))	// if change
	{
		

		inClient->isChatting = (inChatting!=0);
		
		
	}
}

void CMyApplication::SetClientPostting(SMyClient *inClient, bool inPostting)
{
	if (!inClient)
		return;

	if (inClient->isPostting != (inPostting!=0))	// if change
	{
		

		inClient->isPostting = (inPostting!=0);
		
		
	}
}



void CMyApplication::SetClientDownloading(SMyClient *inClient, bool inDownloading)
{
	if (!inClient)
		return;

	if (inClient->isDownloading != (inDownloading!=0))	// if change
	{
		StFieldData data;

		inClient->isDownloading = (inDownloading!=0);
		
		data->AddInteger(myField_UserID, inClient->userID);
		data->AddInteger(myField_UserIconID, inClient->iconID);
		data->AddInteger(myField_UserFlags, GetClientFlags(inClient));
		data->AddPString(myField_UserName, inClient->userName);
		
		//BroadcastTran(myTran_NotifyChangeUser, data);
		
		BroadcastClientUserInfo(inClient);
		
	}
}

void CMyApplication::SetClientFakeRed(SMyClient *inClient, bool inFakeRed)
{
	Uint8 truc = inFakeRed;
	if (!inClient)
		return;
		
		StFieldData data;

		
		if (inClient->isFakeRed == 1){
		inClient->isFakeRed = 0;
		}else{
		inClient->isFakeRed = 1;
		}
		data->AddInteger(myField_UserID, inClient->userID);
		data->AddInteger(myField_UserIconID, inClient->iconID);
		data->AddInteger(myField_UserFlags, GetClientFlags(inClient));
		data->AddPString(myField_UserName, inClient->userName);
		
		
		BroadcastClientUserInfo(inClient);
		
}

void CMyApplication::SetClientVisible(SMyClient *inClient, bool inVisible)
{

	Uint8 truc = inVisible;
	if (!inClient)
		return;
		
		StFieldData data;

		
		if (inClient->isVisible == 0){
		#if !FACELESS
		Log(inClient, "Became Invisible");
		#endif
		////
		mStats.invisible++;
		
		inClient->isVisible = 1;
	
		
		//Broadcast invisible
		data->AddInteger(myField_UserID, inClient->userID);
		data->AddInteger(myField_UserIconID, inClient->iconID);
		data->AddInteger(myField_UserFlags, GetClientFlags(inClient));
		data->AddPString(myField_UserName, inClient->userName);
		
		
		
		//broadcast has left
		//inClient->isVisible = 1;
		BroadcastTranForVisible(myTran_NotifyChangeUser, data);
		//Uint16 userID = inClient->userID;
	StFieldData data2;
	data2->AddInteger(myField_UserID, inClient->userID);
	
	
		BroadcastTranForInvisible(myTran_NotifyDeleteUser, data2);
		
		//BroadcastClientUserInfo(inClient);
		
		
		#if !FACELESS
		UpdateDisplay();
		#endif
		
		
		}else{
		#if !FACELESS
		Log(inClient, "Became Visible");
		#endif
		mStats.invisible--;
		inClient->isVisible = 0;
		BroadcastClientUserInfo(inClient);
		#if !FACELESS
		UpdateDisplay();
		#endif
		
		}
		
		
		
		
}


void CMyApplication::SetClientBlockDownload(SMyClient *inClient, bool inBlockDownload)
{
	Uint8 truc = inBlockDownload;
	if (!inClient)
		return;
		
		StFieldData data;

		
		if (inClient->isBlockDownload == 1){
		inClient->isBlockDownload = 0;
		}else{
		inClient->isBlockDownload = 1;
		}
		

}



void CMyApplication::SetClientAway(SMyClient *inClient, bool inAway)
{
	if (!inClient)
		return;

	if (inClient->isAway != (inAway!=0))	// if change
	{
		StFieldData data;

		inClient->isAway = (inAway!=0);
		
		data->AddInteger(myField_UserID, inClient->userID);
		data->AddInteger(myField_UserIconID, inClient->iconID);
		data->AddInteger(myField_UserFlags, GetClientFlags(inClient));
		data->AddPString(myField_UserName, inClient->userName);
		
		
		
		if (!inClient->isVisible)	
		{
			BroadcastTran(myTran_NotifyChangeUser, data);
		}else{
		BroadcastTranForVisible(myTran_NotifyChangeUser, data);
		}
		
		
		
	}
}



void CMyApplication::SendClientDownloadInfo(SMyClient *inClient, Uint32 inRefNum, Uint32 inWaitingCount)
{
	if (!inClient)
		return;

	StFieldData data;
	data->AddField(myField_RefNum, &inRefNum, sizeof(Uint32));
	data->AddInteger(myField_WaitingCount, inWaitingCount);
			
	inClient->tpt->SendTransaction(myTran_DownloadInfo, data);
}

void CMyApplication::BroadcastTran(Uint16 inType, TFieldData inData)
{

	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	
	while (client)
	{
	
		CheckName(client);
		if (client->tpt->IsEstablished() && client->hasLoggedIn)
			client->tpt->SendTransaction(inType, inData);
		
		client = client->next;
	}
}

void CMyApplication::BroadcastTranForInvisible(Uint16 inType, TFieldData inData)
{
	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	
	while (client)
	{
	
	
		CheckName(client);
	
	
		if (client->tpt->IsEstablished() && client->hasLoggedIn && !client->access.HasPriv(myAcc_Canviewinvisible))
			client->tpt->SendTransaction(inType, inData);
		
		client = client->next;
	}
}

void CMyApplication::BroadcastTranForVisible(Uint16 inType, TFieldData inData)
{
	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	
	while (client)
	{
	
	
		CheckName(client);
	
	
		if (client->tpt->IsEstablished() && client->hasLoggedIn && client->access.HasPriv(myAcc_Canviewinvisible))
			client->tpt->SendTransaction(inType, inData);
		
		client = client->next;
	}
}

void CMyApplication::BuildFileList(TFSRefObj* inFolder, bool inIsDropBox, TFieldData outData)
{
	struct {
		SMyFileInfo fi;
		Uint8 name[256];
	} fiData;
	SMyFileInfo& fi = fiData.fi;
	
	Uint8 name[256];
	Uint32 offset, typeCode, creatorCode, size, flags;
	THdl h;

	outData->DeleteAllFields();
	offset = 0;
	
	h = inFolder->GetListing();
	
	

	try
	{
		while (UFS::GetListNext(h, offset, name, &typeCode, &creatorCode, &size, nil, nil, &flags))
		{
			if ((flags & 1) == 0)	// if visible
			{
				if (inIsDropBox && typeCode != TB((Uint32)'HTft'))
					continue;
				
				fi.rsvd = 0;
				fi.nameScript = 0;
				fi.nameSize = TB((Uint16)name[0]);
				UMemory::Copy(fi.nameData, name+1, name[0]);

				fi.type = typeCode;
				fi.creator = creatorCode;
				fi.fileSize = TB(size);
				
				outData->AddField(myField_FileNameWithInfo, &fi, sizeof(SMyFileInfo) + name[0]);
			}
		}
	}
	catch(...)
	{
		UMemory::Dispose(h);
		throw;
	}

	UMemory::Dispose(h);
}

void CMyApplication::BuildNewsCatList(TFSRefObj* inFolder, TFieldData outData, Uint32 inClientVers)
{
	Uint8 name[256];
	
	union
	{
		Uint8 data[300];
		struct
		{
			Uint16 type;	// 2
			Uint16 count;
			Uint8 nameSize;
			Uint8 name[];
		
		} bundleInfo;
		
		struct
		{
			Uint16 type;	// 3
			Uint16 count;
			SGUID guid;
			Uint32 addSN;
			Uint32 delSN;
			Uint8 nameSize;
			Uint8 name[];
		
		} catInfo;
	};
	THdl h;
	Uint32 size;
	SMyNewsGroupInfo cat;
	
	outData->DeleteAllFields();
	Uint32 offset = 0;
	h = inFolder->GetListing();
	
	Uint32 type, flags;
	
	try
	{
		// the new client supports a more detailed listing.
		// the old code in else can be removed after april 15 1999
		// (since this is the day when the old client expires)
		if (inClientVers >= 15)	
		{
			while (UFS::GetListNext(h, offset, name, &type, nil, &size, nil, nil, &flags))
			{
				if (flags & 1)
					continue;	// if invisible
				
				if (type == TB((Uint32)'fldr'))
				{
					bundleInfo.type = TB((Uint16)2);
					bundleInfo.count = TB((Uint16)size);
					bundleInfo.nameSize = UMemory::Copy(bundleInfo.name, name + 1, name[0]);
				
					outData->AddField(myField_NewsCatListData15, &bundleInfo, sizeof(bundleInfo) + bundleInfo.nameSize);
				}
				else if (type == TB((Uint32)'HLNZ'))
				{
					try
					{
						
						StFileSysRef catRef(inFolder, nil, name);
						UMyNewsDatabase::GetGroupInfo(catRef, cat);
						
						catInfo.type = TB((Uint16)3);
						catInfo.count = TB((Uint16)cat.articleCount);
					#if NEW_NEWS_DB
						UGUID::Flatten(cat.id, &catInfo.guid);
						catInfo.addSN = TB(cat.addSN);
						catInfo.delSN = TB(cat.deleteSN);
					#else
						ClearStruct(cat.id);
						catInfo.addSN = 0;
						catInfo.delSN = 0;
					#endif
						catInfo.nameSize = UMemory::Copy(catInfo.name, name + 1, name[0]);
					
						outData->AddField(myField_NewsCatListData15, &catInfo, sizeof(catInfo) + catInfo.nameSize);
					}
					catch(...)
					{
						continue;	// skip this item - probably not a valid news category
					}				
				}
			}
		}
		else
		{
			while (UFS::GetListNext(h, offset, data, &type, nil, nil, nil, nil, &flags))
			{
				if (flags & 1)
					continue;	// if invisible
				
				Uint32 s = data[0];
				if (type == TB((Uint32)'fldr'))
					data[0] = 1;
				else if (type == TB((Uint32)'HLNZ'))
					data[0] = 10;
				else
				{
				#if WIN32
					if (WinIsNewsCat(data + 1, s))
						data[0] = 10;
					else
				#endif
						data[0] = 255;
				}
					
				outData->AddField(myField_NewsCatListData, data, s + 1);
			}
		}
	}
	catch(...)
	{
		UMemory::Dispose(h);
		throw;
	}

	UMemory::Dispose(h);
}

bool CMyApplication::BuildNewsArtList(TFSRefObj* inFile, TFieldData outData)
{
	Uint32 s;
	TPtr p = UMyNewsDatabase::GetArticleListing(inFile, s);
	if (!p)
		return false;
	
	scopekill(TPtrObj, p);
	
	try
	{
		outData->AddField(myField_NewsArtListData, p, s);
	}
	catch(...)
	{
		// don't throw
		return false;
	}

	return true;
}

bool CMyApplication::BuildNewsArtData(TFSRefObj* inFile, Uint32 inArticleID, const Int8 inFlav[], TFieldData outData)
{
	Uint32 nArticleSize;
	SMyNewsArticleInfo info;
	TPtr pArticle = UMyNewsDatabase::GetArticleData(inFile, inArticleID, inFlav, nArticleSize, &info);
	
	if (pArticle)	// otherwise there's no flavor of this type.  
	{
		outData->AddField(myField_NewsArtData, pArticle, nArticleSize);
		UMemory::Dispose(pArticle);
	}
	
	outData->AddInteger(myField_NewsArtPrevArt, info.prevID);
	outData->AddInteger(myField_NewsArtNextArt, info.nextID);

#if NEW_NEWS_DB
	outData->AddInteger(myField_NewsArtParentArt, info.parentID);
	outData->AddInteger(myField_NewsArt1stChildArt, info.firstChildID);
#endif
	outData->AddPString(myField_NewsArtTitle, info.title);
	outData->AddPString(myField_NewsArtPoster, info.poster);
	outData->AddCString(myField_NewsArtDataFlav, inFlav);

#if CONVERT_INTS
	info.dts.year = TB(info.dts.year);
	info.dts.msecs = TB(info.dts.msecs);
	info.dts.seconds = TB(info.dts.seconds);
#endif

	outData->AddField(myField_NewsArtDate, &info.dts, sizeof(SDateTimeStamp));
	// need a better way to get all the other info in one shot (rather than making multiple loopages through the file)
	// could do a GetOffsetFromID func and then use that offset several times (fairly fast)
	// could just make a func GetArticleDataAndInfo which gives me everything I want in one struct
	// or could do it the slow way and just make diff funcs to do all the work (GetArticlePoster, GetArticleTitle, GetPrevArticleID, GetNextArticleID
	return true;
}

void CMyApplication::ShowQuittingWin()
{
	if (mQuittingWin == nil)
	{
		mQuittingWin = new CMyQuittingWin;
		mQuittingWin->Show();
	}
}

void CMyApplication::HideQuittingWin()
{
	if (mQuittingWin)
	{
		delete mQuittingWin;
		mQuittingWin = nil;
	}
}

// sets the account for all clients logged in with the specified account
void CMyApplication::SetOnlineAccount(const Uint8 *inLogin, const SMyUserDataFile& inUserInfo)
{
	if (!inLogin || !inLogin[0])
		return;
	
	SMyClient *pClient = (SMyClient *)mClientList.GetFirst();

	while (pClient)
	{
		if (!UMemory::Compare(pClient->accountLogin + 1, pClient->accountLogin[0], inLogin + 1, inLogin[0]))
		{
			bool bBroadcastInfo = false;
			
			// check access
			if (pClient->access != inUserInfo.access)
			{
				bBroadcastInfo = true;
				
				// set access
				pClient->access = inUserInfo.access;
		
				// send user access information
				StFieldData data;
				data->AddField(myField_UserAccess, pClient->access.data, sizeof(pClient->access.data));
				pClient->tpt->SendTransaction(myTran_UserAccess, data);
			}
			
			// check account name
			Uint16 nSize = TB(inUserInfo.nameSize);
			if (nSize > 31) nSize = 31;
			if (UMemory::Compare(pClient->accountName + 1, pClient->accountName[0], inUserInfo.nameData, nSize))
				pClient->accountName[0] = UMemory::Copy(pClient->accountName + 1, inUserInfo.nameData, nSize);

			// check account login
			nSize = TB(inUserInfo.loginSize);
			if (nSize > 31) nSize = 31;
			if (UMemory::Compare(pClient->accountLogin + 1, pClient->accountLogin[0], inUserInfo.loginData, nSize))
				pClient->accountLogin[0] = UMemory::Copy(pClient->accountLogin + 1, inUserInfo.loginData, nSize);
			
			// check user name
			if (pClient->access.HasPriv(myAcc_AnyName))
			{
				if (UMemory::Compare(pClient->userName + 1, pClient->userName[0], pClient->realName + 1, pClient->realName[0]))
				{
					bBroadcastInfo = true;
					UMemory::Copy(pClient->userName, pClient->realName, pClient->realName[0] + 1);
				}
			}
			else
			{
				nSize = FB(inUserInfo.nameSize);
				if (nSize > 31) nSize = 31;
				if (UMemory::Compare(pClient->userName + 1, pClient->userName[0], inUserInfo.nameData, nSize))
				{
					bBroadcastInfo = true;
					pClient->userName[0] = UMemory::Copy(pClient->userName + 1, inUserInfo.nameData, nSize);
				}
			}

			if (bBroadcastInfo)
			
			
				BroadcastClientUserInfo(pClient);
				
				
		}
		
		pClient = pClient->next;
	}
}

// disconnect all clients logged in with the specified account
void CMyApplication::DeleteOnlineAccount(const Uint8 *inLogin, SMyClient *inClient)
{
	if (!inLogin || !inLogin[0] || !inClient)
		return;
	
	StFieldData data;
	data->AddPString(myField_Data, "\pYou are logged in with an account which was deleted.");

	SMyClient *pClient = (SMyClient *)mClientList.GetFirst();

	while (pClient)
	{
		if (!UMemory::Compare(pClient->accountLogin + 1, pClient->accountLogin[0], inLogin + 1, inLogin[0]))
		{
			// send message
			pClient->tpt->SendTransaction(myTran_ServerMsg, data);
			
			// disconnect the client
			pClient->tpt->StartDisconnect();
			UApplication::PostMessage(msg_DataArrived);
		
			// log disconnect
			Uint8 ip[256];
			Uint8 *p = pClient->userName;
			if (p[0] == 0)
			{
				ip[0] = pClient->tpt->GetTransport()->GetRemoteAddressText(ip+1, sizeof(ip)-1);
				p = ip;
			}

		#if !FACELESS
		
			Log(inClient, "Disconnect �%#s�", p);
			
		#endif
		}
		
		pClient = pClient->next;
	}
}

bool CMyApplication::IsPermBannedName(SMyClient *inClient)
{
	Uint32 i = 0;
	BanRecord *pBan;
	
	while (mPermBanList.GetNext(pBan, i))
	{
	
	
		if (pBan->MatchName(inClient->userName))
			return true;
	}
	return false;
}


bool CMyApplication::IsPermBanned(Uint32 inAddr)
{
	Uint32 i = 0;
	BanRecord *pBan;
	while (mPermBanList.GetNext(pBan, i))
	{
	
	
		if (pBan->Match(inAddr))
			return true;
	}
	return false;
}

bool CMyApplication::IsTempBanned(Uint32 inAddr)
{
	RemoveOldTempBans();
	Uint32 i = 0;
	BanRecord *pBan;
	while (mTempBanList.GetNext(pBan, i))
	{
		if (pBan->Match(inAddr))
			return true;
	}
	return false;
}

void CMyApplication::AddPermBan(Uint32 inAddr1, Uint32 inAddr2, Uint8 *inDescr)
{
	CPtrList<BanRecord> stPermBanList;
	ReadPermBanInfo(&stPermBanList);
	
	bool overflow = stPermBanList.GetItemCount() >= kMaxPermBans;
	BanRecord *pPermBanInfo = overflow 
							? stPermBanList.RemoveItem(1)
							: new BanRecord();
	if (pPermBanInfo == 0)
		return;
	pPermBanInfo->nActive = 1;
	pPermBanInfo->nAddrFirst = inAddr1;
	pPermBanInfo->nAddrLast  = inAddr2;
	pPermBanInfo->psDateTime[0] = UDateTime::DateToText(
			pPermBanInfo->psDateTime + 1, 
			sizeof(pPermBanInfo->psDateTime) - 1, 
			kShortDateText + kTimeWithSecsText);
	pPermBanInfo->timeout = Uint32(-1);
	if (inDescr == 0)
		pPermBanInfo->description[0] = 0;
	else
		pPermBanInfo->description[0] = inDescr[0];
	if (pPermBanInfo->description[0]+1 > sizeof(pPermBanInfo->description))
		pPermBanInfo->description[0] = sizeof(pPermBanInfo->description)-1;
	if (inDescr != 0)
		UMemory::Copy(pPermBanInfo->description+1, 
					  inDescr, 
					  pPermBanInfo->description[0]);
	stPermBanList.AddItem(pPermBanInfo);

	WritePermBanInfo(&stPermBanList);
}

void CMyApplication::AddTempBan(Uint32 inAddr)
{
	bool overflow = mTempBanList.GetItemCount() >= kMaxTempBans;
	BanRecord *pPermBanInfo = overflow 
							? mTempBanList.RemoveItem(1)
							: new BanRecord();
	if (pPermBanInfo == 0)
		return;
	pPermBanInfo->nActive = 1;
	pPermBanInfo->nAddrFirst = inAddr;
	pPermBanInfo->nAddrLast  = inAddr;
	pPermBanInfo->psDateTime[0] = UDateTime::DateToText(
			pPermBanInfo->psDateTime + 1, 
			sizeof(pPermBanInfo->psDateTime) - 1, 
			kShortDateText + kTimeWithSecsText);
	//pPermBanInfo->timeout = UDateTime::GetSeconds()+kBanTimeLimitSecs;
	pPermBanInfo->timeout = UDateTime::GetSeconds()+mOptions.nbandelay*60;
	tempbantimeout = pPermBanInfo->timeout;
	pPermBanInfo->description[0] = 0;
	mTempBanList.AddItem(pPermBanInfo);
}

void CMyApplication::RemoveOldTempBans()
{
	Uint32 count = mTempBanList.GetItemCount();
	if (count == 0)
		return;
	Uint32 curTime = UDateTime::GetSeconds();
	
	for(Uint32 k = 1; k<=count; ++k)
	{
		BanRecord *pBan = mTempBanList.GetItem(k);
		if (curTime >= pBan->timeout)
		{
			mTempBanList.RemoveItem(k);
			delete pBan;
		}
	}
}

void CMyApplication::DisconnectPermBan()
{
	StFieldData data;
	data->AddPString(myField_Data, "\pYou are permanently banned on this server.");

	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	
	while (client)
	{
		if (IsPermBanned(client->ipAddress))
		{
			// send message
			client->tpt->SendTransaction(myTran_ServerMsg, data);
			
			
			// disconnect the client
			client->tpt->StartDisconnect();
			UApplication::PostMessage(msg_DataArrived);
		
			// log disconnect
			Uint8 ip[256];
			Uint8 *p = client->userName;
			if (p[0] == 0)
			{
				ip[0] = client->tpt->GetTransport()->GetRemoteAddressText(ip+1, sizeof(ip)-1);
				p = ip;
			}

		#if !FACELESS
		
			Log(client, "Disconnect & permanent ban �%#s�", p);
		
		#endif
		}
		
		client = client->next;
	}
}

// return the number of clients with the specified IP address
Uint32 CMyApplication::GetClientAddressCount(Uint32 inAddr)
{
	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	Uint32 n = 0;
	
	while (client)
	{
		if (client->ipAddress == inAddr) n++;
		
		client = client->next;
	}
	
	return n;
}

Uint16 CMyApplication::GetClientDownloadsNumber(Uint16 inClientID)
{
	Uint16 nClientDownloadsNum = 0;

	Uint32 i = 0;
	SMyDownloadData *pDownloadData;
	while (mDownloadList.GetNext(pDownloadData, i))
	{
		if (pDownloadData->clientID == inClientID)
			nClientDownloadsNum++;
	}
	
	i = 0;
	SMyDownloadFldrData *pDownloadFldrData;
	while (mDownloadFldrList.GetNext(pDownloadFldrData, i))
	{
		if (pDownloadFldrData->clientID == inClientID)
			nClientDownloadsNum++;
	}

	return nClientDownloadsNum;
}

// generates a random (for security), unique chat ID
Uint32 CMyApplication::NewUniqueChatID()
{
	Uint32 id = (UMath::GetRandom() & 0xFFFFFF00) | ++mChatIDCounter;	// the random is necessary so people can't guess it

	// we have to check that the ID is not already being used
startAgain:
	if (id == 0) id = 1;								// must not have 0 as the ID
	SMyChat *chat = (SMyChat *)mChatList.GetFirst();
	while (chat)
	{
		if (chat->id == id)		// if ID collision
		{
			id++;
			goto startAgain;
		}
		
		chat = chat->next;
	}
	
	return id;
}

/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark �� Log Functions ��

#if !FACELESS
void CMyApplication::Log(const Int8 inFormat[], ...)
{
	Uint8 str[256];
	
	va_list va;
	va_start(va, inFormat);
	str[0] = UText::FormatArg(str+1, sizeof(str)-1, inFormat, va);
	va_end(va);
	
	LogText("\p", str);
}

void CMyApplication::Log(SMyClient *inClient, const Int8 inFormat[], ...)
{
	Uint8 ip[256];
	Uint8 str[256];
	Uint8 *p = "\p";
	
	va_list va;
	va_start(va, inFormat);
	str[0] = UText::FormatArg(str+1, sizeof(str)-1, inFormat, va);
	va_end(va);
	
	if (inClient)
	{
		p = inClient->userName;
		if (p[0] == 0 && inClient->tpt)
		{
			ip[0] = inClient->tpt->GetTransport()->GetRemoteAddressText(ip+1, sizeof(ip)-1);
			p = ip;
		}
	}
	
	LogText(p, str);
}

void CMyApplication::Log(TTransport inTpt, const Int8 inFormat[], ...)
{
	Uint8 ip[256];
	Uint8 str[256];
	
	va_list va;
	va_start(va, inFormat);
	str[0] = UText::FormatArg(str+1, sizeof(str)-1, inFormat, va);
	va_end(va);
	
	if (inTpt)
	{
		ip[0] = inTpt->GetRemoteAddressText(ip+1, sizeof(ip)-1);
		LogText(ip, str);
	}
	else
		LogText("\p", str);
}

void CMyApplication::Log(SMyDownloadWaitingData *inDownloadWaiting, const Int8 inText[])
{		
	TTransport tpt = nil;
	
	if (inDownloadWaiting)
	{	
		if (inDownloadWaiting->bFileFolder)
		{
			tpt = inDownloadWaiting->pDownloadData->tpt;
			
			if (!tpt && mClientList.IsInList((CLink *)inDownloadWaiting->pDownloadData->client))			
				tpt = inDownloadWaiting->pDownloadData->client->tpt->GetTransport();
		}
		else
		{
			tpt = inDownloadWaiting->pDownloadFldrData->tpt;
			
			if (!tpt && mClientList.IsInList((CLink *)inDownloadWaiting->pDownloadFldrData->client))			
				tpt = inDownloadWaiting->pDownloadFldrData->client->tpt->GetTransport();
		}
	}
		
	Log(tpt, inText);
	
}

void CMyApplication::LogText(const Uint8 inContextName[], const Uint8 inText[])
{
	Uint8 date[128];
	Uint8 buf[256];
	Uint32 bufSize;
	Uint8 *p;
	CTextView *v = mLogWin->GetTextView();
	
	// clear log if getting too big
	THdl h = v->GetTextHandle();
	if (h)
	{
		if (UMemory::GetSize(h) > 16384)	// 16K
			v->DeleteText(0, max_Uint32);
	}

	// get date/time text
	date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateFullYearText + kTimeWithSecsText);
	
	// format log line
	bufSize = UText::Format(buf, sizeof(buf), "\r%#-22.22s %#-20.20s %#s", date, inContextName, inText);
	p = buf;
	if (v->IsEmpty())
	{
		p++;
		bufSize--;
	}
		
	// append to log
	v->InsertText(max_Uint32, p, bufSize);
	v->SetFullHeight();
	mLogWin->ScrollToBottom();
}
#endif

void CMyApplication::LogDownload(SMyClient *inClient, const Uint8 inFileName[])
{
	if (mOptions.nOpts & myOpt_LogDownloads)
	{
		Uint8 buf[1024];
		Uint8 date[128];
		Uint8 addr[128];
		Uint32 s;
		Uint16 type;
		
		date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateFullYearText + kTimeWithSecsText);
		addr[0] = inClient->tpt->GetTransport()->GetRemoteAddressText(addr+1, sizeof(addr)-1);

if (mOptions.nOpts & myOpt_CsvMode)
	{
		buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s;%#s;%#s;%#s;%#s;", date, addr, inClient->accountLogin, inClient->userName, inFileName);
kMyDownloadLogFileName =		"\pDownload Log.csv";
}else{
	buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s\t%#s\t%#s\t%#s\t%#s", date, addr, inClient->accountLogin, inClient->userName, inFileName);
kMyDownloadLogFileName =		"\pDownload Log.txt";
}
		
		StFileSysRef logFile(kProgramFolder, nil, kMyDownloadLogFileName, fsOption_FailIfNotFile + fsOption_ResolveAliases, &type);
		if (type == fsItemType_NonExistant)
			logFile->CreateFile('TEXT', 'ttxt');
		
		logFile->Open(perm_ReadWrite);
		s = logFile->GetSize();
		if (s == 0)
		{

		if (mOptions.nOpts & myOpt_CsvMode)
	{
		const Uint8 *titleStr = "\pDate/time;Address;Account;Name;File Name;";
		s = titleStr[0];
		logFile->Write(0, titleStr+1, s);
	}else{
	const Uint8 *titleStr = "\pDate/time\tAddress\tAccount\tName\tFile Name";
	s = titleStr[0];
	logFile->Write(0, titleStr+1, s);
			
}
			
			
			
		}

		logFile->Write(s, buf+1, buf[0]);
	}
}

void CMyApplication::LogUpload(SMyClient *inClient, const Uint8 inFileName[])
{
	if (mOptions.nOpts & myOpt_LogUploads)
	{
		Uint8 buf[1024];
		Uint8 date[128];
		Uint8 addr[128];
		Uint32 s;
		Uint16 type;
		
		date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateFullYearText + kTimeWithSecsText);
		addr[0] = inClient->tpt->GetTransport()->GetRemoteAddressText(addr+1, sizeof(addr)-1);
if (mOptions.nOpts & myOpt_CsvMode)
	{
		buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s;%#s;%#s;%#s;%#s;", date, addr, inClient->accountLogin, inClient->userName, inFileName);
kMyUploadLogFileName =			"\pUpload Log.csv";
}else{
	buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s\t%#s\t%#s\t%#s\t%#s", date, addr, inClient->accountLogin, inClient->userName, inFileName);
kMyUploadLogFileName =			"\pUpload Log.txt";
}
		StFileSysRef logFile(kProgramFolder, nil, kMyUploadLogFileName, fsOption_FailIfNotFile + fsOption_ResolveAliases, &type);
		if (type == fsItemType_NonExistant)
			logFile->CreateFile('TEXT', 'ttxt');
		
		logFile->Open(perm_ReadWrite);
		s = logFile->GetSize();
		if (s == 0)
		{
if (mOptions.nOpts & myOpt_CsvMode)
	{
		const Uint8 *titleStr = "\pDate/time;Address;Account;Name;File Name;";
		s = titleStr[0];
			logFile->Write(0, titleStr+1, s);
	}else{
	const Uint8 *titleStr = "\pDate/time\tAddress\tAccount\tName\tFile Name";
			s = titleStr[0];
			logFile->Write(0, titleStr+1, s);
}
			
		}

		logFile->Write(s, buf+1, buf[0]);
	}
}

void CMyApplication::LogChats(SMyClient *inClient, const Uint8 inChatString[])
{
	if (mOptions.nOpts & myOpt_LogChats)
	{
		Uint8 buf[2048];
		Uint8 date[128];
		Uint32 s;
		Uint16 type;
		
		date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateFullYearText + kTimeWithSecsText);
		
		if (mOptions.nOpts & myOpt_CsvMode){
			buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s;%#s;%#s;%#s;", date, inClient->accountLogin, inClient->userName, inChatString);
			kMyChatLogFileName =			"\pChat Log.csv";
		}else{
			buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s\t%#s\t%#s\t%#s", date, inClient->accountLogin, inClient->userName, inChatString);
			kMyChatLogFileName =			"\pChat Log.txt";
		}
		StFileSysRef logFile(kProgramFolder, nil, kMyChatLogFileName, fsOption_FailIfNotFile + fsOption_ResolveAliases, &type);
		if (type == fsItemType_NonExistant)
			logFile->CreateFile('TEXT', 'ttxt');
		
		logFile->Open(perm_ReadWrite);
		s = logFile->GetSize();
		if (s == 0){
			if (mOptions.nOpts & myOpt_CsvMode){
				const Uint8 *titleStr = "\pDate/time;Account;Name;Chat;";
				s = titleStr[0];
				logFile->Write(0, titleStr+1, s);
			}else{
				const Uint8 *titleStr = "\p\tDate/time\t\tAccount\tName\tChat";
				s = titleStr[0];
				logFile->Write(0, titleStr+1, s);
			}
			
		}

		logFile->Write(s, buf+1, buf[0]);
	}
}

void CMyApplication::LogConnect(SMyClient *inClient)
{
	if (mOptions.nOpts & myOpt_LogConnects)
	{
		Uint8 buf[1024];
		Uint8 date[128];
		Uint8 addr[128];
		Uint32 s;
		Uint16 type;
		
		date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateFullYearText + kTimeWithSecsText);
		addr[0] = inClient->tpt->GetTransport()->GetRemoteAddressText(addr+1, sizeof(addr)-1);
if (mOptions.nOpts & myOpt_CsvMode)
	{
		buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s;%#s;%#s;%#s;", date, addr, inClient->accountLogin, inClient->userName);
kMyConnectLogFileName =		"\pConnect Log.csv";
}else{
	buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s\t%#s\t%#s\t%#s", date, addr, inClient->accountLogin, inClient->userName);
kMyConnectLogFileName =		"\pConnect Log.txt";
}
		
		StFileSysRef logFile(kProgramFolder, nil, kMyConnectLogFileName, fsOption_FailIfNotFile + fsOption_ResolveAliases, &type);
		if (type == fsItemType_NonExistant)
			logFile->CreateFile('TEXT', 'ttxt');
		
		logFile->Open(perm_ReadWrite);
		s = logFile->GetSize();
		if (s == 0)
		{
		if (mOptions.nOpts & myOpt_CsvMode)
	{
		const Uint8 *titleStr = "\pDate/time;Address;Account;Name;";
		s = titleStr[0];
			logFile->Write(0, titleStr+1, s);
	}else{
	const Uint8 *titleStr = "\pDate/time\tAddress\tAccount\tName";
		s = titleStr[0];
			logFile->Write(0, titleStr+1, s);	
}

			
			
		}

		logFile->Write(s, buf+1, buf[0]);
	}
}

void CMyApplication::LogAccountChange(SMyClient *inClient, const Uint8 *inAction, const Uint8 *inAffectedAccount)
{
	if (mOptions.nOpts & myOpt_LogAccountChanges)
	{
		Uint8 buf[1024];
		Uint8 date[128];
		Uint8 addr[128];
		Uint32 s;
		Uint16 type;
		
		date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateFullYearText + kTimeWithSecsText);
		addr[0] = inClient->tpt->GetTransport()->GetRemoteAddressText(addr+1, sizeof(addr)-1);
if (mOptions.nOpts & myOpt_CsvMode)
	{
		buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s;%#s;%#s;%#s;%#s;%#s;", date, addr, inClient->accountLogin, inClient->userName, inAction, inAffectedAccount);
kMyAccountLogFileName =		"\pAccount Log.csv";
}else{
	buf[0] = UText::Format(buf+1, sizeof(buf)-1, "\r%#s\t%#s\t%#s\t%#s\t%#s\t%#s", date, addr, inClient->accountLogin, inClient->userName, inAction, inAffectedAccount);
kMyAccountLogFileName =		"\pAccount Log.txt";
}
		
		StFileSysRef logFile(kProgramFolder, nil, kMyAccountLogFileName, fsOption_FailIfNotFile + fsOption_ResolveAliases, &type);
		if (type == fsItemType_NonExistant)
			logFile->CreateFile('TEXT', 'ttxt');
		
		logFile->Open(perm_ReadWrite);
		s = logFile->GetSize();
		if (s == 0)
		{
		if (mOptions.nOpts & myOpt_CsvMode)
	{
		const Uint8 *titleStr = "\pDate/time;Address;Account;Name;Action;Affected Account;";
		s = titleStr[0];
			logFile->Write(0, titleStr+1, s);
	}else{
	const Uint8 *titleStr = "\pDate/time\tAddress\tAccount\tName\tAction\tAffected Account";
	s = titleStr[0];
			logFile->Write(0, titleStr+1, s);		
}
			
			
		}

		logFile->Write(s, buf+1, buf[0]);
	}
}

/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark �� User Data File ��

THdl CMyApplication::GetUserFolderList()
{
	if (!mUsersFolder)
		return nil;
	
	return mUsersFolder->GetListing();	
}

void CMyApplication::NewUser(const SMyUserDataFile& inUserInfo)
{
	if (!mUsersFolder)
		return;

	Uint8 loginName[256];
	TFSRefObj* dataFile = nil;
	
	// grab a copy of the login
	loginName[0] = FB(inUserInfo.loginSize);
	UMemory::Copy(loginName+1, inUserInfo.loginData, loginName[0]);
	
	// create folder for this user
	StFileSysRef folder(mUsersFolder, nil, loginName, fsOption_FailIfExists);
	folder->CreateFolder();
	
	try
	{
		// create data file
		dataFile = UFS::New(folder, nil, "\pUserData");
		dataFile->CreateFile('HTud', 'HTLS');
		
		// write data file
		dataFile->Open(perm_ReadWrite);
		dataFile->SetSize(sizeof(SMyUserDataFile));
		dataFile->Write(0, &inUserInfo, sizeof(SMyUserDataFile));
	}
	catch(...)
	{
		// clean up
		if (dataFile)
		{
			dataFile->Close();
			dataFile->DeleteFile();
			delete dataFile;
		}
		folder->DeleteFolder();
		throw;
	}
	
	delete dataFile;
}

void CMyApplication::DeleteUser(const Uint8 *inLogin)
{
	if (!mUsersFolder || !inLogin || !inLogin[0])
		return;

	StFileSysRef folder(mUsersFolder, nil, inLogin, fsOption_RequireExistingFolder);
	folder->MoveToTrash();
}

void CMyApplication::GetUser(const Uint8 *inLogin, SMyUserDataFile& outUserInfo)
{
	if (!mUsersFolder || !inLogin || !inLogin[0])
		Fail(errorType_FileSys, fsError_NoSuchItem);	// fix for the "Operation Blackout" vulnerability

	StFileSysRef folder(mUsersFolder, nil, inLogin, fsOption_RequireExistingFolder);
	
	// disallow hidden accounts
	if (folder->IsHidden()) 
		Fail(errorType_FileSys, fsError_NoSuchItem);
	
	StFileSysRef dataFile(folder, nil, "\pUserData", fsOption_RequireExistingFile);
	
	dataFile->Open(perm_Read);
	if (dataFile->Read(0, &outUserInfo, sizeof(SMyUserDataFile)) != sizeof(SMyUserDataFile))
		Fail(errorType_Misc, error_FormatUnknown);

	if (outUserInfo.version != TB((Uint16)kMyUserDataFileVersion))
		Fail(errorType_Misc, error_VersionUnknown);
}

void CMyApplication::CheckName(SMyClient *inClient)
{

//Verify Username

if (mOptions.nOpts & myOpt_kickunnamedusers)
	{
	
				
				
		if (IsPermBannedName(inClient)){
			StFieldData data;
	
			data->AddPString(myField_Data, mWrongUserName);
			inClient->tpt->SendTransaction(myTran_ServerMsg, data);
			inClient->tpt->StartDisconnect();
		
					
		}

	}


}


void CMyApplication::RenameUser(const Uint8 *inLogin, const SMyUserDataFile& inUserInfo)
{
	if (!mUsersFolder || !inLogin || !inLogin[0])
		return;
		
	// grab a copy of the login
	Uint8 psLogin[32];
	psLogin[0] = UMemory::Copy(psLogin + 1, inUserInfo.loginData, TB(inUserInfo.loginSize) > sizeof(psLogin) - 1 ? sizeof(psLogin) - 1 : TB(inUserInfo.loginSize));

	// rename folder
	StFileSysRef fpUsersFolder(mUsersFolder, nil, inLogin, fsOption_RequireExistingFolder);
	fpUsersFolder->SetName(psLogin);
}

void CMyApplication::SetUser(const SMyUserDataFile& inUserInfo)
{
	if (!mUsersFolder)
		return;

	Uint8 loginName[256];

	// grab a copy of the login
	loginName[0] = FB(inUserInfo.loginSize);
	UMemory::Copy(loginName+1, inUserInfo.loginData, loginName[0]);

	StFileSysRef dataFile(mUsersFolder, loginName, "\pUserData", fsOption_RequireExistingFile);
	
	dataFile->Open(perm_ReadWrite);
	dataFile->SetSize(sizeof(SMyUserDataFile));
	dataFile->Write(0, &inUserInfo, sizeof(SMyUserDataFile));
}

bool CMyApplication::ExistsUser(const Uint8 *inUserLogin)
{
	if (!mUsersFolder || !inUserLogin || !inUserLogin[0])
		return false;

	StFileSysRef fpUsersFolder(mUsersFolder, nil, inUserLogin);
	if (!fpUsersFolder->Exists())
		return false;
		
	return true;
}

bool CMyApplication::ExistsUser(const SMyUserDataFile& inUserInfo)
{
	if (!mUsersFolder)
		return false;

	// grab a copy of the login
	Uint8 psLogin[32];
	psLogin[0] = UMemory::Copy(psLogin + 1, inUserInfo.loginData, TB(inUserInfo.loginSize) > sizeof(psLogin) - 1 ? sizeof(psLogin) - 1 : TB(inUserInfo.loginSize));

	StFileSysRef fpUsersFolder(mUsersFolder, nil, psLogin);
	if (!fpUsersFolder->Exists())
		return false;
		
	return true;
}

bool CMyApplication::SetAdmin(const Uint8 *inLogin, const Uint8 *inPassword, bool inSetAccess)
{
	if (!inLogin || !inLogin[0])
		return false;
		
	bool bNewUser = false;
	SMyUserDataFile stUserInfo;
			
	try
	{
		GetUser(inLogin, stUserInfo);		
	}
	catch(...)
	{
		bNewUser = true;
		ClearStruct(stUserInfo);
	}
	
	stUserInfo.version = TB((Uint16)kMyUserDataFileVersion);

	if (inPassword && inPassword[0])
	{
		// scramble the password
		Uint8 psPassword[32];
		psPassword[0] = UMemory::Copy(psPassword + 1, inPassword + 1, inPassword[0] > sizeof(psPassword) - 1 ? sizeof(psPassword) - 1 : inPassword[0]);
		Uint32 nSize = psPassword[0];
		Uint8 *pPassword = psPassword + 1;
		while (nSize--) { *pPassword = ~(*pPassword); pPassword++; }

		stUserInfo.passwordSize = TB((Uint16)UMemory::Copy(stUserInfo.passwordData, psPassword + 1, psPassword[0]));
	}

	if (bNewUser)
	{
		stUserInfo.nameSize = TB((Uint16)UMemory::Copy(stUserInfo.nameData, "administrator", 13));
		stUserInfo.loginSize = TB((Uint16)UMemory::Copy(stUserInfo.loginData, inLogin + 1, inLogin[0]));
		stUserInfo.access.SetAllPriv();

		try
		{
			NewUser(stUserInfo);
		}
		catch(...)
		{
			return false;
		}
	}
	else
	{		
		if (inSetAccess)
			stUserInfo.access.SetAllPriv();

		try
		{
			SetUser(stUserInfo);
		}
		catch(...)
		{
			return false;
		}		
	}
	
	return true;
}

TFSRefObj* CMyApplication::GetClientRootFolder(SMyClient *inClient)
{
	TFSRefObj* ref;
	
	ref = UFS::New(mUsersFolder, inClient->accountLogin, "\pFiles", fsOption_PreferExistingFolder);
	if (ref) return ref;
	
	return mFilesFolder->Clone();
}

TFSRefObj* CMyApplication::GetClientRootNewsFolder(SMyClient *inClient)
{
	TFSRefObj* ref;
	
	ref = UFS::New(mUsersFolder, inClient->accountLogin, "\pNews", fsOption_PreferExistingFolder);
	if (ref) return ref;
	
	return mNewsFolder->Clone();
}

bool CMyApplication::CheckAdminAccount()
{
	SMyUserDataFile stUserInfo;
			
	try
	{
		GetUser(mAdminLogin, stUserInfo);		
	}
	catch(...)
	{
		return CreatePasswordAdminAccount();
	}

	if (!stUserInfo.passwordSize)
		return SetAdminAccount();
		
	return true;
}

bool CMyApplication::SetAdminAccount()
{
	CMySetAdminPassWin win;
	Uint32 id;

	Uint8 bufMessage[512];
	Uint32 nMessageSize = UText::Format(bufMessage, sizeof(bufMessage), "The Admin account \"%#s\" does not have a password assigned. This account has full access to all controls of the Server and you should not reveal this password to anyone to whom you do not want to give control of the Server. Please enter a new password for this account. If you do not set a password, your Server will not be secure.", mAdminLogin);

	// setup window
	win.SetTitle("\pSet Admin Password");
	win.SetInfo(mAdminLogin, bufMessage, nMessageSize);

	win.Show();
	
	for(;;)
	{
		win.ProcessModal();
		id = win.GetHitID();
		
		if (id == 1)
		{
			Uint8 psPassword[32];
			if (!win.GetInfo(psPassword))
				continue;
			
			return SetAdmin(mAdminLogin, psPassword, false);
		}
		else if (id == 2)
			return false;
		else if (id == 10)
			win.UpdateButtons();
	}
	
	return false;
}

bool CMyApplication::SelectAdminAccount(Uint8 *ioAdminLogin)
{
	Uint32 id;

	CMySelectAdminWin *win = new CMySelectAdminWin();
	win->SetInfo(ioAdminLogin);
	win->Show();
	
	for(;;)
	{
		win->ProcessModal();
		id = win->GetHitID();
		
		if (id == 1 || id == 4)
		{
			if (!win->GetInfo(ioAdminLogin))
			{
				if (!DisplaySetAdminPrivs(ioAdminLogin) || !SetAdmin(ioAdminLogin, nil))
					continue;
			}
			
			delete win;
			return true;
		}
		else if (id == 2)
			break;
		else if (id == 3)
		{
			delete win;
			return CreateLoginAdminAccount(ioAdminLogin);
		}
	}
	
	delete win;
	return false;
}

bool CMyApplication::CreateLoginAdminAccount(Uint8 *outAdminLogin)
{
	if (!outAdminLogin)
		return false;
	
	Uint32 id;

	CMyNewAdminAccountWin win;
	win.Show();
	
	for(;;)
	{
		win.ProcessModal();
		id = win.GetHitID();
		
		if (id == 1)
		{
			Uint8 psPassword[32];
			if (!win.GetInfo(outAdminLogin, psPassword))
				continue;
						
			if (ValidateAdminAccount(outAdminLogin))
			{
				win.SetInfo(outAdminLogin);
				continue;
			}
			
			if (!ExistsUser(outAdminLogin))		
				return SetAdmin(outAdminLogin, psPassword);
				
			if (DisplayAccountExists(outAdminLogin))
				return true;
		}
		else if (id == 2)
			return false;
		else if (id == 10)
			win.UpdateButtons();
	}
	
	return false;
}

bool CMyApplication::CreatePasswordAdminAccount(Uint8 *outAdminPassword)
{
	CMySetAdminPassWin win;
	Uint32 id;

	Uint8 bufMessage[512];
	Uint32 nMessageSize = UText::Format(bufMessage, sizeof(bufMessage), "The Admin account \"%#s\" does not exist. Please enter a password for this account so that it can be created. This account has full access to all controls of the Server and you should not reveal this password to anyone to whom you do not want to give control of the Server. Please enter a new password for this account. If you do not set a password, your Server will not be secure.", mAdminLogin);

	// setup window
	win.SetTitle("\pSet Admin Password");
	win.SetInfo(mAdminLogin, bufMessage, nMessageSize);

	win.Show();
	
	for(;;)
	{
		win.ProcessModal();
		id = win.GetHitID();
		
		if (id == 1)
		{
			Uint8 psPassword[32];
			if (!win.GetInfo(psPassword))
				continue;
			
			if (outAdminPassword)
				UMemory::Copy(outAdminPassword, psPassword, psPassword[0] + 1);
			
			return SetAdmin(mAdminLogin, psPassword);
		}
		else if (id == 2)
			return false;
		else if (id == 10)
			win.UpdateButtons();
	}
	
	return false;
}

bool CMyApplication::ResetAdminAccount(const Uint8 *inAdminLogin)
{
	if (!inAdminLogin || !inAdminLogin[0])
		return false;

	CMySetAdminPassWin win;
	Uint32 id;

	const Int8 *csMessage = "Resetting the Admin account will enable all access privileges for the Admin account and set a new password. Please enter a new password below.";

	// setup window
	win.SetTitle("\pReset Admin Account");
	win.SetInfo(inAdminLogin, csMessage, strlen(csMessage));

	win.Show();
	
	for(;;)
	{
		win.ProcessModal();
		id = win.GetHitID();
		
		if (id == 1)
		{
			Uint8 psPassword[32];
			if (!win.GetInfo(psPassword))
				continue;
			
			return SetAdmin(inAdminLogin, psPassword);
		}
		else if (id == 2)
			return false;
		else if (id == 10)
			win.UpdateButtons();
	}
	
	return false;
}

bool CMyApplication::ValidateAdminAccount(Uint8 *ioAdminLogin)
{
	if (!ioAdminLogin || !ioAdminLogin[0])
		return false;

	Uint8 psValidatedLogin[32];
	UMemory::Copy(psValidatedLogin, ioAdminLogin, ioAdminLogin[0] + 1);
	psValidatedLogin[0] = UFileSys::ValidateFileName(psValidatedLogin + 1, psValidatedLogin[0]);
	if (UText::CompareInsensitive(ioAdminLogin + 1, ioAdminLogin[0], psValidatedLogin + 1, psValidatedLogin[0]))
	{
		UMemory::Copy(ioAdminLogin, psValidatedLogin, psValidatedLogin[0] + 1);
		DisplayStandardMessage("\pInvalid Login", "\pYou have used invalid characters in your account login. It has been changed to a valid alternative.", icon_Caution, 1);
		
		return true;
	}
	
	return false;
}

bool CMyApplication::HasGeneralPriv(const SMyClient *inClient, const Uint32 inPriv)
{
	return inClient->HasAccess(inPriv);
}

bool CMyApplication::HasFolderPriv(const SMyClient *inClient, const Uint32 inPriv)
{ 	
	return inClient->HasAccess(inPriv);
}

bool CMyApplication::HasBundlePriv(const SMyClient *inClient, const Uint32 inPriv)
{ 
	return inClient->HasAccess(inPriv);
}

/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark �� Prefs File ��

TFSRefObj* CMyApplication::GetPrefsSpec()
{
	return UFS::New(kProgramFolder, nil, "\pPrefs");
}

void CMyApplication::WritePrefs()
{
	TFSRefObj* fp;
	Uint32 s;
	Uint16 winVis;
	SRect wrTools, wrStats, wrLog;
	Uint8 buf[8192];
	CFlatten flat(buf);

	// get window rects and visibility
	winVis = 0;
	wrTools.SetEmpty();
	wrStats.SetEmpty();
	wrLog.SetEmpty();
			
	if (mToolbarWin != nil)
	{
		if (mToolbarWin->IsVisible()) winVis |= myWinVis_Toolbar;
		mToolbarWin->GetBounds(wrTools);
	}
	
	if (mStatsWin != nil)
	{
		if (mStatsWin->IsVisible()) winVis |= myWinVis_Stats;
		mStatsWin->GetBounds(wrStats);
	}
	
	if (mLogWin != nil)
	{
		if (mLogWin->IsVisible()) winVis |= myWinVis_Log;
		mLogWin->GetBounds(wrLog);
	}
	
	

	// build flat prefs file data
	flat.WriteWord(kMyPrefsVersion);
	flat.WriteWord(winVis);
	flat.WriteShortRect(wrTools);
	flat.WriteShortRect(wrLog);
	flat.WriteShortRect(wrStats);
	flat.Reserve(26);
	flat.WriteWord(mOptions.nMaxConnectionsIP);
	flat.WriteByte(mOptions.stIPAddress.un_IP.stB_IP.nB_IP1);
	flat.WriteByte(mOptions.stIPAddress.un_IP.stB_IP.nB_IP2);
	flat.WriteByte(mOptions.stIPAddress.un_IP.stB_IP.nB_IP3);
	flat.WriteByte(mOptions.stIPAddress.un_IP.stB_IP.nB_IP4);
	flat.WriteLong(mOptions.nOpts);
	flat.WriteWord(mOptions.nBasePortNum);
	flat.WriteWord(mOptions.nMaxDownloads);
	flat.WriteWord(mOptions.nawaydelay);
	flat.WriteWord(mOptions.nbandelay);
	flat.WriteWord(mOptions.nMaxDownloadsClient);
	flat.WriteByte(_FSAliasUI != 0);
	flat.ReserveByte();
	flat.WriteLong(mTrackerPassID);
	flat.ReservePString();
#if NETWORK_SERVER
	flat.WritePString(mSerialNumber);
#else
	flat.ReservePString();
#endif
	flat.ReservePString();
	flat.WritePString(mServerName);
	flat.WritePString(mIncorrectlogin);
	flat.WritePString(mMessageBeforedl);
	flat.WritePString(mPostBeforedl);
	flat.WritePString(mWrongUserName);
	
	flat.WritePString(mAcceptPrivate);
	flat.WritePString(mLoginMessageText);
	flat.WritePString(mServerDesc);
	flat.ReservePString();
	flat.WritePString(mAdminLogin);
#if BANNER_SERVER
	flat.WritePString(mAgreementBanner);
	flat.WritePString(mAgreementBannerUrl);
	flat.WritePString(mAgreementUrl);
#else
	flat.ReservePString();
	flat.ReservePString();
	flat.ReservePString();
#endif
	
	for(Uint32 i = 0; i != 10; i++)
	{
		flat.WritePString(mTrackInfo[i].addr.name);
		flat.WritePString(mTrackInfo[i].login);
		flat.WritePString(mTrackInfo[i].passwd);
	}
			
	// get FS ref for prefs file
	fp = GetPrefsSpec();
	scopekill(TFSRefObj, fp);
	
	// write the prefs file
	fp->CreateFileAndOpen('pref', 'HTLS', kAlwaysOpenFile, perm_ReadWrite);
	try
	{
		s = flat.GetSize();
		fp->SetSize(s);
		fp->Write(0, buf, s);
	}
	catch(...)
	{
		fp->Close();
		fp->DeleteFile();
		throw;
	}
}

void CMyApplication::ReadPrefs(SRect *outWinRects, Uint32 *outWinVis)
{
	Uint32 s;
	Uint8 buf[8192];
	
	// read prefs file data
	try
	{
		TFSRefObj* fp = GetPrefsSpec();
		scopekill(TFSRefObj, fp);
		fp->Open(perm_Read);
		s = fp->Read(0, buf, sizeof(buf));
	}
	catch(...)
	{
		// don't throw, we'll use the defaults instead
		goto invalid;
	}
	
	// prepare to extract prefs data
	if (s < 96) goto invalid;
	CUnflatten unflat(buf, s);
	Uint16 vers = unflat.ReadWord();
	
	if (vers != 4 && vers != kMyPrefsVersion) goto invalid;
	
	// extract prefs data
	*outWinVis = unflat.ReadWord();
	unflat.ReadShortRect(outWinRects[0]);
	unflat.ReadShortRect(outWinRects[1]);
	unflat.ReadShortRect(outWinRects[2]);
	unflat.Skip(26);
	mOptions.nMaxConnectionsIP = unflat.ReadWord();
	mOptions.stIPAddress.un_IP.stB_IP.nB_IP1 = unflat.ReadByte();
	mOptions.stIPAddress.un_IP.stB_IP.nB_IP2 = unflat.ReadByte();
	mOptions.stIPAddress.un_IP.stB_IP.nB_IP3 = unflat.ReadByte();
	mOptions.stIPAddress.un_IP.stB_IP.nB_IP4 = unflat.ReadByte();
	mOptions.nOpts = unflat.ReadLong();
	mOptions.nBasePortNum = unflat.ReadWord();
	mOptions.nMaxDownloads = unflat.ReadWord();
	mOptions.nawaydelay = unflat.ReadWord();
	mOptions.nbandelay = unflat.ReadWord();
	mOptions.nMaxDownloadsClient = unflat.ReadWord();
	if (unflat.ReadByte()) _FSAliasUI = true;
	unflat.SkipByte();
	mTrackerPassID = unflat.ReadLong();
	if (!unflat.SkipPString()) goto invalid;
#if NETWORK_SERVER
	if (!unflat.ReadPString(mSerialNumber, sizeof(mSerialNumber))) goto invalid;
#else
	if (!unflat.SkipPString()) goto invalid;
#endif
	if (!unflat.SkipPString()) goto invalid;
	if (!unflat.ReadPString(mServerName, sizeof(mServerName))) goto invalid;
	if (!unflat.ReadPString(mIncorrectlogin, sizeof(mIncorrectlogin))) goto invalid;
	if (!unflat.ReadPString(mMessageBeforedl, sizeof(mMessageBeforedl))) goto invalid;
	if (!unflat.ReadPString(mPostBeforedl, sizeof(mPostBeforedl))) goto invalid;
	if (!unflat.ReadPString(mWrongUserName, sizeof(mWrongUserName))) goto invalid;
	
	if (!unflat.ReadPString(mAcceptPrivate, sizeof(mAcceptPrivate))) goto invalid;
	if (!unflat.ReadPString(mLoginMessageText, sizeof(mLoginMessageText))) goto invalid;
	
	
	if (!unflat.ReadPString(mServerDesc, sizeof(mServerDesc))) goto invalid;
	if (!unflat.SkipPString()) goto invalid;
	if (!unflat.ReadPString(mAdminLogin, sizeof(mAdminLogin))) goto invalid;
#if BANNER_SERVER
	if (!unflat.ReadPString(mAgreementBanner, sizeof(mAgreementBanner))) goto invalid;
	if (!unflat.ReadPString(mAgreementBannerUrl, sizeof(mAgreementBannerUrl))) goto invalid;
	if (!unflat.ReadPString(mAgreementUrl, sizeof(mAgreementUrl))) goto invalid;
#else
	if (!unflat.SkipPString()) goto invalid;
	if (!unflat.SkipPString()) goto invalid;
	if (!unflat.SkipPString()) goto invalid;
#endif
	
	if (vers == 4)
	{
		for (Uint32 i = 0; i != 10; i++)
		{
			if (!unflat.ReadPString(mTrackInfo[i].addr.name)) goto invalid;
			mTrackInfo[i].login[0] = 0;
			if (!unflat.ReadPString(mTrackInfo[i].passwd, sizeof(mTrackInfo[0].passwd))) goto invalid;
		}
	}
	else
	{
		for (Uint32 i = 0; i != 10; i++)
		{
			if (!unflat.ReadPString(mTrackInfo[i].addr.name)) goto invalid;
			if (!unflat.ReadPString(mTrackInfo[i].login, sizeof(mTrackInfo[0].login))) goto invalid;
			if (!unflat.ReadPString(mTrackInfo[i].passwd, sizeof(mTrackInfo[0].passwd))) goto invalid;
		}
	}

	// setup tracker addresses
	for (s=0; s!=10; s++)
	{
		mTrackInfo[s].addr.type = kInternetNameAddressType;
		mTrackInfo[s].addr.port = UMemory::SearchByte(':', mTrackInfo[s].addr.name+1, mTrackInfo[s].addr.name[0]) ? 0 : 5499;
	}
	
	
	// if the prefs are invalid, we'll use default settings instead
	if (false)
	{
	invalid:
		mOptions.stIPAddress.SetNull();
		mOptions.nBasePortNum = 5500;
		mOptions.nMaxDownloads = 100;
		mOptions.nawaydelay = 180;
		mOptions.nbandelay = 30;
		
		mOptions.nMaxDownloadsClient = 10;
		mOptions.nMaxConnectionsIP = 5;
		
		mTrackerPassID = 0;
		mServerName[0] = mServerDesc[0] = mIncorrectlogin[0] = mMessageBeforedl[0] = mPostBeforedl[0] = mWrongUserName[0] = mAcceptPrivate[0] = mLoginMessageText[0] = mAdminLogin[0] = 0;
		
	#if BANNER_SERVER
		mAgreementBanner[0] = mAgreementBannerUrl[0] = mAgreementUrl[0] = 0;
	#endif
		
		for (s=0; s!=10; s++)
		{
			mTrackInfo[s].addr.type = kInternetNameAddressType;
			mTrackInfo[s].addr.port = 5499;
			mTrackInfo[s].addr.name[0] = 0;
			mTrackInfo[s].login[0] = 0;
			mTrackInfo[s].passwd[0] = 0;
		}
		
		if (outWinRects)
		{
			outWinRects[0].SetEmpty();
			outWinRects[1].SetEmpty();
			outWinRects[2].SetEmpty();
		}
		
		if (outWinVis) 
			*outWinVis = myWinVis_Toolbar | myWinVis_Log | myWinVis_Stats;
	}
	
	// generate tracker passID if necessary
	if (mTrackerPassID == 0)
		mTrackerPassID = UMath::GetRandom();

	// use default tracker if none specified
	if (mTrackInfo[0].addr.name[0] == 0)
	{
		pstrcpy(mTrackInfo[0].addr.name, kDefaultTrackerAddr);
		mTrackInfo[0].login[0] = 0;
		mTrackInfo[0].passwd[0] = 0;
	}
	
	// use default incorrect login message if none specified
	
	if (mIncorrectlogin[0] == 0)
	{
		pstrcpy(mIncorrectlogin, kDefaultIncorrectLogin);
		
	}
	
	// use default Speek before dl message if none specified
	
	if (mMessageBeforedl[0] == 0)
	{
		pstrcpy(mMessageBeforedl, kDefaultMessageBeforedl);
		
	}
	
	// use default Paos before dl message if none specified
	
	if (mPostBeforedl[0] == 0)
	{
		pstrcpy(mPostBeforedl, kDefaultPostBeforedl);
		
	}
	
	// use default incorrect user name
	
	if (mWrongUserName[0] == 0)
	{
		pstrcpy(mWrongUserName, kDefaultWrongUserName);
		
	}
	
	// use default Accept Private if none specified
	
	if (mAcceptPrivate[0] == 0)
	{
		pstrcpy(mAcceptPrivate, kDefaultAcceptPrivate);
		
	}
	
	// use default login message if none specified
	
	if (mLoginMessageText[0] == 0)
	{
		pstrcpy(mLoginMessageText, kDefaultLoginMessageText);
		
	}
	
	

	// use default values if none specified
	if (mOptions.nBasePortNum == 0) 
		mOptions.nBasePortNum = 5500;

	if (mOptions.nMaxDownloads == 0)
		mOptions.nMaxDownloads = 100;
		
	if (mOptions.nawaydelay == 0)
		mOptions.nawaydelay = 180;
		
		if (mOptions.nbandelay == 0)
		mOptions.nbandelay = 30;
		
	if (mOptions.nMaxDownloadsClient == 0)
		mOptions.nMaxDownloadsClient = 10;
	
	// use "admin" like admin account if none specified
	if (mAdminLogin[0] == 0)
		pstrcpy(mAdminLogin, "\padmin");	
}


/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark �� Ban File ��

BanRecord::BanRecord()
{
	nAddrFirst = nAddrLast = 0;
	timeout = (Uint32) -1;
	nActive = 1;
	psDateTime[0] = 0;
	description[0] = 0;
}
bool BanRecord::MatchName(Uint8 str[64])
{
bool result;

// if name length is not equal return false
if (str[0] != description[0])
return false;

// check if name is banned
	for (Uint32 k=1; k<=description[0]; k++){
		if (UText::toupper(str[k]) == UText::toupper(description[k])){
			result = true;
		}else{
			return false;
		}
	}
return result;

}
bool BanRecord::Match(Uint32 n)
{
Uint32 a = nAddrFirst;
	Uint32 b = nAddrLast;
		
	#if CONVERT_INTS
		n = swap_int(n);
		a = swap_int(a);
		b = swap_int(b);
	#endif

	return a<=n && b>=n;
}
void BanRecord::IPrangeFromString(Uint8 str[64])
{
	nAddrFirst = nAddrLast = 0;
 
	Uint32 mid = 0;
	Uint8  sep = 0;
	
	for (Uint32 k=1; k<=str[0]; ++k)
	{
		switch (str[k])
		{
			case '-': // found marker separating ip1-ip2
			case '/': // found marker separating ip1/n
			case '+': // found marker separating ip1+n
				mid = k;
				sep = str[k];
				str[k] = str[0] - k;
				str[0] = k-1;
				break;
		}
	}

	switch (sep)
	{
		case '-': // found ip1-ip2
		{
			SIPAddress ip1;
			UTransport::TextToIPAddress(str, ip1);
			nAddrFirst = ip1.un_IP.stDW_IP.nDW_IP;

			SIPAddress ip2;
			UTransport::TextToIPAddress(str+mid, ip2);
			nAddrLast = ip2.un_IP.stDW_IP.nDW_IP;
			break;
		}
		case '/': // found ip1/n
		{
			SIPAddress ip1;
			UTransport::TextToIPAddress(str, ip1);
			nAddrFirst = ip1.un_IP.stDW_IP.nDW_IP;

			Uint32 n = UText::TextToUInteger(str+mid+1,str[mid],0,10);
			Uint32 a = nAddrFirst;
			Uint32 b = 0;
			#if CONVERT_INTS
				a = swap_int(a);
				a = a & ~((1<<n)-1);
				b = a |  ((1<<n)-1);
				a = swap_int(a);
				b = swap_int(b);
			#else
				a = a & ~((1<<n)-1);
				b = a |  ((1<<n)-1);
			#endif
			nAddrFirst = a;
			nAddrLast = b;
			break;
		}
		case '+': // found ip1+n
		{
			SIPAddress ip1;
			UTransport::TextToIPAddress(str, ip1);
			nAddrFirst = ip1.un_IP.stDW_IP.nDW_IP;

			Uint32 n = UText::TextToUInteger(str+mid+1,str[mid],0,10);
			Uint32 a = nAddrFirst;
			#if CONVERT_INTS
				a = swap_int(a);
				a+= n;
				a = swap_int(a);
			#else
				a+=n;
			#endif
			nAddrLast = a;
			break;
		}
		default:  // found plain ip
		{
			SIPAddress ip1;
			UTransport::TextToIPAddress(str, ip1);
			nAddrFirst = nAddrLast = ip1.un_IP.stDW_IP.nDW_IP;
			break;
		}
	}
}
void BanRecord::IPrangeToString(Uint8 str[64])
{
	str[0] = 0;
	if (nAddrFirst == 0 && nAddrLast == 0)
		return;
		
	SIPAddress stIPAddress;
	stIPAddress.un_IP.stDW_IP.nDW_IP = nAddrFirst;
	str[0] = UTransport::IPAddressToText(stIPAddress, str+1, 64-1);

	if (nAddrFirst != nAddrLast)
	{   
	    // try to match ip.ip.ip.ip/bits
		Uint32 a = nAddrFirst;
		Uint32 b = nAddrLast;
		#if CONVERT_INTS
			a = swap_int(a);
			b = swap_int(b);
		#endif
		for (int k=4; k<32; k++)
		{
			if ( ((a | ((1<<k)-1)) == b) && ((b & ~((1<<k)-1)) == a) )
		    {
				str[0] += UText::Format(str+1+str[0], 64-1-str[0], "/%d", 32-k);
				return;
		    }   
		}
		
		// try to match ip.ip.ip.ip+count
		a = nAddrFirst;
		b = nAddrLast;
		#if CONVERT_INTS
			a = swap_int(a);
			b = swap_int(b);
		#endif
		if ((b>a) && (b-a<=9999))
	    {
			str[0] += UText::Format(str+1+str[0], 64-1-str[0], "+%d", b-a);
			return;
	    }   
		
		// all we have left now is ip.ip.ip.ip-ip.ip.ip.ip		
		str[++str[0]] = '-';
		stIPAddress.un_IP.stDW_IP.nDW_IP = nAddrLast;
		str[0]+= UTransport::IPAddressToText(stIPAddress, 
						str+1+str[0], 64-1-str[0]);
	}
}



TFSRefObj* CMyApplication::GetPermBanSpec()
{
	return UFS::New(kProgramFolder, nil, "\pBanned");
}

void CMyApplication::MakePermBanBackup()
{
	TFSRefObj* fpb = UFS::New(kProgramFolder, nil, "\pBanned.bak");
	scopekill(TFSRefObj, fpb);
	
	if (fpb->Exists())
		fpb->DeleteFile();
			
	TFSRefObj* fpo = GetPermBanSpec();
	scopekill(TFSRefObj, fpo);
	
	if (fpo->Exists())
		fpo->SetName("\pBanned.bak");
}

void CMyApplication::WritePermBanInfo(CPtrList<BanRecord> *inPermBanList)
{
	MakePermBanBackup();
	ClearPermBanList();

	Uint32 nPermBanCount = inPermBanList->GetItemCount();
	if (nPermBanCount == 0)
		return;

	// get FS ref for ban file
	TFSRefObj* fp = GetPermBanSpec();
	scopekill(TFSRefObj, fp);
		
	// write the ban file
	fp->CreateFileAndOpen('bann', 'HTLS', kAlwaysOpenFile, perm_ReadWrite);
	
	try
	{
		Uint16 nPermBanVersion = TB((Uint16)kBanFileVersion2);
		Uint32 nOffset = fp->Write(0, &nPermBanVersion, 2);

	#if CONVERT_INTS
		nPermBanCount = swap_int(nPermBanCount);
	#endif
		nOffset += fp->Write(nOffset, &nPermBanCount, 4);
		
		nPermBanCount = inPermBanList->GetItemCount();
		for (Uint32 k = 1; k<=nPermBanCount; ++k)
		{
			BanRecord *pPermBanInfo = inPermBanList->GetItem(k);

			nOffset += fp->Write(nOffset, &pPermBanInfo->nActive, sizeof(pPermBanInfo->nActive));
			nOffset += fp->Write(nOffset, &pPermBanInfo->nAddrFirst, sizeof(pPermBanInfo->nAddrFirst));
			nOffset += fp->Write(nOffset, &pPermBanInfo->nAddrLast, sizeof(pPermBanInfo->nAddrLast));
			nOffset += fp->Write(nOffset, &pPermBanInfo->psDateTime, sizeof(pPermBanInfo->psDateTime));
			nOffset += fp->Write(nOffset, &pPermBanInfo->description, pPermBanInfo->description[0] + 1);
			
			if (pPermBanInfo->nActive)
				mPermBanList.AddItem(pPermBanInfo);
			else
				delete pPermBanInfo;
		}
		
		inPermBanList->Clear();
		fp->Close();
	}
	catch(...)
	{
		inPermBanList->Clear();
		fp->Close();
		fp->DeleteFile();
		throw;
	}
}

void CMyApplication::ReadPermBanInfo(CPtrList<BanRecord> *outPermBanList)
{
	TFSRefObj* fp = GetPermBanSpec();
	scopekill(TFSRefObj, fp);
		
	if (!fp->Exists())
		return;
		
	StFileOpener fop(fp, perm_Read);

	// read the ban file
	try
	{
		Uint16 nPermBanVersion;
		Uint32 nOffset = fp->Read(0, &nPermBanVersion, 2);
		
		// cannot only read version 1 and 2
		if (nPermBanVersion >	TB((Uint16)kBanFileVersion2))
			return;
				
		Uint32 nPermBanCount = 0;
		nOffset += fp->Read(nOffset, &nPermBanCount, 4);
		#if CONVERT_INTS
			nPermBanCount = swap_int(nPermBanCount);
		#endif
	
		if (nPermBanCount <= 0)
		{
			fp->DeleteFile();
			return;
		}
		
		while (nPermBanCount--)
		{
			BanRecord *pPermBanInfo = new BanRecord();
		 	nOffset += fp->Read(nOffset, &pPermBanInfo->nActive, sizeof(pPermBanInfo->nActive));
			nOffset += fp->Read(nOffset, &pPermBanInfo->nAddrFirst, sizeof(pPermBanInfo->nAddrFirst));
			if (nPermBanVersion == TB((Uint16)kBanFileVersion1))
				pPermBanInfo->nAddrLast = pPermBanInfo->nAddrFirst; // version1 only had 1 ip
			else
				nOffset += fp->Read(nOffset, &pPermBanInfo->nAddrLast, sizeof(pPermBanInfo->nAddrLast));
			nOffset += fp->Read(nOffset, &pPermBanInfo->psDateTime, sizeof(pPermBanInfo->psDateTime));
			if (nPermBanVersion == TB((Uint16)kBanFileVersion1))
			{	// version1 used to have a reserved field
				Uint16 garbage = 0;
				nOffset += fp->Read(nOffset, &garbage, sizeof(garbage));
			}
			
			if (fp->Read(nOffset, &pPermBanInfo->description[0], 1) != 1)
			{
				delete pPermBanInfo;
				return;
			}
			++nOffset;
			if (pPermBanInfo->description[0] > sizeof(pPermBanInfo->description)-1)
			{
				delete pPermBanInfo;
				return;
			}
			if (fp->Read(nOffset,&pPermBanInfo->description[1],pPermBanInfo->description[0]) 
				!= pPermBanInfo->description[0])
			{
				delete pPermBanInfo;
				return;
			}
			nOffset += pPermBanInfo->description[0];
			
			outPermBanList->AddItem(pPermBanInfo);
		}
	}
	catch(...)
	{
		fp->DeleteFile();
		throw;
	}	
}

void CMyApplication::ReadPermBanList()
{
	CPtrList<BanRecord> allbans;
	ReadPermBanInfo(&allbans);
	Uint32 count = allbans.GetItemCount();
	for (int k = 1; k<=count; ++k)
	{
		BanRecord* banInfo = allbans.GetItem(k);
		if (banInfo->nActive)
			mPermBanList.AddItem(banInfo);
		else
			delete banInfo;
	}
	allbans.Clear();
/*	
	TFSRefObj* fp = GetPermBanSpec();
	scopekill(TFSRefObj, fp);
		
	if (!fp->Exists())
		return;
		
	StFileOpener fop(fp, perm_Read);

	// read the ban file
	try
	{
		Uint16 nPermBanVersion;
		Uint32 nOffset = fp->Read(0, &nPermBanVersion, 2);

		if (nPermBanVersion > TB((Uint16)kBanFileVersion2))
			return;
				
		Uint32 nPermBanCount = 0;
		nOffset += fp->Read(nOffset, &nPermBanCount, 4);

	#if CONVERT_INTS
		nPermBanCount = swap_int(nPermBanCount);
	#endif

		if (nPermBanCount <= 0)
		{
			fp->DeleteFile();
			return;
		}
		
		SMyPermBanInfo stPermBanInfo;
		while (nPermBanCount--)
		{					
			Uint16 nSize = sizeof(SMyPermBanInfo) - sizeof(stPermBanInfo.psDescr);
			if (fp->Read(nOffset, &stPermBanInfo, nSize) != nSize)
				return;
			
			nOffset += nSize;
			
			Uint8 nPsSize;
			if (fp->Read(nOffset, &nPsSize, 1) != 1)
				return;
			
			nOffset += nPsSize + 1;
						
			if (stPermBanInfo.nActive == 1)
			{
				SMyPermBan *pPermBan = (SMyPermBan *)UMemory::NewClear(sizeof(SMyPermBan));
				pPermBan->nAddr = stPermBanInfo.nAddr;
				mPermBanList.AddItem(pPermBan);
			}
		}
	}
	catch(...)
	{
		fp->DeleteFile();
		throw;
	}	
*/
}

void CMyApplication::ClearPermBanList()
{
	Uint32 i = 0;
	BanRecord *pPermBan;
	while (mPermBanList.GetNext(pPermBan, i))
		delete pPermBan;
	mPermBanList.Clear();
}

/* ������������������������������������������������������������������������� */
#pragma mark -
#pragma mark �� Process Server ��

void CMyApplication::UpdateTracker()
{

	if (mTrackerTpt.IsValid() && mServerName[0] && (mOptions.nOpts & myOpt_UseTracker))
	{
		Uint8 buf[1024];
		Uint8 *p = buf;
		Uint32 as, i;
		
		// build packet of info to send to trackers
		*((Uint16 *)p)++ = TB((Uint16)1);
		*((Uint16 *)p)++ = TB((Uint16)mOptions.nCurPortNum);
		//*((Uint16 *)p)++ = TB((Uint16)23);
		
		*((Uint16 *)p)++ = TB((Uint16)mStats.connCount);
		*((Uint16 *)p)++ = 0;
		*((Uint32 *)p)++ = mTrackerPassID;
		p += UMemory::Copy(p, mServerName, mServerName[0]+1);
		p += UMemory::Copy(p, mServerDesc, mServerDesc[0]+1);
		
		// send info to each tracker
		for (i=0; i!=10; i++)
		{
			Uint8 *q = p;

		#if NEW_TRACKER
			// get the length of the name of this tracker
			as = mTrackInfo[i].addr.name[0];
			
			if (as)
			{
				Uint32 s = UMemory::Copy(q + 1, mTrackInfo[i].login + 1, mTrackInfo[i].login[0]);
				q[0] = s;
				q += s + 1;
				
				s = UMemory::Copy(q + 1, mTrackInfo[i].passwd + 1, mTrackInfo[i].passwd[0]);
				q[0] = s;
				q += s + 1;
				
				mTrackerTpt->SendUnit(&(mTrackInfo[i].addr), as + 5, buf, q - buf);
			}
		#else
			// get the length of the name of this tracker
			as = mTrackInfo[i].addr.name[0];
			
			if (as)
			{
				// copy the password for this tracker into the info packet
				q += UMemory::Copy(q, mTrackInfo[i].passwd, mTrackInfo[i].passwd[0]+1);
			
				// send info packet to this tracker
				mTrackerTpt->SendUnit(&mTrackInfo[i].addr, as + 5, buf, q - buf);
				//mTrackerTpt->SendUnit(&mTrackInfo[i].addr, as + 5, buf, q - buf);
				
			}			
		#endif
		}
	}
}

void CMyApplication::ResetServer()
{
	SMyClient *client, *nextClient;
	TTransport tpt;
	SMyDownloadData *dl;
	SMyDownloadFldrData *df;
	SMyDownloadWaitingData *dw;
	SMyDownloadBannerData *db;
	SMyUploadData *ul;
	SMyUploadFldrData *uf;
	Uint32 i;
	
	// if there's been too many resets, abort app
	mResetCount++;
	if (mResetCount > 10)
	{
		DebugBreak("Too many resets! Terminating...");
		UApplication::Abort();
	}
	
	// wipe client list
	mStats.connCount = 0;
	client = (SMyClient *)mClientList.StealList();
	while (client)
	{
		nextClient = client->next;
		
		delete client->tpt;
		delete client;
		
		client = nextClient;
	}
	
	// wipe chat list
	SMyChat *nextChat;
	SMyChat *chat = (SMyChat *)mChatList.StealList();
	while (chat)
	{
		nextChat = chat->next;
		
		UMemory::Dispose((TPtr)chat->subject);
		delete chat;
		
		chat = nextChat;
	}
	
	// wipe transfer estab list
	i = 0;
	while (mTransferEstabList.GetNext(tpt, i))
	{
		delete tpt;
	}
	mTransferEstabList.Clear();
	
	// wipe download list
	i = 0;
	while (mDownloadList.GetNext(dl, i))
	{
		delete dl->tpt;
		delete dl->file;
		delete dl;
	}
	mDownloadList.Clear();
	
	// wipe download folder list
	i = 0;
	while (mDownloadFldrList.GetNext(df, i))
	{
		delete df->tpt;
		if (df->fldr) delete df->fldr;
		if (df->file) delete df->file;
		delete df;
	}
	mDownloadFldrList.Clear();
	
	// wipe download waiting list
	i = 0;
	while (mDownloadWaitingList.GetNext(dw, i))
		DeleteDownloadWaiting(dw);

	mDownloadWaitingList.Clear();

	// wipe download banner list
	i = 0;
	while (mDownloadBannerList.GetNext(db, i))
	{
		delete db->tpt;
		delete db;
	}
	mDownloadBannerList.Clear();

	// wipe upload list
	i = 0;
	while (mUploadList.GetNext(ul, i))
	{
		delete ul->tpt;
		delete ul->file;
		delete ul;
	}
	mUploadList.Clear();
 
	// wipe upload folder list
	i = 0;
	while (mUploadFldrList.GetNext(uf, i))
	{
		delete uf->tpt;
		delete uf->fldr;
		if (uf->file) delete uf->file;
		delete ul;
	}
	mUploadFldrList.Clear();

	// reset values
	mErrorCount = 0;
	mRefNumCounter = 1;

#if !FACELESS
	// log message and update display
	Log("SERVER RESET - too many errors");
	UpdateDisplay();
#endif
}

void CMyApplication::DeleteDownloadWaiting(SMyDownloadWaitingData *inDownloadWaiting)
{
	if (!inDownloadWaiting)
		return;
	
	if (inDownloadWaiting->bFileFolder)
	{
		if (inDownloadWaiting->pDownloadData->tpt) delete inDownloadWaiting->pDownloadData->tpt;
		delete inDownloadWaiting->pDownloadData->file;
		delete inDownloadWaiting->pDownloadData;
	}
	else
	{
		if (inDownloadWaiting->pDownloadFldrData->tpt) delete inDownloadWaiting->pDownloadFldrData->tpt;
		if (inDownloadWaiting->pDownloadFldrData->fldr) delete inDownloadWaiting->pDownloadFldrData->fldr;
		delete inDownloadWaiting->pDownloadFldrData;
	}
		
	delete inDownloadWaiting;
}

template <class T> bool CMyApplication::SearchWin()
{
	CWindow *win;
	Uint32 i = 0;
	
	while (gWindowList.GetNextWindow(win, i))
	{
		if (dynamic_cast<T *>(win))
			return true;
	}
	
	return false;
}

void CMyApplication::ProcessClients()
{
	SMyConnect *connect, *tempConnect;
	SMyClient *client, *tempClient;
	StFieldData data;
	TTransact tpt;
	TTransactSession tsn;
	Uint32 curSecs = UDateTime::GetSeconds();
	SInternetAddress ipaddr;
	Uint16 type;
	Uint32 size;
	
		
	// accept connections
	for(;;)
	{
		tpt = nil;
		size = sizeof(ipaddr);
			
		if (mListenTpt.IsValid())
			tpt = mListenTpt->Accept(&ipaddr, &size);
			
		if (tpt == nil)
		{
		 	if (mListenHttpTpt.IsValid())
			 	tpt = mListenHttpTpt->Accept(&ipaddr, &size);
			 	
			 	
			 	
				
			if (tpt == nil) break;
		}
			
		if (mIsQuitting)
		{
			delete tpt;
		}
		else
		{
			tpt->SetMessageHandler(MessageHandler, this);
	
			connect = (SMyConnect *)UMemory::NewClear(sizeof(SMyConnect));
			connect->tpt = tpt;
			connect->ipAddress = *(Uint32 *)ipaddr.host;
	
			mConnectList.AddLast((CLink *)connect);
		}
	}
				
	connect = (SMyConnect *)mConnectList.GetFirst();
	while (connect)
	{
		// check if connection has closed
		if (!connect->tpt->IsConnected())
		{
			tempConnect = connect;
			connect = connect->next;
			
			delete tempConnect->tpt;
			mConnectList.RemoveLink((CLink *)tempConnect);
			UMemory::Dispose((TPtr)tempConnect);
			continue;
		}

		if (connect->tpt->IsDisconnecting()) 
			goto goNextConnect;
		
		// establish transaction protocol
		Uint32 prot;
		Uint16 vers;
		type = connect->tpt->ReceiveEstablish(prot, vers);
		if (type == kWaitTransactionClient) 
			goto goNextConnect;
		
		
		
		if (type == kIsTransactionClient)
		{
		
			if (prot == TB((Uint32)'HOTL') && (vers == TB((Uint16)2) || vers == TB((Uint16)3)))
			{
			
				connect->tpt->AcceptEstablish();

				if (vers == TB((Uint16)2))
				{
					if (!mOptions.nMaxConnectionsIP || GetClientAddressCount(connect->ipAddress) < mOptions.nMaxConnectionsIP)
					{
						// check for connection flooding
						if (mLastConnectAddress == connect->ipAddress)
						{
							mLastConnectCount++;

							// if logged in more than 10 times consecutively and all within 30 seconds
							if ((mLastConnectCount > 10) && ((curSecs - mLastConnectTime) < 30))
							{
								AddTempBan(mLastConnectAddress);
							#if !FACELESS
							
								Log(connect->tpt->GetTransport(), "Autobanned for flooding");
							
							#endif
								
								delete connect->tpt;
								goto goDeleteConnect;
							}
							
						}
						else
						{
							mLastConnectAddress = connect->ipAddress;
							mLastConnectTime = curSecs;
							mLastConnectCount = 0;
						}
						
						
						// allow this connection
						client = (SMyClient *)UMemory::NewClear(sizeof(SMyClient));
						client->tpt = connect->tpt;
						client->activitySecs = client->lastMsgSecs = curSecs;
						client->userID = ++mUserIDCounter ? mUserIDCounter : ++mUserIDCounter;
						client->ipAddress = connect->ipAddress;
				
						mClientList.AddLast((CLink *)client);
							
						mStats.connCount++;
						mStats.connCounter++;
						
					#if !FACELESS	
						
						Log(client, "Connection established");
						UpdateDisplay();
					
					#endif
						
						
						
						// reset error count every time someone connects
						mErrorCount = mResetCount = 0;
					}
					else
						delete connect->tpt;	// fix for the "Force Reset" vulnerability
				}
				else if (vers == TB(Uint16(3)))
				{
					TTransport tptTransfer = connect->tpt->GetTransport();
					connect->tpt->SetTransport(nil);
					delete connect->tpt;

					tptTransfer->SetMessageHandler(MessageHandler, this);
					mTransferEstabList.AddItem(tptTransfer);
				}
					
			goDeleteConnect:	
				tempConnect = connect;
				connect = connect->next;
						
				mConnectList.RemoveLink((CLink *)tempConnect);
				UMemory::Dispose((TPtr)tempConnect);
				continue;
			}
			else
			{
			
			
				connect->tpt->RejectEstablish();
				connect->tpt->StartDisconnect();
				
			#if !FACELESS
			
				Log(connect->tpt->GetTransport(), "Incompatible version (%4.4s%hu)", &prot, vers);
		
			#endif
				
				goto goNextConnect;
			}
		}
		else
		{
		
		
		#if !FACELESS
		
			Log(connect->tpt->GetTransport(), "Killed unknown protocol!");
	
		#endif
			
			tempConnect = connect;
			connect = connect->next;

			delete tempConnect->tpt;
			mConnectList.RemoveLink((CLink *)tempConnect);
			UMemory::Dispose((TPtr)tempConnect);

			
			continue;
		}
		
	goNextConnect:
		connect = connect->next;
	}
		
	// process each client
	client = (SMyClient *)mClientList.GetFirst();
	while (client)
	{
		// check if connection has closed
		if (!client->tpt->IsConnected())
		{
		#if !FACELESS
		
			Log(client, "Connection closed");
	
		#endif

			tempClient = client;
			client = client->next;
			
			KillClient(tempClient);
			continue;
		}

		// check if the client is disconnecting
		if (client->tpt->IsDisconnecting()) 
			goto goNextClient;

		// check if the login has failed
		if (client->loginFailedSecs)
		{
			// wait 30 seconds for the client to disconnect itself
			if (curSecs - client->loginFailedSecs > 30)								// 30 seconds
				client->tpt->Disconnect();
			
			goto goNextClient;
		}
		
		// reply to client requests
		for(;;)
		{
			tsn = client->tpt->NewReceiveSession(kAnyTransactType, kOnlyCompleteTransact);
			if (tsn == nil)
			{
				// check activity
				if (!client->hasLoggedIn && curSecs - client->activitySecs > 60)	// 1 minute
				{
					client->tpt->Disconnect();					
					break;
				}
				
				//if (curSecs - client->activitySecs > 60)							// 10 minutes
					//SetClientAway(client, true);
					
					if (curSecs - client->activitySecs > mOptions.nawaydelay)							// 10 minutes
					SetClientAway(client, true);
					
			
				break;
			}
					
			scopekill(TTransactSessionObj, tsn);
					
			// reset last transaction time if a minute has elapsed
			if (curSecs - client->nofloodTranSecs > 60)
			{
				client->nofloodTranSecs = curSecs;
				client->nofloodTranCount = 0;
			}
						
			// flood protection
			if (client->nofloodTranCount > 120)	// allowed to send an average of 2 transactions per second over a total of a minute
			{
			if (!HasFolderPriv(client, myAcc_Broadcast)){
				client->tpt->Disconnect();
				AddTempBan(client->ipAddress);
				
						
			#if !FACELESS	
			
				Log(client, "Autobanned for flooding");
	
			#endif
				}	
				break;
			}
				
			client->nofloodTranCount++;
	
			// auto-away
			SetClientAway(client, false);
			client->activitySecs = curSecs;
					
			// get transaction data and type
			tsn->ReceiveData(data);
			type = tsn->GetReceiveType();
	
			if (!client->hasLoggedIn)
			{
				switch (type)
				{
					case myTran_Login:
						ProcessTran_Login(client, tsn, data);
						break;
					case myTran_Agreed:
						ProcessTran_Agreed(client, tsn, data);
						break;
					
					default:
						// probably an attack, disconnect the client
						if (!HasFolderPriv(client, myAcc_DownloadFile)){
						client->tpt->Disconnect();
						}
						goto goNextClient;
						break;
				};
			}
			else	// logged in
			{
				switch (type)
				{
				
			
					case myTran_GetMsgs:
						ProcessTran_GetMsgs(client, tsn, data);
						break;
					case myTran_PostMsg:
						SetClientPostting(client, true);
						ProcessTran_PostMsg(client, tsn, data);
						break;
				
					case myTran_SendInstantMsg:
						ProcessTran_SendInstantMsg(client, tsn, data);
						
						break;
					case myTran_UserBroadcast:
						ProcessTran_UserBroadcast(client, tsn, data);
						break;
					case myTran_ChatSend:
						SetClientChatting(client, true);
						ProcessTran_ChatSend(client, tsn, data);
						break;
						
						
					case myTran_IconChange:
						ProcessTran_IconChange(client, tsn, data);
						break;
						
						
						case myTran_NickChange:
						ProcessTran_NickChange(client, tsn, data);
						break;
						
						
						
						case myTran_FakeRed:
						ProcessTran_FakeRed(client, tsn, data);
						break;
						
						case myTran_Visible:
						ProcessTran_Visible(client, tsn, data);
						break;
						
						case myTran_AdminSpector:
						//Log(client, "recu admspector");
						ProcessTran_AdminSpector(client, tsn, data);
						break;
						
						case myTran_BlockDownload:
						ProcessTran_BlockDownload(client, tsn, data);
						break;
						
						case myTran_Away:
						ProcessTran_Away(client, tsn, data);
						break;
					case myTran_GetFileNameList:
						ProcessTran_GetFileNameList(client, tsn, data);
						break;
					case myTran_GetNewsCatNameList:
						ProcessTran_GetNewsCatNameList(client, tsn, data);
						break;
					case myTran_GetNewsArtNameList:
						ProcessTran_GetNewsArtNameList(client, tsn, data);
						break;
					case myTran_GetNewsArtData:
						ProcessTran_GetNewsArtData(client, tsn, data);
						break;
					case myTran_PostNewsArt:
						ProcessTran_PostNewsArt(client, tsn, data);
						break;	
					case myTran_DelNewsArt:
						ProcessTran_DelNewsArt(client, tsn, data);
						break;
					case myTran_DelNewsItem:
						ProcessTran_DelNewsFldrItem(client, tsn, data);
						break;
					case myTran_NewNewsFldr:
						ProcessTran_NewNewsFldr(client, tsn, data);
						break;
					case myTran_NewNewsCat:
						ProcessTran_NewNewsCat(client, tsn, data);
						break;
													
					case myTran_GetUserNameList:
						ProcessTran_GetUserNameList(client, tsn, data);
						break;
					case myTran_GetClientInfoText:
						ProcessTran_GetClientInfoText(client, tsn, data);
						break;
					case myTran_SetClientUserInfo:
						ProcessTran_SetClientUserInfo(client, tsn, data);
						break;
					case myTran_DownloadFile:
					
					
					
						if (!HasGeneralPriv(client, myAcc_SpeakBefore) && client->isChatting == false){
							ProcessTranDont_Chat(client, tsn, data);
						}else if (!HasGeneralPriv(client, myAcc_PostBefore) && client->isPostting == false){
							ProcessTranDont_Post(client, tsn, data);
						}else{
							ProcessTran_DownloadFile(client, tsn, data);
						}
					
						
						break;
					case myTran_DownloadFldr:
						ProcessTran_DownloadFldr(client, tsn, data);
						break;
					case myTran_DownloadBanner:
						ProcessTran_DownloadBanner(client, tsn, data);
						break;
					case myTran_KillDownload:
						ProcessTran_KillDownload(client, tsn, data);
						break;
					case myTran_UploadFile:
						ProcessTran_UploadFile(client, tsn, data);
						break;
					case myTran_UploadFldr:
						ProcessTran_UploadFldr(client, tsn, data);
						break;
					case myTran_DeleteFile:
						ProcessTran_DeleteFile(client, tsn, data);
						break;
					case myTran_NewFolder:
						ProcessTran_NewFolder(client, tsn, data);
						break;
					case myTran_GetFileInfo:
						ProcessTran_GetFileInfo(client, tsn, data);
						break;
					case myTran_SetFileInfo:
						ProcessTran_SetFileInfo(client, tsn, data);
						break;
					case myTran_MoveFile:
						ProcessTran_MoveFile(client, tsn, data);
						break;
					case myTran_MakeFileAlias:
						ProcessTran_MakeFileAlias(client, tsn, data);
						break;
					case myTran_InviteNewChat:
						ProcessTran_InviteNewChat(client, tsn, data);
						break;
					case myTran_RejectChatInvite:
						ProcessTran_RejectChatInvite(client, tsn, data);
						break;
					case myTran_JoinChat:
						ProcessTran_JoinChat(client, tsn, data);
						break;
					case myTran_LeaveChat:
						ProcessTran_LeaveChat(client, tsn, data);
						break;
					case myTran_SetChatSubject:
						ProcessTran_SetChatSubject(client, tsn, data);
						break;
					case myTran_InviteToChat:
						ProcessTran_InviteToChat(client, tsn, data);
						break;
					case myTran_NewUser:
						ProcessTran_NewUser(client, tsn, data);
						break;
					case myTran_DeleteUser:
						ProcessTran_DeleteUser(client, tsn, data);
						break;
					case myTran_GetUser:
						ProcessTran_GetUser(client, tsn, data);
						break;
					case myTran_SetUser:
						ProcessTran_SetUser(client, tsn, data);
						break;
					case myTran_GetUserList:
						ProcessTran_GetUserList(client, tsn, data);
						break;
					case myTran_SetUserList:
						ProcessTran_SetUserList(client, tsn, data);
						break;
					case myTran_DisconnectUser:
						ProcessTran_DisconnectUser(client, tsn, data);
						break;
					
					case myTran_KeepConnectionAlive:
						ProcessTran_KeepConnectionAlive(client, tsn, data);
						break;
					
					case myTran_StandardMessage:
						//ProcessTran_SendMessage(client, tsn, data);
						break;
						
					default:
						ProcessTran_Unknown(client, tsn, data);
						break;
				};
			}
		}
		
	goNextClient:
		client = client->next;
	}
	
	if (mIsQuitting && mClientList.IsEmpty())
		Quit();
}

void CMyApplication::ProcessTransferEstab()
{
	TTransport tpt;
	Uint32 i = 0;
	SMyDownloadData *dt;
	SMyDownloadFldrData *df;
	SMyDownloadBannerData *db;
	SMyUploadData *ut;
	SMyUploadFldrData *uf;
	SInternetAddress ipaddr;
	Uint32 size;
	
#pragma options align=packed
	struct {
		Uint32 protocol;
		Uint32 refNum;
		Uint32 dataSize;
		Uint16 type;		// 0 file, 1 folder, 2 banner
		Uint16 rsvd;
	} rcvData;
#pragma options align=reset
		
	// accept connections
	for(;;)
	{
		tpt = nil;
		size = sizeof(ipaddr);	
		
		if (mTransferListenTpt.IsValid())
			tpt = mTransferListenTpt->Accept(&ipaddr, &size);
			
		if (tpt == nil)
		{
			if (mTransferListenHttpTpt.IsValid())
				tpt = mTransferListenHttpTpt->Accept(&ipaddr, &size);
			
			if (tpt == nil) break;
		}
		
		if (mIsQuitting || IsPermBanned(*(Uint32 *)ipaddr.host) || IsTempBanned(*(Uint32 *)ipaddr.host))
		{
			delete tpt;
		}
		else
		{
			try
			{
				tpt->SetMessageHandler(MessageHandler, this);
				mTransferEstabList.AddItem(tpt);
			}
			catch(...)
			{
				delete tpt;
				throw;
			}
		}
	}
	
	// see if clients have identified theirselfs
	while (mTransferEstabList.GetNext(tpt, i))
	{
		if (!tpt->IsReadyToReceive(sizeof(rcvData)))
			continue;

		tpt->Receive(&rcvData, sizeof(rcvData));
		if (rcvData.protocol != TB('HTXF')) 
			goto killThisConn;
						
		if (rcvData.refNum & 1)		// if is download
		{
			if (FB(rcvData.type) == 2)		// download banner
			{
				db = GetDownloadBannerByRefNum(rcvData.refNum);
				if (db == nil) goto killThisConn;
					
				mTransferEstabList.RemoveItem(tpt);
				db->tpt = tpt;
			} 
			else if (FB(rcvData.type) == 1)	// download folder
			{
				df = GetDownloadFldrByRefNum(rcvData.refNum);
				if (df == nil) goto killThisConn;
					
				mTransferEstabList.RemoveItem(tpt);
				df->tpt = tpt;
			}
			else							// download file
			{
				dt = GetDownloadByRefNum(rcvData.refNum);
				if (dt == nil) goto killThisConn;
					
				mTransferEstabList.RemoveItem(tpt);
				dt->tpt = tpt;
			}
				
			i--;	
				
		#if !FACELESS	
	
			Log(tpt, "Download connection established");
	
			//SetClientDownloading(dt->client, true);
		#endif
		
		}
		else
		{
			if (FB(rcvData.type) == 1)		// upload folder
			{
				uf = GetUploadFldrByRefNum(rcvData.refNum);
				if (uf == nil) goto killThisConn;
				
				mTransferEstabList.RemoveItem(tpt);
				uf->tpt = tpt;
			}
			else							// upload file
			{
				ut = GetUploadByRefNum(rcvData.refNum);
				if (ut == nil) goto killThisConn;
				
				mTransferEstabList.RemoveItem(tpt);
				ut->tpt = tpt;
				ut->totalSize = FB(rcvData.dataSize);
			}
				
			i--;
				
		#if !FACELESS	
		
			Log(tpt, "Upload connection established");
	
		#endif
		}
				
		continue;
		
killThisConn:

	#if !FACELESS
	
		Log(tpt, "Transfer connection establish failed!");
	
	#endif

		mTransferEstabList.RemoveItem(tpt);
		delete tpt;
		i--;
	}
}

void CMyApplication::ProcessDownloads()
{
	SMyDownloadData *dt = nil;
	Uint32 i = 0;
	TTransport tpt;
	void *buf;
	Uint32 s, n;

	Uint32 curSecs = UDateTime::GetSeconds();

	// process downloads
	while (mDownloadList.GetNext(dt, i))
	{
		tpt = dt->tpt;
		if (!mClientList.IsInList((CLink *)dt->client)) 	// if the client has disconnected
			goto connectionClosed;
			
		
	
		if (tpt == nil)		// if download has not been initiated yet
		{
		dt->lastSecs = UDateTime::GetSeconds();
			// if download has been waiting idle for too long, kill it
			if (UDateTime::GetSeconds() - dt->startSecs > 500)
			{
				mDownloadList.RemoveItem(dt);
				delete dt->file;
				delete dt;
				i--;
			
			#if !FACELESS	
			
				Log("Download aborted (no response)");
				UpdateDisplay();
	
			#endif
			}
		}
		else
		{
		if (dt->client->isBlockDownload){
		
				tpt->Disconnect();
				#if !FACELESS
				Log("Download blocked!");
				UpdateDisplay();
				#endif
				
		}
			// check if connection has closed
			if (!tpt->IsConnected())
			{
connectionClosed:
			#if !FACELESS
			
				Log(tpt, "Download connection closed");

				//SetClientDownloading(dt->client, false);
			#endif
			
				mDownloadList.RemoveItem(dt);
				delete dt->file;
				delete tpt;
				delete dt;
				i--;
			
			#if !FACELESS
				UpdateDisplay();
			#endif
			
				continue;
			}
			
			// send more data if necessary
			try
			{
				if (dt->file)
				{
						s = tpt->GetUnsentSize();
						if ((s <= dt->sendSize) || tpt->IsReadyToSend())
						{
							// if STALLED (had no data to send) then we didn't send enough
							if (s == 0) dt->sendSize += 102400;
							
							// if processed within the last 10 seconds, we didn't send enough
							if ((curSecs - dt->sendSecs) < 10)
								dt->sendSize += 102400;
							
							dt->sendSecs = curSecs;
							
							// don't let sendSize go over 2M
							if (dt->sendSize > 2097152)
								dt->sendSize = 2097152;
							
							// determine size to send
							s = dt->sendSize;
							n = dt->size - dt->bytesSent;
							if (s > n) s = n;
							
							// read data and send
							
							buf = UTransport::NewBuffer(s);
							try
							{
								if (dt->isRaw)
								{
									if (dt->file->Read(dt->offset, buf, s) != s) Fail(errorType_Misc, error_Unknown);
									dt->offset += s;
								}
								else
									dt->file->ProcessFlatten(buf, s);
								
								tpt->SendBuffer(buf);
							}
							catch(...)
							{
								UTransport::DisposeBuffer(buf);
								throw;
							}
							
							
							
							
							
							// get ready for next send
							dt->bytesSent += s;
							
							//if (UDateTime::GetSeconds() - dt->lastSecs > 50){
							tpt->NotifyReadyToSend();
							//dt->lastSecs = UDateTime::GetSeconds();
							
							//dt->lastSecs = UDateTime::GetSeconds();
							
							
							// check if finished
							if (dt->bytesSent >= dt->size)
							{
								delete dt->file;
								dt->file = nil;
							
							#if !FACELESS
							
								Log(tpt, "Download complete");
								dt->client->numdl++;
								

							#endif
							
							}
						}
				}
			}
			catch(...)
			{
				// error occured while trying to send data, so kill the download
				if (tpt) tpt->Disconnect();
				throw;
			}
		}
	}
}

void CMyApplication::ProcessFldrDownloads()
{
	SMyDownloadFldrData *df = nil;
	Uint32 i = 0;
	TTransport tpt;
	void *buf;
	Uint32 s, n;
	Uint8 data[4];
	
	Uint32 curSecs = UDateTime::GetSeconds();

	// process downloads
	while (mDownloadFldrList.GetNext(df, i))
	{
		tpt = df->tpt;
		
		if (!mClientList.IsInList((CLink *)df->client)) 	// if the client has disconnected
			goto connectionClosed;

		if (tpt == nil)		// if download has not been initiated yet
		{
			// if download has been waiting idle for too long, kill it
			if (UDateTime::GetSeconds() - df->startSecs > 500)
			{
			
				mDownloadFldrList.RemoveItem(df);
				if (df->fldr) delete df->fldr;
				delete df;
				i--;
				
			#if !FACELESS
			
				Log("Download aborted (no response)");
				//SetClientDownloading(df->client, false);
				UpdateDisplay();
			
			#endif
			}
		}
		else
		{
			// check if connection has closed
			if (!tpt->IsConnected())
			{
connectionClosed:
			#if !FACELESS
			
				Log(tpt, "Download folder complete");
				//Log(tpt, "Download connection closed");

				//SetClientDownloading(df->client, false);
			#endif
				df->client->numflddl++;
				mDownloadFldrList.RemoveItem(df);
				if (df->fldr) delete df->fldr;
				if (df->file) delete df->file;
				delete tpt;
				delete df;
				i--;
			
			#if !FACELESS
				UpdateDisplay();
			#endif
			
				continue;
			}
			
switchAgain:			
			switch (df->state)
			{
				// the bulk of the beast
								
				case 0:  // waiting for the next message
				{
					if (!tpt->IsReadyToReceive(sizeof(Uint16)))
						break;

					tpt->Receive(data, sizeof(Uint16));
						
					if (FB(*((Uint16 *)data)) == dlFldrAction_NextFile)
					{
goNextFile:
						// send the header for the next file
							
						Uint8 path[1024];
						Int16 maxPathSize = sizeof(path);
						Uint8 *startPtr = path + maxPathSize;
						Uint16 pathCount = 0;
						bool folder = false;

						TFSRefObj* file = df->fldr->GetNextItem(startPtr, maxPathSize, pathCount, &folder);
							
						if (!file)
						{
							// we're done
							mDownloadFldrList.RemoveItem(df);
							if (df->fldr) delete df->fldr;
							delete df;
							delete tpt;
							i--;
								
							continue;
						}
														
						if (maxPathSize < 6)
						{
							// if the path was too large:
							// to do - increment the count
							// check if count == total
							// if so, kill!
							goto goNextFile;
						}
														
						if (df->file) delete df->file;
						df->file = file->Clone();
							
						// skip the first folder in the path since this is our root
						startPtr += 2;
						startPtr += *startPtr + 1;
						pathCount--;
						Uint32 s = path + sizeof(path) - startPtr + 4;
							
						// send that data:
						*--((Uint16 *)startPtr) = TB((Uint16)pathCount);
						*--((Uint16 *)startPtr) = TB((Uint16)(folder ? 1 : 0));
						*--((Uint16 *)startPtr) = TB((Uint16)s);
							
						tpt->Send(startPtr, s + 2);	
						df->state = folder ? 0 : 1;
					}				
				}
				break;
				
				case 1:	// waiting for instructions
				{
					if (!tpt->IsReadyToReceive(sizeof(Uint16)))
						break;

					tpt->Receive(data, sizeof(Uint16));
					Uint16 type = FB(*((Uint16 *)data));
						
					if (type == dlFldrAction_NextFile)
					{
						df->totalSize -= df->file->GetSize();
						goto goNextFile;
					}
					else if (type == dlFldrAction_SendFile)
					{
						df->file->StartFlatten();
sendFileInfo:							
						df->size = df->file->GetFlattenSize();
						df->totalSize -= df->file->GetSize() - df->size;

						// send the size and stuff
						*((Uint32 *)data) = FB(df->size);
						df->tpt->Send(data, sizeof(Uint32));
							
						df->state = 2;
						goto switchAgain;			
					}
					else if (type == dlFldrAction_ResumeFile)
					{
						df->state = 10;
						goto switchAgain;
					}
				}
				break;
				
				case 2:	// sending data
				{
					// send more data if necessary
					try
					{
						if (df->file)
						{
							s = tpt->GetUnsentSize();
							
							if ((s <= df->sendSize) || tpt->IsReadyToSend())
							{
								// if STALLED (had no data to send) then we didn't send enough
								if (df->size < 102400)
								{
									// we could be stalled because of many small files, and if that's the case, don't overdo it!
									if (df->sendSize < df->size)
										df->sendSize = df->size;
								}
								else	// a real stall - let's fix it.
								{
									if (s == 0)
										df->sendSize += 102400;
										
									// if processed within the last 10 seconds, we didn't send enough
									if ((curSecs - df->sendSecs) < 10)
										df->sendSize += 102400;
								
									df->sendSecs = curSecs;
								}
								
								// don't let sendSize go over 2M
								if (df->sendSize > 2097152)
									df->sendSize = 2097152;
								
								// determine size to send
								s = df->sendSize;
								n = df->size - df->bytesSent;
								if (s > n) s = n;
								
								// read data and send
								buf = UTransport::NewBuffer(s);
								try
								{
									df->file->ProcessFlatten(buf, s);
									
									tpt->SendBuffer(buf);
								}
								catch(...)
								{
									UTransport::DisposeBuffer(buf);
									throw;
								}
								
								// get ready for next send
								df->bytesSent += s;
								df->totalBytesSent += s;
								tpt->NotifyReadyToSend();
								
								// check if finished
								if (df->bytesSent >= df->size)
								{
									delete df->file;
									df->file = nil;
									df->size = 0;
									df->bytesSent = 0;
									df->state = 0;
									
									goto switchAgain;
								}
							}
						}
					}
					catch(...)
					{
						// error occured while trying to send data, so kill the download
						tpt->Disconnect();
						throw;
					}
				}
				break;
				
				case 10:	// resume: receive two byte header
				{
					if (!tpt->IsReadyToReceive(sizeof(Uint16)))
						break;

					tpt->Receive(data, sizeof(Uint16));	
					df->resumeHdrSize = FB(*((Uint16 *)data));
						
					df->state++;
					goto switchAgain;
				}
				break;
				
				case 11:	// resume: receive the header
				{
					if (!tpt->IsReadyToReceive(df->resumeHdrSize))
						break;

					StPtr hdr(df->resumeHdrSize);
					df->file->ResumeFlatten(hdr, tpt->Receive(hdr, df->resumeHdrSize));
					
					goto sendFileInfo;
				}
				break;
			}						
		}
	}
}

void CMyApplication::ProcessDownloadsWaiting()
{
	bool bNewDownloads = false;
	SMyDownloadWaitingData *dw = nil;
	Uint32 i = 0;
	
	// process downloads waiting list
	while (mDownloadWaitingList.GetNext(dw, i))
	{
		TTransport tpt;
		SMyClient *pClient;
		
		if (dw->bFileFolder)
		{
			tpt = dw->pDownloadData->tpt;
			pClient = dw->pDownloadData->client;
		}
		else
		{
			tpt = dw->pDownloadFldrData->tpt;
			pClient = dw->pDownloadFldrData->client;
		}
		
		if (!mClientList.IsInList((CLink *)pClient)) 	// if the client has disconnected
			goto connectionClosed;	// this should fix the server queue bug

		if (tpt && !tpt->IsConnected())	// check if connection has closed
		{
connectionClosed:
		#if !FACELESS
		
			Log(dw, "Download connection closed");
	
		#endif
		
			mDownloadWaitingList.RemoveItem(dw);
			DeleteDownloadWaiting(dw);
			i--;
			
		#if !FACELESS
			UpdateDisplay();
		#endif
		
			continue;
		}
		
		Uint32 nRefNum;
		if (dw->bFileFolder)
			nRefNum = dw->pDownloadData->refNum;
		else
			nRefNum = dw->pDownloadFldrData->refNum;
		
		Uint32 nDownloadsAhead = 0;
		Uint32 nDownloadsCount = mDownloadList.GetItemCount() + mDownloadFldrList.GetItemCount();
		if (nDownloadsCount >= mOptions.nMaxDownloads)
			nDownloadsAhead = nDownloadsCount - mOptions.nMaxDownloads + 1;
				
		Uint32 nClientDownloadsAhead = 0;
		Uint32 nClientDownloadsCount = GetClientDownloadsNumber(pClient->userID);
		if (nClientDownloadsCount >= mOptions.nMaxDownloadsClient)
			nClientDownloadsAhead = nClientDownloadsCount - mOptions.nMaxDownloadsClient + 1;
				
		Uint32 nWaitingCount = nDownloadsAhead > nClientDownloadsAhead ? nDownloadsAhead : nClientDownloadsAhead;
		
		
		if (HasGeneralPriv(pClient, myAcc_DontQueue)){
		nWaitingCount = 0;
		}
			
		if (nWaitingCount)	// still in queue
		{
			nWaitingCount += i - 1;
			if (dw->nWaitingCount !=  nWaitingCount)
			{
				dw->nWaitingCount = nWaitingCount;
				SendClientDownloadInfo(pClient, nRefNum, nWaitingCount);
			}
		}
		else				// out of the queue
		{
			SendClientDownloadInfo(pClient, nRefNum, nWaitingCount);
				
			mDownloadWaitingList.RemoveItem(dw);
			if (dw->bFileFolder)
			{
				dw->pDownloadData->startSecs = UDateTime::GetSeconds();		// reset the start time
				mDownloadList.AddItem(dw->pDownloadData);
			}
			else
			{
				dw->pDownloadFldrData->startSecs = UDateTime::GetSeconds();	// reset the start time
				mDownloadFldrList.AddItem(dw->pDownloadFldrData);
			}
					
			delete dw;
			i--;
			
			bNewDownloads = true;
			
		#if !FACELESS
			UpdateDisplay();
		#endif
		}
	}
	
	if (bNewDownloads)
		UApplication::PostMessage(msg_DataArrived);
}

void CMyApplication::ProcessBannerDownloads()
{
	if (!mDownloadBannerList.GetItemCount() || !mServerBanner)
		return;

	SMyDownloadBannerData *db = nil;
	Uint32 i = 0;
	TTransport tpt;

	Uint32 curSecs = UDateTime::GetSeconds();
	Uint32 nBannerSize = UMemory::GetSize(mServerBanner);

	// process banner downloads
	while (mDownloadBannerList.GetNext(db, i))
	{
		tpt = db->tpt;
		
		if (!mClientList.IsInList((CLink *)db->client)) 	// if the client has disconnected
			goto connectionClosed;

		if (tpt == nil)		// if download has not been initiated yet
		{
			// if download has been waiting idle for too long, kill it
			if (UDateTime::GetSeconds() - db->startSecs > 500)
			{
				mDownloadBannerList.RemoveItem(db);
				delete db;
				i--;
				
			#if !FACELESS
			
				Log("Download banner aborted (no response)");

			#endif
			}
		}
		else
		{
			// check if connection has closed
			if (!tpt->IsConnected())
			{
connectionClosed:
			#if !FACELESS
			
				Log(tpt, "Download connection closed");
			
			#endif
				
				mDownloadBannerList.RemoveItem(db);
				delete tpt;
				delete db;
				i--;
				
				continue;
			}
			
			if (!db->bytesSent)
			{
				// send more data if necessary
				void *pBanner = UTransport::NewBuffer(nBannerSize);
				try
				{
					UMemory::Copy(pBanner, UMemory::Lock(mServerBanner), nBannerSize);
					UMemory::Unlock(mServerBanner);

					tpt->SendBuffer(pBanner);
					
					db->bytesSent += nBannerSize;
					tpt->NotifyReadyToSend();
					
				#if !FACELESS
				
					Log(tpt, "Download banner complete");
		
				#endif
				}
				catch(...)
				{
					// error occured while trying to send banner, so kill the download
					tpt->Disconnect();
					UTransport::DisposeBuffer(pBanner);

					throw;
				}
			}
		}
	}	
}

void CMyApplication::ProcessUploads()
{
	SMyUploadData *ut = nil;
	Uint32 i = 0;
	TTransport tpt;
	void *rcvBuf = nil;
	Uint32 s, curSecs;
	
	curSecs = UDateTime::GetSeconds();

	try
	{
		// process uploads
		while (mUploadList.GetNext(ut, i))
		{
			tpt = ut->tpt;
			
			if (!mClientList.IsInList((CLink *)ut->client)) 	// if the client has disconnected
				goto connectionClosed;

			if (tpt == nil)		// if upload has not been initiated yet
			{
				// if upload has been waiting idle for too long, kill it
				if (curSecs - ut->startSecs > 500)
				{
					mUploadList.RemoveItem(ut);
					delete ut->file;
					delete ut;
					i--;
					
				#if !FACELESS
				
					Log("Upload aborted (no response)");
					UpdateDisplay();
		
				#endif
				}
			}
			else
			{
				// check if connection has closed or haven't got any data for 2mins
				if (!tpt->IsConnected() || (curSecs - ut->startSecs > 120))
				{
connectionClosed:
				#if !FACELESS
				
					Log(tpt, "Upload aborted");

				#endif
					
					mUploadList.RemoveItem(ut);
					delete tpt;
					delete ut->file;
					delete ut;
					i--;
				
				#if !FACELESS
					UpdateDisplay();
				#endif
					
					continue;
				}
				
				// receive pending data
				try
				{
					while (tpt->ReceiveBuffer(rcvBuf, s))
					{
						ut->file->ProcessUnflatten(rcvBuf, s);
						
						ut->rcvdSize += s;
						ut->startSecs = curSecs;	// time at which we last received data
						
						UTransport::DisposeBuffer(rcvBuf);
						rcvBuf = nil;
					}
				}
				catch(...)
				{
					tpt->Disconnect();
					throw;
				}
				
				// check if upload complete
				if (ut->rcvdSize >= ut->totalSize)
				{
				#if !FACELESS
				
					Log(tpt, "Upload complete");
					
			
				#endif
					ut->client->numul++;
					mUploadList.RemoveItem(ut);
					ut->file->ProcessUnflatten(nil, 0);
				
				#if WIN32
					Uint8 psValidatedName[256];
					psValidatedName[0] = ut->file->GetName(psValidatedName + 1, sizeof(psValidatedName) - 1);
					
					// take the .hpf out of the name
					if (psValidatedName[0] >= 4 && !UMemory::Compare(psValidatedName + psValidatedName[0] - 3, ".hpf", 4))
					{						
						psValidatedName[0] -= 4;	// in case someone made a name like c.lnk..hpf, revalidate the new name
						psValidatedName[0] = UFS::ValidateFileName(psValidatedName + 1, psValidatedName[0]);

						ut->file->StopUnflatten();	// can't rename open file					
						ut->file->SetName(psValidatedName);
					}
				#endif

					tpt->Disconnect();
					delete ut->file;
					delete tpt;
					delete ut;
					i--;
				
				#if !FACELESS
					UpdateDisplay();
				#endif
				}
			}
		}
	}
	catch(...)
	{
		// clean up
		UTransport::DisposeBuffer(rcvBuf);
		throw;
	}

	UTransport::DisposeBuffer(rcvBuf);
}

void CMyApplication::ProcessFldrUploads()
{
	SMyUploadFldrData *uf = nil;
	Uint32 i = 0;
	TTransport tpt;
	Uint8 data[4];
#if WIN32
	Uint8 str[256];
#endif
	
	Uint32 curSecs = UDateTime::GetSeconds();

	// process folder uploads
	while (mUploadFldrList.GetNext(uf, i))
	{
		tpt = uf->tpt;
			
		if (!mClientList.IsInList((CLink *)uf->client)) 	// if the client has disconnected
			goto connectionClosed;

		if (tpt == nil)		// if upload has not been initiated yet
		{
			// if upload has been waiting idle for too long, kill it
			if (curSecs - uf->startSecs > 500)
			{
				mUploadFldrList.RemoveItem(uf);
				delete uf->fldr;
				delete uf;
				i--;
					
			#if !FACELESS
			
				Log("Upload aborted (no response)");
				UpdateDisplay();
			
			#endif
			}
		}
		else 
		{
			// check if connection has closed or haven't got any data for 2mins
			if (!tpt->IsConnected() || (curSecs - uf->startSecs > 120))
			{
connectionClosed:
			#if !FACELESS
			
				Log(tpt, "Upload aborted");
			
			#endif
				
				mUploadFldrList.RemoveItem(uf);
				delete tpt;
				delete uf->fldr;
				if (uf->file) delete uf->file;
				delete uf;
				i--;

			#if !FACELESS
				UpdateDisplay();
			#endif
				
				continue;
			}
			
switchAgain:			
			switch (uf->state)
			{
				case 0:
				{
					*((Uint16 *)data) = TB((Uint16)dlFldrAction_NextFile);
					tpt->Send(data, sizeof(Uint16));
					
					uf->state++;
				}
				break;
				
				case 1:
				{
					if (!tpt->IsReadyToReceive(sizeof(Uint16)))
						break;

					tpt->Receive(data, sizeof(Uint16));
					uf->headerSize = FB(*((Uint16 *)data));

					uf->state++;
					goto switchAgain;
				}
				break;
					
				case 2:
				{
					if (!tpt->IsReadyToReceive(uf->headerSize))
						break;
					
					// receive the header
					StPtr header(uf->headerSize);
					tpt->Receive(header, uf->headerSize);

					Uint8 *p = BPTR(header);
					Uint16 type = FB(*((Uint16 *)p)++);
					Uint32 nPathSize = 0;
					uf->file = nil;

					bool failed = false;
					try
					{
						nPathSize = UFS::ValidateFilePath(p, uf->headerSize - 2);
						uf->file = UFS::New(uf->fldr, p, nPathSize, nil);
						if (type & 1) // folder name
						{
							bool isFolder;
							if (!uf->file->Exists(&isFolder))
								uf->file->CreateFolder();
							else if (!isFolder)
								failed = true;
						}
						else if (FindResumableFile(uf->file))
						{
							uf->totalSize -= uf->file->GetSize();
							// may fail if file cannot be opened
							THdl hdl = uf->file->ResumeUnflatten(); 
							Uint32 s = hdl->GetSize();
													
							void *hdlPtr;
							StHandleLocker lock(hdl, hdlPtr);
							StPtr buf(s + sizeof(Uint16) + sizeof(Uint16));
							CFlatten flat(buf);
							flat.WriteWord(dlFldrAction_ResumeFile);
							flat.WriteWord(s);
							flat.Write(hdlPtr, s);
							// send a "do it!" with resume data
							tpt->Send(buf, s + sizeof(Uint16) + sizeof(Uint16));
							// change of state , this state is complete
							uf->state = 3;
						}
						else if (uf->file->Exists())
						{   // ignore the already existing file
							uf->totalSize -= uf->file->GetSize();
						}
						else // the item is a file 
						{
							#if WIN32
							{   // disallow uploading of .LNK (shortcuts) and 
							    // .LNK.HPF files on windoze
								Uint8 fileName[256];
								uf->file->GetName(fileName);
								Uint32 ts = fileName[0];

								if (ts > 3 && UText::toupper(fileName[ts-2]) == 'H' 
										   && UText::toupper(fileName[ts-1]) == 'P' 
										   && UText::toupper(fileName[ts]) == 'F' 
										   && fileName[ts-3] == '.')
									ts -= 4;

								if (ts > 3 && UText::toupper(fileName[ts-2]) == 'L' 
										   && UText::toupper(fileName[ts-1]) == 'N' 
										   && UText::toupper(fileName[ts]) == 'K' 
										   && fileName[ts-3] == '.')
									failed = true;
								if (!failed)
								{
									// put the .hpf extension
									uf->file->GetName(str);
									pstrcat(str, "\p.hpf");
									uf->file->SetRefName(str);
								}
							}
							#endif
							
							if (!failed)
							{
								uf->file->CreateFile('HTft', 'HTLC');
								uf->file->StartUnflatten();
								// send a "do it!" msg
								*((Uint16 *)data) = TB((Uint16)dlFldrAction_SendFile);
								tpt->Send(data, sizeof(Uint16));
								// change of state , this state is complete
								uf->state = 3;
							}
						}
					}
					catch (...) // some failure at this item go to next item
					{
						failed = true;
					}

					// failure or no change of state means we're done with this item
					if (failed || uf->state ==2)
					{
						delete uf->file;
						uf->file = nil;

						*((Uint16 *)data) = TB((Uint16)dlFldrAction_NextFile);
						tpt->Send(data, sizeof(Uint16));
							
						if (++uf->uploadedItems == uf->totalItems)
						{   // do all sortsa cleanup here
						#if !FACELESS
						
							if (failed)
								Log(tpt, "Upload folder item failed");
							else
								Log(tpt, "Upload folder complete");
							UpdateDisplay();
						
						#endif
								uf->client->numfldul++;
							mUploadFldrList.RemoveItem(uf);

							tpt->Disconnect();
							delete uf->fldr;
							delete tpt;
							delete uf;
							i--;
							continue; // next upload task
						}
						else 
							uf->state = 1;
					}

					uf->headerSize = 0;
					goto switchAgain;
				}
				break;

				case 3:		// receive the total size of this file	
				{
					if (!tpt->IsReadyToReceive(sizeof(Uint32)))
						break;

					tpt->Receive(data, sizeof(Uint32));
					uf->fileTotalSize = FB(*((Uint32 *)data));
					uf->fileUlSize = 0;
									
					uf->state++;
					goto switchAgain;					
				}
				break;
				
				case 4: // receive and process data
				{
					void *rcvBuf;
					Uint32 s;
					while (tpt->ReceiveBuffer(rcvBuf, s))
					{	
						// process data
						try
						{
							uf->file->ProcessUnflatten(rcvBuf, s);
														
							uf->fileUlSize += s;
							uf->rcvdSize += s;
															
							uf->startSecs = curSecs;	// time at which we last received data
						}
						catch(...)
						{
							UTransport::DisposeBuffer(rcvBuf);
							throw;
						}
				
						UTransport::DisposeBuffer(rcvBuf);
					}
					
					if (uf->fileUlSize >= uf->fileTotalSize)
					{
						// complete unflatten
						uf->file->ProcessUnflatten(nil, 0);

					#if WIN32
						uf->file->GetName(str);
						
						// take the .hpf out of the name
						if (str[0] >= 4 && !UMemory::Compare(str + str[0] - 3, ".hpf", 4))
						{
							str[0] -= 4;	// in case someone made a name like c.lnk..hpf, revalidate the new name
							str[0] = UFS::ValidateFileName(str + 1, str[0]);

							uf->file->StopUnflatten();	// can't rename open file
							uf->file->SetName(str);
						}
					#endif

						// delete stuff
						delete uf->file;
						uf->file = nil;

						if (++uf->uploadedItems == uf->totalItems)
						{
							// do all sortsa cleanup here
						#if !FACELESS
						
							Log(tpt, "Upload folder complete");
						
						#endif
							uf->client->numfldul++;
							mUploadFldrList.RemoveItem(uf);

							tpt->Disconnect();
							delete uf->fldr;
							delete tpt;
							delete uf;
							i--;
						
						#if !FACELESS
							UpdateDisplay();
						#endif
							
							continue;	
						}
						else
						{
							uf->state = 1;

							*((Uint16 *)data) = TB((Uint16)dlFldrAction_NextFile);
							tpt->Send(data, sizeof(Uint16));								
						}					
					}
				}
				break;
			};
		}
	}
}

SMyDownloadData *CMyApplication::GetDownloadByRefNum(Uint32 inRefNum)
{	
	Uint32 n = mDownloadList.GetItemCount();
	if (n)
	{
		SMyDownloadData **p = mDownloadList.GetArrayPtr();
		SMyDownloadData *dt;
		
		while (n--)
		{
			dt = *p++;
			
			if (dt->refNum == inRefNum)
				return dt;
		}
	}
	
	n = mDownloadWaitingList.GetItemCount();
	if (n)
	{
		SMyDownloadWaitingData **p = mDownloadWaitingList.GetArrayPtr();
		SMyDownloadWaitingData *dw;
		
		while (n--)
		{
			dw = *p++;
			
			if (dw->bFileFolder && dw->pDownloadData->refNum == inRefNum)
				return dw->pDownloadData;
		}
	}
	
	return nil;
}

SMyDownloadFldrData *CMyApplication::GetDownloadFldrByRefNum(Uint32 inRefNum)
{
	Uint32 n = mDownloadFldrList.GetItemCount();
	if (n)
	{
		SMyDownloadFldrData **p = mDownloadFldrList.GetArrayPtr();
		SMyDownloadFldrData *dt;
		
		while (n--)
		{
			dt = *p++;
			
			if (dt->refNum == inRefNum)
				return dt;
		}
	}
		
	n = mDownloadWaitingList.GetItemCount();
	if (n)
	{
		SMyDownloadWaitingData **p = mDownloadWaitingList.GetArrayPtr();
		SMyDownloadWaitingData *dw;
		
		while (n--)
		{
			dw = *p++;
			
			if (!dw->bFileFolder && dw->pDownloadFldrData->refNum == inRefNum)
				return dw->pDownloadFldrData;
		}
	}

	return nil;
}

SMyDownloadBannerData *CMyApplication::GetDownloadBannerByRefNum(Uint32 inRefNum)
{	
	Uint32 n = mDownloadBannerList.GetItemCount();
	if (n)
	{
		SMyDownloadBannerData **p = mDownloadBannerList.GetArrayPtr();
		SMyDownloadBannerData *db;
		
		while (n--)
		{
			db = *p++;
			
			if (db->refNum == inRefNum)
				return db;
		}
	}
		
	return nil;
}

bool CMyApplication::KillDownloadByRefNum(SMyClient *inClient, Uint32 inRefNum)
{	
	Uint32 n = mDownloadWaitingList.GetItemCount();
	if (n)
	{
		SMyDownloadWaitingData **p = mDownloadWaitingList.GetArrayPtr();
		SMyDownloadWaitingData *dw;
		
		while (n--)
		{
			dw = *p++;
			
			if ((dw->bFileFolder && dw->pDownloadData->refNum == inRefNum) || (!dw->bFileFolder && dw->pDownloadFldrData->refNum == inRefNum))
			{
				if ((dw->bFileFolder && dw->pDownloadData->client != inClient) || (!dw->bFileFolder && dw->pDownloadFldrData->client != inClient))
					return false;
				
			#if !FACELESS
			
				Log(dw, "Download connection closed");
			
			#endif

				mDownloadWaitingList.RemoveItem(dw);
				DeleteDownloadWaiting(dw);

			#if !FACELESS	
				UpdateDisplay();
			#endif
				
				return true;
			}
		}
	}

	n = mDownloadList.GetItemCount();
	
	if (n)
	{
		SMyDownloadData **p = mDownloadList.GetArrayPtr();
		SMyDownloadData *dt;
		
		while (n--)
		{
			dt = *p++;
			
			if (dt->refNum == inRefNum)
			{
				if (dt->client != inClient)
					return false;
				
				mDownloadList.RemoveItem(dt);

			#if !FACELESS
			
				Log(dt->tpt, "Download connection closed");
				
			#endif

				delete dt->tpt;
				delete dt->file;
				delete dt;
	
			#if !FACELESS	
				UpdateDisplay();
			#endif

				return true;
			}
		}
	}

	n = mDownloadFldrList.GetItemCount();
	if (n)
	{
		SMyDownloadFldrData **p = mDownloadFldrList.GetArrayPtr();
		SMyDownloadFldrData *df;
		
		while (n--)
		{
			df = *p++;
			
			if (df->refNum == inRefNum)
			{
				if (df->client != inClient)
					return false;
					
				df->client->numflddl++;
				#if !FACELESS
				Log(df->tpt, "Download folder complete");
				#endif
			
				mDownloadFldrList.RemoveItem(df);

			#if !FACELESS
			
				
				Log(df->tpt, "Download connection closed");
				
			#endif

				delete df->tpt;
				if (df->fldr) delete df->fldr;
				if (df->file) delete df->file;
				delete df;
	
			#if !FACELESS	
				UpdateDisplay();
			#endif

				return true;
			}
		}
	}
			
	return false;
}

SMyUploadData *CMyApplication::GetUploadByRefNum(Uint32 inRefNum)
{	
	Uint32 n = mUploadList.GetItemCount();
	if (n)
	{
		SMyUploadData **p = mUploadList.GetArrayPtr();
		SMyUploadData *ut;
		
		while (n--)
		{
			ut = *p++;
			
			if (ut->refNum == inRefNum)
				return ut;
		}
	}
	
	return nil;
}

SMyUploadFldrData *CMyApplication::GetUploadFldrByRefNum(Uint32 inRefNum)
{	
	Uint32 n = mUploadFldrList.GetItemCount();
	if (n)
	{
		SMyUploadFldrData **p = mUploadFldrList.GetArrayPtr();
		SMyUploadFldrData *uf;
		
		while (n--)
		{
			uf = *p++;
			
			if (uf->refNum == inRefNum)
				return uf;
		}
	}
	
	return nil;
}

// returns true if inRef was modified to point to a resumable file
bool CMyApplication::FindResumableFile(TFSRefObj* inRef)
{
	TFSRefObj* fp = inRef->Clone();
	scopekill(TFSRefObj, fp);
	
	Uint8 psFileName[256];
	fp->GetName(psFileName);

#if WIN32
	pstrcat(psFileName, "\p.hpf");
	fp->SetRefName(psFileName);

	if (fp->Exists())
	{
		inRef->SetRef(fp);
		return true;
	}	
#else
	if (fp->Exists())
	{
		Uint32 typeCode, creatorCode;
		inRef->GetTypeAndCreatorCode(typeCode, creatorCode);
		return (creatorCode == TB((Uint32)'HTLC') && typeCode == TB((Uint32)'HTft'));
	}
#endif
	
	return false;
}

