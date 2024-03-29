#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc30-c"
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

string getSynonym(string key, int rand); // Gets the synonyn of a word based on # of hops

// These are used in conjunction for DFS searching algorithm
int DFSUtil(const string& node, const string& end, set<string>& visited);
int DFS(string start, string end);

map<string, set<string>> totalGraph; // BIG GRAPH BOI

int main()
{
    int nTimes = 0; // Number of words outputted (used only for 1st word of paragraph)
    string fileName;
    ifstream fin; // Used to parse through files
    ifstream fin2;
    string firstWord; // This is the first word of every line (the main vertices)
    string vertKey; // Used for getting key of the map
    string otherWords; // Every word past the ':' (the synonyms)
    string paragraph1Words; // Used to read through paragraph1
    string paragraph2Words; // Used to read through paragraph2
    set<string>::iterator removeThis; // Used to iterate through my set to remove the key from it
    int userChoice;
    string paragraphWords; // Used to read each word in paragraph
    int userRandTimes; // Number of times the user wants to hop randomly
    char puncChar = 0; // Used to see if final character of string is punctuation character
    bool capLetter; // Used to see if the first letter is capital or not
    bool hasPunc; // If the last character is punc
    
    srand(time(nullptr)); // Seeds rand()
    rand();
    
    cout << "Enter filename containing words and synonyms:";  // Gets filename to open from user
    cin >> fileName;
    cout << endl;
    
    fin.open(fileName.c_str()); // Opens main file for input
    if(!fin.is_open())
    {
        cout << fileName <<  " could not be found";
        return EXIT_FAILURE;
    }
    
    // Reads through all the starting data and puts it into the graph
    while(getline(fin, firstWord, ':'))
    {
        set<string> bigSet; // Beginning set
        set<string> smallSet; // This set is unioned with bigSet
        bigSet.insert(firstWord);
    
        getline(fin, otherWords);
        istringstream commasGoAway(otherWords); // Used to get rid of commas
    
        // Gets rid of commas and inserts all the words into a temp set;
        while(getline(commasGoAway, otherWords, ','))
        {
            bigSet.insert(otherWords);
        }
        
        for(auto it = bigSet.begin(); it != bigSet.end(); it++)
        {
            smallSet.clear();
            
            if(totalGraph.find(*it) == totalGraph.end())
            {// not found in map
                vertKey = *it; // Key of the map
                smallSet = bigSet; // smallSet is used for deleting the key from the set before inserting
    
                // Removes the key from the set
                for(auto itr = smallSet.begin(); itr != smallSet.end(); itr++)
                {
                    if(*itr == vertKey)
                    {
                        removeThis = itr;
                    }
                }
                smallSet.erase(removeThis);
                
                totalGraph.insert(pair<string, set<string>>(vertKey, smallSet));
            } else
            { // found in the map
                vertKey = *it;
                
                set_union(totalGraph[*it].begin(), totalGraph[*it].end(), bigSet.begin(), bigSet.end(),
                        inserter(smallSet, smallSet.begin())); // Combines the old set with new set
    
                //Removes key from set
                for(auto itr = smallSet.begin(); itr != smallSet.end(); itr++)
                {
                    if(*itr == vertKey)
                    {
                        removeThis = itr;
                    }
                }
                smallSet.erase(removeThis);
                
                totalGraph[*it] = smallSet; // Setting the set to be the combined set
            }
        }
    }
    fin.close();
    
    // Getting user choice for what to do
    cout << "1 for random synonym replacement. 2 for performing a paragraph similarity analysis:";
    cin >> userChoice;
    cin.ignore();
    cout << endl;
    
    // Making sure user chooses 1 or 2
    while(userChoice != 2 && userChoice != 1)
    {
        cout << "Invalid choice. Try again." << endl <<
            "1 for random synonym replacement. 2 for performing a paragraph similarity analysis:" << endl;
        cin >> userChoice;
    }
    
    switch(userChoice)
    {
        case 1: // Random synonym replacement
            cout << "Enter filename containing paragraph for replacement:";
            cin >> fileName;
            cout << endl;
        
            fin.open(fileName.c_str()); // Opens main file for input
            if(!fin.is_open())
            {
                cout << fileName <<  " could not be found";
                return EXIT_FAILURE;
            }
            
            cout << "Enter the number of times the words should be swapped (>= 1):";
            cin >> userRandTimes;
            
            while(userRandTimes < 1)
            {
                cout << "Error. Please enter a number that is >= 1:";
                cin >> userRandTimes;
            }
        
            while(getline(fin, paragraphWords, ' '))
            {
                capLetter = false;
                hasPunc = false;
                
                // If the last character is punctuation
                if(ispunct(paragraphWords.back()))
                {
                    hasPunc = true;
                    puncChar = paragraphWords.back(); // Gets last character of each string (to check for punctuation)
                    paragraphWords.pop_back();
                }
    
                // If the first character is a capital letter
                if(isupper(paragraphWords.front()))
                {
                    capLetter = true;
                    paragraphWords[0] = tolower(paragraphWords[0]);
                }
                
                if(totalGraph.find(paragraphWords) == totalGraph.end()) // If I am not changing the word, then just print it out
                {// Not found in map
                    
                    if(hasPunc)
                    {
                        paragraphWords += puncChar;
                    }
    
                    if(capLetter)
                    {
                        paragraphWords[0] = toupper(paragraphWords[0]);
                    }
                    
                    if(nTimes == 0) // If it is the first word of the paragraph;
                    {
                        cout << paragraphWords;
                        nTimes++;
                        continue;
                    }
                    cout << " " << paragraphWords;
                    continue;
                }
                else // If it is in the map
                {
                    paragraphWords = getSynonym(paragraphWords, userRandTimes); // Gets the synonym according to # of hops
    
                    // Change the first letter of synonym to capital if need be
                    if(capLetter)
                    {
                        paragraphWords[0] = toupper(paragraphWords[0]);
                    }
                    
                    if(hasPunc)
                    {
                        paragraphWords += puncChar;
                    }
                    
                    if(nTimes == 0)
                    {
                        cout << paragraphWords;
                        nTimes++;
                        continue;
                    }
                    else
                    {
                        cout << " " << paragraphWords;
                        continue;
                    }
                }
            }
            fin.close();
            break;
        case 2: // Paragraph similarity analysis
            cout << "Enter filename containing first paragraph for analysis:";
            cin >> fileName;
            cout << endl;
        
            fin.open(fileName.c_str()); // Opens main file for input
            if(!fin.is_open())
            {
                cout << fileName <<  " could not be found";
                return EXIT_FAILURE;
            }
        
            cout << "Enter filename containing second paragraph for analysis:";
            cin >> fileName;
            cout << endl;
        
            fin2.open(fileName.c_str()); // Opens main file for input
            if(!fin2.is_open())
            {
                cout << fileName <<  " could not be found";
                return EXIT_FAILURE;
            }
            
            while(getline(fin, paragraph1Words, ' '))
            {
                getline(fin2, paragraph2Words, ' ');
                
                if(paragraph1Words != paragraph2Words)
                {
                    // If the last character is punctuation
                    if(ispunct(paragraph1Words.back()))
                    {
                        paragraph1Words.pop_back();
                        paragraph2Words.pop_back();
                    }
    
                    // If the first character is a capital letter
                    if(isupper(paragraph1Words.front()))
                    {
                        paragraph1Words[0] = tolower(paragraph1Words[0]);
                        paragraph2Words[0] = tolower(paragraph2Words[0]);
                    }
    
                    // If one word is not in the map
                    if(totalGraph.find(paragraph1Words) == totalGraph.end() || totalGraph.find(paragraph2Words) == totalGraph.end())
                    {
                        cout << "Paragraphs are not similar." << endl;
                        fin.close();
                        fin2.close();
                        return 0;
                    }
                    
                    // DFS to find if the words are connected
                    if(DFS(paragraph1Words, paragraph2Words) == 2)
                    { // Not connected
                        cout << "Paragraphs are not similar." << endl;
                        fin.close();
                        fin2.close();
                        return 0;
                    }
                }
            }
            
            cout << "Paragraphs are similar." << endl;
            fin.close();
            fin2.close();
            break;
        default:
            cout << "Please don't go here somehow";
            return 1;
    }
    
    return 0;
}

