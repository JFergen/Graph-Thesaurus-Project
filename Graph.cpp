//
// Created by Joseph Fergen on 4/14/2019.
//

#include "Graph.h"

Graph::Graph()
{
    startWord = "";
}

void Graph::SetStartWord(string s)
{
    startWord = s;
}

void Graph::NextSynWord(string s)
{
    synWords.push_back(s);
}

string Graph::printStartWord()
{
    return startWord;
}

void Graph::PrintSynonyms()
{
    for (auto it = synWords.begin(); it != synWords.end(); it++)
    {
        cout << *it << " ";
    }
}