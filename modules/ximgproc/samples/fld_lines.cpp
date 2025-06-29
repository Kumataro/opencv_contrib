#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
using namespace cv::ximgproc;

int main(int argc, char** argv)
{
    string in;
    CommandLineParser parser(argc, argv, "{@input|corridor.jpg|input image}{help h||show help message}");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    in = samples::findFile(parser.get<string>("@input"));

    Mat image = imread(in, IMREAD_GRAYSCALE);

    if( image.empty() )
    {
        parser.printMessage();
        return -1;
    }

    // Create FLD detector
    // Param               Default value   Description
    // length_threshold    10            - Segments shorter than this will be discarded
    // distance_threshold  1.41421356    - A point placed from a hypothesis line
    //                                     segment farther than this will be
    //                                     regarded as an outlier
    // canny_th1           50            - First threshold for
    //                                     hysteresis procedure in Canny()
    // canny_th2           50            - Second threshold for
    //                                     hysteresis procedure in Canny()
    // canny_aperture_size 3            - Aperturesize for the sobel operator in Canny().
    //                                     If zero, Canny() is not applied and the input
    //                                     image is taken as an edge image.
    // do_merge            false         - If true, incremental merging of segments
    //                                     will be performed
    int length_threshold = 10;
    float distance_threshold = 1.41421356f;
    double canny_th1 = 50.0;
    double canny_th2 = 50.0;
    int canny_aperture_size = 3;
    bool do_merge = false;
    Ptr<FastLineDetector> fld = createFastLineDetector(length_threshold,
            distance_threshold, canny_th1, canny_th2, canny_aperture_size,
            do_merge);
    vector<Vec4f> lines;

    // Because of some CPU's power strategy, it seems that the first running of
    // an algorithm takes much longer. So here we run the algorithm 5 times
    // to see the algorithm's processing time with sufficiently warmed-up
    // CPU performance.
    for (int run_count = 0; run_count < 5; run_count++) {
        double freq = getTickFrequency();
        lines.clear();
        int64 start = getTickCount();
        // Detect the lines with FLD
        fld->detect(image, lines);
        double duration_ms = double(getTickCount() - start) * 1000 / freq;
        cout << "Elapsed time for FLD " << duration_ms << " ms." << endl;
    }

    // Show found lines with FLD
    Mat line_image_fld(image);
    fld->drawSegments(line_image_fld, lines);
    imshow("FLD result", line_image_fld);
    waitKey();

    return 0;
}
