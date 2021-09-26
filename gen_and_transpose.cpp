#include "bits/stdc++.h"
#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"

using namespace std;
using namespace __gnu_pbds;


#define ll long long int
#define pb push_back
#define mp make_pair
#define ff first
#define ss second
#define all(a) a.begin(),a.end()

typedef tree <ll, null_type, less <ll>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
// order_of_key(val): returns the number of values less than val
// find_by_order(k): find_by_order(0) returns an iterator to the smallest element
int main(int argv, char* argc[] )
{
    srand(time(NULL));
    int N = atoi(argc[1]);
    vector<vector<int>> matrix(N, vector<int> (N, 0)), transpose(N, vector<int> (N, 0));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            int cur = (rand());
            matrix[i][j] = cur;
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            transpose[i][j] = matrix[j][i];
        }
    }
    ofstream f1("data/M.csv", ios::trunc), f2("data/MT.csv", ios::trunc);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            f1 << matrix[i][j];
            if(j != N - 1) {
                f1 << ", ";
            } else {
                f1 << "\n";
            }
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            f2 << transpose[i][j];
            if(j != N - 1) {
                f2 << ", ";
            } else {
                f2 << "\n";
            }
        }
    }
}