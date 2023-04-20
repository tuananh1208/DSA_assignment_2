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

class HashTable {
private:
	class HashNode {
	public:
		int result;
		int ID;
		HashNode(int ID = 0, int result = 0) {
			this->ID = ID;
			this->result = result;
		}
		~HashNode() {}
	};
	int size; // max = MAXSIZE/2
	const int max_size = MAXSIZE / 2;
	vector<HashNode*> table;
public:
	HashTable() {
		size = 0;
		table.resize(max_size, nullptr);
	}
	~HashTable() {
		for (int i = 0; i < max_size; i++) {
			delete table[i];
		}
	}
	int hash_function(HashNode* node) {
		return node->ID % max_size;
	}

	void insert(int ID, int result) {
		if (size == max_size) {
			// table is full
			return;
		}
		HashNode* node = new HashNode(ID, result);
		int index = hash_function(node);
		while (table[index] != nullptr) {
			index = (index + 1) % max_size;
		}
		table[index] = node;
		size++;
	}

	void delete_by_ID(int ID) {
		for (int i = 0; i < max_size; i++) {
			if (table[i]->ID == ID) {
				table[i] = nullptr;
				size--;
				break;
			}
		}
	}

	void print() {
		for (int i = 0; i < max_size; i++) {
			if (table[i] != nullptr) {
				cout << table[i]->ID << "-" << table[i]->result << endl;
			}
		}
	}
};

class AVLTree {
private:
	class Node {
	public:
		int ID;
		int result; // <- key
		Node* left;
		Node* right;
		int height;

        Node(int ID, int result) {
            this->ID = ID;
            this->result = result;
            this->left = nullptr;
            this->right = nullptr;
            this->height = 1;
        }
		//Node(int ID = 0, int result = 0, Node* left = nullptr, Node* right = nullptr, int height = 1) : ID(ID), result(result), left(left), height(height) {}
		~Node() {}
	};

	Node* root;

	int const getHeight(Node* node) {
		if (node == nullptr) {
			return 0;
		}
		return node->height;
	}

	void updateHeight(Node* node) {
		if (node == nullptr) {
			return;
		}
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	}
	Node *rotateLeft(Node* node) {
		Node* right = node->right;
		node->right = right->left;
		right->left = node;
		updateHeight(node);
		updateHeight(right);
		
		return right;
	}

	Node *rotateRight(Node* node) {
		Node* left = node->left;
		node->left = left->right;
		left->right = node;
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		left->height = max(getHeight(left->left), getHeight(left->right)) + 1;
		return left;
	}

	int getBalance(Node* node) {
		if (node == nullptr) {
			return 0;
		} else {
			return getHeight(node->left) - getHeight(node->right);
		}
	}

	void deleteAVLTree(Node* node) {
		if (node == nullptr) {
			return;
		} else {
			deleteAVLTree(node->left);
			deleteAVLTree(node->right);
			delete node;
		}
	}

	Node* insert(Node* node, int ID, int result) {
		if (node == nullptr) {
			node = new Node(ID, result);
			return node;
		}

		if (result < node->result) {
			node->left = insert(node->left, ID, result);
		} else {
			node->right = insert(node->right, ID, result);
		}

		updateHeight(node);

		int balance = getBalance(node);
		// left left case
		if (balance > 1 && result < node->left->result) {
			return rotateRight(node);
		}
		// right right case
		if (balance < -1 && result > node->right->result) {
			return rotateLeft(node);
		}
		// left right case
		if (balance > 1 && result > node->left->result) {
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}
		// right left case
		if (balance < -1 && result < node->right->result) {
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}
		return node;
	}

	Node* minValueNode(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}
		Node* current = node;
		while (current->left != nullptr) {
			current = current->left;
		}
		return current;
	}

	Node *remove(Node* node, int result) {
		if (node == nullptr) {
			return node;
		}
		if (result < node->result) {
			node->left = remove(node->left, result);
		} else if (result > node->result) {
			node->right = remove(node->right, result);
		} else {
			// Node with one child or no child
			if ((node->left == nullptr) || (node->right == nullptr)) {
				Node* temp = node->left ? node->left : node->right;
				// No child case
				if (temp == nullptr) {
					temp = node;   	// ???
					node = nullptr; // ???
				} else {
					// One child case
					*node = *temp;
				}
				delete temp;
			} else {
				// Two children case
				Node* temp = minValueNode(node->right);
				node->result = temp->result;
				node->ID = temp->ID;
				node->right = remove(node->right, temp->result);
			}
		}
		if (node == nullptr) {
			return node;
		}

		updateHeight(node);

		int balance = getBalance(node);
		if (balance > 1 && getBalance(node->left) >= 0) {
			return rotateRight(node);
		}
		if (balance > 1 && getBalance(node->left) < 0) {
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}
		if (balance < -1 && getBalance(node->right) <= 0) {
			return rotateLeft(node);
		}
		if (balance < -1 && getBalance(node->right) > 0) {
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}

		return node;
	}
public:
	AVLTree() {
		root = nullptr;
	}
	~AVLTree() {
		deleteAVLTree(root);
	}

	void insert(int ID, int result) {
		root = insert(root, ID, result);
	}

	void remove(int result) {
		root = remove(root, result);
	}

	void print() {
		// print bfs
		queue<Node*> q;
		q.push(root);

		while (!q.empty()) {
			Node* node = q.front();
			q.pop();
			if (node->left != nullptr) {
				q.push(node->left);
			}
			if (node->right != nullptr) {
				q.push(node->right);
			}
			cout << node->ID << "-" << node->result << endl;
		}
	}
};


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






























void cle() {

}

void printHT(HashTable *hash_table) {
	hash_table->print();
}

void printAVL() {

}

void printMH() {
	
}

void simulate(string filename)
{
	HashTable* hash_table = new HashTable();
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
			printHT(hash_table);
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