/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#pragma def_inherited on

#define NEW_NEWS 				1
#define USE_ADD_SN 				1
#define NO_DEFAULT_TRACKER		0
#define NEW_SPLASH				1
#define USE_NEWS_HISTORY		1
#define USE_LARGE_FONT			0
#define DRAW_USERLIST_ICONS		1
#define DISABLE_TABS			0
#define EXTRA_BANNER_BUTTONS	0
#define USE_HELP				0

#define BANNER_AUTO_REFRESH_MS  0

#define kClientVersion	"197"



#include "HotlineClientServerCommon.h"
#include "HotlineFolderDownload.h"

#if WIN32
	#define LIST_BACKGROUND_OPTION		0
	#define MIN_WINDOW_WITDH			108
#elif MACINTOSH
	#define LIST_BACKGROUND_OPTION		scrollerOption_NoBkgnd
#endif


#pragma mark ₯₯ Constants ₯₯



//messages defined by shea
enum {
msgProtocolError				= 4000
};




#pragma mark View IDs
enum {
	viewID_Connect				= 1001,
	viewID_Disconnect			= 1002,
	viewID_Options				= 1003,
	viewID_ChatSend				= 1004,
	viewID_ShowChat				= 1005,
	viewID_ShowFiles			= 1006,
	viewID_Quit					= 1007,
	viewID_ShowNews				= 1008,
	viewID_ShowTasks			= 1009,
	viewID_ShowUsers			= 1010,
	viewID_OpenPrivateChat		= 1011,
	viewID_StopTask				= 1012,
	viewID_ShowAbout			= 1013,
	viewID_SendPrivMsg			= 1014,
	viewID_CustomToolbar		= 1015,
	viewID_UserList				= 1016,
	viewID_SaveEditUser			= 1017,
	viewID_CancelEditUser		= 1018,
	viewID_DeleteEditUser		= 1019,
	viewID_NewUser				= 1020,
	viewID_OpenUser				= 1021,
	viewID_DisconnectUser		= 1022,
	viewID_DismissPrivMsg		= 1023,
	viewID_ReplyPrivMsg			= 1024,
	viewID_GetInfo				= 1025,
	viewID_ConnectToTracked		= 1026,
	viewID_Refresh				= 1027,
	viewID_TrackServList		= 1028,
	viewID_ShowServers			= 1029,
	viewID_BookmarkTracked		= 1030,
	viewID_StartTask			= 1031,
	viewID_BringFrontPrivMsg	= 1032,
	viewID_Secret				= 1033,
	viewID_CloseWindow			= 1034,
	viewID_PrivChatSend			= 1035,
	viewID_AcceptChatInvite		= 1036,
	viewID_RejectChatInvite		= 1037,
	viewID_ChatUserList			= 1038,
	viewID_ChatSubject			= 1039,
	viewID_CloseAllFileWindows	= 1040,
	viewID_Broadcast			= 1041,
	viewID_Flood				= 1042,
	viewID_BannerNext			= 1043,
	viewID_WindowNext			= 1044,
	viewID_ServerInfo			= 1045,
	viewID_ServerBookmark		= 1046,
	
	iconID_HelpToolbar			= 428,
	viewID_HelpToolbar			= 1047,
	viewID_HelpTasks			= 1048,
	viewID_HelpNews				= 1049,
	viewID_HelpFiles			= 1050,
	viewID_HelpUsers			= 1051,
	viewID_HelpChat				= 1052,
	viewID_HelpServers			= 1053,
	viewID_HelpPrivateChat		= 1054,
	viewID_HelpPrivateMessage	= 1055,
	viewID_HelpOptions			= 1056,

	iconID_OpenDloadFolder		= 430,
	viewID_OpenDloadFolder		= 1057,

	viewID_HelpBroadcast		= 1058,

	viewID_ToolbarStatus		= 1060,
	iconID_ShowMsgBoard			= 429,
	viewID_ShowMsgBoard			= 1061,
	
	iconID_UnusedAD				= 2002,
	viewID_UnusedAD				= 1062,
	iconID_ISP					= 142,
	viewID_ISP					= 1063,
	iconID_Securiphone			= 142,
	viewID_Securiphone			= 1064,
	iconID_Xsprings				= 142,
	viewID_Xsprings				= 1065,
	
	viewID_OpenUserList			= 1070,
	viewID_AdmInSpector			= 1071,
	
	viewID_UserInfoOpenUser		= 1090,
	
	viewID_FileView				= 1099,
	viewID_FileDownload			= 1100,
	viewID_FileUpload			= 1101,
	viewID_Delete				= 1102,
	viewID_FileList				= 1104,
	viewID_New					= 1105,
	viewID_FileSaveInfo			= 1107,
	viewID_FileInfoChange		= 1108,
	viewID_OpenParent			= 1109,
	viewID_FileTree				= 1110,
	viewID_FileExplTree			= 1111,
	viewID_FileExplList			= 1112,
	
	viewID_OldNewsPost			= 1140,
	
	viewID_NewsCatOpen			= 1150,
	viewID_NewsCatReply			= 1152,
	viewID_NewsCatTree			= 1155,
	viewID_NewArticle			= 1156,

	viewID_NewsFldrNewCat		= 1161,
	viewID_NewsFldrList			= 1166,
	viewID_NewsFldrTree			= 1167,
	viewID_NewsFldrExplTree		= 1168,
	viewID_NewsFldrExplList		= 1169,

	viewID_NewsArticGoPrev		= 1170,
	viewID_NewsArticGoNext		= 1171,
	viewID_NewsArticReply		= 1172,
	viewID_NewsArticSend		= 1173,
	viewID_NewsArticText		= 1174,
	viewID_NewsArticTitle		= 1175,
	viewID_NewsArticGoParent	= 1176,
	viewID_NewsArticGo1stChild	= 1177,
	viewID_NewsArticTrash		= 1178,

	viewID_AdmInSpectorClose	= 1179,
	viewID_AdminClose			= 1180,
	viewID_AdminRevertUsers		= 1181,
	viewID_AdminSaveUsers		= 1182,
	viewID_AdminNewUser			= 1183,
	viewID_AdminOpenUser		= 1184,
	viewID_AdminDeleteUser		= 1185,
	viewID_AdminUserTree		= 1186,
	viewID_SendFakeRed			= 1087,
	viewID_SendAway				= 1088,
	viewID_ChangeIcon			= 1089,
	viewID_SendVisible			= 1092,
	viewID_BlockDownload		= 1091,
	viewID_GetAdminSpectorInfo	= 1093
	
	
};

enum {
#if USE_LARGE_FONT
	#if MACINTOSH
		kMyDefaultFixedFontSize		= 16
	#else
		kMyDefaultFixedFontSize		= 18
	#endif
#else
	#if MACINTOSH
		kMyDefaultFixedFontSize		= 9
	#else
		kMyDefaultFixedFontSize		= 12
	#endif
#endif // USE_LARGE_FONT
};

enum
{
	hlNewsFlav_plain_text		= 1,
	hlNewsFlav_image_jpeg		= 10,
	hlNewsFlav_image_gif		= 11
};

#pragma mark listStatus

enum
{
	listStat_Loading			= 0,
	listStat_Hide				= 1,
	listStat_0Items				= 2,

	listStat_Custom				= 100
};

const Uint8 statTxt_UserlistNotOnline[] 	= "\pYou are not connected.\r\rClick the \"Connect\" or \"Servers\" button to connect to a Server";
const Uint8 statTxt_ServersList0Items[]		= "\p0 items in list.\r\rClick the \"Refresh\" button to retrieve Server listings from the enabled Trackers.\r\rYou can enable existing Trackers or add new Trackers by clicking the \"Edit Tracker List\" button.";

enum {
	optWindow_List				= 0,
	optWindow_Tree				= 1,
	optWindow_Expl				= 2
};

enum {
	kKeepConnectionAliveTime	= 180000	// 3 minutes
};

// versions
enum {
	kMyPrefsVersion				= 7,
	kMyBuildDataVersion			= 1
};

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Structures ₯₯

// SMySoundPrefs
#pragma options align=packed
struct SMySoundPrefs {
	Uint8 magneticWindows	: 1;	// keep this here for prefs file compatibility
	Uint8 rsvd[15];
	Uint32 playSounds		: 1;
	Uint32 chat				: 1;
	Uint32 news				: 1;
	Uint32 fileTransfer		: 1;
	Uint32 privMsg			: 1;
	Uint32 join				: 1;
	Uint32 leave			: 1;
	Uint32 login			: 1;
	Uint32 error			: 1;
	Uint32 chatInvite		: 1;
};
#pragma options align=reset

//SMyColorInfo
#pragma mark SMyColorInfo
#pragma options align=packed
struct SMyColorInfo{
	SColor textColor;
	SColor backColor;
	Uint16 textSize;
};
#pragma options align=reset

// SMyDefTrackerInfo
#pragma mark SMyDefTrackerInfo
struct SMyDefTrackerInfo {
	Uint8 psName[64];
	Uint8 psAddr[64];
};

//SMyOptions
#pragma mark SMyOptions
struct SMyOptions {
	SMySoundPrefs SoundPrefs;
	bool bShowPrivMsgAtBack;
	bool bQueueTransfers;
	bool bShowJoinLeaveInChat;
	bool bShowDateTime;
	bool bShowSplash;
	bool bShowBanner;
	bool bOriginalIconSet;
	bool bFampastelIconSet;
	bool bBrowseFolders;
	bool bToolbarButtonsTop;
	bool bMagneticWindows;
	bool bRefusePrivateMsg;
	bool bRefusePrivateChat;
	bool bRefuseBroadcast;
	bool bAutomaticResponse;
	bool bcancelAutorespTime;
	bool bAutoAgree;
	bool bLogMessages;
	bool bShowTooltips;
	bool bTunnelThroughHttp;
	bool bDontSaveCookies;
	Uint8 nFilesWindow;
	Uint8 nNewsWindow;
	Uint8 psAutomaticResponse[128];
	Uint8 pscancelAutorespTime[128];
	
	SMyColorInfo ColorPublicChat;
	SMyColorInfo ColorPrivateChat;
	SMyColorInfo ColorNews;
	SMyColorInfo ColorViewText;

	bool bCustomDist;
	bool bAutoconnectFirstLaunch;
	bool bAutoconnectEveryLaunch;
	Uint32 nDistribID;
	Uint8 psBuildName[64];
	Uint8 psAutoconnectName[64];
	Uint8 psAutoconnectAddr[64];
	CPtrList<SMyDefTrackerInfo> stTrackerList;

	struct {
		SRect stAdmin, stAdmInSpector;
		SRect stFiles1, stFiles2;
		SRect stNews1, stNews2;
		SRect stArticles;
	} stWinRect;

	struct {
		Uint8 nAdminPane1, nAdmInSpectorPane1;
		Uint8 nFilesPane1;
		Uint8 nNewsPane1, nNewsPane2;
	} stWinPane;
	
	struct {
		Uint8 nAdminTab1, nAdminTab2, nAdmInSpectorTab1, nAdmInSpectorTab2;
		Uint8 nFilesTab1, nFilesTab2, nFilesTab3, nFilesTab4;
		Uint8 nNewsTab1, nNewsTab2;
		Uint8 nArticlesTab1, nArticlesTab2, nArticlesTab3, nArticlesTab4, nArticlesTab5;
	} stWinTabs;
};

// SMyPrefs
#pragma mark SMyPrefs
#pragma options align=packed
struct SMyPrefs {
	Uint16 version;
	struct {
		Uint16 toolbar			: 1;
		Uint16 tasks			: 1;
		Uint16 news				: 1;
		Uint16 chat				: 1;
		Uint16 files			: 1;
		Uint16 users			: 1;
		Uint16 tracker			: 1;
	} winVis;
	struct {
		SRect toolbar;
		SRect tasks;
		SRect news1;
		SRect chat;
		SRect files1;
		SRect users;
		SRect tracker;
		SRect news2;
		SRect files2;
		SRect articles;
	} winRect;
	SMySoundPrefs sound;
	Int16 iconID;
	Uint16 iconRand1;
	Uint32 iconRand2;
	Uint32 iconCRC;
	Uint32 totalUploaded, totalDownloaded, totalOnline, totalChat;
	Uint32 showPrivMsgAtBack	: 1;
	Uint32 queueTransfers		: 1;
	Uint32 showJoinLeave		: 1;
	Uint32 showDateTime			: 1;
	Uint32 showSplash			: 1;
	Uint32 showBanner			: 1;
	Uint32 originalIconset		: 1;
	Uint32 fampastelIconset		: 1;
	Uint32 browseFolders		: 1;
	Uint32 toolbarButtonsTop	: 1;
	Uint32 refusePrivateMsg 	: 1;    
	Uint32 refusePrivateChat 	: 1;
	Uint32 refuseBroadcast	 	: 1;
	Uint32 isAutomaticResponse  : 1;
	
	Uint32 autoAgree	  		: 1; 
	
	Uint32 logMessages  		: 1;   
	

	Uint8 automaticResponse[128]; 
	Uint8 cancelAutorespTime[128];
	Uint8 userName[33];
		
	struct {
		SRect admin;
		Uint8 adminPane1;
		Uint8 adminTab1, adminTab2;
	} winAdmin;
	
	struct {
		SRect adminspector;
		Uint8 adminspectorPane1;
		Uint8 adminspectorTab1, adminspectorTab2;
	} winAdmInSpector;
	
	Uint8 rsdv1[43];

	Uint8 rsdv2[66];

	SMyColorInfo colorPublicChat; 
	SMyColorInfo colorPrivateChat; 
	SMyColorInfo colorNews; 
	SMyColorInfo colorViewText; 
	
	Uint8 firewall[128];
	Uint8 unlockName[32];
	Uint8 unlockCode[32];
	Uint8 showTooltips			: 1;
	Uint8 magneticWindows		: 1;
	Uint8 tunnelThroughHttp		: 1;
	Uint8 filesWindow;
	Uint8 newsWindow;
	Uint8 dontSaveCookies		: 1;

	struct {
		Uint8 filesPane1;
		Uint8 newsPane1, newsPane2;
	} winPane;

	struct {
		Uint8 filesTab1, filesTab2, filesTab3, filesTab4;
		Uint8 newsTab1, newsTab2;
		Uint8 articlesTab1, articlesTab2, articlesTab3, articlesTab4, articlesTab5;
		Uint8 usersTab1, usersTab2;
		Uint16 trackerTab1, trackerTab2;
	} winTabs;
	
	Uint8 rsvd2[7];
};
#pragma options align=reset

// SMyCustomInfo
#pragma mark SMyCustomInfo
#pragma options align=packed
struct SMyCustomInfo {
	Uint32 sig;		// 'htag'
	Uint16 version;	// 1
	Uint16 rsvdA;
	Uint32 rsvdB[2];
	
	Uint8 autoconnectFirstLaunch;
	Uint8 autoconnectEveryLaunch;
	Uint8 rsvdC[2];
	
	Uint32 distribID;
	Uint8 buildName[64];
	Uint8 autoconnectName[64];
	Uint8 autoconnectAddr[64];
};
#pragma options align=reset

// SMyUserInfo
#pragma mark SMyUserInfo
#pragma options align=packed
struct SMyUserInfo {
	Uint16 id;
	Int16 iconID;
	Uint16 flags;
	Uint16 nameSize;
	Uint8 nameData[];
};
#pragma options align=reset

// SMyFileInfo
#pragma mark SMyFileItem
struct SMyFileItem {
	TIcon icon;
	Uint32 type, creator;
	Uint32 fileSize;
	Uint32 nameSize;
	Uint8 name[];
};

// SMyCacheListInfo
#pragma mark SMyCacheListInfo
struct SMyCacheListInfo {
	TPtr pPathData;
	Uint32 nPathSize;
	Uint32 nPathCheckSum;
	StFieldData pListData;
	bool bChanged;
};

// SMyTrackServInfo
#pragma mark SMyTrackServInfo
#pragma options align=packed
struct SMyTrackServInfo {
	Uint32 addr;
	Uint16 port;
	Uint8 userCount[8];	// text
	Uint8 data[];		// name, desc
};
#pragma options align=reset

// SMyToolbarItem
#pragma mark SMyToolbarItem
#pragma options align=packed
struct SMyToolbarItem {
	Int16 top, left, bottom, right;
	Int16 mouseDownPict, mouseWithinPict;
	Int16 mouseDownSound, mouseUpSound;
	Int16 mouseEnterSound, mouseLeaveSound;
	Uint32 mouseDownAction, mouseUpAction;
	Int16 disabledPict;
	Int16 winPict;
	Int16 dtop, dleft, dbottom, dright;
	Int16 wtop, wleft, wbottom, wright;
};
#pragma options align=reset

// SMyPictToolbarInfo
#pragma mark SMyPictToolbarInfo
#pragma options align=packed
struct SMyPictToolbarInfo {
	Uint32 format;
	Uint16 version;
	Uint16 sideTitlebar : 1;
	Uint16 toggleWinVis : 1;
	Uint16 rsvdFlags : 14;
	Uint32 rsvd;
	Int16 windowPat;
	Int16 connectedPict;
	Int16 ctop, cleft, cbottom, cright;
	Uint16 height, width;
	Int16 bkgndPict;
	Uint16 itemCount;
	SMyToolbarItem items[];
};
#pragma options align=reset

// SMyServerConfig
#pragma mark SMyServerConfig
#pragma options align=packed
struct SMyServerConfig {
	Uint32 format;			// 'HTsc'
	Uint16 version;			// currently 1
	Uint8 rsvd[128];
	Uint16 loginSize;
	Uint8 loginData[32];
	Uint16 passwordSize;
	Uint8 passwordData[32];
	Uint16 addressSize;
	Uint8 addressData[256];
	bool useCrypt;
};
#pragma options align=reset

// SMySaveConnect
#pragma mark SMySaveConnect
struct SMySaveConnect {
	Uint8 psAddress[256];
	Uint8 psServerName[256];
	Uint8 psLogin[33]; 
	Uint8 psPassword[33];
	bool bStartupInfo;
	bool bUseCrypt;
};

// SMyTaskNameInfo
class CMyTask;
#pragma mark SMyTaskNameInfo
struct SMyTaskNameInfo {
	CMyTask *task;
	Uint32 progVal, progMax;
	Uint32 textSize;
	Uint8 *textData;
};

// SMyNewsArticle
#pragma mark SMyNewsArticle
#pragma options align=packed
struct SMyNewsArticle
{
	Uint32 id;
	SDateTimeStamp dts;
	Uint32 flags;
	Uint32 size;		// total byte size of this item
	Uint16 flavCount;
	Uint16 flavors;
	Uint8 read : 1;		// read or unread
	Uint8 title[32];	// pstring   
	Uint8 poster[32];	// pstring

};
#pragma options align=reset

// SNewsCatItm
#pragma mark SNewsCatItm
struct SNewsCatItm
{
	TIcon icon;
	SGUID guid;
	Uint32 addSN;
	Uint32 delSN;
	Uint16 count;
	Uint8 unread	: 1;
	Uint8 type;
	Uint8 name[];

};

// SMyNzHistCatGUIDItm
class CNZReadList;
#pragma mark SMyNzHistCatGUIDItm
struct SMyNzHistCatGUIDItm
{
	SGUID guid;			// in network byte order
	Uint32 fileOffset;
	Uint32 accessSN;
#if USE_ADD_SN
	Uint32 addSN;
	Uint32 servAddSN;
#endif
	CNZReadList *lst;
};

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Miscellaneous ₯₯

