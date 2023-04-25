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
	if (Huff_string == "") {
		Huff_string = "1";
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
			if (table[i]->name == name) {
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

	// vector<pair<int, int>> getResultList() {
	// 	vector<pair<int, int>> result_list;
	// 	for (int i = 0; i < max_size; i++) {
	// 		if (table[i] != nullptr) {
	// 			result_list.push_back(make_pair(table[i]->ID, table[i]->result));
	// 		}
	// 	}
	// 	return result_list;
	// }

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
				node->right = remove(node->right, temp->result, name);
			}
		} else {
			return node;
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
		return this->size >= MAXSIZE;
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
	// vector<pair<int, int>> getResultList() {
	// 	vector<pair<int, int>> result_list;
	// 	queue<Node*> q;
	// 	q.push(root);

	// 	while (!q.empty()) {
	// 		Node* node = q.front();
	// 		q.pop();
	// 		if (node->left != nullptr) {
	// 			q.push(node->left);
	// 		}
	// 		if (node->right != nullptr) {
	// 			q.push(node->right);
	// 		}
	// 		result_list.push_back(make_pair(node->ID, node->result));
	// 	}

	// 	return result_list;
	// }
};


class Linkedlist {
private:
	class Node {
	public:
		int result;
		int ID;
		string name;
		Node* next;

		Node() {
			result = 0;
			ID = 0;
			name = "";
			next = NULL;
		}

