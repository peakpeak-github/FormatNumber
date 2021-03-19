#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <cmath>
//
// Add language here if need be
//
#ifdef SWEDISH
#define SECOND (const char *)"sekund"
#define SECONDS (char *)"sekunder"
#define MINUTE (char *)"minut"
#define MINUTES (char *)"minuter"
#define HOUR (char *)"timme"
#define HOURS (char *)"timmar"
#define DAY (char *)"dag"
#define DAYS (char *)"dagar"
#define YEAR (char *)"år"
#define YEARS (char *)"år"

#define MILLIUNIT "mm" 
#define CENTIUNIT "cm"
//#define DECIUNIT "dm" 
#define METUNIT "m"
#define KILUNIT "km"
#define MILUNIT "mil"
enum
{
	MILLIMETER = 0,
	CENTIMETER,
#ifdef DECIUNIT
	DECIMETER,
#endif
	METER,
	KILOMETER,
	MIL,
	UNITCOUNT
};
#else // English
#define SECOND (const char *)"second"
#define SECONDS (const char *)"seconds"
#define MINUTE (const char *)"minute"
#define MINUTES (const char *)"minutes"
#define HOUR (const char *)"hour"
#define HOURS (const char *)"hours"
#define DAY (const char *)"day"
#define DAYS (const char *)"days"
#define YEAR (const char *)"year"
#define YEARS (const char *)"years"

#define MILLIUNIT "mm"
#define CENTIUNIT "cm"
//#define DECIUNIT "dm" // Uncomment if decimeter wanted
#define METUNIT "m"
#define KILUNIT "km"
enum
{
	MILLIMETER = 0,
	CENTIMETER,
#ifdef DECIUNIT
	DECIMETER, 
#endif
	METER,
	KILOMETER,
	UNITCOUNT
};
#endif
#define BYTES     (const char *)"bytes"
#define KILOBYTES (const char *)"kilobytes"
#define MEGABYTES (const char *)"megabytes"
#define GIGABYTES (const char *)"gigabytes"
#define TERABYTES (const char *)"terabytes"
#define PETABYTES (const char *)"petabytes"
#define EXABYTES  (const char *)"exabytes"

#define ONEKILOBYTE (uint64_t)1024
#define ONEMEGABYTE (ONEKILOBYTE * ONEKILOBYTE)
#define ONEGIGABYTE (ONEMEGABYTE * ONEKILOBYTE)
#define ONETERABYTE (ONEGIGABYTE * ONEKILOBYTE)
#define ONEPETABYTE (ONETERABYTE * ONEKILOBYTE)
#define ONEEXABYTE (ONEPETABYTE * ONEKILOBYTE)

#define SECONDSINAMINUTE 60	
#define SECONDSINANHOUR 3600			// 60 * 60
#define SECONDSINADAY 86400				// 24 * 3600
#define SECONDSINAYEAR 31536000			// 86400 * 365

#define COMMA (char *)", "

