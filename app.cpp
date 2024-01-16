#include <iostream>
#include <stdio.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "header/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "header/stb_image_write.h"

using namespace std;

class ImageProcessor {
private:
    int width, height, channels;
    unsigned char* img;
    string img_name;

public:
    int loadImage(string file_name) {        
        img = stbi_load(file_name.c_str(), &width, &height, &channels, 0);
        img_name = file_name.substr(0,file_name.find("."));
        if (img == 0) {
            cout << "Error loading image file..!" << endl;
            if (stbi_failure_reason()) {
                cout << "Reason :" << stbi_failure_reason() << "\n";
            }
            return false;
        }
        return true;
    }

    void convertToImage(string file_name, unsigned char* resultImage) {
        stbi_write_jpg(file_name.c_str(), width, height, channels, resultImage, 0);
        greeting(file_name);
    }

    void greeting(string file_name) {
        cout<<"Processed Image is stored in "<< file_name << endl;
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
        
        convertToImage("Processed Images/"+img_name+"_bw_image.jpg", blackndwhite);
        delete[] blackndwhite;
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
        convertToImage("Processed Images/"+img_name+"_hflipped_image.jpg", flippedImg);
        delete[] flippedImg;
    }

    void verticalFlip() {
        unsigned char* flippedImg = new unsigned char[width * height * channels];
        int rowSize = width * channels;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                for (int c = 0; c < channels; ++c) {
                    flippedImg[(height - 1 - y) * rowSize + x * channels + c] = img[y * rowSize + x * channels + c];
                }
            }
        }
        convertToImage("Processed Images/"+img_name+"_vflipped_image.jpg", flippedImg);
        delete[] flippedImg;
    }

    void vignetteTheImage() {
        
        unsigned char* vignetteImg = new unsigned char[width * height * channels];

        int centerX = width / 2;
        int centerY = height / 2;

        float maxDistance = std::sqrt(static_cast<float>(centerX * centerX + centerY * centerY));
    
        float vignetteStrength = 0.8;
    
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = (y * width + x) * channels;   
                float distance = std::sqrt(static_cast<float>((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY)));            
                float vignetteFactor = 1.0 - (distance / maxDistance) * vignetteStrength;
                for (int c = 0; c < channels; ++c) {
                    vignetteImg[index + c] = static_cast<unsigned char>(img[index + c] * vignetteFactor);
                }
            }
        }
        convertToImage("Processed Images/"+img_name+"_vignette_image.jpg", vignetteImg);
        delete[] vignetteImg;
    }


    void sharpenTheImage() {
        const float sharpenMatrix[3][3] = {
            { 0, -1,  0 },
            { -1, 5, -1 },
            { 0, -1,  0 }
        };

        unsigned char *sharpenedImg = new unsigned char[width * height * channels];
        int size = width * height * channels;

        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                for (int c = 0; c < channels; ++c) {
                    float sum = 0.0f;
                    int index = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            sum += img[(y + i) * width * channels + (x + j) * channels + c] * sharpenMatrix[i + 1][j + 1];
                        }
                    }
                    sharpenedImg[y * width * channels + x * channels + c] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, sum)));
                }
            }
        }
        convertToImage("Processed Images/"+img_name+"_sharpened_image.jpg", sharpenedImg);
        delete[] sharpenedImg;
    }


    void contrastTheImage() {
        unsigned char* contrastImg = new unsigned char[width * height * channels];

        float factor = 1.5f;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = (y * width + x) * channels;

                for (int c = 0; c < channels; ++c) {
                    contrastImg[index + c] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, (img[index + c] / 255.0f - 0.5f) * factor + 0.5f) * 255.0f));
                }
            }
        }
        convertToImage("Processed Images/"+img_name+"_contrasted_image.jpg", contrastImg);
        delete[] contrastImg;
    }

};

int main()
{
    int choice;
    string file_name;
    while (1) {
        cout << "Enter the image file : ";
        cin >> file_name;

        ImageProcessor image;

        // Image input
        if(!image.loadImage(file_name)) {
            continue;
        }

        cout << "Available Operations :"<< endl; 
        cout << "1. Black and White Image \n2. Horizontal Flip \n3. Vertial Flip \n4. Sharpen Image \n5. Vignette Image \n6. Contrast Image \n7. Exit \nSelect Operation: ";
        cin >> choice;

        if (choice == 1) {                                     
            image.convertToBW();        // Black and White
        }
        else if (choice == 2) {
            image.horizontalFlip();     // Horizontal Flip
        }
        else if (choice == 3) {
            image.verticalFlip();       // Vertical Flip
        }
        else if (choice==4) {            
            image.sharpenTheImage();    // Sharpen Image
        }
        else if(choice==5) {
            image.vignetteTheImage();   // Vignette Image
        }
        else if(choice==6) {            
            image.contrastTheImage();   // Contrast Image
        }
        else if (choice==7) {
            cout << "Good Bye\n";
            break;
        }
        else {
            cout << "Invalid Choice" << "\n";
        }
    }
}