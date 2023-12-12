#ifndef LAB2_BI_RING_H
#define LAB2_BI_RING_H
#include <iostream>

using namespace std;

template <typename Key, typename Info>
class bi_ring {
private:
    class Node {
    private:
        Node* prev;
        Node* next;
    public:
        Key key;
        Info info;

        Node(const Key &key, const Info &info, Node *next, Node *prev): key(key), info(info), next(next), prev(prev){}

        friend  class bi_ring;
    };

    template<typename KeyT, typename InfoT, typename Ring>
    class iterator {
    private:
        friend class bi_ring;

        Node *ptr;
        const Ring *ring;

        iterator(Node *ptr, const Ring *ring): ptr(ptr), ring(ring) {}

    public:
        bool operator==(const iterator &other) const{
            return  ptr == other.ptr;
        }

        bool operator!=(const iterator &other) const{
            return  ptr != other.ptr;
        }

        iterator &operator=(const iterator &src){
            if(this != &src){
                ptr = src.ptr;
            }
            return *this;
        }

        iterator operator+(int step) const{
            iterator res = *this;
            for(int i = 0; i < step % ring->length; i++){
                res++;
            }
            return res;
        }

        iterator operator-(int step) const{
            iterator res = *this;
            for(int i = 0; i < step % ring->length; i++){
                res--;
            }
            return res;
        }

        iterator operator++(){
            next();
            if(ptr == ring->sentinel){
                next();
            }
            return *this;
        }

        iterator operator++(int){
            iterator temp = *this;
            next();
            if(ptr == ring->sentinel){
                next();
            }
            return temp;
        }

        iterator operator--(){
            prev();
            if(ptr == ring->sentinel){
                prev();
            }
            return *this;
        }

        iterator operator--(int){
            iterator temp = *this;
            prev();
            if(ptr == ring->sentinel){
                prev();
            }
            return temp;
        }

        iterator next(){
            if(ptr == nullptr){
                throw runtime_error("Iterator is null");
            }
            ptr = ptr->next;
            return *this;
        }

        iterator get_next(){
            if(ptr == nullptr){
                throw runtime_error("Iterator is null");
            }
            return iterator(ptr->next, ring);
        }

        iterator prev(){
            if(ptr == nullptr){
                throw runtime_error("Iterator is null");
            }
            ptr = ptr->prev;
            return *this;
        }

        iterator get_prev(){
            if(ptr == nullptr){
                throw runtime_error("Iterator is null");
            }
            return iterator(ptr->prev, ring);
        }

        KeyT &key() const{
            return ptr->key;
        }

        InfoT &info() const{
            return ptr->info;
        }
    };

    unsigned int length;

    Node* sentinel;

public:
    typedef iterator<Key, Info, bi_ring> mod_iterator;
    typedef iterator<const Key, const Info, bi_ring> const_iterator;

    bi_ring() : length(0)
    {
        sentinel = new Node(Key(), Info(), nullptr, nullptr);
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }
    bi_ring(const bi_ring &src) : length(0)
    {
        sentinel = new Node(Key(), Info(), nullptr, nullptr);
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        *this = src;
    }
    ~bi_ring()
    {
        clear();
        delete sentinel;
    }
    bi_ring &operator=(const bi_ring &src)
    {
        if (this != &src)
        {
            clear();
            // Copy elements from src to this using iterators
            for (auto it = src.cbegin(); it != src.cend(); it.next())
            {

                push_back(it.key(), it.info());
            }
        }
        return *this;
    }


    [[nodiscard]] unsigned int getLength() const{
        return length;
    }

    [[nodiscard]] bool isEmpty() const{
        return length == 0;
    }

