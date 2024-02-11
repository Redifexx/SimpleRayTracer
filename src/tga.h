#include "Header.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

void FileWrite(string fileName, Header* currentImage);

struct ImageFile 
{
    std::string fileName;
    Header* finalResult;
    int width;
    int height;

    ImageFile (int width_, int height_)
    {
        finalResult = new Header();
        width = width_;
        height = height_;
        fileName = "result.tga";
        finalResult->width = width_;
        finalResult->height = height_;
        finalResult->pixels.resize(height);
        for (int i = 0; i < height_; i++)
        {
            finalResult->pixels[i].resize(width);
        }
    }
    ~ImageFile ()
    {
        delete finalResult;
    }

    void addPixel(int i_, int j_, int r, int g, int b)
    {
        Header::Pixel* currentPixel = new Header::Pixel();
        currentPixel->blue = b;
        //Green
        currentPixel->green = g;
        //Red
        currentPixel->red = r;
        
        //std::cout << "TEST " << finalResult->pixels.size() << std::endl;
        finalResult->pixels.at(j_).at(i_) = currentPixel;
        /*
        if (!finalResult->pixels.at(j_).at(i_) || (finalResult->pixels.at(j_).at(i_) == nullptr))
        {
            finalResult->pixels.at(j_).push_back(currentPixel);
        } else
        {
            finalResult->pixels.at(j_).at(i_) = currentPixel;
        }*/

        //std::cout << "Pixel Added!: " << finalResult->pixels.size() << " " << finalResult->pixels[j_].size() << std::endl;
    }

    void fileWrite()
    {
        ofstream imageFile(fileName, ios_base::out | ios_base::binary);
        std::cout << "TEST1 " << imageFile.is_open() << " " << fileName << std::endl;
        if (imageFile.is_open())
        {
            std::cout << "TEST" << std::endl;
            char idLength_ = finalResult->idLength;
            imageFile.write(&idLength_, sizeof(idLength_));

            char colorMapType_ = finalResult->colorMapType;
            imageFile.write(&colorMapType_, sizeof(colorMapType_));

            char dataTypeCode_ = finalResult->dataTypeCode;
            imageFile.write(&dataTypeCode_, sizeof(dataTypeCode_));

            short colorMapOrigin_ = finalResult->colorMapOrigin;
            imageFile.write((char*)&colorMapOrigin_, sizeof(colorMapOrigin_));

            short colorMapLength_ = finalResult->colorMapLength;
            imageFile.write((char*)&colorMapLength_, sizeof(colorMapLength_));

            char colorMapDepth_ = finalResult->colorMapDepth;
            imageFile.write(&colorMapDepth_, sizeof(colorMapDepth_));

            short xOrigin_ = finalResult->xOrigin;
            imageFile.write((char*)&xOrigin_, sizeof(xOrigin_));

            short yOrigin_ = finalResult->yOrigin;
            imageFile.write((char*)&yOrigin_, sizeof(yOrigin_));

            short width_ = finalResult->width;
            imageFile.write((char*)&width_, sizeof(width_));

            short height_ = finalResult->height;
            imageFile.write((char*)&height_, sizeof(height_));

            char bitsPerPixel_ = finalResult->bitsPerPixel;
            imageFile.write(&bitsPerPixel_, sizeof(bitsPerPixel_));

            char imageDescriptor_ = finalResult->imageDescriptor;
            imageFile.write(&imageDescriptor_, sizeof(imageDescriptor_));
            /*
            std::cout << finalResult->pixels.size() << std::endl;
            for (int i = 0; i < finalResult->pixels.size(); i++)
            {
                Header::Pixel* pixels_ = finalResult->pixels.at(i);
                unsigned char blue_ = pixels_->blue;
                imageFile.write((char*)&blue_, sizeof(blue_));

                unsigned char green_ = pixels_->green;
                imageFile.write((char*)&green_, sizeof(green_));

                unsigned char red_ = pixels_->red;
                imageFile.write((char*)&red_, sizeof(red_));
            }*/
            std::cout << finalResult->pixels.size() << std::endl;
            std::cout << finalResult->pixels.at(finalResult->pixels.size() - 1).size() << std::endl;

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++ )
                {
                    Header::Pixel* pixels_ = finalResult->pixels.at(j).at(i);
                    unsigned char blue_ = pixels_->blue;
                    imageFile.write((char*)&blue_, sizeof(blue_));

                    unsigned char green_ = pixels_->green;
                    imageFile.write((char*)&green_, sizeof(green_));

                    unsigned char red_ = pixels_->red;
                    imageFile.write((char*)&red_, sizeof(red_));
                }
            }
            imageFile.close();
        } else
        {
            std::perror("Error opening file");
            std::cout << "Working Directory: " << std::filesystem::current_path() << std::endl;
        }
    }
};