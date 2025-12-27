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

    OperatorNode* root = new OperatorNode(OperatorNode::SUB);
    root->lhs = new OperatorNode(OperatorNode::ADD);
    root->lhs->lhs = new OperandNode(3.0);
    root->lhs->rhs = new OperandNode(5.0);
    root->rhs = new OperandNode(2.0);

    std::cout << root->lhs->lhs->evaluate() << std::endl;
    std::cout << root->lhs->rhs->evaluate() << std::endl;
    std::cout << root->lhs->evaluate() << std::endl;
    std::cout << root->rhs->evaluate() << std::endl;

    printf("%.1f\n", root->evaluate());

    /*
    delete root->lhs->lhs;
    delete root->lhs->rhs;
    delete root->lhs;
    delete root->rhs;
    */
    delete root;
}
