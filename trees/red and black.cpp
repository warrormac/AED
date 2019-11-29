#include <cstddef>
#include <initializer_list>
#include <iostream>

using namespace std;
enum class Color {
	BLACK,
	RED
};

template <typename T>
struct Node {
	T value;
	Node* parent;
	Node* left;
	Node* right;
	Color color;

	Node();
	Node(const T& value, Color color);
	explicit Node(const Node<T>* other);
};

template <typename T>
Node<T>::Node() : parent(nullptr), left(nullptr), right(nullptr) {}

template <typename T>
Node<T>::Node(const T& value, Color color) : Node() {
	this->value = value;
	this->color = color;
}

template <typename T>
Node<T>::Node(const Node<T>* other)
	: value(other->value),
	parent(other->parent),
	left(other->left),
	right(other->right),
	color(other->color) {}

template <typename T>
class rbTree {
public:
	rbTree();
	rbTree(const rbTree& other);
	explicit rbTree(std::initializer_list<T> list);
	~rbTree();

	template <typename Iter>
	rbTree(Iter begin, Iter end) : rbTree() {
		while (begin != end) {
			T val = *begin;
			insert(val);
			++begin;
		}
	}

	class iterator {
	public:
		iterator();
		iterator(Node<T>* node, Node<T>* root);
		const T& operator*() const;
		const T* operator->() const;
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;

	private:
		Node<T>* current_;
		Node<T>* root_;
	};

	rbTree& operator=(rbTree& other);
	void insert(const T& key);
	void erase(const T& key);
	iterator find(const T& key) const;
	iterator lower_bound(const T& key) const;
	size_t size() const;
	bool empty() const;
	Node<T>* getRoot() const;
	iterator begin() const;
	iterator end() const;

private:
	Node<T>* root_;
	size_t size_;

	Node<T>* find_(const T& key) const;
	Node<T>* lower_bound_(const T& key) const;

	void erase_by_pointer_(Node<T>* node);
	void erase_no_children_(Node<T>* node);
	void erase_one_child_(Node<T>* node);
	void erase_two_children_(Node<T>* node);

	void replace_node_(Node<T>* node, Node<T>* child);
	void rotate_left_(Node<T>* grand);
	void rotate_right_(Node<T>* grand);
	void small_left_rotate_(Node<T>* node);
	void small_right_rotate_(Node<T>* node);

	void insert_1_case_(Node<T>* node);
	void insert_2_case_(Node<T>* node);
	void insert_3_case_(Node<T>* node);
	void insert_4_case_(Node<T>* node);
	void insert_5_case_(Node<T>* node);

	void delete_1_case_(Node<T>* node);
	void delete_2_case_(Node<T>* node);
	void delete_3_case_(Node<T>* node);
	void delete_4_case_(Node<T>* node);
	void delete_5_case_(Node<T>* node);
	void delete_6_case_(Node<T>* node);
};

// Some help functions for rbTree implementation

template <typename T>
bool equals(const T& first, const T& second) {
	return !(first < second) && !(second < first);
}

template <typename T>
void change_node(Node<T>* node, const Node<T>* target) {
	node->color = target->color;
	node->parent = target->parent;
	node->left = target->left;
	node->right = target->right;
	if (node->parent) {
		if (node->parent->left == target) {
			node->parent->left = node;
		}
		else {
			node->parent->right = node;
		}
	}
	if (node->left) {
		node->left->parent = node;
	}
	if (node->right) {
		node->right->parent = node;
	}
}

template <typename T>
void recursionDelete(Node<T>* node) {
	if (!node) return;
	recursionDelete(node->left);
	recursionDelete(node->right);
	delete node;
}

template <typename T>
Node<T>* getGrandparent(Node<T>* node) {
	if (node != nullptr && node->parent != nullptr) {
		return node->parent->parent;
	}
	return nullptr;
}

template <typename T>
Node<T>* getUncle(Node<T>* node) {
	auto grand = getGrandparent(node);
	if (grand == nullptr) {
		return nullptr;
	}
	if (node->parent == grand->left) {
		return grand->right;
	}
	else {
		return grand->left;
	}
}

template <typename T>
Node<T>* subling(Node<T>* node) {
	if (node->parent == nullptr) {
		return nullptr;
	}
	if (node->parent->left == node) {
		return node->parent->right;
	}
	return node->parent->left;
}

template <typename T>
Node<T>* previous(Node<T>* node) {
	if (!node) {
		return nullptr;
	}
	auto* prev = node;
	if (node->left != nullptr) {
		prev = node->left;
		while (prev->right != nullptr) {
			prev = prev->right;
		}
		return prev;
	}
	while (prev->parent != nullptr) {
		if (prev->parent->right == prev) {
			return prev->parent;
		}
		prev = prev->parent;
	}
	return nullptr;
}

template <typename T>
Node<T>* next(Node<T>* node) {
	if (!node) {
		return nullptr;
	}
	auto* next_node = node;
	if (node->right) {
		next_node = node->right;
		while (next_node->left) {
			next_node = next_node->left;
		}
		return next_node;
	}
	while (next_node->parent) {
		if (next_node->parent->left == next_node) {
			return next_node->parent;
		}
		next_node = next_node->parent;
	}
	return nullptr;
}

