#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

class TrieNode {
public:
    map<char, TrieNode*> children;
    TrieNode* shortcut;
    bool endOfWord;
    int weight;
    int lastAccessTime;

    TrieNode() : shortcut(nullptr), endOfWord(false), weight(0), lastAccessTime(0) {}
    ~TrieNode();
};

TrieNode::~TrieNode() {
    for (auto& child : children) {
        delete child.second;
        child.second = nullptr;
    }
}


class Trie {
private:
    TrieNode* root;
    int operationCount;
    const int REBALANCE_THRESHOLD = 10;
    const int DECAY_FACTOR = 1;

public:
    Trie() : operationCount(0) {
        root = new TrieNode();
    }

    ~Trie() {
        clearTrie(root);
    }
    void findWordsWithPrefix(TrieNode* node, const string& prefix, vector<string>& results, string currentWord) {
        if (node == nullptr) {
            return;
        }
        if (node->endOfWord) {
            results.push_back(currentWord);
        }
        for (auto& child : node->children) {
            findWordsWithPrefix(child.second, prefix, results, currentWord + child.first);
        }
    }

    vector<string> searchPrefix(const string& prefix) {
        vector<string> results;
        TrieNode* current = root;
        for (char ch : prefix) {
            if (current->children.find(ch) == current->children.end()) {
                return results;
            }
            current = current->children[ch];
        }
        findWordsWithPrefix(current, prefix, results, prefix);
        return results;
    }

    void searchWildcard(TrieNode* node, const string& word, int index, string currentWord, vector<string>& results) {
        if (node == nullptr) {
            return;
        }

        if (index == word.size()) {
            if (node->endOfWord) {
                results.push_back(currentWord);
            }
            return;
        }

        if (word[index] == '*') {
            for (auto& child : node->children) {
                searchWildcard(child.second, word, index + 1, currentWord + child.first, results);
            }
        } else {
            if (node->children.find(word[index]) != node->children.end()) {
                searchWildcard(node->children[word[index]], word, index + 1, currentWord + word[index], results);
            }
        }
    }

    vector<string> wildcardSearch(const string& word) {
        vector<string> results;
        searchWildcard(root, word, 0, "", results);
        return results;
    }

    
    vector<string> autoComplete(const string& prefix) {
        return searchPrefix(prefix);
    }

    void viewTrie(TrieNode* node, const string& prefix, string indent = "", bool last = true) const {
        if (node != nullptr) {
            cout << indent;
            if (last) {
                cout << "└─";
                indent += "  ";
            } else {
                cout << "├─";
                indent += "| ";
            }

            cout << prefix;
            if (node->endOfWord) cout << "*";
            cout << " (W:" << node->weight << ")";
            
            if (node->shortcut) {
                cout << " -- shortcut to ";
                auto shortcutChild = node->shortcut->children.begin();
                while (shortcutChild != node->shortcut->children.end()) {
                    cout << shortcutChild->first;
                    shortcutChild++;
                }
            }
            cout << endl;

            auto children = node->children.begin();
            while (children != node->children.end()) {
                viewTrie(children->second, string(1, children->first), indent, ++children == node->children.end());
            }
        }
    }

    void view() const {
        cout << "Visual Representation of Trie:\n";
        viewTrie(root, "");
    }



    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
            current->weight++;
        }
        current->endOfWord = true;
        operationCount++;
        if (shouldRebalance()) {
            rebalance();
        }
    }

    bool search(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                return false;
            }
            current = current->children[ch];
        }
        return current != nullptr && current->endOfWord;
    }

    void applyWeightDecay() {
        decayWeights(root);
    }

    void optimizePaths(int currentTime) {
        optimizeNode(root, nullptr, '\0', 0, currentTime);
    }

    bool removeHelper(TrieNode* current, const string& word, int index) {
        if (index == word.size()) {
            if (!current->endOfWord) {
                return false;
            }
            current->endOfWord = false;
            return current->children.empty();
        }

        char ch = word[index];
        TrieNode* node = current->children[ch];
        if (!node) {
            return false;
        }

        bool shouldDeleteCurrentNode = removeHelper(node, word, index + 1) && !node->endOfWord;

        if (shouldDeleteCurrentNode) {
            delete node;
            current->children.erase(ch);
            return current->children.empty();
        }

        return false;
    }

    void remove(const string& word) {
        removeHelper(root, word, 0);
    }
