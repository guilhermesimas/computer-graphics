#include "image.h"

#include <QtMath>

#include "convolution.h"

Image::Image()
    : _width(0), _height(0), _depth(0)
{
}


Image::Image(const Image& orig)
    : _width(orig._width)
    , _height(orig._height)
    , _depth(orig._depth)
    , _buffer(orig._buffer)
{
}


Image::Image(int width, int height, int depth)
    : _width(width)
    , _height(height)
    , _depth(depth)
    , _buffer(_width * _height * _depth, 0.0f) //Inicializa o buffer com 0's
{
}


Image::Image(int width, int height, int depth, float* buffer)
    : _width(width)
    , _height(height)
    , _depth(depth)
    , _buffer(buffer, buffer + _width * _height * _depth)
{
}


Image::~Image()
{
}


void Image::getDimensions(int& width, int& height, int& depth)
{
    width = _width;
    height = _height;
    depth = _depth;
}


std::vector<float>& Image::getColorBuffer()
{
    return _buffer;
}


Image Image::toGrayscale()
{
    if (_depth==1) return *this;

    Image outImage(_width,_height, 1);

    std::vector<float>& dst = outImage.getColorBuffer();

    for( int y = 0; y < _height; y++ )
    {
        for( int x = 0; x < _width; x++ )
        {
            int k = y*_width*_depth+x*_depth;
            dst[y*_width+x]=0.2126f*_buffer[k] +0.7152f*_buffer[k+1]+0.0722f*_buffer[k+2];
        }
    }
   return outImage;
}


void Image::fromQImage(const QImage &qImage)
{
    if( !qImage.isNull() )
    {
        _buffer.clear();

        _width = qImage.width();
        _height = qImage.height();
        _depth = qImage.isGrayscale() ? 1 : 3;

        _buffer.resize(_width*_height*_depth, 0.0f);

        int pos = 0;
        for( int y = 0; y < _height; y++ )
        {
            for( int x = 0; x < _width; x++ )
            {
                QColor color = qImage.pixel(x,y);

                _buffer[pos++] = color.red()/255.f;

                if( _depth == 3 )
                {
                    _buffer[pos++] = color.green()/255.f;
                    _buffer[pos++] = color.blue()/255.f;
                }
            }
        }
    }
}


unsigned char fix(float value){
    value=(value<0)?-value:value;
    value=(value>1)?1:value;
    return (unsigned char) (255*value);
}


QImage Image::toQImage()
{
    QImage qImage( _width, _height, QImage::Format_RGB32 );

    if( !_buffer.empty() )
    {
        int pos = 0;
        for( int y = 0; y < _height; y++ )
        {
            for( int x = 0; x < _width; x++ )
            {
                float value = fix(_buffer[pos++]);
                QColor color(value, value, value);

                if( _depth == 3 )
                {
                    color.setGreen(fix(_buffer[pos++]));
                    color.setBlue(fix(_buffer[pos++]));
                }

                qImage.setPixelColor(x,y,color);
            }
        }
    }
    return qImage;
}


/** Algoritmos a serem implementados **/


void smoothing(Image& input, Image& output)
{
    float mask_array[] = {
        1.0f,2.0f,1.0f,
        2.0f,4.0f,2.0f,
        1.0f,2.0f,1.0f
    };
    std::vector<float> mask(mask_array, mask_array + sizeof(mask_array) / sizeof(mask_array[0]));
    Convolution conv(input,mask);
    int w,h,d;
    input.getDimensions(w,h,d);
    float *bufferOutput = (float*)malloc(w*h*d*sizeof(float));
    for(int k=0;k<d;k++){
        for(int j=0;j<h;j++){
            for(int i=0;i<w;i++){
                bufferOutput[j*w*d + i*d +k] = conv.evaluate(i,j,k);
            }
        }
    }
    output = Image(w,h,d,bufferOutput);
    free(bufferOutput);
}


void sobel(Image& input, Image& output)
{
    float mask_array_x[] = {
        1.0f,0.0f,-1.0f,
        2.0f,0.0f,-2.0f,
        1.0f,0.0f,-1.0f
    };
    float mask_array_y[] = {
        1.0f,2.0f,1.0f,
        0.0f,0.0f,0.0f,
        -1.0f,-2.0f,-1.0f
    };
    std::vector<float> mask_x(mask_array_x, mask_array_x + sizeof(mask_array_x) / sizeof(mask_array_x[0]));
    std::vector<float> mask_y(mask_array_y, mask_array_y + sizeof(mask_array_y) / sizeof(mask_array_y[0]));
    Convolution conv_x(input,mask_x);
    Convolution conv_y(input,mask_y);
    int w,h,d;
    input.getDimensions(w,h,d);
    float *bufferOutput = (float*)malloc(w*h*d*sizeof(float));
    for(int k=0;k<d;k++){
        for(int j=0;j<h;j++){
            for(int i=0;i<w;i++){
                bufferOutput[j*w*d + i*d +k] = sqrt(pow(conv_x.evaluate(i,j,k),2) + pow(conv_y.evaluate(i,j,k),2));
            }
        }
    }
    output = Image(w,h,d,bufferOutput);
    free(bufferOutput);
}


void haar(Image& input, Image& output)
{
    std::vector<float> buffer = input.getColorBuffer();
    int w,h,d;
    input.getDimensions(w,h,d);
    float *bufferOutput = (float*)malloc(w*h*d*sizeof(float));
    for(int k=0;k<d;k++){
        for(int j=0;j<h;j++){
            for(int i=0;i<w/2;i++){
                bufferOutput[j*w*d + i*d +k] = (buffer[j*w*d + (2*i)*d +k] + buffer[j*w*d + (2*i+1)*d +k])/2;
                bufferOutput[j*w*d + (i+(w/2))*d +k] = (buffer[j*w*d + (2*i)*d +k] - buffer[j*w*d + (2*i+1)*d +k])/2;

            }
        }
    }
    output = Image(w,h,d,bufferOutput);
    free(bufferOutput);
}


void haarInv(Image& input, Image& output)
{
    output = Image(input);
}


void enhanceHaar(Image& input, Image& output)
{
    output = Image(input);
}
