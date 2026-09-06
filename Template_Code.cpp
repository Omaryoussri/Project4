#include <iostream>
#include <vector>
#include <string>
#include <cctype> // for tolower()

using namespace std;

// Each node in the Trie
class TrieNode {
public:
    // Each node has up to 26 children (for each letter)
    TrieNode* children[26];

    // Marks if this node completes a word
    bool isEndOfWord;

    // Constructor
    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

// The Trie class
class Trie {
private:
    TrieNode* root;

    // Stores the total number of unique words in the Trie
    int wordCount;

    // Helper function to find all words from a node
    void findAllWords(TrieNode* node, string currentWord, vector<string>& results) {
        if (node->isEndOfWord) {
            results.push_back(currentWord);
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                findAllWords(node->children[i], currentWord + char('a' + i), results);
            }
        }
    }

    // Helper function to delete all nodes recursively (Post-order)
    void deleteNodes(TrieNode* node) {
        if (node == nullptr) {
            return;
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                deleteNodes(node->children[i]);
            }
        }
        delete node;
    }

    // Helper function to count words from a specific node
    int countWordsFromNode(TrieNode* node) {
        int count = 0;
        if (node->isEndOfWord) {
            count = 1;
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                count += countWordsFromNode(node->children[i]);
            }
        }
        return count;
    }

    // Helper function to remove a word recursively
    bool removeHelper(TrieNode* node, string word, int index) {
        if (index == (int)word.length()) {
            if (!node->isEndOfWord) {
                return false; 
            }
            node->isEndOfWord = false; 
            
            for (int i = 0; i < 26; i++) {
                if (node->children[i] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        
        int idx = tolower(word[index]) - 'a';
        if (idx < 0 || idx >= 26 || node->children[idx] == nullptr) {
            return false; 
        }
        
        bool shouldDelete = removeHelper(node->children[idx], word, index + 1);
        
        if (shouldDelete) {
            delete node->children[idx];
            node->children[idx] = nullptr;
        }
        
        if (!node->isEndOfWord) {
            for (int i = 0; i < 26; i++) {
                if (node->children[i] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        
        return false;
    }

public:
    // Constructor
    Trie() {
        root = new TrieNode();
        wordCount = 0;
    }

    // Destructor
    ~Trie() {
        deleteNodes(root);
    }

    // Insert a word into the Trie
    void insert(string word) {
        TrieNode* curr = root;
        for (char ch : word) {
            int index = tolower(ch) - 'a';
            if (index < 0 || index >= 26) continue; // Safety check
            
            if (curr->children[index] == nullptr) {
                curr->children[index] = new TrieNode();
            }
            curr = curr->children[index];
        }

        if (!curr->isEndOfWord) {
            curr->isEndOfWord = true;
            wordCount++;
        }
    }

    // Search for a word in the Trie
    bool search(string word) {
        TrieNode* curr = root;
        for (char ch : word) {
            int index = tolower(ch) - 'a';
            if (index < 0 || index >= 26 || curr->children[index] == nullptr) {
                return false;
            }
            curr = curr->children[index];
        }
        return curr->isEndOfWord;
    }

    // Check if any word starts with the given prefix
    bool startsWith(string prefix) {
        TrieNode* curr = root;
        for (char ch : prefix) {
            int index = tolower(ch) - 'a';
            if (index < 0 || index >= 26 || curr->children[index] == nullptr) {
                return false;
            }
            curr = curr->children[index];
        }
        return true;
    }

    // Get all words that start with the given prefix
    vector<string> autocomplete(string prefix) {
        vector<string> suggestions;
        TrieNode* curr = root;
        string lowerPrefix = "";
        
        for (char ch : prefix) {
            int index = tolower(ch) - 'a';
            if (index < 0 || index >= 26 || curr->children[index] == nullptr) {
                return suggestions; 
            }
            curr = curr->children[index];
            lowerPrefix += tolower(ch);
        }
        
        findAllWords(curr, lowerPrefix, suggestions);
        return suggestions;
    }

    // Remove a word from the Trie
    void remove(string word) {
        if (search(word)) {
            removeHelper(root, word, 0);
            wordCount--;
        }
    }

    // Count the total number of words in the Trie
    int countWords() {
        return wordCount;
    }

    // Count how many words start with a given prefix
    int countWordsWithPrefix(string prefix) {
        TrieNode* curr = root;
        for (char ch : prefix) {
            int index = tolower(ch) - 'a';
            if (index < 0 || index >= 26 || curr->children[index] == nullptr) {
                return 0;
            }
            curr = curr->children[index];
        }
        return countWordsFromNode(curr);
    }

    // Get all words stored in the Trie
    vector<string> getAllWords() {
        vector<string> words;
        findAllWords(root, "", words);
        return words;
    }

    // Find the longest prefix of a given word that exists in the Trie
    string longestPrefixOf(string word) {
        string longest = "";
        TrieNode* curr = root;
        
        for (char ch : word) {
            int index = tolower(ch) - 'a';
            if (index < 0 || index >= 26 || curr->children[index] == nullptr) {
                break; 
            }
            longest += ch;
            curr = curr->children[index];
        }
        return longest;
    }

    // Check whether the Trie contains any words
    bool isEmpty() {
        return wordCount == 0; 
    }

    // Completely clear the Trie
    void clear() {
        deleteNodes(root);
        root = new TrieNode();
        wordCount = 0;
    }

    // Get autocomplete suggestions with a maximum limit
    vector<string> autocomplete(string prefix, int limit) {
        vector<string> suggestions = autocomplete(prefix);
        if (suggestions.size() > (size_t)limit) {
            suggestions.resize(limit);
        }
        return suggestions;
    }
};

// Main function
int main() {
    cout << "=== TRIE DATA STRUCTURE IMPLEMENTATION ===" << endl;
    cout << "Testing all Trie functionalities..." << endl;

    Trie trie;

    // ============================================================
    // TEST 1: BASIC INSERTION AND SEARCH
    // ============================================================
    cout << "\n1. Testing basic insertion and search:" << endl;
    cout << "======================================" << endl;
    vector<string> words = {"apple", "banana", "orange", "grape", "kiwi"};
    for (const string& word : words) {
        trie.insert(word);
        cout << "Inserted: " << word << endl;
    }
    for (const string& word : words) {
        cout << "Search '" << word << "': " << (trie.search(word) ? "FOUND" : "NOT FOUND") << endl;
    }

    vector<string> nonExisting = {"app", "ban", "ora", "graph", "kiwis"};
    for (const string& word : nonExisting) {
        cout << "Search '" << word << "': " << (trie.search(word) ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;
    }

    // ============================================================
    // TEST 2: PREFIX CHECKING
    // ============================================================
    cout << "\n2. Testing prefix checking:" << endl;
    cout << "==========================" << endl;
    vector<string> prefixes = {"app", "ban", "ora", "grap", "k"};
    for (const string& prefix : prefixes) {
        cout << "Prefix '" << prefix << "': " << (trie.startsWith(prefix) ? "EXISTS" : "DOESN'T EXIST") << endl;
    }

    vector<string> nonPrefixes = {"x", "yield", "zed", "micro", "nano"};
    for (const string& prefix : nonPrefixes) {
        cout << "Prefix '" << prefix << "': " << (trie.startsWith(prefix) ? "EXISTS" : "DOESN'T EXIST") << " (expected: DOESN'T EXIST)" << endl;
    }

    // ============================================================
    // TEST 3: AUTOCOMPLETE
    // ============================================================
    cout << "\n3. Testing autocomplete functionality:" << endl;
    cout << "======================================" << endl;
    vector<string> testPrefixes = {"a", "b", "o", "g", "k", "ap", "ban", "ora", "gr", "ki"};
    for (const string& prefix : testPrefixes) {
        vector<string> suggestions = trie.autocomplete(prefix);
        cout << "Autocomplete for '" << prefix << "': ";
        if (suggestions.empty()) cout << "No suggestions found";
        else {
            for (size_t i = 0; i < suggestions.size(); i++) {
                if (i > 0) cout << ", ";
                cout << suggestions[i];
            }
        }
        cout << endl;
    }

    // ============================================================
    // TEST 4: EDGE CASES
    // ============================================================
    cout << "\n4. Testing edge cases:" << endl;
    cout << "======================" << endl;
    cout << "Search empty string: " << (trie.search("") ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;
    cout << "Starts with empty prefix: " << (trie.startsWith("") ? "EXISTS" : "DOESN'T EXIST") << " (expected: EXISTS)" << endl;
    
    vector<string> emptySuggestions = trie.autocomplete("");
    cout << "Autocomplete for empty string: ";
    if (emptySuggestions.empty()) cout << "No suggestions found";
    else {
        for (size_t i = 0; i < emptySuggestions.size(); i++) {
            if (i > 0) cout << ", ";
            cout << emptySuggestions[i];
        }
    }
    cout << " (expected: all words)" << endl;

    // ============================================================
    // TEST 5: ADDITIONAL WORDS
    // ============================================================
    cout << "\n5. Testing with additional words:" << endl;
    cout << "================================" << endl;
    vector<string> additionalWords = {"application", "appetizer", "banister", "bandana", "oracle", "grapefruit"};
    for (const string& word : additionalWords) {
        trie.insert(word);
        cout << "Inserted: " << word << endl;
    }
    for (const string& word : additionalWords) {
        cout << "Search '" << word << "': " << (trie.search(word) ? "FOUND" : "NOT FOUND") << endl;
    }

    vector<string> newPrefixes = {"app", "ban", "ora", "gra"};
    for (const string& prefix : newPrefixes) {
        vector<string> suggestions = trie.autocomplete(prefix);
        cout << "Autocomplete for '" << prefix << "': ";
        if (suggestions.empty()) cout << "No suggestions found";
        else {
            for (size_t i = 0; i < suggestions.size(); i++) {
                if (i > 0) cout << ", ";
                cout << suggestions[i];
            }
        }
        cout << endl;
    }

    // ============================================================
    // TEST 6: CASE SENSITIVITY
    // ============================================================
    cout << "\n6. Testing case sensitivity:" << endl;
    cout << "============================" << endl;
    trie.insert("Hello");
    trie.insert("WORLD");
    vector<string> caseWords = {"hello", "Hello", "WORLD", "world"};
    for (const string& word : caseWords) {
        cout << "Search '" << word << "': " << (trie.search(word) ? "FOUND" : "NOT FOUND") << endl;
    }

    // ============================================================
    // TEST 7: COUNT WORDS
    // ============================================================
    cout << "\n7. Testing word counting:" << endl;
    cout << "=========================" << endl;
    cout << "Total words: " << trie.countWords() << endl;
    cout << "Words starting with 'app': " << trie.countWordsWithPrefix("app") << endl;
    cout << "Words starting with 'ban': " << trie.countWordsWithPrefix("ban") << endl;
    cout << "Words starting with 'xyz': " << trie.countWordsWithPrefix("xyz") << endl;

    // ============================================================
    // TEST 8: GET ALL WORDS
    // ============================================================
    cout << "\n8. Testing getAllWords:" << endl;
    cout << "======================" << endl;
    vector<string> allWords = trie.getAllWords();
    cout << "All words in Trie:" << endl;
    for (const string& word : allWords) {
        cout << "- " << word << endl;
    }

    // ============================================================
    // TEST 9: LONGEST PREFIX
    // ============================================================
    cout << "\n9. Testing longestPrefixOf:" << endl;
    cout << "==========================" << endl;
    vector<string> testWords = {"application", "appreciate", "banana", "band", "grapefruit", "unknown"};
    for (const string& word : testWords) {
        cout << "Longest prefix of '" << word << "': " << trie.longestPrefixOf(word) << endl;
    }

    // ============================================================
    // TEST 10: REMOVE WORDS
    // ============================================================
    cout << "\n10. Testing remove:" << endl;
    cout << "==================" << endl;
    cout << "Before removing 'apple': " << (trie.search("apple") ? "FOUND" : "NOT FOUND") << endl;
    trie.remove("apple");
    cout << "After removing 'apple': " << (trie.search("apple") ? "FOUND" : "NOT FOUND") << endl;
    cout << "Checking 'application': " << (trie.search("application") ? "FOUND" : "NOT FOUND") << endl;
    cout << "Checking 'appetizer': " << (trie.search("appetizer") ? "FOUND" : "NOT FOUND") << endl;

    // ============================================================
    // TEST 11: AUTOCOMPLETE WITH LIMIT
    // ============================================================
    cout << "\n11. Testing autocomplete with limit:" << endl;
    cout << "======================================" << endl;
    vector<string> limitedSuggestions = trie.autocomplete("app", 2);
    cout << "Maximum 2 suggestions for 'app': ";
    for (size_t i = 0; i < limitedSuggestions.size(); i++) {
        if (i > 0) cout << ", ";
        cout << limitedSuggestions[i];
    }
    cout << endl;

    // ============================================================
    // TEST 12: EMPTY CHECK
    // ============================================================
    cout << "\n12. Testing isEmpty:" << endl;
    cout << "===================" << endl;
    cout << "Is Trie empty? " << (trie.isEmpty() ? "YES" : "NO") << endl;

    // ============================================================
    // TEST 13: CLEAR
    // ============================================================
    cout << "\n13. Testing clear:" << endl;
    cout << "=================" << endl;
    cout << "Words before clear: " << trie.countWords() << endl;
    trie.clear();
    cout << "Words after clear: " << trie.countWords() << endl;
    cout << "Is Trie empty after clear? " << (trie.isEmpty() ? "YES" : "NO") << endl;

    cout << "\n=== ALL TESTS COMPLETED ===" << endl;

    return 0;
}