private:
    void rebalance() {
        vector<pair<string, int>> keyWeightPairs = flattenTrie(root, "");
        clearTrie(root);
        root = new TrieNode();
        sort(keyWeightPairs.begin(), keyWeightPairs.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        for (const auto& kwPair : keyWeightPairs) {
            insert(kwPair.first);
        }
        operationCount = 0;
    }

    vector<pair<string, int>> flattenTrie(TrieNode* node, string prefix) {
        vector<pair<string, int>> result;
        if (node->endOfWord) {
            result.emplace_back(prefix, node->weight);
        }
        for (const auto& child : node->children) {
            auto childResult = flattenTrie(child.second, prefix + child.first);
            result.insert(result.end(), childResult.begin(), childResult.end());
        }
        return result;
    }


    void clearTrie(TrieNode* node) {
        if (node == nullptr) return;

        for (auto& child : node->children) {
            clearTrie(child.second);
            child.second = nullptr;
        }
        delete node;
    }

    void decayWeights(TrieNode* node) {
        if (node == nullptr) return;
        node->weight = max(0, node->weight - DECAY_FACTOR);
        for (auto& child : node->children) {
            decayWeights(child.second);
        }
    }

    void optimizeNode(TrieNode* node, TrieNode* parent, char childKey, int depth, int currentTime) {
        if (node == nullptr) return;

        int threshold = calculateThreshold(depth, currentTime - node->lastAccessTime);
        
        if (node->weight > threshold && parent && depth > 1) {
            parent->shortcut = node;
        }

        for (auto& child : node->children) {
            optimizeNode(child.second, parent, child.first, depth + 1, currentTime);
        }
    }

    int calculateThreshold(int depth, int timeSinceLastAccess) {
        const int DEPTH_FACTOR = 5;
        const int TIME_FACTOR = 2;

        return depth * DEPTH_FACTOR + timeSinceLastAccess * TIME_FACTOR;
    }

    bool shouldRebalance() {
        int maxWeight = getMaxWeight(root);
        int averageWeight = getAverageWeight(root);
        int depthOfMaxWeight = getDepthOfMaxWeight(root);

        const int MAX_WEIGHT_THRESHOLD = 50;
        const int AVERAGE_WEIGHT_THRESHOLD = 20;
        const int MAX_DEPTH_THRESHOLD = 5;

        return (maxWeight > MAX_WEIGHT_THRESHOLD) ||
               (averageWeight > AVERAGE_WEIGHT_THRESHOLD) ||
               (depthOfMaxWeight > MAX_DEPTH_THRESHOLD);
    }

    int getMaxWeight(TrieNode* node) {
        if (node == nullptr) {
            return 0;
        }

        int maxWeight = node->weight;
        for (const auto& child : node->children) {
            maxWeight = max(maxWeight, getMaxWeight(child.second));
        }

        return maxWeight;
    }

    pair<int, int> getWeightSumAndCount(TrieNode* node) {
        if (node == nullptr) {
            return {0, 0};
        }

        int totalWeight = node->weight;
        int totalCount = 1;
        for (const auto& child : node->children) {
            auto childResult = getWeightSumAndCount(child.second);
            totalWeight += childResult.first;
            totalCount += childResult.second;
        }

        return {totalWeight, totalCount};
    }

    int getAverageWeight(TrieNode* node) {
        auto result = getWeightSumAndCount(node);
        if (result.second == 0) {
            return 0;
        }
        return result.first / result.second;
    }

    void findMaxWeightDepth(TrieNode* node, int currentDepth, int& maxWeight, int& maxDepth) {
        if (node == nullptr) {
            return;
        }

        if (node->weight > maxWeight) {
            maxWeight = node->weight;
            maxDepth = currentDepth;
        }

        for (auto& child : node->children) {
            findMaxWeightDepth(child.second, currentDepth + 1, maxWeight, maxDepth);
        }
    }

    int getDepthOfMaxWeight(TrieNode* node) {
        int maxWeight = 0;
        int maxDepth = 0;
        findMaxWeightDepth(node, 0, maxWeight, maxDepth);
        return maxDepth;
    }
    
 
};

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

