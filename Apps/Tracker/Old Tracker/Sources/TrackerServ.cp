/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "TrackerServ.h"


#if USES_FILE_EXTENSIONS
	const Uint8 *kMyErrorLogFileName = 		"\pError Log.txt";
	const Uint8 *kMyBannedFileName = 		"\pBanned.txt";
	const Uint8 *kMyPasswordsFileName = 	"\pPasswords.txt";
	const Uint8 *kMyTrackerIPFileName = 	"\pTrackerIP.txt";
	const Uint8 *kMyServersPerIPFileName = 	"\pServersPerIP.txt";
#else
	const Uint8 *kMyErrorLogFileName = 		"\pError Log";
	const Uint8 *kMyBannedFileName = 		"\pBanned";
	const Uint8 *kMyPasswordsFileName = 	"\pPasswords";
	const Uint8 *kMyTrackerIPFileName = 	"\pTrackerIP";
	const Uint8 *kMyServersPerIPFileName = 	"\pServersPerIP";
#endif


/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */

void main()
{
	UOperatingSystem::Init();
	
	try
	{
		UTransport::Init();
		
		if (!UTransport::HasTCP())
			Fail(errorType_Transport, transError_NeedTCP);
		
		CMyApplication app;
		app.StartUp();
		app.Run();
	}
	catch(SError& err)
	{
		UError::Alert(err);
	}
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -

TPtr TextToIPAddressList(const void *inData, Uint32 inDataSize, Uint32& outCount)
{
	const Int8 *addrDelims = ".,-#/ ";
	Uint32 list[1024];
	Uint32 i, addr, tokOffset, tokSize, s;
	Uint8 *p, *ep, *rp, *addrp;
	
	addrp = (Uint8 *)&addr;
	p = (Uint8 *)inData;
	ep = p + inDataSize;
	i = 0;
	
	while (p < ep)
	{
		rp = (Uint8 *)UMemory::SearchByte('\r', p, ep - p);
		if (rp == nil) rp = ep;
		
		s = rp - p;
		if (s >= 7)
		{
			if (*p == '#' || *p == '/') goto cont;
			tokOffset = tokSize = 0;
			
			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			addrp[0] = UText::TextToInteger(p+tokOffset, tokSize);
			
			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			addrp[1] = UText::TextToInteger(p+tokOffset, tokSize);
			
			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			addrp[2] = UText::TextToInteger(p+tokOffset, tokSize);

			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			addrp[3] = UText::TextToInteger(p+tokOffset, tokSize);

			if (i >= (sizeof(list) / sizeof(Uint32))) break;
			list[i] = addr;
			i++;
		}
		
		cont:

		p = rp + 1;
		if (*p == '\n')
			p++;
	}
	
	if (i == 0)
	{
		outCount = 0;
		return nil;
	}
	
	TPtr thePtr = UMemory::New(list, i * sizeof(Uint32));
	outCount = i;
	return thePtr;
}

SIPAddress TextToIPAddress(const void *inData, Uint32 inDataSize)
{
	const Int8 *addrDelims = ".,-#/ ";
	Uint32 tokOffset, tokSize, s;
	Uint8 *p, *ep, *rp;
	
	p = (Uint8 *)inData;
	ep = p + inDataSize;
	
	SIPAddress stIPAddress;
	stIPAddress.un_IP.stDW_IP.nDW_IP = 0;
	
	while (p < ep)
	{
		rp = (Uint8 *)UMemory::SearchByte('\r', p, ep - p);
		if (rp == nil) rp = ep;
		
		s = rp - p;
		if (s >= 7)
		{
			if (*p == '#' || *p == '/') goto cont;
			tokOffset = tokSize = 0;
			
			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			stIPAddress.un_IP.stB_IP.nB_IP1 = UText::TextToInteger(p+tokOffset, tokSize);
			
			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			stIPAddress.un_IP.stB_IP.nB_IP2 = UText::TextToInteger(p+tokOffset, tokSize);
			
			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			stIPAddress.un_IP.stB_IP.nB_IP3 = UText::TextToInteger(p+tokOffset, tokSize);

			if (!UMemory::Token(p, s, addrDelims, tokOffset, tokSize)) goto cont;
			stIPAddress.un_IP.stB_IP.nB_IP4 = UText::TextToInteger(p+tokOffset, tokSize);
			
			return stIPAddress;
		}
		
		cont:
		
		p = rp + 1;
		if (*p == '\n')
			p++;
	}

	stIPAddress.un_IP.stDW_IP.nDW_IP = 0;
	return stIPAddress;
}

TPtr TextToCRCList(const void *inData, Uint32 inDataSize, Uint32& outCount)
{
	Uint32 list[1024];
	Uint32 i, s;
	Uint8 *p, *ep, *rp;
	
	p = (Uint8 *)inData;
	ep = p + inDataSize;
	i = 0;
	
	while (p < ep)
	{
		rp = (Uint8 *)UMemory::SearchByte('\r', p, ep - p);
		if (rp == nil) rp = ep;
		
		s = rp - p;
		if (s)
		{
			if (i >= (sizeof(list) / sizeof(Uint32))) break;
			list[i++] = UMemory::CRC(p, s);
		}
		
		p = rp + 1;
		if (*p == '\n')		// /r/n - end of line (PC)
			p++;			// /r   - end of line (MAC)
	}	
	
	if (i == 0)
	{
		outCount = 0;
		return nil;
	}

	outCount = i;
	return UMemory::New(list, i * sizeof(Uint32));
}

void TextToPasswordList(const void *inData, Uint32 inDataSize, CPtrList<SMyLoginInfo> *outLoginList)
{
	Uint32 s;
	Uint8 *p, *ep, *rp;
	
	p = (Uint8 *)inData;
	ep = p + inDataSize;
	
	while (p < ep)
	{
		rp = (Uint8 *)UMemory::SearchByte('\r', p, ep - p);
		if (rp == nil) rp = ep;
		
		s = rp - p;
		if (s)
		{			
			SMyLoginInfo *pLoginInfo = (SMyLoginInfo *)UMemory::NewClear(sizeof(SMyLoginInfo));
			if (s > sizeof(pLoginInfo->psPassword) - 1)
				s = sizeof(pLoginInfo->psPassword) - 1;
			
			pLoginInfo->psPassword[0] = UMemory::Copy(pLoginInfo->psPassword + 1, p, s);
			outLoginList->AddItem(pLoginInfo);
		}
		
		p = rp + 1;
		if (*p == '\n')		// /r/n - end of line (PC)
			p++;			// /r   - end of line (MAC)
	}		
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -

CMyApplication::CMyApplication()
{
	mErrorCount = mResetCount = 0;
	
	mStatsWin = nil;
#if USE_LOG_WIN
	mLogWin = nil;
#endif
	
	mIPAddress.un_IP.stDW_IP.nDW_IP = 0;
	mRegTpt = mListenTpt = mListenHttpTpt = nil;
	
	mServerCount = 0;
	mServerList = UMemory::NewHandle(1024);
	
	mRemoveOldTimer = NewTimer();
	mBanList = mPassList = nil;
	mBanCount = mPassCount = 0;

	mMaxServersPerIP = 0;
	mListingCounter = mUserCount = 0;
}

CMyApplication::~CMyApplication()
{
	ClearClientList();
	UTransport::Dispose(mRegTpt);
	UTransport::Dispose(mListenTpt);
	UTransport::Dispose(mListenHttpTpt);

	ClearServerList();
	ClearServerPerIPList();
}

void CMyApplication::StartUp()
{
	SInternetAddress addr = { kInternetAddressType, 0, 0, 0, 0, 0 };
	
	ReadBans();
	ReadPasswords();
	ReadIPAddress();
	ReadMaxServersPerIP();

	try
	{
		// make register transport
		addr.port = 5499;
		mRegTpt = UTransport::New(transport_UDPIP);
		mRegTpt->SetIPAddress(mIPAddress);
		mRegTpt->Listen(&addr, sizeof(addr));
		mRegTpt->SetMessageHandler(MessageHandler, this);

		// make client listener
		addr.port = 5498;
		mListenTpt = UTransport::New(transport_TCPIP);
		mListenTpt->SetIPAddress(mIPAddress);
		mListenTpt->Listen(&addr, sizeof(addr));
		mListenTpt->SetMessageHandler(MessageHandler, this);

		addr.port = 5497;
		mListenHttpTpt = UTransport::New(transport_TCPIP, false);
		mListenHttpTpt->SetIPAddress(mIPAddress);
		mListenHttpTpt->Listen(&addr, sizeof(addr));
		mListenHttpTpt->SetMessageHandler(MessageHandler, this);
	}
	catch(...)
	{
		if (mIPAddress.un_IP.stDW_IP.nDW_IP)
		{
			Uint8 bufMsg[256];
			bufMsg[0] = UText::Format(bufMsg + 1, sizeof(bufMsg) - 1, "Cannot bind to the address %d.%d.%d.%d because it is incorrect or it is already in use.", mIPAddress.un_IP.stB_IP.nB_IP1, mIPAddress.un_IP.stB_IP.nB_IP2, mIPAddress.un_IP.stB_IP.nB_IP3, mIPAddress.un_IP.stB_IP.nB_IP4);
			DisplayMessage("\pTracker error", bufMsg, icon_Stop, 1);
		}
		else
			DisplayMessage("\pTracker error", "\pCannot bind to the default address because it is already in use.", icon_Stop, 1);
	}

	// make stats window
	mStatsWin = new CMyStatsWin;
	mStatsWin->Show();
	
#if USE_LOG_WIN
	mLogWin = new CMyTextWin;
	mLogWin->SetTitle("\pServer Log");
	mLogWin->SetAutoBounds(windowPos_TopLeft, windowPosOn_MainScreen);
	mLogWin->Show();
	
	// check if our log file exists
	try
	{
		StFileSysRef logFile(kProgramFolder, nil, "\pServerReg Log.txt");
		if (!logFile->Exists())
			logFile->CreateFile('TEXT', 'ttxt');
	}
	catch(...)
	{
	
	}
#endif
	
	// start timer to regularly remove outdated servers from the list
	mRemoveOldTimer->Start(300000, kRepeatingTimer);
}

void CMyApplication::WindowHit(CWindow */* inWindow */, const SHitMsgData& inInfo)
{
	switch (inInfo.id)
	{
		case viewID_Options:
			DoOptions();
			break;
		
		case CWindow::ClassID:
			UserQuit();
			break;
	}
}

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
			if ((TTransport)inObject == mRegTpt)
				ProcessRegister();
			else
				ProcessClients();
			break;
		
		case 1100:
			DisplayMessage("\pRestart the tracker", "\pChanges of the Tracker IP will only take effect after the tracker is restarted.", icon_Stop, 1);
			break;
	
		default:
			inherited::HandleMessage(inObject, inMsg, inData, inDataSize);
			break;
	}
}

