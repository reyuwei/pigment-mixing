#include <stdio.h>
#include "mixbox.h"
#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <fstream>
#include <string>
#include <vector>

void savelist() {
    srand((unsigned)time(0));

    std::ofstream outfile;
    outfile.open("random_data_test.txt");
    int max_count = 10000;

    for (int c = 0; c < max_count; c++) {
        int r1 = (rand() % 256); // 0-255
        int g1 = (rand() % 256); // 0-255
        int b1 = (rand() % 256); // 0-255
        int r2 = (rand() % 256); // 0-255
        int g2 = (rand() % 256); // 0-255
        int b2 = (rand() % 256); // 0-255

        float t = (rand() * 1.0) / RAND_MAX;

        unsigned char r, g, b;
        mixbox_lerp_srgb8(r1, g1, b1,   // first color
            r2, g2, b2,   // second color
            t,          // mixing ratio
            &r, &g, &b);  // result
        char buffer[100];
        sprintf(buffer, "%d %d %d \t%d %d %d\t %.5f\t%d %d %d", r1, g1, b1, r2, g2, b2, t, r, g, b);
        outfile << buffer << std::endl;
    }
    outfile.close();
}

void rgb2pigment(int r, int g, int b) {
    float latent[7];
    mixbox_srgb8_to_latent(r, g, b, latent);
    printf("目标RGB: %d %d %d\n", r, g, b);
    printf("颜料比例（蓝，黄，红，白）：%.5f %.5f %.5f %.5f\n", latent[0], latent[1], latent[2], latent[3]);
    printf("Residual Light (RGB): %.5f %.5f %.5f\n\n", latent[4], latent[5], latent[6]);
    return;
}

void input_rgb(int &inputr, int &inputg, int &inputb) {

    std::cout << "Input color RGB:" << std::endl;
    std::cout << "R: ";
    std::cin >> inputr;
    std::cout << "G: ";
    std::cin >> inputg;
    std::cout << "B: ";
    std::cin >> inputb;
}

void interacitve() {
    int r = 148;
    int g = 212;
    int b = 100;

    //r = 252;
    //g = 211;
    //b = 0;

    rgb2pigment(252, 211, 0);

    while (true) {
        input_rgb(r, g, b);
        rgb2pigment(r, g, b);
    }
}


std::vector<std::string> SplitString(std::string s) {
    std::string temp = "";
    std::vector<std::string> v;
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == ' ') {
            v.push_back(temp);
            temp = "";
        }
        else {
            temp.push_back(s[i]);
        }
    }
    v.push_back(temp);
    return v;
}

void parselist() {
    std::cout << "Input rgb list: " << std::endl;
    char filen[200];
    std::cin >> filen;
    std::fstream myfile(filen);//创建一个fstream文件流对象
    std::string line;
    std::vector<std::string> colors;


    if (myfile.is_open())
    {
        std::ofstream outfile;
        outfile.open("颜料混合.txt");

        while (getline(myfile, line))
        {
            std::vector<std::string> color_vec = SplitString(line);
            int r = stoi(color_vec[0]);
            int g = stoi(color_vec[1]);
            int b = stoi(color_vec[2]);

            float latent[7];
            char buffer[1000];
            mixbox_srgb8_to_latent(r, g, b, latent);
            sprintf(buffer, "Input RGB: %d %d %d", r, g, b);
            outfile << buffer << std::endl;
            std::cout << buffer << std::endl;
            sprintf(buffer, "Pigment Ratio (blue, yellow, red, white): %.5f %.5f %.5f %.5f", latent[0], latent[1], latent[2], latent[3]);
            outfile << buffer << std::endl;
            std::cout << buffer << std::endl;

            sprintf(buffer, "Residual Light (RGB): %.5f %.5f %.5f\n", latent[4], latent[5], latent[6]);
            outfile << buffer << std::endl;
            std::cout << buffer << std::endl;

        }
        myfile.close();
        outfile.close();
    }
    else
        std::cout << "Unable to open file";
    return;
}

int main()
{
    //savelist();

    //interacitve();

    parselist();

    system("pause");
}