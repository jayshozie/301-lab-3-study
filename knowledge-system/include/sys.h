#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

class document {
public:
    uint64_t id;
    uint8_t priority;
    std::string content;
    std::vector<std::string> tags;
    std::vector<uint64_t> refs;

    document(uint64_t id, std::string content): id(id), content(content) {}

    void add_tag(std::string tag) { this->tags.push_back(tag); }
    void add_ref(uint64_t ref) { this->refs.push_back(ref); }

    void display()
    {
        std::cout << "\nDoc ID: " << this->id << "\n"
                  << "Content:\n"
                  << "--- DOC START ---\n"
                  << this->content << "\n"
                  << "---- DOC END ----\n";
    }
};

class avl_node {
public:
    avl_node(document* data) // avl node constructor
        : data(data), height(0), lhs(nullptr), rhs(nullptr)
    {}

    friend class avl_tree;
private:
    int64_t height;
    document* data;
    avl_node* lhs;
    avl_node* rhs;
};

class avl_tree {
public:
    avl_tree(): root(nullptr) {} // constructor

    void insert(document* doc)
    {
        if(this->root == nullptr) { this->root = this->insert(nullptr, doc); }
        else {
            this->root = this->insert(this->root, doc);
        }
    }
    document* remove(uint64_t id)
    {
        document* doc = nullptr;
        this->root = this->remove(this->root, id, doc);
        return doc;
    }
    document* find(uint64_t id) { return this->find(this->root, id); }
private:
    avl_node* root;

    int64_t get_height(avl_node* node)
    {
        return (node == nullptr ? 0 : node->height);
    }

    int64_t get_balance(avl_node* node)
    {
        return this->get_height(node->lhs) - this->get_height(node->rhs);
    }

    /*
     * |  before:    |  after:     |
     * |      x      |       y     |
     * |       \     |      /      |
     * |        y    |     x       |
     * |       /     |      \      |
     * |    y_lhs    |     y_lhs   |
     */
    avl_node* rotate_left(avl_node* x)
    {
        // create tmp node pointers
        avl_node* y = x->rhs;
        avl_node* y_lhs = y->lhs;

        // perform rotation
        // x becomes y's lhs
        y->lhs = x;
        // y's lhs becomes x's rhs
        x->rhs = y_lhs;

        // update heights
        x->height = 1 + std::max(get_height(x->lhs), get_height(x->rhs));
        y->height = 1 + std::max(get_height(y->lhs), get_height(y->rhs));

        return y;
    }

    /*
     * |  before:    |  after:    |
     * |      y      |       x    |
     * |     /       |        \   |
     * |    x        |        y   |
     * |     \       |       /    |
     * |    x_rhs    |    x_rhs   |
     */
    avl_node* rotate_right(avl_node* y)
    {
        // create tmp node pointers
        avl_node* x = y->lhs;
        avl_node* x_rhs = x->rhs;

        // perform rotation
        // y becomes x's rhs
        x->rhs = y;
        // x's rhs becomes y's lhs
        y->lhs = x_rhs;

        // update heights
        x->height = 1 + std::max(get_height(x->lhs), get_height(x->rhs));
        y->height = 1 + std::max(get_height(y->lhs), get_height(y->rhs));

        return x;
    }

    avl_node* find_min(avl_node* node)
    {
        avl_node* traversal = node->rhs;
        avl_node* follower = nullptr;
        while(traversal != nullptr) {
            follower = traversal;
            traversal = traversal->lhs;
        }
        return follower;
    }

    avl_node* rebalance(avl_node* node)
    {
        int64_t node_balance = this->get_balance(node);
        if(node_balance > 1) { // Left Heavy
            // L ROTATIONS
            int64_t lhs_balance = this->get_balance(node->lhs);
            if(lhs_balance >= 0) {
                // LL ROTATION
                return this->rotate_right(node);
            }
            else {
                // LR ROTATION
                node->lhs = this->rotate_left(node->lhs);
                return this->rotate_right(node);
            }
        }
        else if(node_balance < -1) { // Right Heavy
            // R ROTATIONS
            int64_t rhs_balance = this->get_balance(node->rhs);
            if(rhs_balance <= 0) {
                // RR ROTATION
                return this->rotate_left(node);
            }
            else {
                // RL ROTATION
                node->rhs = this->rotate_right(node->rhs);
                return this->rotate_left(node);
            }
        }
        return node;
    }

    avl_node* insert(avl_node* node, document* doc)
    {
        // base case
        if(node == nullptr) { return new avl_node(doc); }

        // recursive cases
        if(doc->id < node->data->id) {
            node->lhs = this->insert(node->lhs, doc);
        }
        else if(doc->id > node->data->id) {
            node->rhs = this->insert(node->rhs, doc);
        }
        else { // doc-> id == node->data->id
            throw std::logic_error("ERR: A DOCUMENT WITH THE SAME ID EXISTS.");
        }

        // update height
        uint64_t lhs_height = 0;
        uint64_t rhs_height = 0;
        if(node->lhs != nullptr) { lhs_height = this->get_height(node->lhs); }
        if(node->rhs != nullptr) { rhs_height = this->get_height(node->rhs); }
        node->height = 1 + std::max(lhs_height, rhs_height);

        return this->rebalance(node);
    }

    document* find(avl_node* node, uint64_t id)
    {
        if(node != nullptr) {
            if(id < node->data->id) {
                // id is to the left of curr
                return this->find(node->lhs, id);
            }
            else if(id > node->data->id) {
                return this->find(node->rhs, id);
            }
            else {
                // base case
                return node->data;
            }
        }
        else {
            return nullptr;
        }
    }

    avl_node* remove(avl_node* node, uint64_t id, document*& doc)
    {
        // recursive cases
        if(id < node->data->id) {
            node->lhs = this->remove(node->lhs, id, doc);
        }
        else if(id > node->data->id) {
            node->rhs = this->remove(node->rhs, id, doc);
        }
        else { // found
            doc = node->data;

            // leaf node
            if(node->lhs == nullptr && node->rhs == nullptr) {
                delete node;
                return nullptr;
            }
            // one child cases
            else if(node->lhs != nullptr && node->rhs == nullptr) {
                // left child
                avl_node* tmp = node->lhs;
                delete node;
                return tmp;
            }
            else if(node->lhs == nullptr && node->rhs != nullptr) {
                // right child
                avl_node* tmp = node->rhs;
                delete node;
                return tmp;
            }
            // two child case
            else {
                avl_node* min = this->find_min(node);
                node->data = min->data;
                document* t_doc;
                node->rhs = this->remove(node->rhs, min->data->id, t_doc);
            }
        }

        // height correction
        if(node != nullptr) {
            uint64_t lhs_height = 0;
            uint64_t rhs_height = 0;
            if(node->lhs != nullptr) {
                lhs_height = this->get_height(node->lhs);
            }
            if(node->rhs != nullptr) {
                rhs_height = this->get_height(node->rhs);
            }
            node->height = 1 + std::max(lhs_height, rhs_height);
        }

        return this->rebalance(node);
    }
};

class hash_table {
public:
    std::vector<document*> get_by_tag(std::string tag) {}
};

class priority_queue {
public:
    document* get_next_task() {}
};

class knowledge_graphs {
public:
    void add_referrence(int from_id, int to_id) {}
};
