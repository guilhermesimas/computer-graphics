#include "convolution.h"

//for abs
#include <cmath>

Convolution::Convolution(Image& img, std::vector<float> mask)
{
    Image imgInstance(img);
    imgInstance.getDimensions(_width,_height,_depth);
    _buffer = img.getColorBuffer();
    _mask = mask;
    _denominator = 0;
    for(unsigned int i=0;i<_mask.size();i++){
        _denominator+=abs(_mask[i]);
    }
    if(_denominator==0) _denominator = 1;
}

float Convolution::evaluate(int x, int y, int k)
{
    float sum=0;
    for(int i=x-1;i<x+2;i++){
        for(int j=y-1;j<y+2;j++){
            if(!(i<0 || j<0 || i>=_width || j>=_height)){
                int n = j*_width*_depth+i*_depth + k;
                sum+=_buffer[n]*_mask[(j-y+1)*3 + i-x+1];
            }
        }
    }

    return sum/_denominator;
}