    bool operator==(const bi_ring& other) const {
        if (length != other.length) {
            return false;
        }

        auto thisIt = cbegin();
        auto otherIt = other.cbegin();

        for (; thisIt != cend(); thisIt.next(), otherIt.next()) {
            if (thisIt.key() != otherIt.key() || thisIt.info() != otherIt.info()) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const bi_ring& other) const {
        return this != other;
    }

    /**
     * Inserts a new element with the provided key and info before the specified node
     * on which iterator os pointing at.
     *
     * @param position Iterator pointing on node before which the new node has to be inserted
     * @param key The key of the new element to insert.
     * @param info The info of the new element to insert.
     * @return iterator pointing on inserted node
     */
    mod_iterator insert(const_iterator position, const Key &key, const Info &info)
    {
        Node *newNode = new Node(key, info, nullptr, nullptr);

        Node *positionNode = position.ptr;
        newNode->next = positionNode;
        newNode->prev = positionNode->prev;
        positionNode->prev->next = newNode;
        positionNode->prev = newNode;

        length++;

        return mod_iterator(newNode, this);
    }

    /**
     * Removes the specified element.
     *
     * @param position constant iterator pointing on element to be erased.
     * @return mod_iterator pointing on next element after deleted
     */
    mod_iterator erase(const_iterator position)
    {
        if (position == cend())
        {
            // If the iterator points to the end, nothing to erase
            return end();
        }

        Node *eraseNode = position.ptr;
        Node *nextNode = eraseNode->next;

        eraseNode->prev->next = eraseNode->next;
        eraseNode->next->prev = eraseNode->prev;

        delete eraseNode;

        length--;

        return mod_iterator(nextNode, this);
    }

    void clear(){
        while(!isEmpty()){
            pop_back();
        }
    }

    /**
      * Searches for the specified element of a given key.
      *
      * @param [out] it is modifying iterator pointing on found element
      * @param key The key to search for.
      * @param search_from iterator pointing on element from which start searching
      * @param search_till iterator pointing on element until which element to search
      * @return true if element found
      * @return false if element not found
     */
    template <typename iterator>
    bool find_key(iterator &it, const Key &key, iterator &search_from, iterator &search_till) const {
        for (; search_from != search_till; search_from.next()){
            if (search_from.ptr == sentinel){
                continue;
            }
            if (search_from.key() == key){
                it = search_from;
                return true;
            }
        }
        return false;
    }

    /**
     * @brief number of occurrences of key
     *
     * @param key is key which occurrences we count
     * @return unsigned int number of occurrences of key
     */
    unsigned int occurrencesOf(const Key &key) const
    {
        unsigned int counter = 0;
        for (auto it = cbegin(); it != cend(); it.next())
        {
            if (it.key() == key)
            {
                counter++;
            }
        }
        return counter;
    }

    /**
     * @brief inserts element in the beginning of the ring
     *
     * @param key is the key that will be inserted
     * @param info is info that will be inserted
     * @return iterator pointing on inserted element
     */
    mod_iterator push_front(const Key &key, const Info &info)
    {
        return insert(cbegin(), key, info);
    }

    /**
     * @brief inserts element in the end of the ring
     *
     * @param key is the key that will be inserted
     * @param info is info that will be inserted
     * @return iterator pointing on inserted element
     */
    mod_iterator push_back(const Key &key, const Info &info)
    {
        return insert(cend(), key, info);
    }

    /**
     * @brief erases first element in the ring
     *
     * @return Iterator pointing on next element after deleted
     */
    mod_iterator pop_front()
    {
        return erase(cbegin());
    }

    /**
     * @brief erases last element in the ring
     *
     * @return Iterator pointing on next element before deleted
     */
    mod_iterator pop_back()
    {
        return --erase(--cend());
    }

    /**
     * @brief returns iterator pointing on first element of the ring
     *
     * @return mod_iterator
     */
    mod_iterator begin()
    {
        return mod_iterator(sentinel->next, this);
    }

    /**
     * @brief returns constant iterator pointing on first element of the ring
     *
     * @return constant_iterator
     */
    const_iterator cbegin() const
    {
        return const_iterator(sentinel->next, this);
    }

    /**
     * @brief returns iterator pointing on the last element of the ring
     *
     * @return mod_iterator
     */
    mod_iterator end()
    {
        return mod_iterator(sentinel, this);
    }

    /**
     * @brief return the constant iterator pointing on the last element of the ring
     *
     * @return const_iterator
     */
    const_iterator cend() const
    {
        return const_iterator(sentinel, this);
    }

};

template <typename Key, typename Info>
std::ostream& operator<<(std::ostream& os, const bi_ring<Key, Info>& ring) {
    os << "{ ";
    bool first = true;
    for (auto it = ring.cbegin(); it != ring.cend(); it.next()) {
        if (!first) {
            os << ", ";
        }
        os << it.key() << " = " << it.info();
        first = false;
    }
    os << " }";
    return os;
}

template <typename Key, typename Info>
bi_ring<Key, Info> filter(const bi_ring<Key, Info> &source, bool (*pred)(const Key &)){
    bi_ring<Key, Info> result;

    for (auto it = source.cbegin(); it != source.cend(); it.next()){
        if (pred(it.key())){
            result.push_back(it.key(), it.info());
        }
    }

    return result;
}

template <typename Key, typename Info>
Info sum_info(const Key &, const Info &i1, const Info &i2){
    return i1 + i2;
}

template <typename Key, typename Info>
bi_ring<Key, Info> join(const bi_ring<Key, Info> &first, const bi_ring<Key, Info> &second){
    bi_ring<Key, Info> pre_result = first;

    for (auto it = second.cbegin(); it != second.cend(); it.next()) {
        pre_result.push_back(it.key(), it.info());
    }

    return unique(pre_result, sum_info<Key, Info>);
}

template <typename Key, typename Info>
bi_ring<Key, Info> unique(const bi_ring<Key, Info> &src, Info (*aggregate)(const Key &, const Info &, const Info &)) {
    bi_ring<Key, Info> result;

    for (auto it = src.cbegin(); it != src.cend(); it.next()) {
        auto search_res = result.cbegin();
        auto sf = result.cbegin();
        auto st = result.cend();
        Key key = it.key();

        if (result.find_key(search_res, key, sf, st)) {
            continue;
        }

        auto searching_it = src.cbegin();
        auto search_from = it.get_next();
        auto search_till = src.cend();
        Info new_info = it.info();

        while (src.find_key(searching_it, key, search_from, search_till)) {
            new_info = aggregate(key, new_info, searching_it.info());
            search_from.next();
        }

        result.push_back(key, new_info);
    }

    return result;
}

template <typename Key, typename Info>
bi_ring<Key, Info> shuffle(const bi_ring<Key, Info> &first, unsigned int fcnt, const bi_ring<Key, Info> &second, unsigned int scnt, unsigned int reps){
    bi_ring<Key, Info> result;

    auto first_it = first.cbegin();
    auto second_it = second.cbegin();

    for (unsigned int rep = 0; rep < reps; rep++) {
        for (unsigned int i = 0; i < fcnt; i++, first_it++) {
            result.push_back(first_it.key(), first_it.info());
        }

        for (unsigned int i = 0; i < scnt; i++, second_it++) {
            result.push_back(second_it.key(), second_it.info());
        }
    }

    return result;
}

#endif