void CMyApplication::Timer(TTimer inTimer)
{
	if (inTimer == mRemoveOldTimer)
		RemoveOldServers();
}

void CMyApplication::Error(const SError& inInfo)
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

	StFileSysRef logFile(kProgramFolder, nil, kMyErrorLogFileName);
	UError::Log(logFile, inInfo);
	USound::Play(nil, 130, true);

	// just in case there was unfinished work to do
	UApplication::PostMessage(msg_DataArrived);
}

void CMyApplication::DoOptions()
{	
	CPtrList<SMyLoginInfo> stLoginList;
	ReadLoginInfo(&stLoginList);

	CPtrList<SMyPermBanInfo> stPermBanList;
	ReadPermBanInfo(&stPermBanList);
	
	CMyOptionsWin *win = new CMyOptionsWin();
	win->SetInfo(mIPAddress, mMaxServersPerIP, &stLoginList, &stPermBanList);
	
	// clear login list
	Uint32 i = 0;
	SMyLoginInfo *pLoginInfo;
	while (stLoginList.GetNext(pLoginInfo, i))
		UMemory::Dispose((TPtr)pLoginInfo);

	stLoginList.Clear();

	// clear ban list
	i = 0;
	SMyPermBanInfo *pPermBanInfo;
	while (stPermBanList.GetNext(pPermBanInfo, i))
		UMemory::Dispose((TPtr)pPermBanInfo);

	stPermBanList.Clear();
	
	// show window
	win->Show();
	
	// go into modal loop to process modal options window
	Uint32 id;
	for(;;)
	{
		win->ProcessModal();
		id = win->GetHitID();
		
		if (id == 1)	// save
		{
			SIPAddress stIPAddress;
			stIPAddress.un_IP.stDW_IP.nDW_IP = mIPAddress.un_IP.stDW_IP.nDW_IP;			
			Uint32 nMaxServersPerIP = mMaxServersPerIP;
			
			win->GetInfo(&mIPAddress, &mMaxServersPerIP, &stLoginList, &stPermBanList);
			delete win;
						
			WriteLoginInfo(&stLoginList);
			WritePermBanInfo(&stPermBanList);
			
			if (mMaxServersPerIP != nMaxServersPerIP)
			{
				WriteMaxServersPerIP();
				BuildServerPerIPList();
			}
			
			if (mIPAddress.un_IP.stDW_IP.nDW_IP != stIPAddress.un_IP.stDW_IP.nDW_IP)
			{
				WriteIPAddress();
				UApplication::PostMessage(1100);
			}

			return;
		}
		else if (id == 2)	// cancel
		{
			delete win;
			return;
		}
		else if(id == 3)
			win->AddLogin();
		else if(id ==4)
			win->DeleteLogin();
		else if(id == 5)
			win->AddPermBan();
		else if(id ==6)
			win->DeletePermBan();
	}
}