#if USE_NEWS_HISTORY
#pragma mark CNZReadList
class CNZReadList
{
	public:
		CNZReadList(TPtr inData, Uint32 inDataSize);
		virtual ~CNZReadList();
		
		void AddRef()												{	mRefs++;						}
		void Release()												{	if(!--mRefs) delete this;		}
		
		bool CheckRead(Uint32 inID);
		void SetRead(Uint32 inID, bool inRead = true);
		
		void PurgeUnchecked();	// removes all ids in the list that we haven't checked
		void ResetChecked();	// sets all items to unchecked
		
		Uint32 *GetDataPtr(Uint32 &outSize)							{	outSize = mIDCount * sizeof(Uint32); return mID;	}
		bool HasChanged()											{	return mHasChanged;				}
		bool AllItemsRead()											{	return mIDCount == mTotalCount;	}
		
		void SetTotalCount(Uint32 inCount)							{	mTotalCount = inCount;			}
		Uint32 GetTotalCount()										{	return mTotalCount;				}
		
	protected:
		Uint32 mRefs;
		Uint32 *mID;
		Uint32 mIDCount;
		Uint32 mIDDataSize;
		Uint32 mTotalCount;
		
		CBoolArray mCheckedList;
		Uint8 mHasChanged	: 1;
};

#pragma mark CMyNewsReadHistory
class CMyNewsReadHistory
{
	#define GetByteOffset(o, b)		(512 + (o - 1) * b)

	public:
		CMyNewsReadHistory(TFSRefObj* inRef);
		virtual ~CMyNewsReadHistory();
		
		CNZReadList *GetReadList(const SGUID& inGUID);
		bool IsNewAddSN(const SGUID& inGUID, Uint32 inSN);
		void PurgeToDisk();

	protected:
		Uint32 ReadFile(Uint32 inBlock, Uint32 inOffset, void *outData, Uint32 inMaxSize)		{	return mFile.ref ? mFile.ref->Read((512 + (inBlock - 1) * mFile.blockSize) + inOffset, outData, inMaxSize) : 0;	}
		
		void PurgeOldest(Uint32 inCount);
		void DeallocateIDs(Uint32 inOffset);
		void Deallocate(Uint32 inOffset, Uint32 inCount);
		Uint32 Allocate(Uint32 inCount);
		
		struct
		{
			TFSRefObj* ref;
			Uint32 blockSize;
			Uint32 allocTabOffset;
			Uint32 allocTableSize;
			Uint32 allocTabFirstFree;
			Uint32 catOffset;
		} mFile;
		
		Uint32 mNextSN;
		Uint32 mCatMaxCount;
	
		CPtrList<SMyNzHistCatGUIDItm> mList;
};
#endif

#pragma mark CMyCacheList
class CMyCacheList
{
	public:
		CMyCacheList();
		virtual ~CMyCacheList();
		
		bool AddFileList(const void *inPathData, Uint32 inPathSize, TFieldData inListData);
		bool AddBundleList(const void *inPathData, Uint32 inPathSize, TFieldData inListData);
		bool AddArticleList(const void *inPathData, Uint32 inPathSize, TFieldData inListData);

		TFieldData SearchFileList(const void *inPathData, Uint32 inPathSize);
		TFieldData SearchBundleList(const void *inPathData, Uint32 inPathSize);
		TFieldData SearchArticleList(const void *inPathData, Uint32 inPathSize);
		
		bool ChangedFileList(const void *inPathData, Uint32 inPathSize);
		bool ChangedBundleList(const void *inPathData, Uint32 inPathSize);
		bool ChangedArticleList(const void *inPathData, Uint32 inPathSize);

		void Clear();
		void ClearFileList();
		void ClearBundleList();
		void ClearArticleList();
		
	protected:
		CPtrList<SMyCacheListInfo> mFileList;
		CPtrList<SMyCacheListInfo> mBundleList;
		CPtrList<SMyCacheListInfo> mArticleList;

		static bool AddList(CPtrList<SMyCacheListInfo>& inCacheList, const void *inPathData, Uint32 inPathSize, TFieldData inListData);
		static TFieldData SearchList(CPtrList<SMyCacheListInfo>& inCacheList, const void *inPathData, Uint32 inPathSize);
		static bool ChangedList(CPtrList<SMyCacheListInfo>& inCacheList, const void *inPathData, Uint32 inPathSize);
		static void ClearList(CPtrList<SMyCacheListInfo>& inCacheList);
};

#pragma mark CHttpIDList
class CHttpIDList
{
	public:
		CHttpIDList();
		virtual ~CHttpIDList();
		
		bool AddID(const Uint8 *inHttpID);
		void AddIDList(const CPtrList<Uint8>& inIDList);
		const CPtrList<Uint8>& GetIDList();
		
		// write/read
		bool WriteData(TFSRefObj* inFileRef, Uint32 inOffset);
		bool ReadData(TFSRefObj* inFileRef, Uint32 inOffset);
		
	private:
		CPtrList<Uint8> mIDList;
		
		void GetHttpIDFromData(const void *inData, Uint32 inDataSize);
};

#pragma mark CMyPathData
class CMyPathData
{
	public:
		CMyPathData(TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyPathData();
		
		void SetPathData(TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum = 0);
		
		Uint32 GetPathChecksum()			{	return mPathSum;			}
		bool IsPathEqual(const void *inData, Uint32 inDataSize);
		bool IsEmptyPath()					{	return mPathData == nil;	}
		void *GetPathPtr()					{	return mPathData;			}
		Uint32 GetPathSize()				{	return mPathSize;			}
		
	protected:		
		TPtr mPathData;
		Uint32 mPathSize;
		Uint32 mPathSum;
};

#pragma mark CMySearchText
class CMySearchText
{
	public:
		CMySearchText(CContainerView *inContainerView, const SRect& inBounds);
		virtual ~CMySearchText();
		
	protected:
		TTimer mSearchTimer1;
		TTimer mSearchTimer2;
		
		CLabelView *searchText;
	
		bool SearchText_KeyDown(const SKeyMsgData& inInfo);
		virtual void SearchText(const Uint8 *inText) = 0;
		
		static void SearchTimer1Proc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);
		static void SearchTimer2Proc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);	
};

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Views ₯₯

// a list view that shows a status when there are 0 items.
#pragma mark CMyListStatusView
class CMyListStatusView : public CTabbedListView
{
	public:
		CMyListStatusView(CViewHandler *inHandler, const SRect& inBounds);
		~CMyListStatusView();
		
		void SetStatus(Uint32 inStatus);
		void SetStatus(const Uint8 inMsg[]);
		
		virtual Uint32 GetFullHeight() const;
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);

	protected:
		Uint32 mStatus;
		TPtr mCustStatMsg;
};

//-----------------------------------------------------------------------------------------------------------

#pragma mark CMyTreeStatusView
template <class T> class CMyTreeStatusView : public CTabbedTreeView<T>
{
	public:
		CMyTreeStatusView(CViewHandler *inHandler, const SRect& inBounds);
		~CMyTreeStatusView();
		
		void SetStatus(Uint32 inStatus);
		void SetStatus(const Uint8 inMsg[]);

		virtual Uint32 GetFullHeight() const;
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);

	protected:
		Uint32 mStatus;
		TPtr mCustStatMsg;
};

template<class T> CMyTreeStatusView<T>::CMyTreeStatusView(CViewHandler *inHandler, const SRect& inBounds)
	: CTabbedTreeView(inHandler, inBounds, 240, 241)
{
#if USE_LARGE_FONT
	mTabHeight = 23;
	mCellHeight = 27;
#elif WIN32	
	mTabHeight = 18;
#endif

#if DISABLE_TABS
	mTabHeight = 0;
#endif

	mLevelWidth = 20;

	mStatus = 0;
	mCustStatMsg = nil;
}

template<class T> CMyTreeStatusView<T>::~CMyTreeStatusView()
{
	if (mCustStatMsg)
	{
		delete mCustStatMsg;
		mCustStatMsg = nil;
	}
}

template<class T> void CMyTreeStatusView<T>::SetStatus(Uint32 inStatus)
{
	if (inStatus == mStatus)
		return;
	
	Uint32 nOldStatus = mStatus;
	mStatus = inStatus;

	if (!GetTreeCount() && nOldStatus == listStat_Hide)
		SetFullHeight();
	
	Refresh();
}

template<class T> void CMyTreeStatusView<T>::SetStatus(const Uint8 inMsg[])
{
	Uint32 nOldStatus = mStatus;
	mStatus = listStat_Custom;
	
	if (mCustStatMsg)
	{
		delete mCustStatMsg;
		mCustStatMsg = nil;
	}
	mCustStatMsg = UMemory::New(inMsg, inMsg[0] + 1);

	if (!GetTreeCount() && nOldStatus == listStat_Hide)
		SetFullHeight();
	
	Refresh();
}

template<class T> Uint32 CMyTreeStatusView<T>::GetFullHeight() const
{
	if (mHandler && !GetTreeCount())
	{
		CScrollerView *pHandler = dynamic_cast<CScrollerView *>(mHandler);
		if (pHandler)
		{
			SRect stBounds;
			pHandler->GetBounds(stBounds);
			
			return stBounds.GetHeight() - 2;
		}		
	}

	return CTabbedTreeView::GetFullHeight();
}

template<class T> void CMyTreeStatusView<T>::Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth)
{
	UGraphics::SetFont(inImage, kDefaultFont, nil, 9);
#if MACINTOSH
	inImage->SetInkMode(mode_Or);
#endif
	
	// draw the status text
	if (!GetTreeCount() && mStatus != listStat_Hide)
	{
		const Uint8 *statTxt;
		
		switch(mStatus)
		{
			case listStat_Loading:
				statTxt = "\pLoading...";
				break;
			
			case listStat_0Items:
				statTxt = "\p0 items in list";
				break;
						
			case listStat_Custom:
				statTxt = mCustStatMsg ? BPTR(mCustStatMsg) : "\p";
				break;
			
			default:	// listStat_Hide
				statTxt = "\p";
				break;
		}
		
		// set color
		inImage->SetInkColor(color_Gray);
		
		SRect stRect = mBounds;
		stRect.Enlarge(-4);
		stRect.top += mTabHeight;
		if (stRect.bottom < stRect.top)
			stRect.bottom = stRect.top;
		
		inImage->DrawTextBox(stRect, inUpdateRect, statTxt + 1, statTxt[0], 0, textAlign_Left);
	}

	CTabbedTreeView::Draw(inImage, inUpdateRect, inDepth);
}

//-----------------------------------------------------------------------------------------------------------

#pragma mark CMyIconPickView
class CMyIconPickView : public CView
{
	public:
		CMyIconPickView(CViewHandler *inHandler, const SRect& inBounds);
		virtual ~CMyIconPickView();
		
		void SetIcon(Int16 inIconID);
		Int16 GetIcon()							{	return mIconID;			}
		void SetMark(bool inMark);
		bool GetMark()							{	return mMark;			}
		void SetNext(CMyIconPickView *inNext)	{	mNext = inNext;			}
		
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);
		virtual void MouseDown(const SMouseMsgData& inInfo);

	protected:
		TIcon mIcon;
		Int16 mIconID;
		
		bool mMark;
		CMyIconPickView *mNext;
};

#pragma mark CMyFileListView
class CMyFileListView : public CMyListStatusView
{
	public:
		CMyFileListView(CViewHandler *inHandler, const SRect& inBounds, bool inListExpl);
		virtual ~CMyFileListView();

		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2);
	
		virtual Uint32 GetItemCount() const;
		bool AddListFromFields(TFieldData inData);
		Uint32 SelectNames(const Uint8 *inStr);
		Uint32 SearchNames(const Uint8 *inStr);
		
		bool GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode = nil, Uint32 *outCreatorCode = nil, const SMouseMsgData *inInfo = nil);
		void DeleteAll();
		
		bool HasPartialFile(const Uint8 *inFileName, bool inIsFolder);
		TFSRefObj* GetItemToUpload();
		void ClearUploadList();

		virtual void DragEnter(const SDragMsgData& inInfo);
		virtual void DragMove(const SDragMsgData& inInfo);
		virtual bool Drop(const SDragMsgData& inInfo);

		virtual void SetItemSelect(Uint32 inItem, bool inSelect);

	protected:
		bool mListExpl;

		static Uint16 mIconIndent;
		static Uint16 mNameIndent;
						
		CPtrList<SMyFileItem> mFileList;
		CPtrList<TFSRefObj> mUploadList;
		
		virtual void ItemDraw(Uint32 inListIndex, TImage inImage, const SRect& inBounds, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
		static Int32 CompareNames(void *inPtrA, void *inPtrB, void *inRef);
};

#pragma mark CMyFileTreeView
class CMyFileTreeView : public CMyTreeStatusView<SMyFileItem>
{
	public:
		CMyFileTreeView(CViewHandler *inHandler, const SRect& inBounds, TPtr *inPathData, Uint32 *inPathSize, bool inTreeExpl);
		virtual ~CMyFileTreeView();
	
		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2);
	
		bool AddListFromFields(const void *inPathData, Uint32 inPathSize, TFieldData inData);
		Uint32 SelectNames(const Uint8 *inStr);
		Uint32 SelectChildNames(const Uint8 *inStr);
		Uint32 SearchNames(const Uint8 *inStr);
		
		void *RefreshSelectedFolderPath(Uint32& outPathSize);
		void *GetSelectedFolderPath(Uint32& outPathSize);
		void *GetSelectedParentFolderPath(Uint32& outPathSize);
		bool GetDropFolderPath(void **outPathData, Uint32& outPathSize);
		bool GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode = nil, Uint32 *outCreatorCode = nil, const SMouseMsgData *inInfo = nil);

		bool CollapseSelectedFolder();
		bool DiscloseSelectedFolder();
	
		void DeletePath(const void *inPathData, Uint32 inPathSize);
		void DeleteAll();
		
		bool HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder);
		TFSRefObj* GetItemToUpload();
		void ClearUploadList();
		
		virtual void DragEnter(const SDragMsgData& inInfo);
		virtual void DragMove(const SDragMsgData& inInfo);
		virtual void DragLeave(const SDragMsgData& inInfo);
		virtual bool Drop(const SDragMsgData& inInfo);

		// key events
		virtual bool KeyDown(const SKeyMsgData& inInfo);

		// tree events
		virtual void SelectionChanged(Uint32 inTreeIndex, SMyFileItem *inTreeItem, bool inIsSelected);
		virtual void DisclosureChanged(Uint32 inTreeIndex, SMyFileItem *inTreeItem, Uint8 inDisclosure);

	protected:
		TPtr *mPathData;
		Uint32 *mPathSize;
		
		bool mTreeExpl;
		Uint32 mKeyDownTime;

		CPtrList<TFSRefObj> mUploadList;
		Uint32 mRefreshFolderIndex;
		Uint32 mDropFolderIndex;
		Uint32 mSavedFolderIndex;
		
		static Uint16 mIconIndent;
		static Uint16 mNameIndent;
			
		bool GetFolderIndex(const void *inPathData, Uint32 inPathSize, Uint32& outFolderIndex);
		void *GetFolderPath(Uint32 &ioFolderIndex, Uint32& outPathSize);
		void SetDropFolderIndex(const SDragMsgData& inInfo);

		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyFileItem *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
		static Int32 CompareNames(void *inItemA, void *inItemB, void *inRef);
};

#pragma mark CMyTaskListView
class CMyTaskListView : public CListView
{
	public:
		CMyTaskListView(CViewHandler *inHandler, const SRect& inBounds);
		virtual ~CMyTaskListView();

		virtual Uint32 GetItemCount() const;
		virtual void SetItemSelect(Uint32 inItem, bool inSelect);
		virtual void ItemsRemoved(Uint32 inAtItem, Uint32 inCount);
		
		void AddTask(CMyTask *inTask, const Uint8 inText[]);
		void RemoveTask(CMyTask *inTask);
		
		void SetTaskProgress(CMyTask *inTask, Uint32 inVal, Uint32 inMax, const Uint8 inDesc[]);
		CMyTask *GetSelectedTask();
		void ShowFinishedBar(CMyTask *inTask);
		
		virtual Uint32 GetFullHeight() const;
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);

	protected:
		CPtrList<SMyTaskNameInfo> mTaskList;

		virtual void ItemDraw(Uint32 inItem, TImage inImage, const SRect& inBounds, const SRect& inUpdateRect, Uint32 inOptions);
		
		SMyTaskNameInfo *TaskToInfo(CMyTask *inTask, Uint32 *outIndex = nil);
};

#pragma mark CMyUserListView
class CMyUserListView : public CMyListStatusView
{
	public:
		CMyUserListView(CViewHandler *inHandler, const SRect& inBounds);
		virtual ~CMyUserListView();

		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2);

		virtual Uint32 GetItemCount() const;
		void AddListFromFields(TFieldData inData);
		
		bool UpdateUser(Uint16 inID, Int16 inIconID, Uint16 inFlags, const void *inNameData, Uint32 inNameSize, Uint8 *outOldName = nil);
		void AddUser(Uint16 inID, Int16 inIconID, Uint16 inFlags, const void *inNameData, Uint32 inNameSize);
		bool DeleteUserByID(Uint16 inID, Uint8 *outName = nil);
		bool GetUserByID(Uint16 inID, Uint8 *outName = nil);
		void DeleteAll();
		
		Uint16 GetSelectedUserID(Uint8 *outName = nil);
		void GetRandomUserName(Uint8 *outName);
		Uint32 SearchNames(const Uint8 inStr[]);

		virtual void DragEnter(const SDragMsgData& inInfo);
		virtual void DragMove(const SDragMsgData& inInfo);

	protected:
		struct SMyUserListItem {
			TIcon icon;
			Uint16 id;
			Uint16 flags;
			Uint8 name[];
		};
		
	#if DRAW_USERLIST_ICONS
		TIcon mRefMessageIcon, mRefChatIcon; 
		TIcon mNoPrivIcon, mIsDownloading, mAllPrivIcon, mDisconnectPrivIcon, mRegularPrivIcon; 
	#endif
		
		CPtrList<SMyUserListItem> mUserList;

		virtual void ItemDraw(Uint32 inListIndex, TImage inImage, const SRect& inBounds, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
};

#pragma mark CMySubjectView
class CMySubjectView : public CView
{
	public:
		CMySubjectView(CViewHandler *inHandler, const SRect& inBounds);
		virtual ~CMySubjectView();
		
		void SetText(const void *inText, Uint32 inSize);
		void SetText(const Uint8 *inText)		{	SetText(inText+1, inText[0]);	}
		Uint32 GetText(void *outText, Uint32 inMaxSize) const	{	return UMemory::Read(mText, 0, outText, inMaxSize);	}

		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);
		virtual void MouseDown(const SMouseMsgData& inInfo);
		virtual void MouseUp(const SMouseMsgData& inInfo);
		virtual void MouseEnter(const SMouseMsgData& inInfo);
		virtual void MouseLeave(const SMouseMsgData& inInfo);

	protected:
		THdl mText;
		bool mIsHilited;
};

