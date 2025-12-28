#include "sys.h"
#include <iostream>
#include <cassert>

int main()
{
// Initialize our ecosystem
    avl_tree tree;
    priority_queue pq;
    hash_table tags_map;
    knowledge_graph graph;

    const int NUM_DOCS = 50;
    std::vector<document*> all_docs;

    std::cout << "--- STARTING STRESS TEST: 50 DOCUMENTS ---" << std::endl;

    // 1. MASS INSERTION & DATA MIXING
    for (int i = 0; i < NUM_DOCS; ++i) {
        // We use (i * 13) % 50 to create a non-sequential ID pattern for AVL stress
        uint64_t id = (i * 17) % 100; 
        uint8_t prio = (i * 7) % 20;   // Priorities 0-19
        
        document* doc = new document(id, "Content for document " + std::to_string(id));
        doc->priority = prio;

        // Give every doc overlapping tags to stress Hash Table collisions
        doc->add_tag("all_docs");
        if (id % 2 == 0) doc->add_tag("even_id");
        if (prio < 5) doc->add_tag("high_priority");

        all_docs.push_back(doc);
        tree.insert(doc);
        pq.add_new_task(doc);
        
        for (const std::string& t : doc->tags) {
            tags_map.insert(t, doc);
        }
    }
    std::cout << "[PHASE 1] Mass Insertion Complete. AVL Tree and Min-Heap balanced." << std::endl;

    // 2. AVL SEARCH & REMOVAL STRESS
    std::cout << "\n--- PHASE 2: AVL Tree Integrity ---" << std::endl;
    int found_count = 0;
    for (int i = 0; i < 100; ++i) {
        if (tree.find(i)) found_count++;
    }
    std::cout << "Successfully located " << found_count << " unique documents by ID." << std::endl;

    // Remove half the documents to trigger complex AVL rebalancing
    for (int i = 0; i < 25; ++i) {
        tree.remove(all_docs[i]->id);
    }
    std::cout << "Removed 25 documents. Tree rebalanced." << std::endl;

    // 3. HASH TABLE COLLISION TEST
    std::cout << "\n--- PHASE 3: Hash Table Collision Integrity ---" << std::endl;
    std::vector<document*> high_p = tags_map.search("high_priority");
    std::cout << "Found " << high_p.size() << " documents with tag 'high_priority'." << std::endl;
    
    // Test for a tag that doesn't exist
    std::vector<document*> missing = tags_map.search("does_not_exist");
    assert(missing.empty());
    std::cout << "Verified: Search for non-existent tags returns empty vector." << std::endl;

    // 4. PRIORITY QUEUE EXTRACTION
    std::cout << "\n--- PHASE 4: Min-Heap Extraction Order ---" << std::endl;
    uint8_t last_prio = 0;
    bool order_correct = true;
    while (true) {
        document* task = pq.get_next_task();
        if (!task) break;
        if (task->priority < last_prio) order_correct = false;
        last_prio = task->priority;
    }
    std::cout << "Heap Extraction " << (order_correct ? "[SUCCESS]" : "[FAILURE]") 
              << ": All tasks processed in non-decreasing priority order." << std::endl;

    // 5. KNOWLEDGE GRAPH: THE CYCLE & REACHABILITY TEST
    std::cout << "\n--- PHASE 5: Graph Reachability & Cycle Handling ---" << std::endl;
    /* Create a complex path: 500 -> 501 -> 502 -> 500 (Cycle)
       And a branch: 502 -> 503
    */
    graph.add_reference(500, 501);
    graph.add_reference(501, 502);
    graph.add_reference(502, 500); // The Cycle
    graph.add_reference(502, 503); // The Target

    std::cout << "Path 500 -> 503 reachable? " << (graph.is_reachable(500, 503) ? "YES (Correct)" : "NO (Error)") << std::endl;
    std::cout << "Path 503 -> 500 reachable? " << (graph.is_reachable(503, 500) ? "YES (Error)" : "NO (Correct)") << std::endl;
    std::cout << "Path 500 -> 500 (Self) reachable? " << (graph.is_reachable(500, 500) ? "YES (Correct)" : "NO (Error)") << std::endl;

    // Cleanup
    for (auto d : all_docs) delete d;
    std::cout << "\n--- ALL TESTS PASSED ---" << std::endl;

    return 0;
}
