#include "ImageUtil.h"

namespace util
{

    void read(std::string filename, const IMAGE_TYPE image_type, cv::Mat &image)
    {
        switch (image_type)
        {
            case COLOR: 
                image = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
                assert(image.type() == CV_8UC3);
                break;

            case DEPTH:
                image = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
                assert(image.type() == CV_16UC1);
                // If you want to convert TUM RGB-D frames into klg file for using ElasticFusion,
                // uncomment the following 2 lines because
                // depth factor for TUM is 5000
                // while ElasticFusion use 1000 as a depth factor.
                // image.convertTo(image, CV_32FC1);
                // image.convertTo(image, CV_16UC1, 1/5.0f);
                break;
            
            default:
                std::cerr << "Invalid image type!\n";
                std::exit(EXIT_FAILURE);
                break;
        }
        if (!image.data) {
            std::cerr << "Unable to open an image!\n";
            std::exit(EXIT_FAILURE);
        }
    }

    void display(std::string windowname, const cv::Mat image, int delay)
    {
        cv::namedWindow(windowname, CV_WINDOW_FREERATIO);
        cv::imshow(windowname, image);
        cv::waitKey(delay);
    }

    void display(std::string windowname, cv::Mat image)
    {
        display(windowname, image, 0);
    }

    void display(std::string windowname, cv::Mat im1, cv::Mat im2, int delay)
    {
        cv::Mat imageConcat;
        cv::hconcat(im1, im2, imageConcat); 
        display(windowname, imageConcat, delay);
    }

    void display(std::string windowname, cv::Mat im1, cv::Mat im2, cv::Mat im3, int delay)
    {
        cv::Mat imageConcat;
        cv::hconcat(im1, im2, imageConcat);
        cv::hconcat(imageConcat, im3, imageConcat);
        display(windowname, imageConcat, delay);
    }

}
