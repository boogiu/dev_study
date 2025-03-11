//인형 뽑기

//1. 2차원 배열을 받음. + 뽑을 순서 받음.
//2. 바구니 스택 + 일치하는 것. pop
// 3. 터질때 카운트 ++

#include <iostream>
#include <vector>
#include <stack>
using namespace std;
int Solution(vector<vector<int>> board, vector<int> moves);

int main()
{
    vector <vector<int>> tstBoard = { {0, 0, 0, 0, 0}, {0,0,1,0,3} ,{0,2,5,0,1},{4,2,4,4,2},{3,5,1,3,1} };
    vector<int> tstMove = { 1,5,3,5,1,2,1,4 };
    Solution(tstBoard,tstMove);
}

int Solution(vector<vector<int>> board, vector<int> moves) {
    int iAnswer(0); //카운트
    vector<stack<int>> newBoard(board.size());
    stack<int> bucket;

    // 우선 열로 가서, 0이 아닌 행을 집어
    // 그다음에 집은 부분의 index의 값은 0으로.
    // 그다음 스택에 넣어.
    // 그리고 카운트

    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (0 != board[i][j]) {
                newBoard[i].push(board[i][j]);
            }
        }
    }


    for (int i = 0; i < moves.size(); ++i) {
        int picked(0);

        if ((newBoard[moves[i] - 1]).empty()) {
            continue;
        }
        else {
            picked = newBoard[moves[i] - 1].top();
            newBoard[moves[i] - 1].pop();
        }

        if (bucket.empty() || (bucket.top() != picked)) {
            bucket.push(picked);
        }
        else if(bucket.top()==picked){
            bucket.pop();
            iAnswer+=2;
        }
    }

    return iAnswer;
}

