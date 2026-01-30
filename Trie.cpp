//
//  Trie.cpp
//  aaasdd
//
//  Created by Arshia Taghavinejad on 2024-01-01.
//

#include "Trie.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

Trie::Trie() : root(new TrieNode()), operationCount(0), REBALANCE_THRESHOLD(10), DECAY_FACTOR(1) {
    
}

Trie::~Trie() {
    clearTrie(root);
}

void Trie::insert(const string& word) {
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
    if (operationCount >= REBALANCE_THRESHOLD && shouldRebalance()) {
        rebalance();
    }
}

bool Trie::search(const string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            return false;
        }
        current = current->children[ch];
    }
    return current != nullptr && current->endOfWord;
}

void Trie::remove(const string& word) {
    removeHelper(root, word, 0);
}


bool Trie::removeHelper(TrieNode* current, const string& word, size_t index) {
    if (current == nullptr) {
        return false;
    }
    if (index == word.size()) {
        if (!current->endOfWord) {
            return false;
        }
        current->endOfWord = false;
        return current->children.empty();
    }

    char ch = word[index];
    auto it = current->children.find(ch);
    if (it == current->children.end() || it->second == nullptr) {
        return false;
    }

    TrieNode* node = it->second;
    bool shouldDeleteChild = removeHelper(node, word, index + 1);

    if (shouldDeleteChild) {
        delete node;
        current->children.erase(it);
    }

    return current->children.empty() && !current->endOfWord;
}


void Trie::rebalance() {
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


void Trie::clearTrie(TrieNode* node) {
    delete node;
}


void Trie::decayWeights(TrieNode* node) {
    if (node == nullptr) return;
    node->weight = max(0, node->weight - DECAY_FACTOR);
    for (auto& child : node->children) {
        decayWeights(child.second);
    }
}


void Trie::optimizeNode(TrieNode* node, TrieNode* parent, int depth, int currentTime, const string& path, vector<string>* report) {
    if (node == nullptr) return;
    int threshold = calculateThreshold(depth, currentTime - node->lastAccessTime);
    if (node->weight > threshold && parent && depth > 1) {
        if (parent->shortcut != node) {
            parent->shortcut = node;
            if (report) {
                string parentPath = path.size() > 0 ? path.substr(0, path.size() - 1) : "<root>";
                string nodePath = path.empty() ? "<root>" : path;
                int age = currentTime - node->lastAccessTime;
                report->push_back(
                    "shortcut: parent=\"" + parentPath + "\" -> node=\"" + nodePath +
                    "\" (depth=" + to_string(depth) +
                    ", weight=" + to_string(node->weight) +
                    ", threshold=" + to_string(threshold) +
                    ", age=" + to_string(age) + ")"
                );
            }
        }
    }
    for (auto& child : node->children) {
        optimizeNode(child.second, node, depth + 1, currentTime, path + child.first, report);
    }
}


int Trie::calculateThreshold(int depth, int timeSinceLastAccess) {
    const int DEPTH_FACTOR = 5;
    const int TIME_FACTOR = 2;
    return depth * DEPTH_FACTOR + timeSinceLastAccess * TIME_FACTOR;
}


bool Trie::shouldRebalance() {
    const int MAX_WEIGHT_THRESHOLD = 50;
    const int AVERAGE_WEIGHT_THRESHOLD = 20;
    const int MAX_DEPTH_THRESHOLD = 5;
    int maxWeight = getMaxWeight(root);
    int averageWeight = getAverageWeight(root);
    int depthOfMaxWeight = getDepthOfMaxWeight(root);
    return (maxWeight > MAX_WEIGHT_THRESHOLD) ||
           (averageWeight > AVERAGE_WEIGHT_THRESHOLD) ||
           (depthOfMaxWeight > MAX_DEPTH_THRESHOLD);
}


int Trie::getMaxWeight(TrieNode* node) {
    if (node == nullptr) return 0;
    int maxWeight = node->weight;
    for (auto& child : node->children) {
        maxWeight = max(maxWeight, getMaxWeight(child.second));
    }
    return maxWeight;
}


pair<int, int> Trie::getWeightSumAndCount(TrieNode* node) {
    if (node == nullptr) return {0, 0};
    int totalWeight = node->weight;
    int totalCount = 1;
    for (auto& child : node->children) {
        auto childResult = getWeightSumAndCount(child.second);
        totalWeight += childResult.first;
        totalCount += childResult.second;
    }
    return {totalWeight, totalCount};
}


int Trie::getAverageWeight(TrieNode* node) {
    auto totals = getWeightSumAndCount(node);
    return totals.second > 0 ? totals.first / totals.second : 0;
}


void Trie::findMaxWeightDepth(TrieNode* node, int currentDepth, int& maxWeight, int& maxDepth) {
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

int Trie::getDepthOfMaxWeight(TrieNode* node) {
    int maxWeight = 0;
    int maxDepth = 0;
    findMaxWeightDepth(node, 0, maxWeight, maxDepth);
    return maxDepth;
}

vector<pair<string, int>> Trie::flattenTrie(TrieNode* node, string prefix) {
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

void Trie::findWordsWithPrefix(TrieNode* node, const string& prefix, vector<string>& results, string currentWord) {
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

vector<string> Trie::searchPrefix(const string& prefix) {
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

vector<string> Trie::wildcardSearch(const string& word) {
    vector<string> results;
    searchWildcard(root, word, 0, "", results);
    return results;
}

void Trie::searchWildcard(TrieNode* node, const string& word, size_t index, string currentWord, vector<string>& results) {
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

vector<string> Trie::autoComplete(const string& prefix) {
    return searchPrefix(prefix);
}

void Trie::applyWeightDecay() {
    decayWeights(root);
}

vector<string> Trie::optimizePaths(int currentTime) {
    vector<string> report;
    optimizeNode(root, nullptr, 0, currentTime, "", &report);
    return report;
}
void viewTrie(TrieNode* node, const string& prefix, string indent, bool last) {
    if (node == nullptr) return;

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
            ++shortcutChild;
        }
    }
    cout << endl;

    
    auto child = node->children.begin();
    while (child != node->children.end()) {
        
        auto nextChild = child;
        ++nextChild;
        viewTrie(child->second, string(1, child->first), indent, nextChild == node->children.end());
        child = nextChild;
    }
}


void Trie::view() const {
    std::cout << "Visual Representation of Trie:\n";
    viewTrie(root, "", "", true);
}
