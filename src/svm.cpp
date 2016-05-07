//
//  svm.cpp
//  SVM
//
//  Created by Nishant Agarwal on 11/16/15.
//  Copyright © 2015 Nishant Agarwal. All rights reserved.
//


#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;
typedef vector<vector<double>> table;

struct hyper{
    double rate_val;
    double tradeoff_val;
    double acc_val;
};

bool comp(hyper a,hyper b){
    return a.acc_val > b.acc_val;
}
void printweights(vector<double> &weights, ofstream &output)
{
    for(int i =0; i<weights.size();++i)
        output<<weights[i]<<"\t";
    output<<endl;
}


int SVM(double rate, double tradeoff,vector<vector<double>> &weights, table &trainset,bool test,int *t_val)
{
    int mistake = 0;
    double sumw = 0.0;
    double rate_t = 0.0;
    int tempval = *t_val;
    //static int test_val =0;
   // cout<<test_val++<<endl;
   if(!test){
        for(int i =0; i<trainset.size();++i)
        {
            rate_t = ((double)rate/(double)(1+(double)((rate*(tempval))/tradeoff)));
            for(int z=0;z<weights.size();++z){
        
                sumw = 0.0;
                for (int j=1;j<trainset[0].size();++j)
                {
                    sumw += weights[z][j-1]*trainset[i][j];
                }
                //sumw *= trainset[i][0];
            
                if((sumw > 0 && z+1 !=trainset[i][0]) || (sumw < 0 && z+1 == trainset[i][0]))
                {
                    //rate_t = ((double)rate/(double)(1+(double)((rate*(tempval))/tradeoff)));
                    //cout<< rate<<endl;
                    for(int k = 0; k<weights[z].size();++k)
                    {
                        weights[z][k] = weights[z][k]-(double)(rate_t*(weights[z][k]-(double)(tradeoff*trainset[i][k+1]* trainset[i][0])));
                    }
                    //++(*t_val);
                //printweights(weights);
                }
                else{
                    //rate_t = ((double)rate/(double)(1+(double)((rate*(tempval))/tradeoff)));
                    //cout<< rate<<endl;
                    for(int k = 0; k<weights[z].size();++k)
                    {
                        weights[z][k] = weights[z][k]-(double)(rate_t*(weights[z][k]));
                    }
                    //++(*t_val);
                }
            }
            ++(*t_val);
            tempval = *t_val;
        }   
    }
    else{
        vector<double> test_sumw(111,0.0);
        int index;
        double max = 0.0;
        for(int i =0; i<trainset.size();++i)
        {
            for(int z=0;z<weights.size();++z){
        
                for (int j=1;j<trainset[0].size();++j)
                {
                    test_sumw[z] += weights[z][j-1]*trainset[i][j];
                }
            }
            for(int z=0;z<weights.size();++z){
                if(test_sumw[z] > max){
                    max = test_sumw[z];
                    index = z+1;
                }
            }
            if (index+1 != trainset[i][0])
                ++mistake;
        }
    }
    return mistake;
}

void generatetable(table &trainset, string filename, int iter=0)
{
    ifstream file;
    file.open(filename);
    
    string line;
    size_t colpos;
    string index;
    while(getline(file,line))
    {
        vector<double> columnvalues(iter+2);
        size_t sz;
        while (line.length() !=0 && line.find(' ') != string::npos)
        {
            size_t position;
            string value;
            
            
            position = line.find_first_of(' ');
            value = line.substr(0, position);
            colpos = value.find_first_of(':');
            if(colpos == string::npos)
            {
                columnvalues[0]= stod(value,&sz) == 0? -1:stod(value,&sz);
                columnvalues[1]= 1;
            }
            else
            {
                index = value.substr(0, colpos);
                value.erase(0,colpos+1);
                columnvalues[stoi(index,&sz)+1]= stod(value,&sz);
            }
            line.erase(0, position+1);
        }
        if(!line.empty()){
            colpos = line.find_first_of(':');
            index = line.substr(0, colpos);
            line.erase(0,colpos+1);
            columnvalues[stoi(index,&sz)+1]= stod(line,&sz);
        }
        trainset.push_back(columnvalues);
        columnvalues.clear();
        
    }
    file.close();
}

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

