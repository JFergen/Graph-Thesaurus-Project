//
// Created by Joseph Fergen on 4/14/2019.
//

#ifndef INC_2110PROJECT3_GRAPH_H
#define INC_2110PROJECT3_GRAPH_H
#include <iostream>
#include <string>
#include <list>
using namespace std;

class Graph
{
public:
    Graph();
    void SetStartWord(string s);
    void NextSynWord(string s);
    string printStartWord();
    void PrintSynonyms();
private:
    string startWord;
    list<string> synWords;
};


#endif //INC_2110PROJECT3_GRAPH_H