		Node(int result, int ID, string name) {
			this->result = result;
			this->ID = ID;
			this->name = name;
			this->next = NULL;
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
	Linkedlist() {
		size = 0;
		head = nullptr;
	}

	~Linkedlist() {
		deleteLinkedlist(head);
		this->size = 0;
	}

	void insertNode(int result, int ID, string name) {
		Node* newNode = new Node(result, ID, name);
		
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

	void updateNode(int result, string name) {
		if (head->name == name) {
			insertNode(head->result, head->ID, head->name);
			removeHead();
			return;
		}

		Node* temp = head;
		Node* prev = nullptr;
		while (temp && temp->result != result) {
			prev = temp;
			temp = temp->next;
		}
		if (temp == nullptr) {
			return;
		}

		insertNode(temp->result, temp->ID);
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
		while (temp && temp->result != result) {
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
		Node(int ID, int num, int result, string name, int priority) {
			this->ID = ID;
			this->num = num;
			this->result = result;
			this->name = name;
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

	void swap(Node* a, Node* b) {
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

	void insert(int ID, int num, int result, string name) {
		if (this->size >= max_size) {
			return;
		}

		Node* newNode = new Node(ID, num, result, name, increase_num++);
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
				Node* temp = heap[i];
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


	void remove(string name) {
		for (int i = 0; i < size; i++) {
			if (heap[i]->name == name) {
				remove(i);
				return;
			}
		}
	}

	void removeTop() {
		remove(0);
	}

	void print(int index) {

	}
	void print() {
		if (heap[0]) {
			cout << heap[0]->ID << "-" << heap[0]->num << endl;
		}
	}
};

void reg(string command, Linkedlist* order_list, Linkedlist* customer_list, map<int, int>& table, HashTable* zone_1, AVLTree* zone_2) {
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
	

	int result = convertBinToDec(Huff_string);
	cout << result << endl; // del


	// MAIN FUNCTION
	// check if result is [new_customer] or [new_order]
	bool customerExists = false;
	for (int i = 1; i <= MAXSIZE; i++) {
		if (table[i] == result) {
			customerExists = true;
			break;
		}
	}	

	if (customerExists) { // [new_order]
		// update order_list, min_heap, zone 1, zone 2
		order_list->updateNode(result, name);
		zone_1->updateNum(result, name);
		zone_2->updateNum(result, name);
	} else { // [new_customer]
		if (customer_list->getSize() >= MAXSIZE) { // full
		
			int OPT = result % 3;

			switch (OPT) {
			case 0:
				//
			case 1:
				//
			case 2:
				//
			}
		} else { // not full
			int ID = result % MAXSIZE + 1;

			if (table[ID] == -1) {
				table[ID] = result;
			} else {
				bool found = 0;
				for (int i = 1; i <= MAXSIZE; i++) {
					ID++;
					if (ID > MAXSIZE) {
						ID = 1;
					}
					if (table[ID] == -1) {
						table[ID] = result;
						found = 1;
						break;
					}
				}

				if (!found) {
					cout << "error" << endl;
					return;
				}
			}

			// update customer_list, order_list, min_heap, (table - above)
			customer_list->insertNode(result, ID, name);
			order_list->insertNode(result, ID, name);
			
			// choose zone
			if (result % 2 == 1) { // insert to zone 1
				if (zone_1->isFull()) {
					zone_2->insert(ID, result, name);
				} else {
					zone_1->insert(ID, result, name);
				}
			} else { // insert to zone 2
				if (zone_2->isFull()) {
					zone_1->insert(ID, result, name);
				} else {
					zone_2->insert(ID, result, name);
				}
			}

		}
		
	}




	


}






void cle(string command, Linkedlist* order_list, Linkedlist* customer_list, map<int, int>& table, HashTable* zone_1, AVLTree* zone_2) {
	// check valid CLE command
	if (command == "CLE" || command == "CLE ") {
		return;
	}

	string NUM = command.substr(command.find(" ") + 1);
	if (!checkID(NUM)) {
		return;
	} 

	int ID = stoi(NUM);
	if (ID < 1) {	// clear zone 1
		vector<pair<int, int>> result_list = zone_1->getResultList();
		zone_1->clear();
		// update customer_list, order_list, !min_heap, table
		for (pair<int, int> x : result_list) {
			customer_list->removeNode(x.second);
			order_list->removeNode(x.second);
			table[x.first] = -1;
		}
	} else if (ID > MAXSIZE) {	// clear zone 2
		vector<pair<int, int>> result_list = zone_2->getResultList();
		zone_2->clear();
		// update customer_list, order_list, !min_heap, table
		for (pair<int, int> x : result_list) {
			customer_list->removeNode(x.second);
			order_list->removeNode(x.second);
			table[x.first] = -1;
		}
	} else {
		if (table[ID] != -1) {	// table is not empty
			int result = table[ID];
			// update customer_list, order_list, !min_heap, table, zone
			zone_1->remove(result);
			zone_2->remove(result);
			customer_list->removeNode(result);
			order_list->removeNode(result);
			table[ID] = -1;
		} else { // table is empty
			// do nothing
			return;
		}
	}


}

void printHT(HashTable *zone_1) {
	zone_1->print();
}

void printAVL(AVLTree *zone_2) {
	zone_2->print();
}

void printMH() {
	
}

void simulate(string filename)
{
	Linkedlist* customer_list = new Linkedlist();
	Linkedlist* order_list = new Linkedlist();
	HashTable* zone_1 = new HashTable();
	AVLTree* zone_2 = new AVLTree();

	map<int, int> table; // -1 is empty
	for (int i = 1; i <= MAXSIZE; i++) {
		table[i] = -1;
	}



	ifstream myfile(filename);
	string command;
	while (getline(myfile, command)) {
		string key = command.substr(0, command.find(" "));


		//
		if (key == "REG") {
			reg(command, customer_list, order_list, table, zone_1, zone_2);
		} else if (key == "CLE") {
			cle(command, customer_list, order_list, table, zone_1, zone_2);
		} else if (key == "PrintHT") {
			printHT(zone_1);
		} else if (key == "PrintAVL") {
			printAVL(zone_2);
		} else if (key == "PrintMH") {
			//
		} else {
			//
			return;
		}
	}


	delete customer_list;
	delete order_list;
	delete zone_1;
	delete zone_2;






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