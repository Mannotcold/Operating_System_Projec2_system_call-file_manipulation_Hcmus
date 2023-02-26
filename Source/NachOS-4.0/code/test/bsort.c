/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"


void bubbleSort(int* a, int n, int comp)
{
    int i, j;

    if (comp == 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n - 1; j++)
            {
                if (a[j] > a[j + 1])
                { // Swap:
                    a[j + 1] += a[j];
                    a[j] = a[j + 1] - a[j];
                    a[j + 1] -= a[j];
                }
            }
        }
    }

    else
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n - 1; j++)
            {
                if (a[j] < a[j + 1])
                { // Swap:
                    a[j + 1] += a[j];
                    a[j] = a[j + 1] - a[j];
                    a[j + 1] -= a[j];
                }
            }
        }
    }
}


int main()
{
    int n, i, j, choice;
    int a[100];

Input:
    PrintString("Enter number of element: ");
    n = ReadNum();
    if (n < 1 || n > 100)
    {
        PrintString("n must bigger than 0 and smaller than 100. Please try again!\n");
        goto Input;
    }

    // Nhap mang:
    for (i = 0; i < n; i++)
    {
        PrintString("a[");
        PrintNum(i);
        PrintString("] = ");
        a[i] = ReadNum((n - 1) - i);
    }

Choice:
    // Lua chon:
    PrintString("[1]. Increasing bubble sort.\n[2]. Decreasing bubble sort.\n\n");
    PrintString("Enter your choice: ");
    choice = ReadNum();

    if (choice == 1) bubbleSort(a, n, 0);

    else if (choice == 2) bubbleSort(a, n, 1);

    else
    {
        PrintString("The choice is not exist. Please try again!\n");
        goto Choice;
    }

    for (i = 0; i < n; i++)
    {
        PrintNum(a[i]);
        PrintChar(' ');
    }
    PrintChar('\n');
    PrintChar('\n');

    Halt();
    /* not reached */
}
