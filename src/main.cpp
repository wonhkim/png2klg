#include <cassert>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>

#include "ImageUtil.h"
#include "LogWriter.h"

int main(int argc, char ** argv)
{
    
    // Process arguments
    if (argc != 6)
    {
        std::cerr 
            << std::endl
            << "Usage: ./png2klg <in:base_dir> <in:associations.txt> <out:klg_file> <width> <height>"
            << std::endl;
        return EXIT_FAILURE;
    }
    std::string baseDir = std::string(argv[1]); 
    std::string assocFilename = std::string(argv[2]);
    std::string klgFilename = std::string(argv[3]);
    struct stat st;
    if (stat(baseDir.c_str(), &st) != 0) {
        std::cerr << baseDir << " does not exist!\n";
        return EXIT_FAILURE;
    }
    if (baseDir.find_last_of("/") != baseDir.length()-1) { baseDir += "/"; };
    if (stat(assocFilename.c_str(), &st) != 0) {
        std::cerr << assocFilename << " deos not exist!\n";
        return EXIT_FAILURE;
    }
    int w = std::atoi(argv[4]);
    int h = std::atoi(argv[5]);
    if (w <= 0 || h <= 0) {
        std::cerr << "Invalid image size.\n";
        return EXIT_FAILURE;
    }
        
    
    // Ready to write log file
    LogWriter * logwriter = new LogWriter(klgFilename, w, h);


    // Read rgb-depth associations file
    std::string line;
    std::ifstream assocFile(assocFilename);
    std::string delimiter, token;
    size_t pos = 0;
    int count = 0;
    if (assocFile.is_open())
    {
        delimiter = " ";
        while (!assocFile.eof())
        {
            std::getline(assocFile, line);

            // Parse each line with " " as a delimiter.
            // Each line MUST have 4 elements:
            //  timestamp, rgb_filename, timestamp, depth_filename
            std::stringstream ss(line);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vstrings(begin, end);
            if (vstrings.size() != 4) {
                continue;
            }
            std::string rgbFilename = baseDir + vstrings[1];
            std::string depthFilename = baseDir + vstrings[3];

            assert(logwriter);
            std::string::size_type sz;
            double ts = std::stod(vstrings[0], &sz) * 1000000.000000; // TUM format
            int64_t timestamp = (int64_t) ts;

            std::cout << "timestamp: " << timestamp << std::endl;
            std::cout << "rgb: " << rgbFilename << std::endl;
            std::cout << "depth: " << depthFilename << std::endl;
            
            
            // Read images from local PNG files
            cv::Mat rgb, depth;
            util::read(depthFilename, util::DEPTH, depth);
            util::read(rgbFilename, util::COLOR, rgb);
            cv::cvtColor(rgb, rgb, CV_RGB2BGR);
            
            // Check memory size
            size_t rgbSizeInBytes   = rgb.total() * rgb.elemSize();
            size_t depthSizeInBytes = depth.total() * depth.elemSize();
            int32_t rgbSize   = w * h * 3 * sizeof(unsigned char); // 8-bit rgb
            int32_t depthSize = w * h * 2 * sizeof(unsigned char); // 16-bit mono
            assert(rgbSize   == rgbSizeInBytes);
            assert(depthSize == depthSizeInBytes);


            logwriter->logData( &timestamp, 
                                &depthSize,
                                &rgbSize, 
                                (unsigned char*) depth.data,
                                (unsigned char*) rgb.data);
            logwriter->increaseNumFrames();
            std::cout << "Frame " << ++count << " written.\n";
            
            // visualize current frame after written to log
            cv::cvtColor(rgb, rgb, CV_BGR2RGB);
            util::display("rgb", rgb, 1);
            util::display("depth", depth, 1);



        }
        logwriter->stopWriting();
    }
    else
    {
        std::cerr << "Unable to open " << assocFilename << std::endl;
        return EXIT_FAILURE;
    }
     
    delete logwriter; 

    return 0;
}



