#ifndef GPTIME_H
#define GPTIME_H

#include <vector>
#include <iostream>
#include <sys/time.h>

enum TimeType{
    Total_T     = 0,
    Evolucao_T  = 1,
    Avaliacao_T = 2,
    Iteracao_T  = 3
};

class GPTime
{
    public:
        GPTime(int numTimes);
        ~GPTime();

        double getStartTime(TimeType tp);
        double getEndTime(TimeType tp);
        double getElapsedTime(TimeType tp);

        double getTotalTime(TimeType tp){return totalTimes[tp];};
        void setNumTimes(int numTimes){this->numTimes = numTimes;};
        int getNumTimes(){return numTimes;};

    protected:

    private:
        std::vector<double> startTimes;
        std::vector<double> endTimes;
        std::vector<double> totalTimes;

        int numTimes;
};

#endif // GPTIME_H