void CMyApplication::ResetServer()
{	
	// if there's been too many resets, abort app
	mResetCount++;
	if (mResetCount > 10)
	{
		DebugBreak("Too many resets! Terminating...");
		UApplication::Abort();
	}
	
	// wipe client list
	ClearClientList();
	
	// reset values
	mErrorCount = 0;

	// log message and update display
	UpdateDisplay();
}

void CMyApplication::UpdateDisplay()
{
	if (mStatsWin)
	{
		mStatsWin->SetServerCount(mServerCount);
		mStatsWin->SetUserCount(mUserCount);
		mStatsWin->SetListingCounter(mListingCounter);
	}
}

void CMyApplication::ProcessRegister()
{
	Uint8 buf[1024];
	Uint8 addr[128];
	Uint32 addrSize, dataSize;
	Uint16 type;
	Uint8 *p, *ep;
	
	for(;;)
	{
		addrSize = sizeof(addr);
		dataSize = sizeof(buf);

		if (!mRegTpt || !mRegTpt->ReceiveUnit(addr, addrSize, buf, dataSize))
			break;
		
		if (dataSize >= sizeof(Uint16))
		{
			type = FB( *(Uint16 *)buf );
			p = buf;
			ep = p + dataSize;
			
			// validate and dispatch message
			if (type == 1)		// add
			{
				// Uint16 type; Uint16 port; Uint16 userCount; Uint16 flags; Uint32 passID; pstring name; pstring desc; pstring password;
				// flags = 1 means don't show in list (name only)
				
				if (dataSize > 14)
				{
					CUnflatten unflat(buf, dataSize);
					
					unflat.SkipWord();
					Uint16 port = unflat.ReadWord();
					Uint16 userCount = unflat.ReadWord();
					Uint16 flags = unflat.ReadWord();
					Uint32 passID = unflat.ReadLong();
					Uint8 *name = unflat.ReadPString();
					Uint8 *desc = unflat.ReadPString();
					Uint8 *password = unflat.ReadPString();
					
					((SInternetAddress *)addr)->port = port;
					RegisterServer(*(SInternetAddress *)addr, userCount, flags, passID, name, desc, password);
				}
			}
			else if (type == 2)	// remove
			{
				// not done and not really necessary, so just ignore this message type
			}
		}
	}
}

void CMyApplication::ProcessClients()
{
	SMyClient *client;
	TTransport tpt = nil;
	Uint32 i;
	
	// accept connections
	for(;;)
	{
		if (mListenTpt)
			tpt = mListenTpt->Accept();
		
		if (tpt == nil)
		{
			if (mListenHttpTpt)
				tpt = mListenHttpTpt->Accept();
		
			if (tpt == nil) break;
		}
		
		client = nil;
		try
		{
			tpt->SetMessageHandler(MessageHandler, this);
			client = (SMyClient *)UMemory::NewClear(sizeof(SMyClient));
			client->tpt = tpt;
			mClientList.AddItem(client);
		}
		catch(...)
		{
			if (client) delete client;
			delete tpt;
			throw;
		}
					
		// reset error count every time someone connects
		mErrorCount = mResetCount = 0;
	}

	// process each client
	i = 0;
	while (mClientList.GetNext(client, i))
	{
		tpt = client->tpt;
		
		// check if connection has closed
		if (!tpt->IsConnected())
		{
			mClientList.RemoveItem(client);
			delete tpt;
			UMemory::Dispose((TPtr)client);
			i--;
			continue;
		}
		if (tpt->IsDisconnecting()) continue;
		
		// establish transaction protocol if necessary
		if (!client->isEstablished)
		{
			if (tpt->GetReceiveSize() >= 6)
			{
				#pragma options align=packed
				struct {
					Uint32 type;
					Uint16 version;
				} estabData;
				#pragma options align=reset
				
				tpt->Receive(&estabData, sizeof(estabData));
				
				if (estabData.type == TB((Uint32)'HTRK') && estabData.version == TB((Uint16)1))
				{
					tpt->Send(&estabData, sizeof(estabData));
					mListingCounter++;
					SendServerList(tpt);
					UpdateDisplay();
				}
				else
					tpt->Disconnect();
			}
		}
	}
}

