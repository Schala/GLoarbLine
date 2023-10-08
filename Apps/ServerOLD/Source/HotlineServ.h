/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#pragma def_inherited on

#include "HotlineClientServerCommon.h"
#include "HotlineServNewsDatabase.h"
#include "HotlineFolderDownload.h"


#define BANNER_SERVER				1
#define NETWORK_SERVER				0
#define AUTO_BAN_MULTI_PRIV_MSGS 	1
#define NEW_TRACKER					0
#define NEW_NEWS_DB					1
#define SHOW_SERVER_INFO_IN_TOOLBAR	1
#define FACELESS					0
#define BOTVERS						1

#if WIN32
	#define LIST_BACKGROUND_OPTION		0
#elif MACINTOSH
	#define LIST_BACKGROUND_OPTION		scrollerOption_NoBkgnd
#endif

#pragma mark ¥¥ Constants ¥¥

#pragma mark View IDs
enum 
{
	viewID_Broadcast			= 100,
	viewID_Connect				= 101,
	viewID_Reload				= 102,
	viewID_ShowLog				= 103,
	viewID_ShowStats			= 104,
	viewID_Quit					= 105,
	viewID_Options				= 106,
	viewID_QuitNow				= 107,
	viewID_CancelQuit			= 108,
	viewID_ToolbarInfo			= 109,
	viewID_CloseWindow			= 110,
	viewID_WindowNext			= 111
};

#pragma mark Server Options
enum 
{
	myOpt_UseTracker		= 0x0001,
	myOpt_LogDownloads		= 0x0002,
	myOpt_LogUploads		= 0x0004,
	myOpt_LogConnects		= 0x0008,
	
	myOpt_loginmessage		= 0x0010,
	myOpt_ConfirmQuit		= 0x0020,
	myOpt_LogAccountChanges	= 0x0040,
	myOpt_AgreementImageUrl	= 0x0080,
	
	myOpt_kickunnamedusers	= 0x0100,
	myOpt_CsvMode			= 0x0200,
	myOpt_BotMode			= 0x0400,
	myOpt_showKick			= 0x0800,
	
	myOpt_logmsgchat		= 0x1000,
	myOpt_logmsgpvmsg		= 0x2000,
	myOpt_logmsgpvmsg2		= 0x4000,
	myOpt_logmsgbroad		= 0x8000
	
	
	
	//myOpt_speakbeforedl		= 0x0200,
};

enum 
{
	kMaxPermBans		= 500,
	kMaxTempBans		= 32
	//kBanTimeLimitSecs	= 1800		// 30 minutes
};

enum 
{
	kMyPrefsVersion		= 5,
	kBanFileVersion1	= 1,
	kBanFileVersion2	= 2, // with ip-ip range
	
	myWinVis_Toolbar	= 0x01,
	myWinVis_Log		= 0x02,
	myWinVis_Stats		= 0x04
};

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
#pragma mark ¥¥ Structures ¥¥

// SMyOptions
#pragma mark SMyOptions
struct SMyOptions
 {
	SIPAddress stIPAddress;
	Uint16 nBasePortNum;	// port num to save in prefs
	Uint16 nCurPortNum;		// port num currently being used
	Uint16 nMaxDownloads;
	Uint16 nawaydelay;
	Uint16 nbandelay;
	Uint16 nMaxDownloadsClient;
	Uint16 nMaxConnectionsIP;
	Uint32 nOpts;
};

struct SMyClient;

// SMyDownloadData
#pragma mark SMyDownloadData
struct SMyDownloadData 
{
	TTransport tpt;
	TFSRefObj* file;
	Uint32 refNum;
	Uint32 startSecs;
	Uint32 lastSecs;
	Uint32 start;
	Uint32 offset, size, bytesSent, sendSize, sendSecs;
	SMyClient *client;	// THIS MIGHT BE INVALID!!!  Check if in client list
	Uint16 clientID;
	bool isRaw;
	Uint8 name[32];
};

// SMyDownloadFldrData
#pragma mark SMyDownloadFldrData
struct SMyDownloadFldrData
{
	TTransport tpt;
	CMyDLFldr *fldr;
	TFSRefObj* file;
	Uint32 refNum;
	Uint32 startSecs;
	Uint32 start;
	Uint32 totalSize, totalBytesSent;
	// file stuff
	Uint32 size, bytesSent, sendSize, sendSecs;
	SMyClient *client;	// THIS MIGHT BE INVALID!!!  Check if in client list
	Uint16 clientID;
	Uint16 count;
	Uint16 state;
	Uint16 resumeHdrSize;
	Uint8 name[32];
};

// SMyDownloadWaitingData
#pragma mark SMyDownloadWaitingData
struct SMyDownloadWaitingData
{
	bool bFileFolder;
	Uint32 nWaitingCount;
	
