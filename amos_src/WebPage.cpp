#include <cstdlib>
#include <stdexcept>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include "lib/Set.h"
#include "WebPage.h"

//constructor
WebPage::WebPage(){
  //imp
  _fname = "FILE_DOES_NOT_EXIST";
}

//alt constructor
WebPage::WebPage(std::string filename){
  _fname = filename;

  //converting string filename to char array
  char *a = new char[filename.size()+1];
  a[filename.size()] = '\0';
  memcpy(a, filename.c_str(), filename.size());

  std::ifstream inf (a);

  //problems opening up file
  if(inf.fail()){
    throw std::runtime_error("File failed to open");
  }

  //read in the contents of the web page
  std::string temp;
  while(1){
    getline(inf, temp);
    if(inf.fail()){
      break;
    }
    _lines.push_back(temp);
  }

  //close the file
  inf.close();
}

//deconstructor
WebPage::~WebPage(){
}

//returns a set with every word that was found in the web page
Set<std::string> WebPage::allWords() const{
  Set<std::string> words;
  std::string temp;
  int size = _lines.size();

  for(int i = 0; i < size; i++){
    temp = _lines[i];
    //convert each line to lowercase
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

    //convert to char array to use strtok to take away special characters
    char *a = new char[temp.size()+1];
    a[temp.size()] = '\0';
    memcpy(a, temp.c_str(), temp.size());

    char * b;
    b = strtok (a, " <,>.?/:;{[}]|-_+\"=)(*&^$#@!~`");
    while(b != NULL){
      try{
        words.insert(b);
        b = strtok (NULL, " <,>.?/:;{[}]|-_+\"=)(*&^$#@!~`");
      }
      catch(std::invalid_argument& e){
        b = strtok (NULL, " <,>.?/:;{[}]|-_+\"=)(*&^$#@!~`");
      }
    }

  }

  return words;
}

//operator overload for << to easily print out the webpage
std::ostream & operator<<(std::ostream & os, const WebPage & page){
  os << std::endl;
  std::string temp;

  int size = page._lines.size();

  for(int i = 0; i < size; i++){
    os << page._lines[i] << std::endl;
  }
  return os;
}

Set<std::string> WebPage::allOutgoingLinks() const{
  return _outgoing;
}

Set<std::string> WebPage::allIncomingLinks() const{
  return _incoming;
}

std::string WebPage::filename() const{
  return _fname;
}

void WebPage::addIncomingLink(std::string link){
  _incoming.insert(link);
}

void WebPage::addOutgoingLink(std::string link){
  _outgoing.insert(link);
}

void WebPage::parse(std::map<std::string, WebPage*> myMap){
  int size = _lines.size();
  std::string temp;
  std::size_t found1;
  std::size_t found2;
  std::size_t found3;
  std::size_t found4;

  for(int i = 0; i < size; i++){
    temp = _lines[i];
    found1 = temp.find_first_of("[");

    std::string anchorText;
    std::string hyperlink;

    while(found1 != std::string::npos){
      found2 = temp.find("]", found1);
      found3 = temp.find("(", found1);
      found4 = temp.find(")", found1);

      anchorText = temp.substr(found1+1, found2-found1-1);
      hyperlink = temp.substr(found3+1, found4-found3-1);

      temp.replace(found1, found4-found1+1, anchorText);
      _lines[i] = temp;

      addOutgoingLink(hyperlink);

      myMap[hyperlink]->addIncomingLink(_fname);

      found1 = temp.find_first_of("[");
    }
  }
}