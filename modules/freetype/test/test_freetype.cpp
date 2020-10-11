// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"
#include <iostream>

namespace opencv_test { namespace {

static const int MAX_THICKNESS = 32767; // from modules/imgproc/src/drawing.cpp

using namespace cv::freetype;


TEST(CV_freetype_loadFontData, testFailedMissingFile){
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();

    EXPECT_ANY_THROW ( ft2->loadFontData("MISSING_FILE", 0 ) );
}

TEST(CV_freetype_loadFontData, testFailedZeroFilled){
    std::string path;

    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();

    // zero-filled file
    path  = cvtest::findDataFile("freetype/BAD/zerofill.otf", false );
    EXPECT_ANY_THROW  ( ft2->loadFontData(path, 0 ) );
}

TEST(CV_freetype_loadFontData, testFailedZeroLength){
    std::string path;

    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();

    // zero-length file
    path = cvtest::findDataFile("freetype/BAD/zerosize.otf", false );
    EXPECT_ANY_THROW  ( ft2->loadFontData(path, 0 ) );
}

TEST(CV_freetype_loadFontData, testFailedInvalidId){
    std::string path;

    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();

    vector<int> idxInvalidList = {
        INT_MIN,
        INT_MIN + 1,
        -1,
        1,
        INT_MAX - 1,
        INT_MAX
    };

    // Call with Invalid id
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );

    for ( const auto& iter : idxInvalidList ) {
        EXPECT_ANY_THROW  ( ft2->loadFontData(path, iter) );
    }
}

TEST(CV_freetype_loadFontData, testSuccessCallManyTime){
    std::string path;

    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();

    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );

    // Moultiple call is ok.
    for ( int i = 0 ; i < 10 ; i ++ ) {
        EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );
    }
}

TEST(CV_freetype_setSplitNumber, testNumRange ){
    std::string path;

    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();

    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );

    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    vector<int> splitNumberInvalidList = {
        INT_MIN,
        INT_MIN + 1,
        -1,
        0,
    };
    vector<int> splitNumberValidList = {
        1,
        INT_MAX - 1,
        INT_MAX,
    };

    for( const auto &iter : splitNumberInvalidList ) {
        EXPECT_ANY_THROW ( ft2->setSplitNumber( iter ) );
    }
    for( const auto &iter : splitNumberValidList ) {
        EXPECT_NO_THROW ( ft2->setSplitNumber( iter ) );
    }

}

TEST(CV_freetype_putText_Common, testFailedInvalidFace ){
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();

    // ft2->loadFontData() is missing.

    Mat mat = Mat::zeros(640,480,CV_8UC3);
    EXPECT_ANY_THROW (
        ft2->putText(
            mat,
            "TEST",
            Point(0,0),
            20,
            Scalar::all(0),
            -1,
            4,
            false )  );
}

TEST(CV_freetype_putText_Common, testForDestMat ){
    std::string path;
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );
    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    Mat mat; // Empty
    EXPECT_ANY_THROW (
        ft2->putText(
            mat,
            "TEST",
            Point(0,0),
            20,
            Scalar::all(0),
            -1,
            4,
            false )  );


    // _img should be mat.
    EXPECT_ANY_THROW (
        ft2->putText(
            Scalar(0,0,0),
            "TEST",
            Point(0,0),
            20,
            Scalar::all(0),
            -1,
            4,
            false )  );

    // _img should be CV_8UC3
    vector<int> mattypesList = {
        CV_8UC1,  CV_8UC2,            CV_8UC4,
        CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
        CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
        CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
        CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
        CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
        CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4,
    };

    for ( const auto& iter : mattypesList ) {
        mat = Mat::zeros(1,1, iter );
        EXPECT_ANY_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                20,
                Scalar(0,0,0),
                -1,
                4,
                false )  ) ;
    }

    mat = Mat::zeros(1,1, CV_8UC3 );
    EXPECT_NO_THROW (
        ft2->putText(
            mat,
            "TEST",
            Point(0,0),
            20,
            Scalar(0,0,0),
            -1,
            4,
            false )  );
}

TEST(CV_freetype_putText_Common, testForLineType ){
    std::string path;
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );
    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    vector<int> lineTypesValidList = {
        4, 8, 16,
    };
    vector<int> lineTypesInvalidList = {
        INT_MIN, 3, 5, 7, 9, 15, 17, INT_MAX
    };

    for ( const auto& iter : lineTypesValidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_NO_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                20,
                Scalar(0,0,0),
                -1,
                iter,
                false )  );
    }
    for ( const auto& iter : lineTypesInvalidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_ANY_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                20,
                Scalar(0,0,0),
                -1,
                iter,
                false )  );
    }
}

