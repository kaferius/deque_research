#include <cstddef>
#include <iterator>
#include <utility>

#ifndef DEQUE_RESEARCH_LIST_H
#define DEQUE_RESEARCH_LIST_H


template<typename T>
class List {
private:
    struct Node {
        Node* prev;
        Node *next;
        T value;

        Node(const T& v) : prev(nullptr), next(nullptr), value(v) {}
    };

    Node* head_;
    Node* tail_;
    size_t size_;
public:
    List() : head_(nullptr), tail_(nullptr), size_(0) {}

    ~List() {
        clear();
    }

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    size_t size() const noexcept {
        return size_;
    }
    bool empty() const noexcept {
        return size_ == 0;
    }

    T& operator[](size_t index) {
        Node* cur;

        if (index < size_ / 2) {
            cur = head_;
            for (size_t i = 0; i < index; ++i)
                cur = cur->next;
        } else {
            cur = tail_;
            for (size_t i = size_ - 1; i > index; --i)
                cur = cur->prev;
        }

        return cur->value;
    }

    const T& operator[](size_t index) const {
        const Node* cur;

        if (index < size_ / 2) {
            cur = head_;
            for (size_t i = 0; i < index; ++i)
                cur = cur->next;
        } else {
            cur = tail_;
            for (size_t i = size_ - 1; i > index; --i)
                cur = cur->prev;
        }

        return cur->value;
    }

    T& front() {
        return head_->value;
    }

    T& back() {
        return tail_->value;
    }

    void push_front(const T& value) {
        Node* n = new Node(value);
        n->next = head_;

        if (head_) {
            head_->prev = n;
        } else {
            tail_ = n;
        }

        head_ = n;
        ++size_;
    }

    void push_back(const T& value) {
        Node* n = new Node(value);
        n->prev = tail_;

        if (tail_) {
            tail_->next = n;
        } else {
            head_ = n;
        }

        tail_ = n;
        ++size_;
    }

    void pop_front() {
        Node* old = head_;
        head_ = head_->next;

        if (head_)
            head_->prev = nullptr;
        else
            tail_ = nullptr;

        delete old;
        --size_;
    }

    void pop_back() {
        Node* old = tail_;
        tail_ = tail_->prev;

        if (tail_)
            tail_->next = nullptr;
        else
            head_ = nullptr;

        delete old;
        --size_;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node* n) : node_(n) {}

        reference operator*() const {
            return node_->value;
        }
        pointer operator->() const {
            return &node_->value;
        }

        iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }

        iterator operator--(int) {
            auto tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const iterator& other) const {
            return node_ != other.node_;
        }
    private:
        Node* node_;
    };

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }
};


#endif //DEQUE_RESEARCH_LIST_H
