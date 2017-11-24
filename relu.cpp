#include "relu.h"
#include<iostream>
#include<fstream>


//f(x)=max(0,x)

#define T(y, x)  I[(y) * sizeX * sizeC + (x) * sizeC + cI]
#define overlap  2

using namespace std;

uint8 *max(unsigned int *x)
{
  return *x > 0 ? *x : 0;
}

uint8 *relu(uint8 *I, unsigned int sizeX, unsigned int sizeY, unsigned int sizeC)
{
  uint8* Y = new uint8[sizeY * sizeX * sizeC]();
    
  for (unsigned int i = 0; i < sizeY * sizeX * sizeC; ++i)
    {
      Y[i] = max(I(i));
    }
    
  return Y;
}
                       
