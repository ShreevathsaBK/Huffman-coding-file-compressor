#include <unordered_map>
#include <queue>
#include <string>

using namespace std;

class Node{
	public:
		char character;
		int frequency;
		Node* left;
		Node* right;

		Node(){
			left = NULL;
			right = NULL;
			frequency = 0;
		}
};

class Compare{
	public:
		bool operator()(Node* node1, Node* node2){
			return node1->frequency > node2->frequency;
		}
};


class Huffman{
	private:
		unordered_map<char, int> frequencies;
		unordered_map<char, string> huffman_codes;
		priority_queue<Node*, vector<Node*>, Compare> minHeap;
	
	public:
		Huffman(){}
		string filename;
		void readfile();
		void insert_to_minHeap();
		Node* build_tree();
		void traverse(Node*, string&, string);
		void write_header(Node* root);
		void encode();

		string read_encoding();
		Node* tree_from_header(string&);
		void decode(Node*);
		
};
