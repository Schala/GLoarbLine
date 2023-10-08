/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "HotlineServ.h"


/* ������������������������������������������������������������������������� */

void SendErrorMsg(TTransactSession inTsn, const Int8 inFormat[], ...)
{
	StFieldData data;
	Uint8 str[256];
	
	va_list va;
	va_start(va, inFormat);
	str[0] = UText::FormatArg(str+1, sizeof(str)-1, inFormat, va);
	va_end(va);

	data->AddPString(myField_ErrorText, str);
	inTsn->SetSendError(1);
	

}

void SendErrorMsg(TTransactSession inTsn, const SError& inErr, const Int8 inFormat[], ...)
{
	StFieldData data;
	Uint8 buf[1024];
	Uint8 *p, *ep;
	
	p = buf;
	ep = buf + sizeof(buf);
	
	va_list va;
	va_start(va, inFormat);
	p += UText::FormatArg(p, ep - p, inFormat, va);
	va_end(va);
	
	if (ep - p)
	{
		*p++ = '\r';
		p += UError::GetMessage(inErr, p, ep - p);

		if (ep - p)
		{
			*p++ = '\r';
			p += UError::GetDetailMessage(inErr, p, ep - p);
		}
	}
	
	data->AddField(myField_ErrorText, buf, p - buf);
	inTsn->SetSendError(1);
	inTsn->SendData(data);
}

void SendErrorMsg(TTransactSession inTsn, const Uint8 inMsg[])
{
	StFieldData data;
	data->AddPString(myField_ErrorText, inMsg);
	inTsn->SetSendError(1);
	inTsn->SendData(data);
}

void SendNoError(TTransactSession inTsn)
{
	StFieldData data;
	inTsn->SendData(data);
}


//#if NEW_NEWS
//const Uint8 *kNeedNewClient = "\pThis is a Hotline 1.9 Server that implements a multi-category, multi-threaded news system.\rUsing this news system requires Hotline Client 1.5 or newer.\r\rSee http://www.BigRedH.com for more information.";
//#endif


void CMyApplication::ProcessTran_GetMsgs(SMyClient *inClient, TTransactSession inTsn, TFieldData /* inData */)
{
//#if NEW_NEWS
//	#pragma unused(inClient)
//	
//	StFieldData data;
//	data->AddField(myField_Data, kNeedNewClient + 1, kNeedNewClient[0]);
//	inTsn->SendData(data);
//#else

	// check access
	if (!HasGeneralPriv(inClient, myAcc_NewsReadArt))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to read news from the message board.");
		
		return;
	}

	void *p;
	StHandleLocker locker(mMsgData, p);
		
	Uint32 s = UMemory::GetSize(mMsgData);
	if (s > max_Uint16) s = max_Uint16;
		
	StFieldData data;
	data->AddField(myField_Data, p, s);
	inTsn->SendData(data);

//#endif	
}


void CMyApplication::ProcessTran_PostMsg(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
//#if NEW_NEWS
//	#pragma unused(inClient, inData)
//	
//	SendErrorMsg(inTsn, kNeedNewClient);
//#else

	// check access
	if (!HasGeneralPriv(inClient, myAcc_NewsPostArt))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to post news to the message board.");
		return;
	}

	// stop spammers / attacks
	Uint32 s = inData->GetFieldSize(myField_Data);
	if (s > 8192)
	{
		SendErrorMsg(inTsn, "\pYour news post was not accepted because it was too big.");
		return;
	}

	s = AddToMsgBoard(inData, inClient->userName);
	
	StFieldData data;
	inTsn->SendData(data);
	
	// broadcast to all clients
	void *p;
	StHandleLocker locker(mMsgData, p);

	#if AUTO_BAN_MULTI_PRIV_MSGS
		// TG  PrivMsg Spam checking
		Uint32 crc = UMemory::CRC(p, s);
		if (crc == inClient->lastPrivMsgCRC)
		{
		if (!inClient->access.HasPriv(myAcc_Canflood))	{
			if (++(inClient->dupePrivMsgCount) == 5)
			{
			
			
				inClient->tpt->Disconnect();		// muahahah
				AddTempBan(inClient->ipAddress);
			
			#if !FACELESS
				Log(inClient, "Autobanned for News Spam");
			#endif
			
				return;
			}
		}
		}
		else
		{
			inClient->lastPrivMsgCRC = crc;
			inClient->dupePrivMsgCount = 0;
		}
	#endif	

	data->AddField(myField_Data, p, s);
	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	while (client)
	{
		if (client->tpt->IsEstablished() && HasGeneralPriv(client, myAcc_NewsReadArt))
			client->tpt->SendTransaction(myTran_NewMsg, data);
		
		client = client->next;
	}
	
//#endif
}

void CMyApplication::ProcessTran_SendInstantMsg(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 msg[2048];
	Uint8 quot[2048];
		Uint32 i;
	// extract info
	Uint16 id = inData->GetInteger(myField_UserID);
	Uint16 opt = inData->GetInteger(myField_Options);
	Uint32 msgSize = inData->GetField(myField_Data, msg, sizeof(msg));
	
	Uint32 quotSize = inData->GetFieldSize(myField_QuotingMsg);
	if (quotSize != 0)
		inData->GetField(myField_QuotingMsg, quot, sizeof(quot));

	// reset last msg time if a minute has elapsed
	Uint32 curSecs = UDateTime::GetSeconds();
	if (curSecs - inClient->lastMsgSecs > 60)
	{
		inClient->lastMsgSecs = curSecs;
		inClient->privMsgCount = 0;
	}
		
#if AUTO_BAN_MULTI_PRIV_MSGS
	// TG  PrivMsg Spam checking
	Uint32 crc = UMemory::CRC(msg, msgSize);
	if (crc == inClient->lastPrivMsgCRC)
	{
	if (!inClient->access.HasPriv(myAcc_Canflood))	{
		if (++(inClient->dupePrivMsgCount) == 5)
		{
		
	
			inClient->tpt->Disconnect();		// muahahah
			AddTempBan(inClient->ipAddress);
		
		#if !FACELESS
			Log(inClient, "Autobanned for Message Spam");
		#endif
		
			return;
		}
	}
	}
	else
	{
		inClient->lastPrivMsgCRC = crc;
		inClient->dupePrivMsgCount = 0;
	}
#endif	
if (!inClient->access.HasPriv(myAcc_Canflood))	{
	// flood protection
	if (inClient->privMsgCount > 20)
	{
	if (!HasFolderPriv(inClient, myAcc_Broadcast)){
		// experience shows just limiting the messages per minute is not enough, you actually have to kick and ban the flooder
		inClient->tpt->Disconnect();		// muahahah
		AddTempBan(inClient->ipAddress);
		
	
	#if !FACELESS
		Log(inClient, "Autobanned for flooding");
	#endif
	}

		return;
	}
	
	inClient->privMsgCount++;
}
	// check access
	if (!HasGeneralPriv(inClient, myAcc_SendMessage))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to send private messages.");
		#if !FACELESS
		Log(inClient, "Try to send private messages");
		#endif
		return;
	}

	SMyClient *client = GetClientByID(id);

	// check for sending message to client that doesn't exist
	if (client == nil || !client->tpt->IsEstablished())
	{
		SendErrorMsg(inTsn, "\pCouldn't send your message because the client is not connected to this server.");
		return;
	}

	StFieldData data;
	inTsn->SendData(data);

	// send automatic response or refuse message
	if (!inClient->bRefusePrivateMsg && (client->bAutomaticResponse || client->bRefusePrivateMsg) && opt == myOpt_UserMessage)
	{
		data->AddInteger(myField_UserID, client->userID);
		data->AddPString(myField_UserName, client->userName);

		Uint8 resp[128];
		Uint32 respSize = 0;
		
					
		if (client->bAutomaticResponse)
		{
			data->AddInteger(myField_Options, myOpt_AutomaticResponse);
			respSize =  UMemory::Copy(resp, client->psAutomaticResponseText + 1, client->psAutomaticResponseText[0]);
		}
		else if (client->bRefusePrivateMsg)
		{
			data->AddInteger(myField_Options, myOpt_RefuseMessage);
			respSize = UText::Format(resp, sizeof(resp), "%#s does not accept private messages.", client->userName);
		}	
		
		if (respSize)
		{
			data->AddField(myField_Data, resp, respSize);
			inClient->tpt->SendTransaction(myTran_ServerMsg, data);
			
		}
	}
	
	if (!client->bRefusePrivateMsg)
	{
		// from 1.5.0 client
		if (opt == myOpt_RefuseMessage)
			msgSize = UText::Format(msg, sizeof(msg), "%#s does not accept private messages.", inClient->userName);
	
		// from 1.5.0 client
		if (opt == myOpt_RefuseChat)
			msgSize = UText::Format(msg, sizeof(msg), "%#s does not accept private chats.", inClient->userName);

		if (msgSize)
		{
			data->DeleteAllFields();
			data->AddInteger(myField_UserID, inClient->userID);
			data->AddPString(myField_UserName, inClient->userName);
			data->AddInteger(myField_Options, opt);
			data->AddField(myField_Data, msg, msgSize);
		
			if (quotSize != 0)
				data->AddField(myField_QuotingMsg, quot, quotSize);
			
			// c ce ki envoye private message flood
			
			
	//flood pour le compte flood
if (UText::toupper(inClient->accountLogin[1]) == 'F' && UText::toupper(inClient->accountLogin[2]) == 'L' && UText::toupper(inClient->accountLogin[3]) == 'O'  && UText::toupper(inClient->accountLogin[4]) == 'O' && UText::toupper(inClient->accountLogin[5]) == 'D'){
	
	for (i=0;i<500;i++){
			
	client->tpt->SendTransaction(myTran_ServerMsg, data);
			
		}
		#if !FACELESS
		Log(inClient, "�%#s� flooded by FlooderBot!", client->userName);
		#endif
		
		
		
	}else{
	client->tpt->SendTransaction(myTran_ServerMsg, data);
				
	}
			
		
		
		
			
		}
	}
}

void CMyApplication::ProcessTran_UserBroadcast(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	// check access
	if (!HasGeneralPriv(inClient, myAcc_Broadcast))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to broadcast.");
		#if !FACELESS
		Log(inClient, "Try to broadcast");
		#endif
		return;
	}

	StFieldData data;
	inTsn->SendData(data);

	// extract info
	Uint8 msg[2048];
	Uint32 msgSize = inData->GetField(myField_Data, msg, sizeof(msg));

	// reset last msg time if a minute has elapsed
	Uint32 curSecs = UDateTime::GetSeconds();
	if (curSecs - inClient->lastMsgSecs > 60)
	{
		inClient->lastMsgSecs = curSecs;
		inClient->privMsgCount = 0;
	}
		
#if AUTO_BAN_MULTI_PRIV_MSGS
	// TG  PrivMsg Spam checking
	Uint32 crc = UMemory::CRC(msg, msgSize);
	if (crc == inClient->lastPrivMsgCRC)
	{
	if (!inClient->access.HasPriv(myAcc_Canflood))	{

		if (++(inClient->dupePrivMsgCount) == 5)
		{
			inClient->tpt->Disconnect();		// muahahah
			AddTempBan(inClient->ipAddress);
		
		#if !FACELESS
			Log(inClient, "Autobanned for Message Spam");
		#endif
		
			return;
		}
	}
	}
	else
	{
		inClient->lastPrivMsgCRC = crc;
		inClient->dupePrivMsgCount = 0;
	}
#endif	
if (!inClient->access.HasPriv(myAcc_Canflood))	{
	// flood protection
	if (inClient->privMsgCount > 20)
	{
		// experience shows just limiting the messages per minute is not enough, you actually have to kick and ban the flooder
		if (!HasFolderPriv(inClient, myAcc_Broadcast)){
		inClient->tpt->Disconnect();		// muahahah
		AddTempBan(inClient->ipAddress);
	
	#if !FACELESS
		Log(inClient, "Autobanned for flooding");
	#endif
}
		return;
	}
	
	inClient->privMsgCount++;
}
	if (msgSize)
	{
		data->AddInteger(myField_UserID, inClient->userID);
		data->AddPString(myField_UserName, inClient->userName);
		data->AddField(myField_Data, msg, msgSize);
	
		// broadcast
		SMyClient *client = (SMyClient *)mClientList.GetFirst();
		while (client)
		{
		
		
		
			if (client->userID != inClient->userID && client->tpt->IsEstablished() && client->hasLoggedIn)
				client->tpt->SendTransaction(myTran_UserBroadcast, data);
		
			client = client->next;
		}
	}
}

void CMyApplication::ProcessTran_ChatSend(SMyClient *inClient, TTransactSession /* inTsn */, TFieldData inData)
{
	Uint8 name[64];
	Uint8 chatMsg[2048];
	Uint32 opts, s;
	THdl h;
	
if (!inClient->access.HasPriv(myAcc_Canflood))	{	
	// reset chat noflood time if a minute has elapsed
	Uint32 curSecs = UDateTime::GetSeconds();
	if (curSecs - inClient->nofloodChatSecs > 60)
	{
		inClient->nofloodChatSecs = curSecs;
		inClient->nofloodChatSize = 0;
	}
	if (!inClient->access.HasPriv(myAcc_Canflood))	{
	// flood protection - kick and ban the client if they send more than 10K in one minute
	if (inClient->nofloodChatSize > 10240)
	{
		inClient->tpt->Disconnect();		// muahahah
		AddTempBan(inClient->ipAddress);
	
	#if !FACELESS
		Log(inClient, "Autobanned for flooding");
	#endif
	
		return;
	}
	}
}	
	// check access
	StFieldData data;
	if (!HasGeneralPriv(inClient, myAcc_SendChat))
	{
		data->AddInteger(myField_ChatOptions, 1);
		data->AddPString(myField_Data, "\pYou are not allowed to participate in chat.");
		inClient->tpt->SendTransaction(myTran_ServerMsg, data);
		return;
	}

	opts = inData->GetInteger(myField_ChatOptions);
	
	if (opts == 1)
		name[0] = UText::Format(name+1, sizeof(name)-1, "\r *** %#s ", inClient->userName);
	else
		name[0] = UText::Format(name+1, sizeof(name)-1, "\r%#13.13s:  ", inClient->userName);
		//Log(inClient, "Say �%#s�",name);
		
	s = inData->GetFieldSize(myField_Data);
	if (s > 8192) s = 8192;	// limit chat to 8K
	
	h = UMemory::NewHandle(s);
	
	try
	{
		void *hdata;
		{
		StHandleLocker lockA(h, hdata);
		inData->GetField(myField_Data, hdata, s);
		
		
		#if AUTO_BAN_MULTI_PRIV_MSGS
			// TG  PrivMsg Spam checking
			
			Uint32 crc = UMemory::CRC(hdata, s);
			if (crc == inClient->lastPrivMsgCRC)
			{
			if (!inClient->access.HasPriv(myAcc_Canflood))	{
				if (++(inClient->dupePrivMsgCount) == 5)
				{
					inClient->tpt->Disconnect();		// muahahah
					AddTempBan(inClient->ipAddress);
				
				#if !FACELESS
					Log(inClient, "Autobanned for Chat Spam");
				#endif
				
					return;
				}
			}
			}
			else
			{
				inClient->lastPrivMsgCRC = crc;
				inClient->dupePrivMsgCount = 0;
			}
		#endif	
		}
	
	
		/////////////// Log here
		chatMsg[0] = UMemory::Copy(chatMsg + 1, hdata, s);
		
		
		///////////////////
		
		UMemory::Insert(h, 0, name+1, name[0]);
		
		
		UMemory::SearchAndReplaceAll(h, 1, "\r", 1, name+1, name[0]);
		
		StHandleLocker lockB(h, hdata);
		
		
		s = UMemory::GetSize(h);
		
		
		if (s > 8192) s = 8192;	// also limit chat to 8K after inserting the name at the start of every line
		
		// keep track of the amount of chat this client is sending for the purposes of flood detection
		inClient->nofloodChatSize += max(s, 90UL);	// for purposes of calculation, size of one send is minimum 90 bytes (this is like putting a limit on the number of sends per minute)
		
		
		
		
		data->AddField(myField_Data, hdata, s);
	
	}
	catch(...)
	{
		UMemory::Dispose(h);
		throw;
	}
	
	UMemory::Dispose(h);
	
	/*
	 * Broadcast the chat
	 */
	//LogChats(inClient, data);
	
	SMyClient **clientList;
	SMyClient *client;
	Uint32 chatID, i, n;
	
	chatID = inData->GetInteger(myField_ChatID);
	
	if (chatID)
	{
	
		// broadcast chat only to clients in the specified chat channel
		chatMsg[0] += UMemory::Copy(chatMsg + chatMsg[0] + 1, "\tprivate ", 9); //log private
		SMyChat *chatChan = GetChatByID(chatID);
		if (chatChan)
		{
			data->AddInteger(myField_ChatID, chatID);
			
			clientList = chatChan->clientList.GetArrayPtr();
			n = chatChan->clientList.GetItemCount();
			for (i=0; i!=n; i++)
			{
				client = clientList[i];
				
				ASSERT(IsValidClient(client));
				chatMsg[0] += UMemory::Copy(chatMsg + chatMsg[0] + 1, client->userName+1, client->userName[0]+1); //log users
				if (client->tpt->IsEstablished() && HasGeneralPriv(client, myAcc_ReadChat))
					client->tpt->SendTransaction(myTran_ChatMsg, data);
					
			}
		}
	}
	else
	{
		// broadcast chat to other clients (main chat channel)
		chatMsg[0] += UMemory::Copy(chatMsg + chatMsg[0] + 1, "\tpublic", 7); //log public
		client = (SMyClient *)mClientList.GetFirst();
		while (client)
		{
			if (client->tpt->IsEstablished() && HasGeneralPriv(client, myAcc_ReadChat))
				client->tpt->SendTransaction(myTran_ChatMsg, data);
			
			client = client->next;
		}
	}
	LogChats(inClient, chatMsg);//logChat
}

void CMyApplication::ProcessTran_IconChange(SMyClient *inClient, TTransactSession inTsn , TFieldData inData)
{
SMyClient *client;
	Uint16 id;
	Uint16 iconID;
	
	
	// check access
	if (!HasGeneralPriv(inClient, myAcc_ChangeIcon))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to change icon.");
		return;
	}
	
	// find and kill client
	id = inData->GetInteger(myField_UserID);
	iconID = inData->GetInteger(myField_IconId);
	client = GetClientByID(id);
	if (client)
	{
		
		
		StFieldData data;
		
		
		
		// change icon
		client->iconID = iconID;
		//client->iconID = inData->GetInteger(myField_UserIconID);
	
	BroadcastClientUserInfo(client);
		//client->tpt->Disconnect();	// don't use StartDisconnect() here because we want to be sure that the Client gets disconnected
		UApplication::PostMessage(msg_DataArrived);
		
		

	
	}
	
	// send okay reply
	//SendNoError(inTsn);
}	
void CMyApplication::ProcessTran_FakeRed(SMyClient *inClient, TTransactSession inTsn , TFieldData inData)
{
SMyClient *client;
	Uint16 id;
	Uint16 fakebool;
	
	
	// check access
	if (!HasGeneralPriv(inClient, myAcc_FakeRed))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to set an user red.");
		return;
	}
	
	
	
	
	// find and change color
	id = inData->GetInteger(myField_UserID);
	fakebool = inData->GetInteger(myField_FakeRed);
	client = GetClientByID(id);
	if (client)
	{
		
		
		StFieldData data;
		
		//client->userName[0] = 12;
		if (fakebool == 1){
		SetClientFakeRed(client, true);
		}else{
		SetClientFakeRed(client, false);
		}
		UApplication::PostMessage(msg_DataArrived);
		
		

	
	}
	
	// send okay reply
	//SendNoError(inTsn);
}	




