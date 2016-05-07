//
//  KNN.cpp
//  ML2
//
//  Created by Nishant Agarwal on 9/16/15.
//  Copyright (c) 2015 Nishant Agarwal. All rights reserved.
//


#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

typedef vector<vector<double>> table;

void copytable(table &traindata, table &trainset, table &testdata,int iter,int lines)
{
    for(int i = 0; i<trainset.size();++i)
    {
        if((i>=(ceil((lines/10))*iter)) && i<(ceil((lines/10))*(iter+1)))
        {
            testdata.push_back(trainset[i]);
        }
        else
            traindata.push_back(trainset[i]);
    }
}

bool comparator ( const pair<int,double>& l, const pair<int,double>& r)
{ return l.first < r.first; }

void CreateAttributeTable(table &attrtable, string filename)
{
    ifstream file;
    file.open(filename);
    
    string line;
    while(getline(file,line))
    {
        vector<double> columnvalues;
        size_t sz;
        while (line.length() !=0 && line.find(' ') != string::npos)
        {
            size_t position;
            string value;
            
            position = line.find_first_of(' ');
            value = line.substr(0, position);
            //cout<<"here"<<endl;
            columnvalues.push_back(stod(value,&sz));
            line.erase(0, position+1);
        }
        if(!line.empty()){
            columnvalues.push_back(stod(line,&sz));
        }
        attrtable.push_back(columnvalues);
        columnvalues.clear();
        
    }
    file.close();
}
double SortNGetLabel(vector<pair<int,double>> &distnlabel, vector<double> &labels, int k)
{
    int p=0,n=0;
    sort(distnlabel.begin(),distnlabel.end(), comparator);
    vector<int> count(labels.size(),0);
    int index,max =0;
    for(int i =0;i < k; ++i)
    {
        for(int j=0;j<labels.size();++j){
            if(labels[j] == distnlabel[i].second){
                count[j] +=1;
                break;
            }
        }
    }
    for(int j=0;j<labels.size();++j){
        if(count[j] > max){
            max = count[j];
            index = j;
        }
    }
    return labels[index];
}

double CalculateAccuracy(vector<double> &givenlabels, vector<double> &predictedlabels)
{
    double correct = 0.0;
    for (int i = 0; i < givenlabels.size(); ++i) {
        
        if (givenlabels[i] == predictedlabels[i])
            ++correct;
    }
    return (double) (correct/(double)(givenlabels.size()))* 100;
}

double CalculateKNNAccuracy(table &traindata,table &testdata,vector<double> &labels,int k)
{
    vector<pair<int,double>> distnlabel;
    pair<int,double> distlbpair;
    vector<double> predictedlabels;
    vector<double> givenlabels;
    //vector<string> singleinstance;
    
    
    for(int i=0;i<testdata.size();++i)
    {
        for(int j = 0; j < traindata.size(); ++j)
        {
            int dist = 0;
            for(int l = 0; l < traindata[0].size()-1; ++l)
            {
                if(traindata[j][l] != testdata[i][l])
                    ++dist;
            }
            distlbpair.first = dist;
            distlbpair.second = traindata[j][traindata[0].size()-1];
            distnlabel.push_back(distlbpair);
        }
        predictedlabels.push_back(SortNGetLabel(distnlabel,labels,k));
        distnlabel.clear();
    }
    
    for(int n = 0; n<testdata.size(); ++n)
    {
        givenlabels.push_back(testdata[n][testdata[0].size()-1]);
    }
    
    return CalculateAccuracy(givenlabels, predictedlabels);
}

void GetDistinctLabels(table &traindata, vector<double> &labels)
{
    for(int i =0; i<traindata.size(); ++i)
    {
        if(labels.empty())
            labels.push_back(traindata[i][traindata[0].size()-1]);
        else
        {
            bool notpresent = true;
            
            for(int k = 0; k < labels.size(); ++k)
            {
                if(traindata[i][traindata[0].size()-1] == labels[k])
                    notpresent = false;
            }
            if(notpresent)
                labels.push_back(traindata[i][traindata[0].size()-1]);
        }
    }
}

int main(int argc,char **argv)
{
    
    table testdata, traindata;
    table subtraindata,subtestdata;
    vector<double> labels;
    vector<vector<double>> accuracy(10);
    double finalaccuracy;
    
    int Kvalue = 0;
    double sum = 0.0;
    double avg = 0.0;
    double min = -1;
    
    CreateAttributeTable(traindata, argv[1]);
    CreateAttributeTable(testdata, argv[2]);
    random_shuffle(traindata.begin(),traindata.end());
    random_shuffle(testdata.begin(),testdata.end());
    //6-fold cross validation
    for(int i = 0; i<=9; ++i)
    {
        copytable(subtraindata,traindata,subtestdata,i,(int)traindata.size());
        GetDistinctLabels(subtraindata,labels);
        for(int k = 1; k<=10; ++k)
            accuracy[k-1].push_back(CalculateKNNAccuracy(subtraindata,subtestdata,labels,k));
        labels.clear();
        subtraindata.clear();
        subtestdata.clear();
    }
    //ofstream outputfile;
   // outputfile.open("KNNOutput.txt",ios::app);
    
    for(int i = 0; i<accuracy.size(); ++i)
    {
        sum = 0.0;
        for(int j = 0; j<accuracy[0].size();++j)
        {
            sum += accuracy[i][j];
        }
        avg = sum/((double)(accuracy[0].size()));
        cout<<endl<<"K value:"<<i+1<<"\t\t"<<avg<<endl;
        
        if(avg > min)
        {
            min = avg;
            Kvalue = i+1;
        }
    }
    
    cout<<"\n\n"<<"Final K value is:"<<Kvalue<<" with avg accuracy "<<min<<endl;
    
    //Final KNN run
    
    //for(int i = 1; i<=6; ++i)
     //   CreateAttributeTable(traindata,argv[i]);
    
   // CreateAttributeTable(testdata,argv[argc-1]);
    
    GetDistinctLabels(traindata,labels);
    
    finalaccuracy = CalculateKNNAccuracy(traindata,testdata,labels,Kvalue);
    
    cout<<"\n\nThe Final Accuracy using K="<<Kvalue<<" is "<<finalaccuracy<<"%"<<endl;
    
    //outputfile.close();
    labels.clear();
    traindata.clear();
    testdata.clear();
    
    return 0;
}