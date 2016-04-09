// =====================================================================//  CModuleWin.h                         (C) Hotline Communications 1999// =====================================================================//	provide global WIN32 specific module instance handle#ifndef _H_CModuleWin_#define _H_CModuleWin_#if PRAGMA_ONCE	#pragma once#endif		 #ifndef TARGET_OS_WIN32	#pragma message ("This file can be compiled only for Win32 targets")	#pragma !!!#endifHL_Begin_Namespace_BigRedHclass CModuleWin{	public:			// ** constructors **									CModuleWin(HINSTANCE inHinst)										: mHinst(inHinst)										{ sInstance = this; }									~CModuleWin()								   		{ sInstance = nil; }		static const CModuleWin&	Instance()										{ 										  ASSERT(sInstance!=nil); 										  return *sInstance; 										}			// ** operations **		operator 					HINSTANCE() const 										{ return mHinst; }	private:		HINSTANCE 			mHinst;			// ** who's allowed to change this hInstance **		friend int WINAPI 			WinMain( HINSTANCE hInstance, 												 HINSTANCE hPreInst, 											 LPSTR lpszCmdLine, 											 int nCmdShow );		friend BOOL WINAPI 			DllMain( HINSTANCE inHinstDLL,                     						 DWORD inReason,					                	     LPVOID inReserved );			// ** singleton **		static const CModuleWin*	sInstance;};HL_End_Namespace_BigRedH#endif //_H_CModuleWin_