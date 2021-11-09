#include <iostream>
#include "huffman.h"

using namespace std;

int main(int argc, char** argv){
    string filename = argv[1];
    Huffman huff;
    huff.filename = filename;
    huff.readfile();
    huff.insert_to_minHeap();
    Node* root = huff.build_tree();
    huff.write_header(root);
    huff.encode();
    return 0;
}
