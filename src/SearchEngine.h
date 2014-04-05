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
        std::deque<WebPage*> processSearch(std::string inStr);

        // Get page lookup
        WebPage* lookUp(const std::string inStr) const;

    private:
        // Variables
        std::vector<WebPage*> pages;
        std::map<std::string,Set<WebPage*> > queries;
        std::map<std::string,WebPage*> pageNames;

        // Processing
        Set<WebPage*> processAND(const std::string line);
        Set<WebPage*> processOR(const std::string line);
        Set<WebPage*> processSingle(const std::string inStr, const std::string line);
        Set<WebPage*> generateQuery(const std::string query) const;

        // Page Rank
        std::map<std::string,Set<WebPage*> > generateAdjacency(const Set<WebPage*>& S) const;

        // Utilities
        bool alphanumeric(const char ch) const;
        bool whiteSpace(const std::string str) const;

};

#endif
