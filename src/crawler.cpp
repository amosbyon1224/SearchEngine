#include "WebPage.h"
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

void dfs(string inputFile,char* outputFile, set<string>* explored);

int main(int argc, char* argv[])    {
    // Check for correct number of arguments and open filestream
    if(argc < 3)    {
        cerr << "Incorrect number of command line arguments." << endl;
        cerr << "Try again with a seed file." << endl;
    }

    ifstream in(argv[1]);
    if(in.fail())   {
        cerr << "Could not open seed file." << endl;
    }

    // Test output stream & clear output file
    ofstream out(argv[2],ios::out | ios::trunc);
    if(!out.is_open())  {
        cerr << "Could not open output file." << endl;
    }
    out.close();

    // Depth first search!
    string fname;
    set<string>* explored = new set<string>;   // Int is 1 if not complete, 2 if visited

    while(in >> fname)  {
        if(explored->find(fname) == explored->end()) dfs(fname,argv[2],explored);
    }

    in.close();

    return 0;
}

void dfs(string inputFile,char* outputFile, set<string>* explored) {
    // Open input filestream
    ifstream in(inputFile);
    if(in.fail())   {
        cerr << "File " << inputFile << " failed to open." << endl;
    }

    // Do output
    ofstream out;
    out.open(outputFile, ios::out | ios::app);  // want to append
    if(!out.is_open())  {
        cerr << "File " << outputFile << " failed to open." << endl;
    }
    out << inputFile << endl;   // Write down that we found this page
    out.close();

    // List that we've explored this file
    explored->insert(inputFile);

    // Find links and dfs on them
    string line="", linkSyntax = "](", linkText;
    size_t endBracket, startBracket, endParentheses;
    // Take one line at a time
    while(getline(in,line))  {
        // Find signs of a link
        endBracket = line.find(linkSyntax);
        // Get the other things
        while(endBracket != string::npos) {
            startBracket = line.rfind('[',endBracket);
            endParentheses = line.find(')',endBracket);
            // Did we get matches on both sides?
            if(startBracket == std::string::npos || endParentheses == std::string::npos)
                break;
            else    {
                // Get the link
                linkText = line.substr(endBracket+2,endParentheses - endBracket-2);
                // DFS that one as well if that file doesn't already have an entry
                if(explored->find(linkText) == explored->end())   {
                    dfs(linkText,outputFile,explored);
                }

                // Erase so we don't try to reprocess
                line.erase(endBracket,endParentheses-endBracket+1);
                line.erase(startBracket,1);
            }

            // Find the next one
            endBracket = line.find(linkSyntax);
        }
    }

    // We're done now; no need to turn black
}