#pragma mark CMyNewsArticleTreeView
class CMyNewsArticleTreeView : public CMyTreeStatusView<SMyNewsArticle>
{
	public:
		CMyNewsArticleTreeView(CViewHandler *inHandler, const SRect &inBounds, CNZReadList *inReadList);
		virtual ~CMyNewsArticleTreeView();
		
		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2, Uint8 inTabPercent3, Uint8 inTabPercent4, Uint8 inTabPercent5);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2, Uint8& outTabPercent3, Uint8& outTabPercent4, Uint8& outTabPercent5);

		bool SetItemsFromData(const Uint8 *inData, Uint32 inDataSize);
		bool AddListFromFields(TFieldData inFields);
		Uint32 SearchNames(const Uint8 *inStr);
		void DeleteAll();

		Uint32 GetSelectedItemNameAndID(Uint8 *outItemName);
		Uint32 ReadSelectedItemNameAndID(Uint8 *outItemName);
		void SetSelectedItem(Uint32 inID);
		void SetCurrentItem(Uint32 inID);
				
		void SetReadList(CNZReadList *inReadList)	{	mReadList = inReadList;							}
		CNZReadList *GetReadList()					{	return mReadList;								}
		void SetReadListItem(Uint32 inItemID)		{	if (mReadList) mReadList->SetRead(inItemID);	}

		// key events
		virtual bool KeyDown(const SKeyMsgData& inInfo);

		// tree events
		virtual void SelectionChanged(Uint32 inTreeIndex, SMyNewsArticle *inTreeItem, bool inIsSelected);

	protected:
		static TIcon mThreadRootIcon;
		CNZReadList *mReadList;
		
		Uint32 mKeyDownTime;

		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyNewsArticle *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
		virtual void TreeItemMouseDown(Uint32 inTreeIndex, const SMouseMsgData& inInfo);				
};

#pragma mark CMyNewsCategoryListView
class CMyNewsCategoryListView : public CMyListStatusView
{
	public:
		CMyNewsCategoryListView(CViewHandler *inHandler, const SRect &inBounds);
		virtual ~CMyNewsCategoryListView();
	
		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2);
	
		bool AddListFromFields(TFieldData inData);
		Uint32 SelectNames(const Uint8 *inStr);
		Uint32 SearchNames(const Uint8 *inStr);
		
		// returns type and outItemName is a pstring
		Uint16 GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID = nil);
		
		void DeleteAll();
		virtual Uint32 GetItemCount() const;
	
		virtual void SetItemSelect(Uint32 inItem, bool inSelect);
			
	protected:
		CPtrList<SNewsCatItm> mList;

		virtual void ItemDraw(Uint32 inListIndex, TImage inImage, const SRect& inBounds, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
		static Int32 CompareNames(void *inPtrA, void *inPtrB, void *inRef);
};


#pragma mark CMyNewsCategoryTreeView
class CMyNewsCategoryTreeView : public CMyTreeStatusView<SNewsCatItm>
{
	public:
		CMyNewsCategoryTreeView(CViewHandler *inHandler, const SRect& inBounds, TPtr *inPathData, Uint32 *inPathSize, bool inTreeExpl);
		virtual ~CMyNewsCategoryTreeView();
		
		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2);

		bool AddListFromFields(const void *inPathData, Uint32 inPathSize, TFieldData inData);
		Uint32 SelectNames(const Uint8 *inStr);
		Uint32 SearchNames(const Uint8 *inStr);

		void *RefreshSelectedBundlePath(Uint32& outPathSize, bool *outCategory = nil);
		void *GetSelectedBundlePath(Uint32& outPathSize);
		void *GetSelectedParentBundlePath(Uint32& outPathSize);
		void *GetSelectedCategoryPath(Uint32& outPathSize, SGUID *outGUID = nil);
		Uint16 GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID = nil);

		bool CollapseSelectedBundle();
		bool DiscloseSelectedBundle();
	
		void DeletePath(const void *inPathData, Uint32 inPathSize);
		void DeleteAll();
		
		// key events
		virtual bool KeyDown(const SKeyMsgData& inInfo);

		// tree events
		virtual void SelectionChanged(Uint32 inTreeIndex, SNewsCatItm *inTreeItem, bool inIsSelected);
		virtual void DisclosureChanged(Uint32 inTreeIndex, SNewsCatItm *inTreeItem, Uint8 inDisclosure);

	protected:
		TPtr *mPathData;
		Uint32 *mPathSize;

		bool mTreeExpl;
		Uint32 mKeyDownTime;
		
		Uint32 mRefreshBundleIndex;
		
		bool GetBundleIndex(const void *inPathData, Uint32 inPathSize, Uint32& outFolderIndex);
		void *GetBundlePath(Uint32 &ioFolderIndex, Uint32& outPathSize);

		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SNewsCatItm *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
		static Int32 CompareNames(void *inItemA, void *inItemB, void *inRef);
};

#pragma mark CMyAccessCheckBoxView
class CMyAccessCheckBoxView : public CCheckBoxView
{
	public:
		CMyAccessCheckBoxView(CViewHandler *inHandler, const SRect& inBounds);
		
		void SetAccessID(Uint32 inID)			{	mAccessID = inID;	}
		Uint32 GetAccessID()					{	return mAccessID;	}
		
	protected:
		Uint32 mAccessID;


};

#pragma mark CMyConnectedInfoView
class CMyConnectedInfoView : public CView
{
	public:
		CMyConnectedInfoView(CViewHandler *inHandler, const SRect& inBounds);
		CMyConnectedInfoView(CViewHandler *inHandler, const SRect& inBounds, const Uint8 *inText, TFontDesc inFont = nil);
		virtual ~CMyConnectedInfoView();
		
		void SetText(const Uint8 *inText);
		Uint32 GetText(void *outText, Uint32 inMaxSize);

		void SetAltText(const Uint8 *inText);
		Uint32 GetAltText(void *outText, Uint32 inMaxSize);
		void ShowAltText();
				
		void SetFont(TFontDesc inFont);	
		
		virtual void Timer(TTimer inTimer);
		virtual void MouseDown(const SMouseMsgData& inInfo);
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);
		
	protected:
		Uint8 mText[256];
		Uint8 mAltText[256];
		
		TFontDesc mFont;
		TTimer mTimer;
};

#pragma mark CMyImageView
class CMyImageView : public CImageView
{
	public:
		CMyImageView(CViewHandler *inHandler, const SRect& inBounds, CDecompressImage *inDecompressImage);
		
		virtual bool LaunchURL();	
};

#pragma mark CMyAnimatedGifView
class CMyAnimatedGifView : public CAnimatedGifView
{
	public:
		CMyAnimatedGifView(CViewHandler *inHandler, const SRect& inBounds, CDecompressGif *inDecompressGif);
		
		virtual bool LaunchURL();
};

#pragma mark CMyQuickTimeView
class CMySaveFileWin;
class CMyQuickTimeView : public CQuickTimeView
{
	public:
		CMyQuickTimeView(CViewHandler *inHandler, const SRect& inBounds, Uint16 inResizeOptions, Uint16 inOptions);
		
		void SetSaveFileWin(CMySaveFileWin *inSaveFileWin);
		virtual bool SaveMovieAs();
		virtual bool LaunchURL();
		
	protected:
		CMySaveFileWin *mSaveFileWin;
};

#pragma mark CMyOptIconTab
class CMyOptIconTab : public CContainerView
{
	public:
		CMyOptIconTab(const SPoint& inPoint);
		
		void SetText(const Uint8 *inText);
		void SetIconID(Int16 inID);
		Int16 GetIconID();
		
	protected:
		enum {kIconRowNum = 91, kIconColNum = 7, kIconCount = kIconRowNum*kIconColNum, kIconSize = 40, kIconSpace = 1, kIconHoriz = 4, kIconVert = 6};
		
		CLabelView *mTextLbl;
		CMyIconPickView *mIcons[kIconCount];
		CScrollerView *mIconsScroll;
};

#pragma mark CMyClickPicView
class CMyClickPicView : public CPictureView
{
	public:
		CMyClickPicView(CViewHandler *inHandler, const SRect& inBounds, Int32 inPictID);
		virtual void MouseUp(const SMouseMsgData& inInfo);
};
CWindow *MakeClickPicWin(const SRect& inBounds, Int32 inID, const Uint8 *inTxt = nil);


/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Windows ₯₯

#pragma mark CMyOptionsWin
class CMyOptionsWin : public CWindow
{
	public:
		CMyOptionsWin();
				
		void SetInfo(const Uint8 *inName, const Uint8 *inFirewall, const SMyOptions& inOptions);
		void GetInfo(Uint8 *outName, Uint8 *outFirewall, SMyOptions& outOptions);

		Int16 GetIconID()				{	return mViews.iconTab->GetIconID();					}
		void SetIconID(Int16 inID)		{	mViews.iconTab->SetIconID(inID);					}
		void DisablePlaySounds()		{	mSoundChecks[0]->Disable();	UpdateSoundChecks(); 	}
		
		void ShowHideDateTime();
		void ShowHideAutomaticResponse();
		void UpdateSoundChecks();

	protected:
		struct 
		{
			CScrollerView *automaticResponseScroll, *cancelAutorespTimeScroll;
			
			CLabelView *lbl2, *lbl3;
			
			CTextView *nameText, *automaticResponseText, *cancelAutorespTime,  *firewallText;
			CMyOptIconTab *iconTab;
			
			CCheckBoxView *showPrivMsgAtBack, *queueTransfers, *showJoinLeave, *showDateTime, *showSplash, *showBanner, *browseFolders, 
						  *toolbarButtonsTop, *showTooltips, *magneticWindows, *refusePrivateMsg, *refusePrivateChat, *refuseBroadcast,
						  *automaticResponse, *autoAgree, *logMessages,  
						  *tunnelThroughHttp, *dontSaveCookies;
						  
			CCheckBoxView *filesList, *filesTree, *filesExpl, *newsList, *newsTree, *newsExpl, *originalIconset, *fampastelIconset;
			
			struct
			{
				CTextView *size;
				CColorView *col, *backCol;
			
			} fontChat, fontPrivChat, fontPrev, fontNews;
			
		} mViews;
		
		CCheckBoxView *mSoundChecks[10];

		CContainerView *MakeGeneralTab();
		CContainerView *MakeDisplayTab();
		CContainerView *MakeSoundTab();
		CContainerView *MakeAdvancedTab();
		CContainerView *MakeIconSetTab();
};


#pragma mark CMyLoginWin
class CMyLoginWin : public CWindow
{
	public:
		CMyLoginWin();
				
		void SetInfo(const Uint8 *inAddress, const Uint8 *inLogin, const Uint8 *inPassword, bool inUseCrypt);
		void GetInfo(Uint8 *outAddress, Uint8 *outLogin, Uint8 *outPassword, bool *outUseCrypt);
		void UpdateButtons();
		
	protected:
		struct {
			CContainerView *container;
			CTextView *addressText, *nameText;
			CPasswordTextView *passwordText;
			CScrollerView *addressScr, *nameScr, *passwordScr;
			CButtonView *connectBtn, *saveBtn;
			CCheckBoxView *useCrypt;
		} mViews;
};

#pragma mark CMyEnterMsgWin
class CMyEnterMsgWin : public CWindow
{
	public:
		CMyEnterMsgWin();
		
		THdl GetTextHandle()		{	return mViews.msgText->GetTextHandle();		}
		
	protected:
		struct {
			CTextView *msgText;
			CButtonView *sendBtn;
		} mViews;
};

#pragma mark CMySendPrivMsgWin
class CMySendPrivMsgWin : public CWindow
{
	public:
		CMySendPrivMsgWin();
		
		THdl GetTextHandle()									{	return mViews.msgText->GetTextHandle();					}
		void SetQuotHandle(THdl inHdl);

		void SetToText(const Uint8 *inUserName)					{	mViews.toText->SetText(inUserName);						}
		Uint32 GetToText(void *outUserName, Uint32 inMaxSize)	{	return mViews.toText->GetText(outUserName, inMaxSize);	}

	protected:
		struct {
			CScrollerView *msgScroll, *quotScroll;
			CTextView *msgText, *quotText;
			CLabelView *toText;
			CButtonView *sendBtn;
		} mViews;
};

#pragma mark CMyChangeIconWin
class CMyChangeIconWin : public CWindow
{
	public:
		CMyChangeIconWin();
		void SetInfo(const Uint8 inIconId[], const Uint8 inNickId[]);
		
		void GetInfo(Uint8 *outIcon, Uint8 *outNick);
		
	protected:
		struct {
			CTextView  *iconText;
			CTextView  *nickText;
			
		
	
			
		} mViews;
};

#pragma mark CMyCouldNotSendPrivMsgWin
class CMyCouldNotSendPrivMsgWin : public CWindow
{
	public:
		CMyCouldNotSendPrivMsgWin(const Uint8 *inUserName, THdl inTextHdl);
		virtual ~CMyCouldNotSendPrivMsgWin();

		// timer events
		virtual void Timer(TTimer inTimer);
		
	protected:
		struct {
			CButtonView *OKBtn;
		} mViews;

		TTimer mMsgTimer;
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
		} mViews;
};

#pragma mark CMyUserWin
class CMyUserWin
{
	public:
		virtual void DoPrivateChat() = 0;
		virtual void DoPrivateMsg() = 0;
		virtual void DoGetInfo() = 0;
		
		
		};


#pragma mark CMyPrivMsgWin
class CMyPrivMsgWin : public CWindow, public CMyUserWin
{
	public:
		CMyPrivMsgWin(CWindow *inParent, bool bIsAutomaticResponse = false);
		virtual ~CMyPrivMsgWin();
		
		void SetTextHandle(THdl inHdl);
		THdl GetTextHandle() 						{ 	return mViews.msgText->GetTextHandle();					}
		void SetQuotHandle(THdl inHdl);

		void SetUserID(Uint16 inUserID)				{	mUserID = inUserID;										}
		Uint16 GetUserID()							{	return mUserID;											}
		void SetUserName(const Uint8 *inUserName)	{ 	UMemory::Copy(mUserName, inUserName, inUserName[0]+1);	}
		void GetUserName(Uint8 *outUserName)		{ 	UMemory::Copy(outUserName, mUserName, mUserName[0]+1);	}
		void SetAccess();
		
		// actions
		virtual void DoPrivateChat();
		virtual void DoPrivateMsg(){};
		
		virtual void DoGetInfo();
		
		// key events
		virtual void KeyDown(const SKeyMsgData& inInfo);

		// timer events
		virtual void Timer(TTimer inTimer);

	protected:
		struct {
			CScrollerView *msgScroll, *quotScroll;
			CTextView *msgText, *quotText;
			CLabelView* quotLabel;
			CLabelView* msgLabel;
			CIconButtonView *chatBtn, *infoBtn;
			CButtonView *replyBtn;
			CButtonView *dismissBtn;
		} mViews;

		Uint16 mUserID;
		Uint8 mUserName[64];
		
		TTimer mMsgTimer;
};

#pragma mark CMyTextWin
class CMyTextWin : public CWindow
{
	public:
		CMyTextWin(CWindow *inParent);
				
		CTextView *GetTextView()		{	return mTextView;		}
		void SetTextColor(const SMyColorInfo& colorInfo);
		
	protected:
		CTextView *mTextView;
		CScrollerView *mScrollerView;
};

#pragma mark CMyOldNewsWin
class CMyOldNewsWin : public CWindow
{
	public:
		CMyOldNewsWin(CWindow *inParent);
				
		CTextView *GetTextView()		{	return mTextView;		}
		void SetTextColor(const SMyColorInfo& colorInfo);
		
	protected:
		CTextView *mTextView;
		CScrollerView *mScrollerView;
};

#pragma mark CMySaveFileWin
class CMySaveFileWin : public CWindow
{
	public:
		CMySaveFileWin(CWindow *inParent, const SRect& inBounds);
		~CMySaveFileWin();
	
		bool SaveFileAs();
		bool IsThisFile(const void *inPathData, Uint32 inPathSize, const Uint8 *inFileName);
	
		// key events
		virtual void KeyDown(const SKeyMsgData& inInfo);

	protected:

		TPtr mPathData;
		Uint32 mPathSize;
		CTempFile mTempFile;

		void SetPathData(const void *inPathData, Uint32 inPathSize);
		void ClearPathData();

		bool SetTempFile(const Uint8 *inFileName, Uint32 inTypeCode, Uint32 inCreatorCode);
		bool SetTempData(THdl inData);
};

#pragma mark CMyViewFileWin
class CMyViewFileTask;
class CMyViewFileWin : public CMySaveFileWin
{
	public:
		CMyViewFileWin(CWindow *inParent);
		~CMyViewFileWin();
		
		// key events
		virtual void KeyUp(const SKeyMsgData& inInfo);

		bool SetViewFileWin(CMyViewFileTask* inViewFileTask, const void *inPathData, Uint32 inPathSize, const Uint8 *inFileName, Uint32 inTypeCode, Uint32 inCreatorCode, Uint32 inFileSize, Uint32 inTotalSize, Uint32 inRefNum, bool inNewWindow);
		
		bool StopStreamMovie();
		static void ProgressMovieProc(Str255 inFileName, Uint32 inTotalSize, Uint32 inDownloadedSize);

		void InsertText(const void *inTextBuffer, Uint32 inBufferSize);
		void InsertImage(const void *inImageBuffer, Uint32 inBufferSize);
		void SetTextColor(const SMyColorInfo& colorInfo);
		Uint32 GetTextSize();
		
		void EndViewFileTask();
		bool IsEndViewFileTask();
				
		bool CanCloseViewFileWin();
		void SetMustCloseFlag();
		bool GetMustCloseFlag();
		
	protected:
		CContainerView *mContainerView; 
		CScrollerView *mScrollerView;
	
		CTextView *mTextView;
		CImageView *mImageView;
		CMyQuickTimeView *mQuickTimeView;

		CDecompressImage *mDecompressImage;
		CMyViewFileTask *mViewFileTask;
		
		void *mBuffer;
		Uint32 mBufferSize;
		Uint32 mBufferOffset;
		
		bool mNewWindow;
		SRect mWinBounds;
		bool mMustCloseFlag;
		
		void SetWindowSize(const SRect& inBounds);
		
		void ClearViews();
		void ClearData();
};

#pragma mark CMyServerInfoWin
class CMyServerInfoWin : public CMySaveFileWin
{
	public:
		CMyServerInfoWin(CWindow *inParent);
		virtual ~CMyServerInfoWin();

		// key events
		virtual void KeyUp(const SKeyMsgData& inInfo);

		bool SetServerInfoWin(const Uint8 *inServerName, THdl inData, Uint32 inTypeCode, const Uint8 *inURL = nil, const Uint8 *inComment = nil);
		bool IsThisServer(const Uint8 *inServerName, THdl inData, Uint32 inTypeCode, const Uint8 *inURL = nil, const Uint8 *inComment = nil);

		CImageView *MakeImageView(THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment);
		CMyQuickTimeView *MakeQuickTimeView(THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment);

	protected:
		CImageView *mImageView;
		CMyQuickTimeView *mQuickTimeView;
		
		void SetWindowSize(const SRect& inBounds);
};

#pragma mark CMyPublicChatWin
class CMyPublicChatWin : public CWindow
{
	public:
		CMyPublicChatWin(CWindow *inParent);
		
