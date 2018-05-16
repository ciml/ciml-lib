#include "GPTime.h"

GPTime::GPTime(int numTimes)
{
    this->numTimes = numTimes;
    for(int i = 0; i < numTimes; i++){
        startTimes.push_back(0);
        endTimes.push_back(0);
        totalTimes.push_back(0);
    }
}

GPTime::~GPTime()
{
    //dtor
}

double GPTime::getStartTime(TimeType tp){
    struct timeval tv;
    gettimeofday(&tv, 0);
    double time = (double)tv.tv_sec + 1.0e-6*(double)tv.tv_usec;

    startTimes[tp] = time;

    return time;
}

double GPTime::getEndTime(TimeType tp){
    struct timeval tv;
    gettimeofday(&tv, 0);
    double time = (double)tv.tv_sec + 1.0e-6*(double)tv.tv_usec;

    endTimes[tp] = time;

    return time;
}


double GPTime::getElapsedTime(TimeType tp){
    double time = endTimes[tp] - startTimes[tp];
    totalTimes[tp] += time;
    return time;
}