string getSynonym(string key, int randTimes)
{
    int randWordNum = 0;
    int i = 1;
    string prevWord;
    
    do
    {
        randWordNum = rand() % totalGraph[key].size(); //
        
        auto itr = totalGraph[key].begin();
        advance(itr, randWordNum);
        i++;
        
        if(*itr == prevWord)
        {
            randWordNum = rand() % totalGraph[key].size();
            advance(itr, randWordNum);
        }
        
        if(i == randTimes)
        {
            return *itr;
        }
        
        key = *itr;
        prevWord = *itr;
    }while(i != randTimes);
}

//Basis is from Geeks4Geeks
int DFSUtil(const string& node, const string& end, set<string>& visited)
{
    // Mark the current node as visited
    visited.insert(node);
    
    if(node == end)
    {
        return 1;
    }
    
    // Recur for all the vertices adjacent to this vertex
    for(auto it = totalGraph.at(node).begin(); it != totalGraph.at(node).end(); it++)
    {
        if(visited.find(*it) == visited.end())
        {
            DFSUtil(*it, end, visited);
        }
    }
    return 2;
}

// Basis is from Geeks4Geeks
// The function to do DFS traversal. It uses recursive DFSUtil()
int DFS(string start, string end)
{
    set<string> visitedSet; // Holds the strings of where it has visited
    
    // Call the recursive helper function to print DFS traversal
    // starting from all vertices one by one
    DFSUtil(start, end, visitedSet);
}

#pragma clang diagnostic pop