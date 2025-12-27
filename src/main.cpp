#include "math_parser.h"
#include <iostream>

int main() {
    // (3 + 5) - 2
    // to nodes

    /*
     * How the tree should look:
     *       -
     *      / \
     *    +    2
     *   / \
     *  3   5
     */

    /* old
    OperatorNode* root = new OperatorNode(OperatorNode::SUB);
    root->lhs = new OperatorNode(OperatorNode::ADD);
    root->lhs->lhs = new OperandNode(3.0);
    root->lhs->rhs = new OperandNode(5.0);
    root->rhs = new OperandNode(2.0);
    */

    OperatorNode* left = new OperatorNode(OperatorNode::ADD);
    left->set_lhs(new OperandNode(3.0));
    left->set_rhs(new OperandNode(5.0));

    OperatorNode* root = new OperatorNode(OperatorNode::SUB);
    root->set_lhs(left);
    root->set_rhs(new OperandNode(2.0));

    root->print();
    std::cout << std::endl << root->evaluate() << std::endl;
    std::cout << root->get_height() << std::endl;

    /*
    delete root->lhs->lhs;
    delete root->lhs->rhs;
    delete root->lhs;
    delete root->rhs;
    */
    delete root;
}
