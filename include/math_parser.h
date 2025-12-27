#include <iostream>
#include <stdexcept>

class Node {
private:
    Node* lhs;
    Node* rhs;
public:
    Node(): lhs(nullptr), rhs(nullptr) {} // virtual constructor
    virtual ~Node()
    { // virtual destructor
        delete this->lhs;
        delete this->rhs;
    }

    virtual double evaluate() { return 0.0; }

    friend class OperatorNode;
    friend class OperandNode;
};

class OperatorNode : Node {
public:
    enum operators {
        ADD = '+',
        SUB = '-',
        MUL = '*',
        DIV = '/',
    };
    operators op;

    OperatorNode(operators op): op(op) {} // constructor

    double evaluate() override
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

    friend class MathExpression;
};

class OperandNode : public Node {
public:
    double val;
    OperandNode(double val): val(val) {} // constructor
    double evaluate() override { return this->val; }
};
