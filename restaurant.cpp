#include "main.h"


// Huffman Tree node abstract base class
template <class T> 
class HuffNode {
public:
	virtual ~HuffNode() {}	// Destructor
	virtual int frequency() = 0;
	virtual bool isLeaf() = 0;
};

template <class T>	// Leaf node subclass
class LeafNode : public HuffNode<T> {
private:
	T data;   // Data
	int freq; // Frequency
public:
	LeafNode(const T& data, int freq) : data(data), freq(freq) {}
	T getData() { return data; }
	int frequency() { return freq; }
	bool isLeaf() { return true; }
};

template <class T>	// Internal node subclass
class IntlNode : public HuffNode<T> {
private:
	HuffNode<T>* left; // Left child
	HuffNode<T>* right; // Right child
	int freq; // Frequency
public:
	IntlNode(HuffNode<T>* leftChild, HuffNode<T>* rightChild) {
		freq = leftChild->frequency() + rightChild->frequency();
		this->left = leftChild;
		this->right = rightChild;
	}
	int frequency() { return freq; }
	bool isLeaf() { return false; }

	HuffNode<T>* getLeft() { return left; }
	HuffNode<T>* getRight() { return right; }

	void setLeft(HuffNode<T>* leftChild) { this->left = leftChild; }
	void setRight(HuffNode<T>* rightChild) { this->right = rightChild; }
};

template <class T>
class HuffTree {
private:
	HuffNode<T>* root;
public:
	HuffTree(T data, int freq) {	// Leaf constructor
		root = new LeafNode<T>(data, freq);
	}

	HuffTree(HuffTree<T>* left, HuffTree<T>* right) {		// Internal node constructor
		root = new IntlNode<T>(left->root, right->root);
	}

	~HuffTree() { // Destructor
		deleteHuffTree(root);
	}	
	HuffNode<T>* getRoot() { return root; }
	int frequency() { return root->frequency(); }

private:
	void deleteHuffTree(HuffNode<T>* node) {
		if (node) {
			deleteHuffTree(node->getLeft());
			deleteHuffTree(node->getRight());
			delete node;
		}
	}
};

template <class T>
struct Compare {
    bool operator()(HuffTree<T>* a, HuffTree<T>* b) {
        return a->frequency() > b->frequency();
    }
};

HuffTree<char>* buildHuffTree(string name) {
	unordered_map<char, int> freq_map; 
	for (char x : name) {
		freq_map[x]++;
	}

	priority_queue<HuffTree<char>*, vector<HuffTree<char>*>, Compare<char>> freq_queue;
	for (auto x : freq_map) {
		freq_queue.push(new HuffTree<char>(x.first, x.second));
	}

	while (freq_queue.size() > 1) {
		HuffTree<char>* left = freq_queue.top();
		freq_queue.pop();
		HuffTree<char>* right = freq_queue.top();
		freq_queue.pop();
		HuffTree<char>* parent = new HuffTree<char>(left, right);
		freq_queue.push(parent);
	}

	return freq_queue.top();
}

void encodeHuffTree(HuffNode<char>* node, string path, unordered_map<char, string>& code_map) {
	if (!node) {
		return;
	}

	if (node->isLeaf()) {
		code_map[static_cast<LeafNode<char>*>(node)->getData()] = path;
	} else {
		encodeHuffTree(static_cast<IntlNode<char>*>(node)->getLeft(), path + "0", code_map);
		encodeHuffTree(static_cast<IntlNode<char>*>(node)->getRight(), path + "1", code_map);
	}
}

string getHuffString(string text) {
	HuffTree<char>* tree = buildHuffTree(text);
	unordered_map<char, string> code_map;

	encodeHuffTree(tree->getRoot(), "", code_map);

	string Huff_string;
	for (char x : text) {
		Huff_string += code_map[x];
	}

	return Huff_string;
}



void reg(string command) {
	// check valid name
	cout << "command: " << command << endl;   // del
	if (command.find(" ") == string::npos || command.find(" ") != command.rfind(" ")) {
		return;
	}
	string name = command.substr(command.find(" ") + 1);

	if (!checkName(name)) {
		return;
	}



	string Huff_string = getHuffString(name);


}

void simulate(string filename)
{
	ifstream myfile(filename);
	string command;
	while (getline(myfile, command)) {
		string key = command.substr(0, command.find(" "));

		if (key == "REG") {
			//
			reg(command);
		} else if (key == "CLE") {
			//
		} else if (key == "PrintHT") {
			//
		} else if (key == "PrintAVL") {
			//
		} else if (key == "PrintMH") {
			//
		} else {
			//
			return;
		}
	}
	return;
}




/*
REG Johnuigfifbahjasbdfhjbasdhjf
REG iuasgfuigweibjaskdfbjksadf
REG iuiwehruihqwUIAGSIDiernbsandfb
REG uiewhqruihqiuwerhnbdasnbfnmasd
REG ukkajhsdfjkasbndbmnFJKHJKsdbfsabdf
REG kjasdbfjksbDFFDFFfdjksabfdjkasdf
REG skjafdbkjasfnmFFFFsaddnfdf
CLE 5
PrintHT
PrintAVL
PrintMH
*/