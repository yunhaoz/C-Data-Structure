#include "llrec.h"

//*********************************************
// Provide your implementation of llpivot below
//*********************************************

void llpivot(Node*& head, Node*& smaller, Node*& larger, int pivot)
{
	if (head == NULL)
	{
		return;
	}
	else
	{
		if (head->val <= pivot)
		{
			smaller = head;
			llpivot(head->next, smaller->next, larger, pivot);
			head = NULL;
		}
		else
		{
			larger = head;
			llpivot(head->next, smaller, larger->next, pivot);
			head = NULL;
		}
	}
}
