
#define M_B 1000000007
#define POL____Y 31

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>


using namespace std;

vector<bool> Hash02(vector<string> string_list, vector<string> query_list);

long long GetHash(const string& _str);

int main()
{
    std::cout << "Hello World!\n";
    vector<string> tst_str = { "apple" ,"banana"};
    vector<string> tst_query = {"kiwi","bird","apple"};

    Hash02(tst_str, tst_query);
}

vector<bool> Hash02(vector<string> string_list, vector<string> query_list)
{
    vector<bool> answer;
    unordered_set<long long> hashSet;

    for (string str : string_list) {
        long long hash = GetHash(str);
        hashSet.insert(hash);
    }

    for (string str : query_list) {
        long long QueryHash = GetHash(str);
        bool find = hashSet.find(QueryHash) != hashSet.end();
        answer.push_back(find);
    }
    return answer;
}

long long GetHash(const string& _str) {
    
    long long hash_value = 0; 
    for (char c : _str) {
        hash_value = (hash_value * POL____Y + c) % M_B; // 
    }
    return hash_value;
}

//• 다음식에서 p는 31, 은 1, 000, 000, 007로합니댜
//- hash(s) = (s[O] + s[l] + p + s[2] + p2 …… ..s[n - l] + pn)mod m