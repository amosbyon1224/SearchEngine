#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include "WebPage.h"
#include <vector>
#include <map>
#include "Set.h"
#include <deque>

class SearchEngine  {
    public:
        // Constructors and Destructor
        SearchEngine();
        SearchEngine(std::string fname);
        ~SearchEngine();

        // Process a string
        std::deque<WebPage*> processSearch(std::string inStr, bool usePR);

        // Get page lookup
        WebPage* lookUp(const std::string inStr) const;

    private:
        // Variables
        std::vector<WebPage*> pages;
        std::map<std::string,Set<WebPage*> > queries;
        std::map<std::string,WebPage*> pageNames;

        // Processing searches
        // Generate Set of items with specified search terms
        Set<WebPage*> processAND(const std::string line);   // Processing for AND case
        Set<WebPage*> processOR(const std::string line);    // Processing for OR case
        Set<WebPage*> processSingle(const std::string inStr, const std::string line);   // Processing for only 1 word
        Set<WebPage*> generateQuery(const std::string query) const; // Add new search to map, so don't need to recreate later
        // Page rank applications
        Set<WebPage*> generateT(const Set<WebPage*>& S) const;  // Expand set to 1-away.
        std::map<std::string,Set<WebPage*> > generateAdjacency(const Set<WebPage*>& T) const;   // Generate adjacency list
        std::deque<WebPage*> SetToDeque(Set<WebPage*>& source) const;   // Change Set to sorted std::deque

        // Utilities
        bool alphanumeric(const char ch) const; // Returns true iff character is alphanumeric
        bool whiteSpace(const std::string str) const;   // Returns true iff string is all whitespace

};

#endif