	union{
		SMyDownloadData *pDownloadData;
		SMyDownloadFldrData *pDownloadFldrData;
	};
};

// SMyDownloadBannerData
#pragma mark SMyDownloadBannerData
struct SMyDownloadBannerData 
{
	TTransport tpt;
	Uint32 refNum;
	Uint32 startSecs;
	Uint32 bytesSent;
	SMyClient *client;
	Uint16 clientID;
};

// SMyUploadData
#pragma mark SMyUploadData
struct SMyUploadData 
{
	TTransport tpt;
	TFSRefObj* file;
	Uint32 refNum;
	Uint32 startSecs;
	Uint32 start;
	Uint32 rcvdSize, totalSize;
	SMyClient *client;	// THIS MIGHT BE INVALID!!!  Check if in client list
	Uint16 clientID;
	Uint8 name[32];
};

// SMyUploadData
#pragma mark SMyUploadFldrData
struct SMyUploadFldrData 
{
	TTransport tpt;
	TFSRefObj* fldr;
	TFSRefObj* file;
	Uint32 refNum;
	Uint32 startSecs;
	Uint32 start;
	Uint32 rcvdSize, totalSize;
	Uint32 totalItems, uploadedItems;
	Uint32 fileTotalSize, fileUlSize;
	Uint32 headerSize;
	SMyClient *client;	// THIS MIGHT BE INVALID!!!  Check if in client list
	Uint16 clientID;
	Uint16 state;
	Uint8 name[32];
};

// SMyUserInfo
#pragma mark SMyUserInfo
#pragma options align=packed
struct SMyUserInfo 
{
	Uint16 id;
	Int16 iconID;
	Uint16 flags;
	Uint16 nameSize;
	Uint8 nameData[];
};
#pragma options align=reset

// SMyChat
#pragma mark SMyChat
struct SMyChat 
{
	SMyChat *next;
	CPtrList<SMyClient> clientList;
	Uint32 id;
	Uint8 *subject;
};

// SMyTrackInfo
#pragma mark SMyTrackInfo
struct SMyTrackInfo
{
	SInternetNameAddress addr;
	Uint8 login[32];
	Uint8 passwd[32];
};

#pragma mark BanRecord
class BanRecord
{
public:
	BanRecord();
	bool Match(Uint32 addr);
	bool MatchName(Uint8 login[64]);
	void IPrangeFromString(Uint8 str[64]);
	void IPrangeToString(Uint8 str[64]);
	
	Uint8  nActive; 
	Uint32 timeout; // the duration this ban is in effect
	Uint32 nAddrFirst;
	Uint32 nAddrLast;
	Uint8  psDateTime[33]; // time of creation
	Uint8  description[64];
};

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
#pragma mark ¥¥ Views ¥¥

#if SHOW_SERVER_INFO_IN_TOOLBAR
#pragma mark CMyClickLabelView
class CMyClickLabelView : public CLabelView
{
	public:
		CMyClickLabelView(CViewHandler *inHandler, const SRect& inBounds);
		
		virtual void MouseUp(const SMouseMsgData& inInfo);
};
#endif

#pragma mark CMyPermBanListView
class CMyPermBanListView : public CGeneralCheckListView<BanRecord>
{
	public:
		CMyPermBanListView( CViewHandler *inHandler, 
							const SRect &inBounds, 
							CTextView *inBanUrlText, 
							CTextView *inBanDescrText );
	
		Uint32 AddPermBan();
		
		bool ModifyPermBan( Uint32 inIndex, 
							Uint8 *inURL, 
							Uint8 *inDescr, 
							bool inActive = false);
		bool ModifySelectedPermBan();
		bool DeleteSelectedPermBan();
		
		virtual void SetItemSelect(Uint32 inItem, bool inSelect);		
				
	protected:
		CTextView *mBanUrlText;
		CTextView *mBanDescrText;
		Uint32 mPermBanListIndex;

		virtual void ItemDraw(Uint32 inItem, TImage inImage, const SRect& inBounds, const SRect& inUpdateRect, Uint32 inOptions = 0);
};

#pragma mark CMyAccountListView
class CMyAccountListView : public CListView
{
	public:
		CMyAccountListView(CViewHandler *inHandler, const SRect& inBounds);
		virtual ~CMyAccountListView();
		
		void SetInfo(const Uint8 *inAdminLogin);
		bool GetInfo(Uint8 *outAdminLogin);

