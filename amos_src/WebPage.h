// required headers go here
#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <deque>
#include <map>

#include "lib/Set.h"

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

    friend std::ostream& operator<<(std::ostream & os, const WebPage & page);
      /* Declares an operator we can use to print the web page. */

    Set<std::string> allOutgoingLinks() const;
      /* Returns "pointers" to all webpages that this page has links to. 
         As discussed above, this could be as a set or via an iterator,
         and it could be as actual pointers, or as strings, 
         or possibly other. */ 

    Set<std::string> allIncomingLinks() const;
      /* Returns "pointers" to all webpages that link to this page. 
         Same consideration as previous function. */

    std::string filename() const;
      /*Returns the filename from which this page was read. */

    void addIncomingLink(std::string link);

    void addOutgoingLink(std::string link);

    void parse(std::map<std::string, WebPage*> myMap);

  private:
    // you get to decide what goes here.
    std::string _fname;

    std::deque<std::string> _lines;

    Set<std::string> _incoming;
    Set<std::string> _outgoing;

};

#endif