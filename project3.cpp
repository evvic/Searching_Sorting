#include "TIMER.H"
#include "Utility.h"
#include "List.h"
#include "RANDOM.H"
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <queue>

/*
* folder location:
* cd desktop/Programming 3/SearchingSorting_Project_3/SearchingSorting_Project_3/Searching_Sorting
* 
* NEXT STEP
* 
* when comparing sort methods,
* operations = num of comparrisons + num of assignments
* 
* for part 5, add the option to save the data to an csv sheet
* 
* add ONE more sort function (class n^2)
*  -> might have to make new copies of each sort and implement with List
* 
* then make a test function displaying results
* 
* idea: for the last step sorting the array with performance, use the oen same array
*       but dont pass as reference in the origina call of each sort function
*/

using namespace std;

const int SEARCHMETHODS = 2;

struct preferences {
    int listSize;       //user chooses size of the list to be populated
    int numsearches;    //user chooses number of times a search method is used
    bool chooseKey;     //user chooses if they choose the key or it's generated
    int key;            //chosen key
    bool searchMethods[SEARCHMETHODS]; //[0] = sequential search, [1] = binary search
    bool sortMethods[1];
};

//typedef int Key;
typedef int Record;

//typedef Key Record;
//  Definition of a Key class:
class Key {
private:
    int key;
public:
    static int comparisons;
    static int assignments; //for sorting
    Key(int x = 0) {
        key = x;
        assignments++;
    }
    int the_key() const { return key; }
    int assign_key() const{
        assignments++;
        return key;
    }
    void setKey(int x) { key = x; }

    int operator = (Key& k) {
        assignments++;
        this->key = k.the_key();
        return this->key;
    }
    /*int operator = (int k) {
        assignments++;
        this->key = k;
        return this->key;
    }*/
    Key& operator = (int k) {
        assignments++;
        this->key = k;
        return *this;
    }
 
};

int Key::comparisons = 0;
int Key::assignments = 0;

//exension of created List class
class Ordered_list: public List<Record> {
public:
    Ordered_list() : List() {
        cout << "Ordered_list" << endl;
    }
    Ordered_list(Record& x) : List(x) {
        cout << "Ordered_list" << endl;
    }
    Error_code insert(const Record& data);
    Error_code insert(int position, const Record& data);
    Error_code replace(int position, const Record& data);
    //Error_code retrieve(const int position, Record& data) const;
};

//  Declare overloaded comparison operators for keys.
bool operator ==(const Key& x, const Key& y);
bool operator > (const Key& x, const Key& y);
bool operator < (const Key& x, const Key& y);
bool operator >=(const Key& x, const Key& y);
bool operator <=(const Key& x, const Key& y);
bool operator !=(const Key& x, const Key& y);

//Search functions
void test_seq_search(int searches, List<Record>& the_list, int userkey);
void test_bin_search(int searches, Ordered_list& the_list, int userkey);
void performance_comparison(int searches, List<Record>& seq_list, Ordered_list& bin_list);
Error_code sequential_search(const List<Record>& the_list, const Key& target, int& position);
Error_code recursive_binary_1(const Ordered_list& the_list, const Key& target, int bottom, int top, int& position);
Error_code run_recursive_binary_1(const Ordered_list& the_list, const Key& target, int& position);

//Sort functions
void insertion_sort(Record* table, int size);
void merge_sort(Record* table, int l, int r); //(l)eft & (r)ight indexes of subarray. RIGHT index == size - 1!!!
void merge(Record* table, int l, int m, int r); //(l)eft, (m)iddle & (r)ight indexes of subarray. n*log(n)
void quick_sort(Record* table, int l, int r);
int partition(Record* table, int l, int r);
void selection_sort(Record* table, int size); //(O)n^2

//Populating functions
void populate_list(List<Record>& the_list, int size); //always populate with odd
void populate_list(Ordered_list& the_list, int size); //op with odd
void randomly_populate_list(List<Record>& the_list, int size); //[0 - 10,000] range of random values
void randomly_populate_table(Record* table, int size); //[0 - 10,000] range of random values into int array

//Misc
void print_out(string s, double t, int comparissons, int searches);
void table_slice(int* table, int size); //displays a slice of the table, 200 max shown values
preferences select_preferences();
//swap() function in Utility.h

