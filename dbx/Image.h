#pragma once

#include <cstdint>

struct Image {

public:
	BYTE* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;

public:
	Image(const char* filename, int channel_force = 0);
	~Image();

	bool read(const char* filename, int channel_force = 0);
};
