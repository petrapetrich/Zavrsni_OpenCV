#include "opencv2/highgui/highgui.hpp"
#include <cv.h>

#include <stdio.h>

using namespace cv;
using namespace std;

static void help()
{
    printf("\nPerica perich i makija makich \n"
           "    /.Program [video name]\n\n");
}

int main( int argc, const char** argv )
{
    help();
    
    VideoCapture cap;
    if(argc > 1) cap.open(string(argv[1]));
    else {
        cout << "Ucitavam zadani video" << endl;
        cap.open("/media/petrich-wd/Faks/Zavrsni rad/Snimke/MVI_0810.MOV");      
    }
    
    Mat frame;
    namedWindow("Perin prozor", 1);
    
    for(;;){
        cap >> frame;
        if(!frame.data) break;
        imshow("video", frame);
        if(waitKey(30) >= 0) break;
    }
    
    return 0;
    
}
