/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int lb = 0;
    int ub = n-1;
    int guess = ((ub-lb)/2) + lb;

    
    if (n < 0)
        return false;
    while (true)
    {
        if (values[guess] == value)
            {   
                return true;
            }
        else if (ub == lb)
            return false;
        else if (ub == lb+1)
            {
                if (guess == ub)
                {
                    guess = lb;
                    ub = lb;
                }
                else
                {
                    guess = ub;
                    lb = ub;
                }
            }
        else if (values[guess] < value)
        {   // Guess was too small, look in upper half
            lb = guess;
            guess = ((ub-lb)/2) + lb;
        }
        else 
        {  // Guess was too big, look in lower half
            ub = guess;
            guess = ((ub-lb)/2) + lb;
        }
        
        
     }
       return false; 
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int i,j;
    int temp;
    int temp_val;
    for (i=0;i<n;i++)
    {
        temp = i;
        for (j=i+1;j<n;j++)
        {
            if (values[j] < values[temp]) {
                temp = j;
                }
        }
        if (temp != i)
            {
                temp_val = values[temp];
                values[temp] = values[i];
                values[i] = temp_val;
            }
      }
    return;
}
