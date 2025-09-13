#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

// Required C++17 for std::in_place_t, std::in_place
// Compile with: g++ -std=c++17 <your_main_file>.cpp

#include <memory>       // For std::unique_ptr, std::make_unique
#include <stdexcept>    // For std::out_of_range, std::invalid_argument
#include <cstddef>      // For std::size_t, std::ptrdiff_t
#include <iterator>     // For iterator tags and traits
#include <utility>      // For std::move, std::forward, std::in_place, std::in_place_t
#include <initializer_list> // For std::initializer_list constructor
#include <algorithm>    // For std::equal, std::lexicographical_compare

/**
 * @brief A modern C++ implementation of a singly linked list container.
 * * Manages a sequence of elements, storing them in non-contiguous memory.
 * Provides functionality similar to std::forward_list, with automatic memory
 * management via std::unique_ptr and modern C++ features like move semantics
 * and emplacement.
 * * @tparam T The type of the elements.
 */
template <typename T>
class SinglyLinkedList
{
private:
    /**
     * @brief Internal node structure for the linked list.
     * * Each node contains the element data and a smart pointer to the next node,
     * ensuring automatic memory cleanup.
     */
    struct Node
    {
        T data;
        std::unique_ptr<Node> next;

        // Constructor for copying a value
        explicit Node(const T &value) : data(value), next(nullptr) {}
        // Constructor for moving a value
        explicit Node(T &&value) : data(std::move(value)), next(nullptr) {}

        // Emplace constructor for constructing the element in-place
        template <typename... Args>
        explicit Node(std::in_place_t, Args&&... args)
            : data(std::forward<Args>(args)...), next(nullptr) {}
    };

    std::unique_ptr<Node> head_; // Smart pointer to the first node
    Node *tail_;                 // Raw pointer to the last node for O(1) push_back
    std::size_t list_size;       // Cached size of the list

public:
    // Forward declarations for iterator classes
    class iterator;
    class const_iterator;

    // --- ITERATOR CLASSES ---

    /**
     * @brief A forward iterator for mutable access to list elements.
     */
    class iterator
    {
        Node *ptr_;
        
        // Allow the main list class and const_iterator to access ptr_
        friend class SinglyLinkedList<T>;
        friend class const_iterator;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        explicit iterator(Node *p = nullptr) : ptr_(p) {}
        reference operator*() const { return ptr_->data; }
        pointer operator->() const { return &(ptr_->data); }
        iterator &operator++() { ptr_ = ptr_->next.get(); return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator &other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator &other) const { return ptr_ != other.ptr_; }
    };

    /**
     * @brief A forward iterator for read-only access to list elements.
     */
    class const_iterator
    {
        const Node *ptr_;
        friend class SinglyLinkedList<T>;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;

        explicit const_iterator(const Node *p = nullptr) : ptr_(p) {}
        // Implicit conversion from non-const iterator to const_iterator
        const_iterator(const iterator &it) : ptr_(it.ptr_) {}

        reference operator*() const { return ptr_->data; }
        pointer operator->() const { return &(ptr_->data); }
        const_iterator &operator++() { ptr_ = ptr_->next.get(); return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        
        bool operator==(const const_iterator &other) const { return ptr_ == other.ptr_; }
        bool operator!=(const const_iterator &other) const { return ptr_ != other.ptr_; }
        bool operator==(const iterator &other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator &other) const { return ptr_ != other.ptr_; }
    };

    // --- LIFECYCLE (RULE OF FIVE/SIX) ---

    /// @brief Default constructor. Creates an empty list.
    SinglyLinkedList() noexcept : head_(nullptr), tail_(nullptr), list_size(0) {}
    
    /// @brief Destructor. Cleans up all nodes automatically.
    ~SinglyLinkedList() = default;
    
    /**
     * @brief Copy constructor. Creates a deep copy of another list.
     * @param other The list to copy from.
     */
    SinglyLinkedList(const SinglyLinkedList &other) : SinglyLinkedList() {
        for (const auto &val : other)
            push_back(val);
    }
    
    /**
     * @brief Copy assignment operator (copy-and-swap idiom).
     * @param other The list to assign from.
     */
    SinglyLinkedList &operator=(SinglyLinkedList other) noexcept {
        swap(*this, other);
        return *this;
    }
    
    /**
     * @brief Move constructor. Takes ownership of another list's resources.
     * @param other The list to move from (will be empty after move).
     */
    SinglyLinkedList(SinglyLinkedList &&other) noexcept 
        : head_(std::move(other.head_)), tail_(other.tail_), list_size(other.list_size) {
        other.tail_ = nullptr;
        other.list_size = 0;
    }

    /**
     * @brief Constructs the list from an initializer list.
     * @param ilist The initializer list (e.g., {1, 2, 3}).
     */
    SinglyLinkedList(std::initializer_list<T> ilist) : SinglyLinkedList() {
        for (const auto &value : ilist) {
            push_back(value);
        }
    }

