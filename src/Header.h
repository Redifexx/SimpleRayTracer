#pragma once
#include <vector>
using namespace std;

struct Header
{
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor; 
    struct Pixel
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };
    vector<Header::Pixel*> pixels;
    Header();
    ~Header();
};

Header::Header()
{
    this->idLength = 0;
    this->colorMapType = 0;
    this->dataTypeCode = 2;
    this->colorMapOrigin = 0;
    this->colorMapLength = 0;
    this->colorMapDepth = 0;
    this->xOrigin = 0;
    this->yOrigin = 0;
    this->width = 16;
    this->height = 16;
    this->bitsPerPixel = 24;
    this->imageDescriptor = 0;
};

Header::~Header() 
{
    for(int i = 0; i < pixels.size(); i++)
    {
        delete pixels.at(i);
    }
};
