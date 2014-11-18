//
//  main.cpp
//  LogicInference
//
//  Created by Varun Varadarajan on 11/12/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#include "Input.h"
#include "KnowledgeBasedAgent.h"

using namespace std;

int main(int argc, const char * argv[]) {
    Input input("/Users/varun/Documents/Artificial Intelligence/LogicInference/Input/input.txt");
    vector<string>sentences = input.GetSentenceList();
    KnowLedgeBasedAgent agent = *new KnowLedgeBasedAgent();
    int numberOfSentences = input.GetNumberOfSentences();
    for(int sentence = 0; sentence<numberOfSentences; sentence++)
        agent.Tell(sentences[sentence]);
    return 0;
}
