#ifndef STATEVECTORROCKET__H
#define STATEVECTORROCKET__H

#include "Vector.h"
#include <vector>


class StateVectorRocket
{

public:

     int SIZE;
     std::vector <Vector3d> Data;

     StateVectorRocket(){}
       
     StateVectorRocket(int size)             
        {
          SIZE = size;
          Data.reserve(SIZE);
           for (int i = 0;i<SIZE;i++)
                {
                 Data.push_back(Vector3d(0,0,0));
                }
        }
   
     StateVectorRocket(const StateVectorRocket &InpVec):Data(InpVec.Data.size())
         {
          SIZE = InpVec.Data.size();
          for (int i = 0;i<SIZE;i++)
             {
                 Data[i] =InpVec.Data[i];
             }
        }
    
     StateVectorRocket& operator*(const double);
     StateVectorRocket& operator+(const StateVectorRocket&);
     StateVectorRocket& operator=(const StateVectorRocket&);


};

#endif
