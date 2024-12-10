#include <iostream>
#include <random>

namespace {
std::minstd_rand generator;
}

struct Node {
public:
  uint key, priority;
  uint size = 1;
  uint64_t sum = key;
  Node *left = nullptr, *right = nullptr;

  explicit Node(uint key) : key(key), priority(generator()) {
  }
};

class Treap {
private:
  Node* root_ = nullptr;

  static uint GetSize(Node* n) {
    return n != nullptr ? n->size : 0;
  }

  static uint64_t GetSum(Node* n) {
    return n != nullptr ? n->sum : 0;
  }

  static void Update(Node*& n) {
    if (n != nullptr) {
      n->sum = GetSum(n->left) + n->key + GetSum(n->right);
      n->size = GetSize(n->left) + 1 + GetSize(n->right);
    }
  }

  static Node* Merge(Node* a, Node* b) {
    if (a == nullptr) {
      return b;
    }
    if (b == nullptr) {
      return a;
    }

    if (a->priority > b->priority) {
      a->right = Merge(a->right, b);
      Update(a);
      return a;
    } else {
      b->left = Merge(a, b->right);
      Update(b);
      return b;
    }
  }

  static void Split(Node* root, int key, Node*& a, Node*& b) {
    if (root == nullptr) {
      a = nullptr;
      b = nullptr;
      return;
    }

    if (root->key < key) {
      Split(root->right, key, root->right, b);
      a = root;
    } else {
      Split(root->left, key, a, root->left);
      b = root;
    }
    Update(a);
    Update(b);
  }

  uint Min(Node* n) const {
    while (n->left != nullptr) {
      n = n->left;
    }
    return n->key;
  }

  static uint KeyByIndex(Node* n, uint index) {
    uint left_size = GetSize(n->left);
    if (index == left_size) {
      return n->key;
    }
    if (index < left_size) {
      return KeyByIndex(n->left, index);
    } else {
      return KeyByIndex(n->right, index - left_size - 1);
    }
  }

public:
  bool Contains(uint key) {
    // Can be implemented via simple binary search !!!

    Node* less = nullptr;
    Node* greater = nullptr;
    Split(root_, key, less, greater);

    Node* equal = nullptr;
    Split(greater, key + 1, equal, greater);
    bool result = (equal != nullptr);

    less = Merge(less, equal);
    root_ = Merge(less, greater);
    return result;
  }

  void Insert(uint key) {
    Node* less = nullptr;
    Node* greater = nullptr;
    Split(root_, key, less, greater);

    less = Merge(less, new Node(key));
    root_ = Merge(less, greater);
  }

  void Erase(uint key) {
    Node* less = nullptr;
    Node* greater = nullptr;
    Split(root_, key, less, greater);

    Node* equal = nullptr;
    Split(greater, key + 1, equal, greater);

    root_ = Merge(less, greater);
  }

  uint Next(uint key) {
    Node* less = nullptr;
    Node* greater = nullptr;
    Split(root_, key + 1, less, greater);

    uint result = Min(greater);
    root_ = Merge(less, greater);
    return result;
  }

  uint GetIndexByKey(uint key) {
    Node* less = nullptr;
    Node* greater = nullptr;
    Split(root_, key, less, greater);

    uint result = GetSize(less);
    root_ = Merge(less, greater);
    return result;
  }

  uint GetKeyByIndex(uint index) {
    return KeyByIndex(root_, index);
  }

  uint64_t Sum(uint left, uint right) {
    Node* less = nullptr;
    Node* greater = nullptr;
    Node* equal = nullptr;
    Split(root_, left, less, greater);
    Split(greater, right + 1, equal, greater);

    uint64_t result = GetSum(equal);
    root_ = Merge(Merge(less, equal), greater);
    return result;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  return 0;
}
