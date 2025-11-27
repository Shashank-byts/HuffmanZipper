#include <bits/stdc++.h>
using namespace std;

// ----------------------------------------------
// Huffman Node
// ----------------------------------------------
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
    Node(Node* l, Node* r) : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// ----------------------------------------------
// Build Huffman Tree
// ----------------------------------------------
Node* buildTree(unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto &p : freq) pq.push(new Node(p.first, p.second));
    if (pq.size() == 1) pq.push(new Node('\0', 1));
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }
    return pq.top();
}

// ----------------------------------------------
// Generate Codes
// ----------------------------------------------
void buildCodes(Node* root, string code, unordered_map<char,string>& mp) {
    if (!root) return;
    if (!root->left && !root->right) mp[root->ch] = code;
    buildCodes(root->left, code + "0", mp);
    buildCodes(root->right, code + "1", mp);
}

// ----------------------------------------------
// Encode Text
// ----------------------------------------------
string encode(const string &text, unordered_map<char,string>& codes) {
    string out="";
    for (char c : text) out += codes[c];
    return out;
}

// ----------------------------------------------
// Decode Bitstring
// ----------------------------------------------
string decode(const string &bits, Node* root) {
    string res="";
    Node* curr = root;
    for (char b : bits) {
        curr = (b=='0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            res.push_back(curr->ch);
            curr = root;
        }
    }
    return res;
}


int main() {
    string text;
    cout << "Enter text to compress: ";
    getline(cin, text);

    unordered_map<char,int> freq;
    for (char c : text) freq[c]++;

    Node* root = buildTree(freq);

    unordered_map<char,string> codes;
    buildCodes(root, "", codes);

    cout << "\nHuffman Codes:\n";
    for (auto &p : codes)
        cout << p.first << " : " << p.second << "\n";

    string encoded = encode(text, codes);
    cout << "\nEncoded Bitstring: " << encoded << "\n";

    string decoded = decode(encoded, root);
    cout << "Decoded Text: " << decoded << "\n";

    double ratio = (double)encoded.size() / (text.size()*8) * 100;
    cout << "Compression Ratio: " << ratio << "%\n";

    return 0;
}
