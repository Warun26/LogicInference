//
//  Input.h
//  LogicInference
//
//  Created by Varun Varadarajan on 11/13/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef LogicInference_Input_h
#define LogicInference_Input_h

#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Input
{
private:
    string query;
    int numberOfClauses;
    vector<string> KB;
    
public:
    Input(string pathToInputFile = "input.txt");
    string GetQuery() { return query; }
    int GetNumberOfClauses() { return numberOfClauses; }
    vector<string> GetKB() { return KB; }
};

Input::Input(string pathToInputFile)
{
    ifstream inputFile;
    string line;
    char *fileName=new char[pathToInputFile.size()+1];
    fileName[pathToInputFile.size()]=0;
    memcpy(fileName,pathToInputFile.c_str(),pathToInputFile.size());
    inputFile.open(fileName);
    inputFile>>query;
    inputFile>>numberOfClauses;
    inputFile.get(); //Move to the next Line
    while (getline(inputFile, line))
    {
        KB.push_back(line);
    }
}

#endif
