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

        Node(const Key &_key, const Info &_info, Node *_next, Node *_prev)
            : key(_key), info(_info), next(_next), prev(_prev){}

        friend  class bi_ring;
    };

    template<typename KeyT, typename InfoT, typename Ring>
    class iterator {
    private:
        friend class bi_ring;

        Node *ptr;
        const Ring &ring;

        iterator(Node *ptr, const Ring &ring): ptr(ptr), ring(ring) {}

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
            for(int i = 0; i < step % ring.length; i++){
                res++;
            }
            return res;
        }

        iterator operator-(int step) const{
            iterator res = *this;
            for(int i = 0; i < step % ring.length; i++){
                res--;
            }
            return res;
        }

        iterator operator++(){
            next();
            if(ptr == ring.sentinel){
                next();
            }
            return *this;
        }

        iterator operator++(int){
            iterator temp = *this;
            next();
            if(ptr == ring.sentinel){
                next();
            }
            return temp;
        }

        iterator operator--(){
            prev();
            if(ptr == ring.sentinel){
                prev();
            }
            return *this;
        }

        iterator operator--(int){
            iterator temp = *this;
            prev();
            if(ptr == ring.sentinel){
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

        iterator prev(){
            if(ptr == nullptr){
                throw runtime_error("Iterator is null");
            }
            ptr = ptr->prev;
            return *this;
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


};

#endif