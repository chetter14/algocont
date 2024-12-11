#include <iostream>
#include <random>

namespace {
std::minstd_rand generator;
}

struct Node {
public:
  uint priority;
  int size = 1;  // number of children + 1 (itself)
  int value;     // value
  int64_t sum;   // sum of children values
  Node *left = nullptr, *right = nullptr;

  explicit Node(int value) : priority(generator()), value(value), sum(value) {
  }
};

class Treap {
private:
  Node* root_ = nullptr;

  static int GetSize(Node* n) {
    return n != nullptr ? n->size : 0;
  }

  static int64_t GetSum(Node* n) {
    return n != nullptr ? n->sum : 0;
  }

  static void Update(Node* n) {
    if (n != nullptr) {
      n->sum = GetSum(n->left) + n->value + GetSum(n->right);
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
      b->left = Merge(a, b->left);
      Update(b);
      return b;
    }
  }

  static void Split(Node* node, int k, Node*& a, Node*& b) {
    if (node == nullptr) {
      a = nullptr;
      b = nullptr;
      return;
    }

    if (GetSize(node->left) < k) {
      Split(node->right, k - GetSize(node->left) - 1, node->right, b);
      a = node;
    } else {
      Split(node->left, k, a, node->left);
      b = node;
    }
    Update(a);
    Update(b);
  }

  void CleanUp(Node* node) {
    if (node == nullptr) {
      return;
    }
    CleanUp(node->left);
    CleanUp(node->right);
    delete node;
  }

public:
  ~Treap() {
    CleanUp(root_);
  }

  void PushBack(int value) {
    root_ = Merge(root_, new Node(value));
  }

  int64_t Sum(int left, int right) {
    Node* less = nullptr;
    Node* greater = nullptr;
    Node* equal = nullptr;

    Split(root_, left, less, greater);
    Split(greater, right - left + 1, equal, greater);

    int64_t result = GetSum(equal);
    root_ = Merge(Merge(less, equal), greater);
    return result;
  }

  friend void SwapEvenOdd(int left_boundary, int right_boundary, Treap& even, Treap& odd);
};

void SwapEvenOdd(int left_boundary, int right_boundary, Treap& even, Treap& odd) {
  Node *even_left, *even_middle, *even_right;
  int even_left_index = left_boundary % 2 ? (left_boundary + 1) / 2 : left_boundary / 2;
  int even_right_index = right_boundary % 2 ? (right_boundary - 1) / 2 : right_boundary / 2;
  Treap::Split(even.root_, even_left_index, even_left, even_right);
  Treap::Split(even_right, even_right_index - even_left_index + 1, even_middle, even_right);

  Node *odd_left, *odd_middle, *odd_right;
  int odd_left_index = left_boundary % 2 ? (left_boundary - 1) / 2 : left_boundary / 2;
  int odd_right_index = right_boundary % 2 ? (right_boundary - 1) / 2 : right_boundary / 2 - 1;
  Treap::Split(odd.root_, odd_left_index, odd_left, odd_right);
  Treap::Split(odd_right, odd_right_index - odd_left_index + 1, odd_middle, odd_right);

  even.root_ = Treap::Merge(Treap::Merge(even_left, odd_middle), even_right);
  odd.root_ = Treap::Merge(Treap::Merge(odd_left, even_middle), odd_right);
}

int64_t Sum(int left_boundary, int right_boundary, Treap& even, Treap& odd) {
  int even_left_index = left_boundary % 2 ? (left_boundary + 1) / 2 : left_boundary / 2;
  int even_right_index = right_boundary % 2 ? (right_boundary - 1) / 2 : right_boundary / 2;

  int odd_left_index = left_boundary % 2 ? (left_boundary - 1) / 2 : left_boundary / 2;
  int odd_right_index = right_boundary % 2 ? (right_boundary - 1) / 2 : right_boundary / 2 - 1;

  return even.Sum(even_left_index, even_right_index) + odd.Sum(odd_left_index, odd_right_index);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  uint suite_number = 1;

  while (true) {
    uint size = 0;
    uint requests_number = 0;
    std::cin >> size >> requests_number;

    if (size == 0 && requests_number == 0) {  // two 0s in the end of input
      break;
    }

    Treap even_treap;
    Treap odd_treap;

    int temp = 0;
    for (uint i = 0; i < size; ++i) {
      std::cin >> temp;
      if (i % 2 == 0) {
        even_treap.PushBack(temp);
      } else {
        odd_treap.PushBack(temp);
      }
    }

    std::cout << "Suite " << suite_number << ":\n";
    ++suite_number;

    for (uint i = 0; i < requests_number; ++i) {
      int option = 0;
      int left_boundary = 0;
      int right_boundary = 0;
      std::cin >> option >> left_boundary >> right_boundary;

      // convert to 0-th base
      --left_boundary;
      --right_boundary;

      if (option == 1) {
        SwapEvenOdd(left_boundary, right_boundary, even_treap, odd_treap);
      } else if (option == 2) {
        std::cout << Sum(left_boundary, right_boundary, even_treap, odd_treap) << "\n";
      }
    }
    std::cout << "\n";
  }
  return 0;
}
