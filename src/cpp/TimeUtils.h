#include <sys/time.h>

class TimeUtils{
public:
	static double getRealTime()
	{
		struct timeval tv;
		gettimeofday(&tv,0);
		return (double)tv.tv_sec + 1.0e-6*(double)tv.tv_usec;
	}
};
