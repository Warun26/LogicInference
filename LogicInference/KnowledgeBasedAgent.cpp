//
//  KnowledgeBasedAgent.cpp
//  LogicInference
//
//  Created by Varun Varadarajan on 11/13/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#include "KnowledgeBasedAgent.h"

KnowLedgeBasedAgent::KnowLedgeBasedAgent()
{
    t = 0;
}

void KnowLedgeBasedAgent::Tell(string percept)
{
    t = t+1;
    Sentence sentence = MakePerceptSentence(percept);
    KnowledgeBase.push_back(sentence);
    Predicate conclusion = TextParser::GetPredicate(sentence.GetConclusion());
    if (predicateMap.find(conclusion.name) == predicateMap.end())
    {
        vector<int> conclusionId;
        conclusionId.push_back(t);
        auto tuple = make_tuple(*new vector<int>(), conclusionId);
        predicateMap[conclusion.name] = tuple;
    }
    else
    {
        get<1>(predicateMap[conclusion.name]).push_back(t);
    }
    vector<string>premise = sentence.GetPremise();
    for (int i=0; i<premise.size(); i++)
    {
        Predicate p = TextParser::GetPredicate(premise[i]);
        if (predicateMap.find(p.name) == predicateMap.end())
        {
            vector<int> premiseId;
            premiseId.push_back(t);
            auto tuple = make_tuple(premiseId, *new vector<int>());
            predicateMap[p.name] = tuple;
        }
        else
        {
            get<0>(predicateMap[p.name]).push_back(t);
        }
    }
}



Sentence KnowLedgeBasedAgent::MakePerceptSentence(string s)
{
    Sentence sentence = *new Sentence();
    if(s.find("=>") != string::npos)
    {
        vector<string> implication = TextParser::Split(s, "=>");
        sentence.SetConclusion(implication[1]);
        vector<string> predicates = TextParser::Split(implication[0], "&");
        sentence.SetPremise(predicates);
    }
    else
    {
        sentence.SetConclusion(s);
    }
    return sentence;
}

void Sentence::SetPremise(vector<string> predicates)
{
    premise = predicates;
}

void Sentence::SetConclusion(string predicate)
{
    conclusion = predicate;
}

string Sentence::GetConclusion()
{
    return conclusion;
}
vector<string> Sentence::GetPremise()
{
    return premise;
}

vector<string> TextParser::Split(string sentence, string delimiter)
{
    vector<string> clauses;
    size_t pos = 0;
    string token;
    while ((pos = sentence.find(delimiter)) != string::npos) {
        token = sentence.substr(0, pos);
        clauses.push_back(token);
        sentence.erase(0, pos + delimiter.length());
    }
    clauses.push_back(sentence);
    return clauses;
}

Predicate TextParser::GetPredicate(string term)
{
    Predicate p;
    unsigned long index1 = term.find('(');
    p.name = term.substr(0, index1);
    unsigned long index2 = term.find(',', index1);
    if (index2 == string::npos)
    {
        p.arg1 = "";
        index2 = term.find(')', index1);
        p.arg2 = term.substr(index1+1, index2-index1-1);
    }
    else
    {
        p.arg1 = term.substr(index1+1, index2-index1-1);
        index1 = term.find(')', index2);
        p.arg2 = term.substr(index2+1, index1-index2-1);
    }
    return p;
}