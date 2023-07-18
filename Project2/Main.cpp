#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include <future>
#include "Cmath.h"
#include "lodepng.h"

unsigned width = 1000;
float scale = 8;
unsigned iter = 10;
float strength = 100;
float range = 1000;

float R_it = 0.0f;
float Im_it = 0.0f;

const unsigned int threads = 500;

std::vector<unsigned char> image;

std::array<float, 2> func(std::array<float, 2>& a, int depth, float R_it, float Im_it) {    
    std::array<float, 2> b = { R_it, 0 };
    std::array<float, 2> c = { 0, Im_it };
    std::array<float, 2> a2 = { a[0], a[1] };
    if (depth == iter) { Cadd(a, b); }
    if (depth > 0) {
        Cmulti(a, a);
        Cadd(a, b);
        Cadd(a, c);
        Ccos(a2);
        Cdiv(a, a2);

        return func(a, depth - 1, R_it, Im_it);
    }
    else {
        return a;
    }
}


std::vector<unsigned char> generate(std::vector<unsigned char> image) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            std::array<float, 2> pos = { 2.0f * scale * j / width - scale, 2.0f * scale * i / width - scale };
            func(pos, iter, R_it, Im_it);
            std::array<unsigned char, 4> rgb = Crgb(pos, strength, range);
            image.push_back(rgb[0]);
            image.push_back(rgb[1]);
            image.push_back(rgb[2]);
            image.push_back(rgb[3]);
        }
        //std::cout << R_it << "\n";
        //std::cout << i << " / " << width << std::endl;
    }
    return image;
}

std::vector<unsigned char> generate_rows(int start_row, int num_of_rows) {
    std::vector<unsigned char> temp;
    for (int row = start_row; row < start_row + num_of_rows; row++) {
        for (int i = 0; i < width; i++) {
            std::array<float, 2> pos = { 2.0f * scale * i / width - scale, 2.0f * scale * row / width - scale };
            func(pos, iter, R_it, Im_it);
            std::array<unsigned char, 4> rgb = Crgb(pos, strength, range);
            temp.push_back(rgb[0]);
            temp.push_back(rgb[1]);
            temp.push_back(rgb[2]);
            temp.push_back(rgb[3]);
        }
        //if (start_row == 0) {
        //    std::cout << (int)(row * 100 / num_of_rows) << "%" << std::endl;
        //}
    }
    return temp;
}

std::vector<unsigned char> generate_image(std::vector<unsigned char> image) {
    std::thread myThread[threads];
    std::vector<unsigned char> temp[threads];
    std::future<std::vector<unsigned char>> idk[threads];

    for (int i = 0; i < threads; i++) {
        idk[i] = std::async(generate_rows, i * width / (threads), (i + 1) * width / (threads)-i * width / (threads));
    }

    for (int i = 0; i < threads; i++) {
        temp[i] = idk[i].get();
    }

    for (int i = 0; i < threads; i++) {
        for (int j = 0; j < temp[i].size(); j++) {
            image.push_back(temp[i][j]);
        }
    }
    return image;
}

int main()
{
    for (int i = 0; i < 100; i++) {
        image = { };
        image = generate_image(image);
        createPNG(image, width, width, "Outputs/output"+std::to_string(i)+".png");
        //R_it += 0.02f;
        Im_it += 0.03f;
        //scale -= 0.07;
        //iter++;
	}

    //image = generate_image(image);
    //createPNG(image, width, width, "Outputs/output.png");

    return 0;
}