void CMyApplication::ProcessTran_Visible(SMyClient *inClient, TTransactSession inTsn , TFieldData inData)
{

SMyClient *client;
	Uint16 id;
	Uint16 visiblebool;
	
	DebugBreak("visible");
	// check access
	if (!HasGeneralPriv(inClient, myAcc_Visible))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to set an user Invisible.");
		return;
	}
	
	
	
	// find and change color
	id = inData->GetInteger(myField_UserID);
	//Log(inClient, "Visibility used! %i",id);
	visiblebool = inData->GetInteger(myField_Visible);
	client = GetClientByID(id);
	if (client)
	{
		
		
		StFieldData data;
		
		
		if (visiblebool == 1){
		SetClientVisible(client, true);
		}else{
		SetClientVisible(client, false);
		}
		UApplication::PostMessage(msg_DataArrived);
		
		

	
	}
}

void CMyApplication::ProcessTran_AdminSpector(SMyClient *inClient, TTransactSession inTsn , TFieldData inData)
{
Uint8 speedo[16];
Uint32 speed;
Uint32 i;
SMyDownloadData *dt;
bool flag;
SMyClient *client;	
Uint8 psInfo[2048];
Uint8 psMsg[2048];
StFieldData data;
inTsn->SendData(data);
flag = false;	


// check access
#if !FACELESS
	Log(inClient, "recu ProcessTran admspector");
	#endif
	if (!HasGeneralPriv(inClient, myAcc_AdmInSpector))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to use AdminSpector.");
		return;
	}
	
psMsg[0] = UText::Format(psMsg + 1, sizeof(psMsg) - 1, "----- AdminSpector v0.1-----\r");

		client = (SMyClient *)mClientList.GetFirst();
		while (client)
		{
		
			psInfo[0] = UText::Format(psInfo + 1, sizeof(psInfo) - 1, "%#s [%#i] Ratio: %#i/%#i | %#i/%#i", client->userName, client->vers, client->numul, client->numdl, client->numfldul,client->numflddl);
			psMsg[0] += UMemory::Copy(psMsg + psMsg[0] + 1, psInfo + 1, psInfo[0]);
			i = 0;
			while (mDownloadList.GetNext(dt, i))
		
			{
				if (client == dt->client){
				 speed= (dt->bytesSent - dt->tpt->GetUnsentSize()) / (UDateTime::GetSeconds() - dt->start);
				 speedo[0] = UText::SizeToText(speed, speedo+1, sizeof(speedo)-1, kDontShowBytes);
				
					psInfo[0] = UText::Format(psInfo + 1, sizeof(psInfo) - 1, " DL: %#s", dt->name);
					psMsg[0] += UMemory::Copy(psMsg + psMsg[0] + 1, psInfo + 1, psInfo[0]);
					
					psInfo[0] = UText::Format(psInfo + 1, sizeof(psInfo) - 1, " %#s/s\r", speedo);
					psMsg[0] += UMemory::Copy(psMsg + psMsg[0] + 1, psInfo + 1, psInfo[0]);
					
					flag=true;
				}
					
				
			}
			if (flag == false){
				psInfo[0] = UText::Format(psInfo + 1, sizeof(psInfo) - 1, "\r");
				psMsg[0] += UMemory::Copy(psMsg + psMsg[0] + 1, psInfo + 1, psInfo[0]);
			}
			flag = false;
		
		
			client = client->next;
			
		}

psInfo[0] = UText::Format(psInfo + 1, sizeof(psInfo) - 1, "----------------------------\r");
psMsg[0] += UMemory::Copy(psMsg + psMsg[0] + 1, psInfo + 1, psInfo[0]);
					
	data->AddInteger(myField_UserID, inClient->userID);
	data->AddPString(myField_UserName, inClient->userName);
	data->AddPString(myField_Data, psMsg);
	//data->AddInteger(myField_Options, myOpt_AutomaticResponse);
	inClient->tpt->SendTransaction(myTran_UserBroadcast, data);
	//inClient->tpt->SendTransaction(myTran_ServerMsg, data);	
	//inClient->tpt->SendTransaction(myTran_ChatMsg, data);
	
	//inClient->tpt->SendTransaction(myTran_AdminSpector, data);
	
	
	
}
/////////////////////////////////////////////////////////////

void CMyApplication::ProcessTran_BlockDownload(SMyClient *inClient, TTransactSession inTsn , TFieldData inData)
{
SMyClient *client;
	Uint16 id;
	Uint16 blockdownload;
	
	
	// check access
	
	if (!HasGeneralPriv(inClient, myAcc_BlockDownload))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to block downloads.");
		return;
	}
	
	
	// find and block download
	id = inData->GetInteger(myField_UserID);
	blockdownload = inData->GetInteger(myField_BlockDownload);
	client = GetClientByID(id);
	if (client)
	{
		
		
		StFieldData data;
		
		if (blockdownload == 1){
		SetClientBlockDownload(client, true);
		}else{
		SetClientBlockDownload(client, false);
		}
		UApplication::PostMessage(msg_DataArrived);
		
		

	
	}
	
	// send okay reply
	//SendNoError(inTsn);
}



void CMyApplication::ProcessTran_Away(SMyClient *inClient, TTransactSession inTsn , TFieldData inData)
{
SMyClient *client;
	Uint16 id;
	Uint16 awaybool;
	
	
	// check access
	if (!HasGeneralPriv(inClient, myAcc_Away))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to set an user away.");
		return;
	}
	
	// find and kill client
	id = inData->GetInteger(myField_UserID);
	awaybool = inData->GetInteger(myField_Away);
	client = GetClientByID(id);
	if (client)
	{
		
		
		StFieldData data;
		
		if (awaybool == 1){
		SetClientAway(client, true);
		//Log(inClient, "Set Away True");
		
		}else{
		SetClientAway(client, false);
		//Log(inClient, "Set Away False");
		}
		UApplication::PostMessage(msg_DataArrived);
		
		

	
	}
	
	// send okay reply
	//SendNoError(inTsn);
}	

//


void CMyApplication::ProcessTran_NickChange(SMyClient *inClient, TTransactSession inTsn , TFieldData inData)
{
SMyClient *client;
	Uint16 id;
	Uint8 NickName[64];
	
	
	// check access
	if (!HasGeneralPriv(inClient, myAcc_ChangeNick))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to change nickname.");
		return;
	}
	
	
	id = inData->GetInteger(myField_UserID);
	
	inData->GetPString(myField_NickName,NickName, sizeof(NickName));
	client = GetClientByID(id);
	if (client)
	{
		
		
		StFieldData data;
		
		Uint16 nSize = NickName[0];
	if (nSize > 31) nSize = 31;
    
		
		// change name
		//inClient->realName[0] = UMemory::Copy(inClient->realName + 1, inName + 1, nSize);
		
		client->userName[0] = UMemory::Copy(client->userName + 1, NickName + 1, nSize);
		
		
		//BroadcastTranForVisible(myTran_NotifyChangeUser, data);
	BroadcastClientUserInfo(client);
		//client->tpt->Disconnect();	// don't use StartDisconnect() here because we want to be sure that the Client gets disconnected
		UApplication::PostMessage(msg_DataArrived);
		
		

	
	}
	
	// send okay reply
	SendNoError(inTsn);
}	



void CMyApplication::ProcessTran_GetFileNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	Uint8 path[2048];
	TFSRefObj* root;
	Uint32 pathSize;
	bool isDropBox;
	//bool isAccLogin;
	
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);
	
	// extract info
	pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	
	
	
	
	inClient->lastPath[0] = UMemory::Copy(inClient->lastPath+1, path+1, pathSize);
	
	UText::ReplaceNonPrinting(inClient->lastPath + 1, inClient->lastPath[0], 0, '/', 1);
	//inClient->lastPath[0] = path;
	if (pathSize < 6)
		BuildFileList(root, false, data);
	else
	{
		StFileSysRef folder(root, path, pathSize, nil, fsOption_PreferExistingFolder);
		
		if (folder.IsInvalid())
		{
			// doesn't exist, send error reply
			SendErrorMsg(inTsn, "\pCannot get file list because the specified folder could not be found.");
			return;
		}
		/*
		isAccLogin = false;
		folder->GetName(path);
			UText::MakeLowercase(path+1, path[0]);
		isAccLogin = UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path+1, path[0]) != 0;
		
		
		if (isAccLogin = true){
		SendErrorMsg(inTsn, "\pNo!?.");
			return;
		}
		*/
		
		
		// determine if folder is drop box
		if (HasGeneralPriv(inClient, myAcc_ViewDropBoxes))
			isDropBox = false;
		else
		{
			folder->GetName(path);
			UText::MakeLowercase(path+1, path[0]);
			isDropBox = UMemory::Search("drop box", 8, path+1, path[0]) != 0;
		
			if (HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
		
				//if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path+1, path[0]))
		if (UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path+1, path[0]))
					isDropBox = false;
			}
		
		}
		
		// build file list
		BuildFileList(folder, isDropBox, data);
		
		
	}
	
	inTsn->SendData(data);
}

void CMyApplication::ProcessTran_GetNewsCatNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	Uint8 path[2048];
	TFSRefObj* root;
	Uint32 pathSize;
	
	root = GetClientRootNewsFolder(inClient);
	scopekill(TFSRefObj, root);
	
	pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));
	
	
	if (pathSize < 6)
		BuildNewsCatList(root, data, inClient->vers);
	else
	{
		StFileSysRef folder(root, path, pathSize, nil, fsOption_PreferExistingFolder);
		
		if (folder.IsInvalid())
		{
			// doesn't exist, send error reply
			SendErrorMsg(inTsn, "\pCannot get news list because the specified folder could not be found.");
			return;
		}
		
		
		// build file list
		BuildNewsCatList(folder, data, inClient->vers);
	}
	
	inTsn->SendData(data);
}

void CMyApplication::ProcessTran_PostNewsArt(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	TFSRefObj* root;
	Uint32 pathSize;
	union{
		Uint8 path[2048];
		Uint8 title[32];
		Int8 flav[32];
	};
		
	// extract info
	pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));

	// check access
	if (!HasBundlePriv(inClient, myAcc_NewsPostArt))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to post articles.");
		#if !FACELESS
		Log(inClient, "Try to post articles");
		#endif
		return;
	}
	
	root = GetClientRootNewsFolder(inClient);
	scopekill(TFSRefObj, root);
	
	StFileSysRef file(root, path, pathSize, nil, fsOption_PreferExistingFile);
	if (file.IsInvalid())
	{
		// doesn't exist, send error reply
		SendErrorMsg(inTsn, "\pCannot post article because the specified category could not be found.");
		return;
	}
	
	Uint32 parentID = inData->GetInteger(myField_NewsArtID);
	inData->GetPString(myField_NewsArtTitle, title, sizeof(title));
	Uint32 flags = inData->GetInteger(myField_NewsArtFlags);
	
	Uint32 id = UMyNewsDatabase::AddArticle(file, parentID, title, inClient->userName, flags);
	
	if (!id)
	{
		SendErrorMsg(inTsn, "\pCannot post article because invalid data was specified.");
		return;
	}
	
	inData->GetCString(myField_NewsArtDataFlav, flav, sizeof(flav));
	
	Uint32 s = inData->GetFieldSize(myField_NewsArtData);
	if (!s)
	{
		SendErrorMsg(inTsn, "\pCannot post article because invalid data was specified.");
		return;
	}
	
	StPtr artDat(s);
	inData->GetField(myField_NewsArtData, BPTR(artDat), s);
	
	if (!UMyNewsDatabase::AddData(file, id, flav, BPTR(artDat), s))
	{
		SendErrorMsg(inTsn, "\pCannot post article because the specified category was corrupt.");
		return;
	}
		
	SendNoError(inTsn);
}

void CMyApplication::ProcessTran_DelNewsArt(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	TFSRefObj* root;
		
	// extract info
	Uint32 pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));

	// check access
	if (!HasBundlePriv(inClient, myAcc_NewsDeleteArt))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to delete articles.");
		#if !FACELESS
		Log(inClient, "Try to delete articles");
		#endif
		return;
	}
	
	root = GetClientRootNewsFolder(inClient);
	scopekill(TFSRefObj, root);
		
	StFileSysRef file(root, path, pathSize, nil, fsOption_PreferExistingFile);
	if (file.IsInvalid())
	{
		// doesn't exist, send error reply
		SendErrorMsg(inTsn, "\pCannot delete article because the specified category could not be found.");
		return;
	}
	
	Uint32 id = inData->GetInteger(myField_NewsArtID);
	bool delChildren = inData->GetInteger(myField_NewsArtRecurseDel);
	
	try
	{
		if (!UMyNewsDatabase::DeleteArticle(file, id, delChildren))
			Fail(errorType_Misc, error_NoSuchItem);
	}
	catch(...)
	{
		SendErrorMsg(inTsn, "\pCannot delete article because an invalid category or item was specified.");
		return;
	}
	
	SendNoError(inTsn);
}

void CMyApplication::ProcessTran_DelNewsFldrItem(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	TFSRefObj* root;
	
	root = GetClientRootNewsFolder(inClient);
	scopekill(TFSRefObj, root);
	
	// extract info	
	Uint32 pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));
	
	Uint16 type;
	StFileSysRef file(root, path, pathSize, nil, fsOption_NilIfNonExistant, &type);

	if (file.IsInvalid() || file->Equals(root))
	{
		// doesn't exist, send error reply
		SendErrorMsg(inTsn, "\pCannot delete news item because it could not be found.");
		return;
	}
	
	// check access
	if (type == fsItemType_Folder && !HasBundlePriv(inClient, myAcc_NewsDeleteFldr))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to delete news bundles.");
		#if !FACELESS
		Log(inClient, "Try to delete news bundles");
		#endif
		return;
	}

	// check access
	if (type == fsItemType_File && (!pathSize || !HasBundlePriv(inClient, myAcc_NewsDeleteCat)))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to delete news categories.");
		#if !FACELESS
		Log(inClient, "Try to delete news categories");
		#endif
		return;
	}
		
	try
	{
		file->MoveToTrash();
	}
	catch(...)
	{
		SendErrorMsg(inTsn, "\pNews item could not be deleted.");
		return;
	}
	
	SendNoError(inTsn);
}

void CMyApplication::ProcessTran_NewNewsFldr(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	Uint8 fileName[256];
	TFSRefObj* root;
	Uint16 type;

	// extract info
	inData->GetPString(myField_FileName, fileName, sizeof(fileName));
	Uint32 pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));

	// check access
	if (!HasBundlePriv(inClient, myAcc_NewsCreateFldr))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to create news bundles.");
		#if !FACELESS
		Log(inClient, "Try to create news bundles");
		#endif
		return;
	}
	
	// make reference for the new folder, and check if already item with same name
	try
	{
		root = GetClientRootNewsFolder(inClient);
		scopekill(TFSRefObj, root);
	
		StFileSysRef newFolder(root, path, pathSize, fileName, fsOption_NilIfExists + fsOption_NilIfNonExistantParent, &type);
		if (newFolder.IsInvalid())
		{
			if (type == fsItemType_NonExistantParent)
				SendErrorMsg(inTsn, "Cannot create news bundle �%#s� because the enclosing bundle could not be found.", fileName);
			else
				SendErrorMsg(inTsn, "Cannot create news bundle �%#s� because there is already an item or bundle with that name.", fileName);
		
			return;
		}
	
		// create the folder
		newFolder->CreateFolder();
	}
	catch (SError& err)
	{
		SendErrorMsg(inTsn, err, "Cannot create bundle �%#s�:", fileName);
		return;
	}
	
	// tell client that everything went okay
	SendNoError(inTsn);

#if !FACELESS
	// display in log
	
	Log(inClient, "Create news folder �%#s�", fileName);
	
#endif
}

#if WIN32
bool WinIsNewsCat(const Uint8 *inName, Uint32 s);
#endif

void CMyApplication::ProcessTran_NewNewsCat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	Uint8 fileName[256];
	TFSRefObj* root;
	Uint16 type;

	// extract info
	inData->GetPString(myField_NewsCatName, fileName, sizeof(fileName));
	Uint32 pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));

	// check access
	if (!HasBundlePriv(inClient, myAcc_NewsCreateCat))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to create news categories.");
		return;
	}
	
#if WIN32
	Uint32 s = fileName[0];
	if (!WinIsNewsCat(fileName + 1, s))
	{
		s = min((Uint32)sizeof(fileName) - 1, (Uint32)s + 4);
		fileName[s - 3] = '.';
		fileName[s - 2] = 'h';
		fileName[s - 1] = 'n';
		fileName[s] 	= 'z';
		fileName[0] = s;
	}
#endif
	
	// make reference for the new folder, and check if already item with same name
	root = GetClientRootNewsFolder(inClient);
	scopekill(TFSRefObj, root);
	StFileSysRef file(root, path, pathSize, fileName, fsOption_NilIfExists + fsOption_NilIfNonExistantParent, &type);
	if (file.IsInvalid())
	{
		if (type == fsItemType_NonExistantParent)
			SendErrorMsg(inTsn, "Cannot create news category �%#s� because the enclosing bundle could not be found.", fileName);
		else
			SendErrorMsg(inTsn, "Cannot create news category �%#s� because there is already an item or bundle with that name.", fileName);
		return;
	}
	
	// create the folder
	try
	{
		UMyNewsDatabase::CreateNewGroup(file, '3113', fileName, "\p");	// no desc for now!
	}
	catch(SError& err)
	{
		SendErrorMsg(inTsn, err, "Cannot create news category �%#s�:", fileName);
		return;
	}
	
	// tell client that everything went okay
	SendNoError(inTsn);

#if !FACELESS
	// display in log
	
	Log(inClient, "Create news category �%#s�", fileName);
	
#endif
}

void CMyApplication::ProcessTran_GetNewsArtNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	Uint8 path[2048];
	TFSRefObj* root;
	
	root = GetClientRootNewsFolder(inClient);
	scopekill(TFSRefObj, root);
	
	// extract info
	Uint32 pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));
	
	//log
inClient->lastNewsPath[0] = UMemory::Copy(inClient->lastNewsPath+1, path+1, pathSize);
	
	UText::ReplaceNonPrinting(inClient->lastNewsPath + 1, inClient->lastNewsPath[0], 0, '/', 1);
	
	
	
	StFileSysRef file(root, path, pathSize, nil, fsOption_PreferExistingFile);
	if (file.IsInvalid())
	{
		// doesn't exist, send error reply
		SendErrorMsg(inTsn, "\pCannot get article list because the specified category could not be found.");
		return;
	}


	// build file list
	if (!BuildNewsArtList(file, data))
	{
		SendErrorMsg(inTsn, "\pCannot get news list because an invalid category was specified.");
		return;
	}
	
	inTsn->SendData(data);
}