TEST(CV_freetype_putText_Mono, testForFontHeight ){
    std::string path;
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );
    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    vector<int> fontHeightInvalidList = {
        INT_MIN,     // minimun
        INT_MIN + 1, // minimum + 1
        -1,          // negative

        65536,       // too large.
        INT_MAX -1,  // maxmum - 1
        INT_MAX,     // maxmum
    };

    vector<int> fontHeightValidList = {
        0,   // OK
        1,   // Valid minimum
        8,
        16,
        32,
        64,
        128,
        256,
        512,
        1024,
        2048,
        4096,
        8192,
        16348,
        32768,
    };

    for ( auto iter : fontHeightInvalidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_ANY_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                iter,
                Scalar(0,0,0),
                -1, // Filled
                4,  // Mono
                false )  );
    }

    for ( auto iter : fontHeightValidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_NO_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                iter,
                Scalar(0,0,0),
                -1, // Filled
                4,  // Mono
                false )  );
    }
}

TEST(CV_freetype_putText_Blend, testForFontHeight ){
    std::string path;
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );
    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    vector<int> fontHeightInvalidList = {
        INT_MIN,     // minimun
        INT_MIN + 1, // minimum + 1
        -1,          // negative

        65536,       // too large.
        INT_MAX -1,  // maxmum - 1
        INT_MAX,     // maxmum
    };

    vector<int> fontHeightValidList = {
        0,   // OK
        1,   // Valid minimum
        8,
        16,
        32,
        64,
        128,
        256,
        512,
        1024,
        2048,
        4096,
        8192,
        16348,
        32768,
    };

    for ( const auto& iter : fontHeightInvalidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_ANY_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                iter,
                Scalar(0,0,0),
                -1, // Filled
                16, // Blend
                false )  );
    }

    for ( const auto& iter : fontHeightValidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_NO_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                iter,
                Scalar(0,0,0),
                -1, // Filled 
                16, // Blend
                false )  );
    }
}

TEST(CV_freetype_putText_Outline, testForFontHeight ){
    std::string path;
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );
    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    vector<int> fontHeightInvalidList = {
        INT_MIN,     // minimun
        INT_MIN + 1, // minimum + 1
        -1,          // negative
    };

    vector<int> fontHeightValidList = {
        0,   // OK
        1,   // Valid minimum
        8,
        16,
        32,
        64,
        128,
        256,
        512,
        1024,
        2048,
        4096,
        8192,
        16348,
        32768,
        INT_MAX -1,  // maxmum - 1
        INT_MAX,     // maxmum
    };

    for ( const auto& iter : fontHeightInvalidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_ANY_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                iter,
                Scalar(0,0,0),
                1, // Not Filled(Outline)
                4,
                false )  );
    }

    for ( const auto& iter : fontHeightValidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_NO_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                iter,
                Scalar(0,0,0),
                1, // Not Filled(Outline)
                4,
                false )  );
    }
}

TEST(CV_freetype_putText_Outline, testForColor ) {
    std::string path;
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );
    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    vector<Scalar> colorInvalidList = {
    };

    vector<Scalar> colorValidList = {
        Scalar(0,0,0),
        Scalar(128,0,0),
        Scalar(255,0,0),
        Scalar(0,128,0),
        Scalar(0,255,0),
        Scalar(0,0,128),
        Scalar(0,0,255),
        Scalar(0,0,255),
        Scalar(255,255,255),
        Scalar::all(0),
        Scalar::all(255),
    };

    for ( const auto& iter : colorInvalidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_ANY_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                20,
                iter,
                1, // Not Filled(Outline)
                4,
                false )  );
    }

    for ( const auto& iter : colorValidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_NO_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                20,
                iter,
                1, // Not Filled(Outline)
                4,
                false )  );
    }
}

TEST(CV_freetype_putText_Outline, testForThickness ){
    std::string path;
    Ptr<freetype::FreeType2> ft2 = freetype::createFreeType2();
    path = cvtest::findDataFile("freetype/063-OTF/mplus-1c-medium.ttf", false );
    EXPECT_NO_THROW  ( ft2->loadFontData(path, 0) );

    // if thickness<0 -> call putTextBitmapMono() or putTextBitmapBlend()

    vector<int> thicknessInvalidList = {
        MAX_THICKNESS + 1, // 32768
        INT_MAX -1,        // maxmum - 1
        INT_MAX,           // maxmum
    };

    vector<int> thicknessValidList = {
        0,   // OK
        1,   // Valid minimum
        8,
        16,
        32,
        64,
        128,
        256,
        512,
        1024,
        2048,
        4096,
        8192,
        16348,
        MAX_THICKNESS-1,
        MAX_THICKNESS, // 32767
    };

    for ( const auto& iter : thicknessInvalidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_ANY_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                60,
                Scalar(0,0,0),
                iter,
                4,
                false )  );
    }

    for ( const auto& iter : thicknessValidList ) {
        Mat mat = Mat::zeros(1,1, CV_8UC3 );
        EXPECT_NO_THROW (
            ft2->putText(
                mat,
                "TEST",
                Point(0,0),
                60,
                Scalar(0,0,0),
                iter,
                4,
                false )  );
    }
}

} // namespace
} // namespace opencv_test
