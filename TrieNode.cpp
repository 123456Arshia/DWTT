//
//  TrieNode.cpp
//  aaasdd
//
//  Created by Arshia Taghavinejad on 2024-01-01.
//

#include "TrieNode.hpp"

TrieNode::TrieNode() : shortcut(nullptr), endOfWord(false), weight(0), lastAccessTime(0) {}

TrieNode::~TrieNode() {
    for (auto& child : children) {
        delete child.second;
        child.second = nullptr;
    }
}
