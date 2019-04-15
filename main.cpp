#include "Graph.h"
#include <fstream>
#include <vector>
#include <sstream>

int main()
{
    vector<Graph> GraphList; // Holds the entirety of the graph data
    int numWord = 0; // Holds the number of words that have synonyms
    string fileName;
    ifstream fin; // Used to parse through file
    string firstWord; // This is the first word of every line (the main vertices)
    string otherWords; // Every word past the ':' (the synonyms)
    
    cout << "Enter filename containing tables and data: ";  // Gets filename to open from user
    cin >> fileName;
    
    fin.open(fileName.c_str()); // Opens main file for input
    if(!fin.is_open())
    {
        cout << fileName <<  " could not be found";
        return EXIT_FAILURE;
    }
    
    // Reads through all the starting data and puts it into the graph
    while(getline(fin, firstWord, ':'))
    {
        Graph dataEntry;
        dataEntry.SetStartWord(firstWord); // Sets the first word for each graph entry
        
        getline(fin, otherWords); // Gets rest of line (including ',')
    
        // Used to get rid of commas
        istringstream commasGoAway(otherWords);
        while(getline(commasGoAway, otherWords, ','))
        {
            dataEntry.NextSynWord(otherWords);
        }
        
        GraphList.push_back(dataEntry);
        ++numWord;
    }
    
    fin.close();
    return 0;
}