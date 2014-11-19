//
//  KnowledgeBasedAgent.h
//  LogicInference
//
//  Created by Varun Varadarajan on 11/13/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef __LogicInference__KnowledgeBasedAgent__
#define __LogicInference__KnowledgeBasedAgent__

#include <map>
#include <string>
#include <vector>

using namespace std;

struct Predicate
{
    string name;
    string arg1;
    string arg2;
    bool equals(Predicate p)
    {
        if(!this->name.compare(p.name) && !this->arg1.compare(p.arg1) && !this->arg2.compare(p.arg2)) return true;
        return false;
    }
};

class Sentence
{
    private:
    string conclusion;
    vector<string> premise;
    public:
    void SetPremise(vector<string>);
    void SetConclusion(string);
    vector<string> GetPremise();
    string GetConclusion();
};


class KnowLedgeBasedAgent
{
private:
    int t;
    vector<Sentence> KnowledgeBase;
    map<string, vector<int> > predicateMap;
    Sentence MakePerceptSentence(string);
    bool Unify(Predicate, Predicate, string&);
    string Substitute(string, string);
public:
    KnowLedgeBasedAgent();
    void Tell(string);
    bool Ask(string);
};

class TextParser
{
public:
    static vector<string> Split(string, string);
    static Predicate GetPredicate(string);
};

#endif /* defined(__LogicInference__KnowledgeBasedAgent__) */
