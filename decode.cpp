#include <iostream>
#include "huffman.h"

using namespace std;

int main(int argc, char** argv){
    Huffman huff;
    string header = huff.read_encoding();
    Node* root = huff.tree_from_header(header);
    huff.decode(root);
    return 0;
}