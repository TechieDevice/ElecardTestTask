#include "EditorMain.h"
#include "imageService.h"
#include "videoService.h"

int main()
{
    std::cout << "enter path to bmp image\n";
    ImageService imageService;
    std::string imageFileName;
    std::cin >> imageFileName;
    if (!imageService.open(imageFileName.c_str())) return -1;
    std::cout << "done\n";

    std::cout << "enter path to video\n";
    VideoService videoService;
    std::string  videoFileName;
    std::cin >> videoFileName;
    std::cout << "enter width of video\n";
    int videoWidth;
    std::cin >> videoWidth;
    std::cout << "enter height of video\n";
    int videoHeight;
    std::cin >> videoHeight;
    if (!videoService.open(videoFileName.c_str(),
                      imageService.YUVPixels, 
                      videoWidth, 
                      videoHeight,
                      imageService.width,
                      imageService.height)) return -1;
    std::cout << "done\n";

    return 0;
}
