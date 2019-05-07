#include <stdio.h>
#include <tchar.h>
#include "formatnumber.h"
//
// Test code
//
int main()
{
	int i;
	TCHAR buf[100];
	size_t num = 2033198;
	size_t siz = 3451561029;

	double secondsCount = 70.98; 	// double max: 292 471 208 677 years, 195 days, 15 hours, 30 minutes, 8 seconds

	printf("%s\n", formatNumber(num, buf));
	printf("%s\n", formatSize(siz, buf));


	for (i = 0; i < 20; i++)
	{
		printf("%s\n", formatTime(secondsCount, buf));
		secondsCount -= 1.099; 
	}
	return 0;
}