#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <deque>
#include <map>

//QT includes
#include <QApplication>
#include <QtGui>
#include <QTCore>

#include "WebPage.h"
#include "lib/Set.h"

using namespace std;

Set<WebPage*> andSearchResult(string query, map<string, Set<WebPage*> > myMap);
Set<WebPage*> orSearchResult(string query, map<string, Set<WebPage*> > myMap);

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	QWidget *window = new QWidget;
	window->setWindowTitle("Amos Byon Search Engine CSCI 104");

	QLineEdit *searchBox = new QLineEdit();

	QPushButton *searchButton = new QPushButton("Search");
	QPushButton *aboutButton = new QPushButton("About");
	QPushButton *quitButton = new QPushButton("Quit");

	QRadioButton *singleRadioButton = new QRadioButton("Single Word Query");
	QRadioButton *andRadioButton = new QRadioButton("AND");
	QRadioButton *orRadioButton = new QRadioButton("OR");

	QListWidget *list = new QListWidget();

	QGridLayout *layout = new QGridLayout;

	layout->addWidget(searchBox, 0, 0, 1, 5);
	layout->addWidget(searchButton, 0, 5);
	layout->addWidget(singleRadioButton, 1, 0);
	layout->addWidget(andRadioButton, 1, 1);
	layout->addWidget(orRadioButton, 1, 2);
	layout->addWidget(list, 2, 0, 5, 6);
	layout->addWidget(aboutButton, 7, 4);
	layout->addWidget(quitButton, 7, 5);

	//check command line arguments
	if(argc != 2){
		cout << "There was a problem with your command line argument.\n";
		cout << "Usage: open ./project_abyon sampleFile.txt\n";
		cout << "Program will now terminate\n";
		//return 0;
	}

	//open file and try to read from it
	ifstream inf (argv[1]);
	string temp;
	deque<WebPage*> websites;
	map<string, WebPage*> webNames;
	WebPage* page;
	if(inf.fail()){
		cout << "There was a problem opening the file: " << argv[1] << endl;
		cout << "Program will now terminate\n";
		//return 0;
	}else{
		//store all contents of file as webpages
		while(1){
			getline(inf, temp);
			if(inf.fail()){
				break;
			}
			try{
				page = new WebPage(temp);
			}
			catch(runtime_error& e){
				cout << "There was a problem reading webpages from " << argv[1] << endl;
				cout << "Program will now terminate\n";
				//return 0;
			}
			websites.push_back(page);
			webNames[temp] = page;
		}
	}
	inf.close();

	map<string, Set<WebPage*> > myMap;
	Set<string> tempSet;
	int numWebsites = websites.size();
	Set<string>::iterator myIt;
	for(int i = 0; i < numWebsites; i++){
		tempSet = websites[i]->allWords();
		for(myIt = tempSet.begin(); myIt != tempSet.end(); ++myIt){
			if(myMap.find(*myIt) == myMap.end()){
				Set<WebPage*> myS;
				myS.insert(websites[i]);
				myMap[*myIt] = myS;
			}else{
				myMap[*myIt].insert(websites[i]);
			}
		}
	}

	int mySitesSize = websites.size();
	for(int i = 0; i < mySitesSize; i++){
		websites[i]->parse(webNames);
	}

	Set<WebPage*> searchResult = andSearchResult("good wind", myMap);
	Set<WebPage*>::iterator iter;
	for(iter = searchResult.begin(); iter != searchResult.end(); ++iter){
		QString qstr = QString::fromStdString((*iter)->filename());
		list->addItem(qstr);
	}

	/*if(andRadioButton->isChecked()){
		Set<WebPage*> searchResult = andSearchResult(searchBox->displayText().toUtf8().constData(), myMap);
		QObject::connect(searchButton, SIGNAL(clicked()), list, SLOT(clear()));
		Set<WebPage*>::iterator iter;
		for(iter = searchResult.begin(); iter != searchResult.end(); ++iter){
			QString qstr = QString::fromStdString((*iter)->filename());
			list->addItem(qstr);
		}
	}*/

	QObject::connect(quitButton, SIGNAL(clicked()), &app, SLOT(quit()));

	window->setLayout(layout);
	window->show();
	return app.exec();

	string input = "";

	//main loop of program
	//user interacts here and inputs search queries
	do{
		cout << "Enter search query:\n";
		cin >> input;

		//quits out of program in case that input is "."
		if(input.compare(".") == 0){
			break;
		}

		//if the user inputted AND for the first word
		if(input.compare("AND") == 0){
			
			string words;
			getline(cin, words);
			//converts input to all lowercase
			transform(words.begin(), words.end(), words.begin(), ::tolower);
			char *a = new char[words.size()+1];
			a[words.size()] = '\0';
			memcpy(a, words.c_str(), words.size());

			char * b;
			b = strtok(a, " (,)");

			deque<string> inputWords;
			while(b != NULL){
				inputWords.push_front(b);
				b = strtok (NULL, " (,)");
			}

			Set<WebPage*> results = myMap[inputWords.front()];
			inputWords.pop_front();

			while(!inputWords.empty()){
				results = results.setIntersection(myMap[inputWords.front()]);
				inputWords.pop_front();
			}

			cout << "There is/are " << results.size() << " page(s) with your search in it.\n";

			if(results.size() == 0){
				cout << "There were no Web Pages with your search in it.\n";
			}else{
				Set<WebPage*>::iterator iter = results.begin();
				string input2 = "";
				do{
					cout << **iter << endl;
					if((++iter) == results.end()){
						cout << "This is the last page.\n";
					}
					--iter;
					cout << "Enter a command (next, quit)\n";
					cin >> input2;
					if(input2.compare("quit") == 0){
						break;
					}
					if(input2.compare("next") == 0){
						if(++iter == results.end()){
							cout << "No more pages to show\n";
							--iter;
						}
					}
				}while(input2.compare("quit") != 0);
			}

		//if user inputted OR as first word
		}else if(input.compare("OR") == 0){
			string words;
			getline(cin, words);
			transform(words.begin(), words.end(), words.begin(), ::tolower);
			char *a = new char[words.size()+1];
			a[words.size()] = '\0';
			memcpy(a, words.c_str(), words.size());

			char * b;
			b = strtok(a, " (,)");

			deque<string> inputWords;
			while(b != NULL){
				inputWords.push_front(b);
				b = strtok (NULL, " (,)");
			}

			Set<WebPage*> results = myMap[inputWords.front()];
			inputWords.pop_front();

			while(!inputWords.empty()){
				results = results.setUnion(myMap[inputWords.front()]);
				inputWords.pop_front();
			}

			cout << "There is/are " << results.size() << " page(s) with your search in it.\n";

			if(results.size() == 0){
				cout << "There were no Web Pages with your search in it.\n";
			}else{
				Set<WebPage*>::iterator iter = results.begin();
				string input2 = "";
				do{
					cout << **iter << endl;
					if((++iter) == results.end()){
						cout << "This is the last page.\n";
					}
					--iter;
					cout << "Enter a command (next, quit)\n";
					cin >> input2;
					if(input2.compare("quit") == 0){
						break;
					}
					if(input2.compare("next") == 0){
						if(++iter == results.end()){
							cout << "No more pages to show\n";
							--iter;
						}
					}
				}while(input2.compare("quit") != 0);
			}

		}else{
			//if user only inputs one word
			if(cin.get() == 10){
				transform(input.begin(), input.end(), input.begin(), ::tolower);
				//start single word search query
				Set<WebPage*> results = myMap[input];

				cout << "There is/are " << results.size() << " page(s) with " << input << " in it.\n";
				

				if(results.size() == 0){
					cout << "There were no Web Pages with " << input << " in it.\n";
					cout << "Enter a command (next, quit)\n";
				}else{
					Set<WebPage*>::iterator iter = results.begin();
					string input2 = "";
					do{
						cout << **iter << endl;
						if((++iter) == results.end()){
							cout << "This is the last page.\n";
						}
						--iter;
						cout << "Enter a command (next, quit)\n";
						cin >> input2;
						if(input2.compare("quit") == 0){
							break;
						}
						if(input2.compare("next") == 0){
							if(++iter == results.end()){
								cout << "No more pages to show\n";
								--iter;
							}
						}
					}while(input2.compare("quit") != 0);
				}

				cin.clear();
				cin.ignore(100, '\n');
			}else{
				cout << "That was not a valid search query. Try again.\n";
				cin.clear();
				cin.ignore(100, '\n');
			}
		}

	}while(input.compare(".") != 0);

	return 0;

}

