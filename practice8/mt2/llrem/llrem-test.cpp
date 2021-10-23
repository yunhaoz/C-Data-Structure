#include "llrem.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

// Convenience function to help you test your code if you like
template<class T>
void printList(Item<T>* list) {
    Item<T> * tmp = list;
    while(tmp != NULL) {
        cout << tmp->val << " ";
        tmp =tmp->next;
    }
    cout << endl;
}


// Will read all integers on the command line into a linked list
// e.g. ./llremove 2 5 7 1 will produce a linked list with 2 5 7 1
int main(int argc, char* argv[])
{
    Item<int>* head = NULL;
    Item<int>* curr;
    for(int i = 1; i < argc; i++) {
        if(i==1) head = curr = new Item<int>(atoi(argv[i]), NULL);
        else {
            curr->next = new Item<int>(atoi(argv[i]), NULL);
            curr = curr->next;
        }
    }

    cout << "Original list: " << endl;
    printList(head);

    isEven comp1;

    Item<int> *removed = removeCond(head,comp1);

    cout << "Remaining in original: " << endl;
    printList(head);
    cout << "Removed: " << endl;
    printList(removed);

    // Don't worry about freeing memory.
    // We know it is necessary to avoid memory leaks...
    return 0;

}
