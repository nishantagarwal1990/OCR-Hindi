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
    string test_or_train;
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
            cout<<"Enter train or test: ";
            cin >> test_or_train;
            cout<<"Enter the filepath or name(containing name of all image files): ";
            cin >> filename;
            cout<<"Select one of the below functions"<<endl;
            cout<<"1. Binary Extraction"<<endl;
            cout<<"2. Freeman Chain Code"<<endl;
            cout<<"3. HoG"<<endl;
            cin >> sub_choice;
            
           
            inputfile.open(filename,ios::in);
            
            struct stat st = {0};
            if(inputfile.is_open()){
                while(getline(inputfile,line)){
                    //cout<<line<<endl;
                    pos = line.find_last_of("/");
                    //cout<<pos<<endl;
                    file_name = line.substr(pos+1);
                    //cout<<file_name<<endl;
                    pos = file_name.find_first_of("t");
                    //cout<<pos<<endl;
                    char_num = file_name.substr(0,pos);
                    if(sub_choice == 1){
                        if(test_or_train == "train"){
                            if (stat("feature_extracted/newbinary1024/train/", &st) == -1) {
                                system("mkdir -p feature_extracted/newbinary1024/train/");
                                //mkdir("feature_extracted/binary", 0700);
                            }
                            path = "feature_extracted/newbinary1024/train/"+char_num+".txt";
                        }
                        else{
                            if (stat("feature_extracted/newbinary1024/test/", &st) == -1) {
                                system("mkdir -p feature_extracted/newbinary1024/test/");
                                //mkdir("feature_extracted/binary", 0700);
                            }
                            path = "feature_extracted/newbinary1024/test/"+char_num+".txt";
                        }
                        outputfile.open(path, ios::out | ios::app);
                        binaryimagefeature(line,outputfile);
                        outputfile<<" 1 "<<stoi(char_num)+1<<endl;
                        outputfile.close();
                    }
                    if(sub_choice == 2){
                        if(test_or_train == "train"){
                            if (stat("feature_extracted/freeman/train/", &st) == -1) {
                                system("mkdir -p feature_extracted/freeman/train/");
                                //mkdir("feature_extracted/binary", 0700);
                            }
                            path = "feature_extracted/freeman/train/"+char_num+".txt";
                        }
                        else{
                            if (stat("feature_extracted/freeman/test/", &st) == -1) {
                                system("mkdir -p feature_extracted/freeman/test/");
                                //mkdir("feature_extracted/binary", 0700);
                            }
                            path = "feature_extracted/freeman/test/"+char_num+".txt";
                        }
                        outputfile.open(path, ios::out | ios::app);
                        freemanchaincode(line,outputfile);
                        outputfile.close();
                    }
                    if(sub_choice == 3){
                        if(test_or_train == "train"){
                            if (stat("feature_extracted/expandedSVMHOG64128morefeatures/train/", &st) == -1) {
                                system("mkdir -p feature_extracted/expandedSVMHOG64128morefeatures/train/");
                                //mkdir("feature_extracted/binary", 0700);
                            }
                            path = "feature_extracted/expandedSVMHOG64128morefeatures/train/"+char_num+".txt";
                        }
                        else{
                            if (stat("feature_extracted/expandedSVMHOG64128morefeatures/test/", &st) == -1) {
                                system("mkdir -p feature_extracted/expandedSVMHOG64128morefeatures/test/");
                                //mkdir("feature_extracted/binary", 0700);
                            }
                            path = "feature_extracted/expandedSVMHOG64128morefeatures/test/"+char_num+".txt";
                        }
                        //cout<<line<<endl;
                        outputfile.open(path, ios::out | ios::app);
                        /*
                        if(stoi(char_num)==0){
                            outputfile<<"1 0:1 ";
                        }
                        else
                            outputfile<<"-1 0:1 ";*/
                        outputfile<<stoi(char_num)+1<<" 0:1 ";
                        HOGFeatures(line,outputfile);
                            //outputfile<<" 1 "<<stoi(char_num)+1<<endl;
                        outputfile.close();
                    }
                }
            }
            inputfile.close();
        }
    }while(main_choice != 3);

    return 0;
}