Set<WebPage*> andSearchResult(string query, map<string, Set<WebPage*> > myMap){
	//converts input to all lowercase
	transform(query.begin(), query.end(), query.begin(), ::tolower);
	char *a = new char[query.size()+1];
	a[query.size()] = '\0';
	memcpy(a, query.c_str(), query.size());

	char * b;
	b = strtok(a, " (,)");

	deque<string> inputWords;
	while(b != NULL){
		inputWords.push_front(b);
		b = strtok (NULL, " (,)");
	}

	Set<WebPage*> results = myMap[inputWords.front()];
	inputWords.pop_front();

	while(!inputWords.empty()){
		results = results.setIntersection(myMap[inputWords.front()]);
		inputWords.pop_front();
	}
	return results;
}

Set<WebPage*> orSearchResult(string query, map<string, Set<WebPage*> > myMap){
	//converts input to all lowercase
	transform(query.begin(), query.end(), query.begin(), ::tolower);
	char *a = new char[query.size()+1];
	a[query.size()] = '\0';
	memcpy(a, query.c_str(), query.size());

	char * b;
	b = strtok(a, " (,)");

	deque<string> inputWords;
	while(b != NULL){
		inputWords.push_front(b);
		b = strtok (NULL, " (,)");
	}

	Set<WebPage*> results = myMap[inputWords.front()];
	inputWords.pop_front();

	while(!inputWords.empty()){
		results = results.setUnion(myMap[inputWords.front()]);
		inputWords.pop_front();
	}
	return results;
}