//even Keys (0, 2, ... 2n) should always fail
//while odd keys (1, 3, ... 2n + 1) should be successful (unless too high or low)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN BELOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main() {
    
    preferences user = select_preferences();
    /*
    //0 == sequential search
    if (user.searchMethods[0]) {
        cout << "sequentail if{}" << endl;
        List<Record> testme;
        cout << "sequential before populate" << endl;
        populate_list(testme, user.listSize);
        if (user.chooseKey) { test_seq_search(user.numsearches, testme, user.key); }
        else { test_seq_search(user.numsearches, testme, -1); }
        //function for seq search
    }
    //1 == binary search
    if (user.searchMethods[1]) {
        Ordered_list testme2; //
        cout << "about to populate" << endl;
        populate_list(testme2, user.listSize);
        cout << "finished populating list" << endl;
        if (user.chooseKey) { test_bin_search(user.numsearches, testme2, user.key); }
        else { test_bin_search(user.numsearches, testme2, -1); }
        //function for binary search
    }
    */

    //Performance comparison info
    List<Record> testme;
    populate_list(testme, user.listSize);
    Ordered_list testme2; //
    populate_list(testme2, user.listSize);

    //Sorting

    const int amount = 1000;

    int table[amount]; //default should be 1000


    randomly_populate_table(table, amount);

    cout << endl;
    table_slice(table, amount);

    Key test;

    //insertion_sort(table, 1000);
    //merge_sort(table, 0, amount - 1);
    //quick_sort(table, 0, amount - 1);
    selection_sort(table, amount);

    table_slice(table, amount);

    cout << endl << "comparissons: " << Key::comparisons;
    cout << endl << "assignments: " << Key::assignments;

    //performance_comparison(user.numsearches, testme, testme2);

    
    
    //test_search(user.numsearches, testme2);
}

void performance_comparison(const int searches, List<Record>& seq_list, Ordered_list& bin_list) {
    /*
    * both searches should be run 10 times, with same size & order of list
    * the randomly generated key should be the same for both, changes each search 
    */
    cout << "\tPerformance Comparison" << endl;
    if (seq_list.size() != bin_list.size()) {
        cout << "ERROR. Comparison should have equal sized lists" << endl;
        return;
    }
    if (searches < 10) {
        cout << "ERROR. The number of searches must be positive & greater than 10 for performance comparison." << endl;
        return;
    }

    int list_size = seq_list.size();
    string status = "Successful"; //maybe not good practice
    queue<int> targets; //maybe use queue, collect the 10 used target keys for seq and use same ones in order for bin
    int found_at, target;
    Key::comparisons = 0;
    Random number;
    Timer clock;
    double seq_time, bin_time;

    cout << endl << "Error: Failed to find expected target(s) ";
    for (int i = 0; i < searches; i++) {
        //generating random key (odd or even):
        target = number.random_integer(0, list_size * 2) + 1; // [1 - 2 * list_size]
        targets.push(target); //save keys for bin search (same order)
        if (sequential_search(seq_list, target, found_at) == not_present) {
            cout <<  target << ' ';
            status = "Unsuccessful";
        }
    }
    
    seq_time = clock.elapsed_time();
    cout << endl << "Sequential Search results." << endl;
    print_out(status, seq_time, Key::comparisons, searches);
    clock.reset();
    status = "Successful";

    cout << endl << "Error: Failed to find expected target(s) ";
    for (int i = 0; i < searches; i++) {
        if (run_recursive_binary_1(bin_list, (const Key)targets.front(), found_at) == not_present)
             cout << targets.front() << ' ';
        targets.pop();
    }
    bin_time = clock.elapsed_time();
    cout << endl << "Binary Search results." << endl;
    print_out("Successful", bin_time, Key::comparisons, searches);

    //can use bin_time and seq_time to compare
}


void test_seq_search(int searches, List<Record>& the_list, int userkey)
/*

Pre:  None.
Post: The number of key comparisons and CPU time for a
      sequential searching function have been calculated.
Uses: Methods of the classes List, Random, and Timer,
      together with an output function print_out
*/
{
    cout << "\tSequential Search" << endl;
    int list_size = the_list.size();
    if (searches <= 0 || list_size < 0) {
        cout << " Exiting test: " << endl
            << " The number of searches must be positive." << endl
            << " The number of list entries must exceed 0." << endl;
        return;
    }
    int i, target, found_at;
    Key::comparisons = 0;
    Random number;
    Timer clock;

    for (i = 0; i < searches; i++) {
        //setting key to user's option:
        if (userkey > -1) { target = userkey; }
        //generating random odd key:
        else { target = 2 * number.random_integer(0, list_size - 1) + 1; }
        if (sequential_search(the_list, target, found_at) == not_present)
            cout << "Error: Failed to find expected target " << target << endl;
    }
    print_out("Successful", clock.elapsed_time(), Key::comparisons, searches);

    Key::comparisons = 0;
    clock.reset();
    for (i = 0; i < searches; i++) {
        //setting key to user's option:
        if (userkey > -1) { target = userkey; }
        //generating random even key:
        else { target = 2 * number.random_integer(0, list_size); }
        if (sequential_search(the_list, target, found_at) == success)
            cout << "Error: Found unexpected target " << target
            << " at " << found_at << endl;
    }
    print_out("Unsuccessful", clock.elapsed_time(), Key::comparisons, searches);
}


