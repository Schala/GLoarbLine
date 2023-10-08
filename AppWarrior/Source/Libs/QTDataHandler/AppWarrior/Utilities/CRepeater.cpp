/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CRepeater.h"

using namespace std;

HL_Begin_Namespace_BigRedH

list<CRepeater*>	CRepeater::sRepeaters;


// ---------------------------------------------------------------------
//  CRepeater                                                   [public]		
// ---------------------------------------------------------------------
// Constructor

CRepeater::CRepeater()
{
}


// ---------------------------------------------------------------------
//  ~CRepeater                                                  [public]		
// ---------------------------------------------------------------------
// Destructor

CRepeater::~CRepeater()
{
	StopRepeating();
}


#pragma mark -
// ---------------------------------------------------------------------
//  StartRepeating                                              [public]
// ---------------------------------------------------------------------
// Put Repeater in the queue

void
CRepeater::StartRepeating()
{
	if( find( sRepeaters.begin(), sRepeaters.end(), this )
				== sRepeaters.end() ){
		sRepeaters.push_back( this );
	}
}


// ---------------------------------------------------------------------
//  StopRepeating                                               [public]
// ---------------------------------------------------------------------
// Remove Repeater from the queue

void
CRepeater::StopRepeating()
{
	sRepeaters.remove( this );
}


// ---------------------------------------------------------------------
//  IsRepeating                                                 [public]
// ---------------------------------------------------------------------
// Return whether a Repeater is in the queue

bool
CRepeater::IsRepeating() const
{
	return ( find( sRepeaters.begin(), sRepeaters.end(), this )
				!= sRepeaters.end() );
}


// ---------------------------------------------------------------------
//  DevoteTimeToRepeaters                               [public][static]
// ---------------------------------------------------------------------
// Call SpendTime function for each Repeater in the queue

void
CRepeater::DevoteTimeToRepeaters()
{
	list<CRepeater*>::iterator it;
	for( it = sRepeaters.begin(); it != sRepeaters.end(); it++ ){
		(*it)->SpendTime();
	}
}

HL_End_Namespace_BigRedH
