#include "LogWriter.h"

LogWriter::LogWriter(std::string filename, int width, int height)
    : encodedImage(0),
      filename(filename),
      numFrames(0),
      compressed(true),
      width(width),
      height(height)
{
    assert( logFile = fopen(filename.c_str(), "wb+") );
    fwrite(&numFrames, sizeof(int32_t), 1, logFile);
    std::cout << filename << " opened." << std::endl;
    std::cout << "numFrames: " << numFrames << std::endl;
}

LogWriter::~LogWriter()
{
    // free(depth_compress_buf);
    if (encodedImage != 0) {
        cvReleaseMat(&encodedImage);
    }
}

void LogWriter::stopWriting()
{
    fseek(logFile, 0, SEEK_SET);
    fwrite(&numFrames, sizeof(int32_t), 1, logFile);
    std::cout << "numFrames is " << numFrames << std::endl;
    std::cout << filename << " saved." << std::endl;

    fflush(logFile);
    fclose(logFile);
}

void LogWriter::increaseNumFrames()
{
    this->numFrames++;
}

void LogWriter::encodeJpeg(cv::Vec<unsigned char, 3> * rgb_data)
{
    cv::Mat3b rgb(height, width, rgb_data, width * 3);
    IplImage * img = new IplImage(rgb);
    int jpeg_params[] = {CV_IMWRITE_JPEG_QUALITY, 90, 0};

    if (encodedImage != 0) {
        cvReleaseMat(&encodedImage);
    }
    encodedImage = cvEncodeImage(".jpg", img, jpeg_params);

    delete img;
}

void LogWriter::logData(int64_t * timestamp,
                        int32_t * depthSize,
                        int32_t * rgbSize,
                        unsigned char * depthData,
                        unsigned char * rgbData)
{
    fwrite(timestamp, sizeof(int64_t), 1, logFile);
    fwrite(depthSize, sizeof(int32_t), 1, logFile);
    fwrite(rgbSize, sizeof(int32_t), 1, logFile);
    fwrite(depthData, *depthSize, 1, logFile);
    fwrite(rgbData, *rgbSize, 1, logFile);
}