void test_bin_search(int searches, Ordered_list& the_list, int userkey)
/*

Pre:  None.
Post: The number of key comparisons and CPU time for a
      sequential searching function have been calculated.
Uses: Methods of the classes List, Random, and Timer,
      together with an output function print_out
*/
{
    cout << "\tBinary Search" << endl;
    int list_size = the_list.size();
    //cout << "list_size: " << list_size << endl; ////
    if (searches <= 0 || list_size < 0) {
        cout << " Exiting test: " << endl
            << " The number of searches must be positive." << endl
            << " The number of list entries must exceed 0." << endl;
        return;
    }
    int i, target, found_at;
    Key::comparisons = 0;
    Random number;
    Timer clock;

    for (i = 0; i < searches; i++) {
        cout << "first for bin search" << endl;
        //setting key to user's option:
        if (userkey > -1) { target = userkey; }
        //generating random odd key:
        else { target = 2 * number.random_integer(0, list_size - 1) + 1; }
        //cout << "calling recursive_binary" << endl;
        if (run_recursive_binary_1(the_list, (const Key)target, found_at) == not_present)
            cout << "Error: Failed to find expected target " << target << endl;
    }
    print_out("Successful", clock.elapsed_time(), Key::comparisons, searches);

    Key::comparisons = 0;
    clock.reset();
    for (i = 0; i < searches; i++) {
        //setting key to user's option:
        if (userkey > -1) { target = userkey; }
        //generating random even key:
        else { target = 2 * number.random_integer(0, list_size); }

        if (run_recursive_binary_1(the_list, (const Key)target, found_at) == success)
            cout << "Error: Found unexpected target " << target
            << " at " << found_at << endl;
    }
    print_out("Unsuccessful", clock.elapsed_time(), Key::comparisons, searches);
}


bool operator ==(const Key& x, const Key& y) {
    Key::comparisons++;
    return x.the_key() == y.the_key();
}
bool operator > (const Key& x, const Key& y) {
    Key::comparisons++;
    return x.the_key() > y.the_key();
}
bool operator < (const Key& x, const Key& y) {
    Key::comparisons++;
    return x.the_key() < y.the_key();
}
bool operator >=(const Key& x, const Key& y) {
    Key::comparisons++;
    return x.the_key() >= y.the_key();
}
bool operator <=(const Key& x, const Key& y) {
    Key::comparisons++;
    return x.the_key() <= y.the_key();
}
bool operator !=(const Key& x, const Key& y) {
    Key::comparisons++;
    return x.the_key() != y.the_key();
}


Error_code sequential_search(const List<Record>& the_list, const Key& target, int& position)
    /*
    Post: If an entry in the_list has key equal to target, then return
          success and the output parameter position locates such an entry
          within the list.
          Otherwise return not_present and position becomes invalid.
    */
{
    //cout << "sequential_search, target = " << target.the_key() << endl;
    int s = the_list.size();
    for (position = 0; position < s; position++) {
        //cout << position << ' ';                ///////
        Record data;
        the_list.retrieve(position, data);
        //cout << " comparrisons: " << Key::comparisons;
        if (data == target) return success;
    }
    //cout << endl;               ////////
    position = NULL;
    return not_present;
}

void insertion_sort(Record* table, int size) {
    int j = 0;
    Key key;

    for (int i = 0; i < size; i++) {
        
        key = table[i]; //Key::comparisons++;
        j = i - 1;

        while (j >= 0 && key < table[j]) {
            Key temp(table[j]);
            table[j + 1] = temp.the_key();
            //Key::assignments++; 
            j--;
            
        }
        table[j + 1] = key.the_key();
    }
}

//(l)eft & (r)ight indexes of subarray
void merge_sort(Record* table, int l, int r) {
    if ((Key)l >= (Key)r) return; //recursively

    int m = (l + r - 1) / 2;
    merge_sort(table, l, m);    //first sub array
    merge_sort(table, m + 1, r);//second sub-array
    merge(table, l, m, r);      //merge sub-arrays together
}

