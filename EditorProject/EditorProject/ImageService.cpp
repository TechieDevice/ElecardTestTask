#include "ImageService.h"

const INT16 coeffMatrix[16] = {  66, 129, 25, -38, -74, 112, 112, -94,
							   -18,   0,  0,   0,   0,   0,   0,   0};
const __m256i m256iCoeff = *(__m256i*)coeffMatrix;

const INT16 addMatrix[16] = {128, 0, 0, 128, 0, 0, 128, 0,
							   0, 0, 0,   0, 0, 0,   0, 0};
const __m256i m256iAdd = *(__m256i*)addMatrix;


void RGBtoYUV(int i, 
			  std::vector<byte> &pixels,
			  int from,
			  int to,
			  std::vector<YUVPixel> &YUVPixels)
{
	for (int p = from; p < (to); p += 3)
	{
		byte pixelMatrix[32] = {0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0, 
								0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0 };
		INT16 i16pixel[3] = { (pixels)[p + 2], (pixels)[p + 1], (pixels)[p] };
		memcpy(&pixelMatrix[0], (byte*)i16pixel, sizeof(INT16) * 3);
		memcpy(&pixelMatrix[6], (byte*)i16pixel, sizeof(INT16) * 3);
		memcpy(&pixelMatrix[12], (byte*)i16pixel, sizeof(INT16) * 3);

		__m256i aPixel = *(__m256i*)pixelMatrix;

		__m256i resMul = _mm256_mullo_epi16(aPixel, m256iCoeff);
		__m256i res = _mm256_add_epi16(resMul, m256iAdd);

		INT16 Y = res.m256i_i16[0] + res.m256i_i16[1] + res.m256i_i16[2];
		INT16 U = res.m256i_i16[3] + res.m256i_i16[4] + res.m256i_i16[5];
		INT16 V = res.m256i_i16[6] + res.m256i_i16[7] + res.m256i_i16[8];

		Y = Y >> 8;
		U = U >> 8;
		V = V >> 8;

		YUVPixels[i].Y = (byte)Y;
		YUVPixels[i].U = (byte)(U + 128);
		YUVPixels[i].V = (byte)(V + 128);
		i++;
	}
}

bool ImageService::open(const char* FileName)
{
	if (FileName == NULL) return false;
	fopen_s(&imageFile, FileName, "rb");
	if (imageFile == 0) return false;

	fseek(imageFile, 0, SEEK_SET);

	char sign[2];
	fread(&sign, sizeof(char), 2, imageFile);
	if (memcmp(sign, "BM", 2)) 
	{
		fclose(imageFile);
		return false;
	}
		
	int pixelsPos;
	fseek(imageFile, 10, SEEK_SET);
	fread(&pixelsPos, sizeof(int), 1, imageFile);

	fseek(imageFile, 4, SEEK_CUR);
	fread(&width, sizeof(int), 1, imageFile);

	fread(&height, sizeof(int), 1, imageFile);

	int padding = (width * 3) % 4;
	int widthSeparator = (width + padding) / 4;

	YUVPixels = std::vector<YUVPixel>(width * height);
	int i = 0;

	fseek(imageFile, pixelsPos, SEEK_SET);

	std::vector<std::thread> threads;
	std::vector<byte> _pixelsBuf;
	for (int y = 0; y < height; y++)
	{
		i = y * width;
		std::vector<byte> pixels(3 * width);
		fread(pixels.data(), sizeof(byte), (3 * width), imageFile);
		
		for (int t = 0; t < threads.size(); t++)
		{
			threads[t].join();
		}
		threads.clear();

		_pixelsBuf.clear();
		std::copy(pixels.begin(), pixels.end(), back_inserter(_pixelsBuf));
		
		for (int t = 0; t < 4; t++)
		{
			int from = widthSeparator * t;
			int to = t == 3 ? width : widthSeparator * (t + 1);
			threads.push_back(std::thread(RGBtoYUV,
										  i + from,
										  std::ref(_pixelsBuf),
										  from * 3, 
										  to * 3,
										  std::ref(YUVPixels)));
		}
		fseek(imageFile, padding, SEEK_CUR);
	}
	for (int t = 0; t < threads.size(); t++)
	{
		threads[t].join();
	}
	threads.clear();

	fclose(imageFile);
	return true;
}
