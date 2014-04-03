#include "WebPage.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

// Empty constructor
WebPage::WebPage() : rawfile(), words(), fname() {}

// Construct with just filename; checks if file exists
WebPage::WebPage(std::string filename) : fname(filename) {
    // Try to open the file to see if it exists
    std::ifstream in;
    in.open(filename);
    if(in.fail()) throw std::invalid_argument("Passed file did not open.");
    in.close();
}

// Destructor... default is fine; we don't assign anything
WebPage::~WebPage() {}

// Get the set of all words
Set<std::string> WebPage::allWords() const    {
    return words;
}

std::ostream& operator<<(std::ostream &os,const WebPage& page)    {
    os << page.rawfile;
    return os;
}

void WebPage::parse(const std::map<std::string,WebPage*>& pageNames)   {
    // Open up file
    std::ifstream in;
    in.open(fname);
    if(in.fail()) throw std::invalid_argument("Passed file did not open.");

    // Parse file into words and get the links out
    // These are for getting lines
    // char ch;
    // char toLower = 'a' - 'A';
    std::string line="",temp="";
    // These are for processing links
    std::string linkSyntax = "](", linkText;
    size_t endBracket, startBracket, endParentheses;
    // Take one line at a time
    while(getline(in,line))  {
        // Get the links and remove all the stuff around the anchor text
        endBracket = line.find(linkSyntax);
        // While we still find linkSyntax indicators
        while(endBracket != std::string::npos) {
            startBracket = line.rfind('[',endBracket);
            endParentheses = line.find(')',endBracket);
            // Did we get matches on both sides?
            if(startBracket == std::string::npos || endParentheses == std::string::npos)
                break;
            else    {
                // Get the link
                linkText = line.substr(endBracket+2,endParentheses - endBracket-2);
                // Find the corresponding page
                try {
                    // Again, should be by reference so not memory waste
                    WebPage* wp = pageNames.at(linkText);
                    // Add to link things
                    wp->inLinks.insert(this);
                    outLinks.insert(wp);

                }
                catch(std::exception &e) {
                    std::cout << "Broken Link" << std::endl;
                }

                // Erase Everything around the anchor text
                line.erase(endBracket,endParentheses-endBracket+1);
                line.erase(startBracket,1);
            }

            // Find the next one
            endBracket = line.find(linkSyntax);
        }

        // Add contents of each line to rawfile
        rawfile += line += "\n";

        temp = line;
        // Lowercase
        std::transform(temp.begin(),temp.end(),temp.begin(),::tolower);

        char a[temp.size()+1];
        strcpy(a,temp.c_str());
        char* b = strtok(a," <.>.?/:;{[}]|-_+\"=)(*&^$#@!~`");
        while(b != NULL)    {
            try {
                words.insert(b);
            }
            // If it fails, it's fine
            catch(std::exception &e)    {}
            b = strtok(NULL," <.>.?/:;{[}]|-_+\"=)(*&^$#@!~`");
        }
    }
    in.close();
}

Set<WebPage*> WebPage::allOutgoingLinks() const  {return outLinks;}
Set<WebPage*> WebPage::allIncomingLinks() const  {return inLinks;}
std::string WebPage::filename() const    {return fname;}