//(l)eft, (m)iddle & (r)ight indexes of subarray
void merge(Record* table, int l, int m, int r) {
    const int size1 = m - l + 1;
    const int size2 = r - m;

    //dynamically allocate arrays
    Key* left = new Key[size1];
    Key* right = new Key[size2];

    //fill temp arrays                  Key   =  int
    for (int i = 0; i < size1; i++) { left[i] = table[l + i]; } 
    for (int i = 0; i < size2; i++) { right[i] = table[m + 1 + i]; }

    //index of 1st, 2nd, & merged sub-array
    int i = 0, j = 0, k = l;

    //merge the temp arrays into table
    while (i < size1 && j < size2) {
        if (left[i] <= right[j]) {
            table[k] = left[i].assign_key();
            i++;
        }
        else {
            table[k] = right[j].assign_key();
            j++;
        }
        k++;
    }

    //add any remaining values in temp arrays to table
    while (i < size1) { table[k++] = left[i++].assign_key(); }
    while (j < size2) { table[k++] = right[j++].assign_key(); }

    //deallocate arrays
    delete[] left;
    delete[] right;
}

void quick_sort(Record* table, int l, int r) {
    if ((Key)l < (Key)r) {
        int m = partition(table, l, r);

        //seperate quick sort at the partition index or (m)iddle
        quick_sort(table, l, m - 1);
        quick_sort(table, m + 1, r);
    }
}

int partition(Record* table, int l, int r) {
    Key pivot(table[r]);
    int i = (l - 1);

    for (int j = l; j <= r; j++) {
        if (table[j] < pivot) {
            i++;
            Key temp(table[i]);
            table[i] = table[j];
            table[j] = temp.assign_key();
        }
    }
    Key swap(table[i + 1]);
    table[i + 1] = table[r];
    table[r] = swap.assign_key();

    return i + 1;
}

void selection_sort(Record* table, int size) {
    int min;

    for (int i = 0; i < size - 1; i++) {
        min = i;
        for (int j = i + 1; j < size; j++) {
            if ((Key)table[j] < (Key)table[min]) min = j;
        }

        Key temp(table[min]);
        table[min] = table[i];
        table[i] = temp.assign_key();
    }
}

Error_code Ordered_list::insert(const Record& data)
/*
Post: If the Ordered_list is not full, the function succeeds:
      The Record data is inserted into the list, following the
      last entry of the list with a strictly lesser key (or in the
      first list position if no list element has a lesser key).
      Else: the function fails with the diagnostic Error_code overflow.
*/
{
    int s = size();
    int position;
    for (position = 0; position < s; position++) {
        Record list_data;
        retrieve(position, list_data);
        if (list_data >= data) break;
    }
    return List<Record>::insert(position, data);
}

Error_code Ordered_list::insert(int position, const Record& data)
/*
Post: If the Ordered_list is not full, 0 <= position <= n,
      where n is the number of entries in the list,
      and the Record data can be inserted at position in the list,
      without disturbing the list order, then the function succeeds:
      Any entry formerly in position and all later entries have their
      position numbers increased by 1 and data is inserted at position
      of the List.
      Else: the function fails with a diagnostic Error_code.
*/
{
    Record list_data;
    if (position > 0) {
        retrieve(position - 1, list_data);
        if (data < list_data)
            return fail;
    }
    if (position < size()) {
        retrieve(position, list_data);
        if (data > list_data)
            return fail;
    }
    return List<Record>::insert(position, data);
}

Error_code recursive_binary_1(const Ordered_list& the_list, const Key& target, int bottom, int top, int& position)
/*
Pre:  The indices bottom to top define the
      range in the list to search for the target.
Post: If a Record in the range of locations
      from bottom to top in the_list has key equal
      to target, then position locates
      one such entry and a code of success is returned.
      Otherwise, the Error_code of not_present is returned
      and position becomes undefined.
Uses: recursive_binary_1 and methods of the classes List and Record.
*/
{
    //cout << "recursive_binary_1()" << endl;
    Record data;
    if (bottom < top) {              // List has more than one entry.
        int mid = (bottom + top) / 2;
        the_list.retrieve(mid, data);
        //cout << "data: " << data << "\tmid: " << mid << endl;
        if (data < target)  // Reduce to top half of list.
            return recursive_binary_1(the_list, target, mid + 1, top, position);
        else                          // Reduce to bottom half of list.
            return recursive_binary_1(the_list, target, bottom, mid, position);
    }
    else if (top < bottom) {
        //cout << "recursive_binary_1() : not present" << endl;
        return not_present;           // List is empty.
    }

      
    else {                           // List has exactly one entry.
        //cout << "recursive_binary_1 else " << endl;
        position = bottom;
        the_list.retrieve(bottom, data);
        if (target == data) return success;
        else return not_present;
    }
}


