#include <math.h>

#include <iostream>
int main(int argv, char* argc[]) {
    int size = atoi(argc[1]);
    int num = 0;
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            int num = (i - 1)*size + (j - 1);
            std::cout << num;
            if (j < size)
                std::cout << ", ";
            else
                std::cout << "\n";
            num++;
        }
    }
}
/*
0 0 
1 0
2 0 
3 0

2 1
3 1
4 1
5 1
*/