#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
using namespace cv;

void binaryimagefeature(string path, ofstream& outputfile);
void freemanchaincode(string path,ofstream& outputfile);
void HOGFeatures(string path,ofstream& outputfile);