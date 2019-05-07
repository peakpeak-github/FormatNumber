#include <stdio.h>
#include <string.h>
#include <math.h>		// round()
#include <tchar.h>		// Unicode macros
#include "\c\FormatNumber\formatnumber.h"

const TCHAR *g_suffixes[] =
{	
	BYTES,
	KILOBYTES,
	MEGABYTES,
	GIGABYTES,
	TERABYTES,
//	PETABYTES,
//	EXABYTES
};
#define SUFFIXSZ (sizeof(g_suffixes) / sizeof(g_suffixes[0]))

///////////////////////////////////////////////////////////////////////////////////
// Reformat a number from nnnnnnnn to nn nnn nnn
//
TCHAR *formatNumber(size_t num, TCHAR *result)
{
	unsigned int n, len, digits;
	TCHAR temp[24];
	
	sprintf(temp, "%zu", num);
	len = (int)strlen(temp);
	n = digits = 0;
	do
	{
		len--;						// Last non-copied digit
		result[n++] = temp[len];	// Copy in reverse order
		digits++;
		if (len && ((digits % 3) == 0))
			result[n++] = ' '; 
	}
	while (len);
	result[n] = '\0';
	_strrev(result);	// Finally, reverse string
	return result;
}
///////////////////////////////////////////////////
// Format a size with appropriate suffix
//
TCHAR *formatSize(size_t bytes, TCHAR *result)
{
	TCHAR format[] = "%.2f %s";
	int suffix = 0; 
	double count = (double)bytes;

	while (count >= ONEKILOBYTE && suffix < SUFFIXSZ)
	{
		suffix++;
		count /= ONEKILOBYTE;
	}
	if (suffix == 0)						// Bytes only, no decimal
		format[DECIMALCOUNTINDEX] = '0';	// Patch format specification to zero decimals
	sprintf(result, format, count, g_suffixes[suffix]);
	return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Format a time with appropriate units
//
TCHAR *formatTime(double numberOfSeconds, TCHAR *result)
{
	if (numberOfSeconds < 60.0)	// Use fractions for seconds only
	{
		TCHAR *timeText = SECONDS;
		if (numberOfSeconds == 1.0)
			timeText = SECOND;
		sprintf(result, "%0.2f %s", numberOfSeconds, timeText);
		return result;
	}
	size_t roundedSeconds = (size_t)round(numberOfSeconds);
	size_t yearCount = (roundedSeconds / SECONDSINAYEAR);
	int dayCount = ((roundedSeconds % SECONDSINAYEAR) / SECONDSINADAY);
	int hourCount = (int)((roundedSeconds % SECONDSINADAY) / SECONDSINANHOUR);
	int minuteCount = (int)((roundedSeconds % SECONDSINANHOUR) / SECONDSINAMINUTE);
	int secondCount = (int)(roundedSeconds % SECONDSINAMINUTE);
	TCHAR *comma = (TCHAR *)"";	// No comma yet

	*result = '\0';
	if (yearCount)
		oneUnit(yearCount, YEARS, YEAR, &comma, result);
	if (dayCount)
		oneUnit(dayCount, DAYS, DAY, &comma, result);
	if (hourCount)
		oneUnit(hourCount, HOURS, HOUR, &comma, result);
	if (minuteCount)
		oneUnit(minuteCount, MINUTES, MINUTE, &comma, result);
	if (secondCount || (numberOfSeconds == 0))
		oneUnit(secondCount, SECONDS, SECOND, &comma, result);
	return result;
}
void oneUnit(size_t count, TCHAR *plural, TCHAR *singular, TCHAR **comma, TCHAR *result)
{
	TCHAR numBuf[24];
	int len = (int)strlen(result);	// Append to result
	TCHAR *timeText = plural;

	if (count == 1)
		timeText = singular;
	formatNumber(count, numBuf);
	sprintf(result + len, "%s%s %s", *comma, numBuf, timeText);
	*comma = COMMA; // comma from now on
}