		virtual Uint32 GetItemCount() const;
		virtual Uint32 GetFullHeight() const;

		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);

	protected:
		struct SMyAccountListItem {
			Uint8 nFlags;
			SMyUserAccess stAccess;
			Uint8 psLogin[];
		};

		TIcon mHeadAdminIcon, mDisconnectAdminIcon, mRegularAdminIcon;
		CPtrList<SMyAccountListItem> mAccountList;

		void SelectAdminAccount();
		virtual void ItemDraw(Uint32 inItem, TImage inImage, const SRect& inBounds, const SRect& inUpdateRect, Uint32 inOptions = 0);
		static Int32 CompareLogins(void *inPtrA, void *inPtrB, void *inRef);
};

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
#pragma mark ¥¥ Windows ¥¥

#pragma mark CMyOptionsWin
class CMyOptionsWin : public CWindow
{
	public:
		CMyOptionsWin();
			
	#if BANNER_SERVER & NETWORK_SERVER
		void SetInfo( const Uint8 *inName, const Uint8 *inIncorrectlogin, const Uint8 *inMessagebeforedl, const Uint8 *inPostbeforedl, const Uint8 *inAcceptPrivate, const Uint8 *inLoginMessageText, const Uint8 *inDesc,  
					  Uint8* inSerialNumber, const Uint8 *inAdminLogin, 
					  Uint8 *inAgreementBanner, Uint8 *inAgreementBannerUrl, 
					  Uint8 *inAgreementUrl, CPtrList<BanRecord> *inPermBanList, 
					  const SMyOptions& inOptions);
		void GetInfo( Uint8 *outName, Uint8 *outIncorrectlogin, Uint8 *outMessagebeforedl, Uint8 *outPostbeforedl, Uint8 *outAcceptPrivate, Uint8 *outLoginMessageText, Uint8 *outDesc,
					  Uint8* outSerialNumber, Uint8 *outAdminLogin, 
					  Uint8 *outAgreementBanner,  Uint8 *outAgreementBannerUrl, 
					  Uint8 *outAgreementUrl, CPtrList<BanRecord> *outPermBanList, 
					  SMyOptions& ioOptions);
	#elif BANNER_SERVER
		void SetInfo( const Uint8 *inName, 
					  const Uint8 *inIncorrectlogin,
					  const Uint8 *inDesc, 
					  const Uint8 *inMessagebeforedl,
					  const Uint8 *inPostbeforedl,
					  const Uint8 *inAcceptPrivate,
					  const Uint8 *inLoginMessageText,
					  const Uint8 *inAdminLogin, 
					  Uint8 *inAgreementBanner, 
					  Uint8 *inAgreementBannerUrl, 
					  Uint8 *inAgreementUrl, 
					  CPtrList<BanRecord> *inPermBanList, 
					  const SMyOptions& inOptions);
		void GetInfo( Uint8 *outName, 
					  Uint8 *outIncorrectlogin,
					  Uint8 *outDesc,
					  Uint8 *outMessagebeforedl,
					  Uint8 *outPostbeforedl,
					  Uint8 *outAcceptPrivate,
					  Uint8 *outLoginMessageText,
					  Uint8 *outAdminLogin, 
					  Uint8 *outAgreementBanner,  
					  Uint8 *outAgreementBannerUrl, 
					  Uint8 *outAgreementUrl, 
					  CPtrList<BanRecord> *outPermBanList, 
					  SMyOptions& ioOptions);
	#elif NETWORK_SERVER
		void SetInfo( const Uint8 *inName, const Uint8 *inIncorrectlogin, const Uint8 *inMessagebeforedl, const Uint8 *inPostbeforedl, const Uint8 *inAcceptPrivate, const Uint8 *inLoginMessageText, const Uint8 *inDesc,
					  Uint8* inSerialNumber, const Uint8 *inAdminLogin, 
					  CPtrList<BanRecord> *inPermBanList, 
					  const SMyOptions& inOptions);
		void GetInfo( Uint8 *outName, Uint8 *outIncorrectlogin, Uint8 *outMessagebeforedl, Uint8 *outPostbeforedl, Uint8 *outAcceptPrivate, Uint8 *outLoginMessageText, Uint8 *outDesc,
					  Uint8* outSerialNumber, Uint8 *outAdminLogin, 
					  CPtrList<BanRecord> *outPermBanList, 
					  SMyOptions& ioOptions);
	#else
		void SetInfo( const Uint8 *inName, const Uint8 *inIncorrectlogin, const Uint8 *inMessagebeforedl, const Uint8 *inPostbeforedl, const Uint8 *inAcceptPrivate, const Uint8 *inLoginMessageText, const Uint8 *inDesc,
					  const Uint8 *inAdminLogin, 
					  CPtrList<BanRecord> *inPermBanList, 
					  const SMyOptions& inOptions);
		void GetInfo( Uint8 *outName, Uint8 *outIncorrectlogin, Uint8 *outMessagebeforedl, Uint8 *outPostbeforedl, Uint8 *outAcceptPrivate, Uint8 *outLoginMessageText, Uint8 *outDesc, 
					  Uint8 *outAdminLogin, 
					  CPtrList<BanRecord> *outPermBanList, 
					  SMyOptions& ioOptions);
	#endif
	
