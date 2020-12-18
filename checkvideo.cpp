#include "opencv2/opencv.hpp"
#include <iostream>
#include "omp.h"
#include <fstream>
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{    
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap("teapot.mp4");

    // Check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    Mat oldFrame;
     double t0= omp_get_wtime();
    
    vector<int> data_vec;
    double frame_count=0;
    while(1){

        Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        if (oldFrame.empty()) {
            oldFrame = frame;
            continue;
        }
        
        frame_count++;

        cv::Mat newMat = cv::Mat(frame.rows, frame.cols, frame.type()); // frame.clone();
        int diff = 0;
        
        
        #pragma omp parallel for shared(frame, oldFrame, newMat) reduction(+: diff)
        for(int i=0;i<frame.rows;i++){
            int sum = 0;
            for(int j=0;j<frame.cols;j++){
                auto oldPxl = oldFrame.at<Vec3b>(i, j);
                auto currPxl = frame.at<Vec3b>(i, j);

                // Summiert die Vekotren nach der euklidischen Norm (Von OpenCV)
                diff = norm(oldPxl - currPxl);
                sum += diff;
                
                if(diff > 20) {
                    Vec3b & color = newMat.at<Vec3b>(i,j);

                    // ... do something to the color ....
                    color[0] = 255;
                    color[1] = 255;
                    color[2] = 255;

                } else {
                    Vec3b & color = newMat.at<Vec3b>(i,j);

                    // ... do something to the color ....
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 0;
                }
            }
            diff = sum;
        }

        data_vec.push_back(diff);

        oldFrame = frame;

        //cv::imshow( "Live", frame );
        //cv::imshow( "MotionDetection", newMat );

        // Press  ESC on keyboard to exit
        //char c=(char)waitKey(10);
        //if(c==27)
        //    break;

    }
    ofstream output_file("differences.txt");
    for(int i = 0; i < data_vec.size(); i++) {
        output_file<<data_vec.at(i)<<std::endl;
    }
    output_file.close();

     char c=(char)waitKey();
    std::cout<<"frames count  = "<<frame_count<<std::endl;
    std::cout<<"elapsed time: "<<omp_get_wtime()-t0<<std::endl;
    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
