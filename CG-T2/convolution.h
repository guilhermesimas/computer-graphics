#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <vector>
#include "image.h"

class Convolution
{

public:
    Convolution(const Image& img, float *mask);

    float evaluate(int x, int y);

protected:
    int _width;
    int _height;
    int _depth;
    std::vector<float> _buffer;
    std::vector<float> _mask;
    float _denominator;
};

#endif // CONVOLUTION_H
