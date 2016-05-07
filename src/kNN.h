#ifndef KNN_H
#define KNN_h

#include<vector>
#include<map>
#include<string>
#include <string.h>
#include<math.h>
#include <fstream>
#include<cstdio>
using namespace std;
#define inputlength 8096

typedef struct distance_t
{
	double distance;
	double output;
	distance_t()
	{
		distance = 0;
		output = 0;
	}
}distanceIndex;

typedef vector<vector<double> > inputData;
void  createtrainingSet(inputData &trainingSet , size_t indexTestSet,  vector<inputData> &combineTable);
int insertDataintotable( char* data, inputData &table);
double calculateaccurates( inputData &trainingSet ,inputData  &predictionSet, size_t hyperParameter);
#endif
