#include "sys.h"
#include <iostream>

int main() {

    document test_doc0(0, "test0");
    document test_doc1(1, "test1");
    document test_doc2(2, "test2");
    document test_doc3(3, "test3");

    avl_tree test_tree;
    test_tree.insert(&test_doc0);
    test_tree.insert(&test_doc1);
    test_tree.insert(&test_doc2);
    test_tree.insert(&test_doc3);

    document* test = test_tree.find(2);
    if(test != nullptr) {
        test->display();
    }
    document* removed = test_tree.remove(2);
    if(removed != nullptr) {
        removed->display();
    }
    document* debug = test_tree.find(3);
    if(debug != nullptr) {
        debug->display();
    }
}
