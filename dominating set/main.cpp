#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>

using namespace std;

struct Vertex {
	int num_dominated;
	int num_choice;
	vector<Vertex*> adj;

    Vertex() {
        num_dominated = 0;
    }
};

int count = 0;

void countDomSet(int level, int *n, vector<Vertex*> adjList, int *n_dominated) {
    if (*n_dominated == *n && level == *n) {
        ++::count;
        return;
    }

    if (--adjList[level]->num_choice != 0) {
        bool valid = true;
        for (auto itr=adjList[level]->adj.begin(); itr!=adjList[level]->adj.end(); ++itr) {
            if (--(*itr)->num_choice == 0)
                valid = false;
        }
        if (valid)
            countDomSet(level+1, n, adjList, n_dominated);
    }

    if (++adjList[level]->num_choice != 1) {
        for (auto itr=adjList[level]->adj.begin(); itr!=adjList[level]->adj.end(); ++itr)
            ++(*itr)->num_choice;
    }

    if (adjList[level]->num_dominated++ == 0)
        ++*n_dominated;
    for (auto itr=adjList[level]->adj.begin(); itr!=adjList[level]->adj.end(); ++itr)
        if ((*itr)->num_dominated++ == 0)
            ++*n_dominated;

    countDomSet(level+1, n, adjList, n_dominated);

    if (level == 0) return;

    if (--adjList[level]->num_dominated == 0)
        --*n_dominated;
    for (auto itr=adjList[level]->adj.begin(); itr!=adjList[level]->adj.end(); ++itr)
        if (--(*itr)->num_dominated == 0)
            --*n_dominated;
    
    return;
}

int main() {

    ifstream inFile("sample.txt");
    int n, curL, curR, n_dominated;
    inFile >> n;

    vector<pair<int, int> > vertices;
    vector<Vertex*> adjList;

    while (inFile >> curL >> curR) {
        Vertex* v = new Vertex();
        adjList.push_back(v);
        if (!vertices.empty()) {
            int size = vertices.size();
            for (int i = 0; i < size; ++i) {
                int l = vertices[i].first, r = vertices[i].second;
                if (max(curL, l) <= min(curR, r)) {
                    adjList[i]->adj.push_back(adjList[size]);
                    adjList[size]->adj.push_back(adjList[i]);
                }
            }
        }
        vertices.push_back(make_pair(curL, curR));
    }

    for (auto itr=adjList.begin(); itr!=adjList.end(); ++itr)
        (*itr)->num_choice = (*itr)->adj.size()+1;

    countDomSet(0, &n, adjList, &n_dominated);

    cout << ::count << endl;

    return 0;
}