#pragma once

#include <fstream>
#include <windows.h>

#include "YUVPixel.h"

class VideoService
{
public:
	FILE* videoFile;

	bool open(const char* FileName, 
		      std::vector<YUVPixel> YUVPixels,
			  int videoWidth, 
			  int videoHeight, 
			  int imageWidth, 
			  int imageHeight);
};