#include <stdio.h>
#include "FormatNumber.hpp"

int main()
{
	uint64_t n = 138765;
	uint64_t q = 90123719401;

	printf("%s\n", FormatNumber(n));
	printf("%s\n", FormatNumber(q));

	printf("%s\n", FormatDistance(n));
	printf("%s\n", FormatDistance(q, MILLIMETER));

	printf("%s\n", FormatSize(n));
	printf("%s\n", FormatSize(q));

	printf("%s\n", FormatTime(n));
	printf("%s\n", FormatTime(q));
	return 0;
}