		CTextView *GetDisplayTextView()		{	return mViews.allText;				}
		CTextView *GetSendTextView()		{	return mViews.sendText;				}
		void ScrollToBottom()				{	mViews.allScroll->ScrollToBottom();	}
		void ClearInputBox()				{	mViews.sendText->DeleteText(0, max_Uint32, true);	}
		void ClearChatText()				{	mViews.allText->DeleteText(0, max_Uint32, true); mViews.allText->SetFullHeight();	}
		bool IsChatTextActive()				{	return mViews.allText->IsFocus();	}
	
		void SetTextColor(const SMyColorInfo& colorInfo);

		virtual void KeyDown(const SKeyMsgData& inInfo);

	protected:
		struct {
			CTextView *allText, *sendText;
			CScrollerView *allScroll, *sendScroll;
		} mViews;
};

#pragma mark CMyPrivateChatWin
class CMyPrivateChatWin : public CWindow, public CLink, public CDragAndDropHandler, public CMyUserWin
{
	public:
		CMyPrivateChatWin(CWindow *inParent);
		virtual ~CMyPrivateChatWin();

		CTextView *GetDisplayTextView()				{	return mViews.allText;		}
		CTextView *GetSendTextView()				{	return mViews.sendText;		}
		CMyUserListView *GetUserListView()			{	return mViews.list;			}
		CScrollerView *GetUserListScrollerView()	{	return mViews.listScroll;	}

		void ScrollToBottom()				{	mViews.allScroll->ScrollToBottom();	}
		void ClearInputBox()				{	mViews.sendText->DeleteText(0, max_Uint32, true);	}
		void ClearChatText()				{	mViews.allText->DeleteText(0, max_Uint32, true); mViews.allText->SetFullHeight();	}
		bool IsChatTextActive()				{	return mViews.allText->IsFocus();	}
		
		void SetChatID(Uint32 inID)			{	mChatID = inID;		}
		Uint32 GetChatID() const			{	return mChatID;		}
		
		void SetTextColor(const SMyColorInfo& colorInfo);
		void SetAccess();

		void SetSubject(const Uint8 *inText);
		void GetSubject(Uint8 *outText) const	{	outText[0] = mViews.subject->GetText(outText+1, 255);	}

		virtual void DoPrivateChat();
		virtual void DoPrivateMsg();
		virtual void DoGetInfo();
		
		
		
		virtual void KeyDown(const SKeyMsgData& inInfo);
		virtual void UserZoom(const SMouseMsgData& inInfo);
		
		virtual void DragEnter(const SDragMsgData& inInfo);
		virtual void DragMove(const SDragMsgData& inInfo);
		
		virtual void HandleDrag(CDragAndDroppable *inDD, const SMouseMsgData& inInfo);
		virtual void HandleSetDragAction(CDragAndDroppable *inDD, const SDragMsgData& inInfo);
		virtual bool HandleDrop(CDragAndDroppable *inDD, const SDragMsgData& inInfo);

		static CLinkedList gList;
		
	protected:
		struct {
			CTextView *allText, *sendText;
			CScrollerView *allScroll, *sendScroll, *listScroll;
			CMyUserListView *list;
			CMySubjectView *subject;
			CIconButtonView *chatBtn, *msgBtn, *infoBtn, *discBtn;
		} mViews;

		Uint32 mChatID;
		TDrag mCurrentDrag;
		bool mIsListVis;
};

#pragma mark CMyItemsWin
class CMyItemsWin : public CWindow, public CMyPathData
{
	public:
		CMyItemsWin(CWindow *inParent, const SRect &inBounds, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum);

		virtual void SetStatus(Uint32 inStatus) = 0;
		virtual void GetContent() = 0;
		virtual void SetAccess() = 0;

		virtual void SetBoundsInfo() = 0;
		virtual void GetBoundsInfo() = 0;

		void SetPattern(Int16 inID)	{	mBoxView->SetStyle(boxStyle_Pattern); mBoxView->SetPattern(inID);	}

	protected:
		CContainerView *mVC;
		CBoxView *mBoxView;
};

#pragma mark CMyItemsListWin
class CMyItemsListWin : public CMyItemsWin
{
	public:
		CMyItemsListWin(CWindow *inParent, const SRect &inBounds, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		
		virtual void SetStatus(Uint32 inStatus);	// 0 = loading, 1 = loaded, 2 = 0 items in list
		CMyListStatusView *GetListView()	{	return mListView;	}
		static CMyItemsListWin *FindWindowByPath(CVoidPtrList &inWinList, const void *inPathData, Uint32 inPathSize, Uint32 inCheckSum = 0);
		
		virtual void UserZoom(const SMouseMsgData& inInfo);

	protected:
		CMyListStatusView *mListView;
};

//-----------------------------------------------------------------------------------------------------------

#pragma mark CMyItemsTreeWin
template <class T> class CMyItemsTreeWin : public CMyItemsWin
{
	public:
		CMyItemsTreeWin(CWindow *inParent, const SRect &inBounds, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		
		virtual void SetStatus(Uint32 inStatus);	// 0 = loading, 1 = loaded, 2 = 0 items in list
		CMyTreeStatusView<T> *GetTreeView()	{	return mTreeView;	}
		static CMyItemsTreeWin<T> *FindWindowByPath(CVoidPtrList &inWinList, const void *inPathData, Uint32 inPathSize, Uint32 inCheckSum = 0);

		virtual void UserZoom(const SMouseMsgData& inInfo);

	protected:
		CMyTreeStatusView<T> *mTreeView;
};

template<class T> CMyItemsTreeWin<T>::CMyItemsTreeWin(CWindow *inParent, const SRect &inBounds, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsWin(inParent, inBounds, inPathData, inPathSize, inPathSum)
{
	mTreeView = nil;
}

template<class T> void CMyItemsTreeWin<T>::SetStatus(Uint32 inStatus)
{
	if (mTreeView)
		mTreeView->SetStatus(inStatus);
}

template<class T> CMyItemsTreeWin<T> *CMyItemsTreeWin<T>::FindWindowByPath(CVoidPtrList &inWinList, const void *inPathData, Uint32 inPathSize, Uint32 inCheckSum)
{
	Uint32 i = 0;
	CMyItemsTreeWin *win;
	
	if (inCheckSum == 0)
		inCheckSum = UMemory::Checksum(inPathData, inPathSize);
	
	while (inWinList.GetNext((void*&)win, i))
	{
		if (win->GetPathChecksum() == inCheckSum && win->IsPathEqual(inPathData, inPathSize))
			return win;
	}
	
	return nil;
}

template<class T> void CMyItemsTreeWin<T>::UserZoom(const SMouseMsgData& /* inInfo */)
{
	SRect r;
	
	GetBounds(r);
	if (mTreeView)
		r.bottom = r.top + mTreeView->GetFullHeight() + 31;
	else
		r.bottom = r.top + 60;
	
	if (UWindow::GetZoomBounds(mWindow, r, r))
		SetBounds(r);
}

//-----------------------------------------------------------------------------------------------------------

#pragma mark CMyFileWin
class CMyFileWin : public CDragAndDropHandler, public CMySearchText
{
	public:
		CMyFileWin(CContainerView *inContainerView, const SRect& inBounds);

		virtual bool HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder) = 0;
		virtual TFSRefObj* GetItemToUpload() = 0;
		virtual void ClearUploadList() = 0;

		void GetFileContent(const void *inPathData, Uint32 inPathSize);
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache) = 0;
		void SearchCacheFileList(const void *inPathData, Uint32 inPathSize);
		void SetFileAccess(const Uint8 *inItemName, const void *inPathData, Uint32 inPathSize, Uint32 inTypeCode, bool inIsFolder);
		
		virtual bool GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode = nil, Uint32 *outCreatorCode = nil, const SMouseMsgData *inInfo = nil) = 0;
		virtual void *GetSelectedParentFolderPath(Uint32& outPathSize, bool *outDisposePath = nil) = 0;

		virtual void DoOpenParent(Uint16 inMods);
		virtual void DoDownload(Uint16 inMods);
		virtual void DoUpload(Uint16 inMods) = 0;
		virtual void DoUploadFromDrop(bool inTreeList) = 0;
		virtual void DoView(Uint16 inMods);
		virtual void DoGetInfo();
	
		virtual void DoNewFolder() = 0;
		virtual void DoRefresh(Uint16 inMods) = 0;
		virtual void DoMove(Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inTreeList) = 0;
		virtual void DoDoubleClick(Uint16 inMods, bool inTreeList) = 0;
		virtual void DoDelete();

		virtual void HandleDrag(CDragAndDroppable *inDD, const SMouseMsgData& inInfo);
		virtual void HandleSetDragAction(CDragAndDroppable *inDD, const SDragMsgData& inInfo);
		virtual bool HandleDrop(CDragAndDroppable *inDD, const SDragMsgData& inInfo);

	protected:
		TDrag mCurrentDrag;

		struct {
			
			CIconButtonView *parentFolder, *downloadBtn, *uploadBtn, *viewBtn, *infoBtn, *newfolderBtn, *deleteBtn;
		} mViews;

		static void DownloadDragSendProc(TDrag inDrag, Uint32 inItem, Uint32 inFlavor, void *inRef, bool *outDropEnd = nil, bool *outDropError = nil);
		bool File_KeyDown(const SKeyMsgData& inInfo);
};

#pragma mark CMyFileListWin
class CMyFileListWin : public CMyItemsListWin, public CMyFileWin
{
	public:
		CMyFileListWin(CWindow *inParent, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyFileListWin();
		
		CMyFileListView *GetFileListView()				{	return ((CMyFileListView *)mListView);		}
		
		virtual bool HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder);
		virtual TFSRefObj* GetItemToUpload();
		virtual void ClearUploadList();

		void GetContent();
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetAccess();
		
		virtual void SetBoundsInfo();
		virtual void GetBoundsInfo();

		virtual bool GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode = nil, Uint32 *outCreatorCode = nil, const SMouseMsgData *inInfo = nil);
		virtual void *GetSelectedParentFolderPath(Uint32& outPathSize, bool *outDisposePath = nil);

		virtual void DoUpload(Uint16 inMods);
		virtual void DoUploadFromDrop(bool inTreeList);
		virtual void DoNewFolder();
		virtual void DoRefresh(Uint16 inMods);
		virtual void DoMove(Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inTreeList);
		virtual void DoDoubleClick(Uint16 inMods, bool inTreeList);

		virtual void KeyDown(const SKeyMsgData& inInfo);
		
	protected:				
		virtual void SearchText(const Uint8 *inText);
};

#pragma mark CMyFileTreeWin
class CMyFileTreeWin : public CMyItemsTreeWin<SMyFileItem>, public CMyFileWin
{
	public:
		CMyFileTreeWin(CWindow *inParent, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyFileTreeWin();

		CMyFileTreeView *GetFileTreeView()				{	return ((CMyFileTreeView *)mTreeView);		}
		
		virtual bool HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder);
		virtual TFSRefObj* GetItemToUpload();
		virtual void ClearUploadList();

		void GetContent();
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetAccess();

		virtual void SetBoundsInfo();
		virtual void GetBoundsInfo();

		virtual bool GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode = nil, Uint32 *outCreatorCode = nil, const SMouseMsgData *inInfo = nil);
		virtual void *GetSelectedParentFolderPath(Uint32& outPathSize, bool *outDisposePath = nil);
		void *GetSelectedFolderPath(Uint32& outPathSize);

		virtual void DoUpload(Uint16 inMods);
		virtual void DoUploadFromDrop(bool inTreeList);
		virtual void DoNewFolder();
		virtual void DoRefresh(Uint16 inMods);
		virtual void DoMove(Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inTreeList);
		virtual void DoDoubleClick(Uint16 inMods, bool inTreeList);

		virtual void KeyDown(const SKeyMsgData& inInfo);
		
	protected:							
		virtual void SearchText(const Uint8 *inText);
};

#pragma mark CMyFileExplWin
class CMyFileExplWin : public CMyItemsTreeWin<SMyFileItem>, public CMyFileWin
{
	public:
		CMyFileExplWin(CWindow *inParent, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyFileExplWin();

		CMyFileTreeView *GetFileTreeView()				{	return ((CMyFileTreeView *)mTreeView);	}
		CMyFileListView *GetFileListView()				{	return mFileListView;					}
		
		virtual bool HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder);
		virtual TFSRefObj* GetItemToUpload();
		virtual void ClearUploadList();

		void GetContent();
		void GetContentFileList();
		void GetContentFileListTimer();
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetAccess();
		
		virtual void SetBoundsInfo();
		virtual void GetBoundsInfo();

		virtual bool GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode = nil, Uint32 *outCreatorCode = nil, const SMouseMsgData *inInfo = nil);
		virtual void *GetSelectedParentFolderPath(Uint32& outPathSize, bool *outDisposePath = nil);
		void *GetSelectedFolderPath(Uint32& outPathSize);

		virtual void DoUpload(Uint16 inMods);
		virtual void DoUploadFromDrop(bool inTreeList);
		virtual void DoNewFolder();
		virtual void DoRefresh(Uint16 inMods);
		virtual void DoMove(Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inTreeList);
		virtual void DoDoubleClick(Uint16 inMods, bool inTreeList);

		// key events
		virtual void KeyDown(const SKeyMsgData& inInfo);
		
		// timer events
		virtual void Timer(TTimer inTimer);

	protected:
		CPaneView *mFilePaneView;
		CMyFileListView *mFileListView;
		
		TTimer mFileListTimer;

		virtual void SearchText(const Uint8 *inText);
};

#pragma mark CMyNewsArticleTreeWin
class CMyNewsArticleTreeWin : public CMyItemsTreeWin<SMyNewsArticle>, public CMySearchText
{
	public:
		CMyNewsArticleTreeWin(CWindow *inParent, CNZReadList *inReadList, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyNewsArticleTreeWin();
		
		CMyNewsArticleTreeView *GetNewsArticleTreeView()	{	return dynamic_cast<CMyNewsArticleTreeView*>(mTreeView);	}
		
		void GetContent();
		void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetAccess();

		virtual void SetBoundsInfo();
		virtual void GetBoundsInfo();

		void DoNewArticle();
		void DoOpenArticle();
		void DoRefresh();
		void DoDelete();
		
		virtual void KeyDown(const SKeyMsgData& inInfo);
				
	protected:
		struct
		{
			CIconButtonView *newArt, *refresh, *trash;
		} mViews;
		
		virtual void SearchText(const Uint8 *inText);
};

#pragma mark CMyNewsCategoryWin
class CMyNewsCategoryWin  : public CDragAndDropHandler, public CMySearchText
{
	public:
		CMyNewsCategoryWin(CContainerView *inContainerView, const SRect& inBounds);

		void BuildButtons(const SRect& inBounds, CContainerView *inContainerView);
		void GetNewsContent(const void *inPathData, Uint32 inPathSize);
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache) = 0;
		void SetNewsAccess(const Uint8 *inItemName, const void *inPathData, Uint32 inPathSize, Uint16 inType);

		virtual Uint16 GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID = nil) = 0;
		virtual void *GetSelectedParentBundlePath(Uint32& outPathSize, bool *outDisposePath = nil) = 0;

		virtual void DoOpenParent(Uint16 inMods);
		virtual void DoNewCategory() = 0;
		virtual void DoNewFolder() = 0;
		virtual void DoRefresh(Uint16 inMods) = 0;
		virtual void DoDoubleClick(Uint16 inMods);
		virtual void DoDelete() = 0;

	protected:
		struct
		{
			CIconButtonView *parentFolder, *newCategory, *newFolder, *info, *refresh, *trash;
		} mViews;
		
		bool News_KeyDown(const SKeyMsgData& inInfo);
};

#pragma mark CMyNewsCategoryListWin
class CMyNewsCategoryListWin : public CMyItemsListWin, public CMyNewsCategoryWin
{
	public:
		CMyNewsCategoryListWin(CWindow *inParent, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyNewsCategoryListWin();
		
		CMyNewsCategoryListView *GetNewsCategoryListView()		{	return (CMyNewsCategoryListView *)mListView;		}
		
		void GetContent();
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetAccess();	

		virtual void SetBoundsInfo();
		virtual void GetBoundsInfo();

		virtual Uint16 GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID = nil);
		virtual void *GetSelectedParentBundlePath(Uint32& outPathSize, bool *outDisposePath = nil);

		virtual void DoNewCategory();
		virtual void DoNewFolder();
		virtual void DoRefresh(Uint16 inMods);
		virtual void DoDelete();

		virtual void KeyDown(const SKeyMsgData& inInfo);
		
	protected:
		virtual void SearchText(const Uint8 *inText);
};

#pragma mark CMyNewsCategoryTreeWin
class CMyNewsCategoryTreeWin : public CMyItemsTreeWin<SNewsCatItm>, public CMyNewsCategoryWin
{
	public:
		CMyNewsCategoryTreeWin(CWindow *inParent, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyNewsCategoryTreeWin();

		CMyNewsCategoryTreeView *GetNewsCategoryTreeView()		{	return ((CMyNewsCategoryTreeView *)mTreeView);		}

		void GetContent();
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetAccess();

		virtual void SetBoundsInfo();
		virtual void GetBoundsInfo();

		virtual Uint16 GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID = nil);
		virtual void *GetSelectedParentBundlePath(Uint32& outPathSize, bool *outDisposePath = nil);
		void *GetSelectedBundlePath(Uint32& outPathSize);

		virtual void DoNewCategory();
		virtual void DoNewFolder();
		virtual void DoRefresh(Uint16 inMods);
		virtual void DoDelete();

		virtual void KeyDown(const SKeyMsgData& inInfo);
		
	protected:					
		virtual void SearchText(const Uint8 *inText);
};

#pragma mark CMyNewsCategoryExplWin
class CMyNewsCategoryExplWin : public CMyItemsTreeWin<SNewsCatItm>, public CMyNewsCategoryWin
{
	public:
		CMyNewsCategoryExplWin(CWindow *inParent, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0);
		virtual ~CMyNewsCategoryExplWin();

		CMyNewsCategoryTreeView *GetNewsCategoryTreeView()		{	return ((CMyNewsCategoryTreeView *)mTreeView);	}
		CMyNewsArticleTreeView *GetNewsArticleTreeView()		{	return mArticleTreeView;						}
		bool IsActiveArticleView()								{	return mArticleTreeView->IsFocus();				}

		void GetContent();
		void GetContentArticleList();
		void GetContentArticleListTimer();
		void GetContentArticleText();
		void GetContentArticleTextTimer();
		
		virtual void SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetContentArticleList(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache = true);
		void SetContentArticleText(const void *inPathData, Uint32 inPathSize, Uint32 inArticleID, TFieldData inData);
		void SetCurrentArticle(const void *inPathData, Uint32 inPathSize, Uint32 inCurrentID, Uint32 inNewID);
		void SelectArticle(const void *inPathData, Uint32 inPathSize, Uint32 inArticleID);
		void SetTextColor(const SMyColorInfo& inColorInfo);
		void SetAccess();
		
		virtual void SetBoundsInfo();
		virtual void GetBoundsInfo();