///////////////////////////////////////////////////////////////////////////////////
// Format a integer number from (-)nnnnnnnn to (-)nn nnn nnn
//
template <typename iint>
char* FormatNumber(const iint num, char* result = NULL)
{
	static char buf[36];
	char temp[24];
	unsigned int n, len, digits;
	int64_t i64;

	if (result == NULL)
		result = buf;
	i64 = num;					// Make sure 64 bits
	sprintf(temp, "%lld", i64);
	len = (int)strlen(temp);
	n = digits = 0;
	do
	{
		len--;						// Last non-copied digit
		result[n++] = temp[len];	// Copy in reverse order
		digits++;
		if (len && ((digits % 3) == 0))
			result[n++] = ' ';
	} while (len);
	result[n] = '\0';
	_strrev(result);	// Finally, reverse string
	return result;
}
///////////////////////////////////////////////////////////////////////////////////
// Format a distance in mm, cm, m, km
//
template <typename ddouble>
char* FormatDistance(const ddouble distance, const int unit = METER, char* result = NULL)
{
	static char buf[36];
	static const int multiplier[] = { 10,    
#ifdef DECIUNIT
	10, 10,
#else
		100,
#endif
		1000, 10 };				// Multiplier to the next higher unit
	static const char* suffixes[] = { MILLIUNIT, CENTIUNIT, 
#if defined DECIUNIT
		DECIUNIT,
#endif 
		METUNIT, KILUNIT
#if defined SWEDISH 
		, MILUNIT
#endif 
	};
//	char format[] = "%g %s"; // No formatting of number
	char format[] = "%s %s"; // Format the number
	double count = distance;
	int suffix = unit;

	if (result == NULL)
		result = buf;
	while ((count < 1.0) && (suffix))	// Om mindre än 1, minska enhet
		count *= multiplier[--suffix];
	while ((count >= multiplier[suffix]) && (suffix < UNITCOUNT - 1))
		count /= multiplier[suffix++];
//	sprintf(result, format, count, suffixes[suffix]);				// No formatting of number
	sprintf(result, format, FormatNumber(count), suffixes[suffix]); // Format the number
	return result;
}
///////////////////////////////////////////////////
// Format a size with appropriate suffix
//
template <typename sizet>
char* FormatSize(const sizet bytes, char* result = NULL)
{
	static char buf[36];
	static const char* suffixes[] = { BYTES, KILOBYTES, MEGABYTES, GIGABYTES, TERABYTES, PETABYTES, EXABYTES };
#define SUFFIXSZ (sizeof(suffixes) / sizeof(suffixes[0]))
#define DECIMALCOUNTINDEX 2 // Index position for 2 in "%.2f %s"; below

	char format[] = "%.2f %s";
	int suffix = 0;
	double count = (double)bytes;

	if (result == NULL)
		result = buf;

	while ((count >= ONEKILOBYTE) && (suffix < SUFFIXSZ))
	{
		suffix++;
		count /= ONEKILOBYTE;
	}
	if (suffix == 0)						// Bytes only, no decimal
		format[DECIMALCOUNTINDEX] = '0';	// Patch format specification to zero decimals
	sprintf(result, format, count, suffixes[suffix]);
	return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Format a time with appropriate units
// uint64_t->64 bits in millisecs: 18446744073709551615 / 1000 / 31536000 -> 584942417 years
//
template <typename cchar>
void oneUnit(const uint64_t count, const cchar* plural, const cchar* singular, cchar** comma, cchar* result)
{
	char numBuf[24];
	int len = (int)strlen(result);	// Append to result
	const char* timeText = plural;

	if (count == 1)
		timeText = singular;
	FormatNumber(count, numBuf);
	sprintf(result + len, "%s%s %s", *comma, numBuf, timeText);
	*comma = COMMA; // comma from now on
}
template <typename uint64t>
char* FormatTime(const uint64t numberOfMilliSeconds, char* result = NULL, bool noDecimalsForSeconds = false)
{
	static char buf[36];

	if (result == NULL)
		result = buf;
#define DECIMALSECINDEX 3 // Index position for 2 in "%0.2f %s"; below
	if (numberOfMilliSeconds < 60000)	// Use fractions for seconds only
	{
		double seconds = numberOfMilliSeconds / 1000.0f;
		char format[] = "%0.2f %s";
		const char* timeText = SECONDS;
		if (noDecimalsForSeconds)					// No decimals, rounding will occur
		{
			if ((seconds >= 0.5) && (seconds < 1.5))
				timeText = SECOND;
			format[DECIMALSECINDEX] = '0';			// Patch format specification to zero decimals
		}
		else if (seconds == 1.000)
			timeText = SECOND;
		sprintf(result, format, seconds, timeText);	// Result is rounded
		return result;
	}
	uint64_t roundedSeconds = (uint64_t)round(numberOfMilliSeconds / 1000.0f); // 61499 -> 1m 1s, 61500 -> 1m 2s
	uint64_t yearCount = (roundedSeconds / SECONDSINAYEAR);
	int dayCount = ((roundedSeconds % SECONDSINAYEAR) / SECONDSINADAY);
	int hourCount = (int)((roundedSeconds % SECONDSINADAY) / SECONDSINANHOUR);
	int minuteCount = (int)((roundedSeconds % SECONDSINANHOUR) / SECONDSINAMINUTE);
	int secondCount = (int)(roundedSeconds % SECONDSINAMINUTE);
	char* comma = (char*)"";	// No comma yet

	*result = '\0';
	if (yearCount)
		oneUnit(yearCount, YEARS, YEAR, &comma, result);
	if (dayCount)
		oneUnit(dayCount, DAYS, DAY, &comma, result);
	if (hourCount)
		oneUnit(hourCount, HOURS, HOUR, &comma, result);
	if (minuteCount)
		oneUnit(minuteCount, MINUTES, MINUTE, &comma, result);
	if (secondCount || (roundedSeconds == 0))
		oneUnit(secondCount, SECONDS, SECOND, &comma, result);
	return result;
}
