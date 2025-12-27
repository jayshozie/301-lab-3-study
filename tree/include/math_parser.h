#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

class Node {
private:
    Node* lhs;
    Node* rhs;
public:
    Node(): lhs(nullptr), rhs(nullptr) {} // virtual constructor
    virtual ~Node()
    {
        delete this->lhs;
        delete this->rhs;
    } // virtual destructor

    Node(const Node& original)
        : lhs(original.lhs ? original.lhs->clone() : nullptr),
          rhs(original.rhs ? original.rhs->clone() : nullptr)
    {} // copy constructor

    virtual void set_lhs(Node* node)
    {
        if(this->lhs != node) {
            delete this->lhs;
            this->lhs = node;
            return;
        }
        return;
    }
    virtual void set_rhs(Node* node)
    {
        if(this->rhs != node) {
            delete this->rhs;
            this->rhs = node;
            return;
        }
        return;
    }

    virtual int count_nodes() = 0;
    virtual int get_height() = 0;
    virtual void print() = 0;
    virtual double evaluate() = 0;
    virtual Node* clone() = 0;
    virtual Node* optimize() = 0;

    friend class OperandNode;
    friend class OperatorNode;
};

class OperandNode : public Node {
public:
    double val;

    OperandNode(double val): val(val) {} // constructor

    OperandNode& operator=(const OperandNode& original)
    {
        if(this != &original) { // self-assignment check
            this->val = original.val;
        }
        return *this;
    } // overloaded assignment operator

    Node* clone() override { return new OperandNode(this->val); }

    double evaluate() override { return this->val; }
    OperandNode* optimize() override { return this; }

    void print() override { std::cout << this->val; }

    int count_nodes() override { return 1; }
    int get_height() override { return 0; }
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

    OperatorNode(const OperatorNode& original)
        : Node(original), op(original.op) {} // copy constructor

    OperatorNode& operator=(const OperatorNode& original)
    {
        if(this != &original) { // self-assignment check
            OperatorNode* tmp = new OperatorNode(original.op);

            tmp->set_lhs(original.lhs->clone());
            tmp->set_rhs(original.rhs->clone());

            delete this->lhs;
            delete this->rhs;

            this->op = tmp->op;
            this->lhs = tmp->lhs;
            this->rhs = tmp->rhs;

            tmp->lhs = nullptr;
            tmp->rhs = nullptr;
            delete tmp;
        }
        return *this;
    } // overloaded assignment operator

    OperatorNode* clone() override
    {
        OperatorNode* copy = new OperatorNode(this->op);
        if(this->lhs != nullptr) { copy->lhs = this->lhs->clone(); }
        if(this->rhs != nullptr) { copy->rhs = this->rhs->clone(); }
        return copy;
    } // cloner

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

    Node* optimize() override
    {
        this->set_lhs(this->lhs->optimize());
        this->set_rhs(this->rhs->optimize());

        if(this->lhs->get_height() == 0 && this->rhs->get_height() == 0) {
            double result = this->evaluate();
            return new OperandNode(result);
        }
        else {
            return this;
        }
    }

    void print() override
    {
        std::string op_str;
        switch(this->op) {
        case ADD:
            op_str = "+";
            break;
        case SUB:
            op_str = "-";
            break;
        case MUL:
            op_str = "*";
            break;
        case DIV:
            op_str = "/";
            break;
        default:
            op_str = "#";
            break;
        }
        std::cout << "(";
        this->lhs->print();
        std::cout << " " << op_str << " ";
        this->rhs->print();
        std::cout << ") ";
    }

    int count_nodes() override
    {
        return 1 + this->lhs->count_nodes() + this->rhs->count_nodes();
    }

    int get_height() override
    {
        return 1 + std::max(this->lhs->get_height(), this->rhs->get_height());
    }

    friend class MathExpression;
};
