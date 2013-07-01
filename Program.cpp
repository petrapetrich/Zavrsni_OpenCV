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
cv::Mat imgroi;
cv::Mat imgthresh;

int avgValue;

void help();
int averageValue();
void defineRoi( cv::Mat& img, cv::Rect rect);
void onMouse(int event, int x, int y, int flags, void*param);
void choosePoint(int event, int x, int y, int flags, void* param);


int main( int argc, const char** argv ){
    help();
    cv::VideoCapture cap;

    if(argc > 1) cap.open(string(argv[1]));
    else {
        cout << "Ucitavam zadani video. \n Pritisni p za pauzu.\n" << endl;
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
                cout << "Odaberi ROI misem te odaberi s za odabir početka i kraja puta\n"<<endl;
                cv::setMouseCallback( "Video", onMouse, (void*)&frame_gray );
                if (cv::waitKey(0)=='s') break;
                
            case 'x':
                cout << "Odaberi početak i kraj puta te pritisni s za start videa."<< endl;
                cv::setMouseCallback( "Video", choosePoint, (void*)&frame_gray );
                if (cv::waitKey(0)=='s') break;
                 
        }
        if (setRoi) {
        cv::threshold(imgroi, imgthresh, avgValue-60, 255, 0);
        imshow("roi", imgthresh);
        cv::Mat imgcont = cv::Mat::zeros(imgthresh.rows, imgthresh.cols, CV_8UC3);
        
        vector<vector<cv::Point> > contours;
        vector<cv::Vec4i> hierarchy;
        
        imshow("roi", imgthresh);
        cv::findContours(imgthresh, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );
        
        int largestIndex = 0;
        int largestContour = 0;
        int secondLargestIndex = 0;
        int secondLargestContour = 0;
        for( int i = 0; i< contours.size(); i++ )
        {
            if(contours[i].size() > largestContour){
                secondLargestContour = largestContour;
                secondLargestIndex = largestIndex;
                largestContour = contours[i].size();
                largestIndex = i;
            }else if(contours[i].size() > secondLargestContour){
                secondLargestContour = contours[i].size();
                secondLargestIndex = i;
            }
        }       

        cv::Scalar color(255,0,0);
        cv::drawContours(imgcont, contours, largestIndex, color, 1, 8);
        imshow("kont1", imgcont);
        }
        
        
    }
    
    return 0;
    
}

void help(){
    cout <<  "Putanja za odabir videa:\n  /.Program [video name]\n\n";}

int averageValue (){
    
    int value = 0;
    int brojac = 0;
    int i, j;
    
    for (i=0; i<=imgroi.rows; i++){
        for (j=0; j<imgroi.cols; j++){
            
            value += imgroi.at<uint8_t>(i,j) ;
            brojac ++ ; }}
    
    avgValue = value/brojac;
    return avgValue;
    
    
    }   
    
void defineRoi( cv::Mat& img, cv::Rect rect ){
    cout << "Oznacen je ROI" << endl;
    cv::rectangle( img, rect.tl(), rect.br(), cv::Scalar(255,0,0), 1);
    imshow("Video", img);
    setRoi=true;
  
    imgroi = img (rect);
    
    int avgValue = averageValue();
    cout << "Ovo je srednja vrijednost:" <<avgValue <<"\n"<< endl;
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
            cout << "Koordinate ROI \n" 
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
            cout << "\nTocka pocetka:" << start << endl;
            setPosition = false;
    }
        
    if ((event == CV_EVENT_LBUTTONDOWN) && (setPosition == false)){
        
        finish.x = x;
        finish.y = y;
        cout << "\nTocka kraja: " << finish << endl;
        
        }  
}