Error_code run_recursive_binary_1(const Ordered_list& the_list, const Key& target, int& position)
{
    return recursive_binary_1(the_list, target, 0, the_list.size() - 1, position);
}


//sequential
void populate_list(List<Record>& the_list, int size) {
    //always populate with odd integers
    for (int i = 1; i < size * 2; i += 2) {
        Record temp = i;
        //cout << "in populate_list() " << i << endl;
        if (the_list.insert(temp) == overflow) {
            cout << "ERROR: List is full, populating has ended." << endl;
            continue;
        }
        
    }
    the_list.traverse(0, the_list.size());
}

//binary
void populate_list(Ordered_list& the_list, int size) {
    //always populate with odd integers
    for (int i = 1; i < size * 2; i += 2) {
        Record temp = i;
        //cout << "in (ordered) populate_list() " << i << endl;
        if (the_list.insert(temp) == overflow) {
            cout << "ERROR: List is full, populating has ended." << endl;
            continue;
        }
        
    }
    cout << "traversing to check..." << endl;
    the_list.traverse(0, the_list.size());
}

//[0 - 10,000] range of random values into int array
void randomly_populate_table(Record* table, int size) {
    Random guy(false);  //false sets seed to random
    for (int i = 0; i < size; i++) {
        table[i] = guy.random_integer(0, 10000);
    }
}




preferences select_preferences() {
    int limit = 15; //quickly change the max limit for the user
    preferences user;
    char temp;
    string searchNames[SEARCHMETHODS] = { "sequential search", "binary search" };

    cout << "Select search methods to test." << endl;
    for (int i = 0; i < SEARCHMETHODS; i++) {
        do {
            cout << "Use " << searchNames[i] << "? (y/n)\n\t> ";
            temp = _getche();
            temp = toupper(temp);
            cout << endl;
        } while (temp != 'N' && temp != 'Y');

        switch (temp) {
        case 'N':
            user.searchMethods[i] = false;
            break;
        case 'Y':
            user.searchMethods[i] = true;
            break;
        default:
            cout << "ERROR 1: User key choice wasn't clear." << endl;
            break;
        }
    }

    do {
        cout << "Enter desired list size [1-" << limit << "].\n\t> ";
        cin >> user.listSize;
        cout << endl;
    } while (user.listSize > limit || user.listSize <= 0);

    do {
        cout << "Enter how many times a search method will be used [1-" << limit << "].\n\t> ";
        cin >> user.numsearches;
        cout << endl;
    } while (user.numsearches > limit || user.numsearches <= 0);

    do {
        cout << "Do you want to enter your own key? (y/n)\n\t> ";
        temp = _getche();
        temp = toupper(temp);
        cout << endl;
    } while (temp != 'N' && temp != 'Y');

    switch (temp) {
    case 'N': 
        user.chooseKey = false;
        break;
    case 'Y': 
        user.chooseKey = true;
        break;
    default:
        cout << "ERROR 2: User key choice wasn't clear." << endl;
        break;
    }
    /*
    if (user.chooseKey) {
        do {
            cout << "Enter the key to be searched for [integer 0-" << limit << "]." << endl
                << "Expect even values to be unsuccessful." << endl
                << "\t> ";
            cin >> user.key;
            cout << endl;
        } while (user.key < 0 || user.key > (limit * 3));
    }
    */

    return user;
}

//    print_out("Unsuccessful", clock.elapsed_time(), Key::comparisons, searches);
void print_out(string s, double t, int comparissons, int searches) {

    cout << left << setw(13) << "Status" << setw(6) << "Time" << setw(13) << "Comparissons" << setw(9) << "Searches" << endl;
    cout << left << setw(13) << s << setw(6) << t << setw(13) << comparissons << setw(9) << searches << endl;
}

//prints out a slice of the table to check if it is currently sorted
void table_slice(int* table, int size) {
    int amount = 10;
    if (size > 200) amount = 200;       //if table is large, show max of 200 values
    else if (size <= 10) amount = size; //if the table is small just display the whole thing
    else amount = size / 2;             //size < 200, show just first half of values

    for (int i = 0; i < amount; i++) {
        cout << table[i] << ' ';
    }
    cout << endl;
}