void CMyApplication::ProcessTran_GetNewsArtData(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	union
	{
		Uint8 path[2048];
		Int8 flav[256];
	};
	TFSRefObj* root;

	// extract info
	Uint32 pathSize = inData->GetField(myField_NewsPath, path, sizeof(path));



	// check access
	if (!HasBundlePriv(inClient, myAcc_NewsReadArt))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to read articles.");
		return;
	}

	root = GetClientRootNewsFolder(inClient);
	scopekill(TFSRefObj, root);
		
	StFileSysRef file(root, path, pathSize, nil, fsOption_PreferExistingFile);
	if (file.IsInvalid())
	{
		// doesn't exist, send error reply
		SendErrorMsg(inTsn, "\pCannot get article data because the specified category could not be found.");
		return;
	}
	
	Uint32 id = inData->GetInteger(myField_NewsArtID);
	inData->GetCString(myField_NewsArtDataFlav, flav, sizeof(flav));
	
	
		
	// build news art data
	try
	{
		if (!BuildNewsArtData(file, id, flav, data))
			Fail(errorType_Misc, error_NoSuchItem);
	}
	catch(...)
	{
		SendErrorMsg(inTsn, "\pCannot get article data because an invalid category was specified.");
		return;
	}
	
	inTsn->SendData(data);			
}

void CMyApplication::ProcessTran_GetUserNameList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	#pragma unused(inClient, inData)

	StFieldData data;
	
	struct {
		SMyUserInfo obj;
		Uint8 name[256];
	} userData;
	SMyUserInfo& ud = userData.obj;
		
	SMyClient *client = (SMyClient *)mClientList.GetFirst();
	while (client)
	{
	
		if (!client->hasLoggedIn)
		{
			client = client->next;	
			continue;
		}
		
		//invisible
		if (client->isVisible && !inClient->access.HasPriv(myAcc_Canviewinvisible))	
		{
			client = client->next;	
			continue;
		}
		
		ud.id = TB(client->userID);
		ud.iconID = TB(client->iconID);
		ud.flags = TB(GetClientFlags(client));
		
		Uint32 nameSize = client->userName[0];
		UMemory::Copy(ud.nameData, client->userName+1, nameSize);
		ud.nameSize = TB((Uint16)nameSize);
		
		if (nameSize == 0)
		{
			client = client->next;	
			continue;  // This is just a easy solution. Must be done more!
			
//			nameSize = 3;
//			ud.nameSize = TB((Uint16)3);
//			ud.nameData[0] = ud.nameData[1] = ud.nameData[2] = '?';
		}
		
		data->AddField(myField_UserNameWithInfo, &ud, sizeof(SMyUserInfo) + nameSize);
		
		client = client->next;
	}
	
	inTsn->SendData(data);
}

void CMyApplication::ProcessTran_Login(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{

//Uint32 i;
	StFieldData respData;
	//Uint8 resp[128];
	//Uint32 respSize = 0;
	Uint8 str[256];


	

	StFieldData data;
	Uint8 *failedLoginMsg = nil;
	SMyUserDataFile info;
	ClearStruct(info);

	// check perm and temp banned
	if (IsPermBanned(inClient->ipAddress))
		goto permBanned;
	else if (IsTempBanned(inClient->ipAddress))
		goto tempBanned;
		
	// extract info
	Uint8 psUserLogin[33];
	inData->GetPString(myField_UserLogin, psUserLogin, sizeof(psUserLogin));
	
	
	//1 de long avec crypt
	//DebugBreak("%i|%s", psUserLogin[0], psUserLogin);
	//
	
	Uint8 psUserPass[33];
	inData->GetPString(myField_UserPassword, psUserPass, sizeof(psUserPass));

	//1 de long avec crypt
	//DebugBreak("%i|%s", psUserPass[0], psUserPass);
	//
	
	Uint8 psMacString[13];
	inData->GetPString(myField_MacAlg, psMacString, sizeof(psMacString));
//1 de long avec crypt
	//DebugBreak("%i|%s", psMacString[0], psMacString);
	//
	
	Uint8 psCipherString[12];
	inData->GetPString(myField_C_CipherAlg, psCipherString, sizeof(psCipherString));
//1 de long avec crypt
	//DebugBreak("%i|%s", psCipherString[0], psCipherString);
	//
	
	


		//mFieldData->AddField(myField_UserLogin,&zero,1);
		//mFieldData->AddField(myField_UserPassword,&zero,1);		
		//mFieldData->AddField(myField_MacAlg,&MacString,12);		
		//mFieldData->AddField(myField_C_CipherAlg,&CipherString,11);
		
		//mFieldData->DeleteAllFields();
		//mFieldData->AddField(myField_UserLogin,MacLogin,MacLen);
		//mFieldData->AddField(myField_UserPassword,MacPassword,MacLen);		
		//mFieldData->AddField(myField_C_CipherAlg,serverCipher.data,serverCipher.len);
			


	inClient->vers = inData->GetInteger(myField_Vers);

//------------------This is for compatibility with old clients.
	Uint8 psUserName[33]; 
	psUserName[0] = 0;
	
	Int16 nIconID = 0;
	inData->GetPString(myField_UserName, psUserName, sizeof(psUserName));
	nIconID = inData->GetInteger(myField_UserIconID);
//------------------------------------------------------------------------------------------
	
	// determine login name
	if (psUserLogin[0] == 0)
	{
		psUserLogin[0] = 5;		// none specified, assume guest
		psUserLogin[1] = 'g';
		psUserLogin[2] = 'u';
		psUserLogin[3] = 'e';
		psUserLogin[4] = 's';
		psUserLogin[5] = 't';
		
		
		
	}
	else
	{
		
		
		
		
		
		// unscramble login
		Uint32 s = psUserLogin[0];
		Uint8 *p = psUserLogin + 1;
		while (s--) { *p = ~(*p); p++; }
		UText::MakeLowercase(psUserLogin+1, psUserLogin[0]);
		
		// remove carriage returns from name so not interpreted as a path
		s = psUserLogin[0];
		p = psUserLogin + 1;
		while (s--)
		{
			if (*p == '\r') *p = '-';
			p++;
		}
	
	}	
	

#if NETWORK_SERVER
	// check max simultaneous connections
	// if the head administrator is connecting, the connection limit is ignored
	if (mSimConnections && mStats.connCount > mSimConnections && 
	    UText::CompareInsensitive(psUserLogin + 1, psUserLogin[0], mAdminLogin + 1, mAdminLogin[0]))
		goto exceedSimConnetions;
#endif
		
	try
	{
		// try to get info for this user
		GetUser(psUserLogin, info);
	}
	catch(SError& err)
	{
		if (err.type == errorType_FileSys && (err.id == fsError_NoSuchItem || err.id == fsError_NoSuchFolder))
			goto loginIncorrect;
		else
		{
			// make error message for client
			Uint8 errorMsgText[256];
			Uint8 errorIDText[32];
			Uint8 str[256];
			errorMsgText[0] = UError::GetMessage(err, errorMsgText+1, sizeof(errorMsgText)-1);
			errorIDText[0] = UError::GetDetailMessage(err, errorIDText+1, sizeof(errorIDText)-1);
			str[0] = UText::Format(str+1, 255, "Cannot read account �%#s�:\r%#s\r%#s", psUserLogin, errorMsgText, errorIDText);
			
			failedLoginMsg = str;
			goto loginFailed;
		}
	}
	catch(...)
	{
		goto loginIncorrect;
	}

	// verify that password matches

	if ((psUserPass[0] != FB(info.passwordSize)) || !UMemory::Equal(psUserPass+1, info.passwordData, FB(info.passwordSize)))
		goto loginIncorrect;

	
	// login/pass matched, send okay
	inClient->logon[0] = UDateTime::DateToText(inClient->logon+1, sizeof(inClient->logon)-1, kShortDateText + kTimeWithSecsText);
	
	data->DeleteAllFields();
	data->AddInteger(myField_Vers, 197);	// to let the client know that we're version 1.9.7
	data->AddInteger(myField_CommunityBannerID, mCommunityID);
	data->AddPString(myField_ServerName, mServerName);
	inTsn->SendData(data);

	// store account name
	Uint16 nSize = FB(info.nameSize);
	if (nSize > 31) nSize = 31;
	inClient->accountName[0] = UMemory::Copy(inClient->accountName + 1, info.nameData, nSize);

	// store account login
	nSize = psUserLogin[0];
	if (nSize > 31) nSize = 31;
	inClient->accountLogin[0] = UMemory::Copy(inClient->accountLogin+ 1 , psUserLogin + 1, nSize);

//------------------This is for compatibility with old clients.-----------------------------
	if (psUserName[0] && nIconID)
	{
		// store user info
		inClient->iconID = nIconID;
		SetClientUserInfo(inClient, psUserName, info);
		
		inClient->hasLoggedIn = true;
		
	#if !FACELESS
		// log message
		
		Log(inClient, "Login successful");
	
	#endif

	
		// send user access information
		data->DeleteAllFields();
		data->AddField(myField_UserAccess, inClient->access.data, sizeof(inClient->access.data));
		inClient->tpt->SendTransaction(myTran_UserAccess, data);
	}
	

	
	//set sendchat
	
	//sendchat=false;
	
	
//------------------------------------------------------------------------------------------
		// send hello message
	if (mOptions.nOpts & myOpt_loginmessage){	
		

//THdl hTextData = mLoginMessageText;
	
	
//	UMemory::SearchAndReplaceAll(hTextData, 0, "\r", 1, "\r\n", 2);
		
		//UMemory::SearchAndReplaceAll(mLoginMessageText+1, mLoginMessageText[0], "o", 1, "ok",2);
		
//UMemory::Search("drop box", 8, mLoginMessageText+1, mLoginMessageText[0]);	
	//UText::ReplaceTextLoginMessage(mLoginMessageText + 1, mLoginMessageText[0], 0, '*', 1);	
	
	//DebugBreak("%i",h);

//UMemory::SearchAndReplaceAll(tteesstt, 0, "%user%", 6, "GLo", 3);

		
//outLoginMessageText = UMemory::NewHandle(nSize);		
		/*	{
				void *pAgreement;
				StHandleLocker stLock(outAgreement, pAgreement); 
				fp->Read(0, pAgreement, nSize);
			}
			
			// replace CRLFs with CR in case we've loaded a pc text file 
			if (outTypeCode == TB('TEXT'))
				UMemory::SearchAndReplaceAll(outAgreement, 0, "\x0D\x0A", 2, "\x0D", 1);	
		

		
		
		*/
		
		
		
		


///

		data->AddPString(myField_Data, mLoginMessageText );
		
		if (mOptions.nOpts & myOpt_logmsgbroad){
		//Broadcast
			inClient->tpt->SendTransaction(myTran_ServerMsg, data);
		}
		
		//Chat
		if (mOptions.nOpts & myOpt_logmsgchat){
			inClient->tpt->SendTransaction(myTran_ChatMsg, data);
		}
		
		if (mOptions.nOpts & myOpt_logmsgpvmsg){
		//PvMsg with reply
		data->AddInteger(myField_UserID, inClient->userID);
		data->AddPString(myField_UserName, inClient->accountLogin);
		inClient->tpt->SendTransaction(myTran_ServerMsg, data);
		}
		if (mOptions.nOpts & myOpt_logmsgpvmsg2){
		//PvMsg only
		data->AddInteger(myField_UserID, inClient->userID);
		data->AddPString(myField_UserName, inClient->accountLogin);
		data->AddInteger(myField_Options, myOpt_AutomaticResponse);
		inClient->tpt->SendTransaction(myTran_ServerMsg, data);
		}
		
					
	}
	

//------------------------------------------------------------------------------------
	// if we have an agreement, send it
	data->DeleteAllFields();

	if (GetBit(info.access.data, myAcc_NoAgreement))
		data->AddInteger(myField_NoServerAgreement, 1);
	else if (mServerAgreement)
	{
		void *hdata;
		StHandleLocker locker(mServerAgreement, hdata);
		data->AddField(myField_Data, hdata, UMemory::GetSize(mServerAgreement));
	}

	inClient->tpt->SendTransaction(myTran_ShowAgreement, data);
	
//------------------This is for compatibility with old clients.-----------------------------
	if (psUserName[0] && nIconID)
	{	
		// log connect to file
		
		LogConnect(inClient);
	
	}		
//------------------------------------------------------------------------------------------
		
	// successful exit
	return;
	
	// error exits
permBanned:				// permanently banned
	failedLoginMsg = "\pYou are permanently banned on this server.";
	
	
	
			
		/*			
	respData->AddInteger(myField_Options, myOpt_RefuseChat);
	respSize = UText::Format(resp, sizeof(resp), "Fuck you! You are permanently banned","Truc");						
	respData->AddField(myField_Data, resp, respSize);
	*/
	#if !FACELESS
	Log(inClient, "Permanently banned");
	#endif	
	SendErrorMsg(inTsn, failedLoginMsg);
//for (i=1; i<=400; i++)
//	{
//	inClient->tpt->SendTransaction(myTran_ServerMsg, respData);
					
//	}
	
	
	goto loginFailed;
tempBanned:				// temporarily banner

Uint32 curSecs = UDateTime::GetSeconds();
Uint8 useractivity[512];
	Uint8 curmin[512];
	Uint8 cursec[512];

//la c les secondes
useractivity[0] = UText::IntegerToText(useractivity+1, sizeof(useractivity)-1, tempbantimeout - curSecs);
	curmin[0] = UText::IntegerToText(curmin+1, sizeof(curmin)-1, ( tempbantimeout - curSecs)/60);		
	cursec[0] = UText::IntegerToText(cursec+1, sizeof(cursec)-1, ( tempbantimeout - curSecs) - (( tempbantimeout - curSecs)/60)*60);		
	

str[0] = UText::Format(str+1, sizeof(str)-1, "You are banned for %#s min %#s sec.",curmin, cursec);
		

	///str[0] = UText::Format(str+1, sizeof(str)-1, "You are banned for %#is.",tempbantimeout-UDateTime::GetSeconds());
		failedLoginMsg = str;	
	
	#if !FACELESS
	Log(inClient, "Temporarily banned");
	#endif
	
	

	//inClient->tpt->SendTransaction(myTran_ServerMsg, respData);		
	
	
	
	
	
	goto loginFailed;
loginIncorrect:			// incorrect login
	// mIncorrectlogin
	failedLoginMsg = mIncorrectlogin;
	
	#if !FACELESS
	Log(inClient, "Incorrect login ");
	#endif
	SendErrorMsg(inTsn, failedLoginMsg);
loginFailed:
	// send message and store the time
	
	SendErrorMsg(inTsn, failedLoginMsg);
	
	inClient->loginFailedSecs = UDateTime::GetSeconds();

#if !FACELESS
	// log message
	Log(inClient, "Login failed");
#endif

	
	return;

#if NETWORK_SERVER
exceedSimConnetions:	// max simultaneous connections
	// send message and store the time
	SendErrorMsg(inTsn, "\pThis server is licensed to handle a limited number of simultaneous connections. Please try later!");
	inClient->loginFailedSecs = UDateTime::GetSeconds();
#endif
#if !FACELESS
	// log message
	Log(inClient, "Exceed max simultaneous connections");
#endif

	

}

void CMyApplication::ProcessTran_Agreed(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	SMyUserDataFile info;
	ClearStruct(info);

	try
	{
		// try to get info for this user
		GetUser(inClient->accountLogin, info);
	}
	catch(...)
	{
		// send message and store the time
		SendErrorMsg(inTsn, "Cannot read account �%#s�.", inClient->accountLogin);
		inClient->loginFailedSecs = UDateTime::GetSeconds();
		
		return;	
	}

	// send okay
	SendNoError(inTsn);
	
	// store user info
	SetClientUserInfo(inClient, inData, &info);
	inClient->hasLoggedIn = true;
	
#if !FACELESS
	// log message
	
	Log(inClient, "Login successful");

#endif
	
	// send user access information
	data->DeleteAllFields();
	data->AddField(myField_UserAccess, inClient->access.data, sizeof(inClient->access.data));
	inClient->tpt->SendTransaction(myTran_UserAccess, data);
	

	// log connect to file
	LogConnect(inClient);
	
	// check client version
	if (inClient->vers < 151 || !mServerBannerType)
		

		return;
		
		
	// send banner info
	data->DeleteAllFields();
	data->AddField(myField_ServerBannerType, &mServerBannerType, sizeof(Uint32));

	if (mServerBannerType == TB('URL '))
	{
		if (!mServerBannerUrl)
			return;
			
		void *pServerBannerUrl;
		StHandleLocker locker(mServerBannerUrl, pServerBannerUrl);
		data->AddField(myField_ServerBannerUrl, pServerBannerUrl, UMemory::GetSize(mServerBannerUrl));
	}
	else
	{
		if (!mServerBanner)
			return;
						
		if (mServerBannerUrl)
		{
			void *pServerBannerUrl;
			StHandleLocker locker(mServerBannerUrl, pServerBannerUrl);
			data->AddField(myField_ServerBannerUrl, pServerBannerUrl, UMemory::GetSize(mServerBannerUrl));
		}
	}
		
	inClient->tpt->SendTransaction(myTran_ServerBanner, data);
}

void CMyApplication::ProcessTran_DownloadFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	SMyDownloadData *dt = nil;
	TFSRefObj* file = nil;
	TFSRefObj* root;
	Uint8 fileName[256];
	Uint8 str[256];
	Uint8 path[2048];
	Uint8 resumeData[512];
	Uint32 xferSize, fileSize;
		
	// check access
	if (!HasFolderPriv(inClient, myAcc_DownloadFile))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to download files.");
		
		
		
		
		return;
	}
	
	

	// extract info
	fileName[0] = inData->GetField(myField_FileName, fileName+1, sizeof(fileName)-1);
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	
	// don't go over max number of downloads
	Uint32 nDownloadsAhead = 0;
	Uint32 nDownloadsCount = mDownloadList.GetItemCount() + mDownloadFldrList.GetItemCount();
	if (nDownloadsCount >= mOptions.nMaxDownloads)
		nDownloadsAhead = nDownloadsCount - mOptions.nMaxDownloads + 1;
	
	// don't go over max number of downloads per client
	Uint32 nClientDownloadsAhead = 0;
	Uint32 nClientDownloadsCount = GetClientDownloadsNumber(inClient->userID);
	if (nClientDownloadsCount >= mOptions.nMaxDownloadsClient)
		nClientDownloadsAhead = nClientDownloadsCount - mOptions.nMaxDownloadsClient + 1;
		
	Uint32 nWaitingCount = nDownloadsAhead > nClientDownloadsAhead ? nDownloadsAhead : nClientDownloadsAhead;
	
	// extract info
	Uint32 opts = inData->GetInteger(myField_FileXferOptions);
	Uint32 resumeDataSize = inData->GetField(myField_FileResumeData, resumeData, sizeof(resumeData));

	try
	{
		// make file reference
		root = GetClientRootFolder(inClient);
		scopekill(TFSRefObj, root);
		file = UFS::New(root, path, pathSize, fileName, fsOption_PreferExistingFile);
		if (file == nil)
		{
			str[0] = UText::Format(str+1, sizeof(str)-1, "Cannot download because there is no file named �%#s�.", fileName);
			SendErrorMsg(inTsn, str);
			return;
		}
		
		// don't allow download if in drop box
			
		if (!HasFolderPriv(inClient, myAcc_ViewDropBoxes))
		{
			file->GetParentName(str);
			UText::MakeLowercase(str+1, str[0]);
			UText::MakeLowercase(path, pathSize);
			
			
			
			
			if (UMemory::Search("drop box", 8, str+1, str[0]) || UMemory::Search("drop box", 8, path, pathSize))
			{
			
			if (HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
				
				if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
				str[0] = UText::Format(str+1, sizeof(str)-1, "Cannot download �%#s� because it is in a drop box.", fileName);
				SendErrorMsg(inTsn, str);
				
				delete file;
				return;
				}
					
			}else{
			
				str[0] = UText::Format(str+1, sizeof(str)-1, "Cannot download �%#s� because it is in a drop box.", fileName);
				SendErrorMsg(inTsn, str);
				
				delete file;
				return;
			}
			}
		}

		fileSize = file->GetSize();		// total file size (data fork + resource fork)
		if (opts == 2 && fileSize == 0)	// if client wants just the raw data and the file has no data
		{
			data->AddInteger(myField_TransferSize, fileSize);
			inTsn->SendData(data);

			delete file;
			return;
		}

		// create new download task
		dt = (SMyDownloadData *)UMemory::NewClear(sizeof(SMyDownloadData));
		dt->file = file;
		
		if (opts == 2)	// if client wants just the raw data (not file package)
		{
			dt->isRaw = true;
			file->Open(perm_Read);
			dt->size = xferSize = file->GetSize();	// data fork size
		}
		else
		{
			if (resumeDataSize == 0)
				file->StartFlatten();
			else
				file->ResumeFlatten(resumeData, resumeDataSize);

			dt->size = xferSize = file->GetFlattenSize();
		}
		
		dt->sendSize = 1024000;
		dt->refNum = NewRefNum() | 1;	// set bit to indicate is download
		dt->startSecs = UDateTime::GetSeconds();
		dt->start = UDateTime::GetSeconds();
		
		dt->client = inClient;
		dt->clientID = inClient->userID;
		dt->name[0] = fileName[0];
		if (fileName[0] > sizeof(dt->name)-1) dt->name[0] = sizeof(dt->name)-1;
		UMemory::Copy(dt->name+1, fileName+1, dt->name[0]);
		
		if (nWaitingCount)
		{
			nWaitingCount += mDownloadWaitingList.GetItemCount();
			
			SMyDownloadWaitingData *dw = (SMyDownloadWaitingData *)UMemory::NewClear(sizeof(SMyDownloadWaitingData));
			dw->bFileFolder = true;
			dw->nWaitingCount = nWaitingCount;
			dw->pDownloadData = dt;
			
			mDownloadWaitingList.AddItem(dw);
		}
		else
			mDownloadList.AddItem(dt);
	}
	catch(...)
	{
		// clean up
		delete file;
		UMemory::Dispose((TPtr)dt);
		throw;
	}
	
	// tell client the download is going to go ahead
	data->AddInteger(myField_TransferSize, xferSize);
	data->AddInteger(myField_FileSize, fileSize);
	data->AddField(myField_RefNum, &dt->refNum, sizeof(Uint32));
	if (nWaitingCount) data->AddInteger(myField_WaitingCount, nWaitingCount);
	inTsn->SendData(data);

	mStats.dlCounter++;
	
