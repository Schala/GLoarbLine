//======================================================================
// CDateTime.h                           (C) Hotline Communications 1999
//======================================================================
//
#include <time.h>
#include "CDateTime.h"

HL_Begin_Namespace_BigRedH

const UInt16 kSecondsInMinute = 60;
const UInt16 kSecondsInHour   = 3600;
const UInt32 kSecondsInDay    = 86400;

// ---------------------------------------------------------------------
//  CDateTime                                                   [public]
// ---------------------------------------------------------------------
// Constructor

CDateTime::CDateTime()
         : mValue(CDateTime::GetCurrentDateTime().GetDateTime())
{
}

// ---------------------------------------------------------------------
//  CDateTime                                                   [public]
// ---------------------------------------------------------------------
// Constructor

CDateTime::CDateTime(UInt16 inYear,
					 UInt16 inMonth,
					 UInt16 inDay,
					 UInt16 inHour,
					 UInt16 inMinute,
					 UInt16 inSecond)
{
	SetupTime(inYear, inMonth, inDay, inHour, inMinute, inSecond);
}

// ---------------------------------------------------------------------
//  CDateTime                                                   [public]
// ---------------------------------------------------------------------
// Constructor

CDateTime::CDateTime(SInt64 inValue) : mValue(inValue)
{
}

// ---------------------------------------------------------------------
//  CDateTime                                                   [public]
// ---------------------------------------------------------------------
// Constructor

CDateTime::CDateTime(const CDateTime& inOther)
{
	if(this != &inOther)
	{
		mValue = inOther.mValue;
	}
}

// ---------------------------------------------------------------------
//  ~CDateTime                                                  [public]
// ---------------------------------------------------------------------
// Destructor

CDateTime::~CDateTime()
{
	mValue = 0; // doesn't matter ! ...
}

// ---------------------------------------------------------------------
//  GetDateTime                                                 [public]
// ---------------------------------------------------------------------

SInt64 CDateTime::GetDateTime() const
{
	return mValue;
}

// ---------------------------------------------------------------------
//  IsDateValid                                                 [public]
// ---------------------------------------------------------------------

bool
CDateTime::IsDateValid(UInt16 inYear,
					   UInt16 inMonth,
					   UInt16 inDay)
{
	UInt16  year;
	UInt16 month;
	UInt16   day;

	NumberToDate(DateToNumber(inYear, inMonth, inDay), year, month, day);
	return (inYear == year && inMonth == month && inDay == day);
}

// ---------------------------------------------------------------------
//  IsBetween                                                   [public]
// ---------------------------------------------------------------------

bool
CDateTime::IsBetween(const CDateTime& inTime1,
					 const CDateTime& inTime2) const
{
    return mValue >= inTime1.mValue && mValue <= inTime2.mValue;
}


// ---------------------------------------------------------------------
//  GetCurrentDateTime                                          [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::GetCurrentDateTime()
{
	tm*    today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);
	return CDateTime(static_cast<UInt16>(1900+today->tm_year),
	        static_cast<UInt16>(1+today->tm_mon), 
	        static_cast<UInt16>(today->tm_mday),
	        static_cast<UInt16>(today->tm_hour),
	        static_cast<UInt16>(today->tm_min),
	        static_cast<UInt16>(today->tm_sec%60));
}

// ---------------------------------------------------------------------
//  GetCurrentDateTime                                          [public]
// ---------------------------------------------------------------------

void
CDateTime::GetCurrentDateTime(UInt16& outYear,
							  UInt16& outMonth,
							  UInt16& outDay,
							  UInt16& outHour,
							  UInt16& outMinute,
							  UInt16& outSecond)
{
	tm*    today;
	time_t ltime;

	time(&ltime);

	today = localtime(&ltime);

	outYear   = static_cast<UInt16>(1900+today->tm_year);
	outMonth  = static_cast<UInt16>(1+today->tm_mon);
	outDay    = static_cast<UInt16>(today->tm_mday);
	outHour   = static_cast<UInt16>(today->tm_hour);
	outMinute = static_cast<UInt16>(today->tm_min);
	outSecond = static_cast<UInt16>(today->tm_sec%60);
}

// ---------------------------------------------------------------------
//  SetupTime                                                   [public]
// ---------------------------------------------------------------------

void
CDateTime::SetupTime(UInt16 inYear,
					 UInt16 inMonth,
					 UInt16 inDay,
					 UInt16 inHour,
					 UInt16 inMinute,
					 UInt16 inSecond)
{
	if(IsDateValid(inYear, inMonth, inDay))
	{
		mValue  = DateToNumber(inYear, inMonth, inDay);
		mValue *= kSecondsInDay;
		mValue += inHour*kSecondsInHour+inMinute*kSecondsInMinute+inSecond;
	}
	else
	{
		ASSERT(0);
	}
}

