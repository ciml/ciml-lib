#ifndef LEASTSQUAREINDIVIDUO_H
#define LEASTSQUAREINDIVIDUO_H


class LeastSquareIndividuo : public SimpleIndividuo
{
    public:
        double ** leastSquare = NULL;

        LeastSquareIndividuo();
        virtual string nameIndividual(){
            return "LeastSquareIndividuo";
        }
        virtual void print();
        virtual ~LeastSquareIndividuo();

    protected:

    private:
};

#endif // LEASTSQUAREINDIVIDUO_H
