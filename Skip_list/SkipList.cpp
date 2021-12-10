#include <functional>
#include<iostream>

template<typename T>
class skipList {
    struct Node {
        T value;
        Node **pr_list, **nxt_list;
        explicit Node(T curr_value, size_t curr_size) {
            value = curr_value;
            pr_list = new Node *[curr_size];
            nxt_list = new Node *[curr_size];
            for (size_t i = 0; i < curr_size; i++) {
                pr_list[i] = nxt_list[i] = nullptr;
            }
        }
        ~Node() {
            delete next();
            delete[] pr_list;
            delete[] nxt_list;
        }
        Node *next(size_t curr_level = 0) const {
            return nxt_list[curr_level];
        }
        Node *prev(size_t curr_level = 0) const {
            return pr_list[curr_level];
        }
        static void soed(Node *node_1, Node *node_2, size_t curr_level) {
            if (node_1){
                node_1->nxt_list[curr_level] = node_2;
            }
            if (node_2){
                node_2->pr_list[curr_level] = node_1;
            }
        }
    };
    class bidir_iter : public std::iterator<std::bidirectional_iterator_tag, skipList<T>> {
        Node *curr_ptr;
    public:
        explicit bidir_iter(Node *curr_node) {
            curr_ptr = curr_node;
        }
        Node *operator~() {
            return curr_ptr;
        }
        T &operator*() {
            return curr_ptr->value;
        }
        const T &operator*() const {
            return curr_ptr->value;
        }
        bidir_iter &operator++() {
            curr_ptr = curr_ptr->next();
            return *this;
        }
        bidir_iter &operator--() {
            curr_ptr = curr_ptr->prev();
            return *this;
        }
        bool operator==(const bidir_iter &that) const {
            return curr_ptr == that.curr_ptr;
        }
        bool operator!=(const bidir_iter &that) const {
            return curr_ptr != that.curr_ptr;
        }
    };

    std::function<int(T, T)> comparator;
    size_t curr_size = 0;
    size_t max_level{}, level_num{};
    float probability{};
    Node *skip_start, *skip_end;
public:
    explicit skipList(size_t max_depth, float curr_probability, std::function<int(T, T)> comp) {
        comparator = comp;
        max_level = max_depth;
        probability = curr_probability;
        level_num = 1;
        skip_end = skip_start = new Node(-1, max_level);
    }
    void copy(const skipList<T> &source) {
        auto **most_right = new Node *[max_level];
        auto **most_right_source = new Node *[max_level];
        comparator = source.comparator;
        max_level = source.max_level;
        level_num = source.level_num;
        curr_size = source.curr_size;
        skip_end = source.skip_end;
        probability = source.probability;
        skip_start = new Node(-1, max_level);
        for (int i = 0; i < max_level; i++) {
            most_right[i] = skip_start;
            most_right_source[i] = source.skip_start;
        }
        Node *temp = skip_start;
        Node *temp_source = source.skip_start;
        while (temp_source) {
            for (int i = 0; i < max_level; i++) {
                if (most_right_source[i]->nxt_list[i] == temp_source) {
                    Node::soed(most_right[i], temp, i);
                    most_right_source[i] = temp_source;
                    most_right[i] = temp;
                }
            }
            temp_source = temp_source->nxt_list[0];
            if (temp_source) {
                temp->nxt_list[0] = new Node(temp_source->value, max_level);
            }
            temp = temp->nxt_list[0];
        }
        delete[] most_right;
        delete[] most_right_source;
    }
    skipList(const skipList<T> &source) {
        copy(source);
    }
    void move(skipList<T> &&source) {
        source.level_num = 0;
        source.curr_size = 0;
        skip_start = source.skip_start;
        skip_end = source.skip_end;
        max_level = source.max_level;
        level_num = source.level_num;
        curr_size = source.curr_size;
        source.skip_end = source.skip_start = nullptr;
        comparator = source.comparator;
        probability = source.probability;
    }
    skipList(skipList<T> &&source) {
        move();
    }
    skipList &operator=(skipList<T> &&source) {
        if (&source != this) {
            delete[] skip_start;
            move();
        }
        return *this;
    }

    skipList<T> &operator=(const skipList<T> &that) {
        if (this == &that) return *this;
        delete skip_start;
        copy(that);
        return *this;
    }
    ~skipList() {
        delete skip_start;
    }

