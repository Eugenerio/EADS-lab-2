#ifndef LAB2_BI_RING_H
#define LAB2_BI_RING_H

template <typename Key, typename Info>
class bi_ring {
private:
    struct Node {
        Key key;
        Info info;
        Node* prev;
        Node* next;
    };


    Node* head;
    unsigned int count;
public:
    class iterator {
    public:
        iterator(): current(nullptr){}
        iterator(const iterator& other): current(other.current){}
        iterator& operator=(const iterator& other){
            if (this != &other) {
                current = other.current;
            }
            return *this;
        };
        ~iterator() = default;

        iterator& operator++(){
            current = current->next;
            return *this;
        };
        iterator operator++(int){
            iterator temp = *this;
            ++(*this);
            return temp;
        };
        iterator& operator--(){
            current = current->prev;
            return *this;
        };
        iterator operator--(int){
            iterator temp = *this;
            --(*this);
            return temp;
        };

        bool operator==(const iterator& other) const{
            return current == other.current;
        };
        bool operator!=(const iterator& other) const{
            return *this != other;
        };

        Key& operator*() const{
            return current->key;
        };
        Key* operator->() const{
            return &(current->key);
        };

    private:
        Node* current;
        friend class bi_ring;
    };

    class const_iterator {
    public:
        const_iterator(): current(nullptr) {};
        const_iterator(const const_iterator& other): current(other.current) {};
        const_iterator& operator=(const const_iterator& other){
            if (this != &other) {
                current = other.current;
            }
            return *this;
        };
        ~const_iterator() {};

        const_iterator& operator++(){
            current = current->next;
            return *this;
        };
        const_iterator operator++(int){
            const_iterator temp = *this;
            ++(*this);
            return temp;
        };
        const_iterator& operator--(){
            current = current->prev;
            return *this;
        };
        const_iterator operator--(int){
            const_iterator temp = *this;
            --(*this);
            return temp;
        };

        bool operator==(const const_iterator& other) const{
            return current == other.current;
        };
        bool operator!=(const const_iterator& other) const{
            return *this != other;
        };

        const Key& operator*() const{
            return current->key;
        };
        const Key* operator->() const{
            return &(current->key);
        };

    private:
        const Node* current;
        friend class bi_ring;
    };

    bi_ring(): count(0){
        head = new Node;
        head->prev = head->next = head;
    };
    bi_ring(const bi_ring& src) : count(0) {
        head = new Node;
        head->prev = head->next = head;

        for(const auto& elements : src){
            push_front(elements.key, elements.info);
        };
    }
    ~bi_ring(){
        clear();
        delete head;
    };
    bi_ring& operator=(const bi_ring& src){
        if(this != &src){
            clear();

            for(const auto& pair: src){
                push_front(pair.key, pair.info);
            }
        }

        return *this;
    };

    iterator push_front(const Key& key, const Info& info){
        Node *newNode = new Node{key, info, head, head->next};
        head->next->prev = newNode;
        head->next = newNode;
        count++;
        return iterator(newNode);
    };
    iterator pop_front(){
        if(count == 0){
            return end();
        }

        Node* removedNode = head->next;
        head->next = removedNode->next;
        removedNode->next->prev = head;
        iterator result(removedNode);
        delete removedNode;
        count--;
        return result;
    };

    iterator insert(iterator position, const Key& key, const Info& info){
        Node* newNode = new Node{key, info, position.current->prev, position.current};
        position.current->prev->next = newNode;
        position.current->prev = newNode;
        count++;
        return iterator(newNode);
    };
    iterator erase(iterator position){
        if (count == 0) {
            return end();
        }

        Node* removedNode = position.current;
        position.current->prev->next = position.current->next;
        position.current->next->prev = position.current->prev;
        iterator result(position.current->next);
        delete removedNode;
        count--;
        return result;
    };

    bool clear(){
        if(!empty()){
            pop_front();
        }
        return true;
    };
    [[nodiscard]] bool empty() const{
        return count == 0;
    };
    [[nodiscard]] unsigned int size() const{
        return count;
    };
    iterator begin(){
        return iterator(head->next);
    };
    const_iterator cbegin() const{
        return const_iterator(head->next);
    };
    iterator end(){
        return iterator(head);
    };
    const_iterator cend() const{
        return const_iterator(head);
    };
    iterator find(const Key& key){
        for(iterator it = begin(); it != end(); ++it){
            if(*it == key){
                return it;
            }
        }
    };
    const_iterator cfind(const Key& key) const{
        for (const_iterator it = cbegin(); it != cend(); ++it) {
            if (*it == key) {
                return it;
            }
        }
        return cend();
    };
};

#endif
