

//
//  main.cpp
//  dwtt
//
//  Created by Arshia Taghavinejad on 2023-12-19.
//


#include "Cli.hpp"
#include "Trie.hpp"
#include <iostream>

using namespace std;

int main() {
    Trie trie;
    int choice;
    string word;

    do {
        printMenu();       
        choice = getChoice(); 

        switch (choice) {
            case 1: { 
                word = getWord("Enter word to insert: ");
                trie.insert(word);
                cout << "\"" << word << "\" has been inserted into the Trie.\n";
                break;
            }
            case 2: { 
                word = getWord("Enter word to search: ");
                if(trie.search(word)) {
                    cout << "\"" << word << "\" is found in the Trie.\n";
                } else {
                    cout << "\"" << word << "\" is not found in the Trie.\n";
                }
                break;
            }
            case 3: { 
                word = getWord("Enter word to delete: ");
                trie.remove(word);
                cout << "\"" << word << "\" has been removed from the Trie, if it existed.\n";
                break;
            }
            case 4: { 
                trie.applyWeightDecay();
                cout << "Weight decay applied to all nodes.\n";
                break;
            }
            case 5: { 
                int currentTime;
                cout << "Enter the current time for optimization: ";
                cin >> currentTime;
                trie.optimizePaths(currentTime);
                cout << "Paths optimized based on usage and time.\n";
                break;
            }
            case 6: { 
                trie.view();
                break;
            }
            case 7: { 
                printMenu();
                break;
            }
            case 8: { 
                word = getWord("Enter prefix: ");
                for (const auto& result : trie.searchPrefix(word)) {
                    cout << result << endl;
                }
                break;
            }
            case 9: { 
                word = getWord("Enter search pattern (use '*' for wildcard): ");
                for (const auto& result : trie.wildcardSearch(word)) {
                    cout << result << endl;
                }
                break;
            }
            case 10: { 
                word = getWord("Enter prefix for suggestions: ");
                for (const auto& suggestion : trie.autoComplete(word)) {
                    cout << suggestion << endl;
                }
                break;
            }
            case 11: 
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);

    return 0; 
}
