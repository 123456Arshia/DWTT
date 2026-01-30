//
//  Trie.hpp
//  aaasdd
//
//  Created by Arshia Taghavinejad on 2024-01-01.
//

#ifndef Trie_hpp
#define Trie_hpp

#include "TrieNode.hpp"
#include <string>
#include <vector>

using namespace std;

class Trie {
private:
    TrieNode* root;
    int operationCount;
    const int REBALANCE_THRESHOLD;
    const int DECAY_FACTOR;

    friend void viewTrie(const TrieNode* node, const std::string& prefix, std::string indent, bool last);
    void rebalance();
    void clearTrie(TrieNode* node);
    void decayWeights(TrieNode* node);
    void optimizeNode(TrieNode* node, TrieNode* parent, int depth, int currentTime, const string& path, vector<string>* report);
    int calculateThreshold(int depth, int timeSinceLastAccess);
    bool shouldRebalance();
    int getMaxWeight(TrieNode* node);
    pair<int, int> getWeightSumAndCount(TrieNode* node);
    int getAverageWeight(TrieNode* node);
    void findMaxWeightDepth(TrieNode* node, int currentDepth, int& maxWeight, int& maxDepth);
    int getDepthOfMaxWeight(TrieNode* node);
    bool removeHelper(TrieNode* current, const string& word, size_t index);
    vector<pair<string, int>> flattenTrie(TrieNode* node, string prefix);
    void findWordsWithPrefix(TrieNode* node, const string& prefix, vector<string>& results, string currentWord);
    void searchWildcard(TrieNode* node, const string& word, size_t index, string currentWord, vector<string>& results);

public:
    Trie();
    ~Trie();

    void insert(const string& word);
    bool search(const string& word);
    void remove(const string& word);

    vector<string> searchPrefix(const string& prefix);
    vector<string> wildcardSearch(const string& word);
    vector<string> autoComplete(const string& prefix);
    void applyWeightDecay();
    vector<string> optimizePaths(int currentTime);
    void view() const;

    friend void viewTrie(TrieNode* node, const string& prefix, string indent, bool last);
};

#endif /* Trie_hpp */
