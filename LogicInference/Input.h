//
//  Input.h
//  LogicInference
//
//  Created by Varun Varadarajan on 11/13/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef LogicInference_Input_h
#define LogicInference_Input_h

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Input
{
private:
    ifstream inputFile;
    string query;
    int numberOfSentences;
    vector<string> sentences;
    string Trim(string);
    void SetQuery();
    void SetNumberOfSentences();
    void SetSentences();
    
public:
    Input(string pathToInputFile = "input.txt");
    string GetQuery() { return query; }
    int GetNumberOfSentences() { return numberOfSentences; }
    vector<string> GetSentenceList() { return sentences; }
};

Input::Input(string pathToInputFile)
{
    string line;
    char *fileName=new char[pathToInputFile.size()+1];
    fileName[pathToInputFile.size()]=0;
    memcpy(fileName,pathToInputFile.c_str(),pathToInputFile.size());
    inputFile.open(fileName);
    SetQuery();
    SetNumberOfSentences();
    SetSentences();
    inputFile.close();
}

void Input::SetQuery()
{
    string line;
    getline(inputFile, line);
    query = Trim(line);
}

void Input::SetNumberOfSentences()
{
    string line;
    getline(inputFile, line);
    numberOfSentences = atoi(Trim(line).c_str());
}

void Input::SetSentences()
{
    string line;
    while (getline(inputFile, line))
    {
        sentences.push_back(Trim(line));
    }
}

string Input::Trim(string line)
{
    line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
    return line;
}
#endif
