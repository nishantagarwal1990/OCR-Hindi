#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc,char **argv){
	string filename,outfilename,line;
	ifstream inputfile;
	ofstream outputfile;
	int count = 0;
	for(int i =0;i<=110;++i){
		filename = "trainexp/"+to_string(i)+".txt";
		outfilename = "convert/exp/svm/train/"+to_string(i)+".txt";
		inputfile.open(filename);
		outputfile.open(outfilename);
		if(inputfile.is_open()){
			while(getline(inputfile,line)){
				if(count == 0)
					outputfile<<i+1<<" 0:1 ";
				++count;
				if(count>=1 && count<140){
					outputfile<<count<<":"<<line<<" ";
				}
				else if(count == 140){
					outputfile<<count<<":"<<line<<endl;
				}
				else if(count == 141){}
				else
					count = 0;
			}
		}
		inputfile.close();
		outputfile.close();
	}
}