		virtual Uint16 GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID = nil);
		void *GetSelectedCategoryPath(Uint32& outPathSize, SGUID *outGUID = nil);
		virtual void *GetSelectedParentBundlePath(Uint32& outPathSize, bool *outDisposePath = nil);
		void *GetSelectedBundlePath(Uint32& outPathSize);

		void DoNewArticle();
		void DoOpenArticle();
		void DoReplyArticle();
		virtual void DoNewCategory();
		virtual void DoNewFolder();
		virtual void DoRefresh(Uint16 inMods);
		virtual void DoDelete();

		// key events
		virtual void KeyDown(const SKeyMsgData& inInfo);
		
		// timer events
		virtual void Timer(TTimer inTimer);
					
	protected:
		CMyNewsArticleTreeView *mArticleTreeView;

		TTimer mArticleListTimer;
		TTimer mArticleTextTimer;
				
		struct
		{
			CPaneView *paneView1, *paneView2;
			CScrollerView *articleScroll;
			CTextView *articleText;
			CIconButtonView *newArt, *replArt;
		} mNewViews;

		virtual void SearchText(const Uint8 *inText);
};

#pragma mark CMyNewsArticTextWin
class CMyNewsArticTextWin : public CWindow, public CMyPathData
{
	public:
		CMyNewsArticTextWin(CWindow *inParent, CNZReadList *inReadList, bool inIsComposing = true, Uint32 inID = 0, TPtr inPathData = nil, Uint32 inPathSize = 0, Uint32 inPathSum = 0, Uint32 inParentID = 0);
		virtual ~CMyNewsArticTextWin();
		
		void SetContentsFromFields(TFieldData inFieldData);
		void SetMyTitle(const Uint8 inTitle[]);
		void SetAccess();
		
		Uint32 GetArticID()				{	return mID;				}
		Uint32 GetPrevArticID()			{	return mPrevID;			}
		Uint32 GetNextArticID()			{	return mNextID;			}
		Uint32 Get1stChildArticID()		{	return mFirstChildID;	}
				
		void SetArticID(Uint32 inID);
		void SetPrevArticID(Uint32 inID);
		void SetNextArticID(Uint32 inID);
		void SetParentID(Uint32 inID);
		void SetFirstChildID(Uint32 inID);
				
		// stuff used when composing
		bool CanClose();
		void TitleChanged();
		bool IsComposing()				{	return mIsComposing;	}
		THdl GetTextHdl()				{	return mIsComposing ? mViews.data->GetTextHandle() : nil;								}
		Uint32 GetFlags()				{	return 0;				}
		Uint32 GetParentID()			{	return mParentID;		}
		bool IsTitleAndMsgValid()		{	return mIsComposing ? !mViews.titleTxt->IsEmpty() && !mViews.data->IsEmpty() : true;	}
				
		CNZReadList *GetReadList()		{	return mReadList;		}
		void SetTextColor(const SMyColorInfo& colorInfo);

		virtual void StateChanged();
		virtual void KeyDown(const SKeyMsgData& inInfo);
		void DoDelete();
		void DoReplyArticle();
		static CMyNewsArticTextWin *FindWindowByPathAndID(Uint32 inID, const void *inPathData, Uint32 inPathSize, Uint32 inCheckSum = 0);

	protected:
		struct
		{
			CContainerView *vc;
			CBoxView *box;
			
			CSimpleIconButtonView *goNext, *goPrev, *goParent, *goChild;
			CIconButtonView *repl, *send, *trash;
			
			CLabelView *titleLbl, *poster, *date, *category;
			CScrollerView *dataScroller;
			CTextView *titleTxt, *data;
		} mViews;
		
		CNZReadList *mReadList;
		
		Uint32 mID, mPrevID, mNextID, mParentID, mFirstChildID;
		Uint8 mIsComposing : 1;
};

#pragma mark CMyTasksWin
class CMyTasksWin : public CWindow
{
	public:
		CMyTasksWin(CWindow *inParent);
		
		// mouse events
		void UpdateButtons();
		
		CMyTaskListView *GetTaskListView()	{	return mViews.list;	}
		void SetPattern(Int16 inID)			{	mViews.box->SetStyle(boxStyle_Pattern); mViews.box->SetPattern(inID);	}

		virtual void UserZoom(const SMouseMsgData& inInfo);
				
	protected:
		struct {
			CIconButtonView *stopTaskIcon, *startTaskIcon;
			CMyTaskListView *list;
			CBoxView *box;
		} mViews;
};

#pragma mark CMyUserListWin
class CMyUserListWin : public CWindow, public CDragAndDropHandler, public CMyUserWin
{
	public:
		CMyUserListWin(CWindow *inParent);
		virtual ~CMyUserListWin();
		
		CMyUserListView *GetUserListView()	{	return mViews.list;			}
		CScrollerView *GetScrollerView()	{	return mViews.listScroll;	}
		void SetPattern(Int16 inID)			{	mViews.box->SetStyle(boxStyle_Pattern); mViews.box->SetPattern(inID);	}
		
		// key events
		virtual void KeyDown(const SKeyMsgData& inInfo);
		
		// timer events
		virtual void Timer(TTimer inTimer);

		virtual void UserZoom(const SMouseMsgData& inInfo);
		virtual void HandleDrag(CDragAndDroppable *inDD, const SMouseMsgData& inInfo);

		virtual void DoPrivateChat();
		virtual void DoPrivateMsg();
		virtual void DoGetInfo();
		
		
		
		
		
		void SetAccess();
		void DisableIcons();
		
	protected:
		struct {
			CBoxView *box;
			CMyUserListView *list;
			CScrollerView *listScroll;
			CIconButtonView *chatBtn, *msgBtn, *infoBtn, *infoDLBtn, *discBtn;
			CLabelView *searchText;
		} mViews;
		
		TTimer mSearchTimer;
		TDrag mCurrentDrag;	
		
		void SearchUser();	
};

#pragma mark CMyAboutWin
class CMyAboutWin : public CWindow
{
	public:
		CMyAboutWin();
		
		void SetStatsText(const Uint8 inStr[])	{	mStatsText->SetText(inStr);	}
		void HideUnlockButton()					{	mUnlockBtn->Hide();			}
	
	protected:
		CLabelView *mStatsText;
		CButtonView *mUnlockBtn;
};

#pragma mark CMySecretWin
class CMySecretWin : public CWindow
{
	public:
		CMySecretWin();
		
		void GetInfo(Uint8 *outCode, Uint8 *outVal, Uint8 *outExtra);
		
	protected:
		struct {
			CTextView *codeText, *valText, *extraText;
		} mViews;
};


#pragma mark CMyBannerWin
class CMyBannerWin : public CWindow
{
	public:
		CMyBannerWin(const SRect& inBounds, Uint16 inLayer = windowLayer_Standard, Uint16 inOptions = 0, Uint16 inStyle = 0, CWindow *inParent = nil);
		virtual ~CMyBannerWin();

		bool SetBanner(THdl inData, Uint32 inTypeCode, const Uint8 *inURL = nil, const Uint8 *inComment = nil, Uint16 inDepl = 0);
					
	protected:

		enum
		{
		//banner hide
		
			eMinHorizBannerImg	= 400, //30
			eMinVertBannerImg 	= 0,   //30
			eMaxHorizBannerImg	= 510, //500
			eMaxVertBannerImg	= 0,   //150
			
			
			eMinHorizBannerWin	= 430,
			eMaxVertBannerWin	= 300  //400
			
		};
		
		CContainerView *mContainerView, *mBannerContainerView;
		CScrollerView *mBannerScrollerView;
		
		CImageView *mImageView;
		CQuickTimeView *mQuickTimeView;
		
		CImageView *MakeImageView(CContainerView *inContainerView, THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment);
		CQuickTimeView *MakeQuickTimeView(CContainerView *inContainerView, THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment);
		void ClearViews();
};

#pragma mark CMyAgreementWin
class CMyAgreementWin : public CMyBannerWin
{
	public:
		CMyAgreementWin();
		
		bool SetBannerHdl(THdl inData, Uint32 inTypeCode, const Uint8 *inURL = nil, const Uint8 *inComment = nil);
		void SetTextHdl(THdl inData);
					
	protected:
		CScrollerView *mTextScrollerView;
		CButtonView *mAgreeBtn, *mDisagreeBtn;
};

#pragma mark CMyToolbar
class CMyToolbar
{
	public:
		CMyToolbar();

		// user events
		void ShowDisconnectButton()		{	mToolbarViews.disconnectBtn->Show();	}
		void HideDisconnectButton()		{	mToolbarViews.disconnectBtn->Hide();	}
		void EnableNewsButton()			{	mToolbarViews.newsBtn->Enable();		}
		void DisableNewsButton()		{	mToolbarViews.newsBtn->Disable();		}
		void EnableMsgBoardButton()		{	mToolbarViews.msgBoardBtn->Enable();		}
		void DisableMsgBoardButton()	{	mToolbarViews.msgBoardBtn->Disable();		}
		void EnableFilesButton()		{	mToolbarViews.filesBtn->Enable();		}
		void DisableFilesButton()		{	mToolbarViews.filesBtn->Disable();		}
		void ShowBroadcastButton()		{	mToolbarViews.broadcastBtn->Show();		}
		void HideBroadcastButton()		{	mToolbarViews.broadcastBtn->Hide();		}
		void ShowNewUserButton()		{	mToolbarViews.newUserBtn->Show();		}
		void HideNewUserButton()		{	mToolbarViews.newUserBtn->Hide();		}
		void ShowOpenUserButton()		{	mToolbarViews.openUserBtn->Show();		}
		void HideOpenUserButton()		{	mToolbarViews.openUserBtn->Hide();		}
		void HideOpenUserListButton()	{	mToolbarViews.openUserListBtn->Hide();	}
		void HideOpenAdmInSpectorButton()	{	mToolbarViews.openAdmInSpectorBtn->Hide();	}

		virtual void SetAccess();
		virtual void CheckToolbarState() = 0;

		void UpdateInfoBar();
		void SetStatusText(const Uint8 *inText, bool inIsAlt = false)				{	inIsAlt ? mToolbarViews.connectInfo->SetAltText(inText) : mToolbarViews.connectInfo->SetText(inText);									}
		Uint32 GetStatusText(void *outText, Uint32 inMaxSize, bool inIsAlt = false)	{	return inIsAlt ? mToolbarViews.connectInfo->GetAltText(outText, inMaxSize) : mToolbarViews.connectInfo->GetText(outText, inMaxSize);	}
		void ShowAltText()															{	mToolbarViews.connectInfo->ShowAltText();																								}

	protected:
		struct {
			CIconButtonView *optionsBtn, *serversBtn, *usersBtn, *chatBtn, *newsBtn, *msgBoardBtn, 
							*filesBtn, *tasksBtn, *broadcastBtn, *newUserBtn, 
							*openUserBtn, *openUserListBtn, *openAdmInSpectorBtn, *disconnectBtn, *helpBtn, *quitBtn;
			
			CMyConnectedInfoView *connectInfo;
		} mToolbarViews;
};


#pragma mark CMyBannerToolbarWin
class CMyBannerToolbarWin : public CMyToolbar, public CMyBannerWin
{
	public:
		CMyBannerToolbarWin();
		virtual ~CMyBannerToolbarWin();
		
		bool SetBannerHdl(THdl inData, Uint32 inTypeCode, const Uint8 *inURL = nil, const Uint8 *inComment = nil);
		void SetViewsPosition();

		virtual void SetAccess();
		virtual void CheckToolbarState();
		virtual void StateChanged();

	protected:
		
		enum
		{
			
			eButtonHeight	= 24,
			eButtonWidth 	= 24,
			eButtonSpace 	= 2,
			eButtonDepl 	= 5,
			
			eAdButtonHeight = 21,
			eAdButtonWidth	= 22
			
		};
				
		struct 
		{
			CIconButtonView * aboutBtn;
			#if EXTRA_BANNER_BUTTONS
				CIconButtonView* adBtns[3];
			#endif
		} mViews;
			
		void CalculateNewBounds(const SRect *inScrollerBounds = nil);
		void SetIconButton(CIconButtonView *inButton, SRect& inBounds);
		void SetBounds(const SRect& inBounds);
};

#pragma mark CMyEditUserWin
class CMyEditUserWin : public CWindow
{
	public:
		CMyEditUserWin(CWindow *inParent, bool inIsNewUser);
		
		void HideDeleteButton()		{	mViews.deleteBtn->Hide();	}
		
		void SetAccess(const SMyUserAccess& inAccess);
		void GetAccess(SMyUserAccess& outAccess);
		
		void SetInfo(const Uint8 inName[], const Uint8 inLogin[], const Uint8 inPass[]);
		bool GetInfo(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass);
		void SetDummyPassword()				{	if (mViews.bulletPassText) mViews.bulletPassText->SetDummyPassword();		}
		bool IsDummyPassword()				{	return mViews.bulletPassText && mViews.bulletPassText->IsDummyPassword();	}
		
		void SetNewUser(bool inIsNewUser)	{	mIsNewUser = inIsNewUser;	}
		bool IsNewUser()					{	return mIsNewUser;			}
		
		void SetLoginEditable(bool inEdit)	{	mViews.loginText->SetEditable(inEdit);	}
		
	protected:
		struct {
			CContainerView *containerView, *accessContainer;
			CScrollerView *passwordScr;
			CTextView *nameText, *loginText, *passText;
			CPasswordTextView *bulletPassText;
			CButtonView *deleteBtn;
		} mViews;
		
		CMyAccessCheckBoxView *mAccessChecks[51];
		bool mIsNewUser;
};



#pragma mark CMyOpenUserWin
class CMyOpenUserWin : public CWindow
{
	public:
		CMyOpenUserWin();
		
		void UpdateOpenButton();
		void GetLogin(Uint8 *outText)	{	outText[0] = mViews.loginText->GetText(outText+1, 31);	}
		
	protected:
		struct {
			CTextView *loginText;
			CButtonView *openBtn;
		} mViews;
};

#pragma mark CMyNewFolderWin
class CMyNewFolderWin : public CWindow
{
	public:
		CMyNewFolderWin(const Uint8 inMsg[]);
		
		void UpdateCreateButton();
		void GetName(Uint8 *outText)	{	outText[0] = mViews.nameText->GetText(outText+1, 255);	}
		
	protected:
		struct {
			CTextView *nameText;
			CButtonView *createBtn;
		} mViews;
};

#pragma mark CMyNewNewsCatWin
class CMyNewNewsCatWin : public CWindow
{
	public:
		CMyNewNewsCatWin();
		
		void UpdateCreateButton();
		void GetName(Uint8 *outText)	{	outText[0] = mViews.nameText->GetText(outText+1, 255);	}
		
	protected:
		struct {
			CTextView *nameText;
			CButtonView *createBtn;
		} mViews;
};

#pragma mark CMyUserInfoWin
class CMyUserInfoWin : public CWindow, public CMyUserWin
{
	public:
		CMyUserInfoWin(CWindow *inParent);
		~CMyUserInfoWin();
				
		void SetTextColor(const SMyColorInfo& colorInfo);
		void SetPattern(Int16 inID)				{	mViews.box->SetStyle(boxStyle_Pattern); mViews.box->SetPattern(inID);	}
		void SetContent(Uint16 inUserID, const Uint8 *inUserName, TFieldData inData = nil);
		void SetAccess();
		
		Uint16 GetUserID()						{	return mUserID;															}
		void GetUserName(Uint8 *outUserName)	{ 	UMemory::Copy(outUserName, mUserName, mUserName[0]+1);					}
		void GetUserIconID(Uint8 *outUserIconID)	{ 	UMemory::Copy(outUserIconID, mUserIconID, mUserIconID[0]+1);					}
		
		Uint32 GetAccountName(void *outName, Uint32 inMaxSize);

		virtual void DoPrivateChat();
		virtual void DoFakeRed();
		virtual void DoVisible();
		virtual void DoChangeIcon();
		virtual void DoAway();
		
		virtual void DoPrivateMsg();
		
		virtual void DoGetInfo(){};
		
		

	protected:
		struct {
			CScrollerView *textScroll;
			CBoxView *box;
			CIconButtonView *chatBtn, *msgBtn, *refreshBtn, *FakeRedBtn, *VisibleBtn, *AwayBtn, *BlockDownloadBtn, *ChangeIconBtn, *openUserBtn, *discBtn;
			CTextView *infoText;
		} mViews;
		
		Uint16 mUserID;
		Uint8 mUserName[64];
		Uint8 mUserIconID[64];
};

#pragma mark CMyFileInfoWin
class CMyFileInfoWin : public CWindow
{
	public:
		CMyFileInfoWin(CWindow *inParent, bool inIsFolder);
		virtual ~CMyFileInfoWin();
		
		void SetInfo(const Uint8 inName[], const Uint8 inComment[], const Uint8 inInfo[], Int16 inIcon);
		void GetInfo(Uint8 *outName, Uint8 *outComment);
		void EnableSaveButton()							{	mViews.saveBtn->Enable();						}
		bool HasNameChanged()							{	return mViews.nameText->HasChanged();			}
		bool HasCommentChanged()						{	return mViews.commentText->HasChanged();		}
		
		void SetOrigName(const Uint8 *inName)			{	UMemory::Copy(mOrigName, inName, inName[0]+1);	}
		const Uint8 *GetOrigNamePtr()					{	return mOrigName;								}
		
		void SetPathData(TPtr inPathData, Uint32 inPathSize)	{	mPathData = inPathData; mPathSize = inPathSize;	}
		void *GetPathPtr()								{	return mPathData;								}
		Uint32 GetPathSize()							{	return mPathSize;								}

		bool IsFolder()									{	return mIsFolder;								}

	protected:
		struct {
			CTextView *nameText, *commentText;
			CLabelView *infoText;
			CIconView *icon;
			CButtonView *saveBtn;
		} mViews;
		
		TPtr mPathData;
		Uint32 mPathSize;
		Uint8 mOrigName[256];
		bool mIsFolder;
};


#pragma mark CMyChatInviteWin
class CMyChatInviteWin : public CWindow
{
	public:
		CMyChatInviteWin(CWindow *inParent);
		
		void SetNameText(const Uint8 *inStr)	{	mNameLabel->SetText(inStr);		}
		void SetChatID(Uint32 inID)				{	mChatID = inID;					}
		Uint32 GetChatID() const				{	return mChatID;					}
		
	protected:
		CLabelView *mNameLabel;
		Uint32 mChatID;
};

#pragma mark CMyChatSubjectWin
class CMyChatSubjectWin : public CWindow
{
	public:
		CMyChatSubjectWin();
		
		void SetSubject(const Uint8 *inText)	{	mTextView->SetText(inText+1, inText[0]);			}
		void GetSubject(Uint8 *outText) const	{	outText[0] = mTextView->GetText(outText+1, 255);	}
		void SelectAll()						{	mTextView->SetSelection(0, max_Uint32);				}
		
	protected:
		CTextView *mTextView;
};



#pragma mark CMyConfirmDisconnectWin
class CMyConfirmDisconnectWin : public CWindow
{
	public:
		CMyConfirmDisconnectWin(const Uint8 *inServerName, Uint32 inTaskCount);
};

#pragma mark CMyConfirmQuitWin
class CMyConfirmQuitWin : public CWindow
{
	public:
		CMyConfirmQuitWin(const Uint8 *inServerName, Uint32 inTaskCount);
};

#pragma mark CMyWaitDisconnectWin
class CMyWaitDisconnectWin : public CWindow
{
	public:
		CMyWaitDisconnectWin();
		virtual ~CMyWaitDisconnectWin();
		
