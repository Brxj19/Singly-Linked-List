# SinglyLinkedList API Reference

This document provides a detailed API reference for the `SinglyLinkedList` class defined in `SinglyLinkedList.h`.

## `Documentation`
Online Documentation of [Singly Linked List ](https://brxj19.github.io/Singly-Linked-List/)

## `SinglyLinkedList<T>`

A container that manages a sequence of elements, storing them in non-contiguous memory. It provides functionality similar to `std::forward_list`, with automatic memory management via `std::unique_ptr` and modern C++ features like move semantics and emplacement.

### Template Parameters

-   `T`: The type of the elements.

---

### Member Functions

#### Lifecycle

| Function                                             | Description                                                              | Complexity |
| ---------------------------------------------------- | ------------------------------------------------------------------------ | ---------- |
| `SinglyLinkedList()`                                 | Default constructor. Creates an empty list.                              | O(1)       |
| `~SinglyLinkedList()`                                | Destructor. Cleans up all nodes.                                         | O(N)       |
| `SinglyLinkedList(const SinglyLinkedList& other)`    | Copy constructor. Creates a deep copy of another list.                   | O(N)       |
| `SinglyLinkedList(SinglyLinkedList&& other)`         | Move constructor. Takes ownership of another list's resources.           | O(1)       |
| `SinglyLinkedList(std::initializer_list<T> ilist)`   | Constructs the list from an initializer list.                            | O(N)       |
| `operator=(SinglyLinkedList other)`                  | Copy/Move assignment operator (copy-and-swap idiom).                     | O(N)       |
| `swap(SinglyLinkedList& other)`                      | Swaps the contents with another list.                                    | O(1)       |

#### Capacity

| Function                 | Description                                  | Complexity |
| ------------------------ | -------------------------------------------- | ---------- |
| `size() const`           | Returns the number of elements in the list.  | O(1)       |
| `empty() const`          | Checks if the list is empty.                 | O(1)       |

#### Element Access

| Function               | Description                                                        | Complexity |
| ---------------------- | ------------------------------------------------------------------ | ---------- |
| `front()` / `front() const` | Accesses the first element. Throws `std::out_of_range` if empty. | O(1)       |
| `back()` / `back() const`   | Accesses the last element. Throws `std::out_of_range` if empty.  | O(1)       |

#### Modifiers

| Function                                                              | Description                                                                                             | Complexity |
| --------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ---------- |
| `clear()`                                                             | Removes all elements from the list.                                                                     | O(N)       |
| `push_front(const T& value)` / `push_front(T&& value)`                | Inserts an element at the beginning of the list.                                                        | O(1)       |
| `emplace_front(Args&&... args)`                                       | Constructs an element in-place at the beginning of the list.                                            | O(1)       |
| `push_back(const T& value)` / `push_back(T&& value)`                  | Appends an element to the end of the list.                                                              | O(1)       |
| `emplace_back(Args&&... args)`                                        | Constructs an element in-place at the end of the list.                                                  | O(1)       |
| `pop_front()`                                                         | Removes the first element. Throws `std::out_of_range` if empty.                                         | O(1)       |
| `pop_back()`                                                          | Removes the last element. Throws `std::out_of_range` if empty.                                          | O(N)       |
| `insert_after(const_iterator pos, const T& value)` / `(..., T&& value)` | Inserts an element after the given position. Returns an iterator to the new element.                    | O(1)       |
| `emplace_after(const_iterator pos, Args&&... args)`                   | Constructs an element in-place after the given position. Returns an iterator to the new element.        | O(1)       |
| `erase_after(const_iterator pos)`                                     | Erases the element after the given position. Returns an iterator to the element following the erased one. | O(1)       |
| `reverse()`                                                           | Reverses the order of the elements in the list.                                                         | O(N)       |

#### Iterators

| Function                      | Description                                                              |
| ----------------------------- | ------------------------------------------------------------------------ |
| `begin()` / `begin() const`   | Returns an iterator to the beginning of the list.                        |
| `cbegin() const`              | Returns a `const_iterator` to the beginning of the list.                 |
| `end()` / `end() const`       | Returns an iterator to the end of the list (past-the-end element).       |
| `cend() const`                | Returns a `const_iterator` to the end of the list.                       |

---

### Non-Member Functions

#### Comparison Operators

These operators perform comparisons between two `SinglyLinkedList` objects.

| Operator | Description                                                                                             |
| -------- | ------------------------------------------------------------------------------------------------------- |
| `==`     | Checks if two lists are equal (same size and all elements compare equal).                               |
| `!=`     | Checks if two lists are not equal.                                                                      |
| `<`      | Compares two lists lexicographically.                                                                   |
| `<=`     | Lexicographically compares two lists.                                                                   |
| `>`      | Lexicographically compares two lists.                                                                   |
| `>=`     | Lexicographically compares two lists.                                                                   |
