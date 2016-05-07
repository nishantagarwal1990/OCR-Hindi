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
		filename = "testexp/"+to_string(i)+".txt";
		outfilename = "convert/exp/knn/test/"+to_string(i)+".txt";
		inputfile.open(filename);
		outputfile.open(outfilename);
		if(inputfile.is_open()){
			while(getline(inputfile,line)){
				++count;
				if(count<141){
					outputfile<<line<<" ";
				}
				else if(count == 141){
					outputfile<<line<<endl;
				}
				else
					count = 0;
			}
		}
		inputfile.close();
		outputfile.close();
	}
}