    /**
     * @brief Swaps the contents of two lists.
     * @param a The first list.
     * @param b The second list.
     */
    friend void swap(SinglyLinkedList &a, SinglyLinkedList &b) noexcept {
        using std::swap;
        swap(a.head_, b.head_);
        swap(a.tail_, b.tail_);
        swap(a.list_size, b.list_size);
    }

    // --- CAPACITY ---

    /// @brief Returns the number of elements in the list. O(1).
    std::size_t size() const noexcept { return list_size; }
    
    /// @brief Checks if the list is empty. O(1).
    bool empty() const noexcept { return list_size == 0; }

    // --- MODIFIERS ---

    /// @brief Removes all elements from the list. O(N).
    void clear() noexcept {
        head_.reset(); // unique_ptr's destructor handles chained deletion
        tail_ = nullptr;
        list_size = 0;
    }
    
    /**
     * @brief Inserts an element at the beginning of the list (copy). O(1).
     * @param value The value to insert.
     */
    void push_front(const T &value) {
        auto node = std::make_unique<Node>(value);
        if (!head_) tail_ = node.get();
        else node->next = std::move(head_);
        head_ = std::move(node);
        ++list_size;
    }

    /**
     * @brief Inserts an element at the beginning of the list (move). O(1).
     * @param value The rvalue to move from.
     */
    void push_front(T &&value) {
        auto node = std::make_unique<Node>(std::move(value));
        if (!head_) tail_ = node.get();
        else node->next = std::move(head_);
        head_ = std::move(node);
        ++list_size;
    }

    /**
     * @brief Constructs an element in-place at the beginning of the list. O(1).
     * @tparam Args Argument types for the element's constructor.
     * @param args Arguments to forward to the element's constructor.
     */
    template <typename... Args>
    void emplace_front(Args&&... args) {
        auto node = std::make_unique<Node>(std::in_place, std::forward<Args>(args)...);
        if (!head_) tail_ = node.get();
        else node->next = std::move(head_);
        head_ = std::move(node);
        ++list_size;
    }

    /**
     * @brief Appends an element to the end of the list (copy). O(1).
     * @param value The value to append.
     */
    void push_back(const T &value) {
        auto node = std::make_unique<Node>(value);
        Node *raw = node.get();
        if (!head_) head_ = std::move(node);
        else tail_->next = std::move(node);
        tail_ = raw;
        ++list_size;
    }

    /**
     * @brief Appends an element to the end of the list (move). O(1).
     * @param value The rvalue to move from.
     */
    void push_back(T &&value) {
        auto node = std::make_unique<Node>(std::move(value));
        Node* raw = node.get();
        if (!head_) head_ = std::move(node);
        else tail_->next = std::move(node);
        tail_ = raw;
        ++list_size;
    }

    /**
     * @brief Constructs an element in-place at the end of the list. O(1).
     * @tparam Args Argument types for the element's constructor.
     * @param args Arguments to forward to the element's constructor.
     */
    template <typename... Args>
    void emplace_back(Args&&... args) {
        auto node = std::make_unique<Node>(std::in_place, std::forward<Args>(args)...);
        Node* raw = node.get();
        if (!head_) head_ = std::move(node);
        else tail_->next = std::move(node);
        tail_ = raw;
        ++list_size;
    }

    /// @brief Removes the first element of the list. O(1).
    void pop_front() {
        if (!head_) throw std::out_of_range("pop_front on an empty list");
        head_ = std::move(head_->next);
        if (!head_) tail_ = nullptr;
        --list_size;
    }

    /// @brief Removes the last element of the list. O(N).
    void pop_back() {
        if (!head_) throw std::out_of_range("pop_back on an empty list");
        if (head_.get() == tail_) {
            pop_front();
        } else {
            Node *current = head_.get();
            while (current->next.get() != tail_) current = current->next.get();
            current->next.reset();
            tail_ = current;
            --list_size;
        }
    }

    /**
     * @brief Inserts an element after the given position. O(1).
     * @param pos An iterator to the element after which to insert.
     * @param value The value to insert.
     * @return An iterator to the newly inserted element.
     */
    iterator insert_after(const_iterator pos, const T &value) {
        Node *current = const_cast<Node*>(pos.ptr_);
        if (!current) throw std::invalid_argument("Cannot insert_after a null iterator");
        auto node = std::make_unique<Node>(value);
        Node *raw = node.get();
        if (tail_ == current) tail_ = raw;
        node->next = std::move(current->next);
        current->next = std::move(node);
        ++list_size;
        return iterator(raw);
    }
    
