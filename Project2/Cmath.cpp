#include <iostream>
#include <array>
#include "lodepng.h"

float pi = 3.141592f;

void Cadd(std::array<float, 2>& a, std::array<float, 2> b) {
	a[0] = a[0] + b[0];
	a[1] = a[1] + b[1];
}

void Cmulti(std::array<float, 2>& a, std::array<float, 2> b) {
	float a_0 = a[0];
	a[0] = (a[0] * b[0] - (a[1] * b[1]));
	a[1] = (a_0 * b[1] + a[1] * b[0]);
}

void Cdiv(std::array<float, 2>& a, std::array<float, 2> b) {
	float a_0 = a[0];
	a[0] = ((a[0] * b[0] + a[1] * b[1]) / (b[0] * b[0] + b[1] * b[1]));
	a[1] = ((a[1] * b[0] - a_0 * b[1]) / (b[0] * b[0] + b[1] * b[1]));
}

void Csin(std::array<float, 2>& a) {
	float a_0 = a[0];
	a[0] = (sin(a[0]) * cosh(a[1]));
	a[1] = (cos(a_0) * sinh(a[1]));
}

void Ccos(std::array<float, 2>& a) {
	float a_0 = a[0];
	a[0] = (cos(a[0]) * cosh(a[1]));
	a[1] =(-sin(a_0) * sinh(a[1]));
}

void Ctan(std::array<float, 2>& a) {
	float a_0 = a[0];
	a[0] = (sin(2 * a[0]) / (cos(2 * a[0]) + cosh(2 * a[1])));
	a[1] = (sinh(2 * a[1]) / (cos(2 * a_0) + cosh(2 * a[1])));
}

void Csub(std::array<float, 2>& a, std::array<float, 2> b) {
	float a_0 = a[0];
	a[0] = (a[0] - b[0]);
	a[1] = (a[1] - b[1]);
}

void Cr_th(std::array<float, 2>& a) {
	float a_0 = a[0];
	a[0] = (a[0] * a[0] + a[1] * a[1]);
	a[1] = (atan2(a[1], a_0));
}

int clamp(int x, int min, int max) {
	if (x < min) {
		return min;
	}
	else if (x > max) {
		return max;
	}
	else {
		return x;
	}
}

std::array<unsigned char, 4> Crgb(std::array<float, 2>& a, float strength, float range) {
	std::array<unsigned char, 4> rgb;
	Cr_th(a);
	std::array<float, 2> r_th = a;

	float th = (float)r_th[1];
	float l = (float)r_th[0];


	if (th < -pi / 3) {
		rgb[0] = (clamp(((-3.0f * th / (2.0f * pi) - 1.5f + 1.0f)* tanh(l* strength) + tanh(l / range)) * 255, 0, 255));
		rgb[1] = (clamp(((3.0f * th / (2.0f * pi) + 1.5f) * tanh(l * strength) + tanh(l / range)) * 255, 0, 255));
		rgb[2] = (clamp(tanh(l / range) * 255, 0, 255));
		rgb[3] = (255);
	}
	else if (th > -pi / 3 && th < pi / 3) {
		rgb[0] = (clamp(tanh(l / range) * 255, 0, 255));
		rgb[1] = (clamp(((-3.0f * th / (2.0 * pi) - 0.5f + 1.0f) * tanh(l * strength) + tanh(l / range)) * 255, 0, 255));
		rgb[2] = (clamp(((3.0f * th / (2.0f * pi) + 0.5f) * tanh(l * strength) + tanh(l / range)) * 255, 0, 255));
		rgb[3] = (255);
	}
	else {
		rgb[0] = (clamp(((3.0f * th / (2.0f * pi) - 0.5f) * tanh(l * strength) + tanh(l / range)) * 255, 0, 255));
		rgb[1] = (clamp(tanh(l / range) * 255, 0, 255));
		rgb[2] = (clamp(((-3.0f * th / (2.0f * pi) + 0.5f + 1.0f) * tanh(l * strength) + tanh(l / range)) * 255, 0, 255));
		rgb[3] = (255);
	}
	return rgb;
}

void createPNG(const std::vector<unsigned char>& image, unsigned width, unsigned height, const std::string& filename) {
	unsigned error = lodepng::encode(filename, image, width, height);

	if (error) {
		std::cout << "PNG encoding error: " << lodepng_error_text(error) << std::endl;
	}
	else {
		std::cout << "PNG created successfully: " << filename << std::endl;
	}
}