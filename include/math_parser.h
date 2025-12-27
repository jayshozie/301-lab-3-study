#include <iostream>
#include <stdexcept>

class Node {
public:
    Node* lhs;
    Node* rhs;

    virtual Node() : lhs(nullptr), rhs(nullptr) {}

    virtual ~Node() {
        delete this->lhs;
        delete this->rhs;
    }
    virtual double evaluate() {
        return 0.0;
    };
};

class OperatorNode : public Node {
public:
    enum operators {
        ADD = '+',
        SUB = '-',
        MUL = '*',
        DIV = '/',
    };

    operators op;

    OperatorNode(operators op): op(op) {}// constructor

    double evaluate()
    {
        /* DEBUG START */
        /* DEBUG END */
        switch(this->op) {
        case ADD:
            return this->lhs->evaluate() + this->rhs->evaluate();
        case SUB:
            return this->lhs->evaluate() - this->rhs->evaluate();
        case MUL:
            return this->lhs->evaluate() * this->rhs->evaluate();
        case DIV:
            if(this->rhs->evaluate() == 0) {
                throw std::domain_error("ERR: DIVISION BY ZERO");
            }
            return this->lhs->evaluate() / this->rhs->evaluate();
        default:
            throw std::logic_error("ERR: UNKNOWN OPERATOR");
        }
    }
};

class OperandNode : public Node {
public:
    double val;
    OperandNode(double val): val(val) {} // constructor
    double evaluate() override { return this->val; }
};

