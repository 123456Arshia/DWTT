//
//  main.cpp
//  dwtt
//
//  Created by Arshia Taghavinejad on 2023-12-19.
//

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class TrieNode {
public:
    map<char, TrieNode*> children;
    TrieNode* shortcut;
    bool endOfWord;
    int weight;
    int lastAccessTime;

    TrieNode() : shortcut(nullptr), endOfWord(false), weight(0), lastAccessTime(0) {}
};

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

        for (auto it = node->children.begin(); it != node->children.end();) {
            clearTrie(it->second);
            delete it->second;
            it = node->children.erase(it);
        }

        // Make sure to handle the case when node is the root to avoid double deletion
        if (node != root) {
            delete node;
        }
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

int main() {
    Trie trie;
    trie.insert("hello");
    trie.insert("help");
    trie.insert("helicopter");
    trie.insert("world");

    int currentTime = 100; // Example current time
    trie.applyWeightDecay();
    trie.optimizePaths(currentTime);

    cout << "Rebalance triggered after multiple insertions." << endl;
    cout << "Search hello: " << trie.search("hello") << endl;
    return 0;
}