	#if NETWORK_SERVER
		void GetSerialNumber(Uint8 *outSerialNumber);
		void FocusSerialNumber();
	#endif
	
	#if BANNER_SERVER
		void UpdateBannerTab();
	#endif
		
		void SetAdminInfo(const Uint8 *inAdminLogin);
		void GetAdminInfo(Uint8 *outAdminLogin);
	
		
		void AddPermBan();
		void DeletePermBan();

		void SetTrackerAddress(Uint32 inIndex, const Uint8 *inStr)		{	mViews.trackerAddress[inIndex]->SetText(inStr+1, inStr[0]);				}
	#if NEW_TRACKER
		void SetTrackerLogin(Uint32 inIndex, const Uint8 *inStr)		{	mViews.trackerLogin[inIndex]->SetText(inStr+1, inStr[0]);				}
	#endif
		void SetTrackerPassword(Uint32 inIndex, const Uint8 *inStr)		{	mViews.trackerPassword[inIndex]->SetText(inStr+1, inStr[0]);			}
		void GetTrackerAddress(Uint32 inIndex, Uint8 *outStr) const		{	outStr[0] = mViews.trackerAddress[inIndex]->GetText(outStr+1, 255);		}
	#if NEW_TRACKER
		void GetTrackerLogin(Uint32 inIndex, Uint8 *outStr) const		{	outStr[0] = mViews.trackerLogin[inIndex]->GetText(outStr+1, 31);		}
	#endif
		void GetTrackerPassword(Uint32 inIndex, Uint8 *outStr) const	{	outStr[0] = mViews.trackerPassword[inIndex]->GetText(outStr+1, 31);		}

	protected:
		struct {
			CContainerView *vc;
			CTabbedView *tabs;
			CScrollerView *permBanScroll;
			CTextView *nameText, *descText, *banUrlText, *banDescrText, *maxDownText, *awaydelay, *bandelay, *incorrectlogin, *messagebeforedl, *postbeforedl, *acceptprivate, *loginmessagetext, *maxDownClientText, *maxConnectionsIPText, *portNumText;
			CTextView *ip1Text, *ip2Text, *ip3Text, *ip4Text;
			CCheckBoxView *useTracker, *logDownloads, *logUploads, *logConnects, *logmsgchat, *logmsgpvmsg, *logmsgpvmsg2, *logmsgbroad,
						   *confirmQuit, *csvMode, *botMode, *showKick, *logAccountChanges, *kickunnamedusers, *loginmessage;// *speakbeforedl;
			CLabelView *adminLbl;
			CMyPermBanListView *permBanList;
			
			CTextView *trackerAddress[10];
		#if NEW_TRACKER
			CTextView *trackerLogin[10];
		#endif
			CPasswordTextView *trackerPassword[10];
			
		#if NETWORK_SERVER
			CScrollerView *serialNumberScroll;
			CTextView *serialNumberText;
		#endif
		
		#if BANNER_SERVER
			CScrollerView *agreementBannerScroll, *agreementBannerUrlScroll, *agreementUrlScroll;
			CTextView *agreementBannerText, *agreementBannerUrlText, *agreementUrlText;
			CCheckBoxView *agreemLocal, *agreemRemote;
		#endif
		} mViews;
		
		CContainerView *MakeGeneralTab();
		CContainerView *MakeServInfoTab();
	#if BANNER_SERVER
		CContainerView *MakeBannerTab();
	#endif
		CContainerView *MakeBanTab();
		CContainerView *MakeTrackersTab();
		CContainerView *MakeAdminTab();
		CContainerView *MakeAdvancedTab();
		CContainerView *MakeMiscTab();
		CContainerView *MakeLoginMessageTab();
};

#pragma mark CMyToolbarWin
class CMyToolbarWin : public CWindow
{
	public:
		CMyToolbarWin();
		
#if SHOW_SERVER_INFO_IN_TOOLBAR
		void SetInfoText(const Uint8 inTxt[])			{	mInfoLbl->SetText(inTxt);	}
		void ToggleState();
		
	protected:
		CMyClickLabelView *mInfoLbl;
		Uint32 mTrueHeight;
#endif
};

#pragma mark CMySelectAdminWin
class CMySelectAdminWin : public CWindow
{
	public:
		CMySelectAdminWin();

		void SetInfo(const Uint8 *inAdminLogin);
		bool GetInfo(Uint8 *outAdminLogin);
				
	protected:
		struct {
			CMyAccountListView *accountListView;
		} mViews;
};

#pragma mark CMyNewAdminAccountWin
class CMyNewAdminAccountWin : public CWindow
{
	public:
		CMyNewAdminAccountWin();
				
