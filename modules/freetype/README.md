# FreeType Module

This FreeType module allows you to draw UTF-8 strings with outlines or bitmaps.

## Requested Libraries
harfbuzz https://www.freedesktop.org/wiki/Software/HarfBuzz/

License information is https://raw.githubusercontent.com/behdad/harfbuzz/master/COPYING .

freetype https://www.freetype.org/ 

License information is https://www.freetype.org/license.html .

## Usage
    // Create ft2 instance
    cv::Ptr < cv::freetype::FreeType2 > ft2 = cv::freetype::createFreeType2 ();
    
    // load Font Data from filesystem.
    // 2nd argument is the index of a font data in a font file.
    ft2->loadFontData ("FONTDATA.TTF", 0);
    
    // Set split Number to cubic/conic curve (Option)
    ft2->setSplitNumber (8);
    
    cv::Mat src = cv::Mat::zeros (480, 640, CV_8UC3);
    
    // Draw UTF-8 string.
    ft2->putText (src,                      // Destination
                  "Hello, World!",          // UTF-8 String
                  cv::Point(30,30),         // Origin
                  80,                       // Height
                  cv::Scalar(255,255,255),  // Color
                  -1,                       // LineWidth
                  16,                       // LineStyle
                  false);                   // Is Origin is LeftBottom 

    cv::imwrite ("output.png", src);
    return 0;

### Drawing Style (Bitmap/Outline)

| lineWidth | lineStyle |      Result       |
|:---------:|:---------:|:-----------------:|
|    >=0    | 4/8/16    |Outline            |
|    \<0    | 4/8       |Bitmap with 1bit   |
|    \<0    | 16        |Bitmap with 8bit   |

## Future work
- Add test (CJK)
- Validation UTF-8 strings.
- Get a string BBox.
- Load a fontdata on memory.
- Support freetype/harfbuzz libraries which are not managed with pkg-config.
- Support Windows with NuGet Environment ( Optional )
- Glyph Cache ( Optional )
- Support text-direction option (Top to Bottom, Right to Left ...) ( Optional )
