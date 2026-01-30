//
//  Cli.hpp
//  aaasdd
//
//  Created by Arshia Taghavinejad on 2024-01-01.
//

#ifndef Cli_hpp
#define Cli_hpp

#include <iostream>
#include <string>
using namespace std;

void printMenu();
int getChoice();
string getWord(const string& prompt, bool allowWildcard = false);



#endif /* Cli_hpp */