		// timer events
		virtual void Timer(TTimer inTimer);

	protected:
		struct {
			CLabelView *textLabel;
		} mViews;
		
		TTimer mMsgTimer;
		
		void CheckTasksStatus();
};

#pragma mark CMyWaitQuitWin
class CMyWaitQuitWin : public CWindow
{
	public:
		CMyWaitQuitWin();
		virtual ~CMyWaitQuitWin();
		
		// timer events
		virtual void Timer(TTimer inTimer);

	protected:
		struct {
			CLabelView *textLabel;
		} mViews;
		
		TTimer mMsgTimer;
		
		void CheckTasksStatus();
};

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Tracker ₯₯
	
#pragma mark ₯constants₯	
enum
{
	TrackCmd_AddTracker			= 202,
	TrackCmd_Filter				= 204,
	TrackCmd_Info				= 206,
	TrackCmd_ServerConnect		= 209,
	TrackCmd_GuestHit			= 210,
	TrackCmd_AccntHit			= 211,
	TrackCmd_SaveTracker		= 212,
	TrackCmd_Disclose			= 213,

	searchOpt_NoCase 			= 1,
	searchOpt_Case 				= 2,
	searchOpt_Remove 			= 3
};

#pragma mark ₯structs₯
#pragma mark SMyServerInfo
struct SMyServerInfo{
	TIcon Icon;
	Uint32 Address;
	Uint16 Port,uCount,Flags;
	Uint8 User[50];
	Uint8 *Name;
	Uint8 *Desc;
	Uint8 *SearchName;
	Uint8 *SearchDesc;
};

#pragma mark SMyTrackerInfo
struct SMyTrackerInfo{
	Uint8 Login[256];
	Uint8 Pass[256];
	Uint8 Address[256];
	Uint8 Name[256];
	Uint8 Comments[256];
	Uint16 hasAccount;
	Uint16 nTrackerID;
};

#pragma mark STrackDiscInfo
struct STrackDiscInfo{
	Uint16 nTrackID;
	Uint8 nDisclosure;
};

#pragma mark SWordPtr
struct SWordPtr{
	Uint8 *startWord;
	Uint16 wordLen;
	Uint16 negative;		// negative = 1, positive = 0
};

#pragma mark ₯classes₯

class CMyServerWindow;

#pragma mark CMyServerTreeView
class CMyServerTreeView : public CTabbedTreeView<SMyServerInfo>
{
	public:
		CMyServerTreeView(CViewHandler *inHandler, const SRect& inBounds, CMyServerWindow *inServerWin);
		~CMyServerTreeView();
		
		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2, Uint8 inTabPercent3);
		void SetTabs(Uint16 inTabWidth1, Uint16 inTabWidth2);
		void GetTabs(Uint16& outTabWidth1, Uint16& outTabWidth2);

		Uint32 AddTracker(SMyServerInfo *inTracker);
		Uint32 AddServer(Uint32 inParentIndex, SMyServerInfo *inServer);
		void RefreshTrackers();
		void RefreshTracker(Uint32 inTrackerID);

		Uint32 GetSelectedTrackerID(bool *outIsBookmark = nil);
		const Uint8 *GetSelectedBookmarkName();
		SMyServerInfo *GetSelectedServInfo(bool &outBookmark);
		bool GetSelectedServInfo(bool &outBookmark, Uint8 *outName, Uint8 *outAddress);
				
		Uint32 GetTotalServerCount();
		bool SetTrackerDisclosure(Uint16 inTrackID, Uint8 inDisclosure);
		Uint8 GetTrackerDisclosure(Uint16 inTrackID);
		Uint32 GetTrackerIndex(Uint16 inTrackID);
		void ExpandAllTrackers();
		
		void RemoveServers();
		void RemoveTracker(Uint16 inTrackerID, bool inTracker);
		void SetStatusMsg(Uint16 inTrackerID, const Uint8 inMsg[]);

		// tree events
		virtual void SelectionChanged(Uint32 inTreeIndex, SMyServerInfo *inTreeItem, bool inIsSelected);
		virtual void DisclosureChanged(Uint32 inTreeIndex, SMyServerInfo *inTreeItem, Uint8 inDisclosure);

	protected:
		CMyServerWindow *mServerWin;
		
		Uint32 mMaxWidth;
		Uint8 mBigDesc[256];
		
		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyServerInfo *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
};

#pragma mark CMyServerWindow
class CMyServerWindow : public CWindow
{
	public:
		CMyServerWindow(CWindow *inParent);
		CMyServerTreeView *GetServerTreeView()		{	return mServerTreeView;			}
		
		void AddChatText(const Uint8 *inStartText, const void *inText, Uint32 inTextSize);
		void SetServerNum(Uint8 *inText)																		{ mServerNumLabl->SetText(inText); }

		void GetFilterText(Uint8 *text)																			{ text[0] = mFilterText->GetText(text+1, 255); }
		virtual void UserZoom(const SMouseMsgData& inInfo);
		void SetEnableTrash(bool inEnable)			{	mTrash->SetEnable(inEnable);	}

	protected:
		CLabelView *mServerNumLabl;
		CTextView *mTxtChatz, *mFilterText;
		CIconButtonView *mTrash;
		CScrollerView *mScrChatz;
		CMyServerTreeView *mServerTreeView;
};

#pragma mark CMyTrackServWindow
class CMyTrackServWindow : public CWindow
{
	public:
		CMyTrackServWindow(CWindow *inParent);
		
		short IsAccount()				{ return mAccount->GetMark(); 				}
		
		void GuestHit()		{ mScrPass->Disable(); mScrPass->Refresh(); mScrLogin->Disable(); mScrLogin->Refresh(); mLogin->Disable(); mPass->Disable();	}
		void AccntHit()		{ mScrPass->Enable(); mScrPass->Refresh();  mScrLogin->Enable(); mScrLogin->Refresh(); mLogin->Enable(); mPass->Enable();		}

		void GetName(Uint8 *text)		{ text[0] = mName->GetText(text+1, 255);	}
		void GetAddr(Uint8 *text) 		{ text[0] = mAddr->GetText(text+1, 255); 	}
		void GetLogin(Uint8 *text) 		{ text[0] = mLogin->GetText(text+1, 255); 	}
		void GetPass(Uint8 *text) 		{ text[0] = mPass->GetText(text+1, 255); 	}
		
		void SetName(Uint8 *in) 		{ mName->SetText(in + 1, in[0]); 			}
		void SetAddr(Uint8 *in) 		{ mAddr->SetText(in + 1, in[0]); 			}
		void SetLogin(Uint8 *in) 		{ mLogin->SetText(in + 1, in[0]); 			}
		void SetPass(Uint8 *in) 		{ mPass->SetText(in + 1, in[0]); 			}
		
		void SetDiscloseIcon(Uint32 in)	{ mDisclose->SetIconID(in); 				}
		void Disclose();
		
	protected:
		CTextView *mName, *mAddr, *mLogin, *mPass;
		CSimpleIconBtnView *mDisclose;
		CCheckBoxView *mGuest, *mAccount;
		CScrollerView *mScrPass, *mScrLogin;
};

#pragma mark CMyTracker
class CMyTracker
{
	public:
		CMyTracker(CWindow *inParent);
		~CMyTracker();
										
		CMyServerWindow *GetServersWindow()				{	return mServerWin;		}
		void ShowServersWindow();
		void HideServersWindow()						{	mServerWin->Hide();		}
		
		bool CloseWindow(CWindow *inWindow);
		bool KeyCommand(Uint32 inCmd, const SKeyMsgData& inInfo);
		bool WindowHit(CWindow *inWindow, const SHitMsgData& inInfo);
		void Timer(TTimer inTimer);
		
		void SetTabs(Uint16 inTabWidth1, Uint16 inTabWidth2);
		void GetTabs(Uint16& outTabWidth1, Uint16& outTabWidth2);

		void AddTrackerInList(SMyTrackerInfo *inTracker);
		void AddTrackerInList(Uint8 *Name, Uint8 *Comments, Uint8 *inAddress, Uint8 *Login, Uint8 *Pass, short isAccount, Uint16 inTrackerID);
		void AddBookmarksInTree();
		void AddTrackersInTree();
		void AddTrackerInTree(SMyTrackerInfo *inTracker);
		Uint32 AddListFromData(Uint16 inTrackerID, const Uint8 *inData, Uint32 inDataSize);
		void AddServerInTree(Uint32 inParentIndex, Uint32 inParentIndexView, Uint8 *inName, Uint8 *inDesc, Uint32 inAddress, Uint16 inPort, Uint16 inCount, Uint16 inFlags);
		
		void UpdateServersCount(Uint32 inTrackerID);
		void UpdateTrackerInList(Uint16 inTrackerID, Uint8 *Name, Uint8 *Comments, Uint8 *inAddress, Uint8 *Login, Uint8 *Pass, short isAccount);
		void UpdateTrackerInTree(SMyTrackerInfo *inTracker);
		bool EditSelectedTracker();
		
		void RemoveTrackerInList(Uint16 inTrackerID);
		void RemoveTrackersInTree();
		void RemoveTrackerInTree(Uint16 inTrackerID, bool inTracker);
		void RemoveSelectedItem();

		void SetStatusMsg(Uint16 inTrackerID, const Uint8 inMsg[]);
		void SetServerCountLabel();
		bool IsInList(Uint32 inAddress, Uint16 inPort);

		void FilterServerList();
		void FilterServerList(Uint8 *inText);
		short FilterServer(Uint8 *inText, SMyServerInfo *inServer);

		bool RefreshBookmarks();
		void RefreshTrackers();
		void RefreshTrackers(Uint16 inMods);
		void RefreshTracker(Uint16 inTrackerID);
		
		void SetDefaultTracker();
		void ExpandDefaultTracker();
		bool GetSelectedServInfo(Uint8 *outName, Uint8 *outAddress, Uint8 *outLogin, Uint8 *outPassword, bool *outUseCrypt);

		// tracker list stuff
		Uint32 GetTrackerIndex(Uint16 inTrackerID);
		SMyTrackerInfo *GetTrackerInfo(Uint32 inTrackerIndex);
		SMyTrackerInfo *GetTrackerInfoByID(Uint16 inTrackerID);

		Uint32 WritePrefs(TFSRefObj* inFile, Uint32 inOffset);
		Uint32 ReadPrefs(TFSRefObj* inFile, Uint32 inOffset, Uint16 inTabWidth1, Uint16 inTabWidth2, bool iconset);

	protected:				
		CMyServerWindow *mServerWin;
		CMyServerTreeView *mServerTreeView;

		CPtrList<SMyTrackerInfo> mTrackerList;
		CPtrTree<SMyServerInfo> mServerTree;
		
		Uint16 mLastTrackerID;
		TTimer mTimer;

		static void TimerProc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);
};


/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Tasks ₯₯

#pragma mark CMyTask
class CMyTask
{
	public:
		CMyTask(const Uint8 inDesc[]);
		virtual ~CMyTask();
		
		virtual void Process();
		virtual bool QueueUnder();	// whether tasks under this should remain queued

		virtual bool CanKillTask()	{	return true;			}
		virtual void TryKillTask()	{							}
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		
		bool IsFinished()			{	return mIsFinished;		}
		bool IsStandAlone()			{	return mIsStandAlone;	}
		bool IsQueued()				{	return mStage == 0;		}
		
	protected:
		Uint16 mStage;
		Uint16 mIsFinished		: 1;
		Uint16 mIsStandAlone	: 1;
		
		void ShowProgress(Uint32 inVal, Uint32 inMax, const Uint8 inDesc[] = nil);
		void Finish();
		void CheckServerError(Uint32 inError, TFieldData inData);
};

#pragma mark CMyTransactTask
class CMyTransactTask : public CMyTask
{
	public:
		CMyTransactTask(const Uint8 inDesc[]);
		virtual ~CMyTransactTask();
	
	protected:
		StFieldData mFieldData;
		TTransactSession mTranSession;
		
		TTransactSession NewSendSession(Uint16 inType);
};

#pragma mark CMyConnectTask
class CMyConnectTask : public CMyTask
{
	public:
		CMyConnectTask();
		virtual ~CMyConnectTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyDummyTask
class CMyDummyTask : public CMyTask
{
	public:
		CMyDummyTask(const Uint8 inDesc[], bool inIsLoginTask = false);
		
		bool IsLoginTask();
		void FinishDummyTask();

	protected:
		bool mIsLoginTask;
};

#pragma mark CMyGetOldNewsTask
class CMyGetOldNewsTask : public CMyTransactTask
{
	public:
		CMyGetOldNewsTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyGetFileListTask
class CMyGetFileListTask : public CMyTransactTask
{
	public:
		CMyGetFileListTask(const void *inPathData, Uint32 inPathSize, bool inListParent = true);
		virtual ~CMyGetFileListTask();

		bool IsThisPathData(const void *inPathData, Uint32 inPathSize);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		TPtr mPathData;
		Uint32 mPathSize;
	
		Uint8 mName[256];
		bool mListParent;
};

#pragma mark CMyStartupGetFileListTask
class CMyStartupGetFileListTask : public CMyGetFileListTask
{
	public:
		CMyStartupGetFileListTask(const void *inPathData, Uint32 inPathSize, Uint8 *inFileName = nil);

		virtual void Process();
	
	protected:
		Uint8 mFileName[256];
};

#pragma mark CMyGetNewsCatListTask
class CMyGetNewsCatListTask : public CMyTransactTask
{
	public:
		CMyGetNewsCatListTask(const void *inPathData, Uint32 inPathSize, bool inListParent = true);
		virtual ~CMyGetNewsCatListTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		TPtr mPathData;
		Uint32 mPathSize;
		
		Uint8 mName[256];
		bool mListParent;
};

#pragma mark CMyStartupGetNewsCatListTask
class CMyStartupGetNewsCatListTask : public CMyGetNewsCatListTask
{
	public:
		CMyStartupGetNewsCatListTask(const void *inPathData, Uint32 inPathSize, Uint8 *inCategoryName = nil);

		virtual void Process();
	
	protected:
		Uint8 mCategoryName[256];
};

#pragma mark CMyDeleteNewsFldrItemTask
class CMyDeleteNewsFldrItemTask : public CMyTransactTask
{
	public:
		CMyDeleteNewsFldrItemTask(const Uint8* inItemName, Uint16 inType, const void *inPathData, Uint32 inPathSize);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();

	protected:
		Uint8 mName[256];
};

#pragma mark CMyNewNewsFldrTask
class CMyNewNewsFldrTask : public CMyTransactTask
{
	public:
		CMyNewNewsFldrTask(const void *inPathData, Uint32 inPathSize, const Uint8 inName[]);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyNewNewsCatTask
class CMyNewNewsCatTask : public CMyTransactTask
{
	public:
		CMyNewNewsCatTask(const void *inPathData, Uint32 inPathSize, const Uint8 inName[]);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyGetNewsArtListTask
class CMyGetNewsArtListTask : public CMyTransactTask
{
	public:
		CMyGetNewsArtListTask(const void *inPathData, Uint32 inPathSize);
		virtual ~CMyGetNewsArtListTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		TPtr mPathData;
		Uint32 mPathSize;
		Uint8 mName[256];
};

#pragma mark CMyGetNewsArticleDataTask
class CMyGetNewsArticleDataTask : public CMyTransactTask
{
	public:
		CMyGetNewsArticleDataTask(const void *inPathData, Uint32 inPathSize, Uint32 inID, const Uint8 inArticTitle[], const Int8 inFlavor[], Uint32 inOptions = 0);
		virtual ~CMyGetNewsArticleDataTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		bool GetThisNewsArticle(const void *inPathData, Uint32 inPathSize, Uint32 inID);
		virtual void Process();
		
	protected:
		TPtr mPathData;
		Uint32 mPathSize;
		
		Uint32 mID;
		Uint8 mName[256];
		Uint8 mInfoAction;
};

#pragma mark CMyPostNewsTextArticle
class CMyPostNewsTextArticle : public CMyTransactTask
{
	public:
		CMyPostNewsTextArticle(const void *inPathData, Uint32 inPathSize, Uint32 inParentID, const Uint8 inTitle[], Uint32 inFlags, const Uint8 *inData, Uint32 inDataSize);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyNewsDeleteArticTask
class CMyNewsDeleteArticTask : public CMyTransactTask
{
	public:
		CMyNewsDeleteArticTask(const void *inPathData, Uint32 inPathSize, Uint32 inArticID, Uint8 inTitle[] = "\p", bool inDeleteChildren = false);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyOldPostNewsTask
class CMyOldPostNewsTask : public CMyTransactTask
{
	public:
		CMyOldPostNewsTask(const void *inData, Uint32 inDataSize);
		virtual void Process();
};

#pragma mark CMySendPrivMsgTask
class CMySendPrivMsgTask : public CMyTransactTask
{
	public:
		CMySendPrivMsgTask(Uint16 inYourID, const void *inMyMsg, Uint32 inMyMsgSize, Uint16 inOptions, const void* inYourMsg = NULL, Uint32 inYourMsgSize = 0);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyBroadcastTask
class CMyBroadcastTask : public CMyTransactTask
{
	public:
		CMyBroadcastTask(const void *inData, Uint32 inDataSize);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyKeepConnectionAliveTask
class CMyKeepConnectionAliveTask : public CMyTransactTask
{
	public:
		CMyKeepConnectionAliveTask();

		virtual void Process();
};

#pragma mark CMyGetOnlineUsersTask
class CMyGetOnlineUsersTask : public CMyTransactTask
{
	public:
		CMyGetOnlineUsersTask(Uint8 *inDesc);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyLoginTask
class CMyLoginTask : public CMyTransactTask
{
	public:
		CMyLoginTask(const Uint8 inLogin[], const Uint8 inPassword[]);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
};

#pragma mark CMyAgreedTask
class CMyAgreedTask : public CMyTransactTask
{
	public:
		CMyAgreedTask();
		virtual ~CMyAgreedTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyGetBannerTimeTask
class CMyGetBannerTimeTask : public CMyTask
{
	public:
		CMyGetBannerTimeTask(Uint8 *inDesc, THdl inAddress, Uint32 inMsg = 0, bool inNext = false);
		virtual ~CMyGetBannerTimeTask();
		
		virtual void Process();
		bool IsLoginTask();
				
	protected:
		StHttpTransact mHttpTransact;
		
		Uint8 mStartAddress[256];
		Uint8 mAddress[256];
		Uint32 mMsg;

		TTimer mFinishTimer;
		static void FinishTimerProc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);
		virtual void FinishBannerTask();
};

#pragma mark CMyGetBannerTask
class CMyGetBannerTask : public CMyGetBannerTimeTask
{
	public:
		CMyGetBannerTask(Uint8 *inDesc, THdl inAddress, Uint32 inMsg, bool inNext = false);

		virtual void Process();
				
	protected:
		Uint8 mImgSrc[256];
		Uint8 mImgHref[256];
		Uint8 mImgComment[256];
		bool mNoCache;
				
		virtual void FinishBannerTask();

		bool SearchImageTag(void *inHtml, Uint32 inHtmlSize);
		bool CheckURL(Uint8 *ioUrl, bool inProtocolsAllowed);
};

#pragma mark CMyLaunchUrlTask
class CMyLaunchUrlTask : public CMyTask
{
	public:
		CMyLaunchUrlTask(THdl inAddress);
		virtual ~CMyLaunchUrlTask();