SMyServerInfo *CMyApplication::NewServerInfo(SMyServerInfo *ioInfo, const SInternetAddress& inAddr, Uint16 inUserCount, Uint16 inFlags, Uint32 inPassID, Uint32 inNameCRC, const Uint8 *inName, const Uint8 *inDesc)
{
	ASSERT(inName && inDesc);
	
	if (ioInfo == nil)
		ioInfo = (SMyServerInfo *)UMemory::New(sizeof(SMyServerInfo) + inName[0] + inDesc[0] + 2);
	
	ioInfo->timeStamp = UDateTime::GetSeconds();
	ioInfo->address = *(Uint32 *)inAddr.host;
	ioInfo->passID = inPassID;
	ioInfo->port = inAddr.port;
	ioInfo->userCount = inUserCount;
	ioInfo->nameCRC = inNameCRC;
	ioInfo->flags = inFlags;
	
	UMemory::Copy(ioInfo->data, inName, inName[0]+1);
	UMemory::Copy(ioInfo->data + inName[0] + 1, inDesc, inDesc[0]+1);
	
	return ioInfo;
}

Uint32 CMyApplication::GetServerInfoSize(const SMyServerInfo *inInfo)
{
	ASSERT(inInfo);
	
	const Uint8 *p = inInfo->data;
	return sizeof(SMyServerInfo) + 2 + p[0] + *(p + p[0] + 1);
}

void CMyApplication::RegisterServer(const SInternetAddress& inAddr, Uint16 inUserCount, Uint16 inFlags, Uint32 inPassID, const Uint8 *inName, const Uint8 *inDesc, const Uint8 *inPassword)
{
	Uint32 i, n, crc;
	SMyServerInfo *ip, *newInfo;
	bool updateExisting = false;
	
	// ignore servers with invalid info
	if (!inName || !inName[0] || !inDesc || !inPassword) 
		return;
		
	// ignore servers with incorrect passwords or banned addresses
	if (IsInvalidPassword(inPassword) || IsBannedAddress(inAddr)) 	
		return;
			
#if USE_LOG_WIN
	Log(inName, inAddr, inPassword);
#endif	
	
	// expand server list handle if necessary
	n = UMemory::GetSize(mServerList);
	if (((mServerCount + 2) * sizeof(void*)) > n)
		UMemory::SetSize(mServerList, n + 1024);
	
	// calculate CRC of name
	crc = UMemory::CRC(inName+1, inName[0]);

#if INTERNAL_SERVER_PRIORITY
	// give priority to our internal servers!
	const Uint8 matchHost[3] = {	207, 245, 14	};
	
	if (crc < 10)
		crc = 10;
	
	if (UMemory::Equal(inAddr.host, matchHost, 3))
	{
		if (inAddr.host[3] == 170)
		{
			switch (inAddr.port)
			{
				case 5500:
					crc = 0;
					break;
				
				default:
					crc = 1;
					break;
			}
		}
		else if (inAddr.host[3] >= 162 && inAddr.host[3] <= 200)
			crc = 1;
	}
#endif

	// get ptr to list of servers
	void *hdata;
	StHandleLocker locker(mServerList, hdata);
	SMyServerInfo **serverList = (SMyServerInfo **)hdata;

	// decide whether to update existing server info or add new
	if (FindServerByName(crc, i))		// if found
	{
		// move i to first item with same CRC
		if (i)							// to stop i from going negative
		{
			while (i--)
			{
				if (serverList[i]->nameCRC != crc)
				{
					i++;
					break;
				}
			}
		}
		
		for (n=i; n<mServerCount; n++)
		{
			ip = serverList[n];
			if (ip->nameCRC != crc) break;
			
			if (ip->data[0] == inName[0] && UMemory::Equal(ip->data, inName, inName[0]))	// if same name
			{
				if ((ip->address == *(Uint32 *)inAddr.host && ip->port == inAddr.port) || (ip->passID == inPassID))	// if same address or same password
				{
					updateExisting = true;
					break;
				}
				else
				{
					// ***** should auto choose a diff name, eg append number
					return;
				}
			}
		}
	}
	else if (FindServerByAddress(inAddr, n))
	{
		ip = serverList[n];
		updateExisting = true;
	}
	
	// update or add the server registration
	if (updateExisting)								// if updating existing server info
	{
		Uint32 oldUserCount = ip->userCount;
		Uint32 oldNameCRC = ip->nameCRC;

		if (ip->address != *(Uint32 *)inAddr.host)
		{
			if (AddServerPerIP(*(Uint32 *)inAddr.host))
				RemoveServerPerIP(ip->address);
			else	
			{
				// delete servers which are exceeding the mMaxServersPerIP limit
				RemoveServer(n);
				return;
			}
		}

		// build new server-info, and use the existing memory if the size is the same
		if (GetServerInfoSize(ip) == (sizeof(SMyServerInfo) + inName[0] + inDesc[0] + 2))	// if new and old are same size
			newInfo = NewServerInfo(ip, inAddr, inUserCount, inFlags, inPassID, crc, inName, inDesc);
		else
		{
			newInfo = NewServerInfo(nil, inAddr, inUserCount, inFlags, inPassID, crc, inName, inDesc);
			UMemory::Dispose((TPtr)ip);
			serverList[n] = newInfo;
		}
		
		// adjust total user count
		if (inUserCount > oldUserCount)
			mUserCount += inUserCount - oldUserCount;
		else
			mUserCount -= oldUserCount - inUserCount;
				
		// if CRC changed, need to reinsert item to maintain sorting (list is sorted by CRC)
		if (oldNameCRC != crc)
		{
			// n is the current index, i is the new index
			UMemory::Copy(serverList + n, serverList + n + 1, (mServerCount - (n + 1)) * sizeof(void*));
			if (i > n) i--;
			UMemory::Copy(serverList + i + 1, serverList + i, ((mServerCount-1) - i) * sizeof(void *));
			serverList[i] = newInfo;
		}
	}
	else	// adding new server
	{
		// ignore servers which are exceeding the mMaxServersPerIP limit
		if (!AddServerPerIP(*(Uint32 *)inAddr.host))
			return;

		ip = NewServerInfo(nil, inAddr, inUserCount, inFlags, inPassID, crc, inName, inDesc);
		
		UMemory::Copy(serverList+i+1, serverList+i, (mServerCount - i) * 4);
		serverList[i] = ip;
		
		mServerCount++;
		mUserCount += inUserCount;		
	}

	// redraw stats etc
	UpdateDisplay();
}

