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

    virtual void set_lhs(Node* node) {
        this->lhs = node;
    }
    virtual void set_rhs(Node* node) {
        this->rhs = node;
    }

    virtual void print() = 0;
    virtual double evaluate() = 0;

    friend class OperatorNode;
    friend class OperandNode;
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

    OperatorNode(operators op): op(op) {} // constructor

    double evaluate() override
    {
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

    void print() override {
        std::string op_str;
        switch(this->op) {
        case ADD:
            op_str = "+";
        case SUB:
            op_str = "-";
        case MUL:
            op_str = "*";
        case DIV:
            op_str = "/";
        default:
            op_str = "#";
        }
        std::cout << "(";
        this->lhs->print();
        std::cout << " " << op_str << " ";
        this->rhs->print();
        std::cout << ") ";
    }

    friend class MathExpression;
};

class OperandNode : public Node {
public:
    double val;
    OperandNode(double val): val(val) {} // constructor
    double evaluate() override { return this->val; }

    void print() override { std::cout << this->val; }
};
