#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <sys/stat.h>
using namespace std;
using namespace cv;

double threshold_value = 127;
int threshold_type = 0;
double const max_BINARY_value = 1;
Mat src; Mat dst;
//char window_name1[] = "Unprocessed Image";
//char window_name2[] = "Processed Image";

int main( int argc, char** argv )
{
    int i,j;
    string line,path,char_num,file_name;
    size_t pos;
    ifstream inputfile;
    ofstream outputfile;
    inputfile.open(argv[1],"r");
    
    struct stat st = {0};
    
    if (stat("feature_extracted", &st) == -1) {
        mkdir("feature_extracted", 0700);
    }
    if(inputfile.is_open()){
        while(getline(inputfile,line)){
            pos = line.find_last_of("/");
            file_name = line.substr(pos+1);
            pos = file_name.find_first_of("t");
            char_num = file_name.substr(0,pos);
            path = "feature_extracted"+char_num+".txt";
            outputfile.txt(path, ios::out | ios::app);
            /// Load the source image
            src = imread(line, CV_LOAD_IMAGE_UNCHANGED );
            dst = src.clone();
            cvtColor(src, dst, CV_BGR2GRAY);
            dst = dst > 128;
            dst = dst/255;
            resize(dst, dst, Size(64, 64), 0, 0, INTER_CUBIC);
            for(i=0;i<dst.rows;++i){
                for(j=0;j<dst.cols;++j){
                    if(i*j != 3969)
                        outputfile<<int(dst.at<uchar>(Point(i, j)))<<" ";
                    else
                        outputfile<<int(dst.at<uchar>(Point(i, j)))<<"\n";
                }
            }
            outputfile.close();
        }
    }
    
    return 0;
}