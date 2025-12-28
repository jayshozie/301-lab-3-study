#include <algorithm>
#include <cmath>
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
private:
    int64_t height;
    document* data;
    avl_node* lhs;
    avl_node* rhs;
public:
    avl_node(document* data) // avl node constructor
        : height(0), data(data), lhs(nullptr), rhs(nullptr)
    {}

    friend class avl_tree;
};

class avl_tree {
public:
    avl_tree(): root(nullptr) {} // constructor
    ~avl_tree() { this->clear(this->root); }

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

    void clear(avl_node* node)
    {
        if(node == nullptr) { return; }
        this->clear(node->lhs);
        this->clear(node->rhs);
        delete node;
    }

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
    // hash formula:
    // h = (s[0]*(53)^0 + s[1]*(53)^1 + ...) (mod 1009)
public:
    hash_table()
    {
        // initialize the entire table
        for(uint64_t i = 0; i < 1009; i++) { this->table[i] = nullptr; }
    } // hash table constructor

    ~hash_table()
    {
        for(uint64_t i = 0; i < 1009; i++) {
            hash_node* curr = this->table[i];
            while(curr != nullptr) {
                hash_node* tmp = curr;
                curr = curr->next;
                delete tmp;
            }
        }
    }

    void insert(std::string tag, document* doc)
    {
        uint64_t index = this->hash(tag);
        hash_node* new_node = new hash_node();
        new_node->tag = tag;
        new_node->doc = doc;
        new_node->next = this->table[index];
        this->table[index] = new_node;
    }
    std::vector<document*> search(std::string tag)
    {
        uint64_t index = this->hash(tag);

        std::vector<document*> ret_vec;
        hash_node* curr = this->table[index];

        while(curr != nullptr) {
            if(curr->tag == tag) { ret_vec.push_back(curr->doc); }
            curr = curr->next;
        }
        return ret_vec;
    }
private:
    struct hash_node {
        std::string tag;
        document* doc;
        hash_node* next;
    };

    hash_node* table[1009];

    uint64_t hash(std::string tag)
    {
        uint64_t hash = 0;
        uint64_t i = 0;
        uint64_t p = 1;
        while(i < tag.length()) {
            // get ascii value
            uint64_t char_val = tag[i];
            // normalize the number
            char_val %= 1009;

            // calculate the current hash value
            hash += (char_val * p) % 1009;

            // because first char should be multiplied with 1, not 53
            p = (p * 53) % 1009;

            i++;
        }
        return hash % 1009;
    }
};

class priority_queue {
    // min-heap implementation
    // parent of an index i      : (i-1)/2
    // left child of an index i  : (2 * i) + 1
    // right child of an index i : (2 * i) + 2
private:
    std::vector<document*> heap;

    void heapify_up(uint64_t doc_ind)
    {
        if(doc_ind == 0) { return; }

        uint64_t parent_ind = (doc_ind - 1) / 2;
        if(this->heap[doc_ind]->priority < this->heap[parent_ind]->priority) {
            document* tmp = this->heap[doc_ind];
            this->heap[doc_ind] = this->heap[parent_ind];
            this->heap[parent_ind] = tmp;
            this->heapify_up(parent_ind);
        }
    }
    void heapify_down(uint64_t doc_ind)
    {
        uint64_t lhs = (2 * doc_ind) + 1;
        bool lhs_in_bounds = (lhs < this->heap.size()) ? true : false;

        uint64_t rhs = (2 * doc_ind) + 2;
        bool rhs_in_bounds = (rhs < this->heap.size()) ? true : false;

        uint64_t smallest = doc_ind;
        if(lhs_in_bounds && this->heap[lhs] != nullptr
           && this->heap[lhs]->priority < this->heap[smallest]->priority) {
            smallest = lhs;
        }
        if(rhs_in_bounds && this->heap[rhs] != nullptr
           && this->heap[rhs]->priority < this->heap[smallest]->priority) {
            smallest = rhs;
        }
        if(smallest != doc_ind) {
            // swap
            document* tmp = this->heap[doc_ind];
            this->heap[doc_ind] = this->heap[smallest];
            this->heap[smallest] = tmp;

            this->heapify_down(smallest);
        }
    }
public:
    document* get_next_task()
    {
        if(this->heap.empty()) { return nullptr; }

        // swap first with last
        document* tmp = this->heap[0];
        this->heap[0] = this->heap.back();
        this->heap.pop_back();

        if(!this->heap.empty()) { this->heapify_down(0); }

        return tmp;
    }
    void add_new_task(document* doc)
    {
        this->heap.push_back(doc);
        this->heapify_up(this->heap.size() - 1);
    }
};

