#include "feature_extraction.hpp"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
using namespace std;




int main( int argc, char** argv )
{
    int i,j;
    int main_choice,sub_choice;
    string filename;
    ifstream inputfile;
    ofstream outputfile;
    string line,path,char_num,file_name;
    size_t pos;
    main_choice = -1;
    
    do{
        cout<<"Select one of the below functions"<<endl;
        cout<<"1. Feature Extraction"<<endl;
        cout<<"2. Classification"<<endl;
        cout<<"3. Exit"<<endl;
        cin >> main_choice;
        if(main_choice == 1){
            cout<<"Enter the filepath or name(containing name of all image files): ";
            cin >> filename;
            cout<<"Select one of the below functions"<<endl;
            cout<<"1. Binary extraction"<<endl;
            cout<<"2. Contouring"<<endl;
            cin >> sub_choice;
            
           
            inputfile.open(filename,ios::in);
            
            struct stat st = {0};
            if(inputfile.is_open()){
                while(getline(inputfile,line)){
                    pos = line.find_last_of("/");
                    file_name = line.substr(pos+1);
                    pos = file_name.find_first_of("t");
                    char_num = file_name.substr(0,pos);
                    if(sub_choice == 1){
                        if (stat("feature_extracted/binary", &st) == -1) {
                            system("mkdir -p feature_extracted/binary/");
                            //mkdir("feature_extracted/binary", 0700);
                        }
                        path = "feature_extracted/binary/"+char_num+".txt";
                        outputfile.open(path, ios::out | ios::app);
                        binaryimagefeature(line,outputfile);
                        outputfile.close();
                    }
                    if(sub_choice == 2){
                        if (stat("feature_extracted/contour", &st) == -1) {
                            system("mkdir -p feature_extracted/contour/");
                            //mkdir("feature_extracted/contour", 0700);
                        }
                        path = "feature_extracted/contour"+char_num+".txt";
                        outputfile.open(path, ios::out | ios::app);
                        outputfile.close();
                    }
                }
            }
            inputfile.close();
        }
    }while(main_choice != 3);

    return 0;
}