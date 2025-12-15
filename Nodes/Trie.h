#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <string>
#include <vector>

struct TrieNode {
    std::map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}

    ~TrieNode() {
        for (auto const& pair : children) delete pair.second;
    }
};

class Trie {
private:
    TrieNode* root;

    void collectWords(TrieNode* node, std::string currentWord, std::vector<std::string>& results) {
        if (node->isEndOfWord) results.push_back(currentWord);
        for (auto const& pair : node->children) {
            collectWords(pair.second, currentWord + pair.first, results);
        }
    }

public:
    Trie() { root = new TrieNode(); }
    ~Trie() { delete root; }

    void insert(std::string word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end())
                current->children[ch] = new TrieNode();
            current = current->children[ch];
        }
        current->isEndOfWord = true;
    }

    std::vector<std::string> searchByPrefix(std::string prefix) {
        TrieNode* current = root;
        std::vector<std::string> results;
        for (char ch : prefix) {
            if (current->children.find(ch) == current->children.end()) return results;
            current = current->children[ch];
        }
        collectWords(current, prefix, results);
        return results;
    }
};

#endif