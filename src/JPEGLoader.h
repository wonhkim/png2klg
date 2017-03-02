#ifndef JPEGLOADER_H
#define JPEGLOADER_H

//extern "C"
//{
//#include "jpeglib.h"
//}

#include "jpeglib.h"

#include <stdio.h>
#include <string>

static void jpegFail(j_common_ptr cinfo)
{
    assert(false && "JPEG decoding error!");
}

static void doNothing(j_decompress_ptr)
{

}

class JPEGLoader
{
    public:
        JPEGLoader()
        {}

        void readData(unsigned char * src, const int numBytes, unsigned char * data)
        {
            jpeg_decompress_struct cinfo; // IJG JPEG codec structure

            jpeg_error_mgr errorMgr;

            errorMgr.error_exit = jpegFail;

            cinfo.err = jpeg_std_error(&errorMgr);

            jpeg_create_decompress(&cinfo);

            jpeg_source_mgr srcMgr;

            cinfo.src = &srcMgr;

            // Prepare for suspending reader
            srcMgr.init_source = doNothing;
            srcMgr.resync_to_restart = jpeg_resync_to_restart;
            srcMgr.term_source = doNothing;
            srcMgr.next_input_byte = src;
            srcMgr.bytes_in_buffer = numBytes;

            jpeg_read_header(&cinfo, TRUE);

            jpeg_calc_output_dimensions(&cinfo);

            jpeg_start_decompress(&cinfo);

            int width = cinfo.output_width;
            int height = cinfo.output_height;

            JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, width * 4, 1);

            for(; height--; data += (width * 3))
            {
                jpeg_read_scanlines(&cinfo, buffer, 1);

                unsigned char * bgr = (unsigned char *)buffer[0];
                unsigned char * rgb = (unsigned char *)data;

                for(int i = 0; i < width; i++, bgr += 3, rgb += 3)
                {
                    unsigned char t0 = bgr[0], t1 = bgr[1], t2 = bgr[2];
                    rgb[2] = t0; rgb[1] = t1; rgb[0] = t2;
                }
            }

            jpeg_finish_decompress(&cinfo);

            jpeg_destroy_decompress(&cinfo);
        }
};

#endif // JPEGLOADER_H
