#include "stateVectorRocket.h"

StateVectorRocket& StateVectorRocket::operator*(const double b)
{
  for(int i = 0; i<SIZE; i++)
       Data[i] =  Data[i]*b;
  return *this;
}

StateVectorRocket& StateVectorRocket::operator+(const StateVectorRocket &b)
{
 for (int i = 0;i<SIZE;i++)
      Data[i] = Data[i] + b.Data[i];
 return *this;
}

StateVectorRocket& StateVectorRocket::operator=(const StateVectorRocket &b)
{
  SIZE = b.SIZE;
 for (int i = 0;i<SIZE;i++)
      Data[i] = b.Data[i];
 return *this;
}
