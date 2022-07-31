#pragma once

#include <fstream>
#include <windows.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <immintrin.h>

#include "YUVPixel.h"

class ImageService
{
public:
	std::mutex mutex;
	std::condition_variable cv;
	bool write_ready = true;
	FILE* imageFile;
	std::vector<YUVPixel> YUVPixels;
	int width;
	int height;

	bool open(const char* FileName);
};