// ---------------------------------------------------------------------
//  SetupRealTime                                               [public]
// ---------------------------------------------------------------------

void
CDateTime::SetupRealTime(UInt16 inHour,
						 UInt16 inMinute,
						 UInt16 inSecond)
{
	UInt16 year;
	UInt16 month;
	UInt16 day;
	UInt16 hour;
	UInt16 min;
	UInt16 sec;

	GetCurrentDateTime(year, month, day, hour, min, sec);
	SetupTime(year, month, day, inHour, inMinute, inSecond);
}

// ---------------------------------------------------------------------
//  GetYear                                                     [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetYear() 
{
	UInt16  year;
	UInt16 month;
	UInt16   day;

	NumberToDate(static_cast<SInt32>(mValue/kSecondsInDay), year, month, day);
	return year;
}

// ---------------------------------------------------------------------
//  IsYearLeap                                                  [public]
// ---------------------------------------------------------------------

bool
CDateTime::IsYearLeap(UInt16 inYear)
{
	if(inYear%4 != 0)
	{
		return false;
	}

	if(inYear < 1582) // ! ...
	{
		return true;
	}

	if(inYear%100 != 0)
	{
		return true;
	}

	if(inYear%400 != 0)
	{
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------
//  GetMonth                                                    [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetMonth()
{
	UInt16  year;
	UInt16 month;
	UInt16   day;

	NumberToDate(static_cast<SInt32>(mValue/kSecondsInDay), year, month, day);
	return month;
}

// ---------------------------------------------------------------------
//  GetDay                                                      [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetDay()
{
	UInt16  year;
	UInt16 month;
	UInt16   day;

	NumberToDate(static_cast<SInt32>(mValue/kSecondsInDay), year, month, day);
	return day;
}

// ---------------------------------------------------------------------
//  GetDayOfWeek                                                [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetDayOfWeek()
{
	return static_cast<UInt16>(abs(((
		   static_cast<SInt32>(mValue/kSecondsInDay)+2)%7)+1));
}

// ---------------------------------------------------------------------
//  GetDayOfWeek                                                [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetDayOfWeek(UInt16 inYear, UInt16 inMonth, UInt16 inDay)
{
	SInt32 date = abs(DateToNumber(inYear, inMonth, inDay));
	return static_cast<UInt16>(((date+2)%7)+1);
}

// ---------------------------------------------------------------------
//  AddDays                                                     [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::AddDays(UInt16 inDay)
{
	return CDateTime(mValue+inDay*kSecondsInDay);
}

// ---------------------------------------------------------------------
//  SubtractDays                                                [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::SubtractDays(UInt16 inDay)
{
	return CDateTime(mValue-inDay*kSecondsInDay);
}

// ---------------------------------------------------------------------
//  GetHours                                                    [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetHours()
{
	return static_cast<UInt16>(abs(static_cast<SInt32>(mValue%kSecondsInDay)/kSecondsInHour));
}

// ---------------------------------------------------------------------
//  AddHours                                                    [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::AddHours(UInt16 inHour)
{
	return CDateTime(mValue+inHour*kSecondsInHour);
}

// ---------------------------------------------------------------------
//  SubtractHours                                               [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::SubtractHours(UInt16 inHour)
{
	return CDateTime(mValue-inHour*kSecondsInHour);
}

// ---------------------------------------------------------------------
//  GetMinutes                                                  [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetMinutes()
{
	return static_cast<UInt16>(abs((
		   static_cast<SInt32>(mValue%kSecondsInDay)%kSecondsInHour)/
		   kSecondsInMinute));
}

// ---------------------------------------------------------------------
//  SubtractMinutes                                             [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::AddMinutes(UInt16 inMinute)
{
	return CDateTime(mValue+inMinute*kSecondsInMinute);
}

// ---------------------------------------------------------------------
//  AddMinutes                                                  [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::SubtractMinutes(UInt16 inMinute)
{
	return CDateTime(mValue-inMinute*kSecondsInMinute);
}

// ---------------------------------------------------------------------
//  GetSeconds                                                  [public]
// ---------------------------------------------------------------------

UInt16
CDateTime::GetSeconds()
{
	return static_cast<UInt16>(abs((
		   static_cast<SInt32>(mValue%kSecondsInDay)%kSecondsInHour)%
		   kSecondsInMinute));
}

// ---------------------------------------------------------------------
//  AddSeconds                                                  [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::AddSeconds(UInt16 inSecond)
{
	return CDateTime(mValue+inSecond);
}

// ---------------------------------------------------------------------
//  SubtractSeconds                                             [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::SubtractSeconds(UInt16 inSecond)
{
	return CDateTime(mValue-inSecond);
}

// ---------------------------------------------------------------------
//  operator =                                                  [public]
// ---------------------------------------------------------------------

const CDateTime&
CDateTime::operator = (const CDateTime& inOther)
{
	if(this != &inOther)
	{
		mValue = inOther.mValue;
	}
	return *this;
}

// ---------------------------------------------------------------------
//  operator +                                                  [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::operator + (const CDateTime& inOther)
{
	return CDateTime(mValue+inOther.mValue);
}

// ---------------------------------------------------------------------
//  operator -                                                  [public]
// ---------------------------------------------------------------------

CDateTime
CDateTime::operator - (const CDateTime& inOther)
{
	return CDateTime(mValue-inOther.mValue);
}

// ---------------------------------------------------------------------
//  operator <                                                  [public]
// ---------------------------------------------------------------------

bool
CDateTime::operator < (const CDateTime& inOther) const
{
	return mValue < inOther.mValue;
}

// ---------------------------------------------------------------------
//  operator <=                                                 [public]
// ---------------------------------------------------------------------

bool
CDateTime::operator <= (const CDateTime& inOther) const
{
	return mValue <= inOther.mValue;
}

// ---------------------------------------------------------------------
//  operator >                                                  [public]
// ---------------------------------------------------------------------

bool
CDateTime::operator > (const CDateTime& inOther) const
{
	return mValue > inOther.mValue;
}

// ---------------------------------------------------------------------
//  operator >=                                                 [public]
// ---------------------------------------------------------------------

bool
CDateTime::operator >= (const CDateTime& inOther) const
{
	return mValue >= inOther.mValue;
}

// ---------------------------------------------------------------------
//  operator ==                                                 [public]
// ---------------------------------------------------------------------

bool
CDateTime::operator == (const CDateTime& inOther) const
{
	return mValue == inOther.mValue;
}

// ---------------------------------------------------------------------
//  operator !=                                                 [public]
// ---------------------------------------------------------------------

bool
CDateTime::operator != (const CDateTime& inOther) const
{
	return mValue != inOther.mValue;
}

// ---------------------------------------------------------------------
//  DateToNumber                                                [public]
// ---------------------------------------------------------------------

SInt32
CDateTime::DateToNumber(UInt16 inYear,
						UInt16 inMonth,
						UInt16 inDay)
{
	SInt32 a, b;

	if(inMonth <= 2)
	{
		--inYear;
		inMonth += 12;
	}

	b = 0;

	if(inYear*10000.0+inMonth*100.0+inDay >= 15821015.0)
	{
		a = inYear/100;
		b = 2-a+a/4;
	}

	return static_cast<SInt32>(365.25*inYear-(inYear > 0 ? 0.0 : 0.75)+
		   static_cast<SInt32>(30.6001*(inMonth+1))+inDay+1720994L+b);
}

// ---------------------------------------------------------------------
//  NumberToDate                                                [public]
// ---------------------------------------------------------------------

void
CDateTime::NumberToDate(SInt32 inValue,
						UInt16& outYear,
						UInt16& outMonth,
						UInt16& outDay)
{
	// some magic stuff, see "Computer LANGUAGE", December, 1990, pp. 57-62 ! ...
	SInt32 a, b, c, d, e, z, alpha;

	z = inValue+1;

	if(z < 2299161L)
	{
		a = z;
	}
	else
	{
		alpha = static_cast<SInt32>((z-1867216.25)/36524.25);
		a = z+1+alpha-alpha/4;
	}

	b = static_cast<SInt32>(a+1524);
	c = static_cast<SInt32>((b-122.1)/365.25);
	d = static_cast<SInt32>(365.25*c);
	e = static_cast<SInt32>((b-d)/30.6001);

	outDay   = static_cast<UInt16>(
			   static_cast<SInt32>(b-d-static_cast<SInt32>(30.6001*e)));
	outMonth = static_cast<UInt16>(
			   static_cast<SInt32>(e < 13.5 ? e-1 : e-13));
	outYear  = static_cast<UInt16>(
			   static_cast<SInt32>(outMonth > 2.5 ? c-4716 : c-4715));
}

HL_End_Namespace_BigRedH
