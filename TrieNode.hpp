//
//  TrieNode.hpp
//  aaasdd
//
//  Created by Arshia Taghavinejad on 2024-01-01.
//

#ifndef TrieNode_hpp
#define TrieNode_hpp

#include <map>

class TrieNode {
public:
    std::map<char, TrieNode*> children;
    TrieNode* shortcut;
    bool endOfWord;
    int weight;
    int lastAccessTime;

    TrieNode();
    ~TrieNode();
};

#endif /* TrieNode_hpp */
