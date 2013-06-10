#include "opencv2/highgui/highgui.hpp"
#include <cv.h>

#include <stdio.h>

//using namespace cv;
using namespace std;

static void help()
{
    cout << "\nPerica perich i makija makich \n"
           "    /.Program [video name]\n\n";
}
cv::Rect box;
cv::Point point;
bool drawing_box = false;

void defineRoi( cv::Mat& img, cv::Rect rect ){
    cout << "crtam" << endl;
    cv::rectangle( img, rect.tl(), rect.br(), cv::Scalar(255,0,0), 1);
    imshow("Video", img);

} 

void onMouse( int event, int x, int y, int flags, void* param ) {
    cv::Mat& image = *(cv::Mat*) param;
    switch( event )
    {
        case CV_EVENT_LBUTTONDOWN:
            drawing_box = true;
            box = cv::Rect(x, y, 0, 0);
            break;
        case CV_EVENT_MOUSEMOVE: 
            if( drawing_box ) {
                box.width = x-box.x;
                box.height = y-box.y;
            }
            break;
        case CV_EVENT_LBUTTONUP: 
            drawing_box = false;
            if( box.width<0 ) {
                box.x+=box.width;
                box.width *= -1;
            }
            if( box.height<0 ) {
                box.y+=box.height;
                box.height*=-1;
            }
            defineRoi(image, box );
            cout << "box coordinates \n" 
                << "x\t y\t height\t width\n"
                << box.x << "\t" << box.y << "\t" 
                << box.height << "\t" << box.width << "\n";
            break;
    }
}
void choosePoint( int event, int x, int y, int flags, void* param ) {
    cv::Mat& image = *(cv::Mat*) param;
    switch( event )
    {
        case CV_EVENT_LBUTTONUP:
            point.x = x;
            point.y = y;
            cout << "Tocka " << point << endl;
            // Kod pristupanja elementu bitno je Napravit objekt tipa
            // Vec3b koji je zapravo vektor s tri unsinged int člana
            cv::Vec3b elem = image.at<cv::Vec3b>(x,y);
            cout << elem << endl;
            break;
    }
}


int main( int argc, const char** argv )
{
    help();
    
    cv::VideoCapture cap;
    if(argc > 1) cap.open(string(argv[1]));
    else {
        cout << "Ucitavam zadani video" << endl;
        cap.open("/media/petrich-wd/Faks/Zavrsni rad/Snimke/MVI_0810.MOV");      
    }
    
    cv::Mat frame, frame_hsv;
    cv::namedWindow("Video", 1);
    
    for(;;){
        cap >> frame;
        cv::cvtColor(frame,frame_hsv,CV_BGR2HSV);
        if(!frame_hsv.data) break;
        cv::imshow("Video", frame_hsv);
                
        int c = cv::waitKey(15);
        switch( (char)c )
        {
            case 27:
                cout << "Izlazim ... \n ";
                return 0;
        
            case 'p':
                cout << "Stisni s za start" << endl;
                cv::setMouseCallback( "Video", onMouse, (void*)&frame_hsv );
                if (cv::waitKey(0)=='s') break;
                
            case 'x':
                cout << "Klikni na kuglicu"<< endl;
                cv::setMouseCallback( "Video", choosePoint, (void*)&frame_hsv );
                if (cv::waitKey(0)=='s') break;
                
        }
    }
    
    return 0;
    
}