		virtual void Process();
		
	protected:
		StHttpTransact mHttpTransact;
		Uint8 mAddress[256];
		
		TTimer mFinishTimer;
		static void FinishTimerProc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);
};

#pragma mark CMyDownloadTask
class CMyDownloadTask : public CMyTransactTask
{
	public:
		CMyDownloadTask(const void *inPathData, Uint32 inPathSize, const Uint8 *inFileFldrName, const Uint8 *inValidatedName, bool inQueued);
		virtual ~CMyDownloadTask();
		
		virtual bool ShowWaitingCount(Uint32 inRefNum, Uint32 inWaitingCount);
		
		virtual bool QueueUnder();
		void Start();

	protected:
		TTransport mTpt;
		
		Uint32 mStartSecs;
		Uint32 mTotalSize, mDownloadedSize;
		Uint32 mDisplayedSize, mDisplayedBPS, mDisplayedETR;
		Uint32 mRefNum;
		
		TPtr mPathData;
		Uint32 mPathSize;
		Uint8 mFileFldrName[256];
		Uint8 mValidatedName[256];

		bool mWasQueued;
		Uint32 mWaitingCount;
		Uint32 mMaxWaitingCount;
		
		// send padding data while wait in the server queue
		TTimer mWaitingTimer;
		void StartWaitingTimer();
		void StopWaitingTimer();
		void SendWaitingData();
		static void WaitingTimerProc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);

};

#pragma mark CMyDownloadFileTask
class CMyDownloadFileTask : public CMyDownloadTask
{
	public:
		CMyDownloadFileTask(
					const void *inPathData, Uint32 inPathSize, 
					const Uint8 *inFileName, const Uint8 *inValidatedName, 
					TFSRefObj* inDestSpec, 
					bool inResume, bool inQueued, bool inCreateFile, bool inViewAfterDload,
					bool *outDropEnd, bool *outDropError);
		virtual ~CMyDownloadFileTask();
		
		virtual bool ShowWaitingCount(Uint32 inRefNum, Uint32 inWaitingCount);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		TFSRefObj* mFile;
		Uint32 mFileSize;
		THdl mResumeData;

		bool *mDropEnd;
		bool *mDropError;
		
		bool mViewAfterDownload;
};

#pragma mark CMyViewFileTask
class CMyViewFileTask : public CMyDownloadTask
{
	public:
		CMyViewFileTask(
					const void *inPathData, Uint32 inPathSize, const Uint8 *inFileName, 
					const Uint8 *inValidatedName, Uint32 inTypeCode, Uint32 inCreatorCode, 
					bool inQueued, bool inNewWindow);
		virtual ~CMyViewFileTask();

		virtual bool ShowWaitingCount(Uint32 inRefNum, Uint32 inWaitingCount);

		void EndViewFileWin();
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);

		virtual void Process();
		virtual bool CanKillTask();
		virtual void TryKillTask();
		
		void SetProgress(Uint32 inTotalSize, Uint32 inDownloadedSize);
		void ShowFileProgress();

	protected:
		Uint32 mFileSize;
		Uint32 mTypeCode;
		Uint32 mCreatorCode;

		bool mCanKillTask;
		bool mTryKillTask;
		bool mNewViewFileWin;

		CMyViewFileWin* mViewFileWin;
		bool MakeViewFileWindow();
		
		bool IsTextFile();
		bool IsImageFile();
		bool IsQuickTimeFile();
		bool StartQuickTime();
};

#pragma mark CMyDownloadFldrTask
class CMyDownloadFldrTask : public CMyDownloadTask
{
	public:
		CMyDownloadFldrTask(const void *inPathData, Uint32 inPathSize, const Uint8 *inFldrName, TFSRefObj* inDestSpec, bool inResume, bool inQueued, bool inCreateFldr, bool *outDropEnd = nil, bool *outDropError = nil);
		virtual ~CMyDownloadFldrTask();

		virtual bool ShowWaitingCount(Uint32 inRefNum, Uint32 inWaitingCount);
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		TFSRefObj* mFldr;
		Uint16 mTotalItems, mDownloadedItems;
				
		// file stuff
		TFSRefObj* mFile;
		TPtr mFilePath;
		Uint32 mFilePathSize;
		Uint32 mFileTotalSize, mFileDlSize;
		Uint32 mHeaderSize;
		
		bool *mDropEnd;
		bool *mDropError;
};

#pragma mark CMyDownloadBannerTask
class CMyDownloadBannerTask : public CMyTransactTask
{
	public:
		CMyDownloadBannerTask(Uint32 inBannerType, Uint8 *inBannerURL);
		~CMyDownloadBannerTask();

		virtual void Process();
		
	protected:
		TTransport mTpt;
		
		TPtr mBanner;
		Uint32 mBannerType;
		Uint8 mBannerURL[256];	
		Uint32 mBannerSize, mDownloadedSize;
		Uint32 mRefNum;
};

#pragma mark CMyKillDownloadTask
class CMyKillDownloadTask : public CMyTransactTask
{
	public:
		CMyKillDownloadTask(Uint32 inRefNum);

		virtual void Process();
};

#pragma mark CMyUploadTask
class CMyUploadTask : public CMyTransactTask
{
	public:
		CMyUploadTask(TFSRefObj* inSpec, bool inQueued);
		virtual ~CMyUploadTask();

		virtual bool QueueUnder();
		void Start();
		
	protected:
		TTransport mTpt;
		
		TFSRefObj* mFile;
		Uint8 mFileFldrName[256];
		
		Uint32 mStartSecs;
		Uint32 mTotalSize, mUploadedSize;
		Uint32 mSendSize, mSendSecs;
		Uint32 mDisplayedSize, mDisplayedBPS, mDisplayedETR;
		Uint32 mRefNum;

		bool mWasQueued;
};

#pragma mark CMyUploadFileTask
class CMyUploadFileTask : public CMyUploadTask
{
	public:
		CMyUploadFileTask(const void *inPathData, Uint32 inPathSize, TFSRefObj* inFile, bool inResume, bool inQueued);
		virtual ~CMyUploadFileTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
		
	protected:
		bool mDisplayedFinish;
};

#pragma mark CMyUploadFldrTask
class CMyUploadFldrTask : public CMyUploadTask
{
	public:
		CMyUploadFldrTask(const void *inPathData, Uint32 inPathSize, TFSRefObj* inFldr, bool inResume, bool inQueued);
		virtual ~CMyUploadFldrTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
		
	protected:
		CMyDLFldr *mFldr;
		Uint32 mFileTotalSize, mFileUlSize;
		Uint32 mTotalItems, mUploadedItems;
		Uint16 mResumeHdrSize;
};

#pragma mark CMyOpenUserListTask
class CMyOpenUserListTask : public CMyTransactTask
{
	public:
		CMyOpenUserListTask();
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};
#pragma mark CMyAdmInSpectorTask
class CMyAdmInSpectorTask : public CMyTransactTask
{
	public:
		CMyAdmInSpectorTask();
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};
#pragma mark CMySetUserListTask
class CMySetUserListTask : public CMyTransactTask
{
	public:
		CMySetUserListTask(TFieldData inData);
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyNewUserTask
class CMyNewUserTask : public CMyTransactTask
{
	public:
		CMyNewUserTask(const Uint8 inName[], const Uint8 inLogin[], const Uint8 inPass[], const SMyUserAccess& inAccess);
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyDeleteUserTask
class CMyDeleteUserTask : public CMyTransactTask
{
	public:
		CMyDeleteUserTask(const Uint8 inLogin[]);
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMySetUserTask
class CMySetUserTask : public CMyTransactTask
{
	public:
		CMySetUserTask(const Uint8 inName[], const Uint8 inLogin[], const Uint8 inPass[], const SMyUserAccess& inAccess);
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyOpenUserTask
class CMyOpenUserTask : public CMyTransactTask
{
	public:
		CMyOpenUserTask(const Uint8 inLogin[]);
		
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyDisconnectUserTask
class CMyDisconnectUserTask : public CMyTransactTask
{
	public:
		CMyDisconnectUserTask(Uint16 inID, const Uint8 inName[], Uint8 inBanOptions = 0);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};


#pragma mark CMyIconChangeTask
class CMyIconChangeTask : public CMyTransactTask
{
	public:
		CMyIconChangeTask(Uint16 inID, Uint16 inIconID);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};


#pragma mark CMyFakeRedTask
class CMyFakeRedTask : public CMyTransactTask
{
	public:
		CMyFakeRedTask(Uint16 inID, Uint16 infakebool);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyVisibleTask
class CMyVisibleTask : public CMyTransactTask
{
	public:
		CMyVisibleTask(Uint16 inID, Uint16 invisiblebool);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};
#pragma mark CMyStandardMessageTask
class CMyStandardMessageTask : public CMyTransactTask
{
	public:
		CMyStandardMessageTask(Uint16 inID, Uint16 inmessage);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyBlockDownloadTask
class CMyBlockDownloadTask : public CMyTransactTask
{
	public:
		CMyBlockDownloadTask(Uint16 inID, Uint16 inblockdownload);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyCrazyServerTask
class CMyCrazyServerTask : public CMyTransactTask
{
	public:
		CMyCrazyServerTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};
#pragma mark CMyAdminSpectorTask
class CMyAdminSpectorTask : public CMyTransactTask
{
	public:
		CMyAdminSpectorTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyAwayTask
class CMyAwayTask : public CMyTransactTask
{
	public:
		CMyAwayTask(Uint16 inID, Uint16 inawaybool);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyNickChangeTask
class CMyNickChangeTask : public CMyTransactTask
{
	public:
		CMyNickChangeTask(Uint16 inID, Uint8 inNickName[]);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};





#pragma mark CMyGetClientInfoTask
class CMyGetClientInfoTask : public CMyTransactTask
{
	public:
		CMyGetClientInfoTask(Uint16 inUserID, const Uint8 *inUserName);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
		
	protected:
		Uint16 mUserID;
		Uint8 mUserName[64];
};

#pragma mark CMyDeleteFileTask
class CMyDeleteFileTask : public CMyTransactTask
{
	public:
		CMyDeleteFileTask(const void *inPathData, Uint32 inPathSize, const Uint8 inFileName[], bool inIsFolder);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyNewFolderTask
class CMyNewFolderTask : public CMyTransactTask
{
	public:
		CMyNewFolderTask(const void *inPathData, Uint32 inPathSize, const Uint8 inName[]);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyGetTrackServListTask
class CMyGetTrackServListTask : public CMyTransactTask
{
	public:
		CMyGetTrackServListTask(const Uint8 inAddr[], const Uint8 inName[], Uint16 inTrackerID, const Uint8 inLogin[], const Uint8 inPasswd[]);
		virtual ~CMyGetTrackServListTask();
		
		Uint16 GetTrackerID();
		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		Uint8 mName[256];
		TTransport mTpt;
		Uint32 mCount, mTotalCount;
		THdl mMsgData;
		Uint32 mMsgDataSize, mRcvdSize;
		Uint16 mMsgType;
		Uint8 mLoginHeader[sizeof(Uint32) + sizeof(Uint16) + 32 + 32];
		bool mClearedList;
		
		SInternetNameAddress mTrackerAddress;
		Uint16 mTrackerID;
};

#pragma mark CMyGetFileInfoTask
class CMyGetFileInfoTask : public CMyTransactTask
{
	public:
		CMyGetFileInfoTask(const void *inPathData, Uint32 inPathSize, const Uint8 inFileName[]);
		virtual ~CMyGetFileInfoTask();

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		TPtr mPathData;
		Uint32 mPathSize;
		Uint8 mName[256];
};

#pragma mark CMySetFileInfoTask
class CMySetFileInfoTask : public CMyTransactTask
{
	public:
		CMySetFileInfoTask(const void *inPathData, Uint32 inPathSize, const Uint8 inFileName[], const Uint8 inNewName[], const Uint8 inComment[]);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyMoveFileTask
class CMyMoveFileTask : public CMyTransactTask
{
	public:
		CMyMoveFileTask(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, const void *inNewPath, Uint32 inNewPathSize);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyMakeFileAliasTask
class CMyMakeFileAliasTask : public CMyTransactTask
{
	public:
		CMyMakeFileAliasTask(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, const void *inDestPath, Uint32 inDestPathSize);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	protected:
		Uint8 mName[256];
};

#pragma mark CMyInviteNewChatTask
class CMyInviteNewChatTask : public CMyTransactTask
{
	public:
		CMyInviteNewChatTask(const Uint32 *inUserIDs, Uint32 inCount);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
};

#pragma mark CMyJoinChatTask
class CMyJoinChatTask : public CMyTransactTask
{
	public:
		CMyJoinChatTask(Uint32 inChatID);

		virtual Uint32 GetShortDesc(Uint8 *outText, Uint32 inMaxSize);
		virtual void Process();
	
	protected:
		Uint32 mChatID;
};

#pragma mark CChatLog
class CChatLog
{
    TFSRefObj* mRef;
    bool mEnabled;
    UInt32 mPos;
public:
	CChatLog();
	~CChatLog();

	void Enable(bool on);
	void AppendLog(const UInt8* type, const UInt8* user, const UInt8 *msg, UInt32 msgZ);

private:
	void OpenLog();
	void CloseLog();
};

#pragma mark CLoginISP_Task


#pragma mark CStMonica_Task

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Application ₯₯

class CMyAdminWin;
class CMyAdmInSpectorWin;

#pragma mark CMyApplication
class CMyApplication : public CApplication
{
	// friend classes
	friend class CMyTask;
	friend class CMyTransactTask;
	friend class CMyConnectTask;
	friend class CMyLoginTask;
	friend class CMyAgreedTask;
	friend class CMyGetOnlineUsersTask;
	friend class CMyViewFileTask;
	friend class CMyDownloadTask;
	friend class CMyDownloadFileTask;
	friend class CMyDownloadFldrTask;
	friend class CMyDownloadBannerTask;
	friend class CMyUploadTask;
	friend class CMyUploadFileTask;
	friend class CMyUploadFldrTask;
	friend class CMyDeleteFileTask;
	friend class CMyNewFolderTask;
	friend class CMyGetOldNewsTask;
	friend class CMyGetFileListTask;
	friend class CMyOpenUserTask;
	friend class CMyOpenUserListTask;
	friend class CMyGetTrackServListTask;
	friend class CMyGetClientInfoTask;
	friend class CMyIconChangeTask;
	friend class CMyFakeRedTask;
	friend class CMyVisibleTask;
	friend class CMyStandardMessageTask;
	friend class CMyBlockDownloadTask;
	friend class CMyCrazyServerTask;
	friend class CMyAwayTask;
	friend class CMyNickChangeTask;
	friend class CMyAdmInSpectorTask;

	
	friend class CMyGetFileInfoTask;
	friend class CMySetFileInfoTask;
	friend class CMyMoveFileTask;
	friend class CMyMakeFileAliasTask;
	friend class CMyGetNewsCatListTask;
	friend class CMyNewNewsFldrTask;
	friend class CMyNewNewsCatTask;
	friend class CMyGetNewsArtListTask;
	friend class CMyGetNewsArticleDataTask;
	friend class CMyNewsDeleteArticTask;
	friend class CMyPostNewsTextArticle;
	friend class CMyFileWin;
	friend class CMyFileListWin;
	friend class CMyFileTreeWin;
	friend class CMyFileExplWin;
	friend class CMySaveFileWin;
	friend class CMyViewFileWin;
	friend class CMyUserListWin;
	friend class CMyPrivateChatWin;
	friend class CMyPrivMsgWin;
	friend class CMyUserInfoWin;
	friend class CMyJoinChatTask;
	friend class CMyInviteNewChatTask;
	friend class CTrackersListWin;
	friend class CMyNewsArticleTreeWin;
	friend class CMyNewsCategoryWin;
	friend class CMyNewsCategoryListWin;
	friend class CMyNewsCategoryTreeWin;
	friend class CMyNewsCategoryExplWin;
	friend class CMyNewsArticTextWin;
	friend class CMyDeleteNewsFldrItemTask;
	friend class CMyGetBannerTimeTask;
	friend class CMyGetBannerTask;
	friend class CMyLaunchUrlTask;
	friend class CMyEditUserWin;
	//friend class CMyAdminSpectorWin;
	
	friend class CMyServerInfoWin;
	friend class CMyToolbar;
	friend class CMyBannerToolbarWin;
	friend class CMyTracker;
	friend class CMyTaskListView;
	friend class CMyNewsCategoryListView;
	friend class CMyNewsCategoryTreeView;
	friend class CMyStartupGetFileListTask;
	friend class CMyStartupGetNewsCatListTask;
	friend class CMyWaitDisconnectWin;
	friend class CMyWaitQuitWin;
	
	public:
		// construction
		CMyApplication();
		void StartUp();
		~CMyApplication();

		// application functions
		virtual void WindowHit(CWindow *inWindow, const SHitMsgData& inInfo);
		virtual void UserQuit();
		virtual void KeyCommand(Uint32 inCmd, const SKeyMsgData& inInfo);
		virtual void Error(const SError& inInfo);
		virtual void HandleMessage(void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);
		virtual void OpenDocument(TFSRefObj* inRef);
		virtual void ShowPreferences();
		virtual void Timer(TTimer inTimer);

		bool IsConnected();
		Uint16 GetServerVersion();
		bool HasGeneralPriv(const Uint32 inPriv); 	
		bool HasFolderPriv(const Uint32 inPriv);
		bool HasBundlePriv(const Uint32 inPriv);
		
		static void DisplayStandardMessage(const Uint8 *inTitle, const Uint8 *inMsg, Int16 inIcon = 0, Int16 inSound = 0);
		static void DisplayStandardMessage(const Uint8 *inTitle, const void *inMsg, Uint16 inMsgSize, Int16 inIcon = 0, Int16 inSound = 0, bool inShowTextView = false);
		static bool SaveTextAs(CTextView *inTextView);

	protected:
		// misc data
		Uint32 mTotalBytesUploaded, mTotalBytesDownloaded, mTotalSecsOnline, mTotalChatBytes;
		Uint32 mAppInBackgroundSecs, mConnectStartSecs;
		Uint32 mBasePortNum, mHttpPortNum;
		SInternetNameAddress mAddress;
		SInternetNameAddress mFirewallAddress;
		SMySaveConnect mSaveConnect;
		Uint8 mUserName[32];
		Uint8 mUserLogin[33];
		Uint8 mUserPassword[33];
		Uint8 mServerName[32];
		bool mUseCrypt;
		TTransact mTpt;
		CPtrList<CMyTask> mTaskList;
		
		TPtr mStartupPath;
		Uint32 mStartupPathSize;
		TFSRefObj* mResumableFile;
			
		bool mNoServerAgreement;
		THdl mServerAgreement;
		
		THdl mServerBanner;
		Uint32 mServerBannerType;
		Uint8 mServerBannerURL[256];	
		Uint8 mServerBannerComment[256];
		Uint32 mServerBannerCount;
		
		Uint16 mActiveTaskCount;
		Uint16 mQueuedTaskCount;

		TTimer mUserQuitTimer, mFinishTaskTimer, mCloseWinTimer, mDisableAutoResponse;
		TTimer mKeepConnectionAliveTimer, mAboutTimer;
	#if BANNER_AUTO_REFRESH_MS
	public:
		TTimer mBannerTimer;
		
