#include<iostream>
using namespace std;
#include "kNN.h"
#define MAX_VALUE_HYPER_PARAMETER 6
#define NO_OF_ROWS 		3050
#define NO_OF_NEIGHBOURS 10

int main(int argc, char *argv[])
{
        FILE *fp;
        vector<inputData> combineTable;

        char inputString[inputlength];
        fp = fopen(argv[1], "r");
		if (fp == NULL )
		{
				cout << "unable to open file:"<<argv[1]<<endl ;
				return -1;
		}
		else
				cout<<"succesfull opened"<<argv[1]<<endl;

		for(int l =0; l < MAX_VALUE_HYPER_PARAMETER; ++l)
		{
			inputData table;
			for(int m =0; m < NO_OF_ROWS; m++)
			{
				if (fgets(inputString, inputlength, fp))
				{
						insertDataintotable(inputString, table);
				}
				else
					break;
			}
			combineTable.push_back(table);
		}
			vector<double> accuracy;
	cout<<"FIle closed"<<endl;
	if(fp)
		fclose(fp);
	fp = NULL;
        for( size_t i = 1; i <=NO_OF_NEIGHBOURS; i++)
        {
			double accuratePredicted  = 0;
			//vector<double> kaccuracy;
			for( size_t j = 0; j < combineTable.size(); j++)
			{
							//Create Training set
				inputData trainingSet, predictionSet;
				createtrainingSet(trainingSet , j, combineTable);
				predictionSet = combineTable[j];
				double correctlypredicted  = calculateaccurates( trainingSet , predictionSet, i);
			//	kaccuracy.push_back(correctlypredicted);
				accuratePredicted = accuratePredicted + correctlypredicted;
			}
			accuracy.push_back(accuratePredicted/(double)combineTable.size());	
			cout<<"accuracy for hyperParameter " <<i <<":"<< accuracy[i-1]*100 << endl;
		}
	double maxAccuracy = accuracy[0];
	int hyperParameter = 1;
	for(size_t i =1; i < accuracy.size(); i++)
	{
		if(maxAccuracy < accuracy[i])
		{
			maxAccuracy = accuracy[i];
			hyperParameter = (i+1);
		}
	}
	cout<<"hyperParameter Selected is: "<<hyperParameter<<endl;	
	fp = fopen(argv[2], "r");
        inputData finalpredictiontable;
               if (fp == NULL )
                {
                        cout << "unable to open file" ;
                        return -1;
                }
                while (fgets(inputString, inputlength, fp))
                {
                        insertDataintotable(inputString, finalpredictiontable);
                }
                if(fp)
                        fclose(fp);
                fp = NULL;
	inputData finaltrainingSet;
	createtrainingSet(finaltrainingSet , 100, combineTable);
	double FinalAccuracy = 0;
	FinalAccuracy =  calculateaccurates( finaltrainingSet,finalpredictiontable, hyperParameter);
	cout<<"Final accuracy is:" <<FinalAccuracy*100<<endl;
	
		return 0;
}