		void SetInfo(const Uint8 *inAdminLogin);
		bool GetInfo(Uint8 *outAdminLogin, Uint8 *outAdminPassword);
		void UpdateButtons();
		
	protected:
		struct {
			CContainerView *containerView;
			CScrollerView *passwordScr1;
			CTextView *loginText;
			CPasswordTextView *passwordText1, *passwordText2;
			CButtonView *createBtn;
		} mViews;
};

#pragma mark CMySetAdminPassWin
class CMySetAdminPassWin : public CWindow
{
	public:
		CMySetAdminPassWin();
				
		void SetInfo(const Uint8 *inAdminLogin, const void *inMessage, Uint32 inMessageSize);
		bool GetInfo(Uint8 *outAdminPassword);
		void UpdateButtons();
		
	protected:
		struct {
			CContainerView *containerView;
			CScrollerView *passwordScr1;
			CLabelView *loginLbl, *messageLbl;
			CPasswordTextView *passwordText1, *passwordText2;
			CButtonView *saveBtn;
		} mViews;
};

#pragma mark CMySetAdminPrivsWin
class CMySetAdminPrivsWin : public CWindow
{
	public:
		CMySetAdminPrivsWin(const Uint8 *inLogin);						
};

#pragma mark CMyTextWin
class CMyTextWin : public CWindow
{
	public:
		CMyTextWin();
				
		CTextView *GetTextView()	{	return mViews.textView;						}
		void ScrollToBottom()		{	mViews.scrollerView->ScrollToBottom();		}
		
	protected:
		struct {
			CTextView *textView;
			CScrollerView *scrollerView;
		} mViews;
};

#pragma mark CMyBroadcastWin
class CMyBroadcastWin : public CWindow
{
	public:
		CMyBroadcastWin();
		
		THdl GetTextHandle()		{	return mViews.msgText->GetTextHandle();		}
		
	protected:
		struct {
			CTextView *msgText;
			CButtonView *sendBtn;
		} mViews;
};

#pragma mark CMyDownloadClientWin
class CMyDownloadClientWin : public CWindow
{
	public:
		CMyDownloadClientWin();						
};

#pragma mark CMyStatsWin
class CMyStatsWin : public CWindow
{
	public:
		CMyStatsWin();
		
		void SetStats(Uint32 inConnected, Uint32 inInvisible, Uint32 inDownloading, Uint32 inUploading, Uint32 inWaiting, Uint32 inConnPeak, Uint32 inConnCounter, Uint32 inDlCounter, Uint32 inUlCounter);
			
	protected:
		struct {
			CLabelView *connected, *invisible, *downloading, *uploading, *waiting, *connPeak, *connCounter;
			CLabelView *dlCounter, *ulCounter;
		} mStatLabels;
		
		struct {
			Uint32 connected, invisible, downloading, uploading, waiting, connPeak, connCounter;
			Uint32 dlCounter, ulCounter;
		} mStatValues;
};

#pragma mark CMyQuittingWin
class CMyQuittingWin : public CWindow
{
	public:
		CMyQuittingWin();
};

#pragma mark CMyQuitWin
class CMyQuitWin : public CWindow
{
	public:
		CMyQuitWin();
		
		void GetText(Uint8 *outText)	{	outText[0] = mViews.msgText->GetText(outText+1, 255);	}
	
	protected:
		struct {
			CTextView *msgText;
		} mViews;
};


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
#pragma mark ¥¥ Application ¥¥

#pragma mark SMyConnect
struct SMyConnect
{
	SMyConnect *next;
	TTransact tpt;
	Uint32 ipAddress;
};

#pragma mark SMyClient
struct SMyClient
{
	SMyClient *next;
	TTransact tpt;
	SMyUserAccess access;
	Uint32 ipAddress;
	Uint16 userID;
	Int16 iconID;
	Uint8 logon[33];
	Uint8 realName[32];		// the name received from client
	Uint8 userName[32];		// can be realName if has priv myAcc_AnyName or accountName otherwise 
	Uint8 accountName[32];	// account name (read from account file)
	Uint8 accountLogin[32];	// login name (read from account file)
	Uint32 loginFailedSecs, activitySecs, lastMsgSecs, nofloodChatSecs, nofloodNameSecs, nofloodTranSecs;

	bool bRefusePrivateMsg;
	bool bRefusePrivateChat;
	bool bAutomaticResponse;
	Uint8 psAutomaticResponseText[128];

	Uint32 nSecsOnline;
	Uint32 vers;	// version of the client

#if AUTO_BAN_MULTI_PRIV_MSGS
	// tg priv msg ban if duplicate
	Uint32 lastPrivMsgCRC;
	Uint16 dupePrivMsgCount;	// count of msgs that have had equal crcs
#endif

