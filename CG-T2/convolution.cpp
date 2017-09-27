#include "convolution.h"

//for abs
#include <cmath>

Convolution::Convolution(const Image& img, float* mask)
{
    img.getDimensions(_width,_height,_depth);
    _buffer = img.getColorBuffer();
    _mask = mask;
    _denominator = 0;
    for(int i=0;i<_mask.size();i++){
        _denominator+=abs(_mask[i]);
    }
    if(_denominator==0) _denominator = 1;
}

float Convolution::evaluate(int x, int y)
{
    float sum=0;
    for(int k=0; k<_depth;k++){
        for(int i=x-1;i<x+2;i++){
            for(int j=y-1;j<y+2;j++){
                if(!(i<0 || j<0 || i>=_width || j>=_height)){
                    int k = j*_width*_depth+i*_depth + k;
                    sum+=_buffer[k]*_mask[(j+1)*3 + i+1];
                }
            }
        }
    }
    return sum/_denominator;
}