class visited_node_set { // small helper class for knowledge graph
private:
    struct visited_node { // linked list of visited nodes
        uint64_t id;
        visited_node* next;
    };
    visited_node* visited_node_table[101];
public:
    visited_node_set()
    {
        for(uint64_t i = 0; i < 101; i++) {
            this->visited_node_table[i] = nullptr;
        }
    } // helper visited node set constructor

    ~visited_node_set()
    {
        for(uint64_t i = 0; i < 101; i++) {
            visited_node* traveler = this->visited_node_table[i];
            while(traveler != nullptr) {
                visited_node* saver = traveler->next;
                ;
                delete traveler;
                traveler = saver;
            }
        }
    } // helper visited node set destructor

    uint64_t hash(uint64_t id) { return (id * 2654435761) % 101; }

    void mark(uint64_t id)
    {
        uint64_t index = this->hash(id);
        visited_node* new_node = new visited_node();
        new_node->id = id;
        new_node->next = this->visited_node_table[index];
        this->visited_node_table[index] = new_node;
    }
    bool has(uint64_t id)
    {
        uint64_t index = this->hash(id);
        visited_node* curr = this->visited_node_table[index];
        while(curr != nullptr) {
            if(curr->id == id) { return true; }
            curr = curr->next;
        }
        return false;
    }
};
class knowledge_graph {
public:
    knowledge_graph()
    {
        for(uint64_t i = 0; i < 1009; i++) { this->node_table[i] = nullptr; }
    } // knowledge graph constructor

    ~knowledge_graph()
    {
        for(uint64_t i = 0; i < 1009; i++) {
            graph_node* curr_node = this->node_table[i];
            while(curr_node != nullptr) {
                ref* curr_ref = curr_node->refs;
                /* delete all refs for this node */
                while(curr_ref != nullptr) {
                    ref* tmp_ref = curr_ref;
                    curr_ref = curr_ref->next;
                    delete tmp_ref;
                }

                /* delete the graph node itself */
                graph_node* tmp_node = curr_node;
                curr_node = curr_node->next;
                delete tmp_node;
            }
        }
    }

    void add_reference(uint64_t from_id, uint64_t to_id)
    {
        uint64_t from_index = this->hash(from_id);

        graph_node* node = this->node_table[from_index];
        while(node != nullptr) {
            if(node->from_id == from_id) { break; }
            else {
                node = node->next;
            }
        }
        if(node == nullptr) { // no refs
            graph_node* new_node = new graph_node();
            node = new_node;
            node->from_id = from_id;
            node->refs = nullptr;

            node->next = this->node_table[from_index];
            this->node_table[from_index] = node;
        }
        ref* new_ref = new ref();
        new_ref->to_id = to_id;
        new_ref->next = node->refs;
        node->refs = new_ref;

        // locate node from_id
        // append to_id to the list of documents that from_id references
    }

    bool is_reachable(uint64_t source_id, uint64_t target_id)
    {
        visited_node_set visited;

        return dfs_recursive(source_id, target_id, visited);
    }
private:
    /* a linked list of the references made in a document */
    struct ref {
        uint64_t to_id; // id of the document this document references to
        ref* next; // next reference in made in this document
    };

    /* a linked list of the graph nodes */
    struct graph_node {
        uint64_t from_id; // id of the document
        ref* refs; // references made from this node
        graph_node* next; // next node in the graph
    };

    graph_node* node_table[1009];
    uint64_t hash(uint64_t id) { return (id * 2654435761) % 1009; }

    bool dfs_recursive(uint64_t current_id, uint64_t target_id,
                       visited_node_set& visited_nodes)
    {
        if(current_id == target_id) { return true; }
        if(visited_nodes.has(current_id)) { return false; }
        visited_nodes.mark(current_id);

        uint64_t index = this->hash(current_id);
        graph_node* node = this->node_table[index];
        while(node != nullptr && node->from_id != current_id) {
            node = node->next;
        }

        if(node != nullptr) {
            ref* curr_ref = node->refs;
            while(curr_ref != nullptr) {
                if(dfs_recursive(curr_ref->to_id, target_id, visited_nodes)) {
                    return true;
                }
                curr_ref = curr_ref->next;
            }
        }
        return false;
    }
};
