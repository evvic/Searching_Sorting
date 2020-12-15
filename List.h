#ifndef	LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "Utility.h"

template <class Node_entry>
struct Node {
    //  data members
    Node_entry entry;
    Node<Node_entry>* next;
    Node<Node_entry>* back;
    //  constructors
    Node() {
        entry = NULL;
        next = NULL;
        back = NULL;
    }
    Node(Node_entry e, Node<Node_entry>* link_back = NULL,
        Node<Node_entry>* link_next = NULL) {
        entry = e;
        next = link_next;
        back = link_back;
    }
};


template <class List_entry>
class List {
public:
    List() {
        cout << "List.h" << endl;
        current_position = count = 0;
        current = head = NULL;
    }
    List(List_entry& x) {
        current_position = count = 0;
        current = head = NULL;

        head->entry = x;
        count++;
    }

    bool clear() {
        List_entry buffer;
        while (remove(0, buffer) == success);
        return true;
    }
    bool empty() { return (count == 0) ? true : false; }
    bool full() { return (MAXSIZE == count) ? true : false; }
    
    Error_code replace(int pos, const List_entry& x) {
        if (pos > count) return overflow;
        if (pos < 0) return underflow;
        if (count == 0) return underflow;

        Node<List_entry>* temp = new Node<List_entry>;
        temp = head;

        for (int i = 0; i < pos; i++) {
            temp = temp->next;
        }

        temp->entry = x;

        return success;

    }

    void setHead(Node<List_entry>* h) { head = h; }
    Node<List_entry>* getHead() { return head; }
    int getCount() { return count; }
    int size() const { return count; }

    Error_code retrieve(int pos, List_entry& x) const {
      
        if (pos == 0) {
            x = head->entry;
            return success;
        }
        else {
            cout << "Retrieve else: ";
            Node<List_entry>* temp = new Node<List_entry>;
            temp = head;

            for (int i = 0; i < pos; i++) {
                cout << " temp->entry: " << temp->entry;
                temp = temp->next;
            }

            x = temp->entry;
            //cout << "temp->entry: " << temp->entry << "\tx: " <<  endl;
            //delete temp;
            return success;
        }
        return fail;
    }

    Error_code insert(const List_entry& x) {
        //inserting at tail position
        int position = count;
        if (count > MAXSIZE) return overflow;
        if (position < 0) return underflow;
        if (position > count) return overflow;

        Node<List_entry>* new_node = new Node<List_entry>;



        if (position == 0) {
            if (count == 0) {
                new_node->entry = x;
                new_node->back = NULL;
            }
            new_node->next = 0;
            // following = current;
            head = new_node;
        }
        else {
            //if inserting at tail of list
            set_position(position - 1);
            current->next = new_node;
            new_node->back = current;
            new_node->next = NULL;
        }
     

        new_node->entry = x;

        current = new_node;
        current_position = position;
        count++;
        return success;
    }

    Error_code insert(int position, const List_entry& x) {
        if (count > MAXSIZE) return overflow;
        if (position < 0) return underflow;
        if (position > count) return overflow;

        Node<List_entry>* new_node = new Node<List_entry>;

        

        if (position == 0) {
            if (count == 0) {
                new_node->entry = x;
                new_node->back = NULL;
            }
            new_node->next = 0;
            // following = current;
            head = new_node;
        }
        else if (position == count) {
            //if inserting at tail of list
            set_position(position - 1);
            current->next = new_node;
            new_node->back = current;
            new_node->next = NULL;
        }
        else {
            //if inserting in middle of list
            set_position(position - 1);
            new_node->back = current;
            new_node->next = current->next;

        }

        new_node->entry = x;

        current = new_node;
        current_position = position;
        count++;
        return success;
    }

    

    Error_code remove(int position, List_entry& x) {
        /*
    * position 0 -> count
    * current_position 0 -> count
    */
        if (count > MAXSIZE) return overflow;
        if (position < 0) return underflow;
        if (position > count) return overflow;

        Node<List_entry>* temp = new Node<List_entry>;

        temp = head;

        if (position == 0) {
            if (head->next == NULL) return overflow;
            head = head->next;
        }
        else {
            for (int i = 0; i < position; i++) {
                temp = temp->next;
            }

            Node<List_entry>* nearhead = new Node<List_entry>;
            Node<List_entry>* neartail = new Node<List_entry>;

            nearhead = temp->back;
            neartail = temp->next;

            nearhead->next = neartail;
            neartail->back = nearhead;
        }

        x = temp->entry;

        current = temp->next;

        delete temp;
        count--;

        return success;
    }