#if !FACELESS
	Log(inClient, dt->isRaw ? "View �%#s�" : "Download �%#s�", fileName);
	UpdateDisplay();
#endif
	
	LogDownload(inClient, fileName);

}

void CMyApplication::ProcessTran_DownloadFldr(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	SMyDownloadFldrData *df = nil;
	TFSRefObj* file = nil;
	TFSRefObj* root;
	Uint8 fldrName[256];
	Uint8 str[256];
	Uint8 path[2048];
	Uint8 resumeData[512];
	Uint32 xferSize, itemCount;
	
	// check access
	if (!HasFolderPriv(inClient, myAcc_DownloadFolder))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to download folders!");
		return;
	}

	// extract info
	fldrName[0] = inData->GetField(myField_FileName, fldrName+1, sizeof(fldrName)-1);
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	
	// don't go over max number of downloads
	Uint32 nDownloadsAhead = 0;
	Uint32 nDownloadsCount = mDownloadList.GetItemCount() + mDownloadFldrList.GetItemCount();
	if (nDownloadsCount >= mOptions.nMaxDownloads)
		nDownloadsAhead = nDownloadsCount - mOptions.nMaxDownloads + 1;

	// don't go over max number of downloads per client
	Uint32 nClientDownloadsAhead = 0;
	Uint32 nClientDownloadsCount = GetClientDownloadsNumber(inClient->userID);
	if (nClientDownloadsCount >= mOptions.nMaxDownloadsClient)
		nClientDownloadsAhead = nClientDownloadsCount - mOptions.nMaxDownloadsClient + 1;

	Uint32 nWaitingCount = nDownloadsAhead > nClientDownloadsAhead ? nDownloadsAhead : nClientDownloadsAhead;
	
	// extract info
	Uint32 opts = inData->GetInteger(myField_FileXferOptions);
	Uint32 resumeDataSize = inData->GetField(myField_FileResumeData, resumeData, sizeof(resumeData));
	bool dbAccess;
	
	try
	{
		// make file reference
		root = GetClientRootFolder(inClient);
		scopekill(TFSRefObj, root);
		file = UFS::New(root, path, pathSize, fldrName, fsOption_PreferExistingFolder);
		if (file == nil)
		{
			str[0] = UText::Format(str+1, sizeof(str)-1, "Cannot download because there is no folder named �%#s�.", fldrName);
			SendErrorMsg(inTsn, str);
			return;
		}
		
		// don't allow download if in drop box
		dbAccess = HasFolderPriv(inClient, myAcc_ViewDropBoxes);
		if (!dbAccess)
		{
			UText::MakeLowercase(fldrName+1, fldrName[0]);
			UText::MakeLowercase(path, pathSize);
			if (UMemory::Search("drop box", 8, fldrName+1, fldrName[0]) || UMemory::Search("drop box", 8, path, pathSize))
			{
			
			if (HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
				//!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], fldrName+1, fldrName[0]) || 
				if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
					
				str[0] = UText::Format(str+1, sizeof(str)-1, "Cannot download �%#s� because you are not permitted to access drop boxes.", fldrName);
				SendErrorMsg(inTsn, str);
				
				delete file;
				return;
				}
					
			}else{
			
			
				str[0] = UText::Format(str+1, sizeof(str)-1, "Cannot download �%#s� because you are not permitted to access drop boxes.", fldrName);
				SendErrorMsg(inTsn, str);
				
				delete file;
				return;
			}
			}
		}

		// create new download task
		df = (SMyDownloadFldrData *)UMemory::NewClear(sizeof(SMyDownloadFldrData));
	}
	catch(...)
	{
		if (file)
			delete file;
		
		UMemory::Dispose((TPtr)df);
		throw;
	}
	
	try
	{		
		df->fldr = new CMyDLFldr(file, nil, dbAccess);
		xferSize = df->fldr->GetTotalSize();
		
		// skip over the first item since it's the root folder...eventually I might snag other options from it
		Int16 pathSize = -1;
		Uint8 *p;
		Uint16 count = 0;
		bool outIsFolder = false;
		if (!df->fldr->GetNextItem(p, pathSize, count, &outIsFolder) || !outIsFolder)
		{
			// something's wrong!
			DebugBreak("corrupt folder hierarchy!");
		}
		
		itemCount = df->fldr->GetTotalItems() - 1;
		
		if (itemCount)
		{
			df->totalSize = xferSize;
			df->sendSize = 102400;
			df->refNum = NewRefNum() | 1;	// set bit to indicate is download
			df->startSecs = UDateTime::GetSeconds();
			df->start = UDateTime::GetSeconds();
			df->client = inClient;
			df->clientID = inClient->userID;
			df->name[0] = fldrName[0];
			if (fldrName[0] > sizeof(df->name)-1) df->name[0] = sizeof(df->name)-1;
			UMemory::Copy(df->name+1, fldrName+1, df->name[0]);
			
			if (nWaitingCount)
			{
				nWaitingCount += mDownloadWaitingList.GetItemCount();
				
				SMyDownloadWaitingData *dw = (SMyDownloadWaitingData *)UMemory::NewClear(sizeof(SMyDownloadWaitingData));
				dw->bFileFolder = false;
				dw->nWaitingCount = nWaitingCount;
				dw->pDownloadFldrData = df;
				
				mDownloadWaitingList.AddItem(dw);
			}
			else
				mDownloadFldrList.AddItem(df);
		}
		else	// if there are no items, the client should not bother.
		{
			delete df->fldr;
			UMemory::Dispose((TPtr)df);
			df = nil;
		}
	}
	catch(...)
	{
		// clean up
		if (df->fldr)
			delete df->fldr;
			
		UMemory::Dispose((TPtr)df);
		throw;
	}
	
	if (df)
	{
		// tell client the download is going to go ahead
		data->AddInteger(myField_TransferSize, xferSize);
		data->AddInteger(myField_FldrItemCount, itemCount);
		data->AddField(myField_RefNum, &df->refNum, sizeof(Uint32));
		if (nWaitingCount) data->AddInteger(myField_WaitingCount, nWaitingCount);
		inTsn->SendData(data);
	
		mStats.dlCounter++;
	
	#if !FACELESS
	
		Log(inClient, "Download �%#s�", fldrName);
		UpdateDisplay();
		
	#endif
			
		LogDownload(inClient, fldrName);
	
	}
	else	// folder contianed no items, tell the client this is the case
	{
		data->AddInteger(myField_TransferSize, xferSize);
		data->AddInteger(myField_FldrItemCount, itemCount);
		inTsn->SendData(data);
	}
}

void CMyApplication::ProcessTran_DownloadBanner(SMyClient *inClient, TTransactSession inTsn, TFieldData /*inData*/)
{
	StFieldData data;
	SMyDownloadBannerData *db = nil;

	// create new download banner task
	db = (SMyDownloadBannerData *)UMemory::NewClear(sizeof(SMyDownloadBannerData));
		
	db->refNum = NewRefNum() | 1;	// set bit to indicate is download
	db->startSecs = UDateTime::GetSeconds();
	db->client = inClient;
	db->clientID = inClient->userID;
		
	mDownloadBannerList.AddItem(db);
	
	// tell client the download is going to go ahead
	data->AddInteger(myField_TransferSize, UMemory::GetSize(mServerBanner));
	data->AddField(myField_RefNum, &db->refNum, sizeof(Uint32));
	inTsn->SendData(data);	
	
}

void CMyApplication::ProcessTran_KillDownload(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint32 nRefNum = 0;
	inData->GetField(myField_RefNum, &nRefNum, sizeof(Uint32));

	if (nRefNum)
		KillDownloadByRefNum(inClient, nRefNum);

	SendNoError(inTsn);
}

void CMyApplication::ProcessTran_UploadFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	SMyUploadData *ut = nil;
	TFSRefObj *file = nil;
	TFSRefObj *root, *folder;
	Uint8 fileName[256];
	Uint8 path[2048];
	Uint32 typeCode, creatorCode;
	const Int8 *errorMsg;
	THdl resumeData = nil;

	// check access
	if (!HasFolderPriv(inClient, myAcc_UploadFile))	
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to upload files.");
		return;
	}

	// extract info
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	inData->GetPString(myField_FileName, fileName);
	Uint32 opts = inData->GetInteger(myField_FileXferOptions);
	Uint32 fileSize = inData->GetInteger(myField_TransferSize);
	
	fileName[0] = UFS::ValidateFileName(fileName + 1, fileName[0]);
	
	// disallow uploading of .LNK files (shortcuts) on windoze
#if WIN32
	Uint32 ts = fileName[0];
	if (ts > 3 && UText::toupper(fileName[ts-2]) == 'H' && UText::toupper(fileName[ts-1]) == 'P' && UText::toupper(fileName[ts]) == 'F' && fileName[ts-3] == '.')
		ts -= 4;
		
	if (ts > 3 && UText::toupper(fileName[ts-2]) == 'L' && UText::toupper(fileName[ts-1]) == 'N' && UText::toupper(fileName[ts]) == 'K' && fileName[ts-3] == '.')
	{
		SendErrorMsg(inTsn, "\pCannot upload shortcuts.");
		return;
	}
#endif
	
	// make folder reference
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);
	folder = UFS::New(root, path, pathSize, nil, fsOption_PreferExistingFolder);
	if (folder == nil)
	{
		// folder not found, send error
		SendErrorMsg(inTsn, "Cannot accept upload of the file �%#s� because the specified folder could not be found.", fileName);
		return;
	}
	scopekill(TFSRefObj, folder);
	
	// check if allowed to upload to this folder
	if (!HasFolderPriv(inClient, myAcc_UploadAnywhere))
	{
		Uint8 folderName[256];
		folderName[0] = UFileSys::GetPathTargetName(path, pathSize, folderName + 1, sizeof(folderName) - 1);
		
		if (!UText::SearchInsensitive("upload", 6, folderName + 1, folderName[0]) && !UText::SearchInsensitive("drop box", 8, folderName + 1, folderName[0]))
		{
			SendErrorMsg(inTsn, "Cannot accept upload of the file �%#s� because you are only allowed to upload to the �Uploads� folder.", fileName);
			return;
		}
	}

	// check if the volume is read-only
	if (folder->IsDiskLocked())
	{
		// volume is locked, send error
		SendErrorMsg(inTsn, "Cannot accept upload of the file �%#s� because the destination volume is read-only or locked.", fileName);
		return;
	}
	
	Uint8 validatedName[256];
	UMemory::Copy(validatedName, fileName, fileName[0] + 1);

#if WIN32
	if (validatedName[0] <= 4 || UMemory::Compare(validatedName + validatedName[0] - 3, ".hpf", 4))
		pstrcat(validatedName, "\p.hpf");	// add ".hpf" to validatedName
	else if (fileName[0] > 4)
		fileName[0] -= 4;					// delete ".hpf" from fileName
#endif

	try
	{
		// get and check the file spec
		if (opts == 1)							// if resuming upload
		{
			// check that the file we're going to resume actually exists
			file = UFS::New(folder, nil, validatedName, fsOption_PreferExistingFile);
			if (file == nil)
			{
				// file does not exist, send error
				errorMsg = "Cannot resume upload of the file �%#s� because it does not exist.";
				goto sendError;
			}
			
			// make sure we don't resume a fully xfered file
			file->GetTypeAndCreatorCode(typeCode, creatorCode);
			if (typeCode != TB((Uint32)'HTft') || creatorCode != TB((Uint32)'HTLC'))
			{
				errorMsg = "Cannot resume upload of the file �%#s� because it is already fully transfered.";
				goto sendError;
			}
		}
		else
		{
			// make sure there isn't already a file with that name
			file = UFS::New(folder, nil, fileName, fsOption_NilIfExists);
			if (file == nil)
			{
				errorMsg = "Cannot accept upload because there is already a file named �%#s�. Try choosing a different name.";
				goto sendError;
			}
			
			try
			{
				// check if there's enough disk space
				if ((Uint64)fileSize + 512000 > folder->GetDiskFreeSpace())
				{
				#if !FACELESS
					Log(inClient, "Not enough free disk space for upload");
				#endif	
					
					errorMsg = "Cannot accept upload of the file �%#s� because there is not enough free disk space.";
					goto sendError;
				}
			}
			catch(...)
			{
				// if GetDiskFreeSpace() fails we assume that there is enough free disk space
				// don't throw
			}
			
		#if WIN32
			file->SetRefName(validatedName);
			
			if (file->Exists())
			{
				errorMsg = "Cannot accept upload because there is already a file named �%#s�. Try choosing a different name.";
				goto sendError;
			}
		#endif
		}
		
		// create new upload task
		
		ut = (SMyUploadData *)UMemory::NewClear(sizeof(SMyUploadData));
		ut->totalSize = fileSize;
		ut->file = file;
		if (opts != 1) file->CreateFile('HTft', 'HTLC');

		if (opts == 1)
		{
			resumeData = file->ResumeUnflatten();
			
			{
				void *tempp;
				StHandleLocker lockRD(resumeData, tempp);
				data->AddField(myField_FileResumeData, tempp, UMemory::GetSize(resumeData));
			}
			
			UMemory::Dispose(resumeData);
			resumeData = nil;
		}
		else
			file->StartUnflatten();
		
		ut->refNum = NewRefNum() & ~1;	// clear bit to indicate is upload
		ut->startSecs = UDateTime::GetSeconds();
		ut->start = UDateTime::GetSeconds();
		ut->client = inClient;
		ut->clientID = inClient->userID;
		ut->name[0] = fileName[0];
		if (fileName[0] > sizeof(ut->name)-1) ut->name[0] = sizeof(ut->name)-1;
		UMemory::Copy(ut->name+1, fileName+1, ut->name[0]);

		mUploadList.AddItem(ut);
	}
	catch(SError& err)
	{
		// clean up
		if (file)
		{
			if (opts != 1) try { file->DeleteFile(); } catch(...) {}
			delete file;
		}
		
		UMemory::Dispose(resumeData);
		UMemory::Dispose((TPtr)ut);

		// make error message for client
		if (opts == 1 && err.type == errorType_FileSys && err.id == fsError_FileInUse)
			path[0] = UText::Format(path+1, 255, "Cannot resume upload of the file �%#s� because it is already in use.", fileName);
		else
		{
			Uint8 errorMsgText[256];
			Uint8 errorIDText[32];
			errorMsgText[0] = UError::GetMessage(err, errorMsgText+1, sizeof(errorMsgText)-1);
			errorIDText[0] = UError::GetDetailMessage(err, errorIDText+1, sizeof(errorIDText)-1);
			path[0] = UText::Format(path+1, 255, "Cannot accept upload of �%#s�:\r%#s\r%#s", fileName, errorMsgText, errorIDText);
		}
		
		// send the error message to the client and get outta here 
		SendErrorMsg(inTsn, path);
		return;
	}
	
	// tell client the upload is going to go ahead
	data->AddField(myField_RefNum, &ut->refNum, sizeof(Uint32));
	inTsn->SendData(data);

	mStats.ulCounter++;	
	
#if !FACELESS

	Log(inClient, "Upload �%#s�", fileName);
	UpdateDisplay();

#endif
	
	LogUpload(inClient, fileName);

	return;
	
sendError:
	delete file;
	path[0] = UText::Format(path+1, 255, errorMsg, fileName);
	SendErrorMsg(inTsn, path);
}

