// =====================================================================
//	CRepeater.h                          (C) Hotline Communications 1999
// =====================================================================
//
//	An abstract building block class for objects that want to receive a
//	function call at regular intervals

#ifndef _H_CRepeater_
#define _H_CRepeater_

#include "AW.h"

#if PRAGMA_ONCE
	#pragma once
#endif

HL_Begin_Namespace_BigRedH

class	CRepeater 
{
public:
						CRepeater();
	virtual				~CRepeater();
	
	virtual	void		StartRepeating();
	virtual	void		StopRepeating();
	bool				IsRepeating() const;
	static	void		DevoteTimeToRepeaters();
	
						// Pure Virtual. Concrete subclasses must override
	virtual	void		SpendTime() = 0;
	
protected:
	static	std::list<CRepeater*>	sRepeaters;
};

HL_End_Namespace_BigRedH

#endif	// _H_CRepeater_
