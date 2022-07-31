#include "VideoService.h"

bool VideoService::open(const char* FileName, 
						std::vector<YUVPixel> YUVPixels,
						int videoWidth, 
						int videoHeight, 
						int imageWidth, 
						int imageHeight)
{
	if (FileName == NULL) return false;
	fopen_s(&videoFile, FileName, "r+b");
	if(videoFile == 0) return false;

	_fseeki64(videoFile, 0, SEEK_END);
	size_t size = _ftelli64(videoFile);
	_fseeki64(videoFile, 0, SEEK_SET);

	int paddingWidth = videoWidth - imageWidth;
	int paddingHeight = videoWidth * (videoHeight - imageHeight);
	int frames = (int)(size / (videoHeight * videoWidth * 1.5));

	for (int f = 0; f < frames; f++)
	{
		for (int y = imageHeight; y > 0; y--)
		{
			for (int x = 0; x < imageWidth; x++)
			{
				int i = (y - 1) * imageWidth + x;
				fwrite(&YUVPixels[i].Y, sizeof(byte), 1, videoFile);
			}
			_fseeki64(videoFile, paddingWidth, SEEK_CUR);
		}
		_fseeki64(videoFile, paddingHeight, SEEK_CUR);
		for (int y = imageHeight; y > 0; y += -2)
		{
			for (int x = 0; x < imageWidth; x += 2)
			{
				int i = (y - 1) * imageWidth + x;
				fwrite(&YUVPixels[i].U, sizeof(byte), 1, videoFile);
			}
			_fseeki64(videoFile, paddingWidth / 2, SEEK_CUR);
		}
		_fseeki64(videoFile, paddingHeight / 4, SEEK_CUR);
		for (int y = imageHeight; y > 0; y += -2)
		{
			for (int x = 0; x < imageWidth; x += 2)
			{
				int i = (y - 1) * imageWidth + x;
				fwrite(&YUVPixels[i].V, sizeof(byte), 1, videoFile);
			}
			_fseeki64(videoFile, paddingWidth / 2, SEEK_CUR);
		}
		_fseeki64(videoFile, paddingHeight / 4, SEEK_CUR);
	}

	fclose(videoFile);
	return true;
}