	Uint16 privMsgCount, dontChatCount, nofloodChatSize, nofloodNameCount, nofloodTranCount;
	Uint16 numdl, numul, numflddl, numfldul;
	Uint16 hasLoggedIn	: 1;
	Uint16 isAway		: 1;
	Uint16 isDownloading		: 1;
	Uint16 isFakeRed		: 1;
	Uint16 isVisible		: 1;
	Uint16 isBlockDownload		: 1;
	Uint16 isChatting		: 1;
	Uint16 isPostting		: 1;
	
	bool HasAccess(Uint32 inPriv) const		{	return access.HasPriv(inPriv);		}
};

#pragma mark CMyApplication
class CMyApplication : public CApplication
{
	// friend classes
	friend class CMyAccountListView;

	public:
		CMyApplication();
		virtual ~CMyApplication();
		void StartUp();
	
		// application functions
		virtual void WindowHit(CWindow *inWindow, const SHitMsgData& inInfo);
		virtual void Error(const SError& inInfo);
		virtual void UserQuit();
		virtual void Quit();
		virtual void HandleMessage(void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);
		virtual void Timer(TTimer inTimer);
		virtual void KeyCommand(Uint32 inCmd, const SKeyMsgData& inInfo);
		virtual void ShowPreferences();
		
		void DisplayStandardMessage(Uint8 *inTitle, Uint8 *inMsg, Int16 inIcon = 0, Int16 inSound = 0);
		
	protected:
		bool DisplayCreateAdmin();
		bool DisplayAccountExists(const Uint8 *inLogin);
		bool DisplaySetAdminPrivs(const Uint8 *inLogin);
		bool DisplayDownloadClient();
		
	protected:
		
		// misc data
		THdl mMsgData;
		
		THdl mServerAgreement;
		
		THdl mServerBanner;
		Uint32 mServerBannerType;
		THdl mServerBannerUrl;

		StTransact mListenTpt, mListenHttpTpt;
		StTransport mTransferListenTpt, mTransferListenHttpTpt;
		StTransport mTrackerTpt;
		TTimer mTrackerTimer;
			
		CLinkedList mConnectList;
		CLinkedList mClientList;
		CPtrList<SMyDownloadData> mDownloadList;
		CPtrList<SMyDownloadFldrData> mDownloadFldrList;
		CPtrList<SMyDownloadWaitingData> mDownloadWaitingList;
		CPtrList<SMyDownloadBannerData> mDownloadBannerList;
		CPtrList<SMyUploadData> mUploadList;
		CPtrList<SMyUploadFldrData> mUploadFldrList;
		CPtrList<TTransportObj> mTransferEstabList;
		CMyStatsWin *mStatsWin;
		CMyTextWin *mLogWin;
		CMyToolbarWin *mToolbarWin;
		CMyQuittingWin *mQuittingWin;
		
		SMyOptions mOptions;
		TFSRefObj *mUsersFolder;
		TFSRefObj *mFilesFolder;
		TFSRefObj *mNewsFolder;
		Uint16 mErrorCount;
		Uint16 mResetCount;
		Uint32 mFirstErrorTime;
		Uint16 mRefNumCounter;
		Uint32 mIsQuitting		: 1;
		
		Uint8 mServerName[32];
		Uint8 mIncorrectlogin[128];
		Uint8 mMessageBeforedl[128];
		Uint8 mPostBeforedl[128];
		Uint8 mAcceptPrivate[128];
		//Uint8 mLoginMessageText[128];
		Uint8 mLoginMessageText[128];
		Uint8 mServerDesc[128];
		
		Uint8 mAdminLogin[32];
	#if BANNER_SERVER
		Uint8 mAgreementBanner[128];
		Uint8 mAgreementBannerUrl[256];
		Uint8 mAgreementUrl[256];
	#endif
		Uint16 mUserIDCounter;
		Uint32 mLastConnectAddress, mLastConnectTime, mLastConnectCount;

		Uint32 mCommunityID;
	#if NETWORK_SERVER
		Uint8 mSerialNumber[32];
		Uint16 mSimConnections;
	#endif
		
		// chat data
		CLinkedList mChatList;
		Uint8 mChatIDCounter;
		
		// statistical data
		struct {
			Uint32 connCount, invisible, connPeak, connCounter, dlCounter, ulCounter;
		} mStats;

	
		// ban data
		CPtrList<BanRecord> mTempBanList;
		CPtrList<BanRecord> mPermBanList;
		
		// tracker data
		Uint32 mTrackerPassID;
		SMyTrackInfo mTrackInfo[10];

		bool IsValidClient(SMyClient *inClient)	
		{	return mClientList.IsInList((CLink *)inClient);	}

