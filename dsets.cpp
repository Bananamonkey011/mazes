/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num)
{
    for (int i = 0; i < num; i++)
    {
        s_.push_back(-1);
    }
}

int DisjointSets::find(int elem)
{
    if (s_[elem] < 0)
    {
        return elem;
    }
    else
    {
        int newHead = find(s_[elem]);
        s_[elem] = newHead;
        return newHead;
    }
}

void DisjointSets::setunion(int a, int b)
{
    int root1 = find(b);
    int root2 = find(a);
    if (root1 != root2)
    {
        int newSize = s_[root1] + s_[root2];
        // If s_[root1] is less than (more negative), it is the larger set; // we union the smaller set, root2, with root1.
        if (s_[root1] < s_[root2])
        {
            s_[root2] = root1;
            s_[root1] = newSize;
        }
        // Otherwise, do the opposite:
        else
        {
            s_[root1] = root2;
            s_[root2] = newSize;
        }
    }
}

int DisjointSets::size(int elem) {
    return -1*s_[find(elem)];
}