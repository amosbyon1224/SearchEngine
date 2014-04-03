#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include "WebPage.h"
#include <vector>
#include <map>
#include "Set.h"

class SearchEngine  {
    public:
        // Constructors and Destructor
        SearchEngine();
        SearchEngine(std::string fname);
        ~SearchEngine();

        // Process a string
        Set<WebPage*> processSearch(std::string inStr);

        // Get page lookup
        WebPage* lookUp(const std::string inStr) const;

    private:
        // Variables
        std::vector<WebPage*> pages;
        std::map<std::string,Set<WebPage*> > queries;
        std::map<std::string,WebPage*> pageNames;

        // Processing
        Set<WebPage*> processAND(std::string line);
        Set<WebPage*> processOR(std::string line);
        Set<WebPage*> processSingle(std::string inStr, std::string line);
        Set<WebPage*> generateQuery(std::string query);

        // Page Rank
        std::map<std::string,Set<WebPage*> > generateAdjacency(const Set<WebPage*>& S);

        // Utilities
        bool alphanumeric(char ch);
        bool whiteSpace(std::string str);

};

#endif
