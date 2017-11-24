#include "matrixmultiply.h"

MatrixMultiply::MatrixMultiply(const uint8 *K,
                               const unsigned int sizeKx,
                               const unsigned int sizeKy) : K(K), sizeKx(sizeKx), sizeKy(sizeKy)
{

}

uint8* MatrixMultiply::apply(uint8 *I, const unsigned int sizeY)
{
    uint8 *Y = new uint8[sizeKx * sizeY];


    for (unsigned int iX = 0; iX < sizeKx; ++iX)
        {
        for (unsigned int iY = 0; iY < sizeY; ++iY)
            {
	   
	      Y[iX][iY] = 0;
	      for (int z = 0; z < sizeKy; z++)
	      Y[iX][iY] += I[iX][z] * K[z][iY];
	    }

	}
}
		
		  
		    
		    
		    
		 
   