	#endif // BANNER_AUTO_REFRESH_MS
	
	protected:			
		Uint16 mIconID;
		Uint16 mServerVers;
		
		Uint16 mIsConnected			: 1;
		Uint16 mIsAgreed			: 1;
		Uint16 mAllowChatWithCA		: 1;
		Uint16 mIsLeechMode			: 1;
		Uint16 mToggleWinVis		: 1;
		Uint16 mScramChat			: 1;
		Uint16 mSoundDisabled		: 1;
		Uint16 mPorkMode			: 1;
		Int16 mWindowPat;
		
		SMyOptions mOptions;
		SMyUserAccess mMyAccess;
		
		Uint8 *mScramTable;
		Uint8 *mUnscramTable;
		Uint8 mUnlockName[32];
		Uint8 mUnlockCode[32];
				
		//Uint32 mAppTime;  BANNER_AUTO_REFRESH_MS
		Uint32 mServTime;
		Uint32 mTrakTime;
		Uint32 mServRatio;
		Uint32 mTrakRatio;
			
		// window data
		CWindow *mAuxParentWin;
		CPtrList<CWindow> mAuxChildrenList;
		
		CWindow *mAboutWin;
		CMyOldNewsWin *mOldNewsWin;
		CMyPublicChatWin *mChatWin;
		CMyTasksWin *mTasksWin;
		CMyUserListWin *mUsersWin;
		CMyAdminWin *mAdminWin;
		CMyAdmInSpectorWin *mAdmInSpectorWin;
		CMyTracker *mTracker;
	public:
		CMyToolbar *mToolbarWin;
		Uint16 mNoBanner			: 1;
		Uint16 mIconSet;
	protected:	
		// lists
		CPtrList<CMyItemsWin> mFileWinList;
		CPtrList<CMyViewFileWin> mViewWinList;
		CPtrList<CMyServerInfoWin> mServerInfoWinList;
		CPtrList<CMyItemsWin> mCategoryWinList;
		CPtrList<CMyNewsArticleTreeWin> mArticleTreeWinList;
		CPtrList<CMyNewsArticTextWin> mNewsArticTxtWinList;
		CPtrList<CMyUserInfoWin> mUserInfoWinList;
		
		CMyCacheList mCacheList;
		CHttpIDList mHttpIDList;
				
	#if USE_NEWS_HISTORY
		CMyNewsReadHistory *mNZHist;
	#endif
	
		CChatLog mChatLog;							
	
		// GUI hits
		void DoWizard();
		bool DoOptions();
		void DoBroadcast();
		void DoOldPostNewsWin();
		void DoShowConnect(const Uint8 *inAddress = nil, const Uint8 *inUserLogin = nil, const Uint8 *inUserPassword = nil, bool inStartupInfo = false, bool inUseCrypt = false);
		void DoConnect(const Uint8 *inAddress, const Uint8 *inServerName = nil, const Uint8 *inLogin = nil, const Uint8 *inPassword = nil, bool inStartupInfo = false, bool inConfirmDisconnect = true, bool inUseCrypt = false);
		void DoDisconnect();
		void DoChatSend(bool inAction);
		void DoPrivChatSend(CMyPrivateChatWin *inWin, bool inAction);
		void DoShowServerInfo();
		void DoSaveServerBookmark();
		
		void DoFileView(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, Uint32 inTypeCode, Uint32 inCreatorCode, Uint16 inMods);
		void DoFileDownload(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, Uint32 inTypeCode, bool inIsFolder, Uint16 inMods);
		void DoFileUpload(CMyFileWin *inWin, const void *inPathData, Uint32 inPathSize, Uint16 inMods);
		void DoFileUploadFromDrop(CMyFileWin *inWin, const void *inPathData, Uint32 inPathSize);
		void DoFileMove(CMyItemsWin *inWin, Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, const void *inDestPathData, Uint32 inDestPathSize);
		void DoFileDelete(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder);
		void DoFileRefresh(const void *inPathData, Uint32 inPathSize);
		void DoFileDoubleClick(CMyItemsWin *inWin, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, Uint16 inMods);
		void DoFileOpenParent(CMyItemsWin *inWin, Uint16 inMods);
		void DoFileNewFolder(CMyItemsWin *inWin, const void *inPathData, Uint32 inPathSize);
		void DoFileGetInfo(CMyFileWin *inWin);
		void DoFileSaveInfo(CMyFileInfoWin *inWin);
		
		void DoNewsFldrOpen(CMyItemsWin *inWin, const Uint8 *inItemName, const void *inPathData, Uint32 inPathSize, Uint16 inType, SGUID& inGuid, Uint16 inMods);
		void DoNewsFldrRefresh(const void *inPathData, Uint32 inPathSize);
		void DoNewsFldrTrash(CMyItemsWin *inWin, const Uint8 *inItemName, const void *inPathData, Uint32 inPathSize, Uint16 inType);
		void DoNewsFldrNewCat(CMyItemsWin *inWin, const void *inPathData, Uint32 inPathSize);
		void DoNewsFldrNewFldr(CMyItemsWin *inWin, const void *inPathData, Uint32 inPathSize);
		void DoNewsFldrOpenParent(CMyItemsWin *inWin, Uint16 inMods);

		void DoNewsCatOpen(const void *inPathData, Uint32 inPathSize, Uint32 inPathChecksum, Uint32 inArticleID, Uint8 *inArticleName, CNZReadList *inReadList, SRect inBounds);
		void DoNewsCatOpen(const void *inPathData, Uint32 inPathSize, Uint32 inArticleID, Uint8 *inArticleName);
		void DoNewsCatOpenParent(CMyNewsArticleTreeWin *inWin);
		void DoNewsCatRefresh(const void *inPathData, Uint32 inPathSize);
		void DoNewsCatNewArticle(const void *inPathData, Uint32 inPathSize, Uint32 inPathChecksum, CNZReadList *inReadList, SRect inBounds);
		void DoNewsCatTrash(CMyItemsWin *inWin, const void *inPathData, Uint32 inPathSize, Uint32 inArticleID, Uint8 *inArticleName);
		
		void DoNewsArticGoPrev(CMyNewsArticTextWin *inWin, Uint16 inMods);
		void DoNewsArticGoNext(CMyNewsArticTextWin *inWin, Uint16 inMods);
		void DoNewsArticGoParent(CMyNewsArticTextWin *inWin, Uint16 inMods);
		void DoNewsArticGoFirstChild(CMyNewsArticTextWin *inWin, Uint16 inMods);
		
		void DoNewsArticTitleChanged(CMyNewsArticTextWin *inWin);
		void DoNewsArticSend(CMyNewsArticTextWin *inWin);
		void DoNewsArticReply(const void *inPathData, Uint32 inPathSize, Uint32 inPathChecksum, Uint32 inArticleID, Uint8 *inArticleName, CNZReadList *inReadList);
		void DoNewsArticOpenParent(CMyNewsArticTextWin *inWin);
		void DoNewsNavigateArticle(CMyNewsArticTextWin *inWin, Uint32 inArticleID, Uint32 inType, Uint16 inMods);
		
		void DoAbout();
		void DoSendFakeRed(Uint16 inUserID);
		void DoSendVisible(Uint16 inUserID);
		void DoBlockDownload(Uint16 inUserID);
		void DoSendAway(Uint16 inUserID);
		
		void DoStopTask();
		void DoStartTask();
		
		//void DoSendPrivMsg(Uint32 inUserID, const Uint8 *inUserName, Uint16 inMods);
		void DoSendPrivMsg(Uint32 inUserID, const Uint8 *inUserName);
		void DoReplyPrivMsg(CMyPrivMsgWin *inWin);
		void CouldNotSendPrivMsg(const Uint8 *inUserName, THdl inTextHdl);
		void DoBringFrontPrivMsg();
		void DoDisconnectUser(Uint16 inMods);
		void DoDisconnectUser(CMyUserInfoWin *inWin, Uint16 inMods);
		void DoGetUserAccountInfo(CMyUserInfoWin *inWin);
		void DoGetClientInfo(Uint16 inUserID, const Uint8 *inUserName);
		
		void DoUserInfoRefresh(CMyUserInfoWin *inWin);
		void DoShowMessage(TFieldData inData);
		void DoSecret();
		void DoFlood();
		void DoAdmInSpector();
		void DoChangeIcon(Uint16 inUserID, Uint8 inUserName[64]);
		
		void DoShowPublicFiles();
		void DoShowChatWin();
		void DoShowTasksWin();
		void DoShowNewsWin();
		void DoShowMsgBoardWin();
		void DoShowUsersWin();
		void DoCloseWindow(CWindow *inWindow);
		void DoCloseAllFileWindows();
		void DoCloseAllNewsWindows();
		void DoCloseAllUserInfoWindows();
		void DoShowAgreement();
		bool DoCheckServerBanner();
		bool DoShowServerBannerToolbar();

		void DoOpenUserList();
		
		
		void DoAdminNewUser();
		void DoAdminOpenUser();
		void DoAdminDeleteUser();
		void DoAdminSaveUsers();
		void DoAdminRevertUsers();
		void DoAdminClose();
		void DoAdmInSpectorClose();
		void DoNewUser();
		void DoOpenUser();
		void DoSaveEditUser(CMyEditUserWin *inWin);
		void DoCancelEditUser(CMyEditUserWin *inWin);
		void DoDeleteEditUser(CMyEditUserWin *inWin);
		void DoMacMenuBar(Int16 inMenu, Int16 inItem);
		void DoShowTracker();
		void DoRefreshTracker(Uint16 inMods);
		void DoConnectToTracked(bool inShowWin);

		void DoGoHotlineURL(const void *inText, Uint32 inTextSize);
		void DoOpenPrivateChat(Uint32 inUserID);
		void DoClosePrivateChat(CMyPrivateChatWin *inWin);
		void DoRejectChatInvite(CMyChatInviteWin *inWin);
		void DoAcceptChatInvite(CMyChatInviteWin *inWin);
		void DoSetChatSubject(CMyPrivateChatWin *inWin);
		
		void DoCreateToolbar(SRect& ioBounds);
	
		void DoUpdateVersion();
		
		// utility functions
		void ChatScram(void *ioData, Uint32 inDataSize);
		void ChatUnscram(void *ioData, Uint32 inDataSize);
		void DisplayServerName(const Uint8 *inServerName);
		template <class T> bool SearchWin();
		CMyItemsWin *GetRootFolderWin();
		CMyItemsWin *GetFolderWinByPath(const void *inPathData, Uint32 inPathSize, Uint32 inCheckSum = 0);
		void SetFolderWinContent(const void *inPathData, Uint32 inPathSize, TFieldData inData);
		CMyItemsWin *GetRootNewsFolderWin();
		CMyItemsWin *GetNewsFolderWinByPath(const void *inData, Uint32 inDataSize, Uint32 inCheckSum = 0);
		void SetBundleWinContent(const void *inPathData, Uint32 inPathSize, TFieldData inData);
		CMyNewsArticleTreeWin *GetNewsArticleListWinByPath(const void *inData, Uint32 inDataSize, Uint32 inCheckSum = 0);
		void SetCategoryWinContent(const void *inPathData, Uint32 inPathSize, TFieldData inData);
		CMyNewsArticTextWin *GetNewsArticleWinByPathAndID(Uint32 inID, const void *inData, Uint32 inDataSize, Uint32 inCheckSum = 0)	{	return CMyNewsArticTextWin::FindWindowByPathAndID(inID, inData, inDataSize, inCheckSum);	}
		void SetArticleWinContent(const void *inPathData, Uint32 inPathSize, Uint32 inArticleID, TFieldData inData);
		void SetCurrentArticle(const void *inPathData, Uint32 inPathSize, Uint32 inPathChecksum, Uint32 inCurrentID, Uint32 inNewID);
		void SelectArticle(const void *inPathData, Uint32 inPathSize, Uint32 inPathChecksum, Uint32 inArticleID);
		CMyUserInfoWin *GetUserInfoWinByID(Uint16 inUserID);
		void SetUserInfoWinContent(Uint16 inUserID, const Uint8 *inUserName, TFieldData inData);
		void GetTransferStatText(Uint32 inXferSize, Uint32 inTotalSize, Uint32 inBPS, Uint32 inETR, Uint8 *outText);
		void CheckServerError(Uint32 inError, TFieldData inData);
		void SetOnlineUsers(TFieldData inData);
		void GetClientUserInfo(TFieldData outData);
		void HideAllWindows();
		void ShowAllWindows();
		void ShowNotConnected();
		void ShowConnecting();
		void ShowConnected();
		void ProcessStartupPath();
		
		
		void ShowAdminWin(TFieldData inData);
		void ShowAdmInSpectorWin(TFieldData inData);
		CMyEditUserWin *ShowEditUserWin(TFieldData inData);
		CMyPrivateChatWin *GetChatWinByID(Uint32 inID);
		//CMyAdminSpectorWin *ShowAdminSpectorWin(TFieldData inData);
		void GetFilesBoundsInfo();
		void GetNewsBoundsInfo();
		void GetArticlesBoundsInfo();

		void CloseMarkedWin();
		void DisableAutoResponse();
		void ClearStartupInfo();
		void ClearOptions();
		
		void ClearServerBanner();
		void SetServerBanner(const void *inData, Uint32 inDataSize, Uint32 inTypeCode, const Uint8 *inBannerHref, const Uint8 *inBannerComment);

		bool AutoResp;
		bool GetAgreement(TFieldData inData, Uint16 inField_Data, THdl& outAgreement);
		bool GetResourceBanner(bool inQuickTimeBanner = false);

public:
		void SetupHttpTransaction(THttpTransact inHttpTransact, Uint8 *inReferer = nil, Uint8 *inCustomField = nil, bool inSetHttpIDList = false);
		void ShowStandardMessage(TFieldData inData, Uint8 *inTitle, bool inSound);
		void ShowAdminSpector(TFieldData inData);
		Uint8 GiveIconSet();
		
		void SetIconSet(Uint8 uIconSet);
protected:
		bool AutomaticResponseOrRefuseMsg(Uint16 inUserID, bool inSendResponse);
		bool AutomaticResponseOrRefuseChat(Uint16 inUserID);
		void SetChatColor(SMyColorInfo& colorPublicChat, SMyColorInfo& colorPrivateChat);
		void SetNewsColor(SMyColorInfo& colorNews);
		void SetViewTextColor(SMyColorInfo& colorViewText);

		void AddChatText(CMyPrivateChatWin *inWin, const void *inText, Uint32 inTextSize, bool inPlaySound);
		bool FindResumableFile(TFSRefObj* inRef, bool inIsFolder);

		// files and prefs
		void ReadServerFile(TFSRefObj* inFile, Uint8 *outAddress, Uint8 *outLogin, Uint8 *outPassword, bool *outUseCrypt);
		void SaveServerFile(TFSRefObj* inFile, const Uint8 inAddress[], const Uint8 inLogin[], const Uint8 inPassword[], bool inUseCrypt);
		TFSRefObj* GetDataRef();
		void LoadRezFile();
		TFSRefObj* GetNewsHistRef();
		TFSRefObj* GetPrefsRef();
		void WritePrefs();
		void ReadPrefs(SMyPrefs *outPrefs = nil);
		TFSRefObj* GetCustomRef();
		void ReadCustomInfo();
		TFSRefObj* GetDownloadFolder();
		TFSRefObj* GetCookiesRef();
		void WriteCookies();
		void ReadCookies();
		void DoOpenDloadFolder();
		void DoViewDloadFile(UInt8* fName);
		
		// tasks and transport
		void AddTask(CMyTask *inTask, const Uint8 inDesc[]);
		void RemoveTask(CMyTask *inTask);
		bool IsTaskInList(CMyTask *inTask);
		void FinishTask(CMyTask *inTask);
		void FinishDummyTasks();
		bool SearchLoginTask();
		bool SearchTrackServTask(Uint8 inTrackerID);
		
		bool SearchGetFileListTask(const void *inPathData, Uint32 inPathSize);
		bool SearchGetNewsArticleTask(const void *inPathData, Uint32 inPathSize, Uint32 inArticleID);
		bool IsFirstQueuedTask(CMyTask *inTask);
		void KillFinishedTasks();
		void ProcessTasks();
		void KillTasksAndDisconnect();
		void ProcessIncomingData();
		void ProcessAll();
		void SetTaskProgress(CMyTask *inTask, Uint32 inVal, Uint32 inMax, const Uint8 inDesc[]);
		void ScheduleTaskProcess();
		TTransact MakeTransactor();
		TTransport MakeTransferTransport();
		TTransact GetTransactor()	{	return mTpt;	}
		void DisposeTransactor();
		void StartTransactorConnect();
		TTransport StartTransferTransportConnect();
		void StartTransferConnect(TTransport inTpt, const void *inAddr, Uint32 inAddrSize);
		static void TransferMonitorProc(TRegularTransport inTpt, const void *inData, Uint32 inDataSize, bool inIsSend);
		void KeepConnectionAlive();
		

		// disconnect and quit confirmation
		bool ConfirmDisconnect(bool *outSaveConnect = nil);
		bool ConfirmQuit();
		void WaitDisconnect();
		void WaitQuit();
		bool CanQuit();
		Uint32 GetTransferTaskCount();
		Uint32 GetSignificantTaskCount();
		void SaveConnect(const Uint8 *inAddress, const Uint8 *inServerName, const Uint8 *inLogin, const Uint8 *inPassword, bool inStartupInfo, bool inUseCrypt);
		void RestoreDisconnect();
		void AutoReconnect();
		// incoming transactions
		void ProcessTran_NewMsg(TFieldData inData);
		void ProcessTran_ServerMsg(TFieldData inData);
		void ProcessTran_DisconnectMsg(TFieldData inData);
		void ProcessTran_ChatMsg(TFieldData inData);
		void ProcessTran_NotifyDeleteUser(TFieldData inData);
		void ProcessTran_NotifyChangeUser(TFieldData inData);
		void ProcessTran_ShowAgreement(TFieldData inData);
		void ProcessTran_ServerBanner(TFieldData inData);
		void ProcessTran_InviteToChat(TFieldData inData);
		void ProcessTran_NotifyChatChangeUser(TFieldData inData);
		void ProcessTran_NotifyChatDeleteUser(TFieldData inData);
		void ProcessTran_NotifyChatSubject(TFieldData inData);
		void ProcessTran_UserBroadcast(TFieldData inData);
		void ProcessTran_AdminSpector(TFieldData inData);
		
		void ProcessTran_UserAccess(TFieldData inData);
		void ProcessTran_DownloadInfo(TFieldData inData);
		
		void DoLaunchSecuriphone();
	
};

inline void CMyApplication::StartTransferConnect(TTransport inTpt, const void *inAddr, Uint32 inAddrSize)
{
	if (mFirewallAddress.name[0])
		inTpt->StartConnectThruFirewall(inAddr, inAddrSize, &mFirewallAddress, mFirewallAddress.name[0]+5);
	else
		inTpt->StartConnect(inAddr, inAddrSize);
}

Int16 HotlineFileTypeToIcon(Uint32 inType);

extern CMyApplication *gApp;
extern HLCrypt *gCrypt;
extern TFontDesc fd_Default9, fd_Default9Right, fd_Default9Bold, fd_Default9BoldRight, fd_Default9BoldCtr, fd_Default10, fd_Fixed9, fd_Default9BoldCenter;

