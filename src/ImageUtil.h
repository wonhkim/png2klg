#include <cassert>

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/gpu/gpu.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/contrib/contrib.hpp>



namespace util
{
    enum IMAGE_TYPE {COLOR, DEPTH};

    // basic image I/O
    void read(std::string filename, const IMAGE_TYPE image_type, cv::Mat &image);
    void display(std::string windowname, cv::Mat image, int delay);
    void display(std::string windowname, cv::Mat image);
    void display(std::string windowname, cv::Mat im1, cv::Mat im2, int delay); 
    void display(std::string windowname, cv::Mat im1, cv::Mat im2, cv::Mat im3, int delay);
}