#if 0
void CMyApplication::UnregisterServer(const SInternetAddress& inAddr, const Uint8 inName[])
{
	// can implement this later.  RemoveOldServers() will do for starters
	
	search for crc
	go up if same crc
	while crc is same
		if same crc and same addr and same port, remove it
}
#endif

void CMyApplication::RemoveOldServers()
{
	if (!mServerList || !mServerCount) 
		return;

	Uint32 curTime;
	SMyServerInfo **pp, **spp, **epp, *info;
	
	//Uint8 buf[8192];
	//Uint8 *p, *ep;
	//Uint32 count, s;

	//p = buf;
	//ep = buf + sizeof(buf);
	//count = 0;
	
	//*((Uint16 *)p)++ = 2;	// add type
	//*((Uint32 *)p)++ = 0;	// space for size and count

	curTime = UDateTime::GetSeconds();
	
	spp = (SMyServerInfo **)UMemory::Lock(mServerList);
	epp = spp - 1;
	pp = epp + mServerCount;

	while (pp != epp)
	{
		info = *pp;
		
		if (curTime - info->timeStamp > 660)
		{
			//if not enough space in buf, stop (we'll get the rest in the next sweep)
			//if (info->data[0] + 7 > ep - p)
			//	break;
			
			//count++;
			//*((Uint32 *)p)++ = info->address;
			//*((Uint16 *)p)++ = info->port;
			//p += UMemory::Copy(p, info->data, info->data[0]+1);
			
			mUserCount -= info->userCount;
			RemoveServerPerIP(info->address);
			
			UMemory::Dispose((TPtr)info);
			UMemory::Copy(pp, pp + 1, BPTR(spp + mServerCount) - BPTR(pp + 1));
			mServerCount--;
		}
		
		pp--;
	}
	
	UMemory::Unlock(mServerList);
	
	//*(Uint16 *)(buf+2) = p - buf - 4;	// size of following data
	//*(Uint16 *)(buf+4) = count;		// number of servers
	//s = p - buf;
	
	UpdateDisplay();
}

bool CMyApplication::RemoveServer(Uint32 inIndex)
{
	if (!mServerCount || inIndex > mServerCount - 1) 
		return false;
		
	SMyServerInfo **spp = (SMyServerInfo **)UMemory::Lock(mServerList);
	SMyServerInfo **pp = spp + inIndex;
	SMyServerInfo *info = *pp;
			
	mUserCount -= info->userCount;
	RemoveServerPerIP(info->address);
			
	UMemory::Dispose((TPtr)info);
	UMemory::Copy(pp, pp + 1, BPTR(spp + mServerCount) - BPTR(pp + 1));
	mServerCount--;
	
	UMemory::Unlock(mServerList);
		
	UpdateDisplay();
	return true;
}

// returns true if found
SMyServerInfo *CMyApplication::NameToInfo(const Uint8 inName[])
{
	Uint32 crc, i;
	SMyServerInfo **serverList;
	SMyServerInfo *ip;
	
	crc = UMemory::CRC(inName+1, inName[0]);
	serverList = (SMyServerInfo **)UMemory::Lock(mServerList);

	if (FindServerByName(crc, i))
	{
		// move i to first item with same CRC
		while (i--)
		{
			if (serverList[i]->nameCRC != crc)
			{
				i++;
				break;
			}
		}
		
		for (; i<mServerCount; i++)
		{
			ip = serverList[i];
			if (ip->nameCRC != crc) break;
			
			if (ip->data[0] == inName[0] && UMemory::Equal(ip->data, inName, inName[0]))	// if same name
			{
				UMemory::Unlock(mServerList);
				return ip;
			}
		}
	}
	
	UMemory::Unlock(mServerList);
	
	return nil;
}

// outIndex is 0-based
bool CMyApplication::FindServerByName(Uint32 inCRC, Uint32& outIndex)
{
	if (mServerCount == 0)
	{
		outIndex = 0;
		return false;
	}

	void *hdata;
	StHandleLocker lock(mServerList, hdata);
	SMyServerInfo* *lookupTab = (SMyServerInfo* *)hdata;
	Uint32 crc;
	Uint32 l, r, i;
	
	crc = lookupTab[0]->nameCRC;
	
	if (inCRC == crc)
	{
		outIndex = 0;
		goto found;
	}
	else if (inCRC < crc)
	{
		outIndex = 0;
		return false;
	}
	
	l = 1;
	r = mServerCount - 1;
	
	if (l > r)
	{
		outIndex = 1;
		return false;
	}
	
	while (l <= r)
	{
		i = (l + r) >> 1;
		
		crc = lookupTab[i]->nameCRC;

		if (inCRC == crc)
		{
			outIndex = i;
			goto found;
		}
		else if (inCRC < crc)
			r = i - 1;
		else
			l = i + 1;
	}
	
	if (inCRC > crc)
		i++;
	
	outIndex = i;
	return false;
	
found:
	return true;
}

// outIndex is 0-based
bool CMyApplication::FindServerByAddress(const SInternetAddress& inAddr, Uint32& outIndex)
{
	void *hdata;
	StHandleLocker lock(mServerList, hdata);
	SMyServerInfo **pp = (SMyServerInfo **)hdata;
	SMyServerInfo **sp = pp;
	
	Uint32 addr = *(Uint32 *)inAddr.host;
	Uint16 port = inAddr.port;
	
	Uint32 n = mServerCount;
	
	while (n--)
	{
		if ((**pp).address == addr && (**pp).port == port)
		{
			outIndex = pp - sp;
			return true;
		}
		
		pp++;
	}
	
	outIndex = 0;
	return false;
}

