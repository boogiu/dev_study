

#include <iostream>
#include<vector>

using namespace std;

bool solution(vector<int> _arr, int _target);

vector<int> mapping(vector<int> _vector);

int main()
{
    std::cout << "Hello World!\n";
    vector<int> tstVector = { 1,2,3,7,4,5 };

    cout<<    solution(tstVector, 10);
}

bool solution(vector<int> _arr, int _target)
{
    vector<int> storage(_target +1,0);

    for (int i : _arr) {
        if (i > _target) { continue; }

        int iMap = _target - i;
        if (storage[i] == 0 && storage[iMap] == 0) {
            storage[i] = 1;
        }
        else {
            return true;
        }
    }

    return false;
}
