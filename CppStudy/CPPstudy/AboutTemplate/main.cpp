#include "Template.h"

using namespace TemplateStd;

int main() {
	InstanceTemplate<int,int> intTemp;
	intTemp.Add(1, 2);
	InstanceTemplate <double, int> diTemp;
	diTemp.Double(2.3,1);
	InstanceTemplate <char, char> chTemp;
	chTemp.Double('r', 'o');

	coutAll(1);
	coutAll((char)65);

	return 0;
}