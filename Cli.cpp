//
//  Cli.cpp
//  aaasdd
//
//  Created by Arshia Taghavinejad on 2024-01-01.
//

#include "Cli.hpp"
#include "Trie.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;

void printMenu() {
    cout << "\n--- Trie CLI ---\n";
    cout << "1: Insert word\n";
    cout << "2: Search word\n";
    cout << "3: Delete word\n";
    cout << "4: Apply weight decay\n";
    cout << "5: Optimize paths\n";
    cout << "6: View Trie\n";
    cout << "7: Help\n";
    cout << "8: Prefix Search\n";
    cout << "9: Wildcard Search\n";
    cout << "10: Auto-Complete\n";
    cout << "11: Exit\n";
    cout << "Choose an option: ";
}

int getChoice() {
    int choice;
    string input;
    while (true) {
        getline(cin, input);
        stringstream inputStream(input);
        if (inputStream >> choice && inputStream.eof() && choice >= 1 && choice <= 11) {
            return choice;
        } else {
            cout << "Invalid choice, please enter a number between 1 and 11.\n";
        }
    }
}

std::string getWord(const std::string& prompt, bool allowWildcard) {
    string word;
    cout << prompt;
    while (true) {
        cin >> word;
        if (!word.empty() && all_of(word.begin(), word.end(), [allowWildcard](char ch) {
                return std::isalpha(static_cast<unsigned char>(ch)) || (allowWildcard && ch == '*');
            })) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return word;
        } else {
            cout << "Invalid input, please enter a valid word (letters only";
            if (allowWildcard) {
                cout << ", '*' allowed for wildcard";
            }
            cout << "): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    }
}
