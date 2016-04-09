/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */// the way a CMemoryStreamBuffer is used#ifndef _H_CFileStream_#define _H_CFileStream_#include "CIOStream.h"#include "CFileStreamBuffer.h"#if PRAGMA_ONCE	#pragma once#endif			   HL_Begin_Namespace_BigRedHclass CFileInStream : public CInStream{	public:							CFileInStream()								// throws nothing								: CInStream(mBuf)								{}							CFileInStream(const CFSFileRef& inName)								// throws CStreamException								: CInStream(mBuf)								{ Open(inName); }		void 				Open(const CFSFileRef& inName)								// throws CStreamException								{ mBuf.Open(inName,CFileStreamBuffer::eReadOnly); }	private:		CFileStreamBuffer 	mBuf;};class CFileOutStream : public COutStream{	public:							CFileOutStream()								// throws nothing								: COutStream(mBuf)								{}							CFileOutStream(const CFSFileRef& inName)								// throws CStreamException								: COutStream(mBuf)								{ Open(inName); }		void 				Open(const CFSFileRef&	inName)								// throws CStreamException								{ mBuf.Open(inName,CFileStreamBuffer::eWrite); }	private:		CFileStreamBuffer 	mBuf;};class CFileIOStream : public CIOStream{	public:							CFileIOStream()								// throws nothing								: CIOStream(mBuf)								{}							CFileIOStream(const CFSFileRef& inName)								// throws CStreamException								: CIOStream(mBuf)								{ Open(inName); }		void 				Open(const CFSFileRef& inName)								// throws CStreamException								{ mBuf.Open(inName,CFileStreamBuffer::eWrite); }	private:		CFileStreamBuffer 	mBuf;};HL_End_Namespace_BigRedH#endif // _H_CFileStream_