int main(int argc, char**argv){
    int i,j,k,l,d,num,count=0,t_val=0;
    double dist = 0.0,far_dist = 0.0;
    vector<double> far_dist_vect;
    vector<string> folders;
    string n;
    string filepath,trainfilename,testfilename;
    //vector<double>weights;
    //vector<double>initweights;
    vector<vector<double>>weights(111,vector<double>(325,0.0));;
    vector<vector<double>> initweights;
    vector<double> rate = {0.0001,0.001,0.01,0.1,1.0};
    vector<double> tradeoff = {0.0001,0.001,0.01,0.1,1.0,10.0,20.0};
    vector<vector<double>> avgacc(5,vector<double>(10,0.0));
    vector<double> avgaccsimple(10,0.0);
    double maxacc = -1.0;
   // double maxaccsimple = -1.0;
    double acc = 0.0;
    //double accsimple = 0.0;
    double bestrate = 0.0;
    double besttradeoff = 0.0;
    table trainset2;
    table subsettrain;
    table subsettest;
    table testdata;
    table astro_org_train;
    table astro_org_test;
    table astro_scaled_train;
    table astro_scaled_test;
    int mistake;
    //int minmistake = 999;
    ofstream outputfile;
    vector<hyper> hyper_table;
    double margin = 0.0,min_pos_margin = 99999999,min_neg_margin = 0.0,min_margin=99999999;
    double norm = 0.0;

    //outputfile.open("output2.txt");
    //outputfile<<"Experiment 2"<<endl;
    /*
    struct stat st = {0};
    
    if (stat("astro/original.transformed", &st) == -1) {
        mkdir("astro/original.transformed", 0700);
    }
    if (stat("astro/scaled.transformed", &st) == -1) {
        mkdir("astro/scaled.transformed", 0700);
    }
    
    outputfile.open("astro/original.transformed/train");
    generatetable(astro_org_train,"astro/original/train",4);
    for(i =0;i<astro_org_train.size();++i)
    {
        for(k=0;k<astro_org_train[0].size();++k){
            if(k==0){
                outputfile<<astro_org_train[i][k];
            }
            else{
                outputfile<<" "<<k-1<<":"<<astro_org_train[i][k];
            }
        }
        count=4;
        for(j=2;j<astro_org_train[0].size();++j){
            for(k=j;k<astro_org_train[0].size();++k){
                outputfile<<" "<<++count<<":"<<astro_org_train[i][j]*astro_org_train[i][k];
            }
        }
        outputfile<<"\n";
    }
    outputfile.close();
    
    outputfile.open("astro/original.transformed/test");
    generatetable(astro_org_test,"astro/original/test",4);
    for(i =0;i<astro_org_test.size();++i)
    {
        for(k=0;k<astro_org_test[0].size();++k){
            if(k==0){
                outputfile<<astro_org_test[i][k];
            }
            else{
                outputfile<<" "<<k-1<<":"<<astro_org_test[i][k];
            }
        }
        count=4;
        for(j=2;j<astro_org_test[0].size();++j){
            for(k=j;k<astro_org_test[0].size();++k){
                outputfile<<" "<<++count<<":"<<astro_org_test[i][j]*astro_org_test[i][k];
            }
        }
        outputfile<<"\n";
    }
    outputfile.close();
    
    outputfile.open("astro/scaled.transformed/train");
    generatetable(astro_scaled_train,"astro/scaled/train",4);
    for(i =0;i<astro_scaled_train.size();++i)
    {
        for(k=0;k<astro_scaled_train[0].size();++k){
            if(k==0){
                outputfile<<astro_scaled_train[i][k];
            }
            else{
                outputfile<<" "<<k-1<<":"<<astro_scaled_train[i][k];
            }
        }
        count=4;
        for(j=2;j<astro_scaled_train[0].size();++j){
            for(k=j;k<astro_scaled_train[0].size();++k){
                outputfile<<" "<<++count<<":"<<astro_scaled_train[i][j]*astro_scaled_train[i][k];
            }
        }
        outputfile<<"\n";
    }
    outputfile.close();
    
    outputfile.open("astro/scaled.transformed/test");
    generatetable(astro_scaled_test,"astro/scaled/test",4);
    for(i =0;i<astro_scaled_test.size();++i)
    {
        for(k=0;k<astro_scaled_test[0].size();++k){
            if(k==0){
                outputfile<<astro_scaled_test[i][k];
            }
            else{
                outputfile<<" "<<k-1<<":"<<astro_scaled_test[i][k];
            }
        }
        count=4;
        for(j=2;j<astro_scaled_test[0].size();++j){
            for(k=j;k<astro_scaled_test[0].size();++k){
                outputfile<<" "<<++count<<":"<<astro_scaled_test[i][j]*astro_scaled_test[i][k];
            }
        }
        outputfile<<"\n";
    }
    outputfile.close();
    */
    outputfile.open("Output.txt");
    //folders = {"data0","astro/original","astro/scaled","astro/original.transformed","astro/scaled.transformed"};
   /* for(int z = 0; z < folders.size(); ++z)
    {
        if(folders[z] == "data0"){
            filepath = "data0/";
            trainfilename = filepath+"train0.10";
            testfilename = filepath+"test0.10";
            d=10;
            num = 1000;
            weights.resize(11,0.0);
        }
        else if(folders[z] == "astro/original"){
            filepath = "astro/original/";
            trainfilename = filepath+"train";
            testfilename = filepath+"test";
            d=4;
            num = 3089;
            weights.resize(5,0.0);
        }
        else if(folders[z] == "astro/scaled"){
            filepath = "astro/scaled/";
            trainfilename = filepath+"train";
            testfilename = filepath+"test";
            d=4;
            num = 3089;
            weights.resize(5,0.0);
        }
        else if(folders[z] == "astro/original.transformed"){
            filepath = "astro/original.transformed/";
            trainfilename = filepath+"train";
            testfilename = filepath+"test";
            d=14;
            num = 3089;
            weights.resize(15,0.0);
        }
        else{
            filepath = "astro/scaled.transformed/";
            trainfilename = filepath+"train";
            testfilename = filepath+"test";
            d=14;
            num = 3089;
            weights.resize(15,0.0);
        }*/
        //cout<<"here"<<endl;
        trainfilename = "trainsvmhog.txt";
        testfilename = "testsvmhog.txt";
        d = 324;
        num = 18357;
        outputfile<<"Filename: "<<trainfilename<<"\n";
        generatetable(trainset2, trainfilename,d);
        //cout<<"here2"<<endl;
        //weights.resize(325,0.0);
        //initweights = weights;
        for(k=0;k<=9;++k)
        {
            cout<<"Run : "<<k+1<<endl;
            copytable(subsettrain,trainset2,subsettest,k,num);
            for(i = 0; i<5; ++i)
            {
                for(j=0; j<7; ++j)
                {
                    for(l=0;l<10;++l){
                        random_shuffle(subsettrain.begin(),subsettrain.end());
                        SVM(rate[i], tradeoff[j], weights, subsettrain,false,&t_val);
                    }
                    t_val = 0;
                    mistake = SVM(rate[i], tradeoff[j], weights, subsettest,true,&t_val);
                    avgacc[i][j]+=(double)((double)(subsettest.size()-mistake)/subsettest.size());
                    //weights = initweights;
                    cout<<avgacc[i][j]<<endl;
                    for(int g = 0 ; g<weights.size();++g)
                    {
                        weights[g].clear();
                        weights[g].resize(325,0.0);
                    }
                    t_val = 0;
                }
            }
            subsettrain.clear();
            subsettest.clear();
        }
        hyper temp;
        hyper_table.clear();
        for(i=0;i<5;++i)
        {
            for(j = 0 ; j<7; ++j)
            {
                temp.rate_val= rate[i];
                temp.tradeoff_val = tradeoff[j] ;
                acc = avgacc[i][j]/10.0;
                temp.acc_val = acc;
                if(acc > maxacc)
                {
                    maxacc = acc;
                    bestrate = rate[i];
                    besttradeoff = tradeoff[j];
                }
                hyper_table.push_back(temp);
            }
        }
        sort(hyper_table.begin(),hyper_table.end(),comp);
        outputfile<<"\n\nInitial Rate\t\tC\t\tAverage Accuracy\n";
        for(i=0;i<5;++i)
        {
            outputfile<<hyper_table[i].rate_val<<"\t\t\t"<<hyper_table[i].tradeoff_val<<"\t\t\t"<<hyper_table[i].acc_val<<"\n";
        }
        //bestrate = 0.1;
        //besttradeoff = 10;
        //int mistakes = 0;
        int totmistakes = 0;
        //int totmistake = 0;
        //vector<double> weightsimple = initweights;
        //vector<double> weightmargin = initweights;
        //weights = initweights;
        cout<<"Starting main epochs for training"<<endl;
        t_val = 0;
        for(int x = 0; x<30; ++x)
        {
            random_shuffle(trainset2.begin(),trainset2.end());
            totmistakes += SVM(bestrate, besttradeoff, weights, trainset2,false,&t_val);
            //random_shuffle(trainset2.begin(),trainset2.end());
        }

        generatetable(testdata, testfilename,d);
        //outputfile<<"Simple Perceptron Weights:"<<endl;
        //printweights(weights,outputfile);
        /*far_dist = 0.0;
        for(i=0;i<trainset2.size();++i){
            dist = 0.0;
            for(j=2;j<trainset2[0].size();++j){
                dist += pow(trainset2[i][j],2.0);
            }
            dist = sqrt(dist);
            if(dist>far_dist)
            {
                far_dist = dist;
                far_dist_vect = trainset2[i];
            }
        }
        outputfile<<"\n\nThe farthest point from the origin at a distance of "<<far_dist<<" is:\n\n";
        for(i=0;i<far_dist_vect.size();++i){
            if(i==0)
                outputfile<<far_dist_vect[i];
            else{
                outputfile<<" "<<i-1<<":"<<far_dist_vect[i];
            }
        }*/
        outputfile<<"\n\n";
        t_val = 0;
        mistake = SVM(bestrate, besttradeoff, weights, trainset2,true,&t_val);
        acc = (double)((double)(trainset2.size()-mistake)/trainset2.size());
        outputfile<<"\nAccuracy on training set: "<<acc<<endl;
        t_val = 0;
        mistake = SVM(bestrate, besttradeoff, weights, testdata,true,&t_val);
        acc = (double)((double)(testdata.size()-mistake)/testdata.size());
        //outputfile<<"Simple Perceptron Accuracy(On test data): "<<acc*100<<endl;
        outputfile<<"\nAccuracy on test set: "<<acc<<endl;
        margin = 0.0,min_pos_margin = 99999999,min_neg_margin = 99999999,min_margin=99999999;
        /*norm = 0.0;
        for(i=1;i<weights.size();++i)
        {
            norm += pow(weights[i],2.0);
        }
        norm = sqrt(norm);
        for(i =0; i<trainset2.size();++i)
        {
            margin = 0.0;
            for (j=1;j<trainset2[0].size();++j)
            {
                margin += weights[j-1]*trainset2[i][j];
            }
            margin = abs(margin)/norm;
            if(margin<min_margin){
                min_margin = margin;
            }*/
            /*
            if(margin>0){
                margin = abs(margin)/norm;
                if(margin < min_pos_margin)
                    min_pos_margin = margin;
            }
            else{
                margin = abs(margin)/norm;
                if(margin < min_neg_margin)
                    min_neg_margin = margin;
            }
             */
        //}
       // outputfile<<"\n\nMargin: "<<min_margin<<"\n\n\n";
        //outputfile<<"\n\nMargin for positive labels: "<<min_pos_margin<<"\n";
        //outputfile<<"\n\nMargin for negative labels: "<<min_neg_margin<<"\n\n\n\n";
        avgaccsimple.clear();
        avgaccsimple.resize(10,0.0);
        for(int g = 0 ; g<avgacc.size();++g)
        {
            avgacc[g].clear();
            avgacc[g].resize(10,0.0);
        }
        trainset2.clear();
        testdata.clear();
        subsettest.clear();
        subsettrain.clear();
        initweights.clear();
        outputfile<<"\n\n"<<endl;
        outputfile.close();
        weights.clear();
        hyper_table.clear();
        far_dist_vect.clear();
   // }
}