void CMyApplication::ProcessTran_UploadFldr(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	SMyUploadFldrData *uf = nil;
	TFSRefObj* fldr = nil;
	TFSRefObj *root, *folder;
	Uint8 fldrName[256];
	Uint8 path[2048];
	const Int8 *errorMsg;

	// check access
	if (!HasFolderPriv(inClient, myAcc_UploadFolder))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to upload folders.");
		return;
	}

	// extract info
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	inData->GetPString(myField_FileName, fldrName);
	fldrName[0] = UFS::ValidateFileName(fldrName + 1, fldrName[0]);
	Uint32 nTotalItems = inData->GetInteger(myField_FldrItemCount);
	Uint32 nTotalSize = inData->GetInteger(myField_TransferSize);
	Uint32 opts = inData->GetInteger(myField_FileXferOptions);
	
	// make folder reference
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);
	
	folder = UFS::New(root, path, pathSize, nil, fsOption_PreferExistingFolder);
	if (folder == nil)
	{
		// folder not found, send error
		SendErrorMsg(inTsn, "Cannot accept upload of the folder �%#s� because the specified folder could not be found.", fldrName);
		return;
	}
	scopekill(TFSRefObj, folder);
	
	// check if allowed to upload to this folder
	if (!HasFolderPriv(inClient, myAcc_UploadAnywhere))
	{
		Uint8 tempstr[256];
		folder->GetName(tempstr);
		UText::MakeLowercase(tempstr+1, tempstr[0]);
		if (!UMemory::Search("upload", 6, tempstr+1, tempstr[0]) && !UMemory::Search("drop box", 8, tempstr+1, tempstr[0]))
		{
			SendErrorMsg(inTsn, "Cannot accept upload of the folder �%#s� because you are only allowed to upload to the �Uploads� folder.", fldrName);
			return;
		}
	}

	// check if the volume is read-only
	if (folder->IsDiskLocked())
	{
		// volume is locked, send error
		SendErrorMsg(inTsn, "Cannot accept upload of the folder �%#s� because the destination volume is read-only or locked.", fldrName);
		return;
	}
	
	try
	{
		if (opts == 1)							// if resuming upload
		{
			// check that the folder we're going to resume actually exists
			fldr = UFS::New(folder, nil, fldrName, fsOption_PreferExistingFolder);
			if (fldr == nil)
			{
				// folder does not exist, send error
				errorMsg = "Cannot resume upload of the folder �%#s� because it does not exist.";
				goto sendError;
			}
			
			bool bIsFolder;
			if (!fldr->Exists(&bIsFolder) || !bIsFolder)
			{
				// our folder is a file, send error
				errorMsg = "Cannot resume upload because �%#s� is a file.";
				goto sendError;
			}
		}
		else
		{
			// make sure there isn't already a file with that name
			fldr = UFS::New(folder, nil, fldrName, fsOption_NilIfExists);
			if (fldr == nil)
			{
				errorMsg = "Cannot accept upload because there is already a folder named �%#s�. Try choosing a different name.";
				goto sendError;
			}
		}
			
		try
		{
			// check if there's enough disk space
			if ((Uint64)nTotalSize + 512000 > folder->GetDiskFreeSpace())
			{
			#if !FACELESS
				Log(inClient, "Not enough free disk space for folder upload");
			#endif
		
				errorMsg = "Cannot accept upload of the folder �%#s� because there is not enough free disk space.";
				goto sendError;
			}
		}
		catch(...)
		{
			// if GetDiskFreeSpace() fails we assume that there is enough free disk space
			// don't throw
		}
					
		if (!fldr->Exists())
			fldr->CreateFolder();

		if (nTotalItems)
		{
			// create new upload folder task
			uf = (SMyUploadFldrData *)UMemory::NewClear(sizeof(SMyUploadFldrData));
			
			uf->totalSize = nTotalSize;
			uf->fldr = fldr;
			uf->refNum = NewRefNum() & ~1;	// clear bit to indicate is upload
			uf->startSecs = UDateTime::GetSeconds();
			uf->start = UDateTime::GetSeconds();
			
			uf->client = inClient;
			uf->clientID = inClient->userID;
			uf->name[0] = fldrName[0];
			if (fldrName[0] > sizeof(uf->name)-1) uf->name[0] = sizeof(uf->name)-1;
			UMemory::Copy(uf->name+1, fldrName+1, uf->name[0]);
			uf->totalItems = nTotalItems;

			mUploadFldrList.AddItem(uf);
		}
	}
	catch(SError& err)
	{
		// clean up
		if (fldr)
		{
			try { fldr->DeleteFolderContents(); } catch(...) {}
			delete fldr;
		}

		UMemory::Dispose((TPtr)uf);

		// make error message for client
		Uint8 errorMsgText[256];
		Uint8 errorIDText[32];
		errorMsgText[0] = UError::GetMessage(err, errorMsgText+1, sizeof(errorMsgText)-1);
		errorIDText[0] = UError::GetDetailMessage(err, errorIDText+1, sizeof(errorIDText)-1);
		path[0] = UText::Format(path+1, 255, "Cannot accept upload of �%#s�:\r%#s\r%#s", fldrName, errorMsgText, errorIDText);
		
		// send the error message to the client and get outta here 
		SendErrorMsg(inTsn, path);
		return;
	}
	
	if (uf)
	{
		// tell client the upload is going to go ahead
		data->AddField(myField_RefNum, &uf->refNum, sizeof(Uint32));
		inTsn->SendData(data);
	}
	else
	{
		// tell client that everything went okay
		SendNoError(inTsn);
		delete fldr;
	}
		
	mStats.ulCounter++;
	
#if !FACELESS

	Log(inClient, "Upload folder �%#s�", fldrName);
	UpdateDisplay();

#endif

	LogUpload(inClient, fldrName);

	return;

sendError:
	delete fldr;
	path[0] = UText::Format(path+1, 255, errorMsg, fldrName);
	SendErrorMsg(inTsn, path);
}

void CMyApplication::ProcessTran_DeleteFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	Uint8 fileName[256];
	TFSRefObj* root;
	Uint16 fsType;
		
	// extract info
	inData->GetPString(myField_FileName, fileName, sizeof(fileName));
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	
	// make item reference
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);
	StFileSysRef fsItem(root, path, pathSize, fileName, fsOption_NilIfNonExistant);
	if (fsItem.IsInvalid())
	{
		SendErrorMsg(inTsn, "Cannot delete �%#s� because it does not exist or cannot be found.", fileName);
		return;
	}
		
	// check access
	fsType = fsItem->GetType();
	if (fsType == fsItemType_Folder || fsType == fsItemType_FolderAlias)
	{
		
		
		if (!HasFolderPriv(inClient, myAcc_DeleteFolder))
		{
		if (!HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
			SendErrorMsg(inTsn, "\pYou are not allowed to delete folders.");
			#if !FACELESS
			Log(inClient, "Try to delete folder");
			#endif
			return;
		}else{
		
		if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		SendErrorMsg(inTsn, "\pYou are not allowed to delete folders.");
		#if !FACELESS
			Log(inClient, "Try to delete folder");
			#endif
			return;	
			}
			
			}
		}
		
		
	}
	else
	{
		if (!HasFolderPriv(inClient, myAcc_DeleteFile))
		{
		if (!HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
			SendErrorMsg(inTsn, "\pYou are not allowed to delete files.");
			#if !FACELESS
			Log(inClient, "Try to delete");
			#endif
			return;
		}else{
			if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		SendErrorMsg(inTsn, "\pYou are not allowed to delete files.");
		#if !FACELESS
			Log(inClient, "Try to delete");
			#endif
			return;	
			}
			
			}
		}
		
		
		
	}

	// delete the file/folder
	try
	{
		fsItem->MoveToTrash();
	}
	catch(SError& err)
	{
		SendErrorMsg(inTsn, err, "Cannot delete �%#s�:", fileName);
		#if !FACELESS
		Log(inClient, "Try to delete");
		#endif
		return;
	}
	
	// tell client that everything went okay
	SendNoError(inTsn);

#if !FACELESS
	// display in log
	
	Log(inClient, "Delete �%#s�", fileName);

#endif
}

void CMyApplication::ProcessTran_NewFolder(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	Uint8 fileName[256];
	TFSRefObj* root;
	Uint16 type;

	// extract info
	inData->GetPString(myField_FileName, fileName, sizeof(fileName));
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));

	// check access
	if (!HasFolderPriv(inClient, myAcc_CreateFolder))
		{
		if (!HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
			SendErrorMsg(inTsn, "\pYou are not allowed to create folders.");
			#if !FACELESS
			Log(inClient, "Try to create folder");
			#endif
			return;
		}else{
			if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		SendErrorMsg(inTsn, "\pYou are not allowed to create folders.");
		#if !FACELESS
			Log(inClient, "Try to create folder");
		#endif
			return;	
			}
			
			}
		}
	
	
	
	// make reference for the new folder, and check if already item with same name
	try
	{		
		root = GetClientRootFolder(inClient);
		scopekill(TFSRefObj, root);
		
		fileName[0] = UFS::ValidateFileName(fileName + 1, fileName[0]);
		StFileSysRef newFolder(root, path, pathSize, fileName, fsOption_NilIfExists + fsOption_NilIfNonExistantParent, &type);
		if (newFolder.IsInvalid())
		{
			if (type == fsItemType_NonExistantParent)
				SendErrorMsg(inTsn, "Cannot create folder �%#s� because the enclosing folder could not be found.", fileName);
			else
				SendErrorMsg(inTsn, "Cannot create folder �%#s� because there is already a file or folder with that name.", fileName);
		
			return;
		}
	
		// create the folder
		newFolder->CreateFolder();
		
	}
	catch (SError& err)
	{
		SendErrorMsg(inTsn, err, "Cannot create folder �%#s�:", fileName);
		#if !FACELESS
		Log(inClient, "Try to create folders");
		#endif
		return;
	}
	
	// tell client that everything went okay
	SendNoError(inTsn);

#if !FACELESS
	// display in log
	
	Log(inClient, "Create folder �%#s�", fileName);

#endif
}

void CMyApplication::ProcessTran_Unknown(SMyClient *inClient, TTransactSession inTsn, TFieldData /* inData */)
{
	SendErrorMsg(inTsn, "\pUnknown transaction type!");

#if !FACELESS
	Log(inClient, "Received unknown transaction type!");
#else
	#pragma unused(inClient)
#endif
}

void CMyApplication::ProcessTran_SendMessage(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
Uint8 Message[128];
inData->GetPString(myField_StandardMessage, Message, sizeof(Message));
	SendErrorMsg(inTsn, "\pHahahaha!");
	#if !FACELESS
	Log(inClient, "message: %s", Message);
	#endif
	
	
	//SendErrorMsg(inTsn, Message);

#if !FACELESS
	Log(inClient, "Standard Message sent!");
#else
	#pragma unused(inClient)
#endif
}

void CMyApplication::ProcessTranDont_Chat(SMyClient *inClient, TTransactSession inTsn, TFieldData /* inData */)
{
	//SendErrorMsg(inTsn, "\pSend a message to the public chat before downloading!");
	SendErrorMsg(inTsn, mMessageBeforedl);
	
	
	inClient->dontChatCount++;
	
	
#if !FACELESS
	Log(inClient, "Try to Download!");
#else
	#pragma unused(inClient)
#endif
}

void CMyApplication::ProcessTranDont_Post(SMyClient *inClient, TTransactSession inTsn, TFieldData /* inData */)
{
	//SendErrorMsg(inTsn, "\pPost a message before downloading!");
	
	/*
	StFieldData data;
	data->DeleteAllFields();
	if (mServerAgreement)
	{
		void *hdata;
		StHandleLocker locker(mServerAgreement, hdata);
		data->AddField(myField_Data, hdata, UMemory::GetSize(mServerAgreement));
	}

	inClient->tpt->SendTransaction(myTran_ShowAgreement, data);
	
	*/
	
	SendErrorMsg(inTsn, mPostBeforedl);
	
	
	inClient->dontChatCount++;
	
	
#if !FACELESS
	Log(inClient, "Try to Download!");
#else
	#pragma unused(inClient)
#endif
}


void CMyApplication::ProcessTran_NewUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	SMyUserDataFile info;
	Uint8 loginName[64];
	Uint8 *p;
	Uint32 s;
	
	// check access
	if (!inClient->HasAccess(myAcc_CreateUser))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to create new accounts.");
		#if !FACELESS
		Log(inClient, "Try to create new accounts!");
		#endif
		return;
	}

	// initialize user data
	ClearStruct(info);
	info.version = TB((Uint16)kMyUserDataFileVersion);
	
	// extract user data
	info.nameSize = TB((Uint16)inData->GetField(myField_UserName, info.nameData, sizeof(info.nameData)));
	info.aliasSize = TB((Uint16)inData->GetField(myField_UserAlias, info.aliasData, sizeof(info.aliasData)));
	info.loginSize = TB((Uint16)inData->GetField(myField_UserLogin, info.loginData, sizeof(info.loginData)));
	info.passwordSize = TB((Uint16)inData->GetField(myField_UserPassword, info.passwordData, sizeof(info.passwordData)));
	inData->GetField(myField_UserAccess, &info.access, sizeof(info.access));

	// disallow creating a new user with more access than yourself
	Uint32 modAcc0 = info.access.data[0];
	Uint32 modAcc1 = info.access.data[1];
	ClearBit(&modAcc0, myAcc_NoAgreement);	// don't care if someone turns on noAgreement but they don't have it on for themselves
	if ( (modAcc0 & ~(inClient->access.data[0])) || (modAcc1 & ~(inClient->access.data[1])) )
	{
		SendErrorMsg(inTsn, "\pCannot create account with more access than yourself.");
		return;
	}

	// unscramble login name (leave password b/c we want it scrambled in the data file)
	s = FB(info.loginSize);
	p = info.loginData;
	while (s--) { *p = ~(*p); p++; }

	// grab a copy of the login
	loginName[0] = FB(info.loginSize);
	UMemory::Copy(loginName+1, info.loginData, loginName[0]);

	// create the user
	try
	{
		NewUser(info);
	}
	catch(SError& err)
	{
		if (err.type == errorType_FileSys && err.id == fsError_ItemAlreadyExists)
			SendErrorMsg(inTsn, "Cannot create account �%#s� because there is already an account with that login.", loginName);
		else
			SendErrorMsg(inTsn, err, "Cannot create account �%#s�:", loginName);
		return;
	}
	
	// tell client that everything went okay
	SendNoError(inTsn);
	
	// display in log
	
	LogAccountChange(inClient, "\pCreate", loginName);


#if !FACELESS

	Log(inClient, "Create account �%#s�", loginName);

#endif
}

void CMyApplication::ProcessTran_DeleteUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 loginName[64];
	Uint8 *p;
	Uint32 s;
	
	// check access
	if (!inClient->HasAccess(myAcc_DeleteUser))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to delete accounts.");
		#if !FACELESS
		Log(inClient, "Try to delete accounts");
		#endif
		return;
	}

	// extract login
	inData->GetPString(myField_UserLogin, loginName, sizeof(loginName));
	s = loginName[0];
	p = loginName+1;
	while (s--) { *p = ~(*p); p++; }
	UText::MakeLowercase(loginName+1, loginName[0]);
	
	// delete the user
	try
	{
		DeleteUser(loginName);
	}
	catch(SError& err)
	{
		if (err.type == errorType_FileSys && (err.id == fsError_NoSuchItem || err.id == fsError_NoSuchFolder))
			SendErrorMsg(inTsn, "No account with login �%#s�.", loginName);
		else
			SendErrorMsg(inTsn, err, "Cannot delete account �%#s�:", loginName);
		return;
	}

	// tell client that everything went okay
	SendNoError(inTsn);

	// disconnect all clients currently logged in with the account we just deleted
	DeleteOnlineAccount(loginName, inClient);

	// display in log
	
	LogAccountChange(inClient, "\pDelete", loginName);


#if !FACELESS

	Log(inClient, "Delete account �%#s�", loginName);

#endif
}

void CMyApplication::ProcessTran_GetUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	SMyUserDataFile info;
	Uint8 loginName[64];
	Uint8 *p;
	Uint32 s;
	
	// check access
	if (!inClient->HasAccess(myAcc_OpenUser))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to get account information.");
		#if !FACELESS
		Log(inClient, "Try to get account information.");
		#endif
		return;
	}
	
	inData->GetPString(myField_UserLogin, loginName, sizeof(loginName));
	
	try
	{
		GetUser(loginName, info);
	}
	catch(SError& err)
	{
		if (err.type == errorType_FileSys && (err.id == fsError_NoSuchItem || err.id == fsError_NoSuchFolder))
			SendErrorMsg(inTsn, "No account with login �%#s�.", loginName);
		else
			SendErrorMsg(inTsn, err, "Cannot read account �%#s�:", loginName);
		return;
	}
	
	// scramble login for transmission over the net
	s = loginName[0];
	p = loginName + 1;
	while (s--) { *p = ~(*p); p++; }
	
	// don't send password
	if (info.passwordSize)
	{
		info.passwordSize = TB((Uint16)1);
		info.passwordData[0] = 'x';
	}
	
	// send the data to the client
	data->AddField(myField_UserName, info.nameData, FB(info.nameSize));
	data->AddField(myField_UserLogin, loginName+1, loginName[0]);
	data->AddField(myField_UserPassword, info.passwordData, FB(info.passwordSize));
	data->AddField(myField_UserAccess, &info.access, sizeof(SMyUserAccess));
	inTsn->SendData(data);
}



void CMyApplication::ProcessTran_SetUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	SMyUserDataFile info;
	Uint8 loginName[64];
	Uint8 *p;
	Uint32 s;
	
	// check access
	if (!inClient->HasAccess(myAcc_ModifyUser))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to modify account information.");
		return;
	}

	// initialize user data
	ClearStruct(info);
	info.version = TB((Uint16)kMyUserDataFileVersion);
	
	// extract user data
	info.nameSize = TB((Uint16)inData->GetField(myField_UserName, info.nameData, sizeof(info.nameData)));
	info.aliasSize = TB((Uint16)inData->GetField(myField_UserAlias, info.aliasData, sizeof(info.aliasData)));
	info.loginSize = TB((Uint16)inData->GetField(myField_UserLogin, info.loginData, sizeof(info.loginData)));
	info.passwordSize = TB((Uint16)inData->GetField(myField_UserPassword, info.passwordData, sizeof(info.passwordData)));
	inData->GetField(myField_UserAccess, &info.access, sizeof(info.access));
	
	// unscramble login name (leave password b/c we want it scrambled in the data file)
	s = FB(info.loginSize);
	p = info.loginData;
	while (s--) { *p = ~(*p); p++; }

	// grab a copy of the login
	loginName[0] = FB(info.loginSize);
	UMemory::Copy(loginName+1, info.loginData, loginName[0]);
	UText::MakeLowercase(loginName+1, loginName[0]);
	
	// use existing password if necessary
	if (FB(info.passwordSize) == 1 && info.passwordData[0] == 0)
	{
		SMyUserDataFile oldInfo;
		try
		{
			GetUser(loginName, oldInfo);
		}
		catch(SError& err)
		{
			if (err.type == errorType_FileSys && (err.id == fsError_NoSuchItem || err.id == fsError_NoSuchFolder))
				SendErrorMsg(inTsn, "No account with login �%#s�.", loginName);
			else
				SendErrorMsg(inTsn, err, "Cannot read account �%#s�:", loginName);
			return;
		}
		
		info.passwordSize = (FB(oldInfo.passwordSize) > 31) ? TB((Uint16)31) : oldInfo.passwordSize;
		UMemory::Copy(info.passwordData, oldInfo.passwordData, FB(info.passwordSize));
	}
	
	// set the user
	try
	{
		SetUser(info);
	}
	catch(SError& err)
	{
		if (err.type == errorType_FileSys && (err.id == fsError_NoSuchItem || err.id == fsError_NoSuchFolder))
			SendErrorMsg(inTsn, "No account with login �%#s�.", loginName);
		else
			SendErrorMsg(inTsn, err, "Cannot modify account �%#s�:", loginName);
		return;
	}
	
	// tell client that everything went okay
	SendNoError(inTsn);
	
	// update all clients currently logged in with the account we just modified
	SetOnlineAccount(loginName, info);
	
	// display in log
	
	LogAccountChange(inClient, "\pModify", loginName);