    void insert(T curr_value) {
        Node **most_right = new Node *[max_level];
        for (int i = 0; i < max_level; i++) {
            most_right[i] = skip_start;
        }
        Node *curr_node = skip_start, *next_node;
        for (int i = level_num - 1; i >= 0; i--) {
            next_node = curr_node->next(i);
            while (next_node != nullptr && comparator(next_node->value, curr_value) < 0) {
                curr_node = next_node;
                next_node = curr_node->next(i);
            }
            most_right[i] = curr_node;
        }
        Node *new_node = new Node(curr_value, max_level);
        Node::soed(new_node, most_right[0]->next(0), 0);
        Node::soed(most_right[0], new_node, 0);
        for (int i = 1; (i < max_level) && RNG(); i++) {
            Node::soed(new_node, most_right[i]->next(i), i);
            Node::soed(most_right[i], new_node, i);
            if (i + 1 > level_num){
                level_num = i + 1;
            }
        }
        curr_size++;
        if (most_right[0] == skip_end)skip_end = curr_node;
        delete[] most_right;
    }


    size_t size() const {
        return curr_size;
    }
    bool empty() const {
        return !(skip_start->next());
    }
    bool RNG() const {
        return (((float) rand()) / RAND_MAX) < probability;
    }

    void print() {
        for (int l = max_level - 1; l >= 0; l--) {
            std::cout << std::endl << "Level " << l << " : ";
            Node *P = skip_start->next(l);
            while (P) {
                std::cout << P->value << ' ';
                P = P->next(l);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    void clear() {
        delete skip_start;
        skip_end = skip_start = new Node(-1, max_level);
        level_num = 1;
        curr_size = 0;
    }

    bidir_iter begin() {
        return bidir_iter(skip_start->next());
    }
    bidir_iter end() {
        return bidir_iter(nullptr);
    }
    bidir_iter find(T curr_value) {
        Node *p1 = skip_start, *p2;
        for (int i = level_num - 1; i >= 0; i--) {
            p2 = p1->next(i);
            while (p2 != nullptr) {
                if (comparator(p2->value, curr_value) < 0) {
                    p1 = p2;
                    p2 = p1->next(i);
                } else if (comparator(p2->value, curr_value) == 0) {
                    return bidir_iter(p2);
                } else {
                    break;
                }
            }
        }
        return end();
    }


    size_t count(T value) {
        bidir_iter left_iter = find(value);
        if (left_iter == end())return 0;
        size_t num = 1;
        bidir_iter right_iter = left_iter, curr_iter = right_iter;
        while (*++curr_iter == *right_iter) {
            right_iter = curr_iter;
            num++;
        }
        curr_iter = left_iter;
        while (*--curr_iter == *left_iter) {
            left_iter = curr_iter;
            num++;
        }
        return num;
    }

    std::pair<bidir_iter, bidir_iter> equal_range(T curr_value) {
        bidir_iter left_iter = find(curr_value);
        if (left_iter == end()) std::make_pair(left_iter, left_iter);
        bidir_iter right_iter = left_iter, curr_iter = right_iter;
        while (*++curr_iter == *right_iter) {
            right_iter = curr_iter;
        }
        curr_iter = left_iter;
        while (*--curr_iter == *left_iter) {
            left_iter = curr_iter;
        }
        return std::make_pair(left_iter, right_iter);
    }


    bidir_iter upper_bound(T curr_value) {
        Node *p1 = skip_start, *p2;
        for (int i = level_num - 1; i >= 0; i--) {
            p2 = p1->next(i);
            while (p2 != nullptr) {
                if (comparator(p2->value, curr_value) <= 0) {
                    p1 = p2;
                    p2 = p1->next(i);
                } else {
                    break;
                }
            }
        }
        return bidir_iter(p2);
    }

    bidir_iter lower_bound(T curr_value) {
        Node *p1 = skip_start, *p2;
        for (int i = level_num - 1; i >= 0; i--) {
            p2 = p1->next(i);
            while (p2 != nullptr) {
                if (comparator(p2->value, curr_value) < 0) {
                    p1 = p2;
                    p2 = p1->next(i);
                } else {
                    break;
                }
            }
        }
        return bidir_iter(p2);
    }


    void erase(bidir_iter curr_node) {
        Node *deleted = ~curr_node;
        for (int i = 0; i < max_level; i++) {
            Node::soed(deleted->prev(i), deleted->next(i), i);
            deleted->nxt_list[i] = nullptr;
        }
        delete deleted;
    }
    void erase(std::pair<bidir_iter, bidir_iter> curr_pair) {
        erase(curr_pair.first, curr_pair.second);
    }
    void erase(bidir_iter first, bidir_iter last) {
        bidir_iter curr_last = last;
        ++curr_last;
        while (first != curr_last) {
            bidir_iter temp = first;
            ++temp;
            erase(first);
            first = temp;
        }
    }
};

int comp(int a, int b){
    return a - b;
}


int main() {
    skipList<int> A(7, 0.5, comp);
    A.insert(10);
    for (int  i = 0; i < 300; i++){
        A.insert(rand());
    }
    A.insert(10);
    A.print();
    std::cout << A.count(10) << std::endl;
    A.erase(A.equal_range(10));
    A.print();

    return 0;
}
