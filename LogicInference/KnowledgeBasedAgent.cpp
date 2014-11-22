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
        predicateMap[conclusion.name] = conclusionId;
        if (conclusion.arg1[0] != 'x' && find(constants.begin(), constants.end(), conclusion.arg1) == constants.end())
            constants.push_back(conclusion.arg1);
        if (conclusion.arg2[0] != 'x' && find(constants.begin(), constants.end(), conclusion.arg2) == constants.end())
            constants.push_back(conclusion.arg2);
        }
    else
    {
        (predicateMap[conclusion.name]).push_back(t);
        if (conclusion.arg1[0] != 'x' && find(constants.begin(), constants.end(), conclusion.arg1) == constants.end())
            constants.push_back(conclusion.arg1);
        if (conclusion.arg2[0] != 'x' && find(constants.begin(), constants.end(), conclusion.arg2) == constants.end())
            constants.push_back(conclusion.arg2);
    }
    vector<string> premises = sentence.GetPremise();
    for (int i=0; i<premises.size(); i++)
    {
        Predicate premise = TextParser::GetPredicate(premises[i]);
        if (premise.arg1[0] != 'x' && find(constants.begin(), constants.end(), premise.arg1) == constants.end())
            constants.push_back(premise.arg1);
        if (premise.arg2[0] != 'x' && find(constants.begin(), constants.end(), premise.arg2) == constants.end())
            constants.push_back(premise.arg2);
    }
}

/*bool KnowLedgeBasedAgent::Ask(string query)
{
    Predicate p = TextParser::GetPredicate(query);
    if (predicateMap.find(p.name) != predicateMap.end())
    {
        bool flag = true;
        vector<int> conclusions = predicateMap[p.name];
        for (int i=0; i < conclusions.size(); i++)
        {
            Sentence sentence = KnowledgeBase[conclusions[i]-1];
            string substitution = "";
            Predicate s = TextParser::GetPredicate(sentence.GetConclusion());
            if(s.equals(p))
            {
                return true;
            }
            bool unificationResult = Unify(p, s, substitution);
            if (unificationResult)
            {
                vector<string> premise = sentence.GetPremise();
                for (int j=0; j<premise.size(); j++)
                {
                    string newQuery = Substitute(premise[j], substitution);
                    bool conjunctValidity = Ask(newQuery);
                    if (!conjunctValidity)
                    {
                        flag = false;
                        break;
                    }
                    else flag = true;
                }
                if (!flag) continue;
                return true;
            }
            else flag = false;
        }
        if (flag) return true;
        else return false;
        
    }
    return false;
}*/

bool KnowLedgeBasedAgent::Ask(string query)
{
    string substitution = "";
    return askOr(query, substitution);
}

bool KnowLedgeBasedAgent::askOr(string query, string& theta)
{
    Predicate p = TextParser::GetPredicate(query);
    if (predicateMap.find(p.name) != predicateMap.end())
    {
        vector<int>rules = predicateMap[p.name];
        for (int ruleIterator = 0; ruleIterator < rules.size(); ruleIterator++)
        {
            Sentence sentence = KnowledgeBase[rules[ruleIterator]-1];
            vector<string> premise = sentence.GetPremise();
            string conclusion = sentence.GetConclusion();
            Predicate q = TextParser::GetPredicate(conclusion);
            string substitution = "";
            bool unificationResult = Unify(p, q, substitution);
            if (!unificationResult) continue;
            theta = *new string(substitution);
            bool validity = askAnd(premise, substitution);
            if (validity) return true;
        }
        return false;
    }
    else return false;
}

bool KnowLedgeBasedAgent::askAnd(vector<string> conjuncts, string substitution)
{
    if (conjuncts.size() == 0) return true;
    string first = conjuncts[0];
    vector<string> rest = *new vector<string>(conjuncts.begin()+1, conjuncts.end());
    if (substitution.compare(""))
        first = Substitute(first, substitution);
    if (askOr(first, substitution))
    {
        bool validity = askAnd(rest, substitution);
        if (!validity) return false;
    }
    else return false;
    return true;
}

string KnowLedgeBasedAgent::Substitute(string originalString, string substitution)
{
    string term = *new string(originalString);
    unsigned long index = term.find('x');
    if (index == string::npos)
    {
        return originalString;
    }
    term.replace(index, 1, substitution);
    index = term.find('x', index);
    
    return term;
}

bool KnowLedgeBasedAgent::Unify(Predicate p1, Predicate p2, string& substitution)
{
    if (p1.arg1.empty() && p2.arg1.empty())
    {
        if (p1.arg2.at(0) != 'x' && p2.arg2.at(0) != 'x' && p1.arg2.compare(p2.arg2)) return false;
        else if(p1.arg2.at(0) == 'x') substitution = *new string(p2.arg2);
        else if(p2.arg2.at(0) == 'x') substitution = *new string(p1.arg2);
        return true;
    }
    else if (p1.arg1.at(0) == 'x' || p1.arg2.at(0) == 'x')
    {
        if (p1.arg1.at(0) == 'x' && !p1.arg2.compare(p2.arg2))
        {
            substitution = *new string(p2.arg1);
            return true;
        }
        else if (p1.arg2.at(0) == 'x' && !p1.arg1.compare(p2.arg1))
        {
            substitution = *new string(p2.arg2);
            return true;
        }
    }
    else if (p2.arg1.at(0) == 'x' || p2.arg2.at(0) == 'x')
    {
        if (p2.arg1.at(0) == 'x' && !p2.arg2.compare(p1.arg2))
        {
            substitution = *new string(p1.arg1);
            return true;
        }
        else if (p2.arg2.at(0) == 'x' && !p2.arg1.compare(p1.arg1))
        {
            substitution = *new string(p1.arg2);
            return true;
        }
    }
    else if (p1.arg1.at(0) != 'x' && p1.arg2.at(0) != 'x' && p2.arg1.at(0) != 'x' && p2.arg2.at(0) != 'x' && !p1.arg1.compare(p2.arg1) && !p1.arg2.compare(p2.arg2))
        return true;
    return false;
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