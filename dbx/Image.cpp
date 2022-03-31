#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Image.h"

Image::Image(const char* filename, int channel_force) {
	if (read(filename, channel_force)) {
		printf("Read %s\n", filename);
		size = w * h * channels;
	}
	else {
		printf("Failed to read %s\n", filename);
	}
}

Image::~Image() {
	stbi_image_free(data);
}

bool Image::read(const char* filename, int channel_force) {
	data = stbi_load(filename, &w, &h, &channels, channel_force);
	channels = channel_force == 0 ? channels : channel_force;
	return data != nullptr;
}