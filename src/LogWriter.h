#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <string>
#include <iostream>
#include <stdio.h>

#include <inttypes.h>
#include <opencv2/opencv.hpp>

#include "zlib.h"
#include "JPEGLoader.h"

class LogWriter
{
public:
    LogWriter(std::string filename, int width, int height);
    virtual ~LogWriter();

    // void startWriting(std::string filename);
    void increaseNumFrames();
    void stopWriting();
    void encodeJpeg(cv::Vec<unsigned char, 3> * rgb_data);
    void logData(int64_t * timestamp,
                 int32_t * depthSize,
                 int32_t * rgbSize,
                 unsigned char * depthData,
                 unsigned char * rgbData);

    CvMat * encodedImage;
private:
    int depth_compress_buf_size;
    uint8_t * depth_compress_buf;

    std::string filename;
    FILE * logFile;
    int32_t numFrames;
    bool compressed;

    int width;
    int height;


};

#endif // LOGWRITER_H
