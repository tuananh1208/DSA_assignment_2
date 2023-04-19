#include "main.h"


// Huffman Tree node abstract base class
template <class T> 
class HuffNode {
public:
	virtual ~HuffNode() {}	// Destructor
	virtual int freq() = 0;
	virtual bool isLeaf() = 0;
};

template <class T>	// Leaf node subclass
class LeafNode : public HuffNode<T> {
private:
	T Data;   // Data
	int Freq; // freq
public:
	LeafNode(const T& data, int freq) : Data(data), Freq(freq) {}
	T data() { return Data; }
	int freq() { return this->Freq; }
	bool isLeaf() { return true; }
};

template <class T>	// Internal node subclass
class IntlNode : public HuffNode<T> {
private:
	HuffNode<T>* left; // Left child
	HuffNode<T>* right; // Right child
	int Freq; // freq
public:
	IntlNode(HuffNode<T>* leftChild, HuffNode<T>* rightChild) {
		this->Freq = leftChild->freq() + rightChild->freq();
		this->left = leftChild;
		this->right = rightChild;
	}
	int freq() { return this->Freq; }
	bool isLeaf() { return false; }

	HuffNode<T>* getLeft() { return left; }
	HuffNode<T>* getRight() { return right; }

	void setLeft(HuffNode<T>* leftChild) { this->left = leftChild; }
	void setRight(HuffNode<T>* rightChild) { this->right = rightChild; }
};

template <class T>
class HuffTree {
public: // change to private
	HuffNode<T>* Root;
public:
	HuffTree(T data, int freq) {	// Leaf constructor
		Root = new LeafNode<T>(data, freq);
	}

	HuffTree(HuffTree<T>* left, HuffTree<T>* right) {		// Internal node constructor
		Root = new IntlNode<T>(left->root(), right->root());
	}

	~HuffTree() { // Destructor
		deleteHuffTree(this->Root);
	}	
	
	HuffNode<T>* root() { return Root; }
	int freq() { return Root->freq(); }
	bool isLeaf() { return Root->isLeaf(); }
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
    bool operator()(pair<HuffTree<char>*, int> a, pair<HuffTree<char>*, int> b) {
		if (a.first->freq() == b.first->freq()) {
			if (a.first->isLeaf() && b.first->isLeaf()) {
				return static_cast<LeafNode<char>*>(a.first->root())->data() > static_cast<LeafNode<char>*>(b.first->root())->data();
			} else {
				return a.second > b.second;
			}
		} else {
			return a.first->freq() > b.first->freq();
		}
    }
};

HuffTree<char>* buildHuffTree(string name) {
	unordered_map<char, int> freq_map; 
	for (char x : name) {
		freq_map[x]++;
	}
	int order = 0;
	priority_queue<pair<HuffTree<char>*, int>, vector<pair<HuffTree<char>*, int>>, Compare<char>> freq_queue;//, temp_queue;
	for (auto x : freq_map) {
		freq_queue.push(make_pair(new HuffTree<char>(x.first, x.second), order++));
		//temp_queue.push(make_pair(new HuffTree<char>(x.first, x.second), order++));
	}

	// while (temp_queue.size() > 0) {// del
	// 	cout << temp_queue.top().first->freq() << "-" << static_cast<LeafNode<char>*>(temp_queue.top().first->root())->data() << ";";// del
	// 	temp_queue.pop();// del
	// }// del
	// cout << endl;// del

	while (freq_queue.size() > 1) {
		HuffTree<char>* left = freq_queue.top().first;
		freq_queue.pop();
		HuffTree<char>* right = freq_queue.top().first;
		freq_queue.pop();
		HuffTree<char>* parent = new HuffTree<char>(left, right);
		freq_queue.push(make_pair(parent, order++));
		//temp_queue = freq_queue; del
		// while (temp_queue.size() > 0) {// del
		// 	cout << temp_queue.top().first->freq() << "-" << static_cast<LeafNode<char>*>(temp_queue.top().first->root())->data() << ";";// del
		// 	temp_queue.pop();// del
		// }// del
		// cout << endl;// del
	}	
	return freq_queue.top().first;
}

void encodeHuffTree(HuffNode<char>* node, string path, unordered_map<char, string>& code_map) {
	if (!node) {
		return;
	}

	if (node->isLeaf()) {
		code_map[static_cast<LeafNode<char>*>(node)->data()] = path;
	} else {
		encodeHuffTree(static_cast<IntlNode<char>*>(node)->getLeft(), path + "0", code_map);
		encodeHuffTree(static_cast<IntlNode<char>*>(node)->getRight(), path + "1", code_map);
	}
}

string getHuffString(string text) {
	HuffTree<char>* tree = buildHuffTree(text);
	unordered_map<char, string> code_map;

	encodeHuffTree(tree->root(), "", code_map);

	string Huff_string;
	for (char x : text) {
		Huff_string += code_map[x];
	}

	return Huff_string;
}

bool checkName(string name) {
	for (char x : name) {
		if (!isalpha(x)) {
			return false;
		}
	}
	return true;
}

int convertBinToDec(string bin) {
	int dec = 0;
	for (int i = bin.size() - 1, j = 0; i >= 0; i--, j++) {
		if (bin[i] == '1') {
			dec += pow(2, j);
		}
	}
	return dec;
}

void reg(string command) {
	// check valid REG command
	if (command == "REG" || command == "REG ") {
		return;
	}

	string name = command.substr(command.find(" ") + 1);
	if (!checkName(name)) {
		return;
	} 

	string Huff_string = getHuffString(name); 
	cout << Huff_string << endl; // del
	if (Huff_string.size() > 15) {
		int start = Huff_string.size() - 15;
		int end = Huff_string.size() - 1;
		Huff_string = Huff_string.substr(start, end - start + 1);
	}
	

	int Huff_dec = convertBinToDec(Huff_string);
	cout << Huff_dec << endl; // del




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