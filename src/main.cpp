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

    OperatorNode* root = new OperatorNode(OperatorNode::SUB);
    /* well, i can't access the root's lhs so what now? */
    root->set_lhs(new OperandNode(5.0));
    root->set_rhs(new OperandNode(3.0));

    root->print();

    /*
    delete root->lhs->lhs;
    delete root->lhs->rhs;
    delete root->lhs;
    delete root->rhs;
    */
    delete root;
}
