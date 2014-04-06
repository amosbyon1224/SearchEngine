#include "SearchEngine.h"
#include "msort.h"
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>

// Comparator object
bool AlphaWPComp::operator()(const WebPage* lhs, const WebPage* rhs)    {
    return lhs->filename() < rhs->filename();
}

//Constructors
SearchEngine::SearchEngine()    {}
SearchEngine::SearchEngine(std::string fname)    {
    // Open file
    std::ifstream in;
    in.open(fname);
    if(in.fail()) throw std::invalid_argument("Could not open file");

    // Make the web pages; they'll be empty
    std::string filename;
    while(in >> filename)  {
        pageNames.insert(std::pair<std::string,WebPage*>(filename,new WebPage(filename)));
    }
    in.close();

    // Parse the webpages
    for(std::map<std::string,WebPage*>::iterator it = pageNames.begin(); it != pageNames.end(); ++it)  {
        it->second->parse(pageNames);
        pages.push_back(it->second);
    }
}

// Destructor
SearchEngine::~SearchEngine()   {
    for(std::vector<WebPage*>::iterator it = pages.begin(); it != pages.end(); ++it)  {
        delete *it;
    }
} 

// Look up Item
WebPage* SearchEngine::lookUp(const std::string inStr) const    {
    try {
        return pageNames.at(inStr);
    }
    catch(std::exception &e) {
        throw std::invalid_argument("File does not exist");
    }
}

// Process Search
std::deque<WebPage*> SearchEngine::processSearch(std::string inStr,bool usePR) {
    // Bring input to lowercase
    std::string line;
    std::transform(inStr.begin(),inStr.end(),inStr.begin(),::tolower);

    // Get only the first word
    std::stringstream ss;
    ss << inStr;
    ss >> inStr;
    // And then the rest of the line
    getline(ss,line);

    // Add on blank space to allow last word to be processed if AND/OR
    line += "\n";

    // Get Set S
    Set<WebPage*> S;
    if(inStr == "and") S = processAND(line);
    else if(inStr == "or") S = processOR(line);
    else S = processSingle(inStr,line);

    // Generate T
    Set<WebPage*> T = generateT(S);

    return SetToDeque(T);
    /*
    // Then make adjacency matrix & generate page rank
    if(usePR) return FUNCTIONNAME(generateAdjacency(T));
    else return SetToDeque(T);
    */
}

Set<WebPage*> SearchEngine::generateT(const Set<WebPage*>& S) const {
    Set<WebPage*> T(S);

    // For each entry
    for(Set<WebPage*>::const_iterator it = S.begin(); it != S.end(); ++it)    {
        // Add all its outgoing links
        Set<WebPage*> outgoing = (*it)->allOutgoingLinks();
        for(Set<WebPage*>::iterator oit = outgoing.begin(); oit != outgoing.end(); ++oit) {
            T.insert(*oit);
        }

        // Add all its incoming links
        Set<WebPage*> incoming = (*it)->allIncomingLinks();
        for(Set<WebPage*>::iterator iit = incoming.begin(); iit != incoming.end(); ++iit)   {
            T.insert(*iit);
        }
    }
    return T;
}

std::map<std::string,Set<WebPage*> > SearchEngine::generateAdjacency(const Set<WebPage*>& T) const  {
    std::map<std::string,Set<WebPage*> > adjList;   // Actually only contains the outgoing links in set T

    // Only care about intersection of outgoing and T.
    for(Set<WebPage*>::const_iterator it = T.begin(); it != T.end(); ++it)    {
        // Find what outgoing links are a member of T and add them to adjList entry
        adjList[(*it)->filename()] = T.setIntersection((*it)->allOutgoingLinks());
    }

    return adjList;
}

// Take all elements of Set and place them in sorted deque
std::deque<WebPage*> SearchEngine::SetToDeque(Set<WebPage*>& source) const  {
    std::deque<WebPage*> destination;
    for(Set<WebPage*>::iterator it = source.begin(); it != source.end(); ++it)  {
        destination.push_back(*it);
    }

    AlphaWPComp comp;
    mergeSort(destination,comp);
    return destination;
}

Set<WebPage*> SearchEngine::processAND(const std::string line) {
    Set<WebPage*> result,singleword;
    std::string word="";

    // Go through and generate all the queries
    for(unsigned int i=0; i<line.size(); i++)    {
        if(alphanumeric(line[i])) word += line[i];
        else    {
            // This means we're at end of the word - check for content
            if(word.size() > 0) {
                // Put the query to lowercase and try to get; if not, make an entry if necessary
                std::transform(word.begin(),word.end(),word.begin(),::tolower);
                singleword = processSingle(word,"");
                // Then add it to result
                if(result.empty()) result = singleword;
                else result = result.setIntersection(singleword);
                // Reset word for the next word in query
                word = "";
            }
        }
    }
    return result;
}

// What to do when the user issues an or command
Set<WebPage*> SearchEngine::processOR(const std::string line)  {
    Set<WebPage*> result,singleword;
    std::string word="";

    // Go through and generate all the queries
    for(unsigned int i=0; i<line.size(); i++)    {
        if(alphanumeric(line[i])) word += line[i];
        else    {
            // This means we're at end of the word - check for content
            if(word.size() > 0) {
                // Put the query to lowercase and try to get; if not, make an entry if necessary
                std::transform(word.begin(),word.end(),word.begin(),::tolower);
                singleword = processSingle(word,"");
                // Then add it to result
                if(result.empty()) result = singleword;
                else result = result.setUnion(singleword);
                // Reset word for the next word in query
                word = "";
            }
        }
    }
    return result;
}

Set<WebPage*> SearchEngine::processSingle(const std::string inStr, const std::string line)  {
    Set<WebPage*> word;

    //Check if valid input
    if(whiteSpace(line))  {
        // Get corresponding set if it exists
        try {
            word = queries.at(inStr);
        }
        // If not, generate it
        catch(std::exception &e) {
            word = generateQuery(inStr);
            queries.insert(std::pair<std::string,Set<WebPage*> >(inStr,word));
        }
    }
    return word;
}

Set<WebPage*> SearchEngine::generateQuery(const std::string query) const    {
    Set<WebPage*> containing;
    for(std::vector<WebPage*>::const_iterator it = pages.begin(); it != pages.end(); ++it)  {
        Set<std::string> words = (*it)->allWords();
        if(words.find(query) != words.end()) containing.insert(*it);
    }
    return containing;
}

// Checks if char is alphanumeric
bool SearchEngine::alphanumeric (const char ch) const {
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch<='Z') || (ch>='0' && ch<='9'));
}

// Checks if string is only whitespace
bool SearchEngine::whiteSpace (const std::string str) const {
    for(unsigned int i=0; i<str.size(); i++) {
        if (!(str[i] == ' ' || str[i] == '\t' || str[i] == '\n')) return false;
    }
    return true;
}
