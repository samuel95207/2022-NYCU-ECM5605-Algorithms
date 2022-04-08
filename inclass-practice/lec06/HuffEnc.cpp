/* A greedy algorithm C++/STL implementation for the Huffman Encoding problem;  
modified from Aditya Goel's code at http://www.geeksforgeeks.org */   
#include <iostream>
#include <string>
#include <queue>
using namespace std;
 
// A Huffman tree node
struct MinHeapNode {
  char data;                 // One of the input characters
  unsigned freq;             // Frequency of the character
  MinHeapNode *left, *right; // Left and right child

  MinHeapNode(char data, unsigned freq) {
    left = right = NULL;
    this->data = data;
    this->freq = freq;
  }
};
 
// For comparison of two heap nodes (needed in min heap)
struct compare {
  bool operator()(MinHeapNode* l, MinHeapNode* r) {
    return (l->freq > r->freq);
  }
};
 
// Prints huffman codes from the root of Huffman Tree.
void printCodes(struct MinHeapNode* root, string str) {
  if (!root)
      return;
  if (root->data != '$')
      cout << root->data << ": " << str << "\n";
  printCodes(root->left, str + "0");
  printCodes(root->right, str + "1");
}
 
// Main function that builds a Huffman Tree and print codes by traversing the Tree
void HuffmanCodes(char data[], int freq[], int size) {
  struct MinHeapNode *left, *right, *top;
  // Create a min heap & inserts all characters of data[]
  priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
  for (int i = 0; i < size; ++i)
    minHeap.push(new MinHeapNode(data[i], freq[i]));
  // Iterate while size of heap doesn't become 1
  while (minHeap.size() != 1) {
    // Extract the two minimum freq items from min heap
    left = minHeap.top();
    minHeap.pop();

    right = minHeap.top();
    minHeap.pop();

    // Create a new internal node with frequency equal to the sum of the two nodes frequencies. 
		// Make the two extracted node as left and right children of this new node. 
		// Add this node to the min heap; '$' is a special value for internal nodes, not used
    top = new MinHeapNode('$', left->freq + right->freq);
    top->left = left;
    top->right = right;
    minHeap.push(top);
  }

  // Print Huffman codes using the Huffman tree built above
  printCodes(minHeap.top(), "");
}
 
// Driver program to test above functions
int main() {
	//test #1 
    char arr[] = {' ','E','T','A','O','N','R','I','S','H','D','L','F','C','M','U','G','Y','P','W','B','V','K','J','X','Q','Z'};
    int freq[] = {15, 12.42, 9.69, 8.20, 7.68, 7.64, 7.14, 7.07, 6.68, 4.48, 3.63, 3.50, 3.44, 2.88, 2.82, 2.35, 2.03, 1.89, 1.81, 1.35, 1.25, 1.07, 0.39, 0.22, 0.20, 0.09, 0.05};
    //test #2
    //char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f'};
    //int freq[] = { 5, 9, 12, 13, 16, 45 };
    
	int size = sizeof(arr) / sizeof(arr[0]);
 
    HuffmanCodes(arr, freq, size);
 
    return 0;
}
// This code is 