void viewTrie(TrieNode* node, const string& prefix) {
    if (node == nullptr) return;
    if (node->endOfWord) cout << prefix << endl;
    for (auto& child : node->children) {
        viewTrie(child.second, prefix + child.first);
    }
}

// Function to safely get the user's menu choice.
int getChoice() {
    int choice;
    string input;
    while (true) {
        cout << "Enter your choice (number): ";
        getline(cin, input);
        stringstream inputStream(input);
        if (inputStream >> choice && inputStream.eof() && choice >= 1 && choice <= 11) {
            return choice;
        } else {
            cout << "Invalid choice, please enter a number between 1 and 11.\n";
        }
    }
}

string getWord(const string& prompt) {
    string word;
    cout << prompt;
    while (true) {
        cin >> word;
        if (!word.empty() && all_of(word.begin(), word.end(), ::isalpha)) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return word;
        } else {
            cout << "Invalid input, please enter a valid word (letters only): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

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
                break;
            }
            case 2: {
                word = getWord("Enter word to search: ");
                trie.search(word);
                break;
            }
            case 3: {
                word = getWord("Enter word to delete: ");
                trie.remove(word);
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
                cout << "Visual representation of the Trie:\n";
                trie.view();
                break;
            }
            case 7: {
                cout << "\nHelp - Trie CLI Commands:\n";
                cout << "1: Insert a word into the Trie.\n";
                cout << "2: Search for a word in the Trie.\n";
                cout << "3: Delete a word from the Trie.\n";
                cout << "4: Apply weight decay to all nodes.\n";
                cout << "5: Optimize paths based on usage and time.\n";
                cout << "6: View a visual representation of the Trie.\n";
                cout << "7: Display this help message.\n";
                cout << "8: Perform a prefix search.\n";
                cout << "9: Perform a wildcard search.\n";
                cout << "10: Get auto-complete suggestions.\n";
                cout << "11: Exit the application.\n";
                cout << "Choose an option by entering the corresponding number.\n";
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


/*int main() {
    int passedTests = 0;
    int totalTests = 6;

    {
        Trie trie;
        trie.insert("hello");
        if (trie.search("hello")) {
            cout << "Test 1 Passed: Insert and Search Single Word\n";
            passedTests++;
        } else {
            cout << "Test 1 Failed: Insert and Search Single Word\n";
        }
    }

    {
        Trie trie;
        if (!trie.search("world")) {
            cout << "Test 2 Passed: Search Non-Existent Word\n";
            passedTests++;
        } else {
            cout << "Test 2 Failed: Search Non-Existent Word\n";
        }
    }

    {
        Trie trie;
        trie.insert("hello");
        trie.insert("world");
        trie.insert("help");
        if (trie.search("hello") && trie.search("world") && trie.search("help")) {
            cout << "Test 3 Passed: Insert and Search Multiple Words\n";
            passedTests++;
        } else {
            cout << "Test 3 Failed: Insert and Search Multiple Words\n";
        }
    }

    {
        Trie trie;
        trie.insert("hello");
        trie.insert("hello");
        if (trie.search("hello")) {
            cout << "Test 4 Passed: Insert Duplicates and Search\n";
            passedTests++;
        } else {
            cout << "Test 4 Failed: Insert Duplicates and Search\n";
        }
    }

    {
        Trie trie;
        trie.insert("");
        if (trie.search("")) {
            cout << "Test 5 Passed: Insert and Search Empty String\n";
            passedTests++;
        } else {
            cout << "Test 5 Failed: Insert and Search Empty String\n";
        }
    }

    {
        Trie trie;
        if (!trie.search("")) {
            cout << "Test 6 Passed: Search Empty Trie\n";
            passedTests++;
        } else {
            cout << "Test 6 Failed: Search Empty Trie\n";
        }
    }

    cout << "\nTotal Tests Passed: " << passedTests << "/" << totalTests << endl;

    return passedTests == totalTests ? 0 : 1;
}*/
