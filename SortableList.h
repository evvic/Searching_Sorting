#ifndef	SORTABLELIST_H_INCLUDED
#define SORTABLELIST_H_INCLUDED

#include "List.h"
#include "Utility.h"

using namespace std;

template <class Record>
class Sortable_list :public List<Record> {
public:   //  Add prototypes for sorting methods here.

    void insertion_sort()
        /*
        Post: The entries of the Sortable_list have been rearranged so that
              the keys in all the entries are sorted into nondecreasing order.
        Uses: Methods for the class Record; the linked List implementation of Chapter 6.
        */
    {
        Node<Record>* first_unsorted,  //  the first unsorted node to be inserted
            * last_sorted,     //  tail of the sorted sublist
            * current,         //  used to traverse the sorted sublist
            * trailing;        //  one position behind current
        cout << "this->head" << this->head->entry << endl;

        if (this->head != NULL) {          //  Otherwise, the empty list is already sorted.
            last_sorted = this->head;       //  The first node alone makes a sorted sublist.
            cout << "last_sorted: " << last_sorted->entry << ' ' << last_sorted->next->entry << endl;
            //while (last_sorted->next != NULL && i < this->getCount()) {
            for(int i = 0; last_sorted->next != NULL; i++) {
                first_unsorted = last_sorted->next;
                cout << "first_unsorted->entry = " << first_unsorted->entry << endl;
                if (first_unsorted->entry < this->head->entry) {
                    //  Insert *first_unsorted at the head of the sorted list:
                    last_sorted->next = first_unsorted->next; //incrememnt nodes
                    first_unsorted->next = this->head;
                    Record temp = this->head->entry;
                    this->head->entry = first_unsorted->entry;
                    first_unsorted->entry = temp;
                }
                else {
                    //  Search the sorted sublist to insert *first_unsorted:
                    trailing = this->head;
                    current = trailing->next;
                    while (first_unsorted->entry > current->entry) {
                        trailing = current;
                        current = trailing->next; //
                    }

                    //  *first_unsorted now belongs between *trailing and *current.

                    if (first_unsorted == current)
                        last_sorted = first_unsorted;  //  already in right position
                    else {
                        last_sorted->next = first_unsorted->next;
                        first_unsorted->next = current;
                        trailing->next = first_unsorted; //
                    }
                }
            }
        }
    }
    //void shell_sort()
    /*
    void sort()
    {
        if the list has length greater than 1 {
            partition the list into lowlist, highlist;
            lowlist.sort();
            highlist.sort();
            combine(lowlist, highlist);
        }
    }
    */
    void merge_sort()
        /*
        Post: The entries of the sortable list have been rearranged so that
              their keys are sorted into nondecreasing order.
        Uses: The linked List implementation of Chapter 6 and recursive_merge_sort.
        */
    {
        recursive_merge_sort(this->head);
    }



private: //  Add prototypes for auxiliary functions here.
    Record entry[];

    int max_key(int low, int high)
        /*
        Pre:  low and high are valid positions in the Sortable_list and low <= high.
        Post: The position of the entry between low and high with the largest
              key is returned.
        Uses: The class Record, the contiguous List implementation of Chapter 6.
        */
    {
        int largest, current;
        largest = low;
        for (current = low + 1; current <= high; current++)
            if (entry[largest] < entry[current])
                largest = current;
        return largest;
    }
    void swap(int low, int high)
        /*
        Pre:  low and high are valid positions in the Sortable_list.
        Post: The entry at position low is swapped with the entry at position high.
        Uses: The contiguous List implementation of Chapter 6.
        */
    {
        Record temp;
        temp = entry[low];
        entry[low] = entry[high];
        entry[high] = temp;
    }

    void recursive_merge_sort(Node<Record>*& sub_list)
        /*
        Post: The nodes referenced by sub_list have been rearranged so that their
              keys are sorted into nondecreasing order.  The pointer parameter
              sub_list is reset to point at the node containing the smallest key.
        Uses: The linked List implementation of Chapter 6;
              the functions divide_from, merge, and recursive_merge_sort.
        */
    {
        if (sub_list != NULL && sub_list->next != NULL) {
            Node<Record>* second_half = divide_from(sub_list);
            recursive_merge_sort(sub_list);
            recursive_merge_sort(second_half);
            sub_list = merge(sub_list, second_half);
        }
    }




};

#endif
