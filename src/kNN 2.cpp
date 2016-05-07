 #include<iostream>
#include"kNN.h"
#include <algorithm>    // std::sort
bool myfunction (distanceIndex  i, distanceIndex j) { return (i.distance < j.distance); }

int insertDataintotable( char* data, inputData &table)
{
        char * pch;
        pch = strtok (data ," ,\n");
        vector<double> row;
        while (pch != NULL)
        {
                row.push_back(atof(pch));
                pch = strtok (NULL, " ,\n");
        }
        table.push_back(row);
        return 0;
}
void  createtrainingSet(inputData &trainingSet , size_t indexTestSet,  vector<inputData> &combineTable)
{
	for(size_t i = 0; i < combineTable.size(); i++)
	{
		if(indexTestSet == i)
			continue;
		trainingSet.insert(trainingSet.end(), combineTable[i].begin(), combineTable[i].end());
	}
}


double calculatedistance( vector<double> &p1, vector<double> &t1, distanceIndex &obj)
{
	double distance = 0;
	for( size_t i = 0; i < p1.size() -1 ; i++)
	{
		distance += pow ((p1[i] - t1[i]), 2.0); //Calculate the eucledian distance
	}
	obj.distance = distance;
	obj.output = t1[t1.size()-1];
	return 0;
}
bool isDistancequalifies( vector<distanceIndex> &kdistances, distanceIndex &dist, size_t hyperParameter)
{
	if(kdistances.size() < hyperParameter)
	{
		kdistances.push_back(dist);
		return true;
	}
	else
	{
   	 	std::sort (kdistances.begin(),kdistances.end(), myfunction);
		if( dist.distance < kdistances[kdistances.size()-1].distance)
		{
			kdistances.pop_back();
			kdistances.push_back(dist);
			return true;
		}
			

	}
	return false;
}
void predictLabels( vector<distanceIndex> &kdistances, double &predictLabel)
{
	double  label = 0;
	map<double,int> lablemap; 
	vector<std::pair<double,int>> mapVector;
	for(size_t i = 0; i < kdistances.size(); i++)
	{
		if(lablemap.find(kdistances[i].output) != lablemap.end())
		{
			lablemap[kdistances[i].output]++;
		}
		else
			lablemap.insert(pair<double,int>(kdistances[i].output,1));
	}
	for (auto iterator = lablemap.begin(); iterator != lablemap.end(); ++iterator) {
		mapVector.push_back(*iterator);
	}
	sort(mapVector.begin(), mapVector.end());
	predictLabel =  mapVector[mapVector.size()-1].first;
}
double calculateaccurates( inputData &trainingSet ,inputData  &predictionSet, size_t  hyperParameter)
{
	int accurates = 0;
	for(size_t i = 0; i < predictionSet.size(); i++)
	{
		vector<distanceIndex> kdistances;
		for(size_t j = 0; j < trainingSet.size(); j++)
		{
			distanceIndex obj;
			double distance = calculatedistance(predictionSet[i], trainingSet[j], obj);
			isDistancequalifies( kdistances, obj, hyperParameter);
		}
		double predictLabel = 0;
		predictLabels(kdistances, predictLabel);
		if(predictLabel == (predictionSet[i][predictionSet[i].size()-1]))
			accurates++;	
	}
	return (accurates/(double)predictionSet.size());
}