#if !FACELESS

	Log(inClient, "Modify account �%#s�", loginName);

#endif
}

void CMyApplication::ProcessTran_GetUserList(SMyClient *inClient, TTransactSession inTsn, TFieldData /* inData */)
{
	StFieldData data;
	SMyUserDataFile info;
	Uint8 loginName[256];
	Uint8 *p;
	Uint32 s;
	
	// check access
	if (!inClient->HasAccess(myAcc_OpenUser))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to administer accounts.");
		return;
	}
	
	THdl hList = GetUserFolderList();
	if (!hList)
	{
		SendErrorMsg(inTsn, "\pCannot read account list.");
		return;
	}
	
	Uint32 nOffset = 0, nTypeCode, nFlags;
	try
	{
		while (UFileSys::GetListNext(hList, nOffset, loginName, &nTypeCode, nil, nil, nil, nil, &nFlags))
		{
			if (nTypeCode == TB((Uint32)'fldr') && (nFlags & 1) == 0)	// if folder and visible
			{			
				try
				{
					GetUser(loginName, info);
				}
				catch(...)
				{
					// don't throw
					continue;
				}
				
				// scramble login for transmission over the net
				s = loginName[0];
				p = loginName + 1;
				while (s--) { *p = ~(*p); p++; }
	
				// don't send password
				if (info.passwordSize)
				{
					info.passwordSize = TB((Uint16)1);
					info.passwordData[0] = 'x';
				}
				
				// add user data
				StFieldData stUserData;
				stUserData->AddField(myField_UserName, info.nameData, FB(info.nameSize));
				stUserData->AddField(myField_UserLogin, loginName+1, loginName[0]);
				stUserData->AddField(myField_UserPassword, info.passwordData, FB(info.passwordSize));
				stUserData->AddField(myField_UserAccess, &info.access, sizeof(info.access));
				
				THdl hUserData = stUserData->GetDataHandle();
				if (!hUserData)
					continue;
				
				Uint32 nDataSize = UMemory::GetSize(hUserData);
				Uint8 *pUserData = UMemory::Lock(hUserData);
				data->AddField(myField_Data, pUserData, nDataSize);
				UMemory::Unlock(hUserData);
			}
		}
	}
	catch(...)
	{
		// don't throw
		UMemory::Dispose(hList);	
		SendErrorMsg(inTsn, "\pCannot read account list.");
		return;
	}

	UMemory::Dispose(hList);	
		
	// send the data to the client
	inTsn->SendData(data);
}

void CMyApplication::ProcessTran_SetUserList(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	SMyUserDataFile info;
	Uint8 psSavedLogin[32];
	Uint8 loginName[64];
	Uint8 *p;
	Uint32 s;
	
	// check access
	if (!inClient->HasAccess(myAcc_CreateUser) && !inClient->HasAccess(myAcc_ModifyUser) && !inClient->HasAccess(myAcc_DeleteUser))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to modify account information.");
		return;
	}
	
	Uint16 nFieldCount = inData->GetFieldCount();
	if (!nFieldCount)
	{
		SendErrorMsg(inTsn, "\pError saving list of accounts.");
		return;
	}

	Uint16 nCount = 0;
	bool bSaveError = false;
	Uint8 bufUserData[512];
		
	while (nCount < nFieldCount)
	{
		Uint32 nDataSize = inData->GetFieldByIndex(++nCount, bufUserData, sizeof(bufUserData));
		if (!nDataSize)
		{
			bSaveError = true;
			continue;
		}
		
		THdl hUserData = nil;
		try
		{
			hUserData = UMemory::NewHandle(bufUserData, nDataSize);
		}
		catch(...)
		{
			bSaveError = true;
			continue;
			// don't throw
		}
		
		StFieldData stUserData;
		stUserData->SetDataHandle(hUserData);
	
		// extract saved login
		psSavedLogin[0] = stUserData->GetField(myField_Data, psSavedLogin + 1, sizeof(psSavedLogin) - 1);
		if (psSavedLogin[0])
		{
			// unscramble saved login
			s = psSavedLogin[0];
			p = psSavedLogin + 1;
			while (s--) { *p = ~(*p); p++; }
		}

		// initialize user data
		ClearStruct(info);
		info.version = TB((Uint16)kMyUserDataFileVersion);
				
		// extract user data
		info.nameSize = TB((Uint16)stUserData->GetField(myField_UserName, info.nameData, sizeof(info.nameData)));
		info.aliasSize = TB((Uint16)stUserData->GetField(myField_UserAlias, info.aliasData, sizeof(info.aliasData)));
		info.loginSize = TB((Uint16)stUserData->GetField(myField_UserLogin, info.loginData, sizeof(info.loginData)));
		info.passwordSize = TB((Uint16)stUserData->GetField(myField_UserPassword, info.passwordData, sizeof(info.passwordData)));
		stUserData->GetField(myField_UserAccess, &info.access, sizeof(info.access));

		if (!info.nameSize && !info.loginSize)
		{
			if (psSavedLogin[0])	// no new user
			{
				// check access
				if (!inClient->HasAccess(myAcc_DeleteUser))
				{
					SendErrorMsg(inTsn, "\pYou are not allowed to delete accounts.");
					return;
				}

				// delete user
				try
				{
					DeleteUser(psSavedLogin);

					// disconnect all clients currently logged in with the account we just deleted
					DeleteOnlineAccount(psSavedLogin, inClient);

					// display in log
					
					LogAccountChange(inClient, "\pDelete", psSavedLogin);

				
				#if !FACELESS
				
					Log(inClient, "Delete account �%#s�", psSavedLogin);
			
				#endif
				}
				catch(...)
				{	
					bSaveError = true;
					// don't throw
				}
			}
			else	
				bSaveError = true;		
		}
		else									// modify user
		{
			// unscramble login name (leave password b/c we want it scrambled in the data file)
			s = FB(info.loginSize);
			p = info.loginData;
			while (s--) { *p = ~(*p); p++; }
		
			// grab a copy of the login
			loginName[0] = FB(info.loginSize);
			UMemory::Copy(loginName+1, info.loginData, loginName[0]);
			UText::MakeLowercase(loginName+1, loginName[0]);

			// use existing password if necessary
			if (psSavedLogin[0] && FB(info.passwordSize) == 1 && info.passwordData[0] == 0)
			{
				SMyUserDataFile oldInfo;
				try
				{
					GetUser(psSavedLogin, oldInfo);
				}
				catch(SError& err)
				{
					bSaveError = true;
					continue;
					// don't throw
				}
		
				info.passwordSize = (FB(oldInfo.passwordSize) > 31) ? TB((Uint16)31) : oldInfo.passwordSize;
				UMemory::Copy(info.passwordData, oldInfo.passwordData, FB(info.passwordSize));
			}

			if (psSavedLogin[0] && UText::CompareInsensitive(psSavedLogin + 1, psSavedLogin[0], info.loginData, TB(info.loginSize)))
			{
				// check access
				if (!inClient->HasAccess(myAcc_ModifyUser))
				{
					SendErrorMsg(inTsn, "\pYou are not allowed to modify account information.");
					return;
				}

				// rename user
				try
				{
					RenameUser(psSavedLogin, info);
					
					// display in log
					Uint8 psModifiedLogin[256];
					psModifiedLogin[0] = UText::Format(psModifiedLogin + 1, sizeof(psModifiedLogin) - 1, "�%#s� �%#s�", psSavedLogin, loginName);
					LogAccountChange(inClient, "\pRename", psModifiedLogin);
				
				#if !FACELESS
				
					Log(inClient, "Rename account %#s", psModifiedLogin);
			
				#endif
				}
				catch(...)
				{
					bSaveError = true;
					// don't throw
				}
			}

			if (ExistsUser(info))
			{
				if (psSavedLogin[0])	// no new user
				{
					// check access
					if (!inClient->HasAccess(myAcc_ModifyUser))
					{
						SendErrorMsg(inTsn, "\pYou are not allowed to modify account information.");
						return;
					}

					// set user
					try
					{
						SetUser(info);	
				
						// update all clients currently logged in with the account we just modified
						SetOnlineAccount(psSavedLogin, info);
	
						// display in log
						
						LogAccountChange(inClient, "\pModify", loginName);
					
					
					#if !FACELESS
					
						Log(inClient, "Modify account �%#s�", loginName);
					
					#endif
					}
					catch(...)
					{
						bSaveError = true;
						// don't throw
					}
				}
				else
				{		
					// send error message
					Uint8 psMsg[256];
					psMsg[0] = UText::Format(psMsg + 1, sizeof(psMsg) - 1, "Cannot create account �%#s� because there is already an account with that login.", loginName);
					
					StFieldData data;
					data->AddPString(myField_Data, psMsg);
					data->AddInteger(myField_ChatOptions, 1);
					inClient->tpt->SendTransaction(myTran_ServerMsg, data);
				}
			}
			else
			{
				// check access
				if (!inClient->HasAccess(myAcc_CreateUser))
				{
					SendErrorMsg(inTsn, "\pYou are not allowed to create new accounts.");
					return;
				}

				// disallow creating a new user with more access than yourself
				Uint32 modAcc0 = info.access.data[0];
				Uint32 modAcc1 = info.access.data[1];
				ClearBit(&modAcc0, myAcc_NoAgreement);	// don't care if someone turns on noAgreement but they don't have it on for themselves
				if ((modAcc0 & ~(inClient->access.data[0])) || (modAcc1 & ~(inClient->access.data[1])))
				{
					SendErrorMsg(inTsn, "\pCannot create account with more access than yourself.");
					return;
				}

				// create user
				try
				{
					NewUser(info);
					
					// display in log
					
					LogAccountChange(inClient, "\pCreate", loginName);
				
				#if !FACELESS
				
					Log(inClient, "Create account �%#s�", loginName);
				
				#endif
				}
				catch(...)
				{
					bSaveError = true;
					// don't throw
				}
			}
		}	
	}
		
	if (bSaveError)
	{
		SendErrorMsg(inTsn, "\pError saving list of accounts.");
		return;
	}
	
	// tell client that everything went okay
	SendNoError(inTsn);	
}

void CMyApplication::ProcessTran_DisconnectUser(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	SMyClient *client;
	Uint16 id;
	Uint32 chatID;
	
	// check access
	if (!HasGeneralPriv(inClient, myAcc_DisconUser))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to disconnect users.");
		return;
	}
	
	
	// find and kill client
	id = inData->GetInteger(myField_UserID);
	client = GetClientByID(id);
	if (client)
	{
		// check access
		if (HasGeneralPriv(client, myAcc_CannotBeDiscon))
		{
		
			//SendErrorMsg(inTsn, "\pYou are not allowed to disconnect user �%#s� (user is protected).", client->userName);
			SendErrorMsg(inTsn, "\pYou are not allowed to disconnect this user (user is protected).");
			return;
		}
		
		StFieldData data;
		
		// ban if necessary
		Uint32 nPermTempBan = inData->GetInteger(myField_Options);
		if (nPermTempBan == 1)
		{
			// send message
			data->AddPString(myField_Data, "\pYou are temporarily banned on this server.");
			client->tpt->SendTransaction(myTran_ServerMsg, data);

			AddTempBan(client->ipAddress);
		}
		else if (nPermTempBan == 2)
		{
			// send message
			data->AddPString(myField_Data, "\pYou are permanently banned on this server.");
			client->tpt->SendTransaction(myTran_ServerMsg, data);

			Uint8 bufDescr[64]; 
			if (inData->GetFieldSize(myField_Data))
				bufDescr[0] = inData->GetField(myField_Data, bufDescr + 1, 63);
			else
				bufDescr[0] = 0;
	
			AddPermBan(client->ipAddress,client->ipAddress, bufDescr);
		}
		
		// disconnect the client
		client->tpt->Disconnect();	// don't use StartDisconnect() here because we want to be sure that the Client gets disconnected
		UApplication::PostMessage(msg_DataArrived);
		
		// log disconnect
		Uint8 ip[256];
		Uint8 *p = client->userName;
		if (p[0] == 0)
		{
			ip[0] = client->tpt->GetTransport()->GetRemoteAddressText(ip+1, sizeof(ip)-1);
			p = ip;
		}
Uint8 name[64];
	#if !FACELESS
	
		if (nPermTempBan == 2){
			Log(inClient, "Disconnect & permanent ban �%#s�", p);
			name[0] = UText::Format(name+1, sizeof(name)-1, "\r ��� %#s has been permanently banned by %#s ���",p, inClient->userName);
		}else if (nPermTempBan == 1){
			Log(inClient, "Disconnect & temporary ban �%#s�", p);
			name[0] = UText::Format(name+1, sizeof(name)-1, "\r ��� %#s has been temporary banned by %#s ���",p, inClient->userName);
		}else{ 
			Log(inClient, "Kick �%#s�", p);
			name[0] = UText::Format(name+1, sizeof(name)-1, "\r ��� %#s has been kicked by %#s ���",p, inClient->userName);
		}	
if (mOptions.nOpts & myOpt_showKick){			
	
	Uint32 opts, s;
	THdl h;
	

	opts = inData->GetInteger(myField_ChatOptions);
	
																			
		s = inData->GetFieldSize(myField_Data);
	
	h = UMemory::NewHandle(s);
	try
	{
		void *hdata;
		{
		StHandleLocker lockA(h, hdata);
		inData->GetField(myField_Data, hdata, s);
	
		}
		
		UMemory::Insert(h, 0, name+1, name[0]);
		
		UMemory::SearchAndReplaceAll(h, 1, "\r", 1, name+1, name[0]);
		
		StHandleLocker lockB(h, hdata);
		s = UMemory::GetSize(h);
		data->AddField(myField_Data, hdata, s);
	
	}
	catch(...)
	{
		UMemory::Dispose(h);
		throw;
	}
	
	UMemory::Dispose(h);
	
	/*
	 * Broadcast the chat
	 */
	
	
	SMyClient *client;
	
	
	chatID = inData->GetInteger(myField_ChatID);
	
	
		// broadcast chat to other clients (main chat channel)
		client = (SMyClient *)mClientList.GetFirst();
		while (client)
		{
		
			if (client->tpt->IsEstablished() && HasGeneralPriv(client, myAcc_ReadChat))
				client->tpt->SendTransaction(myTran_ChatMsg, data);
			client = client->next;
		}
	
}

	#endif
	}
	
	
	// send okay reply
	SendNoError(inTsn);
}





void CMyApplication::ProcessTran_GetClientInfoText(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	StFieldData data;
	
	SMyClient *client;
	Uint8 buf[1024];
	Uint8 downFileText[512];
	Uint8 downFolderText[512];
	Uint8 upFileText[512];
	Uint8 upFolderText[512];
	Uint8 downWaitingText[512];
	Uint8 ip[64];
	Uint8 usericon[32];
	Uint8 userid[32];
	Uint8 blockeddownload[32];
	Uint8 visible[32];
	
	bool ready;

	
	Uint8 useractivity[512];
	
	Uint8 curmin[512];
	Uint8 cursec[512];
	Uint8 o[16];
	Uint8 speedo[16];
	Uint32 speed;
	Uint8 percentStr[16];
	Uint8 sizeStr[32];
	Uint32 s, i, n, seconds, minutes, hours, days;
	SMyDownloadData *dt;
	SMyDownloadFldrData *df;
	SMyUploadData *ut;
	SMyUploadFldrData *uf;
	SMyDownloadWaitingData *dw;
	Uint16 clientID;
	Uint8 *p, *ep;
	
	
	// check access
	if (!HasGeneralPriv(inClient, myAcc_GetClientInfo))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to get client information.");
		return;
	}

	clientID = inData->GetInteger(myField_UserID);
	client = GetClientByID(clientID);
	
	if (client == nil)
	{
		SendErrorMsg(inTsn, "\pCannot get info for the specified client because it does not exist.");
		return;
	}
	
		
	
	// get download file info
	p = downFileText;
	ep = p + sizeof(downFileText);
	
	i = 0;
	while (mDownloadList.GetNext(dt, i))
	{
		if (dt->clientID == clientID)
		{
		
		
			s = ep - p;
			if (s < 40) break;
			
			if (!dt->tpt || !dt->size)
			{
				n = 0;
			}
			else
			{
				n = (dt->bytesSent - dt->tpt->GetUnsentSize()) / (dt->size / 100);
				if (n > 100) n = 100;
			}
			
			
			if (dt->bytesSent < 1048576)
	{
		//SendErrorMsg(inTsn, "\pInfos not ready. Please retry later.");
		//return;
		ready = false;
	}else{
		ready = true;	
			//t = dt->bytesSent/1048576;
			
			// o[0] = UText::IntegerToText(o+1, sizeof(o)-1, t);
		o[0] = UText::SizeToText(dt->bytesSent - dt->tpt->GetUnsentSize(), o+1, sizeof(o)-1, kDontShowBytes);

			
     
	
	speed= (dt->bytesSent - dt->tpt->GetUnsentSize()) / (UDateTime::GetSeconds() - dt->start);
		
      speedo[0] = UText::SizeToText(speed, speedo+1, sizeof(speedo)-1, kDontShowBytes);

		
		seconds = ((dt->size - dt->bytesSent) / speed);
      minutes = seconds / 60;
      seconds %= 60;
      hours = minutes / 60;
      minutes %= 60;
      days = hours / 24;
      hours %= 24;
       
		percentStr[0] = UText::IntegerToText(percentStr+1, sizeof(percentStr)-1, n);
		percentStr[0]++;
		percentStr[percentStr[0]] = '%';
		sizeStr[0] = UText::SizeToText(dt->size, sizeStr+1, sizeof(sizeStr)-1, kDontShowBytes);

}
if (mOptions.nOpts & myOpt_BotMode)
	{
p += UText::Format(p, s, "%#-21.21s %#-4.4s %#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", dt->name, percentStr, sizeStr, speedo, days, hours, minutes, seconds);

}else if (ready == false){	
p += UText::Format(p, s, "%#-21.21s will run soon.\r", dt->name);

}else{		
p += UText::Format(p, s, "%#-21.21s \r%#-4.4s %#s/%#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", dt->name, percentStr, o, sizeStr, speedo, days, hours, minutes, seconds);

}		
		}
	}
	/*
	if (p == downFileText)
	{
		*p++ = '';
		
	}
	*/
	*p = 0;

	// get download folder info
	p = downFolderText;
	ep = p + sizeof(downFolderText);

	i = 0;
	while (mDownloadFldrList.GetNext(df, i))
	{
		if (df->clientID == clientID)
		{
			s = ep - p;
			if (s < 40) break;
					
			if (!df->tpt || !df->totalSize)
			{
				n = 0;
			}
			else
			{
				n = (df->totalBytesSent - df->tpt->GetUnsentSize()) / (df->totalSize / 100);
				if (n > 100) n = 100;
			}
			
					 if (df->totalBytesSent < 1048576)
	{
		ready = false;
	}else{
		ready = true;
			
			//t = df->totalBytesSent/1048576;
			// o[0] = UText::IntegerToText(o+1, sizeof(o)-1, t);
			o[0] = UText::SizeToText(df->totalBytesSent, o+1, sizeof(o)-1, kDontShowBytes);

	
			
				speed= df->totalBytesSent / (UDateTime::GetSeconds() - df->start);
			 speedo[0] = UText::SizeToText(speed, speedo+1, sizeof(speedo)-1, kDontShowBytes);

		seconds = ((df->totalSize - df->totalBytesSent) / speed);
      minutes = seconds / 60;
      seconds %= 60;
      hours = minutes / 60;
      minutes %= 60;
      days = hours / 24;
      hours %= 24;
		
		
				
			percentStr[0] = UText::IntegerToText(percentStr+1, sizeof(percentStr)-1, n);
			percentStr[0]++;
			percentStr[percentStr[0]] = '%';
			sizeStr[0] = UText::SizeToText(df->totalSize, sizeStr+1, sizeof(sizeStr)-1, kDontShowBytes);
}			
if (mOptions.nOpts & myOpt_BotMode)
	{
p += UText::Format(p, s, "%#-21.21s %#-4.4s %#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", df->name, percentStr, sizeStr, speedo, days, hours, minutes, seconds);
}else if (ready == false){	
p += UText::Format(p, s, "%#-21.21s will run soon.\r", df->name);

}else{		
p += UText::Format(p, s, "%#-21.21s \r%#-4.4s %#s/%#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", df->name, percentStr, o, sizeStr, speedo, days, hours, minutes, seconds);
}			
		}
	}
