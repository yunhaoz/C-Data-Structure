#include <iostream>
#include "todolist.h"
using namespace std;

void printPriorityList(const DailyTodoList& t)
{
    cout << "Priority items " << endl;
    for (size_t i = 0; i < t.numPriorityItems(); i++)
    {
        cout << t.getPriorityVal(i) << " ";
    }
    cout << endl;

}
void printDailyList(const DailyTodoList& t, size_t day)
{
    cout << "Items on day " << day << endl;
    size_t num = t.numItemsOnDay(day);
    for (size_t i = 0; i < num; i++)
    {
        cout << t.getNormalVal(day,i) << " ";
    }
    cout << endl;

}
int main()
{
    DailyTodoList t1;
    cout << "numDays = " << t1.numDays() << endl;
    cout << "numPriority = " << t1.numPriorityItems() << endl;
    t1.insert(6, 0, "6b", false);
    t1.push_back(6, "6c", true);
    t1.insert(6, 0, "6a", false);
    t1.insert(6, 3, "6d", false);
    t1.insert(2, 0, "2a", true);
    t1.insert(14, 0, "14a", true);
    t1.push_back(30, "14a", true);
    cout << "numDays = " << t1.numDays() << endl;
    cout << "numDays = " << t1.numItemsOnDay(6) << endl;
    cout << "numPriority = " << t1.numPriorityItems() << endl;
    printDailyList(t1, 6U);
    printPriorityList(t1);
    t1.remove(2, 0);
    printPriorityList(t1);
    t1.remove(14, 0);
    printPriorityList(t1);
    t1.remove(6, 2);
    printPriorityList(t1);
    return 0;
}

