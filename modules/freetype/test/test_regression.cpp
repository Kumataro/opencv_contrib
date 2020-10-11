// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"

// Test Module Configuration
//
// OUTPUT_IMAGE_TO_UPDATE
//   Output "*.png" to update refence images if enabled.
//   ( And one test will be failed automatically).
// Copy those images into <OPENCV_EXTRA>/cv/testdata/freetype/reference/
// #define OUTPUT_IMAGES_TO_UPDATE

namespace opencv_test { namespace {

class FreeTypeRegressionTest : public cvtest::BaseTest
{
public:
    FreeTypeRegressionTest():
        mTestName( "dmy" ),
        mThickness( -1 ),
        mImgSize( 1, 1 )
    {
        mFontNameList.clear();
        mFontHeightList.clear();
        mTextList.clear();
    }

public: // Setter
    void setTestName ( String _testName );
    void setFontNameList ( vector<String> _fontNameList );
    void setFontHeightList( vector<int> _fontHeightList );
    void setThickness( int _thickness );
    void setImgSize( Size _imgsize );
    void setText( String _text );
    void setText( vector<String> _textList );

protected:
    void run(int);

private: // const definitions.
    const int      mDilateCount = 1;
    const Point    mTextShift   = Point ( 0, 0 );

private: // member variables.
    String         mTestName;
    vector<String> mFontNameList;
    int            mThickness;
    Size           mImgSize;
    vector<int>    mFontHeightList;
    vector<String> mTextList;

private: // helper functions.
    void   runSub(String _fontName);
    String createRefPath( String _fontFileName );

public:
    String cvtUTF32toUTF8_simple( uint32_t _code);
};

void FreeTypeRegressionTest::setTestName( String _testName ){
    mTestName = _testName;
}
void FreeTypeRegressionTest::setThickness( int _thickness ){
    mThickness = _thickness;
}
void FreeTypeRegressionTest::setImgSize( Size _imgsize ){
    mImgSize = _imgsize;
}
void FreeTypeRegressionTest::setText( String _text ){
    mTextList.clear();
    mTextList.push_back ( _text );
}
void FreeTypeRegressionTest::setFontHeightList(vector<int> _fontHeightList){
    mFontHeightList = _fontHeightList;
}
void FreeTypeRegressionTest::setText( vector<String> _textList ){
    mTextList = _textList;
}
void FreeTypeRegressionTest::setFontNameList( vector<String> _fontNameList ){
    mFontNameList = _fontNameList;
}
String FreeTypeRegressionTest::cvtUTF32toUTF8_simple( uint32_t _code){
    if ( _code <= 0x7f ){
        return cv::format("%c", (uint8_t) _code );
    }

    if ( _code <= 0x7ff ) {
        uint8_t oUpper = 0xC0;
        uint8_t oLower = 0x80;
        oUpper = oUpper | ( ( _code >> 6 ) & 0x1F );
        oLower = oLower | ( ( _code      ) & 0x3F );
        return cv::format("%c%c", oUpper,oLower );
    }

    if ( _code <= 0xffff ) {
        uint8_t oUpper  = 0xE0;
        uint8_t oMiddle = 0x80;
        uint8_t oLower  = 0x80;
        oUpper  = oUpper  | ( ( _code >> 12 ) & 0x0F );
        oMiddle = oMiddle | ( ( _code >>  6 ) & 0x3F );
        oLower  = oLower  | ( ( _code       ) & 0x3F );

        return cv::format("%c%c%c", oUpper,oMiddle,oLower );
    }
  
    return "NG";
}

String FreeTypeRegressionTest::createRefPath(String _fontFileName){
    String oBasePath = "";

    oBasePath += mTestName + "_" + _fontFileName + "_";

    if ( mThickness < 0 ) {
        oBasePath += "fill";
    }else{
        oBasePath += cv::format("%d", mThickness );
    }

    // fontFileName includes direcotry path.
    // Convert to flatten file path.
    String oRetPath = "";
    for ( const auto& iter : oBasePath ){
        if( isalpha(iter) || isdigit(iter) ){
            oRetPath += iter;
        }else{
            oRetPath += "_";
        }
    }

    // Add ".png".
    oRetPath+=".png";

    // return it.
    return oRetPath;
}

void FreeTypeRegressionTest::runSub( String _fontName ){
    // Show Parameters
    String oParamLog = "" ;
    oParamLog += "_fontName = " + _fontName ;

    // Initilize module.
    Ptr<freetype::FreeType2> ft2;
    EXPECT_NO_THROW ( ft2 = freetype::createFreeType2() );

    // Load font file
    std::string oFilePath = "";
    EXPECT_NO_THROW ( oFilePath = cvtest::findDataFile( _fontName, false ) );
    EXPECT_NO_THROW ( ft2->loadFontData(oFilePath, 0) );

    // Make reference image filename
    String oReferenceImageBasePath = "";
    EXPECT_NO_THROW (
        oReferenceImageBasePath = createRefPath( _fontName )
    );

    // Create output buffer
    Mat mat = Mat::zeros( mImgSize, CV_8UC3 );

    // Initilize position and shift.
    Point oOrg   = Point(10,10);

    // Iteration it
    for ( const auto& iterFontHeight : mFontHeightList ) {
        for ( const auto& iterText : mTextList ) {
            String oText = "";
            if ( mFontHeightList.size() > 1 ) {
                oText = cv::format("%d pt:", iterFontHeight);
            }
            oText += iterText;

            // Draw text into mat.
            vector <int> oLineType = { 4, 8, 16 /* CV_AA */ };
            for (const auto& iterLineType : oLineType ){
                oOrg += Point(0, iterFontHeight * 1.5 );
                EXPECT_NO_THROW (
                    ft2->putText(
                        mat,
                        oText,
                        oOrg + mTextShift,
                        iterFontHeight,
                        Scalar(255,255,255),
                        mThickness,
                        iterLineType,
                        false )
                );
            }
        }
    }

#ifdef OUTPUT_IMAGES_TO_UPDATE
    // Output images if needed.
    // To reduce file size, mat is converted to gray.
    cvtColor(mat, mat, COLOR_BGR2GRAY);
    EXPECT_NO_THROW( imwrite(oReferenceImageBasePath, mat) );
#else
    // Find reference image file.
    std::string oCmpImgPath;

    EXPECT_NO_THROW(
        oCmpImgPath = cvtest::findDataFile(
            cv::format("freetype/reference/%s", oReferenceImageBasePath.c_str()),
            false )
    );

    // Compare reference image and output image.
    Mat oCmpImg;
    EXPECT_NO_THROW( oCmpImg = imread(oCmpImgPath) );
    EXPECT_NO_THROW( dilate( oCmpImg, oCmpImg, Mat(), Point(-1, -1), mDilateCount ));
    EXPECT_NO_THROW( oCmpImg = mat - oCmpImg; );

    EXPECT_PRED_FORMAT2(
        cvtest::MatComparator(0, 0),
        oCmpImg,
        Mat::zeros( oCmpImg.rows, oCmpImg.cols,CV_8UC3 )
    ) << oParamLog;
#endif
}

void FreeTypeRegressionTest::run( int )
{
    for ( const auto& iter : mFontNameList ) {
        runSub(iter);
    }
}

static void drawingRegressionTest(int _thickness ) {
    FreeTypeRegressionTest test;

    test.setTestName( "regression" );

    test.setThickness( _thickness);

    test.setImgSize( Size( 1600, 720 ) );
    // Set Font Height List
    const vector<int> fontHeightList = { 8, 10, 12, 14, 16, 18, 20, 24, };
    test.setFontHeightList( fontHeightList ) ;

    // Create Text from ascii code.
    String oText = "";
    for ( int i = 0 ; i <= 255; i ++ ){
        if ( isprint(i) ) {
            oText += cv::format("%c",i );
        }
    }
    test.setText( oText );

    const vector<String> oFontNameList = {
        "freetype/063-OTF/mplus-1c-black.ttf",
        "freetype/063-OTF/mplus-1c-bold.ttf",
        "freetype/063-OTF/mplus-1c-heavy.ttf",
        "freetype/063-OTF/mplus-1c-light.ttf",
        "freetype/063-OTF/mplus-1c-medium.ttf",
        "freetype/063-OTF/mplus-1c-regular.ttf",
        "freetype/063-OTF/mplus-1c-thin.ttf",
        "freetype/063-OTF/mplus-1m-bold.ttf",
        "freetype/063-OTF/mplus-1m-light.ttf",
        "freetype/063-OTF/mplus-1m-medium.ttf",
        "freetype/063-OTF/mplus-1m-regular.ttf",
        "freetype/063-OTF/mplus-1m-thin.ttf",
        "freetype/063-OTF/mplus-1mn-bold.ttf",
        "freetype/063-OTF/mplus-1mn-light.ttf",
        "freetype/063-OTF/mplus-1mn-medium.ttf",
        "freetype/063-OTF/mplus-1mn-regular.ttf",
        "freetype/063-OTF/mplus-1mn-thin.ttf",
        "freetype/063-OTF/mplus-1p-black.ttf",
        "freetype/063-OTF/mplus-1p-bold.ttf",
        "freetype/063-OTF/mplus-1p-heavy.ttf",
        "freetype/063-OTF/mplus-1p-light.ttf",
        "freetype/063-OTF/mplus-1p-medium.ttf",
        "freetype/063-OTF/mplus-1p-regular.ttf",
        "freetype/063-OTF/mplus-1p-thin.ttf",
    };

    test.setFontNameList( oFontNameList );

    // Test it.
    test.safe_run();
}

static void drawingLigatureTest(int _thickness ) {
    FreeTypeRegressionTest test;

    test.setTestName( "ligature" );

    test.setThickness( _thickness);

    test.setImgSize( Size( 1000, 2000 ) );

    // Set Font Height List
    const vector<int> fontHeightList = { 16, };
    test.setFontHeightList( fontHeightList ) ;

    // Create Text
    vector<String> oTextList;
    oTextList.clear();
    for ( int i = 'a' ; i <= 'z'; i ++ ){
        String oText = "";
        for ( int j = 'a' ; j <= 'z'; j ++ ){
            oText += cv::format(" %c%c",i,j );
        }
        oTextList.push_back(oText);
    }
    test.setText( oTextList );

    const vector<String> oFontNameList = {
        "freetype/063-OTF/mplus-1c-black.ttf",
        "freetype/063-OTF/mplus-1c-bold.ttf",
        "freetype/063-OTF/mplus-1c-heavy.ttf",
        "freetype/063-OTF/mplus-1c-light.ttf",
        "freetype/063-OTF/mplus-1c-medium.ttf",
        "freetype/063-OTF/mplus-1c-regular.ttf",
        "freetype/063-OTF/mplus-1c-thin.ttf",
        "freetype/063-OTF/mplus-1m-bold.ttf",
        "freetype/063-OTF/mplus-1m-light.ttf",
        "freetype/063-OTF/mplus-1m-medium.ttf",
        "freetype/063-OTF/mplus-1m-regular.ttf",
        "freetype/063-OTF/mplus-1m-thin.ttf",
        "freetype/063-OTF/mplus-1mn-bold.ttf",
        "freetype/063-OTF/mplus-1mn-light.ttf",
        "freetype/063-OTF/mplus-1mn-medium.ttf",
        "freetype/063-OTF/mplus-1mn-regular.ttf",
        "freetype/063-OTF/mplus-1mn-thin.ttf",
        "freetype/063-OTF/mplus-1p-black.ttf",
        "freetype/063-OTF/mplus-1p-bold.ttf",
        "freetype/063-OTF/mplus-1p-heavy.ttf",
        "freetype/063-OTF/mplus-1p-light.ttf",
        "freetype/063-OTF/mplus-1p-medium.ttf",
        "freetype/063-OTF/mplus-1p-regular.ttf",
        "freetype/063-OTF/mplus-1p-thin.ttf",
    };

    test.setFontNameList( oFontNameList );

    // Test it.
    test.safe_run();
}

static void drawingMultilingualTest(int _thickness ) {
    FreeTypeRegressionTest test;

    test.setTestName( "multilingual");

    test.setThickness( _thickness);

    test.setImgSize( Size( 2000, 3000 ) );

    // Set Font Height List
    const vector<int> fontHeightList = { 24, };
    test.setFontHeightList( fontHeightList ) ;

    // Create Text
    vector<String> oTextList;
    oTextList.clear();

    String oText = "";

    std::vector<std::pair<uint32_t, uint32_t>> oUnicodeMap = {
        { 0x20,  0x7E}, // Basic Latin
        { 0xA0,  0xFF}, // Latin-1 Supplement
        {0x100, 0x17F}, // Latin Extended-A
        {0x180, 0x24F}, // Latin Extended-B
        {0x250, 0x2AF}, // IPA Extensions
        {0x2b0, 0x2FF}, // Spacing Modifier Letters
        {0x370, 0x3FF}, // Greek and Coptic
        {0x400, 0x4FF}, // Cyrillic
        {0x500, 0x52F}, // Cyrillic Supplement
        {0x1E00, 0x1EFF} , // Greek Extended
    };

    for ( const auto& iter : oUnicodeMap ) {
        oText = "";
        for ( uint32_t i = iter.first ; i <= iter.second; i ++ ){
            if ( ( (i - iter.first) % 64) == 0 ) {
                oText = cv::format(" 0x%04x : ", i );
            }

            oText += " ";
            oText += test.cvtUTF32toUTF8_simple( i );

            if ( ( (i - iter.first) % 64) == 63 ) {
                oTextList.push_back(oText);
                oText = "";
            }
        }
        if ( oText != "" ) {
            oTextList.push_back(oText);
            oText = "";
        }
    }

    test.setText( oTextList );

    const vector<String> oFontNameList = {
        "freetype/063-OTF/mplus-1c-medium.ttf",
    };

    test.setFontNameList( oFontNameList );

    // Test it.
    test.safe_run();
}

//
//  TESTS
//

TEST(CV_freetype_putText_Common, testMode ){
    bool oOutputImagesTest = true;
#ifdef OUTPUT_IMAGES_TO_UPDATE
    oOutputImagesTest = false;
#endif
    ASSERT_TRUE(oOutputImagesTest) << "[NG] OUTPUT_IMAGES_TO_UPDATE is enabled.";
}

TEST(CV_freetype_putText_Outline, testRegression ){
    drawingRegressionTest( 0 );
}
TEST(CV_freetype_putText_Outline, testLigature ){
    drawingLigatureTest( 0 );
}
TEST(CV_freetype_putText_Outline, testMultilingual ){
    drawingMultilingualTest( 0 );
}

TEST(CV_freetype_putText_Mono_Blend, testRegression ){
    // Set Thickness = -1 -> filled(blend/mono)
    drawingRegressionTest(-1);
}
TEST(CV_freetype_putText_Mono_Blend, testLigature ){
    drawingLigatureTest( -1 );
}
TEST(CV_freetype_putText_Mono_Blend, testMultilingual ){
    drawingMultilingualTest( -1 );
}


} // namespace
} // namespace opencv_test