template <typename T>
Node<T>* begin_node(Node<T>* root) {
	Node<T>* node = root;
	while (node && node->left) {
		node = node->left;
	}
	return node;
}

template <typename T>
Node<T>* end_node(Node<T>* root) {
	Node<T>* node = root;
	while (node && node->right) {
		node = node->right;
	}
	return node;
}

template <typename T>
Node<T>* recursionClone(const Node<T>* root) {
	if (!root) {
		return nullptr;
	}
	Node<T>* left = nullptr;
	Node<T>* right = nullptr;
	Node<T>* node = new Node<T>(root->value, root->color);
	if (root->left) {
		left = recursionClone(root->left);
		left->parent = node;
	}
	if (root->right) {
		right = recursionClone(root->right);
		right->parent = node;
	}
	node->left = left;
	node->right = right;
	return node;
}

// Iterator implementation

template <typename T>
rbTree<T>::iterator::iterator() : current_(nullptr), root_(nullptr) {}

template <typename T>
rbTree<T>::iterator::iterator(Node<T>* node, Node<T>* root) : current_(node), root_(root) {}

template <typename T>
const T& rbTree<T>::iterator::operator*() const {
	return current_->value;
}

template <typename T>
const T* rbTree<T>::iterator::operator->() const {
	return &current_->value;
}

