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

int main(int argc,char **argv){
	int i,j,k,count;
	table astro_org_train;
	ofstream outputfile;
	outputfile.open("trainexpsvmblow.txt");
	generatetable(astro_org_train,"trainexpsvm.txt",140);
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
        count=140;
        for(j=2;j<astro_org_train[0].size();++j){
            for(k=j;k<astro_org_train[0].size();++k){
                outputfile<<" "<<++count<<":"<<astro_org_train[i][j]*astro_org_train[i][k];
            }
        }
        outputfile<<"\n";
    }
    outputfile.close();
    return 0;
}