void CMyApplication::SendServerList(TTransport inTpt)
{
	Uint8 buf[8192];
	Uint8 *p, *ep;
	SMyServerInfo **pp;
	SMyServerInfo *info;
	Uint32 n, count;
	Uint8 *name, *desc;

	p = buf;
	ep = buf + sizeof(buf);
	n = mServerCount;
	count = 0;
	
	*((Uint16 *)p)++ = TB((Uint16)1);	// add type
	*((Uint16 *)p)++ = 0;				// space for size
	*((Uint32 *)p)++ = 0;				// space for counts

	pp = (SMyServerInfo **)UMemory::Lock(mServerList);
	
	try
	{
		while (n--)
		{
			info = *pp;
						
			name = info->data;
			desc = name + name[0] + 1;
			
			if ((name[0] + desc[0] + 12) > (ep - p))	// if not enough space
			{
				// send what's in the buf
				*(Uint16 *)(buf+2) = TB((Uint16)(p - buf - 4));	// size of following data
				*(Uint16 *)(buf+4) = TB((Uint16)mServerCount);	// number of servers in total
				*(Uint16 *)(buf+6) = TB((Uint16)count);			// number of servers in this message
				inTpt->Send(buf, p - buf);
				
				// reset buf
				count = 0;
				p = buf;
				*((Uint16 *)p)++ = TB((Uint16)1);	// add type
				*((Uint16 *)p)++ = 0;				// space for size
				*((Uint32 *)p)++ = 0;				// space for counts
			}
			
			// add server to buf
			count++;
			*((Uint32 *)p)++ = info->address;
			*((Uint16 *)p)++ = TB((Uint16)info->port);
			*((Uint16 *)p)++ = TB((Uint16)info->userCount);
			*((Uint16 *)p)++ = TB((Uint16)info->flags);
			p += UMemory::Copy(p, name, name[0]+1);
			p += UMemory::Copy(p, desc, desc[0]+1);
			
			pp++;
		}
		
		// send what's in the buf
		*(Uint16 *)(buf+2) = TB((Uint16)(p - buf - 4));	// size of following data
		*(Uint16 *)(buf+4) = TB((Uint16)mServerCount);	// number of servers in total
		*(Uint16 *)(buf+6) = TB((Uint16)count);			// number of servers in this message
		inTpt->Send(buf, p - buf);
	}
	catch(...)
	{
		UMemory::Unlock(mServerList);
		throw;
	}
	
	UMemory::Unlock(mServerList);
}

void CMyApplication::SendLookup(TTransport inTpt, const Uint8 inName[])
{
	SMyServerInfo *info;
	Uint8 buf[1024];
	Uint8 *p = buf;
	Uint8 *ep = buf + sizeof(buf);
	Uint8 *name, *desc;

	info = CMyApplication::NameToInfo(inName);
	
	if (info)
	{
		name = info->data;
		desc = name + name[0] + 1;

		*((Uint16 *)p)++ = TB((Uint16)4);				// type 4 means we found it
		*((Uint16 *)p)++ = 0;							// space for the size
		
		*((Uint32 *)p)++ = info->address;
		*((Uint16 *)p)++ = TB((Uint16)info->port);
		*((Uint16 *)p)++ = TB((Uint16)info->userCount);
		*((Uint16 *)p)++ = TB((Uint16)info->flags);
		p += UMemory::Copy(p, name, name[0]+1);
		p += UMemory::Copy(p, desc, desc[0]+1);
		
		*(Uint16 *)(buf+2) = TB((Uint16)(p - buf - 4));	// size of following data
	}
	else
	{
		*((Uint16 *)p)++ = TB((Uint16)5);				// type 5 means not found
		*((Uint16 *)p)++ = 0;	// no data
	}
	
	inTpt->Send(buf, p - buf);
}

void CMyApplication::ClearServerList()
{
	if (!mServerList)
	{
		mServerCount = 0;
		return;
	}
	
	SMyServerInfo **pp = (SMyServerInfo **)UMemory::Lock(mServerList);
	Uint32 nCount = mServerCount;

	while (nCount--)
	{
		UMemory::Dispose((TPtr)(*pp));
		pp++;
	}
		
	UMemory::Unlock(mServerList);
	UMemory::Dispose(mServerList);

	mServerList = nil;
	mServerCount = 0;
}

