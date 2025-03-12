//트리 직접 구현해보기. (미완)

#include <iostream>

using namespace std;

namespace myTree {

	class Node {
	public:
		Node* left;
		Node* right;
		int iValue;
		Node(int _val) : iValue(_val), left(nullptr), right(nullptr) {}
		Node() :iValue(0), left(nullptr), right(nullptr) {}
	};
	class Tree {
	public:
		Node* Top;

		Tree() :Top(nullptr) {}
		void insert(Node& _node) {
			//예외처리(top이 없을때)
			if (Top == nullptr) {
				Top = &_node;
				return;
			}
			//탑보다 작으면, 탑의 left를 탑으로 재설정. 언제까지? (top보다 작고 left가 null / top보다 크고, top의 right가 null일때까지)
			Node* _Temp = Top;
			while (true)
			{
				if (_Temp->iValue > _node.iValue) {
					//왼쪽 자식이 없으면 순회 멈춤.
					if (_Temp->left == nullptr) {
						_Temp->left = &_node;
						break;
					}
					//왼쪽 자식이 있으면 다시.
					_Temp = _Temp->left;
				}
				else if (_Temp->iValue < _node.iValue) {
					if (_Temp->right == nullptr) {
						_Temp->right = &_node;
						break;
					}
					_Temp = _Temp->right;
				}
			}
		}
	};

}

using namespace myTree;

int main()
{
	Tree tstTree;

	for (int i = 0; i < 10; ++i) {
		Node m_node(i);
		tstTree.insert(m_node);
	}

	cout << "테스트!" << endl;

}