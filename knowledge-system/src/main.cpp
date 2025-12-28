#include "sys.h"
#include <iostream>

int main()
{
    // 1. Setup Test Documents
    // We'll create them with IDs out of order and Priorities out of order
    document d0(100, "Routine Checkup");
    d0.priority = 10;
    document d1(50, "EMERGENCY");
    d1.priority = 1;
    document d2(150, "Urgent Update");
    d2.priority = 3;
    document d3(75, "Standard Note");
    d3.priority = 5;

    avl_tree tree;
    priority_queue pq;

    // 2. Test Insertion
    std::cout << "--- Testing Insertion ---" << std::endl;
    std::vector<document*> docs = {&d0, &d1, &d2, &d3};
    for(auto doc: docs) {
        tree.insert(doc);
        pq.add_new_task(doc);
    }
    std::cout << "Added 4 documents to AVL and Priority Queue." << std::endl;

    // 3. Test AVL Search (By ID)
    std::cout << "\n--- Testing AVL Search (By ID) ---" << std::endl;
    uint64_t search_id = 75;
    document* found = tree.find(search_id);
    if(found) {
        std::cout << "Found ID " << search_id << ": " << found->content
                  << std::endl;
    }
    else {
        std::cout << "ID " << search_id << " not found!" << std::endl;
    }

    // 4. Test Priority Queue (Min-Heap Property)
    // They should come out in order: 1, 3, 5, 10
    std::cout << "\n--- Testing Priority Queue (Extracting Tasks) ---"
              << std::endl;
    while(true) {
        document* task = pq.get_next_task();
        if(!task) break;
        std::cout << "Processing [Priority " << (int)task->priority << "] ID "
                  << task->id << ": " << task->content << std::endl;
    }

    // 5. Test AVL Removal
    std::cout << "\n--- Testing AVL Removal ---" << std::endl;
    document* removed = tree.remove(100);
    if(removed) {
        std::cout << "Removed ID 100. Verification find(100): "
                  << (tree.find(100) ? "STILL EXISTS (Fail)"
                                     : "NOT FOUND (Success)")
                  << std::endl;
    }

    return 0;
}
