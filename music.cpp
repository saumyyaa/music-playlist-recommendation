#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

// Trie Node Structure
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    string songName;
    TrieNode() : isEndOfWord(false) {}
};

// Trie Class for Searching Songs
class Trie {
public:
    TrieNode* root;
    Trie() { root = new TrieNode(); }

    void insert(string song) {
        TrieNode* node = root;
        for (char c : song) {
            if (!node->children[c]) node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->isEndOfWord = true;
        node->songName = song;
    }

    void search(string prefix, TrieNode* node, vector<string>& results) {
        if (node->isEndOfWord) results.push_back(node->songName);
        for (auto it : node->children) search(prefix + it.first, it.second, results);
    }

    vector<string> getSongsWithPrefix(string prefix) {
        TrieNode* node = root;
        vector<string> results;
        for (char c : prefix) {
            if (!node->children[c]) return results;
            node = node->children[c];
        }
        search(prefix, node, results);
        return results;
    }
};

// Song Recommendation using Priority Queue
struct Song {
    string name;
    int popularity; // Higher value means more popular
    bool operator<(const Song& other) const { return popularity < other.popularity; }
};

// Graph for Similar Song Recommendations
class Graph {
public:
    unordered_map<string, vector<string>> adjList;

    void addEdge(string song1, string song2) {
        adjList[song1].push_back(song2);
        adjList[song2].push_back(song1);
    }

    vector<string> getSimilarSongs(string song) {
        return adjList[song];
    }
};

int main() {
    Trie songTrie;
    Graph songGraph;
    priority_queue<Song> pq;

    // Adding Songs
    vector<pair<string, int>> songs = {
        {"Shape of You", 95}, {"Blinding Lights", 90}, {"Despacito", 85},
        {"Believer", 80}, {"Faded", 75}, {"Closer", 70}, {"Havana", 65}
    };

    for (auto& song : songs) {
        songTrie.insert(song.first);
        pq.push({song.first, song.second});
    }

    // Creating Graph for Similar Songs
    songGraph.addEdge("Shape of You", "Perfect");
    songGraph.addEdge("Blinding Lights", "Save Your Tears");
    songGraph.addEdge("Despacito", "Bailando");
    songGraph.addEdge("Believer", "Radioactive");
    songGraph.addEdge("Faded", "Alone");

    // User Input for Searching Songs
    string query;
    cout << "Enter song prefix: ";
    cin >> query;
    vector<string> searchResults = songTrie.getSongsWithPrefix(query);

    cout << "\nSongs matching '" << query << "':\n";
    if (searchResults.empty()) cout << "No matches found.\n";
    else for (string song : searchResults) cout << song << endl;

    // Displaying Top Recommended Songs (Most Popular)
    cout << "\nTop Recommended Songs:\n";
    int count = 3; // Show top 3 songs
    while (!pq.empty() && count--) {
        cout << pq.top().name << " (Popularity: " << pq.top().popularity << ")\n";
        pq.pop();
    }

    // User Input for Similar Songs
    string songChoice;
    cout << "\nEnter a song to find similar ones: ";
    cin.ignore();
    getline(cin, songChoice);

    vector<string> similarSongs = songGraph.getSimilarSongs(songChoice);
    if (similarSongs.empty()) cout << "No similar songs found.\n";
    else {
        cout << "Similar Songs to '" << songChoice << "':\n";
        for (string s : similarSongs) cout << s << endl;
    }

    return 0;
}
