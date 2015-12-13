#include "feature_extraction.hpp"


void binaryimagefeature(string path, ofstream& outputfile){
	Mat src, dst;
    int i,j;
	src = imread(path, 1);//CV_LOAD_IMAGE_UNCHANGED );
    dst = src.clone();
    cvtColor(src, dst, CV_BGR2GRAY);
    resize(dst, dst, Size(32, 32), 0, 0, INTER_CUBIC);
    dst = dst > 128;
    dst = dst/255;
    for(i=0;i<dst.rows;++i){
    	for(j=0;j<dst.cols;++j){
        	if(i*j != 961)
            	outputfile<<int(dst.at<uchar>(Point(i, j)))<<" ";
            else
            	outputfile<<int(dst.at<uchar>(Point(i, j)))<<"\n";
         }
      }
}

void freemanchaincode(string path,ofstream& outputfile)
{
    Mat src, dst,contimg,contresizeimg;
    vector<Mat> contours;
    vector<Vec4i> hierarchy;
    int i,j;
    src = imread(path, 1);//CV_LOAD_IMAGE_UNCHANGED );
    dst = src.clone();
    cvtColor(src, dst, CV_BGR2GRAY);
    dst = dst > 128;
    dst = dst/255;
    resize(dst, dst, Size(64,64), 0, 0, INTER_CUBIC);
    
    findContours(dst,contours,hierarchy,RETR_LIST,CV_CHAIN_CODE,Point(0,0));
    contimg = Mat::zeros(dst.rows, dst.cols, CV_8UC3);
    cout<<contours.size()<<endl;
    cout<<contours[0].size()<<endl;
    for(i=0; i < contours.size(); ++i){
        cout<<contours[i]<<endl;
        ///for(j=0;j< contours[0].size();++j){
            //if(i*j != 3969)
            	//cout<<contours[i][j];//int(c.at<uchar>(Point(i, j)))<<" ";
            //else
           // 	cout<<contours[i][j]<<"\n";//int(c.at<uchar>(Point(i, j)))<<"\n";
       // }
    }
    for( int idx = 0; idx >= 0; idx = hierarchy[idx][0] )
    {
        Scalar color( rand()&255, rand()&255, rand()&255 );
        drawContours( contimg, contours, idx, color, CV_FILLED, 8, hierarchy );
    }
    
    resize(contimg,contresizeimg,Size(5,5),0,0,INTER_CUBIC);
    /*
    for(i=0;i<contresizeimg.rows;++i){
        for(j=0;j<contresizeimg.cols;++j){
            
        }
    }*/
    /*
    namedWindow( "Components", 1 );
    imshow( "Components", contimg );
    waitKey(0);*/
    /*
    CvChain* chain = 0;
    CvMemStorage* storage=0;
    storage=cvCreateMemStorage(0);
    cvFindContours(dst,storage,(CvSeq**)&chain,sizeof(*chain),CV_RETR_LIST,CV_CHAIN_CODE);
    int chainsize=chain->elem_size;
    for(int j=0; j<chainsize;j++){
        chain=(CvChain*)chain ->h_next;
        CvSeqReader reader;
        int i, total = chain->total;
        cvStartReadSeq((CvSeq*)chain,&reader,0);
        printf("--------------------chain");
               
        for(i=0;i<total;i++)
        {
            char code;
            CV_READ_SEQ_ELEM(code, reader);
            printf("%d",code);
         }
    } */
}

void HOGFeatures(string path,ofstream& outputfile)
{
    Mat src;
    int i;
    HOGDescriptor hog(Size(32,32), //winSize
                    Size(16,16), //blocksize
                    Size(8,8), //blockStride,
                    Size(8,8), //cellSize,
                    9 //nbins
                    );
    vector<float> features;
    vector<Point>locs;
    //cout<<"here1"<<endl;
    src = imread(path, 1);
    if(!src.data)
    {
        cout << "Failed loading "<< endl;
        return;
    }
    //cout<<"here2"<<endl;
    resize(src, src, Size(32,32), 0, 0, INTER_CUBIC);
    hog.compute(src,features,Size(32,32),Size(0,0),locs);
    //cout<<"here3"<<endl;
    //cout<<features.size()<<endl;
    
    for(i=0;i<features.size();++i){
        outputfile<<features[i]<<" ";
    }
    outputfile<<endl;
}