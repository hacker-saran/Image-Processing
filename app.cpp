#include <iostream>
#include <stdio.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "header/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "header/stb_image_write.h"

using namespace std;

class ImageProcessor
{
private:
    int width, height, channels;
    unsigned char* img;

public:
    void loadImage(string file_name)
    {
        
        img = stbi_load(file_name.c_str(), &width, &height, &channels, 0);
        if (img == 0)
        {
            cout << "Error loading image file..!" << endl;
            if (stbi_failure_reason())
                cout << "Reason :" << stbi_failure_reason() << "\n";
            return;
        }
    }

    void convertToImage(string file_name, unsigned char* resultImage) {
        stbi_write_jpg(file_name.c_str(), width, height, channels, resultImage, 0);
    }

    void convertToBW() {
        unsigned char *blackndwhite = new unsigned char[width * height * channels];
        int size = width * height * channels;


        for (int i = 0; i < size; i += channels) {
            int mean = 0;
            for (int j = 0; j < channels; ++j) {
                mean += img[i + j];
            }
            mean /= channels;
            
            for (int j = 0; j < channels; ++j) {
                blackndwhite[i + j] = (mean < 128) ? 0 : 255;
            }
        }
        cout<< width << " " << height << " " << channels << "\n";
        convertToImage("BlackAndWhite.jpg", dup);
    }

    void horizontalFlip() {
        unsigned char* flippedImg = new unsigned char[width * height * channels];
        int rowSize = width * channels;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                for (int c = 0; c < channels; ++c) {
                    flippedImg[y * rowSize + (width - 1 - x) * channels + c] = img[y * rowSize + x * channels + c];
                }
            }
        }

        convertToImage("HorizontalFlip.jpg", flippedImg);

        delete[] flippedImg;
    }
};

int main()
{

    int choice;
    string file_name;
    while (1)
    {
        cout << "Enter the image file : ";
        cin >> file_name;
        cout << file_name << "\n";

        ImageProcessor image;
        image.loadImage(file_name);

        cout << "Available Operations : \n1. Black and White Image 1 \n2. Operation 2 \n3.Exit \nSelect Operation: ";
        cin >> choice;
        if (choice == 1)
        {                                     
            image.convertToBW();    // Black and White

        }
        else if (choice == 2)
        {
            image.horizontalFlip();    // operation2
        }
        else if (choice == 3)
        {
            cout << "Good Bye\n";
            break;
        }
        else
        {
            cout << "Invalid Choice"
                 << "\n";
        }
    }
}