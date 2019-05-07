#pragma once

// #define SWEDISH
//
// Add language here if need be
//
#ifdef SWEDISH
#define SECOND (TCHAR *)"sekund"
#define SECONDS (TCHAR *)"sekunder"
#define MINUTE (TCHAR *)"minut"
#define MINUTES (TCHAR *)"minuter"
#define HOUR (TCHAR *)"timme"
#define HOURS (TCHAR *)"timmar"
#define DAY (TCHAR *)"dag"
#define DAYS (TCHAR *)"dagar"
#define YEAR (TCHAR *)"år"
#define YEARS (TCHAR *)"år"
#else
#define SECOND (TCHAR *)"second"
#define SECONDS (TCHAR *)"seconds"
#define MINUTE (TCHAR *)"minute"
#define MINUTES (TCHAR *)"minutes"
#define HOUR (TCHAR *)"hour"
#define HOURS (TCHAR *)"hours"
#define DAY (TCHAR *)"day"
#define DAYS (TCHAR *)"days"
#define YEAR (TCHAR *)"year"
#define YEARS (TCHAR *)"years"
#endif
#define BYTES (TCHAR *)"bytes"
#define KILOBYTES (TCHAR *)"kilobytes"
#define MEGABYTES (TCHAR *)"megabytes"
#define GIGABYTES (TCHAR *)"gigabytes"
#define TERABYTES (TCHAR *)"terabytes"
#define PETABYTES (TCHAR *)"petabytes"
#define EXABYTES (TCHAR *)"exabytes"
// --------------------------------------------------------------
#define COMMA (TCHAR *)", "
#define ONEKILOBYTE 1024
#define ONEMEGABYTE (ONEKILOBYTE * ONEKILOBYTE)
#define ONEGIGABYTE (ONEMEGABYTE * ONEMEGABYTE)
#define ONETERABYTE (ONEGIGABYTE * ONEGIGABYTE)
#define ONEPETABYTE (ONETERABYTE * ONETERABYTE)
#define ONEEXABYTE (ONEPETABYTE * ONEPETABYTE)

//#define ACCOUNTFORLEAPYEARS // Experimental only

#define SECONDSINAMINUTE 60	
//
// 365 * 86400. 31557816 - 31536000 = 21816 seconds -> Lagging 6 hours 3 minutes 36 seconds per year
// 3 minutes 36 seconds: 216 * 400 = 86400 seconds, one day
//
#ifdef ACCOUNTFORLEAPYEARS				// Experimental only
#define SECONDSINANHOUR 3602			// 3600 * 365.2525/365 = 3602.490
#define SECONDSINADAY 86460				// 24 * 3600 * 365.2525/365 = 86459.769063013
#define SECONDSINAYEAR 31557816			// 365.2525 * 86400 (365 + 1/4 + 1/400)
#else
#define SECONDSINANHOUR 3600			// 60 * 60
#define SECONDSINADAY 86400				// 24 * 3600
#define SECONDSINAYEAR 31536000			// 86400 * 365
#endif

#define DECIMALCOUNTINDEX 2 // Index position for 2 in "%.2f %s";

TCHAR *formatNumber(size_t num, TCHAR *result);
TCHAR * formatSize(size_t bytes, TCHAR * result);
TCHAR * formatTime(double numberOfSeconds, TCHAR * result);
void oneUnit(size_t count, TCHAR * plural, TCHAR * singular, TCHAR ** comma, TCHAR * result);
