#include "SearchEngine.h"
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>

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
Set<WebPage*> SearchEngine::processSearch(std::string inStr) {
    std::string line;
    std::transform(inStr.begin(),inStr.end(),inStr.begin(),::tolower);
    std::stringstream ss;
    ss << inStr;
    ss >> inStr;
    getline(ss,line);
    if(inStr == "and") return processAND(line);
    else if(inStr == "or") return processOR(line);
    else return processSingle(inStr,line);
}

Set<WebPage*> SearchEngine::processAND(std::string line) {
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
Set<WebPage*> SearchEngine::processOR(std::string line)  {
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

Set<WebPage*> SearchEngine::processSingle(std::string inStr, std::string line)  {
    Set<WebPage*> word;

    //Check if valid input
    if(whiteSpace(line))  {
        try {
            word = queries.at(inStr);
        }
        catch(std::exception &e) {
            word = generateQuery(inStr);
            queries.insert(std::pair<std::string,Set<WebPage*> >(inStr,word));
        }
    }
    return word;
}

Set<WebPage*> SearchEngine::generateQuery(std::string query)    {
    Set<WebPage*> containing;
    for(std::vector<WebPage*>::iterator it = pages.begin(); it != pages.end(); ++it)  {
        Set<std::string> words = (*it)->allWords();
        if(words.find(query) != words.end()) containing.insert(*it);
    }
    return containing;
}

// Checks if char is alphanumeric
bool SearchEngine::alphanumeric (char ch) {
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch<='Z') || (ch>='0' && ch<='9'));
}

// Checks if string is only whitespace
bool SearchEngine::whiteSpace (std::string str)   {
    for(unsigned int i=0; i<str.size(); i++) {
        if (!(str[i] == ' ' || str[i] == '\t' || str[i] == '\n')) return false;
    }
    return true;
}
