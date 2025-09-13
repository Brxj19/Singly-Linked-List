#include <iostream>
#include <string>
#include <vector>
#include <cassert> // For basic assertions

// Include the header file for the linked list library
#include "SinglyLinkedList.h"

// A helper function to print the contents and state of a list
template <typename T>
void printList(const SinglyLinkedList<T>& list, const std::string& name) {
    std::cout << "--- List '" << name << "' ---" << std::endl;
    std::cout << "Size: " << list.size() << ", Empty: " << (list.empty() ? "Yes" : "No") << std::endl;
    
    if (!list.empty()) {
        std::cout << "Front: " << list.front() << ", Back: " << list.back() << std::endl;
    }
    
    std::cout << "Contents: [ ";
    // Use cbegin/cend to test const_iterator
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl << std::endl;
}

// A simple struct to test non-primitive types, emplacement, and move semantics.
// It prints messages to trace its lifecycle.
struct Person {
    std::string name;
    int id;

    Person(const std::string& n, int i) : name(n), id(i) {
        std::cout << "  [Person CONSTRUCTED: " << name << "]" << std::endl;
    }

    Person(const Person& other) : name(other.name), id(other.id) {
        std::cout << "  [Person COPIED: " << name << "]" << std::endl;
    }

    Person(Person&& other) noexcept : name(std::move(other.name)), id(other.id) {
        std::cout << "  [Person MOVED: " << name << "]" << std::endl;
    }

    ~Person() {
        std::cout << "  [Person DESTRUCTED: " << name << "]" << std::endl;
    }
    
    // So we can print Person objects
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << "{" << p.name << ", " << p.id << "}";
        return os;
    }
};


void testLifecycle() {
    std::cout << "\n========== 1. TESTING LIFECYCLE (CONSTRUCTORS, ASSIGNMENT) ==========\n" << std::endl;

    // Initializer list constructor
    SinglyLinkedList<int> list1 = {10, 20, 30};
    printList(list1, "list1 (from initializer_list)");

    // Copy constructor
    SinglyLinkedList<int> list2 = list1;
    printList(list2, "list2 (copy of list1)");

    // Copy assignment
    SinglyLinkedList<int> list3;
    list3 = list1;
    printList(list3, "list3 (assigned from list1)");

    // Modify original to prove it's a deep copy
    list1.push_back(40);
    std::cout << "*** After modifying list1 ***" << std::endl;
    printList(list1, "list1");
    printList(list2, "list2 (unaffected)");

    // Move constructor
    SinglyLinkedList<int> list4 = std::move(list1);
    printList(list4, "list4 (moved from list1)");
    printList(list1, "list1 (after move - should be empty)");

    // Move assignment
    SinglyLinkedList<int> list5;
    list5 = std::move(list2);
    printList(list5, "list5 (move-assigned from list2)");
    printList(list2, "list2 (after move - should be empty)");
    
    // Swap
    SinglyLinkedList<int> a = {1, 2};
    SinglyLinkedList<int> b = {9, 8, 7};
    std::cout << "*** Before swap ***" << std::endl;
    printList(a, "a");
    printList(b, "b");
    swap(a, b);
    std::cout << "*** After swap ***" << std::endl;
    printList(a, "a");
    printList(b, "b");
}

void testModifiersPushPop() {
    std::cout << "\n========== 2. TESTING MODIFIERS (PUSH, POP, CLEAR) ==========\n" << std::endl;
    
    SinglyLinkedList<int> list;
    printList(list, "Initial");

    std::cout << "--> push_front(10)" << std::endl;
    list.push_front(10);
    printList(list, "After push_front");

    std::cout << "--> push_back(30)" << std::endl;
    list.push_back(30);
    printList(list, "After push_back");
    
    std::cout << "--> push_front(5)" << std::endl;
    list.push_front(5);
    printList(list, "After push_front");

    std::cout << "--> push_back(40)" << std::endl;
    list.push_back(40);
    printList(list, "After push_back");

    std::cout << "--> pop_front()" << std::endl;
    list.pop_front();
    printList(list, "After pop_front");
    
    std::cout << "--> pop_back()" << std::endl;
    list.pop_back();
    printList(list, "After pop_back");

    std::cout << "--> clear()" << std::endl;
    list.clear();
    printList(list, "After clear");
}

