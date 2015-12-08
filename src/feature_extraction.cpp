#include "feature_extraction.hpp"


void binaryimagefeature(string path, ofstream& outputfile){
	Mat src, dst;
    int i,j;
	src = imread(path, 1);//CV_LOAD_IMAGE_UNCHANGED );
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
}