    iterator insert_after(const_iterator pos, T &&value) {
        Node *current = const_cast<Node*>(pos.ptr_);
        if (!current) throw std::invalid_argument("Cannot insert_after a null iterator");
        auto node = std::make_unique<Node>(std::move(value));
        Node *raw = node.get();
        if (tail_ == current) tail_ = raw;
        node->next = std::move(current->next);
        current->next = std::move(node);
        ++list_size;
        return iterator(raw);
    }

    /**
     * @brief Constructs an element in-place after the given position. O(1).
     * @param pos An iterator to the element after which to emplace.
     * @param args Arguments to forward to the element's constructor.
     * @return An iterator to the newly emplaced element.
     */
    template <typename... Args>
    iterator emplace_after(const_iterator pos, Args&&... args) {
        Node *current = const_cast<Node*>(pos.ptr_);
        if (!current) throw std::invalid_argument("Cannot emplace_after a null iterator");
        auto node = std::make_unique<Node>(std::in_place, std::forward<Args>(args)...);
        Node *raw = node.get();
        if (tail_ == current) tail_ = raw;
        node->next = std::move(current->next);
        current->next = std::move(node);
        ++list_size;
        return iterator(raw);
    }

    /**
     * @brief Erases the element after the given position. O(1).
     * @param pos An iterator to the element before the one to erase.
     * @return An iterator to the element that followed the erased element.
     */
    iterator erase_after(const_iterator pos) {
        Node *current = const_cast<Node*>(pos.ptr_);
        if (!current || !current->next) throw std::out_of_range("Cannot erase_after: no next element");
        auto to_delete = std::move(current->next);
        Node *next_node = to_delete->next.get();
        if (tail_ == to_delete.get()) tail_ = current;
        current->next = std::move(to_delete->next);
        --list_size;
        return iterator(next_node);
    }

    /// @brief Reverses the order of the elements in the list. O(N).
    void reverse() noexcept {
        if (list_size < 2) return;
        Node *prev = nullptr;
        std::unique_ptr<Node> current = std::move(head_);
        tail_ = current.get();
        while (current) {
            std::unique_ptr<Node> next = std::move(current->next);
            current->next.reset(prev);
            prev = current.release();
            current = std::move(next);
        }
        head_.reset(prev);
    }

    // --- ELEMENT ACCESS ---

    /// @brief Accesses the first element. Throws if the list is empty. O(1).
    T &front() {
        if (!head_) throw std::out_of_range("Accessing front() on an empty list");
        return head_->data;
    }

    /// @brief Accesses the first element (const version). Throws if empty. O(1).
    const T &front() const {
        if (!head_) throw std::out_of_range("Accessing front() on an empty list");
        return head_->data;
    }

    /// @brief Accesses the last element. Throws if the list is empty. O(1).
    T &back() {
        if (!tail_) throw std::out_of_range("Accessing back() on an empty list");
        return tail_->data;
    }

    /// @brief Accesses the last element (const version). Throws if empty. O(1).
    const T &back() const {
        if (!tail_) throw std::out_of_range("Accessing back() on an empty list");
        return tail_->data;
    }

    // --- ITERATORS ---

    /// @brief Returns an iterator to the beginning of the list.
    iterator begin() { return iterator(head_.get()); }
    /// @brief Returns a const_iterator to the beginning of the list.
    const_iterator begin() const { return const_iterator(head_.get()); }
    /// @brief Returns a const_iterator to the beginning of the list.
    const_iterator cbegin() const { return const_iterator(head_.get()); }
    
    /// @brief Returns an iterator to the end of the list (past-the-end element).
    iterator end() { return iterator(nullptr); }
    /// @brief Returns a const_iterator to the end of the list.
    const_iterator end() const { return const_iterator(nullptr); }
    /// @brief Returns a const_iterator to the end of the list.
    const_iterator cend() const { return const_iterator(nullptr); }
};

// --- NON-MEMBER FUNCTIONS ---

/// @brief Checks if two lists are equal.
template <typename T>
bool operator==(const SinglyLinkedList<T> &lhs, const SinglyLinkedList<T> &rhs) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

/// @brief Checks if two lists are not equal.
template <typename T>
bool operator!=(const SinglyLinkedList<T> &lhs, const SinglyLinkedList<T> &rhs) {
    return !(lhs == rhs);
}

/// @brief Lexicographically compares two lists.
template <typename T>
bool operator<(const SinglyLinkedList<T> &lhs, const SinglyLinkedList<T> &rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename T>
bool operator<=(const SinglyLinkedList<T> &lhs, const SinglyLinkedList<T> &rhs) { return !(rhs < lhs); }
template <typename T>
bool operator>(const SinglyLinkedList<T> &lhs, const SinglyLinkedList<T> &rhs) { return rhs < lhs; }
template <typename T>
bool operator>=(const SinglyLinkedList<T> &lhs, const SinglyLinkedList<T> &rhs) { return !(lhs < rhs); }


#endif // SINGLY_LINKED_LIST_H
