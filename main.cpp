#include <iostream>
#include <string>

using namespace std;

const int ALPHABET_SIZE = 26;
const int MAX_SUGGESTIONS = 100;
const int MAX_WORD_LENGTH = 50;

struct TrieNode
{
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    int frequency;
};

TrieNode* getNewNode() 
{
    TrieNode* pNode = new TrieNode;
    pNode->isEndOfWord = false;
    pNode->frequency = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) 
    {
        pNode->children[i] = nullptr;
    }
    return pNode;
}

void insert(TrieNode* root, const string& key, int freq = 1)
{
    TrieNode* node = root;
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!node->children[index])
        {
            node->children[index] = getNewNode();
        }
        node = node->children[index];
    }
    node->isEndOfWord = true;
    node->frequency += freq;
}

bool search(TrieNode* root, const string& key) 
{
    TrieNode* node = root;
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!node->children[index]) 
        {
            return false;
        }
        node = node->children[index];
    }
    return node != nullptr && node->isEndOfWord;
}

struct Suggestion 
{
    string word;
    int frequency;
};

void findAllWords(TrieNode* node, string currentPrefix, Suggestion suggestions[], int& count)
{
    if (node->isEndOfWord) 
    {
        if (count < MAX_SUGGESTIONS) 
        {
            suggestions[count].word = currentPrefix;
            suggestions[count].frequency = node->frequency;
            count++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i]) 
        {
            char nextChar = 'a' + i;
            findAllWords(node->children[i], currentPrefix + nextChar, suggestions, count);
        }
    }
}

void sortSuggestions(Suggestion suggestions[], int count)
{
    for (int i = 0; i < count - 1; i++) 
    {
        for (int j = 0; j < count - i - 1; j++) {
            if (suggestions[j].frequency < suggestions[j + 1].frequency)
            {
                Suggestion temp = suggestions[j];
                suggestions[j] = suggestions[j + 1];
                suggestions[j + 1] = temp;
            }
        }
    }
}

int getAutocompleteSuggestions(TrieNode* root, const string& prefix, string results[]) 
{
    TrieNode* node = root;

    for (int i = 0; i < prefix.length(); i++) 
    {
        int index = prefix[i] - 'a';
        if (!node->children[index]) 
        {
            return 0; 
        }
        node = node->children[index];
    }

    Suggestion suggestions[MAX_SUGGESTIONS];
    int count = 0;
    findAllWords(node, prefix, suggestions, count);

    sortSuggestions(suggestions, count);


    int resultCount = 0;
    for (int i = 0; i < count && resultCount < MAX_SUGGESTIONS; i++) 
    {
        results[resultCount++] = suggestions[i].word;
    }

    return resultCount;
}

void buildDictionary(TrieNode* root) 
{
    insert(root, "apple", 5);
    insert(root, "application", 3);
    insert(root, "appetite", 2);
    insert(root, "banana", 4);
    insert(root, "band", 3);
    insert(root, "bank", 2);
    insert(root, "cat", 6);
    insert(root, "car", 5);
    insert(root, "card", 4);
    insert(root, "care", 3);
    insert(root, "dog", 7);
    insert(root, "door", 4);
    insert(root, "dot", 3);
    insert(root, "elephant", 2);
    insert(root, "elevator", 1);
    insert(root, "hello", 8);
    insert(root, "help", 6);
    insert(root, "hell", 4);
    insert(root, "world", 9);
    insert(root, "word", 5);
    insert(root, "work", 7);
}

void displaySuggestions(const string suggestions[], int count)
{
    if (count == 0)
    {
        cout << "Доступных вариантов не найдено" << endl;
        return;
    }
cout << "Доступные варианты: (" << count << "): ";
for (int i = 0; i < count; i++) 
{
    cout << suggestions[i];
    if (i < count - 1) 
    {
        cout << ", ";
    }
}
cout << endl;
}

void deleteTrie(TrieNode* node)
{
    if (node == nullptr) 
    {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != nullptr)
        {
            deleteTrie(node->children[i]);
        }
    }

    delete node;
}

int main() 
{
    setlocale(LC_ALL, "ru");
    TrieNode* root = getNewNode();
    buildDictionary(root);

    cout << "=== Автоматическое заполнение ===" << endl;
    cout << "Введите 'quit' для выхода из программы" << endl << endl;

    string input;
    while (true)
    {
        cout << "Введите начало: ";
        getline(cin, input);

        if (input == "quit") 
        {
            break;
        }

        bool valid = true;
        for (char c : input) 
        {
            if (c < 'a' || c > 'z') 
            {
                valid = false;
                break;
            }
        }

        if (!valid) 
        {
            cout << "Введите буквы только от (a-z)" << endl;
            continue;
        }

        string suggestions[MAX_SUGGESTIONS];
        int count = getAutocompleteSuggestions(root, input, suggestions);

        displaySuggestions(suggestions, count);
        cout << endl;
    }

    deleteTrie(root);

    cout << "Завершение программы" << endl;

    return 0;
}