#ifndef WEBPAGE_H
#define WEBPAGE_H
#include "Set.h"
#include <map>
#include <iostream>
#include <cstring>

class WebPage {
    public:
        WebPage ();    // constructor that creates an empty page
        WebPage (std::string filename);
        /* constructor that initializes the page from the given file.
           Should throw an exception if the file does not exist
           or has other problems. */
        ~WebPage ();   // destructor

        Set<std::string> allWords () const;
        /* Returns a set containing all individual words on the web page. */   

        friend std::ostream & operator<< (std::ostream & os, const WebPage & page);
        /* Declares an operator we can use to print the web page. */

        Set<WebPage*> allOutgoingLinks () const;
        /* Returns "pointers" to all webpages that this page has links to. 
           As discussed above, this could be as a set or via an iterator,
           and it could be as actual pointers, or as strings, 
           or possibly other. */   

        Set<WebPage*> allIncomingLinks () const;
        /* Returns "pointers" to all webpages that link to this page. 
           Same consideration as previous function. */   

        std::string filename () const;
        /* Returns the filename from which this page was read. */

        void parse(const std::map<std::string,WebPage*>& pageNames);
        /* actually parses the content of the filename that was passed 
           to the constructor into the object. */
        
        std::string rawfile;
    private:
        Set<std::string> words;
        Set<WebPage*> outLinks;
        Set<WebPage*> inLinks;
        std::string fname;
        // you get to decide what goes here.
};
#endif
