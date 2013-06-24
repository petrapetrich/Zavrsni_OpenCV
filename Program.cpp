#include "opencv2/highgui/highgui.hpp"
#include <cv.h>

#include <stdio.h>

//using namespace cv;
using namespace std;

cv::Rect box;
cv::Point start, finish;
cv::Vec3b hsv_value;
bool drawing_box = false;
bool setRoi = false;
bool setPosition = true;

void help(){
    cout <<  "    /.Program [video name]\n\n";
}

void defineRoi( cv::Mat& img, cv::Rect rect ){
    cout << "crtam" << endl;
    cv::rectangle( img, rect.tl(), rect.br(), cv::Scalar(255,0,0), 1);
    imshow("Video", img);
    setRoi = true;

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
    
    if ((event == CV_EVENT_LBUTTONUP)&& (setPosition == true)){
      
            start.x = x;
            start.y = y;
            cout << "Tocka pocetka" << start << endl;
            setPosition = false;
    }
        
    if ((event == CV_EVENT_LBUTTONDOWN) && (setPosition == false)){
        
        finish.x = x;
        finish.y = y;
        cout << "Tocka kraja: " << finish << endl;
        
        }
    
    
}

/*void comparePixelValues( cv::Mat& img ){
    Mat roi = (img, box);
    for(int i=0; i<=box.width; i++){
        for(int j=0; j<=box.height; j++){
            roi.at<
        }
    }
    
}*/

int main( int argc, const char** argv )
{
    help();
    
    cv::VideoCapture cap;
    if(argc > 1) cap.open(string(argv[1]));
    else {
        cout << "Ucitavam zadani video" << endl;
        cap.open("/media/petrich-wd/Faks/Zavrsni rad/Snimke/nove/MVI_0067.MOV");      
    }
    
    cv::Mat frame, frame_gray;
    cv::namedWindow("Video", 1);
    
    for(;;){
        cap >> frame;
        cv::cvtColor(frame,frame_gray,CV_BGR2GRAY);
        if(!frame_gray.data) break;
        cv::imshow("Video", frame_gray);
                
        char c = cv::waitKey(15);
        switch( c )
        {
            case 27:
                cout << "Izlazim ... \n ";
                return 0;
        
            case 'p':
                cout << "Odaberi ROI pa stisni s za start" << endl;
                cv::setMouseCallback( "Video", onMouse, (void*)&frame_gray );
                if (cv::waitKey(0)=='s') break;
                
            case 'x':
                cout << "Odaberi početak i kraj puta"<< endl;
                cv::setMouseCallback( "Video", choosePoint, (void*)&frame_gray );
                if (cv::waitKey(0)=='s') break;
                 
        }
        if ( setRoi && setPosition) {
        cout << "Trazim piksele koji odgovaraju boji kuglice" << endl;
        //comparePixelValues(frame_gray);
        }
    }
    
    return 0;
    
}
