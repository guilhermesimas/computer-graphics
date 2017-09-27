#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <vector>
#include "image.h"

class Convolution
{

public:
    Convolution(Image& img, std::vector<float> mask);

    float evaluate(int x, int y, int k);

protected:
    int _width;
    int _height;
    int _depth;
    std::vector<float> _buffer;
    std::vector<float> _mask;
    float _denominator;
};

#endif // CONVOLUTION_H
