#include <cstddef>
#include <iterator>
#include <utility>

#ifndef DEQUE_RESEARCH_DYNAMICARRAY_H
#define DEQUE_RESEARCH_DYNAMICARRAY_H


template<typename T>
class DynamicArray {
private:
    T* data_;
    size_t capacity_;
    size_t size_;
    size_t head_;

    size_t physical_index(size_t logical_index) const {
        return (head_ + logical_index) % capacity_;
    }

    static size_t next_power_of_two(size_t n) {
        if (n == 0) return 1;

        --n;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        if constexpr (sizeof(size_t) == 8)
        n |= n >> 32;
        return n + 1;
    }

    void ensure_capacity() {
        if (size_ < capacity_)
            return;

        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
public:
    DynamicArray() : data_(nullptr), capacity_(0), size_(0), head_(0) {}

    ~DynamicArray() {
        clear();
        operator delete[](data_);
    }

    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    size_t size() const noexcept {
        return size_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    T& operator[](size_t index) {
        return data_[physical_index(index)];
    }

    const T& operator[](size_t index) const {
        return data_[physical_index(index)];
    }

    T& front() {
        return data_[head_];
    }

    T& back() {
        return data_[physical_index(size_ - 1)];
    }

    void push_front(const T& value) {
        ensure_capacity();
        head_ = (head_ + capacity_ - 1) % capacity_;
        new (&data_[head_]) T(value);
        ++size_;
    }

    void push_back(const T& value) {
        ensure_capacity();
        new (&data_[physical_index(size_)]) T(value);
        ++size_;
    }

    void pop_front() {
        data_[head_].~T();
        head_ = (head_ + 1) % capacity_;
        --size_;
    }

    void pop_back() {
        data_[physical_index(size_ - 1)].~T();
        --size_;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i)
            (*this)[i].~T();
        size_ = 0;
        head_ = 0;
    }

    void reserve(size_t n) {
        if (n <= capacity_)
            return;

        size_t new_capacity = next_power_of_two(n);
        T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

        for (size_t i = 0; i < size_; ++i) {
            new (&new_data[i]) T(std::move((*this)[i]));
            (*this)[i].~T();
        }

        operator delete[](data_);
        data_ = new_data;
        capacity_ = new_capacity;
        head_ = 0;
    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(DynamicArray* d, size_t pos) : deque_(d), pos_(pos) {}

        reference operator*() const {
            return (*deque_)[pos_];
        }

        pointer operator->() const {
            return &(**this);
        }

        iterator& operator++() {
            ++pos_;
            return *this;
        }

        iterator operator++(int) {
            auto t = *this;
            ++*this;
            return t;
        }

        iterator& operator--() {
            --pos_;
            return *this;
        }
        iterator operator--(int) {
            auto t = *this;
            --*this;
            return t;
        }

        iterator& operator+=(difference_type n) {
            pos_ += n;
            return *this;
        }

        iterator& operator-=(difference_type n) {
            pos_ -= n;
            return *this;
        }

        iterator operator+(difference_type n) const {
            return iterator(deque_, pos_ + n);
        }

        iterator operator-(difference_type n) const {
            return iterator(deque_, pos_ - n);
        }

        difference_type operator-(const iterator& other) const {
            return static_cast<difference_type>(pos_) -
                   static_cast<difference_type>(other.pos_);
        }

        bool operator==(const iterator& o) const {
            return pos_ == o.pos_;
        }
        bool operator!=(const iterator& o) const {
            return pos_ != o.pos_;
        }
        bool operator<(const iterator& o) const {
            return pos_ < o.pos_;
        }

    private:
        DynamicArray* deque_;
        size_t pos_;
    };

    iterator begin() { return iterator(this, 0); }
    iterator end()   { return iterator(this, size_); }
};


#endif //DEQUE_RESEARCH_DYNAMICARRAY_H