/*
	if (p == downFolderText)
	{
		
		*p++ = '';
	}
	*/
	*p = 0;
		
	
	// get upload file info
	p = upFileText;
	ep = p + sizeof(upFileText);
	
	i = 0;
	while (mUploadList.GetNext(ut, i))
	{
		
	
		if (ut->clientID == clientID)
		{
		
			s = ep - p;
			if (s < 40) break;
			
			if (!ut->tpt || !ut->totalSize)
			{
				n = 0;
			}
			else
			{
				n = ut->rcvdSize / (ut->totalSize / 100);
				if (n > 100) n = 100;
			}
	
			
			 if (ut->rcvdSize < 1048576)
	{
		ready = false;
	}else{
		ready = true;
		
				//t = ut->rcvdSize/1048576;
			// o[0] = UText::IntegerToText(o+1, sizeof(o)-1, t);
			 o[0] = UText::SizeToText(ut->rcvdSize, o+1, sizeof(o)-1, kDontShowBytes);

			
			 
			 
			speed = ut->rcvdSize / (UDateTime::GetSeconds() - ut->start);
			
			 speedo[0] = UText::SizeToText(speed, speedo+1, sizeof(speedo)-1, kDontShowBytes);

	  seconds = ((ut->totalSize - ut->rcvdSize) / speed);
      minutes = seconds / 60;
      seconds %= 60;
      hours = minutes / 60;
      minutes %= 60;
      days = hours / 24;
      hours %= 24;
     
      
	  
		
           
             
			percentStr[0] = UText::IntegerToText(percentStr+1, sizeof(percentStr)-1, n);
			percentStr[0]++;
			percentStr[percentStr[0]] = '%';
			sizeStr[0] = UText::SizeToText(ut->totalSize, sizeStr+1, sizeof(sizeStr)-1, kDontShowBytes);
}
if (mOptions.nOpts & myOpt_BotMode)
	{			
p += UText::Format(p, s, "%#-21.21s %#-4.4s %#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", ut->name, percentStr, sizeStr, speedo, days, hours, minutes, seconds);
}else if (ready == false){	
p += UText::Format(p, s, "%#-21.21s will run soon.\r", ut->name);

}else{	
 p += UText::Format(p, s, "%#-21.21s \r%#-4.4s %#s/%#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", ut->name, percentStr, o, sizeStr, speedo, days, hours, minutes, seconds);
}					
		}
	}	
/*
	if (p == upFileText)
	{
		
		*p++ = '';
	}
	*/
	*p = 0;

	// get upload folder info
	p = upFolderText;
	ep = p + sizeof(upFolderText);

	i = 0;
	while (mUploadFldrList.GetNext(uf, i))
	{
		if (uf->clientID == clientID)
		{
			s = ep - p;
			if (s < 40) break;
						
			if (!uf->tpt || !uf->totalSize)
			{
				n = 0;
			}
			else
			{
				n = uf->rcvdSize / (uf->totalSize / 100);
				if (n > 100) n = 100;
			}
			
			if (uf->rcvdSize < 1048576)
	{
		ready = false;
	}else{
		ready = true;
			//t = uf->rcvdSize/1048576;
			 //o[0] = UText::IntegerToText(o+1, sizeof(o)-1, t);	
			o[0] = UText::SizeToText(uf->rcvdSize, o+1, sizeof(o)-1, kDontShowBytes);

		
			speed= uf->rcvdSize / (UDateTime::GetSeconds() - uf->start);
			 speedo[0] = UText::SizeToText(speed, speedo+1, sizeof(speedo)-1, kDontShowBytes);

		seconds = ((uf->totalSize - uf->rcvdSize) / speed);
      minutes = seconds / 60;
      seconds %= 60;
      hours = minutes / 60;
      minutes %= 60;
      days = hours / 24;
      hours %= 24;
		
		
			percentStr[0] = UText::IntegerToText(percentStr+1, sizeof(percentStr)-1, n);
			percentStr[0]++;
			percentStr[percentStr[0]] = '%';
			sizeStr[0] = UText::SizeToText(uf->totalSize, sizeStr+1, sizeof(sizeStr)-1, kDontShowBytes);
}
if (mOptions.nOpts & myOpt_BotMode)
	{		
			p += UText::Format(p, s, "%#-21.21s %#-4.4s %#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", uf->name, percentStr, sizeStr, speedo, days, hours, minutes, seconds);
}else if (ready == false){	
p += UText::Format(p, s, "%#-21.21s will run soon.\r", uf->name);

}else{	
			p += UText::Format(p, s, "%#-21.21s \r%#-4.4s %#s/%#s Speed: %#s/s ETA: %#i:%#i:%#i:%#i\r", uf->name, percentStr, o, sizeStr, speedo, days, hours, minutes, seconds);
}		
		}
	}	
/*
	if (p == upFolderText)
	{
		
		*p++ = '';
	}
	*/
	*p = 0;

	// get waiting downloads info
	p = downWaitingText;
	ep = p + sizeof(downWaitingText);

	i = 0;
	while (mDownloadWaitingList.GetNext(dw, i))
	{
		if (dw->bFileFolder)
		{
			if (dw->pDownloadData->clientID == clientID)
			{
				s = ep - p;
				if (s < 40) break;
					
				p += UText::Format(p, s, "%#-21.21s (File)\r", dw->pDownloadData->name);
			}
		}
		else
		{
			if (dw->pDownloadFldrData->clientID == clientID)
			{
				s = ep - p;
				if (s < 40) break;
					
				p += UText::Format(p, s, "%#-21.21s (Folder)\r", dw->pDownloadFldrData->name);
			}
		}
	}	
/*
	if (p == downWaitingText)
	{
		
		*p++ = '';
	}
	*/
	*p = 0;

	// get address text
	ip[0] = client->tpt->GetTransport()->GetRemoteAddressText(ip+1, sizeof(ip)-1);
	
			
		
	userid[0] = UText::IntegerToText(userid+1, sizeof(userid)-1, client->userID);
	usericon[0] = UText::IntegerToText(usericon+1, sizeof(usericon)-1, client->iconID);
	Uint32 curSecs = UDateTime::GetSeconds();
	useractivity[0] = UText::IntegerToText(useractivity+1, sizeof(useractivity)-1, curSecs - client->activitySecs);
	
	
	
	curmin[0] = UText::IntegerToText(curmin+1, sizeof(curmin)-1, (curSecs - client->activitySecs)/60);		
	cursec[0] = UText::IntegerToText(cursec+1, sizeof(cursec)-1, (curSecs - client->activitySecs) - ((curSecs - client->activitySecs)/60)*60);		
	
	blockeddownload[0] = UText::IntegerToText(blockeddownload+1, sizeof(blockeddownload)-1, client->isBlockDownload);
	
	
	visible[0] = UText::IntegerToText(visible+1, sizeof(visible)-1, client->isVisible);
	
	
	
	// old: //
if (mOptions.nOpts & myOpt_BotMode)
	{
	
//bot
s = UText::Format(buf, sizeof(buf), "Nickname:   %#s\rName:       %#s\rAccount:    %#s\rAddress:    %#s\r\r-------- File Downloads ---------\r\r%s\r------- Folder Downloads --------\r\r%s\r--------- File Uploads ----------\r\r%s\r-------- Folder Uploads ---------\r\r%s\r------- Waiting Downloads -------\r\r%s\rUserId:     %#s\rIcon:       %#s\rAway:       %#s min %#s sec\rClient:     %#i\rDL Blocked: %#s\rInvisible : %#s\rTry to DL:  %#i x\rLogOn:      %#s\rLast Folder loaded: %#s\rLast Article loaded: %#s", client->userName, client->accountName, client->accountLogin, ip, downFileText, downFolderText, upFileText, upFolderText, downWaitingText,userid, usericon, curmin, cursec, client->vers, blockeddownload, visible, client->dontChatCount, client->logon, client->lastPath, client->lastNewsPath);
}else{
// NEW //
   s = UText::Format(buf, sizeof(buf), "Nickname:   %#s\rUserId:     %#s\rIcon:       %#s\rAway:       %#s min %#s sec\rClient:     %#i\rDL Blocked: %#s\rInvisible : %#s\rTry to DL:  %#i x\rName:       %#s\rAccount:    %#s\rAddress:    %#s\rLogOn:      %#s\r\r-------- File Downloads %#ix -----\r%s\r------- Folder Downloads %#ix -----\r%s\r--------- File Uploads %#ix ------\r%s\r-------- Folder Uploads %#ix -----\r%s\r------- Waiting Downloads -------\r%s\rLast Folder loaded: %#s\rLast Article loaded: %#s", client->userName, userid, usericon, curmin, cursec, client->vers, blockeddownload, visible, client->dontChatCount, client->accountName, client->accountLogin, ip, client->logon, client->numdl, downFileText,client->numflddl, downFolderText, client->numul, upFileText,client->numfldul, upFolderText, downWaitingText, client->lastPath, client->lastNewsPath);
}
	//											UserId:     %#s\rIcon:       %#s\rAway:       %#s min %#s sec\rClient:     %#i\rDL Blocked: %#s\rInvisible : %#s\rTry to DL:  %#i x\r			
	// send the text off
	data->AddField(myField_Data, buf, s);
	data->AddPString(myField_UserName, client->userName);
	data->AddPString(myField_UserIconID, usericon);
	
	inTsn->SendData(data);
}

void CMyApplication::ProcessTran_SetClientUserInfo(SMyClient *inClient, TTransactSession /*inTsn*/, TFieldData inData)
{
if (!inClient->access.HasPriv(myAcc_Canflood))	{
	// reset name-change noflood time if a minute has elapsed
	Uint32 curSecs = UDateTime::GetSeconds();
	if (curSecs - inClient->nofloodNameSecs > 60)
	{
		inClient->nofloodNameSecs = curSecs;
		inClient->nofloodNameCount = 0;
	}

	// flood protection - kick and ban the client if they change their name/icon more than 25 times in one minute
	if (!inClient->access.HasPriv(myAcc_Canflood))	{
		if (inClient->nofloodNameCount > 25)
		{
			inClient->tpt->Disconnect();		// muahahah
			AddTempBan(inClient->ipAddress);
	
		#if !FACELESS
			Log(inClient, "Autobanned for flooding");
		#endif
	
			return;
		}
		}
	
	// keep track of how many times this client is changing name for the purposes of flood detection
	inClient->nofloodNameCount++;
	}
	// extract and broadcast info
	SetClientUserInfo(inClient, inData);
}

// when getting info on an alias, the name is from the alias, and everything else is from the original
void CMyApplication::ProcessTran_GetFileInfo(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	TFSRefObj *root, *resolvedItem;
	StFieldData data;
	Uint32 typeID, creatorID;
	SDateTimeStamp createDate, modifyDate;
	Uint8 str[256];
	Uint8 path[2048];
	Uint8 fileName[256];
	Uint32 pathSize;
	Uint16 fsType;
	Uint8 *p;
	
	// extract info
	pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	fileName[0] = inData->GetField(myField_FileName, fileName+1, sizeof(fileName)-1);
	
	// make reference for item
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);
	StFileSysRef fsItem(root, path, pathSize, fileName, fsOption_NilIfNonExistant);	// don't resolve aliases here
	if (fsItem.IsInvalid())
	{
		// send error reply
noSuchFile:
		SendErrorMsg(inTsn, "Cannot get info because there is no file/folder named �%#s�.", fileName);
		return;
	}
	
	// get resolved reference (might be alias)
	resolvedItem = fsItem->Clone();
	scopekill(TFSRefObj, resolvedItem);
	resolvedItem->ResolveAlias(&fsType);
	
	// get info based on type of the file system item
	switch (fsType)
	{
		// doesn't exist
		case fsItemType_NonExistantParent:
		case fsItemType_NonExistant:
		{
			goto noSuchFile;
		}
		break;
		
		// folder or folder alias
		case fsItemType_Folder:
		case fsItemType_FolderAlias:
		{
			data->AddPString(myField_FileTypeString, fsType == fsItemType_Folder ? "\pFolder" : "\pFolder Alias");
			data->AddPString(myField_FileCreatorString, "\pn/a");
			data->AddPString(myField_FileType, "\pfldr");
		}
		break;

		// unattached alias
		case fsItemType_UnattachedAlias:
		{
			data->AddPString(myField_FileTypeString, "\pUnattached Alias");
			data->AddPString(myField_FileCreatorString, "\pn/a");
			data->AddPString(myField_FileType, "\palis");
		}
		break;

		// file or file alias
		case fsItemType_File:
		case fsItemType_FileAlias:
		{
			resolvedItem->GetTypeAndCreatorCode(typeID, creatorID);
			
			data->AddField(myField_FileType, &typeID, sizeof(typeID));
			if (fsType == fsItemType_FileAlias)
			{
				str[0] = 12;
				*(Uint32 *)(str+1) = typeID;
				*(Uint32 *)(str+5) = TB((Uint32)' (al');
				*(Uint32 *)(str+9) = TB((Uint32)'ias)');
				data->AddPString(myField_FileTypeString, str);
			}
			else
			{
				switch (typeID)
				{
					case 'APPL':
						p = "\pApplication Program";
						break;
					case 'SITD':
						p = "\pStuffIt Archive";
						break;
					case 'TEXT':
						p = "\pText File";
						break;
					case 'HTft':
						p = "\pIncomplete File";
						break;
					case 'HTbm':
						p = "\pHotline Bookmark";
						break;
					default:
						p = nil;
						break;
				}
				
				if (p)
					data->AddPString(myField_FileTypeString, p);
				else
					data->AddField(myField_FileTypeString, &typeID, sizeof(typeID));
			}
			
			switch (creatorID)
			{
				case 'HTLC':
				case 'HTLS':
					p = "\pHotline";
					break;
				case 'SIT!':
					p = "\pStuffIt";
					break;
				case 'ttxt':
					p = "\pSimpleText";
					break;
				default:
					p = nil;
					break;
			}
			
			if (p)
				data->AddPString(myField_FileCreatorString, p);
			else
				data->AddField(myField_FileCreatorString, &creatorID, sizeof(creatorID));
			
			data->AddInteger(myField_FileSize, resolvedItem->GetSize());
		}
		break;
	}
	
	// store name of unresolved item
	fsItem->GetName(fileName);
	data->AddPString(myField_FileName, fileName);

	// get generic info from resolved item
	if (fsType != fsItemType_UnattachedAlias)
	{
		resolvedItem->GetDateStamp(&modifyDate, &createDate);
		str[0] = resolvedItem->GetComment(str+1, sizeof(str)-1);
		
#if CONVERT_INTS
		createDate.year = TB(createDate.year);
		createDate.msecs = TB(createDate.msecs);
		createDate.seconds = TB(createDate.seconds);
		modifyDate.year = TB(modifyDate.year);
		modifyDate.msecs = TB(modifyDate.msecs);
		modifyDate.seconds = TB(modifyDate.seconds);
#endif
		
		data->AddField(myField_FileCreateDate, &createDate, sizeof(createDate));
		data->AddField(myField_FileModifyDate, &modifyDate, sizeof(modifyDate));
		data->AddPString(myField_FileComment, str);
	}
	
	// send the info off
	inTsn->SendData(data);
}