void testAccessAndExceptions() {
    std::cout << "\n========== 3. TESTING ELEMENT ACCESS AND EXCEPTIONS ==========\n" << std::endl;

    SinglyLinkedList<std::string> list;
    list.push_back("Hello");
    list.push_back("World");
    printList(list, "string list");

    // Access and modify
    std::cout << "Modifying front()..." << std::endl;
    list.front() = "Hi";
    printList(list, "After modifying front");
    
    const SinglyLinkedList<std::string>& const_list = list;
    std::cout << "Accessing const front(): " << const_list.front() << std::endl << std::endl;

    // Test exceptions on empty list
    list.clear();
    printList(list, "Cleared list");

    try {
        std::cout << "Trying to access front() on empty list..." << std::endl;
        list.front();
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }

    try {
        std::cout << "Trying to pop_front() on empty list..." << std::endl;
        list.pop_front();
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    
    try {
        std::cout << "Trying to pop_back() on empty list..." << std::endl;
        list.pop_back();
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl << std::endl;
    }
}

void testInsertEraseReverse() {
    std::cout << "\n========== 4. TESTING INSERT, ERASE, REVERSE ==========\n" << std::endl;
    
    SinglyLinkedList<int> list = {10, 50};
    printList(list, "Initial");
    
    // Insert after the first element
    auto it = list.begin();
    std::cout << "--> insert_after(begin(), 20)" << std::endl;
    list.insert_after(it, 20);
    printList(list, "After insert in middle");
    
    // Insert after the new second element
    ++it;
    std::cout << "--> emplace_after(iterator_at_20, 30)" << std::endl;
    list.emplace_after(it, 30);
    printList(list, "After emplace");

    // Insert after the last element
    ++it; ++it; // now at 50
    std::cout << "--> insert_after(iterator_at_50, 60)" << std::endl;
    list.insert_after(it, 60);
    printList(list, "After insert at end");
    assert(list.back() == 60);

    // Erase after the first element
    it = list.begin();
    std::cout << "--> erase_after(begin()) (erasing 20)" << std::endl;
    list.erase_after(it);
    printList(list, "After erase");

    // Reverse
    std::cout << "--> reverse()" << std::endl;
    list.reverse();
    printList(list, "After reverse");
    assert(list.front() == 60 && list.back() == 10);
}

void testEmplacementAndMove() {
    std::cout << "\n========== 5. TESTING EMPLACEMENT AND MOVE SEMANTICS ==========\n" << std::endl;
    
    SinglyLinkedList<Person> people;
    
    std::cout << "--> emplace_front(\"Alice\", 101)" << std::endl;
    people.emplace_front("Alice", 101); // Should construct in-place
    printList(people, "people");
    
    std::cout << "--> emplace_back(\"Bob\", 102)" << std::endl;
    people.emplace_back("Bob", 102); // Should construct in-place
    printList(people, "people");
    
    std::cout << "\n--> push_front(Person(\"Charlie\", 103))" << std::endl;
    people.push_front(Person("Charlie", 103)); // Should construct then move
    printList(people, "people");
    
    std::cout << "\n--> Creating temporary Person object for push_back" << std::endl;
    Person diane("Diane", 104);
    std::cout << "--> push_back(std::move(diane))" << std::endl;
    people.push_back(std::move(diane)); // Should move
    printList(people, "people");
    
    std::cout << "\n--> Clearing the list. Destructors should be called for all:" << std::endl;
    people.clear();
    std::cout << std::endl;
}

void testComparisons() {
    std::cout << "\n========== 6. TESTING COMPARISON OPERATORS ==========\n" << std::endl;
    
    SinglyLinkedList<int> l1 = {1, 2, 3};
    SinglyLinkedList<int> l2 = {1, 2, 3};
    SinglyLinkedList<int> l3 = {1, 2, 4};
    SinglyLinkedList<int> l4 = {1, 2};

    printList(l1, "l1");
    printList(l2, "l2");
    printList(l3, "l3");
    printList(l4, "l4");

    std::cout << std::boolalpha; // Print true/false instead of 1/0
    std::cout << "l1 == l2: " << (l1 == l2) << " (Expected: true)" << std::endl;
    std::cout << "l1 == l3: " << (l1 == l3) << " (Expected: false)" << std::endl;
    std::cout << "l1 != l3: " << (l1 != l3) << " (Expected: true)" << std::endl;
    std::cout << "l1 < l3:  " << (l1 < l3) << " (Expected: true)" << std::endl;
    std::cout << "l3 > l1:  " << (l3 > l1) << " (Expected: true)" << std::endl;
    std::cout << "l4 < l1:  " << (l4 < l1) << " (Expected: true)" << std::endl;
    std::cout << "l1 >= l2: " << (l1 >= l2) << " (Expected: true)" << std::endl;
}


int main() {
    std::cout << "--- SINGLY LINKED LIST TEST SUITE ---" << std::endl;
    
    testLifecycle();
    testModifiersPushPop();
    testAccessAndExceptions();
    testInsertEraseReverse();
    testEmplacementAndMove();
    testComparisons();

    std::cout << "\n--- ALL TESTS COMPLETED ---" << std::endl;

    return 0;
}
