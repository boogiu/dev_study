CBase::CBase():m_dwRefCnt(0){}
CBase::~CBase(){}

unsigned long CBase::AddRef() {
	return ++m_dwRefCnt;
	//셰어드 포인터의 레퍼런스 카운트를 증가 해줌.
}
unsigned long CBase::Release() {
	if (0 == m_dwRefCnt) {
		//이번이 0번째 카운트 즉, 마지막이라면
		Free();
		delete this;
		return 0;
	}

	return m_dwRefCnt--;
	//후위 필수
	/*
	만약 카운트가 1일 때 불러지면
	전치라면 카운트가 0이 되고, 0이 반환됨.
	순회를 돌며 if(obj -> Release() == 0)에 브레이크를 걸었다면
	마지막 1개가 삭제 되지 않게 됨
	*/
}