    Error_code traverse(int start, int end) {
        if (start == end) return duplicate_error;
        if (count < 1) return underflow;
        if (start > count || end > count) return overflow;
        if (start < 0 || end < 0) return underflow;

        Node<List_entry>* temp = new Node<List_entry>;
        temp = head;
        std::cout << "temp = " << temp->entry;
        std::cout << " temp.next: " << temp->next->entry << endl;

        for (int i = 0; i < start; i++) {
            temp = temp->next;
        }

        if (start < end) {
            //traverse forward
            for (int i = 0; i < end - start; i++) {
                std::cout << temp->entry << ' ';
                temp = temp->next;
            }
        }
        else if (end < start) {
            //trverse backward
            for (int i = 0; i < start - end; i++) {
                std::cout << temp->entry << ' ';
                temp = temp->back;
            }
        }
        else { return fail; }

        current = temp;

        delete temp;

        return success;
    }

    Error_code median(List_entry& x) {
        Node<List_entry>* temp = new Node<List_entry>;
        temp = head;

        if (count == 0) return underflow;
        if (count == 1) {
            x = temp->entry;
            return success;
        }

        if (count % 2 == 0) {
            for (int i = 0; i < (count / 2); i++) {
                temp = temp->next;
            }
            x = temp->entry;
        }
        else {
            for (int i = 0; i < (count / 2) + 1; i++) {
                temp = temp->next;
            }
            x = temp->entry;
        }

        return success;

    }

    Error_code reverse() {
        if (count == 0 || count == 1) return underflow;

        /*while (head->next != NULL) {
            head = head->next;
            std::cout << " head " << head->entry << ' ';
        }*/

        List<List_entry> tlist;
        Node<List_entry>* temp = new Node<List_entry>;
        temp = head;

        List_entry buffer;
        for (int i = 0; remove(0, buffer) == success; i++) {
            tlist.insert(i, buffer);
        }

        for (int i = 0; tlist.remove(0, buffer) == success; i++) {
            insert(0, buffer);
        }

        return success;
    }

    Error_code interchange(int pos1, int pos2) {
        if (pos1 > count || pos2 > count) return overflow;
        if (pos1 == pos2) return duplicate_error;
        if (pos1 < 0 || pos2 < 0) return underflow;
        if (count < 2) return underflow;

        Node<List_entry>* switcher = new Node<List_entry>;
        Node<List_entry>* temp1 = new Node<List_entry>;
        Node<List_entry>* temp2 = new Node<List_entry>;

        temp1 = temp2 = head;

        for (int i = 0; i < pos1; i++) {
            temp1 = temp1->next;
        }
        for (int i = 0; i < pos2; i++) {
            temp2 = temp2->next;
        }

        switcher->entry = temp1->entry;
        temp1->entry = temp2->entry;
        temp2->entry = switcher->entry;

        return success;
    }


    //operator overloading, these templates must be in the header

    List& operator = (const List& rhs) {
        const Node<List_entry>* cpcurrent = rhs.head;
        Node<List_entry>* lscurrent = new Node<List_entry>;
        lscurrent = NULL;

        if (cpcurrent != NULL) {
            this->head = new Node<List_entry>;
            head->entry = cpcurrent->entry;
            lscurrent = head;
            cpcurrent = cpcurrent->next;
        }

        while (cpcurrent != NULL) {
            Node<List_entry>* new_node = new Node<List_entry>;
            new_node->entry = cpcurrent->entry;
            lscurrent = lscurrent->next;
            cpcurrent = cpcurrent->next;
        }

        this->count = rhs.count;

        return *this;
    }

    List& operator + (const List& rhs) {
        const Node<List_entry>* cpcurrent = rhs.head;

        this->current = this->head;
        while (current->next != NULL) {
            this->current = this->current->next;
            //cout << this->current->entry;
        }

        //this->current is now at this->tail

        while (cpcurrent != NULL) {
            this->count++;
            Node<List_entry>* new_node = new Node<List_entry>;
            this->current->next = new_node;

            new_node->back = this->current;
            new_node->entry = cpcurrent->entry;

            this->current = this->current->next;
            cpcurrent = cpcurrent->next; //incrememnt
        }

        return *this;
    }

protected:
    //  Data members for the doubly-linked list implementation follow:
    int count;
    const int MAXSIZE = 15;
    mutable int current_position;
    mutable Node<List_entry>* current;

    Node<List_entry>* head;

    //  The auxiliary function to locate list positions follows:
    void set_position(int position) const {
    /*
        Pre:  position is a valid position in the List: 0 <= position < count.
        Post: The current Node pointer references the Node at position.
    */
        if (current_position <= position)
            for (; current_position != position; current_position++)
                current = current->next;
        else
            for (; current_position != position; current_position--)
                current = current->back;
    }
};


#endif