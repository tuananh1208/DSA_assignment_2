#include "main.h"

enum Area {area1 = 1, area2 = 2};

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
	if (code_map.size() == 1) {
		for (auto &x : code_map) {
			x.second = "1";
		}
	}
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

bool checkID(string ID) {
	if (ID[0] != '-' && !isdigit(ID[0])) {
		return false;
	}
	for (int i = 1; i < ID.size(); i++) {
		if (!isdigit(ID[i])) {
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
		int ID;
		int result;
		string name;
		int num; // so lan goi mon
		HashNode(int ID = 0, int result = 0, string name = "", int num = 0) {
			this->ID = ID;
			this->result = result;
			this->name = name;
			this->num = num;
		}
		~HashNode() {}
	};
	int size; // max = MAXSIZE/2
	const int max_size = MAXSIZE / 2;
	vector<HashNode*> table;
public:
	bool isFull() {
		return size >= max_size;
	}
	HashTable() {
		size = 0;
		table.resize(max_size, nullptr);
	}
	~HashTable() {
		clear();
	}
	int hash_function(int result) {
		return result % max_size;
	}

	void insert(int ID, int result, string name) {
		// insert new customer
		if (size >= max_size) {
			// table is full
			return;
		}
		HashNode* node = new HashNode(ID, result, name, 1);
		int index = hash_function(node->result);
		while (table[index] != nullptr) {
			index = (index + 1) % max_size;
		}
		table[index] = node;
		size++;
	}

	void updateNum(int result, string name) {
		for (int i = 0; i < max_size; i++) {
			if (table[i] && table[i]->name == name) {
				table[i]->num++;
				return;
			}
		}
	}

	void remove(int result, string name) {
		for (int i = 0; i < max_size; i++) {
			if (table[i] && table[i]->name == name) {
				table[i] = nullptr;
				size--;
				return;
			}
		}
	}

	void print() {
		for (int i = 0; i < max_size; i++) {
			if (table[i] != nullptr) {
				cout << table[i]->ID << "-" << table[i]->result << "-" << table[i]->num << endl;
			}
		}
	}

	void clear() {
		for (int i = 0; i < max_size; i++) {
			if (table[i] != nullptr) {
				delete table[i];
				table[i] = nullptr;
			}
		}
		size = 0;
	}
};

class AVLTree {
private:
	class Node {
	public:
		int ID;
		int result; // <- key
		string name;
		int num;
		Node* left;
		Node* right;
		int height;

        Node(int ID, int result, string name) {
            this->ID = ID;
            this->result = result;
			this->name = name;
			this->num = 1;
            this->left = nullptr;
            this->right = nullptr;
            this->height = 1;
        }
		//Node(int ID = 0, int result = 0, Node* left = nullptr, Node* right = nullptr, int height = 1) : ID(ID), result(result), left(left), height(height) {}
		~Node() {}
	};

	Node* root;
	int size;
	const int max_size = MAXSIZE / 2;

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
		root = nullptr;
		size = 0;
	}

	Node* insert(Node* node, int ID, int result, string name) {
		if (node == nullptr) {
			node = new Node(ID, result, name);
			size++;
			return node;
		}

		if (result < node->result) {
			node->left = insert(node->left, ID, result, name);
		} else {
			node->right = insert(node->right, ID, result, name);
		}

		updateHeight(node);

		int balance = getBalance(node);
		// left left case
		if (balance > 1 && result < node->left->result) {
			return rotateRight(node);
		}
		// right right case
		if (balance < -1 && result >= node->right->result) {
			return rotateLeft(node);
		}
		// left right case
		if (balance > 1 && result >= node->left->result) {
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

	void updateNum(Node* node, int result, string name) {
		if (node == nullptr) {
			return;
		}
		if (result < node->result) {
			updateNum(node->left, result, name);
		} else if (result > node->result) {
			updateNum(node->right, result, name);
		} else if (node->name == name) {
			node->num++;
		} else {
			updateNum(node->left, result, name);
			updateNum(node->right, result, name);
		}
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

	Node *remove(Node* node, int result, string name) {
		if (node == nullptr) {
			return node;
		}
		if (result < node->result) {
			node->left = remove(node->left, result, name);
		} else if (result > node->result) {
			node->right = remove(node->right, result, name);
		} else if (node->name == name) {
			
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
				size--;
			} else {
				// Two children case
				Node* temp = minValueNode(node->right);
				node->result = temp->result;
				node->ID = temp->ID;
				node->name = temp->name;
				node->num = temp->num;
				node->right = remove(node->right, temp->result, temp->name);
			}
		} else {
			//return node;
			node->left = remove(node->left, result, name);
			node->right = remove(node->right, result, name);
		}
		if (node == nullptr) {
			return node;
		}

		updateHeight(node);
		// ll
		int balance = getBalance(node);
		if (balance > 1 && getBalance(node->left) >= 0) {
			return rotateRight(node);
		}
		// lr
		if (balance > 1 && getBalance(node->left) < 0) {
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}
		// rr
		if (balance < -1 && getBalance(node->right) <= 0) {
			return rotateLeft(node);
		}
		// rl
		if (balance < -1 && getBalance(node->right) > 0) {
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}

		return node;
	}
public:
	AVLTree() {
		size = 0;
		root = nullptr;
	}
	~AVLTree() {
		deleteAVLTree(root);
	}

	void clear() {
		deleteAVLTree(root);
	}
	
	int getSize() {
		return this->size;
	}
	bool isFull() {
		return this->size >= max_size;
	}

	void insert(int ID, int result, string name) {
		if (this->size >= max_size) {
			return;
		}
		root = insert(root, ID, result, name);
	}

	void updateNum(int result, string name) {
		if (this->size <= 0) {
			return;
		}
		updateNum(root, result, name);
	}

	void remove(int result, string name) {
		if (this->size <= 0) {
			return;
		}
		root = remove(root, result, name);
	}

	void print() {
		// print bfs
		// "ID-result-num"
		if (size <= 0) {
			return;
		}
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
			cout << node->ID << "-" << node->result << "-" << node->num << endl;
		}
	}
};

class LinkedList {
private:
	class Node {
	public:
		int result;
		int ID;
		string name;
		Node* next;
		Area area;

		Node() {
			result = 0;
			ID = 0;
			name = "";
			next = NULL;
		}

		Node(int result, int ID, string name, Area area) {
			this->result = result;
			this->ID = ID;
			this->name = name;
			this->next = NULL;
			this->area = area;
		}
	};
	Node* head;
	int size;

	void deleteLinkedlist(Node* node) {
		if (node == NULL) {
			return;
		}
		deleteLinkedlist(node->next);
		delete node;
	}

public:
	LinkedList() {
		size = 0;
		head = nullptr;
	}

	~LinkedList() {
		deleteLinkedlist(head);
		this->size = 0;
	}

	void insertNode(int result, int ID, string name, Area area) {
		Node* newNode = new Node(result, ID, name, area);
		
		if(head == NULL) {
			head = newNode;
			size++;
			return;
		}

		Node* temp = head;
		while(temp->next != NULL) {
			temp = temp->next;
		}

		temp->next = newNode;
		size++;
	}

	void removeHead() {
		Node* temp = head;
		head = head->next;
		delete temp;
		size--;
	}

	int getSize() {
		return this->size;
	}

	void updateNum(int result, string name) {
		if (head->name == name) {
			insertNode(head->result, head->ID, head->name, head->area);
			removeHead();
			return;
		}

		Node* temp = head;
		Node* prev = nullptr;
		while (temp && temp->name != name) {
			prev = temp;
			temp = temp->next;
		}
		if (temp == nullptr) {
			return;
		}

		insertNode(temp->result, temp->ID, temp->name, temp->area);
		prev->next = temp->next;
		delete temp;
		size--;
	}
	Node* getHead() {
		return head;
	}

	void removeNode(int result, string name) {
		if (head->name == name) {
			removeHead();
			return;
		}
		Node* temp = head;
		Node* prev = nullptr;
		while (temp && temp->name != name) {
			prev = temp;
			temp = temp->next;
		}
		if (temp == nullptr) {
			return;
		}
		prev->next = temp->next;
		delete temp;
		size--;

	}

	vector<tuple<int, int, string>> getArea1IDListAndDelete() {
		vector<tuple<int, int, string>> result;
		Node* temp = head;
		while (temp) {
			if (temp->area == 1) {
				result.push_back(make_tuple(temp->ID, temp->result, temp->name));
				Node* prev = temp;
				temp = temp->next;
				removeNode(prev->result, prev->name);
				continue;
			}
			temp = temp->next;
		}
		return result;
	}

	void deleteArea1() {
		Node* temp = head;
		while (temp) {
			if (temp->area == 1) {
				removeNode(temp->result, temp->name);
			}
			temp = temp->next;
		}
	}
	vector<tuple<int, int, string>> getArea2IDListAndDelete() {
		vector<tuple<int, int, string>> result;
		Node* temp = head;
		while (temp) {
			if (temp->area == 2) {
				result.push_back(make_tuple(temp->ID, temp->result, temp->name));
				Node* prev = temp;
				temp = temp->next;
				removeNode(prev->result, prev->name);
				continue;
			}
			temp = temp->next;
		}
		return result;
	}

	void deleteArea2() {
		Node* temp = head;
		while (temp) {
			if (temp->area == 2) {
				removeNode(temp->result, temp->name);
			}
			temp = temp->next;
		}
	}
};

class MinHeap {
private:
	class Node {
	public:
		int ID;
		int num;
		int result;
		string name;
		int priority;
		Node(int ID, int result, string name, int priority) {
			this->ID = ID;
			this->num = 1;
			this->result = result;
			this->name = name;
			this->priority = priority;
		}
		Node(int ID, int result, string name, int num, int priority) {
			this->ID = ID;
			this->result = result;
			this->name = name;
			this->num = num;
			this->priority = priority;
		}
	};

	Node* heap[MAXSIZE];
	int max_size = MAXSIZE;
	int size;
	int increase_num;

	int parent(int i) {
		return (i - 1) / 2;
	}

	int left(int i) {
		return 2 * i + 1;
	}

	int right(int i) {
		return 2 * i + 2;
	}

	void swap(Node*& a, Node*& b) {
		Node* temp = a;
		a = b;
		b = temp;
	}

	void reheapUp(int pos) {
		if (pos <= 0 || pos >= size) {
			return;
		}
		if (heap[parent(pos)]->num > heap[pos]->num || (heap[parent(pos)]->num == heap[pos]->num && heap[parent(pos)]->priority > heap[pos]->priority)) {
			swap(heap[parent(pos)], heap[pos]);
			reheapUp(parent(pos));
		}
	}

	void reheapDown(int pos) {
		if (pos < 0 || pos >= size - 1) {
			return;
		}

		int l = left(pos); 
		if (l > size-1) {
			return;
		}
		int r = right(pos);
		int min_child;

		if (r > size-1) {
			min_child = l;
		} else {
			if (heap[l]->num < heap[r]->num || (heap[l]->num == heap[r]->num && heap[l]->priority < heap[r]->priority)) {
				min_child = l;
			} else {
				min_child = r;
			}
		}

		if (heap[pos]->num > heap[min_child]->num || (heap[pos]->num == heap[min_child]->num && heap[pos]->priority > heap[min_child]->priority)) {
			swap(heap[pos], heap[min_child]);
			reheapDown(min_child);
		}
	}
public:
	MinHeap() {
		this->size = 0;
		this->max_size = MAXSIZE;
		this->increase_num = 0;
	}
	~MinHeap() {
		for (int i = 0; i < max_size; i++) {
			if (heap[i]) {
				delete heap[i];
			}
		}
		this->size = 0;
		this->max_size = MAXSIZE;
		this->increase_num = 0;
	}

	void insert(int ID, int result, string name) {
		if (this->size >= max_size) {
			return;
		}

		Node* newNode = new Node(ID, result, name, increase_num++);
		heap[size++] = newNode;
		reheapUp(size-1);
	}

	void insert(Node* node) {
		if (this->size >= max_size) {
			return;
		}
		heap[size++] = node;
		reheapUp(size-1);
	}

	void updateNum(int result, string name) {
		for (int i = 0; i < size; i++) {
			if (heap[i]->name == name) {
				Node* temp = new Node(heap[i]->ID, heap[i]->result, heap[i]->name, heap[i]->num, heap[i]->priority);
				
				temp->num++;
				remove(i);
				insert(temp);
				return;
			}
		}
	}

	void remove(int pos) {
		if (pos < 0 || pos >= size || size == 0) {
			return;
		}
		delete heap[pos];
		heap[pos] = heap[size-1];
		heap[size-1] = nullptr;
		size--;
		reheapDown(pos);
	}


	void remove(int result, string name) {
		for (int i = 0; i < size; i++) {
			if (heap[i]->name == name) {
				remove(i);
				return;
			}
		}
	}

	Node* getHead() {
		return heap[0];
	}

	void removeTop() {
		remove(0);
	}

	void print(int index) {
		if (index >= size) {
			return;
		}
		cout << heap[index]->ID << "-" << heap[index]->num << endl;
		print(left(index));
		print(right(index));
	}
	void print() {
		print(0);
	}
};

void reg(string command, LinkedList* FIFO, LinkedList* LRCO, MinHeap* LFCO, map<int, pair<int, string>>& table, HashTable* area_1, AVLTree* area_2) {
	// check valid REG command
	if (command == "REG" || command == "REG ") {
		return;
	}

	string name = command.substr(command.find(" ") + 1);
	if (!checkName(name)) {
		return;
	} 

	// get Huffcode
	string Huff_string = getHuffString(name); 
	if (Huff_string.size() > 15) {
		int start = Huff_string.size() - 15;
		int end = Huff_string.size() - 1;
		Huff_string = Huff_string.substr(start, end - start + 1);
	}
	

	int result = convertBinToDec(Huff_string);

	// MAIN FUNCTION
	// check if result is [new_customer] or [new_order]
	bool customerExists = false;
	for (int i = 1; i <= MAXSIZE; i++) {
		if (table[i].first != -1 && table[i].second == name) {
			customerExists = true;
			break;
		}
	}	

	if (customerExists) { // [new_order]
		// update LRCO, min_heap, area 1, area 2
		LRCO->updateNum(result, name);
		LFCO->updateNum(result, name);
		area_1->updateNum(result, name);
		area_2->updateNum(result, name);
	} else { // [new_customer]
		int ID;
		if (FIFO->getSize() >= MAXSIZE) { // full
		
			int OPT = result % 3;
			int rm_result;
			string rm_name;

			switch (OPT) {
				case 0: { // FIFO
					rm_result = FIFO->getHead()->result;
					rm_name = FIFO->getHead()->name;
					ID = FIFO->getHead()->ID;
					break;
				}
				case 1: { // LRCO
					rm_result = LRCO->getHead()->result;
					rm_name = LRCO->getHead()->name;
					ID = LRCO->getHead()->ID;
					break;
				}
				case 2: { // LFCO
					rm_result = LFCO->getHead()->result;
					rm_name = LFCO->getHead()->name;
					ID = LFCO->getHead()->ID;
					break;
				}
			}

			area_1->remove(rm_result, rm_name);
			area_2->remove(rm_result, rm_name);
			FIFO->removeNode(rm_result, rm_name);
			LRCO->removeNode(rm_result, rm_name);				LFCO->remove(rm_result, rm_name);
			table[ID].first = -1;


		} else { // not full
			// find ID
			ID = result % MAXSIZE + 1;

			bool found = 0;
			for (int i = 1; i <= MAXSIZE; i++) {
				if (table[ID].first == -1) {
					table[ID].first = result;
					table[ID].second = name;
					found = 1;
					break;
				}
				ID++;
				if (ID > MAXSIZE) {
					ID = 1;
				}
			}

			if (!found) {
				cout << "error" << endl;
				return;
			}
		}	
		// cout << result << "-" << ID << endl; // del
		
		// choose area
		Area area;
		if (result % 2 == 1) { // insert to area 1
			if (area_1->isFull()) {
				area_2->insert(ID, result, name);
				area = area2;
			} else {
				area_1->insert(ID, result, name);
				area = area1;
			}
		} else { // insert to area 2
			if (area_2->isFull()) {
				area_1->insert(ID, result, name);
				area = area1;
			} else {
				area_2->insert(ID, result, name);
				area = area2;
			}
		}
		// update FIFO, LRCO, min_heap, table
		FIFO->insertNode(result, ID, name, area);
		LRCO->insertNode(result, ID, name, area);
		LFCO->insert(ID, result, name);
		table[ID].first = result;
		table[ID].second = name;
	}
}

void cle(string command, LinkedList* FIFO, LinkedList* LRCO, MinHeap* LFCO, map<int, pair<int, string>>& table, HashTable* area_1, AVLTree* area_2) {
	// check valid CLE command
	if (command == "CLE" || command == "CLE ") {
		return;
	}

	string NUM = command.substr(command.find(" ") + 1);
	if (!checkID(NUM)) {
		return;
	} 
	int ID = stoi(NUM);

	if (ID < 1) {	// clear area 1
		// update FIFO, LRCO, LFCO, min_heap, table, 
		vector<tuple<int, int, string>> info_list = FIFO->getArea1IDListAndDelete();
		for (auto x : info_list) {
			//FIFO->removeNode(get<1>(x), get<2>(x));
			LRCO->removeNode(get<1>(x), get<2>(x));
			LFCO->remove(get<1>(x), get<2>(x));
			area_1->remove(get<1>(x), get<2>(x));
			table[get<0>(x)].first = -1;
		}
	} else if (ID > MAXSIZE) {	// clear area 2
		vector<tuple<int, int, string>> info_list = FIFO->getArea2IDListAndDelete();
		for (auto x : info_list) {
			//FIFO->removeNode(get<1>(x), get<2>(x));
			LRCO->removeNode(get<1>(x), get<2>(x));
			LFCO->remove(get<1>(x), get<2>(x));
			area_2->remove(get<1>(x), get<2>(x));
			table[get<0>(x)].first = -1;
		}
	} else {
		if (table[ID].first != -1) {	// table is not empty
			int result = table[ID].first;
			string name = table[ID].second;
			// update FIFO, LRCO, !min_heap, table, area
			area_1->remove(result, name);
			area_2->remove(result, name);
			FIFO->removeNode(result, name);
			LRCO->removeNode(result, name);
			LFCO->remove(result, name);
			table[ID].first = -1;
		} else { // table is empty
			// do nothing
			return;
		}
	}
}

void printHT(HashTable *area_1) {
	area_1->print();
}

void printAVL(AVLTree *area_2) {
	area_2->print();
}

void printMH(MinHeap *LFCO) {
	LFCO->print();
}

void simulate(string filename)
{
	LinkedList* FIFO = new LinkedList();
	LinkedList* LRCO = new LinkedList();
	HashTable* area_1 = new HashTable();
	AVLTree* area_2 = new AVLTree();
	MinHeap* LFCO = new MinHeap();

	map<int, pair<int, string>> table; // .first = -1 is empty table
	for (int i = 1; i <= MAXSIZE; i++) {
		table[i].first = -1;
	}
	ifstream myfile(filename);
	string command;
	while (getline(myfile, command)) {
		string key = command.substr(0, command.find(" "));
		if (key == "REG") {
			reg(command, FIFO, LRCO, LFCO, table, area_1, area_2);
		} else if (key == "CLE") {
			cle(command, FIFO, LRCO, LFCO, table, area_1, area_2);
		} else if (key == "PrintHT") {
			printHT(area_1);
		} else if (key == "PrintAVL") {
			printAVL(area_2);
		} else if (key == "PrintMH") {
			printMH(LFCO);
		} else {
			//
			int n = 1;
			//return;
		}
	}

	delete FIFO;
	delete LRCO;
	delete LFCO;
	delete area_1;
	delete area_2;

	return;
}