		void DoShowLog();
		void DoShowStats();
		void DoPoliteQuit();
		bool DoOptions();
		void DoCancelQuit();
		void DoServerBroadcast();
		void DoAdminConnect();
		void DoCloseWindow(CWindow *inWin);
		void DoMacMenuBar(Int16 inMenu, Int16 inItem);
	
	#if NETWORK_SERVER
		bool ValidateSerialNumber(Uint8 *inSerialNumber);
		bool DecodeSerialNumber(Uint8 *inSerialNumber, Uint32& outCommunityID, Uint16& outSimConnections);
	#endif
				
		void ShowQuittingWin();
		void HideQuittingWin();
		
	#if !FACELESS
		void UpdateDisplay();
	#endif
	
	#if SHOW_SERVER_INFO_IN_TOOLBAR
		void UpdateToolbarInfo();
	#endif
		
		TFSRefObj* GetPrefsSpec();
		void WritePrefs();
		void ReadPrefs(SRect *outWinRects, Uint32 *outWinVis);

		TFSRefObj* GetPermBanSpec();
		void MakePermBanBackup();
		void WritePermBanInfo(CPtrList<BanRecord> *inPermBanList);
		void ReadPermBanInfo(CPtrList<BanRecord> *outPermBanList);
		void ReadPermBanList();
		void ClearPermBanList();

	#if !FACELESS
		void Log(const Int8 inFormat[], ...);
		void Log(SMyClient *inClient, const Int8 inFormat[], ...);
		void Log(TTransport inTpt, const Int8 inFormat[], ...);
		void Log(SMyDownloadWaitingData *inDownloadWaiting, const Int8 inText[]);
		void LogText(const Uint8 inContextName[], const Uint8 inText[]);
	#endif
		
		void LogDownload(SMyClient *inClient, const Uint8 inFileName[]);
		void LogUpload(SMyClient *inClient, const Uint8 inFileName[]);
		void LogConnect(SMyClient *inClient);
		void LogAccountChange(SMyClient *inClient, const Uint8 *inAction, const Uint8 *inAffectedAccount);

		Uint32 NewRefNum();
		void SetOnlineAccount(const Uint8 *inLogin, const SMyUserDataFile& inUserInfo);
		void DeleteOnlineAccount(const Uint8 *inLogin, SMyClient *inClient);
		void UpdateTracker();
		
		TFSRefObj* GetMsgBoardFileSpec();
		void LoadMsgBoard();
		Uint32 AddToMsgBoard(TFieldData inData, const Uint8 inUserName[]);
		
		void BuildFileList(TFSRefObj* inFolder, bool inIsDropBox, TFieldData outData);
		void BuildNewsCatList(TFSRefObj* inFolder, TFieldData outData, Uint32 inClientVers);
		bool BuildNewsArtList(TFSRefObj* inFile, TFieldData outData);
		bool BuildNewsArtData(TFSRefObj* inFile, Uint32 inArticleID, const Int8 inFlav[], TFieldData outData);
		
		void LoadAgreement(bool inReload = false);
		bool LoadAgreement(const Uint8 *inAgreementFileName, bool inReload, THdl& outAgreement, Uint32& outTypeCode);
		bool LoadUrl(const Uint8 *inAgreementUrl, THdl& outUrl);

		THdl GetUserFolderList();
		void NewUser(const SMyUserDataFile& inUserInfo);
		void DeleteUser(const Uint8 *inLogin);
		void GetUser(const Uint8 *inLogin, SMyUserDataFile& outUserInfo);
		
		
		void RenameUser(const Uint8 *inLogin, const SMyUserDataFile& inUserInfo);
		void SetUser(const SMyUserDataFile& inUserInfo);
		bool ExistsUser(const Uint8 *inUserLogin);
		bool ExistsUser(const SMyUserDataFile& inUserInfo);
		bool SetAdmin(const Uint8 *inLogin, const Uint8 *inPassword, bool inSetAccess = true);
		TFSRefObj* GetClientRootFolder(SMyClient *inClient);
		TFSRefObj* GetClientRootNewsFolder(SMyClient *inClient);

		bool CheckAdminAccount();
		bool SetAdminAccount();
		bool SelectAdminAccount(Uint8 *ioAdminLogin);
		bool CreateLoginAdminAccount(Uint8 *outAdminLogin);
		bool CreatePasswordAdminAccount(Uint8 *outAdminPassword = nil);
		bool ResetAdminAccount(const Uint8 *inAdminLogin);
		bool ValidateAdminAccount(Uint8 *ioAdminLogin);


		bool sendchat;

		
		bool HasGeneralPriv(const SMyClient *inClient, const Uint32 inPriv);
		bool HasFolderPriv(const SMyClient *inClient, const Uint32 inPriv);
		bool HasBundlePriv(const SMyClient *inClient, const Uint32 inPriv);
		