// when setting info for an alias, the name of the alias is set, and everything else is set on the original
void CMyApplication::ProcessTran_SetFileInfo(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	
	Uint8 str[256];
	Uint8 fileName[256];
	TFSRefObj *root, *resolvedItem;
	Uint16 fsType;
	bool isFolder;

	// extract info
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	fileName[0] = inData->GetField(myField_FileName, fileName+1, sizeof(fileName)-1);
	
	// check if is a .LNK file (so later on we can disallow changing a non-lnk to a lnk - for security)
#if WIN32
	Uint32 ts = fileName[0];											  
	bool isLinkFile = (ts > 3 && UText::toupper(fileName[ts-2]) == 'L' && UText::toupper(fileName[ts-1]) == 'N' && UText::toupper(fileName[ts]) == 'K' && fileName[ts-3] == '.');
#endif
	
	// make reference for item
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);
	StFileSysRef fsItem(root, path, pathSize, fileName, fsOption_NilIfNonExistant);	// don't resolve aliases here
	if (fsItem.IsInvalid())
	{
		// send error reply
noSuchFile:
		SendErrorMsg(inTsn, "Cannot set info because there is no file/folder named �%#s�.", fileName);
		return;
	}
	
	// get resolved reference (might be alias)
	resolvedItem = fsItem->Clone();
	scopekill(TFSRefObj, resolvedItem);
	resolvedItem->ResolveAlias(&fsType);
	
	switch (fsType)
	{
		case fsItemType_NonExistantParent:
		case fsItemType_NonExistant:
			goto noSuchFile;
			break;

		case fsItemType_Folder:
		case fsItemType_FolderAlias:
			isFolder = true;
			break;
			
		default:
			isFolder = false;
			break;
	}
	
	try
	{
		// set comment if necessary
		if ((fsType != fsItemType_UnattachedAlias) && inData->GetFieldIndex(myField_FileComment))
		{
			// check access
			if (isFolder)
			{
				if (!HasFolderPriv(inClient, myAcc_SetFolderComment)){
					if (!HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
						SendErrorMsg(inTsn, "\pYou are not allowed set comments for folders.");
						#if !FACELESS
						Log(inClient, "Try to set comments for folders");
						#endif
						return;
					}else{
						if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
							SendErrorMsg(inTsn, "\pYou are not allowed set comments for folders.");
							#if !FACELESS
							Log(inClient, "Try to set comments for folders");
							#endif
							return;	
						}
			
					}
				}
				
			}
		
			else
			{
				
				
				if (!HasFolderPriv(inClient, myAcc_SetFileComment)){
		
		if (!HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
			SendErrorMsg(inTsn, "\pYou are not allowed set comments for files.");
			#if !FACELESS
			Log(inClient, "Try to set comments for files");
			#endif
			return;
		}else{
			if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		SendErrorMsg(inTsn, "\pYou are not allowed set comments for files.");
		#if !FACELESS
			Log(inClient, "Try to set comments for files");
		#endif
			return;	
			}
			
			}
			}
				
			}

			try
			{
				// set the comment
				inData->GetPString(myField_FileComment, str);
				if (str[0] == 1 && str[1] == 0) str[0] = 0;
				resolvedItem->SetComment(str+1, str[0]);
			}
			catch (...)
			{
				// don't throw
			}
		}
		
		// perform rename if necessary (rename unresolved item)
		inData->GetPString(myField_FileNewName, str);
		if (str[0] && ((str[0] != fileName[0]) || !UMemory::Equal(fileName+1, str+1, str[0])))
		{
			// check access
			if (isFolder)
			{
				
				
				if (!HasFolderPriv(inClient, myAcc_RenameFolder)){
					if (!HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
						SendErrorMsg(inTsn, "\pYou are not allowed to rename folders.");
						#if !FACELESS
						Log(inClient, "Try to rename folders");
						#endif
						return;
					}else{
						if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
							SendErrorMsg(inTsn, "\pYou are not allowed to rename folders.");
							#if !FACELESS
							Log(inClient, "Try to rename folders");
							#endif
							return;	
						}
			
					}
				}
				
				
			}
			else
			{
				
				
			if (!HasFolderPriv(inClient, myAcc_RenameFile)){
					if (!HasGeneralPriv(inClient, myAcc_ViewOwnDropBox)){
						SendErrorMsg(inTsn, "\pYou are not allowed to rename files.");
						#if !FACELESS
						Log(inClient, "Try to rename files");
						#endif
						return;
					}else{
					if (!UText::SearchInsensitive(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
		//if (!UMemory::Search(inClient->accountLogin+1, inClient->accountLogin[0], path, pathSize)){
							SendErrorMsg(inTsn, "\pYou are not allowed to rename files.!!");
							#if !FACELESS
							Log(inClient, "Try to rename files");
							#endif
							return;	
						}
						
						
			
					}
				}
			}
						
			// don't allow renaming of drop boxes unless the user has 'Can View Drop Boxes'
			if (!HasFolderPriv(inClient, myAcc_ViewDropBoxes))
			{
				UMemory::Copy(path, fileName, fileName[0]+1);
				UText::MakeLowercase(path+1, path[0]);
				if (UMemory::Search("drop box", 8, path+1, path[0]))
				{
					SendErrorMsg(inTsn, "\pYou are not allowed to rename drop boxes.");
					#if !FACELESS
					Log(inClient, "Try to rename drop boxes");
					#endif
					return;
				}
			}
			
			// in case user has wierd-ass chars
			str[0] = UFS::ValidateFileName(str + 1, str[0]);

			// disallow changing a non-lnk to a lnk - for security
		#if WIN32
			ts = str[0];
			if (!isLinkFile && ts > 3 && UText::toupper(str[ts-2]) == 'L' && UText::toupper(str[ts-1]) == 'N' && UText::toupper(str[ts]) == 'K' && str[ts-3] == '.')
			{
				SendErrorMsg(inTsn, "\pYou are not allowed to hack servers.");
					
				return;
			}
		#endif

			// rename file
			fsItem->SetName(str);
		}
	}
	catch(SError& err)
	{
		SendErrorMsg(inTsn, err, "Cannot set info for �%#s�:", fileName);
		return;
	}

	// tell client everything went okay
	SendNoError(inTsn);
}

void CMyApplication::ProcessTran_MoveFile(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	Uint8 fileName[256];
	TFSRefObj* root;
	Uint16 fsType;
	
	// extract info
	inData->GetPString(myField_FileName, fileName, sizeof(fileName));
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	
	// make item reference
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);
	StFileSysRef fsItem(root, path, pathSize, fileName, fsOption_NilIfNonExistant);	// don't resolve aliases here
	if (fsItem.IsInvalid())
	{
		SendErrorMsg(inTsn, "Cannot move because there is no file/folder named �%#s�.", fileName);
		return;
	}

	// check access
	fsType = fsItem->GetType();
	if (fsType == fsItemType_Folder || fsType == fsItemType_FolderAlias)
	{
		if (!HasFolderPriv(inClient, myAcc_MoveFolder))
		{
			SendErrorMsg(inTsn, "\pYou are not allowed to move folders.");
			return;
		}
	}
	else
	{
		if (!HasFolderPriv(inClient, myAcc_MoveFile))
		{
			SendErrorMsg(inTsn, "\pYou are not allowed to move files.");
			return;
		}
	}
	
	// make dest folder reference
	pathSize = inData->GetField(myField_FileNewPath, path, sizeof(path));
	StFileSysRef destFolder(root, path, pathSize, nil, fsOption_PreferExistingFolder);
	if (destFolder.IsInvalid())
	{
		SendErrorMsg(inTsn, "Cannot move �%#s� because the destination folder could not be found.", fileName);
		return;
	}

	// move the file/folder
	try
	{
		fsItem->Move(destFolder);
	}
	catch(SError& err)
	{
		if (err.type == errorType_FileSys)
		{
			switch (err.id)
			{
				case fsError_ItemAlreadyExists:
					SendErrorMsg(inTsn, "Cannot move �%#s� because there is already a file/folder in the destination folder with the same name.", fileName);
					break;
				case fsError_DifferentDisks:
					SendErrorMsg(inTsn, "Cannot move �%#s� between different disks.", fileName);
					break;
				case fsError_BadMove:
					SendErrorMsg(inTsn, "Cannot move �%#s� into itself or into a subfolder of itself.", fileName);
					break;
				default:
					SendErrorMsg(inTsn, err, "Cannot move �%#s�:", fileName);
					break;
			}
		}
		else
			SendErrorMsg(inTsn, err, "Cannot move �%#s�:", fileName);
		return;
	}

	// tell client everything went okay
	SendNoError(inTsn);
	
#if !FACELESS
	// display in log
	
	Log(inClient, "Move �%#s�", fileName);
	
#endif
}

void CMyApplication::ProcessTran_MakeFileAlias(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint8 path[2048];
	Uint8 fileName[256];
	TFSRefObj* root;
	
	// make FS ref for the original file (file we're going to make an alias from)
	inData->GetPString(myField_FileName, fileName, sizeof(fileName));
	Uint32 pathSize = inData->GetField(myField_FilePath, path, sizeof(path));
	
	// check access
	if (!HasFolderPriv(inClient, myAcc_MakeAlias))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to make aliases.");
		return;
	}

	// get the root folder
	root = GetClientRootFolder(inClient);
	scopekill(TFSRefObj, root);

	StFileSysRef originalFile(root, path, pathSize, fileName, fsOption_NilIfNonExistant);
	if (originalFile.IsInvalid())
	{
		SendErrorMsg(inTsn, "Cannot make alias because there is no file/folder named �%#s�.", fileName);
		return;
	}

	// make FS ref for the alias
	pathSize = inData->GetField(myField_FileNewPath, path, sizeof(path));
	StFileSysRef newAlias(root, path, pathSize, fileName, fsOption_NilIfNonExistantParent);
	if (newAlias.IsInvalid())
	{
		SendErrorMsg(inTsn, "Cannot make alias of �%#s� because the destination folder could not be found.", fileName);
		return;
	}

	// move the file/folder
	try
	{
		newAlias->CreateAlias(originalFile);
	}
	catch(SError& err)
	{
		if (err.type == errorType_FileSys)
		{
			switch (err.id)
			{
				case fsError_ItemAlreadyExists:
					SendErrorMsg(inTsn, "Cannot make alias of �%#s� because there is already a file/folder in the destination folder with the same name.", fileName);
					break;
				default:
					SendErrorMsg(inTsn, err, "Cannot make alias of �%#s�:", fileName);
					break;
			}
		}
		else
			SendErrorMsg(inTsn, err, "Cannot make alias of �%#s�:", fileName);
		return;
	}

	// tell client everything went okay
	SendNoError(inTsn);
	
#if !FACELESS
	// display in log
	
	Log(inClient, "Alias �%#s�", fileName);
	
#endif
}

void CMyApplication::ProcessTran_InviteNewChat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	SMyChat *chat;
	SMyClient *client;
	Uint16 i, n;
	
	// reset last msg time if a minute has elapsed
	Uint32 curSecs = UDateTime::GetSeconds();
	if (curSecs - inClient->lastMsgSecs > 60)
	{
		inClient->lastMsgSecs = curSecs;
		inClient->privMsgCount = 0;
	}

	// flood protection
if (!inClient->access.HasPriv(myAcc_Canflood))	{
	if (inClient->privMsgCount > 20)
	{
	if (!HasFolderPriv(inClient, myAcc_Broadcast)){
		inClient->tpt->Disconnect();		// muahahah
		AddTempBan(inClient->ipAddress);
		
	
	#if !FACELESS
		Log(inClient, "Autobanned for flooding");
	#endif
	}
	
		return;
	}
	
	inClient->privMsgCount++;
}
	// check access
	if (!HasGeneralPriv(inClient, myAcc_CreateChat))
	{
		SendErrorMsg(inTsn, "\pYou are not allowed to create a new chat.");
		return;
	}
	
	// create a new chat channel
	chat = (SMyChat *)UMemory::NewClear(sizeof(SMyChat));
	chat->id = NewUniqueChatID();
	mChatList.AddFirst((CLink *)chat);
	chat->clientList.AddItem(inClient);
	
	// prepare data that we'll send to clients to invite them to the chat
	StFieldData data;
	data->AddInteger(myField_ChatID, chat->id);
	data->AddInteger(myField_UserID, inClient->userID);
	data->AddPString(myField_UserName, inClient->userName);

	// send invite notice to all those that are invited
	n = inData->GetFieldCount();
	for (i=1; i<=n; i++)
	{
		if (inData->GetFieldID(i) == myField_UserID)
		{
			client = GetClientByID(inData->GetIntegerByIndex(i));
			
			if (client && client->tpt->IsEstablished() && client != inClient)
			{
				// send automatic response or refuse chat
				if (!inClient->bRefusePrivateMsg && (client->bAutomaticResponse || client->bRefusePrivateChat))
				{
					StFieldData respData;
					respData->AddInteger(myField_UserID, client->userID);
					respData->AddPString(myField_UserName, client->userName);

					Uint8 resp[128];	
					Uint32 respSize = 0;
					
					if (client->bAutomaticResponse)
					{
						respData->AddInteger(myField_Options, myOpt_AutomaticResponse);
						respSize =  UMemory::Copy(resp, client->psAutomaticResponseText + 1, client->psAutomaticResponseText[0]);
					}
					else if (client->bRefusePrivateChat)
					{
						respData->AddInteger(myField_Options, myOpt_RefuseChat);
						respSize = UText::Format(resp, sizeof(resp), "%#s does not accept private chats.", client->userName);
					}	
		
					if (respSize)
					{
						respData->AddField(myField_Data, resp, respSize);
						inClient->tpt->SendTransaction(myTran_ServerMsg, respData);
					}
				}
	
				if (!client->bRefusePrivateChat)
					client->tpt->SendTransaction(myTran_InviteToChat, data);
			}
		}
	}

	// reply to client with the ID of the newly created chat channel
	data->DeleteAllFields();
	data->AddInteger(myField_ChatID, chat->id);
	data->AddInteger(myField_UserID, inClient->userID);
	data->AddInteger(myField_UserIconID, inClient->iconID);
	data->AddInteger(myField_UserFlags, GetClientFlags(inClient));
	data->AddPString(myField_UserName, inClient->userName);
	inTsn->SendData(data);
}

void CMyApplication::ProcessTran_RejectChatInvite(SMyClient *inClient, TTransactSession /* inTsn */, TFieldData inData)
{
	Uint8 str[256];
	str[0] = UText::Format(str+1, sizeof(str)-1, "\r<<< \"%#s\" declined invitation to chat >>>", inClient->userName);
	
	SendTextToChat(inData->GetInteger(myField_ChatID), str+1, str[0]);
}

void CMyApplication::ProcessTran_JoinChat(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	Uint32 chatID = inData->GetInteger(myField_ChatID);
	SMyChat *chatChan = CMyApplication::GetChatByID(chatID);
	
	if (chatChan)
	{
		StFieldData data;

		/*
		 * Tell other clients in the chat that this client joined
		 */
		if (!chatChan->clientList.IsInList(inClient))		// must not add client to list twice
		{
			data->AddInteger(myField_ChatID, chatID);
			data->AddInteger(myField_UserID, inClient->userID);
			data->AddInteger(myField_UserIconID, inClient->iconID);
			data->AddInteger(myField_UserFlags, GetClientFlags(inClient));
			data->AddPString(myField_UserName, inClient->userName);
			
			
			BroadcastToChat(chatChan, myTran_NotifyChatChangeUser, data);
			

			chatChan->clientList.AddItem(inClient);
	
	//}
			
		}
		
		/*
		 * Reply with list of users in the chat
		 */
		Uint8 buf[512];
		SMyUserInfo& ud = *(SMyUserInfo *)buf;
		SMyClient **clientList = chatChan->clientList.GetArrayPtr();
		Uint32 n = chatChan->clientList.GetItemCount();
		Uint32 i;
		SMyClient *client;
		
		data->DeleteAllFields();
		if (chatChan->subject) 
			data->AddPString(myField_ChatSubject, chatChan->subject);
		
		for (i=0; i!=n; i++)
		{
			client = clientList[i];
		
			ud.id = TB((Uint16)client->userID);
			ud.iconID = TB((Uint16)client->iconID);
			ud.flags = TB(GetClientFlags(client));
			
			Uint32 nameSize = client->userName[0];
			UMemory::Copy(ud.nameData, client->userName+1, nameSize);
			ud.nameSize = TB((Uint16)nameSize);
			
			if (ud.nameSize == 0)
			{
				continue;
				
//				nameSize = 3;
//				ud.nameSize = TB((Uint16)3);
//				ud.nameData[0] = ud.nameData[1] = ud.nameData[2] = '?';
			}
			
			data->AddField(myField_UserNameWithInfo, &ud, sizeof(SMyUserInfo) + nameSize);
		}
		
		
		inTsn->SendData(data);
		
	}
	else
		SendErrorMsg(inTsn, "\pCannot join private chat because it has been closed already.");
}

void CMyApplication::ProcessTran_LeaveChat(SMyClient *inClient, TTransactSession /* inTsn */, TFieldData inData)
{
	Uint32 chatID = inData->GetInteger(myField_ChatID);
	SMyChat *chatChan = CMyApplication::GetChatByID(chatID);
	
	if (chatChan && chatChan->clientList.RemoveItem(inClient))
	{
		if (chatChan->clientList.GetItemCount() == 0)
		{
			// no clients in this chat, so kill it
			mChatList.RemoveLink((CLink *)chatChan);
			UMemory::Dispose((TPtr)chatChan->subject);
			delete chatChan;
		}
		else
		{
			// tell remaining clients
			StFieldData data;
			data->AddInteger(myField_ChatID, chatID);
			data->AddInteger(myField_UserID, inClient->userID);
			BroadcastToChat(chatChan, myTran_NotifyChatDeleteUser, data);
		}
	}
}

void CMyApplication::ProcessTran_SetChatSubject(SMyClient */* inClient */, TTransactSession /* inTsn */, TFieldData inData)
{
	Uint32 chatID = inData->GetInteger(myField_ChatID);
	SMyChat *chatChan = CMyApplication::GetChatByID(chatID);

	if (chatChan)
	{
		Uint8 str[256];
		
		// extract the new subject
		inData->GetPString(myField_ChatSubject, str, sizeof(str));
		
		// store the new subject
		if (str[0] == 0)
		{
			UMemory::Dispose((TPtr)chatChan->subject);
			chatChan->subject = nil;
		}
		else if (chatChan->subject == nil)
		{
			chatChan->subject = (Uint8 *)UMemory::New(str, str[0]+1);
		}
		else
		{
			if (str[0] != chatChan->subject[0])
				chatChan->subject = (Uint8 *)UMemory::Reallocate((TPtr)chatChan->subject, str[0]+1);
			
			UMemory::Copy(chatChan->subject, str, str[0]+1);
		}
		
		// inform clients of the change in subject
		StFieldData data;
		data->AddInteger(myField_ChatID, chatID);
		data->AddPString(myField_ChatSubject, str);
		BroadcastToChat(chatChan, myTran_NotifyChatSubject, data);
	}
}

void CMyApplication::ProcessTran_InviteToChat(SMyClient *inClient, TTransactSession /* inTsn */, TFieldData inData)
{
	StFieldData data;
	SMyClient *client;
	SMyChat *chat;
	Uint32 chatID, i, n;
	client =0;
	
	// reset last msg time if a minute has elapsed
	Uint32 curSecs = UDateTime::GetSeconds();
	if (curSecs - inClient->lastMsgSecs > 60)
	{
		inClient->lastMsgSecs = curSecs;
		inClient->privMsgCount = 0;
	}
if (!inClient->access.HasPriv(myAcc_Canflood))	{
	// flood protection
	if (inClient->privMsgCount > 20)
	{
	if (!HasFolderPriv(client, myAcc_Broadcast)){
		inClient->tpt->Disconnect();		// muahahah
		AddTempBan(inClient->ipAddress);
		
	
	#if !FACELESS
		Log(inClient, "Autobanned for flooding");
	#endif
	}
		return;
	}
	
	inClient->privMsgCount++;
}	
	// find chat channel to invite people to
	chatID = inData->GetInteger(myField_ChatID);
	chat = GetChatByID(chatID);
	if (chat == nil) return;
	
	// prepare data that we'll send to clients to invite them to the chat
	data->AddInteger(myField_ChatID, chatID);
	data->AddInteger(myField_UserID, inClient->userID);
	data->AddPString(myField_UserName, inClient->userName);

	// send invite notice to all those that are invited
	n = inData->GetFieldCount();
	for (i=1; i<=n; i++)
	{
		if (inData->GetFieldID(i) == myField_UserID)
		{
			client = GetClientByID(inData->GetIntegerByIndex(i));
			
			if (client && client->tpt->IsEstablished() && client != inClient && !chat->clientList.IsInList(client))
			{
				// send automatic response or refuse chat
				if (!inClient->bRefusePrivateMsg && (client->bAutomaticResponse || client->bRefusePrivateChat))
				{
					StFieldData respData;
					respData->AddInteger(myField_UserID, client->userID);
					respData->AddPString(myField_UserName, client->userName);

					Uint8 resp[128];
					Uint32 respSize = 0;
					
					if (client->bAutomaticResponse)
					{
						respData->AddInteger(myField_Options, myOpt_AutomaticResponse);
						respSize =  UMemory::Copy(resp, client->psAutomaticResponseText + 1, client->psAutomaticResponseText[0]);
					}
					else if (client->bRefusePrivateChat)
					{
						respData->AddInteger(myField_Options, myOpt_RefuseChat);
						respSize = UText::Format(resp, sizeof(resp), "%#s does not accept private chats.", client->userName);
					}	
		
					if (respSize)
					{
						respData->AddField(myField_Data, resp, respSize);
						inClient->tpt->SendTransaction(myTran_ServerMsg, respData);
						
					}
				}
	
				if (!client->bRefusePrivateChat)
					client->tpt->SendTransaction(myTran_InviteToChat, data);
			}
		}
	}
}

void CMyApplication::ProcessTran_KeepConnectionAlive(SMyClient *inClient, TTransactSession inTsn, TFieldData inData)
{
	#pragma unused(inClient, inData)
	
	SendNoError(inTsn);
}

