#include <algorithm>

template<unsigned int size>
ReLU<size>::ReLU()
{

}

template<unsigned int size>
void ReLU<size>::apply(layerOut_t *I)
{
    for (unsigned int i = 0; i < size; ++i)
        {
        Y[i] = I[i] > 0 ? I[i] : 0;
        }
}

