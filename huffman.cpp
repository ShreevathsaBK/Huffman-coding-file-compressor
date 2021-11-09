#include <iostream>
#include <fstream>
#include <algorithm>
#include "huffman.h"

#define BYTE 8
#define UNIT_SEPERATOR 31

using namespace std;

void Huffman::readfile(){
	ifstream file;
	file.open(filename, ios::in);

	char ch;
	while(file.get(ch)){
		frequencies[ch]++;
	}
	file.close();
}


void Huffman::insert_to_minHeap(){
	for(auto& it : frequencies){
		Node* new_node = new Node();
		new_node->character = it.first;
		new_node->frequency = it.second;
		minHeap.push(new_node);
	}
}


Node* Huffman::build_tree(){
	while(minHeap.size() > 1){
		Node* top_node = new Node;
		top_node->left = minHeap.top();
		top_node->frequency += minHeap.top()->frequency;
		minHeap.pop();
		top_node->right = minHeap.top();
		top_node->frequency += minHeap.top()->frequency;
		minHeap.pop();
		minHeap.push(top_node);
	}
	return minHeap.top();
}


void Huffman::traverse(Node* root, string& header, string h_code){
	if(!root)
		return;
	
	if(!root->left && !root->right){
		header += '1';
		header += root->character;
		huffman_codes[root->character] = h_code;
	}
	else{
		header += '0';
		traverse(root->left, header, h_code + '0');
		traverse(root->right, header, h_code + '1');
	}
}


void Huffman::write_header(Node* root){
	string header;
	traverse(root, header, "");

	ofstream file;
	file.open("Encoding.txt", ios::out);
	char ch = UNIT_SEPERATOR;

	if(file.is_open()){
		file << header;
		file << ch;
		file.close();
	}
}


void Huffman::encode(){
	ifstream text_file;
	text_file.open(filename, ios::in);
	
	char ch;
	string binaryString;
	
	while(text_file.get(ch)){
		binaryString += huffman_codes[ch];
	}
	text_file.close();

	ofstream file;
	file.open("Encoding.txt", ios::out | ios::app | ios::binary);

	char byte = 0;
	int N = binaryString.size();
	for(int i=1; i<=N; ++i){
		if(binaryString[i-1] == '1'){
			byte |= (1 << (BYTE-1 - ((i-1) % BYTE)));
		}

		if(i % BYTE == 0){
			file << byte;
			byte = 0;
		}
	}

	if(N % BYTE != 0){
		file << byte;
	}

	file << N % BYTE;
	file.close();
}


string Huffman::read_encoding(){
	ifstream file;
	file.open("Encoding.txt", ios::in);

	string header;
	char ch;

	while(file.get(ch)){
		if(ch == UNIT_SEPERATOR){
			break;
		}else{
			header.push_back(ch);
		}
	}
	
	reverse(header.begin(), header.end());
	return header;
}


Node* Huffman::tree_from_header(string& header){
	if(header.back() == '1'){
		header.pop_back();
		Node* root = new Node;
		root->character = header.back();
		header.pop_back();
		return root;
	}
	else{
		if(header == "") return NULL;
		header.pop_back();

		Node* left = tree_from_header(header);
		Node* right = tree_from_header(header);
		
		Node* root = new Node;
		root->left = left;
		root->right = right;
		return root;
	}
}


void Huffman::decode(Node* root){
	ifstream file;
	ofstream outfile;

	file.open("Encoding.txt", ios::in | ios::binary);
	outfile.open("Decoded.txt", ios::out | ios::binary);

	char ch, lastByte;
	int last, end;

	file.seekg(-1, ios::end);
	file.get(ch);
	last = ch - '0';
	file.seekg(-2, ios::end);
	file.get(lastByte);
	end = file.tellg();

	int pos = 1, flag = 0;
	file.seekg(0, ios::beg);
	Node* curr = root;

	while(pos != end){
		file.get(ch);
		if(flag){
			for(int i=0; i<BYTE; ++i){
				if(ch & (1<<(BYTE - 1 - i))){
					curr = curr->right;
				}
				else{
					curr = curr->left;
				}
				if(!curr->left && ! curr->right){
					outfile << curr->character;
					curr = root;
				}
			}
		}
		if(ch == UNIT_SEPERATOR){
			flag = 1;
		}
		++pos;
	}

	for(int i=0; i<last; ++i){
		if(lastByte & (1<<(BYTE - 1 - i))){
			curr = curr->right;
		}
		else{
			curr = curr->left;
		}
		if(!curr->left && ! curr->right){
			outfile << curr->character;
			curr = root;
		}
	}

	file.close();
	outfile.close();
}