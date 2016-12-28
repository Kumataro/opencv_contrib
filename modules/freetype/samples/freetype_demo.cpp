//M*//////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>
using namespace cv;

int main (int argc, char **argv){
    if (argc == 1){
        fprintf(stderr,"usage: %s fontfile\n", argv[0]);
        return 1; 
    }
    cv::Ptr < cv::freetype::FreeType2 > ft2 = cv::freetype::createFreeType2 ();

    ft2->loadFontData (argv[1], 0);
    ft2->setSplitNumber (8);
    cv::Mat src = cv::Mat::ones (3200, 3200, CV_8UC3) * 128;

    int hList[] = { 8, 12, 16, 20, 24, 28, 32 };
    int lList[] = { 4, 8, 16 };

    cv::Scalar col (255, 255, 0);
    int px = 0;
    for (unsigned int h = 0; h < sizeof (hList) / sizeof (hList[0]); h++){
        int fontHeight = hList[h];
        int py = 0;
        for (unsigned int lineWidth = -1; lineWidth < 4; lineWidth++){
            for (unsigned int l = 0; l < sizeof(lList)/sizeof(lList[0]);l++){
                int lineStyle = lList[l];
                if (lineWidth == 0) { continue; }

                for (int c = 0; c <= 0x7F - 0x20; c++){
                    cv::String text = "0";
                    text = (char)c + 0x20;
                    ft2->putText (src,
                    text,
                    cv::Point (
                        px + (c % 16) * fontHeight,
                        py + (c / 16) * fontHeight
                    ),
                    fontHeight, col, lineWidth, lineStyle, false);
                }
                char buf[128];
                sprintf(buf, "sz=%d,th=%d,st=%d", 
                    fontHeight, lineWidth, lineStyle);
                ft2->putText (src,
                    buf,
                    cv::Point (px,
                    py + 6 * fontHeight),
                    fontHeight, col, lineWidth, lineStyle, false);
                py = py + (fontHeight * 8);
            }
        }
        px = px + (fontHeight * 18);
    }
    cv::imwrite ("output.png", src);
    return 0;
}
