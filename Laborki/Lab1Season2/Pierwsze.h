#ifndef UNTITLED2_PIERWSZE_H
#define UNTITLED2_PIERWSZE_H

#include "Lista.h"

template<typename T>
class Pierwsze
{
private:
    const int ileLiczb = 10000;
     Lista<T> ListaLiczb;

public:
    Pierwsze(int aleIleLiczb) : ileLiczb(aleIleLiczb)
    {
        ListaLiczb.add(2);
        for(int i=0; i< ileLiczb-1; i++)
        {
            int lp=ListaLiczb.getLast();
            bool islp=1;
            while (1)
            {
                lp++;
                for (auto h : ListaLiczb)
                {
                    if (lp%h==0)
                        islp=0;

                }
                if (islp==1)
                    ListaLiczb.add(lp);
                    break;
            }
        }

    }
};


#endif //UNTITLED2_PIERWSZE_H
