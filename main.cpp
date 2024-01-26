#include "Header.h"
#include "types.h"
#include "Camera.h"

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

void FileWrite(string fileName, Header* currentImage);
float DotProduct(VectorThree a, VectorThree b);

int main()
{
    std::string fileName = "output/result.tga";
    std::cout << "testing c++";
    float ColorValToSmall(unsigned char colorVal);
    unsigned char SmallToColorVal(float colorValSmall);

    //Create a new Header
    Header* finalResult = new Header;

    //Set the image resolution
    finalResult->width = 256;
    finalResult->height = 256;
    int pixelCount = (finalResult->width * finalResult->width);

    //Set Up Camera
    PerspectiveCam camera;
    UnitSphere sphere;
    for (int y = 0; y < finalResult->width; y++)
    {
        for (int x = 0; x < finalResult->height; x++) 
        {
            Ray* curRay = camera.getRay(x, y);
            VectorThree negDirection;
            negDirection.x = curRay->direction.x;
            negDirection.y = curRay->direction.y;
            negDirection.z = curRay->direction.z;
            float t = ((DotProduct(negDirection, sphere.position)) + sqrt(pow((DotProduct(curRay->direction, sphere.position)), 2) - (DotProduct(sphere.position, sphere.position) + 1)));
            cout << t;
            //hitSUrface, t = s.intersect(curRay, 0, +inf)
            //if hitSurface is not null
            //image.set(x, y, white);
        }
    }


    //Edit the image
    for (int i = 0; i < pixelCount; i++)
    {
        Header::Pixel* currentPixel = new Header::Pixel;

    
        if (i == ((pixelCount/2) + 128))
        {
            //Blue
            currentPixel->blue = (unsigned char) 255;

            //Green
            currentPixel->green = (unsigned char) 255;

            //Red
            currentPixel->red = (unsigned char) 255;
        }
        else
        {
            //Blue
            currentPixel->blue = (unsigned char) 0;

            //Green
            currentPixel->green = (unsigned char) 0;

            //Red
            currentPixel->red = (unsigned char) 0;
        }
        finalResult->pixels.push_back(currentPixel);
    }

    FileWrite(fileName, finalResult);
    return 0;
}

//Type Conversion
float ColorValToSmall(unsigned char colorVal)
{
   float colorVal_ = (int)colorVal;
   return (colorVal_ / 255.0);
}

unsigned char SmallToColorVal(float colorValSmall)
{
    float colorVal_ = colorValSmall * 255.0;
    colorVal_ += 0.5f;
    unsigned char finalVal = (unsigned char) colorVal_;
    return colorVal_;
}

void FileWrite(string fileName, Header* currentImage) 
{
    ofstream imageFile(fileName, ios_base::out | ios_base::binary);

    if (imageFile.is_open())
    {
        char idLength_ = currentImage->idLength;
        imageFile.write(&idLength_, sizeof(idLength_));

        char colorMapType_ = currentImage->colorMapType;
        imageFile.write(&colorMapType_, sizeof(colorMapType_));

        char dataTypeCode_ = currentImage->dataTypeCode;
        imageFile.write(&dataTypeCode_, sizeof(dataTypeCode_));

        short colorMapOrigin_ = currentImage->colorMapOrigin;
        imageFile.write((char*)&colorMapOrigin_, sizeof(colorMapOrigin_));

        short colorMapLength_ = currentImage->colorMapLength;
        imageFile.write((char*)&colorMapLength_, sizeof(colorMapLength_));

        char colorMapDepth_ = currentImage->colorMapDepth;
        imageFile.write(&colorMapDepth_, sizeof(colorMapDepth_));

        short xOrigin_ = currentImage->xOrigin;
        imageFile.write((char*)&xOrigin_, sizeof(xOrigin_));

        short yOrigin_ = currentImage->yOrigin;
        imageFile.write((char*)&yOrigin_, sizeof(yOrigin_));

        short width_ = currentImage->width;
        imageFile.write((char*)&width_, sizeof(width_));

        short height_ = currentImage->height;
        imageFile.write((char*)&height_, sizeof(height_));

        char bitsPerPixel_ = currentImage->bitsPerPixel;
        imageFile.write(&bitsPerPixel_, sizeof(bitsPerPixel_));

        char imageDescriptor_ = currentImage->imageDescriptor;
        imageFile.write(&imageDescriptor_, sizeof(imageDescriptor_));

        int pixelCount = (int)height_ * (int)width_;

        for (int i = 0; i < pixelCount; i++)
        {
            Header::Pixel* pixels_ = currentImage->pixels[i];
            unsigned char blue_ = pixels_->blue;
            imageFile.write((char*)&blue_, sizeof(blue_));

            unsigned char green_ = pixels_->green;
            imageFile.write((char*)&green_, sizeof(green_));

            unsigned char red_ = pixels_->red;
            imageFile.write((char*)&red_, sizeof(red_));
        }
        imageFile.close();
    }
}

float DotProduct(VectorThree a, VectorThree b)
{
    float curFloat = 0;
    curFloat += (a.x + b.x);
    curFloat += (a.y + b.y);
    curFloat += (a.z + b.z);
    return curFloat;
}
