
#include "Utility.h"
#include "List.h"
#include "TIMER.H"
#include "RANDOM.H"
#include <iostream>
#include <conio.h>
#include <iomanip>

/*
* folder location:
* cd desktop/Programming 3/SearchingSorting_Project_3/SearchingSorting_Project_3/Searching_Sorting
* 
    There is a large bug using the Record class.
    Although it builds, the List member funcitons don't work with the Record datatype.
    Most likely have to use the typedef int Record while in the List class
    But may still have to use the record class when overload casting to Key

    Maybe change class member function?

    error line 318
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
    Key(int x = 0) {
        key = x;
    }
    int the_key() const { return key; }
    void getKey(int x) { key = x; }
};

int Key::comparisons = 0;

//  Definition of a Record class:
/*
class Record {
public:
    Record() { value = 0; }
    Record(record v) { value = v; cout << "constructor " << value << endl; }

    //  implicit conversion from Record to Key.
    operator Key() const { return Key(value); }

    Record& operator = (const Record& r) {
        this->value = r.value;
        return *this;
    }
    Record& operator = (const record x) {
        this->value = x;
        return *this;
    }
    int operator << (const Record&) {
        return this->value;
    }
    record getRec() const { return value; }
    void setRec(record v) { value = v; }
  
private:
    record value;
    //  Add data components.
};
*/

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


void test_seq_search(int searches, List<Record>& the_list, int userkey);
void test_bin_search(int searches, Ordered_list& the_list, int userkey);
Error_code sequential_search(const List<Record>& the_list, const Key& target, int& position);

Error_code recursive_binary_1(const Ordered_list& the_list, const Key& target, int bottom, int top, int& position);
Error_code run_recursive_binary_1(const Ordered_list& the_list, const Key& target, int& position);
/*
Error_code binary_search_1(const Ordered_list& the_list, const Key& target, int& position);

Error_code recursive_binary_2(const Ordered_list& the_list, const Key& target, int bottom, int top, int& position);
Error_code run_recursive_binary_2(const Ordered_list& the_list, const Key& target, int& position);
Error_code binary_search_2(const Ordered_list& the_list, const Key& target, int& position);
*/

void print_out(string s, double t, int comparissons, int searches);
void populate_list(List<Record>& the_list, int size); //always populate with odd
void populate_list(Ordered_list& the_list, int size); //op with odd
preferences select_preferences();

//even Keys (0, 2, ... 2n) should always fail
//while odd keys (1, 3, ... 2n + 1) should be successful (unless too high or low)

int main() {
    
    preferences user = select_preferences();

    //0 == sequential search
    if (user.searchMethods[0]) {
        List<Record> testme;
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
        //if (user.chooseKey) { test_bin_search(user.numsearches, testme2, user.key); }
        //else { test_bin_search(user.numsearches, testme2, -1); }
        //function for binary search
    }

    
    
    //test_search(user.numsearches, testme2);
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
        cout << "calling recursive_binary" << endl;
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
        if (data == target) return success;
    }
    //cout << endl;               ////////
    position = NULL;
    return not_present;
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
    cout << "Ordered_list::insert(Record) size = " << size() << " data = " << data;
    int s = size();
    int position;
    /* teacer's code
    for (position = 0; position < s; position++) {
        Record list_data;
        retrieve(position, list_data);
        cout << " list_data = " << list_data << endl;
        if (data >= list_data) break;
    }
    */

    //cout << "position before return p = " << position << endl;
    return List<Record>::insert(size(), data);
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
    cout << "recursive_binary_1()" << endl;
    Record data;
    if (bottom < top) {              // List has more than one entry.
        int mid = (bottom + top) / 2;
        the_list.retrieve(mid, data);
        cout << "data: " << data << "\tmid: " << mid << endl;
        if (data < target)  // Reduce to top half of list.
            return recursive_binary_1(the_list, target, mid + 1, top, position);
        else                          // Reduce to bottom half of list.
            return recursive_binary_1(the_list, target, bottom, mid, position);
    }
    else if (top < bottom) {
        cout << "recursive_binary_1() : not present" << endl;
        return not_present;           // List is empty.
    }

      
    else {                           // List has exactly one entry.
        cout << "recursive_binary_1 else " << endl;
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

/*
Error_code binary_search_1(const Ordered_list& the_list, const Key& target, int& position)
    /*
    Post: If a Record in the_list  has Key equal to target, then position locates
          one such entry and a code of success is returned.
          Otherwise, not_present is returned and position is undefined.
    Uses: Methods for classes List and Record.
    
{
    Record data;
    int bottom = 0, top = the_list.size() - 1;

    while (bottom < top) {
        int mid = (bottom + top) / 2;
        the_list.retrieve(mid, data);
        if (data < target)
            bottom = mid + 1;
        else
            top = mid;
    }
    if (top < bottom) return not_present;
    else {
        position = bottom;
        the_list.retrieve(bottom, data);
        if (data == target) return success;
        else return not_present;
    }
}


Error_code recursive_binary_2(const Ordered_list& the_list, const Key& target, int bottom, int top, int& position)
    /*
    Pre:  The indices bottom to top define the
          range in the list to search for the target.
    Post: If a Record in the range from bottom to top in the_list
          has key equal to target, then position locates
          one such entry, and a code of success is returned.
          Otherwise, not_present is returned, and position is undefined.
    Uses: recursive_binary_2, together with methods from the classes
          Ordered_list and Record.
    
{
    Record data;
    if (bottom <= top) {
        int mid = (bottom + top) / 2;
        the_list.retrieve(mid, data);
        if (data == target) {
            position = mid;
            return success;
        }

        else if (data < target)
            return recursive_binary_2(the_list, target, mid + 1, top, position);
        else
            return recursive_binary_2(the_list, target, bottom, mid - 1, position);
    }
    else return not_present;
}


Error_code run_recursive_binary_2(const Ordered_list& the_list, const Key& target, int& position)
{
    return recursive_binary_2(the_list, target, 0, the_list.size() - 1, position);
}


Error_code binary_search_2(const Ordered_list& the_list, const Key& target, int& position)
    /*
    Post: If a Record in the_list has key equal to target, then position locates
          one such entry and a code of success is returned.
          Otherwise, not_present is returned and position is undefined.
    Uses: Methods for classes Ordered_list and Record.
    
{
    Record data;
    int bottom = 0, top = the_list.size() - 1;
    while (bottom <= top) {
        position = (bottom + top) / 2;
        the_list.retrieve(position, data);
        if (data == target) return success;
        if (data < target) bottom = position + 1;
        else top = position - 1;
    }
    return not_present;
}

*/



void populate_list(List<Record>& the_list, int size) {
    //always populate with odd integers
    for (int i = 1; i < size * 2; i += 2) {
        Record temp;
        cout << "in populate_list() " << i << endl;
        if (the_list.insert(temp) == overflow) {
            cout << "ERROR: List is full, populating has ended." << endl;
            continue;
        }
        the_list.traverse(0, the_list.size());
    }
}


void populate_list(Ordered_list& the_list, int size) {
    //always populate with odd integers
    for (int i = 1; i < size * 2; i += 2) {
        Record temp = i;
        cout << "in (ordered) populate_list() " << i << endl;
        if (the_list.insert(temp) == overflow) {
            cout << "ERROR: List is full, populating has ended." << endl;
            continue;
        }
        
    }
    cout << "traversing to check..." << endl;
    the_list.traverse(0, the_list.size());
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