template <typename T>
typename rbTree<T>::iterator& rbTree<T>::iterator::operator++() {
	current_ = next(current_);
	return *this;
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::iterator::operator++(int ind) {
	rbTree<T>::iterator result(current_, root_);
	++(*this);
	return result;
}

template <typename T>
typename rbTree<T>::iterator& rbTree<T>::iterator::operator--() {
	if (!current_) {
		current_ = end_node(root_);
	}
	else {
		current_ = previous(current_);
	}
	return *this;
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::iterator::operator--(int ind) {
	rbTree<T>::iterator result(current_, root_);
	--(*this);
	return result;
}

template <typename T>
bool rbTree<T>::iterator::operator==(const rbTree<T>::iterator& other) const {
	return (current_ == other.current_);
}

template <typename T>
bool rbTree<T>::iterator::operator!=(const rbTree<T>::iterator& other) const {
	return !((*this) == other);
}

// rbTree implementation

template <typename T>
rbTree<T>::rbTree() : root_(nullptr), size_(0) {}

template <typename T>
rbTree<T>::rbTree(const rbTree<T>& other) : rbTree() {
	if (other.getRoot()) {
		root_ = recursionClone(other.getRoot());
		size_ = other.size();
	}
}

template <typename T>
rbTree<T>::rbTree(std::initializer_list<T> list) : rbTree(list.begin(), list.end()) {}

template <typename T>
rbTree<T>& rbTree<T>::operator=(rbTree<T>& other) {
	if (root_ != other.getRoot()) {
		recursionDelete(root_);
		root_ = recursionClone(other.getRoot());
		size_ = other.size();
	}
	return *this;
}

template <typename T>
void rbTree<T>::insert(const T& key) {
	if (!root_) {
		root_ = new Node<T>(key, Color::BLACK);
		++size_;
		return;
	}

	Node<T>* node = root_;
	Node<T>* parent = node;
	do {
		if (equals(key, node->value)) {
			return;
		}
		parent = node;
		if (key < node->value) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	} while (node != nullptr);

	node = new Node<T>(key, Color::RED);
	node->parent = parent;

	if (key < parent->value) {
		node->parent->left = node;
	}
	else {
		node->parent->right = node;
	}

	insert_1_case_(node);
	++size_;
}

template <typename T>
void rbTree<T>::erase(const T& key) {
	auto* node = find_(key);
	if (node) {
		erase_by_pointer_(node);
		--size_;
	}
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::find(const T& key) const {
	return rbTree<T>::iterator(find_(key), root_);
}

template <typename T>
Node<T>* rbTree<T>::find_(const T& key) const {
	auto* node = root_;
	while (node && !equals(key, node->value)) {
		if (key < node->value) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}
	return node;
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::lower_bound(const T& key) const {
	return rbTree<T>::iterator(lower_bound_(key), root_);
}

template <typename T>
Node<T>* rbTree<T>::lower_bound_(const T& key) const {
	auto* node = root_;
	Node<T>* answer = nullptr;
	while (node) {
		if (key < node->value) {
			if (answer) {
				if (node->value < answer->value || equals(node->value, answer->value)) {
					answer = node;
				}
			}
			else {
				answer = node;
			}
			node = node->left;
		}
		else if (node->value < key) {
			node = node->right;
		}
		else {  // euqals
			answer = node;
			break;
		}
	}
	return answer;
}

template <typename T>
size_t rbTree<T>::size() const {
	return size_;
}

template <typename T>
bool rbTree<T>::empty() const {
	return size_ == 0;
}

template <typename T>
Node<T>* rbTree<T>::getRoot() const {
	return root_;
}

template <typename T>
rbTree<T>::~rbTree() {
	recursionDelete(root_);
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::begin() const {
	return rbTree<T>::iterator(begin_node(root_), root_);
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::end() const {
	return rbTree<T>::iterator(nullptr, root_);
}

// Insert implementation

template <typename T>
void rbTree<T>::rotate_left_(Node<T>* grand) {
	auto* alter = grand->parent;
	auto* past = grand->right;
	auto* child = past->left;

	past->parent = alter;
	if (alter != nullptr) {
		if (alter->left == grand) {
			alter->left = past;
		}
		else {
			alter->right = past;
		}
	}
	grand->parent = past;
	past->left = grand;
	grand->right = child;
	if (child != nullptr) {
		child->parent = grand;
	}
}

template <typename T>
void rbTree<T>::rotate_right_(Node<T>* grand) {
	auto* alter = grand->parent;
	auto* past = grand->left;
	auto* child = past->right;

	past->parent = alter;
	grand->parent = past;
	past->right = grand;
	if (alter != nullptr) {
		if (alter->left == grand) {
			alter->left = past;
		}
		else {
			alter->right = past;
		}
	}
	grand->left = child;
	if (child != nullptr) {
		child->parent = grand;
	}
}

template <typename T>
void rbTree<T>::small_left_rotate_(Node<T>* node) {
	auto* parent = node->parent;
	auto* grand = getGrandparent(node);
	node->parent = grand;
	grand->left = node;
	parent->parent = node;
	parent->right = node->left;
	if (parent->right != nullptr) {
		parent->right->parent = parent;
	}
	node->left = parent;
}

template <typename T>
void rbTree<T>::small_right_rotate_(Node<T>* node) {
	auto* parent = node->parent;
	auto* grand = getGrandparent(node);
	node->parent = grand;
	grand->right = node;
	parent->parent = node;
	parent->left = node->right;
	if (parent->left != nullptr) {
		parent->left->parent = parent;
	}
	node->right = parent;
}

template <typename T>
void rbTree<T>::insert_1_case_(Node<T>* node) {
	if (node->parent == nullptr) {  // is root
		node->color = Color::BLACK;
		root_ = node;
	}
	else if (node->color == Color::RED) {
		insert_2_case_(node);  // not root and red
	}
}

template <typename T>
void rbTree<T>::insert_2_case_(Node<T>* node) {
	if (node->parent->color == Color::RED) {  // else all is ok
		insert_3_case_(node);
	}
}

template <typename T>
void rbTree<T>::insert_3_case_(Node<T>* node) {
	auto* grand = getGrandparent(node);             // always exists
	auto* uncle = getUncle(node);                   // can be null
	if (uncle != nullptr && uncle->color == Color::RED) {  // parent already RED
		node->parent->color = Color::BLACK;
		uncle->color = Color::BLACK;
		grand->color = Color::RED;
		insert_1_case_(grand);
	}
	else {
		insert_4_case_(node);
	}
}

template <typename T>
void rbTree<T>::insert_4_case_(Node<T>* node) {
	auto* grand = getGrandparent(node);
	if (node == node->parent->right && node->parent == grand->left) {
		small_left_rotate_(node);
		node = node->left;
	}
	else if (node == node->parent->left && node->parent == grand->right) {
		small_right_rotate_(node);
		node = node->right;
	}
	insert_5_case_(node);
}

template <typename T>
void rbTree<T>::insert_5_case_(Node<T>* node) {
	auto* grand = getGrandparent(node);
	node->parent->color = Color::BLACK;
	grand->color = Color::RED;
	if (node == node->parent->left && node->parent == grand->left) {
		rotate_right_(grand);
	}
	else {
		rotate_left_(grand);
	}
	insert_1_case_(node->parent);
}

// Erase implementation

template <typename T>
void rbTree<T>::erase_by_pointer_(Node<T>* node) {
	if (!node->left && !node->right) {  // no children
		erase_no_children_(node);
	}
	else if (node->left && node->right) {  // two children
		erase_two_children_(node);
	}
	else if (node->left || node->right) {  // one child
		erase_one_child_(node);
	}
	delete node;
}

template <typename T>
void rbTree<T>::erase_no_children_(Node<T>* node) {
	erase_one_child_(node);
	if (node->parent == nullptr) {  // this is last element and it's the root
		root_ = nullptr;
	}
	else if (node->parent->left == node) {
		node->parent->left = nullptr;
	}
	else {
		node->parent->right = nullptr;
	}
}

template <typename T>
void rbTree<T>::erase_two_children_(Node<T>* node) {
	auto* prev = previous(node);
	Node<T>* tmp = new Node<T>(prev);
	change_node(tmp, prev);
	change_node(prev, node);
	if (!prev->parent) {
		root_ = prev;
	}
	erase_by_pointer_(tmp);
}

template <typename T>
void rbTree<T>::erase_one_child_(Node<T>* node) {
	Node<T>* child = node->left ? node->left : node->right;
	if (child) {
		replace_node_(node, child);
		if (node->color == Color::BLACK) {
			if (child->color == Color::RED) {
				child->color = Color::BLACK;
			}
			else {
				delete_1_case_(child);
			}
		}
		if (!child->parent) {
			root_ = child;
		}
	}
	else {
		if (node->color == Color::BLACK) {
			delete_1_case_(node);
		}
	}
}

// Place child instead of node
template <typename T>
void rbTree<T>::replace_node_(Node<T>* node, Node<T>* child) {
	child->parent = node->parent;
	if (node->parent) {
		if (node == node->parent->left) {
			node->parent->left = child;
		}
		else {
			node->parent->right = child;
		}
	}
}

template <typename T>
void rbTree<T>::delete_1_case_(Node<T>* node) {
	// node and it's child are black
	if (node->parent) {
		delete_2_case_(node);
	}
	else {
		node->color = Color::BLACK;
		root_ = node;
	}
}

template <typename T>
void rbTree<T>::delete_2_case_(Node<T>* node) {
	// node's parent exists
	auto* sub = subling(node);  // sub isn't null
	if (sub->color == Color::RED) {
		node->parent->color = Color::RED;
		sub->color = Color::BLACK;
		if (node == node->parent->left) {
			rotate_left_(node->parent);
		}
		else {
			rotate_right_(node->parent);
		}
	}
	if (!sub->parent) {
		root_ = sub;
	}
	delete_3_case_(node);
}

template <typename T>
void rbTree<T>::delete_3_case_(Node<T>* node) {
	auto* sub = subling(node);
	if ((node->parent->color == Color::BLACK) && (sub->color == Color::BLACK) &&
		(!sub->left || sub->left->color == Color::BLACK) && (!sub->right || sub->right->color == Color::BLACK)) {
		sub->color = Color::RED;
		delete_1_case_(node->parent);
	}
	else {
		delete_4_case_(node);
	}
}

template <typename T>
void rbTree<T>::delete_4_case_(Node<T>* node) {
	auto* sub = subling(node);
	if ((node->parent->color == Color::RED) && (sub->color == Color::BLACK) &&
		(!sub->left || sub->left->color == Color::BLACK) && (!sub->right || sub->right->color == Color::BLACK)) {
		sub->color = Color::RED;
		node->parent->color = Color::BLACK;
	}
	else {
		delete_5_case_(node);
	}
}

template <typename T>
void rbTree<T>::delete_5_case_(Node<T>* node) {
	auto* sub = subling(node);
	if (sub->color == Color::BLACK) {
		if ((node == node->parent->left) && (!sub->right || sub->right->color == Color::BLACK) &&
			(sub->left->color == Color::RED)) {
			sub->color = Color::RED;
			sub->left->color = Color::BLACK;
			rotate_right_(sub);
		}
		else if ((node == node->parent->right) && (!sub->left || sub->left->color == Color::BLACK) &&
			(sub->right->color == Color::RED)) {
			sub->color = Color::RED;
			sub->right->color = Color::BLACK;
			rotate_left_(sub);
		}
	}
	delete_6_case_(node);
}

template <typename T>
void rbTree<T>::delete_6_case_(Node<T>* node) {
	auto* sub = subling(node);
	sub->color = node->parent->color;
	node->parent->color = Color::BLACK;
	if (node == node->parent->left) {
		sub->right->color = Color::BLACK;
		rotate_left_(node->parent);
	}
	else {
		sub->left->color = Color::BLACK;
		rotate_right_(node->parent);
	}

	if (!sub->parent) {
		root_ = sub;
	}
}

struct Trunk
{
	Trunk* prev;
	string str;

	Trunk(Trunk* prev, string str)
	{
		this->prev = prev;
		this->str = str;
	}
};

void showTrunks(Trunk* p)
{
	if (p == nullptr)
		return;

	showTrunks(p->prev);

	cout << p->str;
}

template <typename T>
void printTree(Node<T>* root, Trunk* prev, bool isLeft)
{
	if (root == nullptr)
		return;
	string color;
	string prev_str = "    ";
	Trunk* trunk = new Trunk(prev, prev_str);

	printTree(root->left, trunk, true);
	

	if (root->color == Color::BLACK)
	{
		color = " (B) ";
	}

	if (root->color == Color::RED)
	{
		color = " (R) ";
	}
	if (!prev)
		trunk->str = "---";
	else if (isLeft)
	{
		trunk->str = ".---";
		prev_str = "   |";
	}
	else
	{
		trunk->str = "`---";
		prev->str = prev_str;
	}

	showTrunks(trunk);
	cout << root->value << color<<endl;

	if (prev)
		prev->str = prev_str;
	trunk->str = "   |";

	printTree(root->right, trunk, false);
}


int main() {

	rbTree<int> test;
	int menu, n;
	while (true) {
		cout << "\t         MENU   \n";
		cout << "1. Insertar elementos en el arbol\n";
		cout << "2. Mostrar el arbol\n";
		//cout << "3. Eliminar un Nodo de la Lista\n";
		cout << "5. Salir\n";
		cout << "Opcion: ";
		cin >> menu;
		if (menu == 1) {
			std::cout << "ingrese un numero: " << std::endl;
			std::cin >> n;
			if (n == 0)
				break;
			test.insert(n);
			printTree(test.getRoot(), nullptr, false);

		}
		else if (menu == 2) {
			std::cout << "ingrese el numero que quiere borrar: " << std::endl;
			std::cin >> n;
			test.erase(n);
			printTree(test.getRoot(), nullptr, false);
		}
		else if (menu == 0)
			break;
	}
	return 0;
}


--------------------------------------------
	
	#include <cstddef>
#include <initializer_list>
#include <iostream>

using namespace std;
enum class Color {
	BLACK,
	RED
};

template <typename T>
struct Node {
	T value;
	Node* parent;
	Node* left;
	Node* right;
	Color color;

	Node();
	Node(const T& value, Color color);
	explicit Node(const Node<T>* other);
};

template <typename T>
Node<T>::Node() : parent(nullptr), left(nullptr), right(nullptr) {}

template <typename T>
Node<T>::Node(const T& value, Color color) : Node() {
	this->value = value;
	this->color = color;
}

template <typename T>
Node<T>::Node(const Node<T>* other)
	: value(other->value),
	parent(other->parent),
	left(other->left),
	right(other->right),
	color(other->color) {}

template <typename T>
class rbTree {
public:
	rbTree();
	rbTree(const rbTree& other);
	explicit rbTree(std::initializer_list<T> list);
	~rbTree();

	template <typename Iter>
	rbTree(Iter begin, Iter end) : rbTree() {
		while (begin != end) {
			T val = *begin;
			insert(val);
			++begin;
		}
	}

	class iterator {
	public:
		iterator();
		iterator(Node<T>* node, Node<T>* root);
		const T& operator*() const;
		const T* operator->() const;
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;

	private:
		Node<T>* current_;
		Node<T>* root_;
	};

	rbTree& operator=(rbTree& other);
	void insert(const T& key);
	void erase(const T& key);
	iterator find(const T& key) const;
	iterator lower_bound(const T& key) const;
	size_t size() const;
	bool empty() const;
	Node<T>* getRoot() const;
	iterator begin() const;
	iterator end() const;

private:
	Node<T>* root_;
	size_t size_;

	Node<T>* find_(const T& key) const;
	Node<T>* lower_bound_(const T& key) const;

	void erase_by_pointer_(Node<T>* node);
	void erase_no_children_(Node<T>* node);
	void erase_one_child_(Node<T>* node);
	void erase_two_children_(Node<T>* node);

	void replace_node_(Node<T>* node, Node<T>* child);
	void rotate_left_(Node<T>* grand);
	void rotate_right_(Node<T>* grand);
	void small_left_rotate_(Node<T>* node);
	void small_right_rotate_(Node<T>* node);

	void insert_1_case_(Node<T>* node);
	void insert_2_case_(Node<T>* node);
	void insert_3_case_(Node<T>* node);
	void insert_4_case_(Node<T>* node);
	void insert_5_case_(Node<T>* node);

	void delete_1_case_(Node<T>* node);
	void delete_2_case_(Node<T>* node);
	void delete_3_case_(Node<T>* node);
	void delete_4_case_(Node<T>* node);
	void delete_5_case_(Node<T>* node);
	void delete_6_case_(Node<T>* node);
};

// Some help functions for rbTree implementation

template <typename T>
bool equals(const T& first, const T& second) {
	return !(first < second) && !(second < first);
}

template <typename T>
void change_node(Node<T>* node, const Node<T>* target) {
	node->color = target->color;
	node->parent = target->parent;
	node->left = target->left;
	node->right = target->right;
	if (node->parent) {
		if (node->parent->left == target) {
			node->parent->left = node;
		}
		else {
			node->parent->right = node;
		}
	}
	if (node->left) {
		node->left->parent = node;
	}
	if (node->right) {
		node->right->parent = node;
	}
}

template <typename T>
void recursionDelete(Node<T>* node) {
	if (!node) return;
	recursionDelete(node->left);
	recursionDelete(node->right);
	delete node;
}

template <typename T>
Node<T>* getGrandparent(Node<T>* node) {
	if (node != nullptr && node->parent != nullptr) {
		return node->parent->parent;
	}
	return nullptr;
}

template <typename T>
Node<T>* getUncle(Node<T>* node) {
	auto grand = getGrandparent(node);
	if (grand == nullptr) {
		return nullptr;
	}
	if (node->parent == grand->left) {
		return grand->right;
	}
	else {
		return grand->left;
	}
}

template <typename T>
Node<T>* subling(Node<T>* node) {
	if (node->parent == nullptr) {
		return nullptr;
	}
	if (node->parent->left == node) {
		return node->parent->right;
	}
	return node->parent->left;
}

template <typename T>
Node<T>* previous(Node<T>* node) {
	if (!node) {
		return nullptr;
	}
	auto* prev = node;
	if (node->left != nullptr) {
		prev = node->left;
		while (prev->right != nullptr) {
			prev = prev->right;
		}
		return prev;
	}
	while (prev->parent != nullptr) {
		if (prev->parent->right == prev) {
			return prev->parent;
		}
		prev = prev->parent;
	}
	return nullptr;
}

template <typename T>
Node<T>* next(Node<T>* node) {
	if (!node) {
		return nullptr;
	}
	auto* next_node = node;
	if (node->right) {
		next_node = node->right;
		while (next_node->left) {
			next_node = next_node->left;
		}
		return next_node;
	}
	while (next_node->parent) {
		if (next_node->parent->left == next_node) {
			return next_node->parent;
		}
		next_node = next_node->parent;
	}
	return nullptr;
}

template <typename T>
Node<T>* begin_node(Node<T>* root) {
	Node<T>* node = root;
	while (node && node->left) {
		node = node->left;
	}
	return node;
}

template <typename T>
Node<T>* end_node(Node<T>* root) {
	Node<T>* node = root;
	while (node && node->right) {
		node = node->right;
	}
	return node;
}

template <typename T>
Node<T>* recursionClone(const Node<T>* root) {
	if (!root) {
		return nullptr;
	}
	Node<T>* left = nullptr;
	Node<T>* right = nullptr;
	Node<T>* node = new Node<T>(root->value, root->color);
	if (root->left) {
		left = recursionClone(root->left);
		left->parent = node;
	}
	if (root->right) {
		right = recursionClone(root->right);
		right->parent = node;
	}
	node->left = left;
	node->right = right;
	return node;
}

// Iterator implementation

template <typename T>
rbTree<T>::iterator::iterator() : current_(nullptr), root_(nullptr) {}

template <typename T>
rbTree<T>::iterator::iterator(Node<T>* node, Node<T>* root) : current_(node), root_(root) {}

template <typename T>
const T& rbTree<T>::iterator::operator*() const {
	return current_->value;
}

template <typename T>
const T* rbTree<T>::iterator::operator->() const {
	return &current_->value;
}

template <typename T>
typename rbTree<T>::iterator& rbTree<T>::iterator::operator++() {
	current_ = next(current_);
	return *this;
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::iterator::operator++(int ind) {
	rbTree<T>::iterator result(current_, root_);
	++(*this);
	return result;
}

template <typename T>
typename rbTree<T>::iterator& rbTree<T>::iterator::operator--() {
	if (!current_) {
		current_ = end_node(root_);
	}
	else {
		current_ = previous(current_);
	}
	return *this;
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::iterator::operator--(int ind) {
	rbTree<T>::iterator result(current_, root_);
	--(*this);
	return result;
}

template <typename T>
bool rbTree<T>::iterator::operator==(const rbTree<T>::iterator& other) const {
	return (current_ == other.current_);
}

template <typename T>
bool rbTree<T>::iterator::operator!=(const rbTree<T>::iterator& other) const {
	return !((*this) == other);
}

// rbTree implementation

template <typename T>
rbTree<T>::rbTree() : root_(nullptr), size_(0) {}

template <typename T>
rbTree<T>::rbTree(const rbTree<T>& other) : rbTree() {
	if (other.getRoot()) {
		root_ = recursionClone(other.getRoot());
		size_ = other.size();
	}
}

template <typename T>
rbTree<T>::rbTree(std::initializer_list<T> list) : rbTree(list.begin(), list.end()) {}

template <typename T>
rbTree<T>& rbTree<T>::operator=(rbTree<T>& other) {
	if (root_ != other.getRoot()) {
		recursionDelete(root_);
		root_ = recursionClone(other.getRoot());
		size_ = other.size();
	}
	return *this;
}

template <typename T>
void rbTree<T>::insert(const T& key) {
	if (!root_) {
		root_ = new Node<T>(key, Color::BLACK);
		++size_;
		return;
	}

	Node<T>* node = root_;
	Node<T>* parent = node;
	do {
		if (equals(key, node->value)) {
			return;
		}
		parent = node;
		if (key < node->value) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	} while (node != nullptr);

	node = new Node<T>(key, Color::RED);
	node->parent = parent;

	if (key < parent->value) {
		node->parent->left = node;
	}
	else {
		node->parent->right = node;
	}

	insert_1_case_(node);
	++size_;
}

template <typename T>
void rbTree<T>::erase(const T& key) {
	auto* node = find_(key);
	if (node) {
		erase_by_pointer_(node);
		--size_;
	}
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::find(const T& key) const {
	return rbTree<T>::iterator(find_(key), root_);
}

template <typename T>
Node<T>* rbTree<T>::find_(const T& key) const {
	auto* node = root_;
	while (node && !equals(key, node->value)) {
		if (key < node->value) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}
	return node;
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::lower_bound(const T& key) const {
	return rbTree<T>::iterator(lower_bound_(key), root_);
}

template <typename T>
Node<T>* rbTree<T>::lower_bound_(const T& key) const {
	auto* node = root_;
	Node<T>* answer = nullptr;
	while (node) {
		if (key < node->value) {
			if (answer) {
				if (node->value < answer->value || equals(node->value, answer->value)) {
					answer = node;
				}
			}
			else {
				answer = node;
			}
			node = node->left;
		}
		else if (node->value < key) {
			node = node->right;
		}
		else {  // euqals
			answer = node;
			break;
		}
	}
	return answer;
}

template <typename T>
size_t rbTree<T>::size() const {
	return size_;
}

template <typename T>
bool rbTree<T>::empty() const {
	return size_ == 0;
}

template <typename T>
Node<T>* rbTree<T>::getRoot() const {
	return root_;
}

template <typename T>
rbTree<T>::~rbTree() {
	recursionDelete(root_);
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::begin() const {
	return rbTree<T>::iterator(begin_node(root_), root_);
}

template <typename T>
typename rbTree<T>::iterator rbTree<T>::end() const {
	return rbTree<T>::iterator(nullptr, root_);
}

// Insert implementation

template <typename T>
void rbTree<T>::rotate_left_(Node<T>* grand) {
	auto* alter = grand->parent;
	auto* past = grand->right;
	auto* child = past->left;

	past->parent = alter;
	if (alter != nullptr) {
		if (alter->left == grand) {
			alter->left = past;
		}
		else {
			alter->right = past;
		}
	}
	grand->parent = past;
	past->left = grand;
	grand->right = child;
	if (child != nullptr) {
		child->parent = grand;
	}
}

template <typename T>
void rbTree<T>::rotate_right_(Node<T>* grand) {
	auto* alter = grand->parent;
	auto* past = grand->left;
	auto* child = past->right;

	past->parent = alter;
	grand->parent = past;
	past->right = grand;
	if (alter != nullptr) {
		if (alter->left == grand) {
			alter->left = past;
		}
		else {
			alter->right = past;
		}
	}
	grand->left = child;
	if (child != nullptr) {
		child->parent = grand;
	}
}

template <typename T>
void rbTree<T>::small_left_rotate_(Node<T>* node) {
	auto* parent = node->parent;
	auto* grand = getGrandparent(node);
	node->parent = grand;
	grand->left = node;
	parent->parent = node;
	parent->right = node->left;
	if (parent->right != nullptr) {
		parent->right->parent = parent;
	}
	node->left = parent;
}

template <typename T>
void rbTree<T>::small_right_rotate_(Node<T>* node) {
	auto* parent = node->parent;
	auto* grand = getGrandparent(node);
	node->parent = grand;
	grand->right = node;
	parent->parent = node;
	parent->left = node->right;
	if (parent->left != nullptr) {
		parent->left->parent = parent;
	}
	node->right = parent;
}

template <typename T>
void rbTree<T>::insert_1_case_(Node<T>* node) {
	if (node->parent == nullptr) {  // is root
		node->color = Color::BLACK;
		root_ = node;
	}
	else if (node->color == Color::RED) {
		insert_2_case_(node);  // not root and red
	}
}

template <typename T>
void rbTree<T>::insert_2_case_(Node<T>* node) {
	if (node->parent->color == Color::RED) {  // else all is ok
		insert_3_case_(node);
	}
}

template <typename T>
void rbTree<T>::insert_3_case_(Node<T>* node) {
	auto* grand = getGrandparent(node);             // always exists
	auto* uncle = getUncle(node);                   // can be null
	if (uncle != nullptr && uncle->color == Color::RED) {  // parent already RED
		node->parent->color = Color::BLACK;
		uncle->color = Color::BLACK;
		grand->color = Color::RED;
		insert_1_case_(grand);
	}
	else {
		insert_4_case_(node);
	}
}

template <typename T>
void rbTree<T>::insert_4_case_(Node<T>* node) {
	auto* grand = getGrandparent(node);
	if (node == node->parent->right && node->parent == grand->left) {
		small_left_rotate_(node);
		node = node->left;
	}
	else if (node == node->parent->left && node->parent == grand->right) {
		small_right_rotate_(node);
		node = node->right;
	}
	insert_5_case_(node);
}

template <typename T>
void rbTree<T>::insert_5_case_(Node<T>* node) {
	auto* grand = getGrandparent(node);
	node->parent->color = Color::BLACK;
	grand->color = Color::RED;
	if (node == node->parent->left && node->parent == grand->left) {
		rotate_right_(grand);
	}
	else {
		rotate_left_(grand);
	}
	insert_1_case_(node->parent);
}

// Erase implementation

template <typename T>
void rbTree<T>::erase_by_pointer_(Node<T>* node) {
	if (!node->left && !node->right) {  // no children
		erase_no_children_(node);
	}
	else if (node->left && node->right) {  // two children
		erase_two_children_(node);
	}
	else if (node->left || node->right) {  // one child
		erase_one_child_(node);
	}
	delete node;
}

template <typename T>
void rbTree<T>::erase_no_children_(Node<T>* node) {
	erase_one_child_(node);
	if (node->parent == nullptr) {  // this is last element and it's the root
		root_ = nullptr;
	}
	else if (node->parent->left == node) {
		node->parent->left = nullptr;
	}
	else {
		node->parent->right = nullptr;
	}
}

template <typename T>
void rbTree<T>::erase_two_children_(Node<T>* node) {
	auto* prev = previous(node);
	Node<T>* tmp = new Node<T>(prev);
	change_node(tmp, prev);
	change_node(prev, node);
	if (!prev->parent) {
		root_ = prev;
	}
	erase_by_pointer_(tmp);
}

template <typename T>
void rbTree<T>::erase_one_child_(Node<T>* node) {
	Node<T>* child = node->left ? node->left : node->right;
	if (child) {
		replace_node_(node, child);
		if (node->color == Color::BLACK) {
			if (child->color == Color::RED) {
				child->color = Color::BLACK;
			}
			else {
				delete_1_case_(child);
			}
		}
		if (!child->parent) {
			root_ = child;
		}
	}
	else {
		if (node->color == Color::BLACK) {
			delete_1_case_(node);
		}
	}
}

// Place child instead of node
template <typename T>
void rbTree<T>::replace_node_(Node<T>* node, Node<T>* child) {
	child->parent = node->parent;
	if (node->parent) {
		if (node == node->parent->left) {
			node->parent->left = child;
		}
		else {
			node->parent->right = child;
		}
	}
}

template <typename T>
void rbTree<T>::delete_1_case_(Node<T>* node) {
	// node and it's child are black
	if (node->parent) {
		delete_2_case_(node);
	}
	else {
		node->color = Color::BLACK;
		root_ = node;
	}
}

template <typename T>
void rbTree<T>::delete_2_case_(Node<T>* node) {
	// node's parent exists
	auto* sub = subling(node);  // sub isn't null
	if (sub->color == Color::RED) {
		node->parent->color = Color::RED;
		sub->color = Color::BLACK;
		if (node == node->parent->left) {
			rotate_left_(node->parent);
		}
		else {
			rotate_right_(node->parent);
		}
	}
	if (!sub->parent) {
		root_ = sub;
	}
	delete_3_case_(node);
}

template <typename T>
void rbTree<T>::delete_3_case_(Node<T>* node) {
	auto* sub = subling(node);
	if ((node->parent->color == Color::BLACK) && (sub->color == Color::BLACK) &&
		(!sub->left || sub->left->color == Color::BLACK) && (!sub->right || sub->right->color == Color::BLACK)) {
		sub->color = Color::RED;
		delete_1_case_(node->parent);
	}
	else {
		delete_4_case_(node);
	}
}

template <typename T>
void rbTree<T>::delete_4_case_(Node<T>* node) {
	auto* sub = subling(node);
	if ((node->parent->color == Color::RED) && (sub->color == Color::BLACK) &&
		(!sub->left || sub->left->color == Color::BLACK) && (!sub->right || sub->right->color == Color::BLACK)) {
		sub->color = Color::RED;
		node->parent->color = Color::BLACK;
	}
	else {
		delete_5_case_(node);
	}
}

template <typename T>
void rbTree<T>::delete_5_case_(Node<T>* node) {
	auto* sub = subling(node);
	if (sub->color == Color::BLACK) {
		if ((node == node->parent->left) && (!sub->right || sub->right->color == Color::BLACK) &&
			(sub->left->color == Color::RED)) {
			sub->color = Color::RED;
			sub->left->color = Color::BLACK;
			rotate_right_(sub);
		}
		else if ((node == node->parent->right) && (!sub->left || sub->left->color == Color::BLACK) &&
			(sub->right->color == Color::RED)) {
			sub->color = Color::RED;
			sub->right->color = Color::BLACK;
			rotate_left_(sub);
		}
	}
	delete_6_case_(node);
}

template <typename T>
void rbTree<T>::delete_6_case_(Node<T>* node) {
	auto* sub = subling(node);
	sub->color = node->parent->color;
	node->parent->color = Color::BLACK;
	if (node == node->parent->left) {
		sub->right->color = Color::BLACK;
		rotate_left_(node->parent);
	}
	else {
		sub->left->color = Color::BLACK;
		rotate_right_(node->parent);
	}

	if (!sub->parent) {
		root_ = sub;
	}
}

struct Trunk
{
	Trunk* prev;
	string str;

	Trunk(Trunk* prev, string str)
	{
		this->prev = prev;
		this->str = str;
	}
};

void showTrunks(Trunk* p)
{
	if (p == nullptr)
		return;

	showTrunks(p->prev);

	cout << p->str;
}

template <typename T>
void printTree(Node<T>* root, Trunk* prev, bool isLeft)
{
	if (root == nullptr)
		return;
	string color;
	string prev_str = "    ";
	Trunk* trunk = new Trunk(prev, prev_str);


	printTree(root->right, trunk, true);
	


	if (root->color == Color::BLACK)
	{
		color = " (B) ";
	}

	if (root->color == Color::RED)
	{
		color = " (R) ";
	}
	if (!prev)
		;// trunk->str = "---";
	else if (isLeft)
	{
		//prev_str = "   |";
		trunk->str = ".---";
		
	}
	else
	{
		trunk->str = "`---";
		//prev_str = "   |";
		prev->str = prev_str;
	}

	showTrunks(trunk);
	cout << root->value << color << endl;
	trunk->str = " ";
	if (prev)
		prev->str = prev_str;
	trunk->str = "   ";
	

	printTree(root->left, trunk, false);
	
}


int main() {

	rbTree<int> test;
	int menu, n;
	while (true) {
		cout << "\t         MENU   \n";
		cout << "1. Insertar elementos en el arbol\n";
		cout << "2. Eliminar un nodo del arbol\n";
		//cout << "3. Eliminar un Nodo de la Lista\n";
		cout << "5. Salir\n";
		cout << "Opcion: ";
		cin >> menu;
		if (menu == 1) {
			std::cout << "ingrese un numero: " << std::endl;
			std::cin >> n;
			if (n == 0)
				break;
			test.insert(n);
			printTree(test.getRoot(), nullptr, false);

		}
		else if (menu == 2) {
			std::cout << "ingrese el numero que quiere borrar: " << std::endl;
			std::cin >> n;
			test.erase(n);
			printTree(test.getRoot(), nullptr, false);
		}
		else if (menu == 0)
			break;
	}
	return 0;
}