		void SetClientUserInfo(SMyClient *inClient, const Uint8 *inName, const SMyUserDataFile& inUserInfo);
		void CheckName(SMyClient *inClient);
		
		void SetClientUserInfo(SMyClient *inClient, TFieldData inData, const SMyUserDataFile *inUserInfo = nil);
		void BroadcastClientUserInfo(SMyClient *inClient);
		Uint16 GetClientFlags(SMyClient *inClient);
		void SetClientAway(SMyClient *inClient, bool inAway);
		void SetClientDownloading(SMyClient *inClient, bool inDownloading);
		void SetClientFakeRed(SMyClient *inClient, bool inFakeRed);
		void SetClientBlockDownload(SMyClient *inClient, bool inBlockDownload);
		void SetClientVisible(SMyClient *inClient, bool inVisible);
		
		
		void SetClientChatting(SMyClient *inClient, bool inChatting);
		void SetClientPostting(SMyClient *inClient, bool inPostting);
		
		void SendClientDownloadInfo(SMyClient *inClient, Uint32 inRefNum, Uint32 inWaitingCount);

		void ResetServer();
		void DeleteDownloadWaiting(SMyDownloadWaitingData *inDownloadWaiting);
		template <class T> bool SearchWin();
		
		bool IsPermBanned(Uint32 inAddr);
		bool IsPermBannedName(SMyClient *inClient);
		
		bool IsTempBanned(Uint32 inAddr);
		void AddPermBan(Uint32 inAddr1,Uint32 inAddr2, Uint8 *inDescr);
		void AddTempBan(Uint32 inAddr);
		void RemoveOldTempBans();
		void DisconnectPermBan();
		Uint32 GetClientAddressCount(Uint32 inAddr);
		Uint16 GetClientDownloadsNumber(Uint16 inClientID);

		SMyChat *GetChatByID(Uint32 inID);
		void SendTextToChat(Uint32 inChatID, const void *inText, Uint32 inTextSize);
		void BroadcastToChat(SMyChat *inChan, Uint16 inType, TFieldData inData);
		Uint32 NewUniqueChatID();

		SMyClient *GetClientByID(Uint16 inID);
		void KillClient(SMyClient *inClient);
		void BroadcastTran(Uint16 inType, TFieldData inData);
		void BroadcastTranForInvisible(Uint16 inType, TFieldData inData);
		void BroadcastTranForVisible(Uint16 inType, TFieldData inData);
		
		void ProcessClients();
		void ProcessTransferEstab();
		void ProcessDownloads();
		void ProcessFldrDownloads();
		void ProcessDownloadsWaiting();
		void ProcessBannerDownloads();
		void ProcessUploads();
		void ProcessFldrUploads();
	public:	
		SMyDownloadData *GetDownloadByRefNum(Uint32 inRefNum);
		SMyDownloadFldrData *GetDownloadFldrByRefNum(Uint32 inRefNum);
		SMyDownloadBannerData *GetDownloadBannerByRefNum(Uint32 inRefNum);
		bool KillDownloadByRefNum(SMyClient *inClient, Uint32 inRefNum);
		SMyUploadData *GetUploadByRefNum(Uint32 inRefNum);
		SMyUploadFldrData *GetUploadFldrByRefNum(Uint32 inRefNum);
		bool FindResumableFile(TFSRefObj* inRef);
		
		Uint32 tempbantimeout;
   protected:
		void ProcessTran_GetMsgs(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_PostMsg(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_SendInstantMsg(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_UserBroadcast(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_ChatSend(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_IconChange(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_NickChange(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_Flood(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		
		void ProcessTran_FakeRed(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_Visible(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_BlockDownload(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		
		void ProcessTran_Away(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_AdminSpector(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		
		void ProcessTran_GetFileNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetNewsCatNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetNewsArtNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetNewsArtData(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_PostNewsArt(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DelNewsArt(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DelNewsFldrItem(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_NewNewsFldr(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_NewNewsCat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetUserNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_Login(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_Agreed(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DownloadFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DownloadFldr(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DownloadBanner(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_KillDownload(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_UploadFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_UploadFldr(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DeleteFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_NewFolder(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_Unknown(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_SendMessage(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		
		void ProcessTranDont_Chat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTranDont_Post(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_NewUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DeleteUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_SetUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetUserList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_SetUserList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_DisconnectUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetClientInfoText(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_SetClientUserInfo(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_GetFileInfo(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_SetFileInfo(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_MoveFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_MakeFileAlias(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_InviteNewChat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_RejectChatInvite(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_JoinChat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_LeaveChat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_SetChatSubject(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_InviteToChat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
		void ProcessTran_KeepConnectionAlive(SMyClient *inClient, TTransactSession inTsn, TFieldData inData);
};

extern CMyApplication *gApp;
extern HLCrypt *gCrypt;

