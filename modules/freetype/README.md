# FreeType Module

This FreeType module allows you to draw strings with outlines and bitmaps.

# Installation
- harfbuzz is requested to convert UTF8 to gid(GlyphID).
  - harfbuzz https://www.freedesktop.org/wiki/Software/HarfBuzz/
- freetype library is requested to rasterize given gid.
  - freetype https://www.freetype.org/

# Usage

```
cv::Ptr<cv::freetype::FreeType2> ft2;
ft2->loadFontData("your-font.ttf", 0);
ft2->setSplitNumber( 4 ); // Bezier-line is splited by 4 segment.
ft2->putText(dst, .... )
```

## Limitation
- A destination mat must be 8UC3.

## Option
- 2nd argument of loadFontData is used if font file has many font data.
- Drawing glyphs as Outlines is supprted.
  - thickness must be >=0.
  - thickness and linestyle is used to drawing outlines.
- Drawing glyphs as 1 bit bitmaps is supported.
  - thickness must be < 0(Usually set -1).
  - line_style != 16(CV_AA).
- Drawing glyphs as Gray(8bit) bitmaps is supported.
  - thickness must be < 0(Usually set -1).
  - line_style == 16(CV_AA).