void CMyApplication::ClearClientList()
{
	Uint32 i = 0;
	SMyClient *pClient;
	
	while (mClientList.GetNext(pClient, i))
	{
		delete pClient->tpt;
		UMemory::Dispose((TPtr)pClient);
	}
	
	mClientList.Clear();
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Login File ₯₯

void CMyApplication::ReadPasswords()
{
	UMemory::Dispose(mPassList);
	mPassList = nil;
	mPassCount = 0;
	
	StFSRef fp(kProgramFolder, nil, kMyPasswordsFileName, fsOption_PreferExistingFile);
	
	Uint32 s, c;
	if (fp.IsValid())
	{
		fp->Open(perm_Read);
		
		s = fp->GetSize();
		if (s)
		{
			StPtr buf(s);
			s = fp->Read(0, buf, s);
			
			if (s)
			{
				mPassList = TextToCRCList(buf, s, c);
				mPassCount = c;
			}
		}
	}
}

bool CMyApplication::IsInvalidPassword(const Uint8 *inPass)
{
	return mPassList && (inPass == nil || inPass[0] == 0 || UMemory::SearchLongArray(UMemory::CRC(inPass+1, inPass[0]), (Uint32 *)mPassList, mPassCount) == nil);
}

void CMyApplication::WriteLoginInfo(CPtrList<SMyLoginInfo> *inLoginList)
{
	StFSRef fp(kProgramFolder, nil, kMyPasswordsFileName);
	if (fp->Exists())
		fp->DeleteFile();

	if (inLoginList->GetItemCount())
	{
		// write login file
		fp->CreateFileAndOpen('TEXT', 'ttxt', kAlwaysOpenFile, perm_ReadWrite);
		Uint32 nOffset = 0;

		try
		{
			Uint32 i = 0;
			SMyLoginInfo *pLoginInfo;
				
			while(inLoginList->GetNext(pLoginInfo, i))
			{				
				nOffset += fp->Write(nOffset, pLoginInfo->psPassword + 1, pLoginInfo->psPassword[0]);
				nOffset += fp->Write(nOffset, "\r", 1);
			#if WIN32
				nOffset += fp->Write(nOffset, "\n", 1);
			#endif
			
				UMemory::Dispose((TPtr)pLoginInfo);
			}			
		}
		catch(...)
		{
			inLoginList->Clear();
			fp->Close();
			fp->DeleteFile();
			// don't throw
		}

		inLoginList->Clear();
		fp->Close();
	}
	
	ReadPasswords();
}

void CMyApplication::ReadLoginInfo(CPtrList<SMyLoginInfo> *outLoginList)
{
	StFSRef fp(kProgramFolder, nil, kMyPasswordsFileName, fsOption_PreferExistingFile);
	
	if (fp.IsValid())
	{
		fp->Open(perm_Read);
		
		Uint32 nSize = fp->GetSize();
		if (nSize)
		{
			StPtr buf(nSize);
			nSize = fp->Read(0, buf, nSize);
			
			if (nSize)
				TextToPasswordList(buf, nSize, outLoginList);
		}
	}
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark ₯₯ Ban File ₯₯

void CMyApplication::ReadBans()
{
	UMemory::Dispose(mBanList);
	mBanList = nil;
	mBanCount = 0;
	
	StFSRef fp(kProgramFolder, nil, kMyBannedFileName, fsOption_PreferExistingFile);
	
	Uint32 s, c;
	if (fp.IsValid())
	{
		fp->Open(perm_Read);
		
		s = fp->GetSize();
		if (s)
		{
			StPtr buf(s);
			s = fp->Read(0, buf, s);
			
			if (s)
			{
				mBanList = TextToIPAddressList(buf, s, c);
				mBanCount = c;
			}
		}
	}
}

bool CMyApplication::IsBannedAddress(const SInternetAddress& inAddr)
{
	if (mBanList && mBanCount)
	{
		Uint32 addr = *(Uint32 *)inAddr.host;
		Uint32 n = mBanCount;
		Uint32 *lp = (Uint32 *)mBanList;
		
		while (n--)
		{
			if (*lp++ == addr) 
				return true;
		}
	}
	
	return false;
}

void CMyApplication::WritePermBanInfo(CPtrList<SMyPermBanInfo> *inPermBanList)
{
	StFSRef fp(kProgramFolder, nil, kMyBannedFileName);
	if (fp->Exists())
		fp->DeleteFile();

	if (inPermBanList->GetItemCount())
	{
		// write ban file
		fp->CreateFileAndOpen('TEXT', 'ttxt', kAlwaysOpenFile, perm_ReadWrite);
	
		Uint8 bufURL[16];
		SIPAddress stIPAddress;
		Uint32 nOffset = 0;

		try
		{
			Uint32 i = 0;
			SMyPermBanInfo *pPermBanInfo;
				
			while(inPermBanList->GetNext(pPermBanInfo, i))
			{	
				stIPAddress.un_IP.stDW_IP.nDW_IP = pPermBanInfo->nAddr;
				bufURL[0] = UTransport::IPAddressToText(stIPAddress, bufURL + 1, sizeof(bufURL) - 1);
			
				if (bufURL[0])
				{
					nOffset += fp->Write(nOffset, bufURL + 1, bufURL[0]);
					nOffset += fp->Write(nOffset, "\r", 1);
				#if WIN32
					nOffset += fp->Write(nOffset, "\n", 1);
				#endif
				}
				
				UMemory::Dispose((TPtr)pPermBanInfo);
			}
		}
		catch(...)
		{
			inPermBanList->Clear();
			fp->Close();
			fp->DeleteFile();
			// don't throw
		}

		inPermBanList->Clear();
		fp->Close();
	}
	
	ReadBans();
}

void CMyApplication::ReadPermBanInfo(CPtrList<SMyPermBanInfo> *outPermBanList)
{
	if (mBanList && mBanCount)
	{
		Uint32 n = mBanCount;
		Uint32 *lp = (Uint32 *)mBanList;
		
		while (n--)
		{
			SMyPermBanInfo *pPermBanInfo = (SMyPermBanInfo *)UMemory::New(sizeof(SMyPermBanInfo));
			pPermBanInfo->nAddr = *lp++;				
	
			outPermBanList->AddItem(pPermBanInfo);
		}
	}
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -

void CMyApplication::WriteIPAddress()
{
	StFSRef fp(kProgramFolder, nil, kMyTrackerIPFileName);
	if (fp->Exists())
		fp->DeleteFile();

	if (!mIPAddress.un_IP.stDW_IP.nDW_IP)
		return;
	
	// write IP
	fp->CreateFileAndOpen('TEXT', 'ttxt', kAlwaysOpenFile, perm_ReadWrite);
	
	try
	{
		Uint8 bufURL[16];
		bufURL[0] = UTransport::IPAddressToText(mIPAddress, bufURL + 1, sizeof(bufURL) - 1);
			
		if (bufURL[0])
			fp->Write(0, bufURL + 1, bufURL[0]);
	}
	catch(...)
	{
		fp->Close();
		fp->DeleteFile();
		// don't throw
	}

	fp->Close();
}

void CMyApplication::ReadIPAddress()
{
	StFSRef fp(kProgramFolder, nil, kMyTrackerIPFileName, fsOption_PreferExistingFile);
	
	if (fp.IsValid())
	{
		fp->Open(perm_Read);
		
		Uint32 nSize = fp->GetSize();
		if (nSize)
		{
			StPtr buf(nSize);
			nSize = fp->Read(0, buf, nSize);
			
			if (nSize)
				mIPAddress = TextToIPAddress(buf, nSize);
		}
	}
}

void CMyApplication::WriteMaxServersPerIP()
{
	StFSRef fp(kProgramFolder, nil, kMyServersPerIPFileName);
	if (fp->Exists())
		fp->DeleteFile();

	if (!mMaxServersPerIP)
		return;
	
	// write server/IP
	fp->CreateFileAndOpen('TEXT', 'ttxt', kAlwaysOpenFile, perm_ReadWrite);
	
	try
	{
		Uint8 bufURL[32];
		bufURL[0] = UText::IntegerToText(bufURL + 1, sizeof(bufURL) - 1, mMaxServersPerIP);
			
		if (bufURL[0])
			fp->Write(0, bufURL + 1, bufURL[0]);
	}
	catch(...)
	{
		fp->Close();
		fp->DeleteFile();
		// don't throw
	}

	fp->Close();
}

void CMyApplication::ReadMaxServersPerIP()
{
	StFSRef fp(kProgramFolder, nil, kMyServersPerIPFileName, fsOption_PreferExistingFile);
	
	if (fp.IsValid())
	{
		fp->Open(perm_Read);
		
		Uint32 nSize = fp->GetSize();
		if (nSize)
		{
			StPtr buf(nSize);
			nSize = fp->Read(0, buf, nSize);
			
			if (nSize)
				mMaxServersPerIP = UText::TextToUInteger(buf, nSize);
		}
	}
}

void CMyApplication::BuildServerPerIPList()
{
	ClearServerPerIPList();
	if (!mMaxServersPerIP || !mServerList || !mServerCount)
		return;
 
	SMyServerInfo **pp, **spp, **epp, *info;
	spp = (SMyServerInfo **)UMemory::Lock(mServerList);
	epp = spp - 1;
	pp = epp + mServerCount;

	while (pp != epp)
	{
		info = *pp;
		
		if (!AddServerPerIP(info->address))
		{			
			mUserCount -= info->userCount;
			
			UMemory::Dispose((TPtr)info);
			UMemory::Copy(pp, pp + 1, BPTR(spp + mServerCount) - BPTR(pp + 1));
			mServerCount--;
		}
		
		pp--;
	}
	
	UMemory::Unlock(mServerList);
		
	UpdateDisplay();
}

bool CMyApplication::FindServerByAddress(Uint32 inAddr, Uint32& outIndex)
{
	if (!mServerPerIPList.GetItemCount())
	{
		outIndex = 0;
		return false;
	}

	Uint32 nAddr = mServerPerIPList.GetItem(1)->address;
	if (inAddr == nAddr)
	{
		outIndex = 1;
		return true;
	}
	else if (inAddr < nAddr)
	{
		outIndex = 0;
		return false;
	}
	
	Uint32 nFirst = 2;
	Uint32 nLast = mServerPerIPList.GetItemCount();
	if (nFirst > nLast)
	{
		outIndex = 2;
		return false;
	}
	
	Uint32 nIndex;
	while (nFirst <= nLast)
	{
		nIndex = (nFirst + nLast)/2;
		nAddr = mServerPerIPList.GetItem(nIndex)->address;
	
		if (inAddr == nAddr)
		{
			outIndex = nIndex;
			return true;
		}
		else if (inAddr < nAddr)
			nLast = nIndex - 1;
		else
			nFirst = nIndex + 1;
	}
	
	if (inAddr > nAddr)
		nIndex++;
	
	outIndex = nIndex;
	return false;	
}

bool CMyApplication::AddServerPerIP(Uint32 inAddr)
{
	if (!mMaxServersPerIP)	// don't need to add to list
		return true;

	Uint32 nIndex;
	if (FindServerByAddress(inAddr, nIndex))
	{
		SMyServerPerIP *pServerPerIP = mServerPerIPList.GetItem(nIndex);
		if (pServerPerIP)
		{
			if (pServerPerIP->count >= mMaxServersPerIP)
				return false;
		
			pServerPerIP->count++;
		}
	}
	else
	{
		SMyServerPerIP *pServerPerIP = (SMyServerPerIP *)UMemory::New(sizeof(SMyServerPerIP));
		
		pServerPerIP->address = inAddr;
		pServerPerIP->count = 1;
		
		mServerPerIPList.InsertItem(nIndex, pServerPerIP);
	}
	
	return true;
}

bool CMyApplication::RemoveServerPerIP(Uint32 inAddr)
{
	Uint32 nIndex;
	if (!FindServerByAddress(inAddr, nIndex))
		return false;

	SMyServerPerIP *pServerPerIP = mServerPerIPList.GetItem(nIndex);
	if (!pServerPerIP)
		return false;
	
	pServerPerIP->count--;
	if (!pServerPerIP->count)
	{
		mServerPerIPList.RemoveItem(nIndex);
		UMemory::Dispose((TPtr)pServerPerIP);
	}
	
	return true;
}

void CMyApplication::ClearServerPerIPList()
{
	Uint32 i = 0;
	SMyServerPerIP *pServerPerIP;
	
	while (mServerPerIPList.GetNext(pServerPerIP, i))
		UMemory::Dispose((TPtr)pServerPerIP);
	
	mServerPerIPList.Clear();
}

#if USE_LOG_WIN
void CMyApplication::Log(const Uint8 *inName, const SInternetAddress& inAddr, const Uint8 *inPasswd)
{
	Uint8 data[256];
	Uint8 addr[16];
	Uint8 date[64];
	
	CTextView *v = mLogWin->GetTextView();
	
	// clear log if getting too big
	THdl h = v->GetTextHandle();
	if (h)
	{
		if (UMemory::GetSize(h) > 16384)
			v->DeleteText(0, max_Uint32);
	}

	addr[0] = UText::Format(addr + 1, sizeof(addr) - 1, "%li\.%li\.%li\.%li", inAddr.host[0], inAddr.host[1], inAddr.host[2], inAddr.host[3]);
	date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateText + kTimeWithSecsText);
	
	Uint32 s = UText::Format(data, sizeof(data), "\r%#-21.21s %#-15.15s %#-20.20s %#-20.20s", date, addr, inName, inPasswd);
				
	// append to log
	v->InsertText(max_Uint32, data, s);
	v->SetFullHeight();
	mLogWin->ScrollToBottom();
	
	// also log to log file
	try
	{
		StFileSysRef logFile(kProgramFolder, nil, "\pServerReg Log.txt");
		StFileOpener open(logFile);
		logFile->Write(logFile->GetSize(), data, s);
	}
	catch(...)
	{
	}
}
#endif

