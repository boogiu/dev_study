//스택 직접 구현
/*
구현목표
1. push / pop
2. 이터레이터 구현
3. 연결형 리스트(node) 활용
*/

#include <iostream>
namespace MyStack {
	class stack
	{
	public:
		stack();
		~stack();
		//노드
		struct Node {
			Node* prev;
			int iValue;
			Node(Node* prev, int iValue) : prev(prev), iValue(iValue) {}
		};
		//이터레이터
		class myIter
		{
		public:
			myIter(Node* node) :nowNode(node) {}
			myIter& operator++(){ //전위 연산
				if (nowNode == nullptr) return *this;
				nowNode = nowNode->prev;
				return *this;
			}

			myIter& operator++(int) { //후위연산
				if (nowNode == nullptr) return *this;
				myIter tmp = *this;
				nowNode = nowNode->prev;
				return tmp;
			}

			bool operator==(const myIter& _other) {
				return(this->nowNode == _other.nowNode);
			}
			bool operator !=(const myIter& _other) {
				return !(*this == _other);
			}
			int operator*() const {
				return nowNode->iValue;
			}
		private:
			Node* nowNode;
		};

	public:
		void push(int i);
		void pop();
		bool isEmpty();
		
		myIter begin() { return myIter(curNode); }
		myIter end() {return myIter(&startNode);}

	private:
		Node* curNode; //최근 삽입 위치
		Node startNode; //가장 아래 노드(비어있는 노드)
	};

	//-----------------------------------------------------------
	stack::stack() : startNode(nullptr, -1),curNode(&startNode)
	{
		curNode = &startNode;
	}

	void stack::push(int i)
	{
		Node* tmpNode = new Node(curNode, i);
		curNode = tmpNode; //새로 노드 구성 + 이전 curr을 prev로 이어주기
	}

	void stack::pop() {
		Node* tmpNode = curNode;
		curNode = curNode->prev;
		delete tmpNode;
	}

	bool stack::isEmpty()
	{
		return &startNode == curNode;
	}

	stack::~stack()
	{
		while (curNode != &startNode) {
			Node* tmpNode = curNode;
			curNode = curNode->prev;
			delete tmpNode;
		}
	}
}
using namespace MyStack;

int main()
{
	std::cout << "Hello World!\n";
}