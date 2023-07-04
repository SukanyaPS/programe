#include <functional>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

class MyHashTB {
private:
  vector<list<pair<string, string>>> m_hashTable;
  unsigned long N;

  // Hash function to determine index

  unsigned long hashFunction(const string &key) {
    std::hash<std::string> hasher;
    return hasher(key) % N;
  }

public:
  MyHashTB(unsigned long tableSize) : N(tableSize) { m_hashTable.resize(N); }

  // Insert a key-value pair into the hashFunction m_hashTable
  void insert(const string &key, const string &value) {
    unsigned long index = hashFunction(key);
    list<pair<string, string>> &_row = m_hashTable[index];

    // Check if key already exists in the m_hashTable
    for (const auto &entry : _row) {
      if (entry.first == key) {
        cout << "Key already exists." << endl;
        return;
      }
    }

    // Insert the key-value pair into the _row
    _row.push_back(make_pair(key, value));
  }

  // Retrieve the value associated with a given key
  string get(const string &key) {
    unsigned long index = hashFunction(key);
    list<pair<string, string>> &_row = m_hashTable[index];

    // Search for the key in the _row
    for (const auto &entry : _row) {
      if (entry.first == key) {
        return entry.second;
      }
    }

    return "Key not found.";
  }

  // Remove a key-value pair from the hashFunction m_hashTable
  void remove(const string &key) {
    unsigned long index = hashFunction(key);
    list<pair<string, string>> &_row = m_hashTable[index];

    // Search for the key in the _row and remove it
    for (auto it = _row.begin(); it != _row.end(); ++it) {
      if (it->first == key) {
        _row.erase(it);
        return;
      }
    }

    cout << "Key not found." << endl;
  }
};

int main() {
  MyHashTB hashTable(10000);

  hashTable.insert("one", "su");
  hashTable.insert("two", "ka");
  hashTable.insert("three", "nya");

  cout << hashTable.get("one") << endl;
  cout << hashTable.get("two") << endl;
  cout << hashTable.get("three") << endl;
  cout << hashTable.get("four") << endl;

  hashTable.remove("two");
  cout << hashTable.get("two") << endl;

  return 0;
}
