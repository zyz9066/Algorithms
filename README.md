# Algorithms

## Graph Algorithm

### Dijkstrs's Algorithm
Implemented Dijkstra's Algorithm in C++.

### Dominating Set
Little red riding hood is a very competent graph theorist. She has *n* intervals `[l_i, r_i]`. She created a *n*
vertex graph where each vertex represents an interval. There is an edge between *u* and *v* if and only if their
corresponding intervals (call them interval *u* and *v*) intersect, i.e. `max(lv, lu) ≤ min(rv, ru)`. Now she want
you to help her to count the number of Dominating sets in this graph.
Set *S* of vertices of graph *G* is a Dominating set if and only if each vertex *v* of *G* is either a member of *S*
or one of its neighbor is in *S*.

**Input**

Line 1: Integer number *n*, number of intervals.

Lines 2 to n+1: *l_i* and *r_i* the start and end point of interval *i*.

**Output**

One integer which is the number of Dominating sets of the graph.

**Sample Test Cases**
• Input 1
```
2
1 5
3 3
```
• Output 1
```
3
```
• Input 2
```
3
1 3
2 5
4 6
```
• Output 2
```
5
```

## Back-tracking Algorithm
Solving n-queens problem in C++.
