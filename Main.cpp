#include <iostream>
#include <map>
#include <algorithm>
//lvalue , rvalue referens
int func() {
	return 2;
}
int globalvalue = 20;
int& bar() {
	return globalvalue;
}
class A {
public:
	void foo()const {
		std::cout << "A: foo() const\n";
	}
	void foo() {
		std::cout << "A: foo()\n";
	}

};
A aFunc() { return A(); }
const A C_aFunc() { return A(); }

class Intvec {
public:
	explicit Intvec(size_t num = 0) : m_size(num), m_data(new int[m_size]) {
		log("constraction");
	}
	~Intvec() {
		log("destraction");
		if (m_data) {
			delete[] m_data;
			m_data = nullptr;
		}
	}
	Intvec(const Intvec& other) :m_size(other.m_size), m_data(new int[m_size]) {
		log("copy constrution");
		for (size_t i = 0; i < m_size; i++) {
			m_data[i] = other.m_data[i];
		}
	}
	Intvec& operator= (const Intvec& other) {// оператор присвоения копированием
		log("copy assignment operator");
		Intvec tmp(other);
		std::swap(m_size, tmp.m_size);//copy-swap  идиома
		std::swap(m_data, tmp.m_data);
		return *this;
	}
	Intvec& operator= (Intvec&& other) noexcept{//ссылка на rvalue (унивирсальная ссылка)
		//оператор присваивания перемещением
		log("move assignment operator");
		std::swap(m_size, other.m_size);//copy-swap  идиома
		std::swap(m_data, other.m_data);
		return *this;
	}
private:
	void log(const char* msg) {
		std::cout << "[" << this << "] " << msg << '\n';
	}
	size_t m_size;
	int* m_data;
};

int main() {
	setlocale(LC_ALL, "Russian");
	//lvalue - обьект которое занимает идентифицируемое значение в памяти , занимает адресс
	int x = 5;
	//func() = 2;
	bar() = 10;
	std::map<int, float> mymap;
	mymap[10] = 5.6f;
	int t = 1;
	int tt = 11;
	int res = t + tt;//оператор+ ожидает rvalue  поэтому lvalue неявно преобразуется в  rvalue
	int arr[] = { 1,2 };
	int* p = &arr[0];
	*(p + 1) = 10;//явное преоброзование  rvalue (p+1) в lvalue с помощью *  - *(p+1)-lvalue

	int var = 10;
	//int* bad_addr = &(var + 1);//error для унарного оператора & требуется lvalue;
	int* addr = &var;//ok
	//&var = 40;//error , с левой сторону требуется lvalue
	// *******************************************************
	//lvalue  на тип Т , не являющемся функциональным или массивом, может быть преобразован в rvalue.<...>
	//Если Т не класс, типом rvalue  является cv-неспецифицированная версия типа Т, иначе , типом rvalue  является Т
	// (CV: c -const, v - volatile)
	//***********************************************************
	//если Т - это класс , то rvalue  может сохранить  cv-специфицированность
	aFunc().foo();
	C_aFunc().foo();//cv-квалифицированный rvalue

	Intvec v1(20);
	Intvec v2;
	Intvec v3;
	std::cout << "Assigning lvalue...\n";
	v2 = v1;
	std::cout << "Ended assigning lvalue...\n";
	std::cout << "***********************\n";
	std::cout << "Assigning rvalue...\n";
	v3 = Intvec(33);
	v3 = std::move(Intvec(33));//функция move преобразует lvalue в rvalue
	std::cout << "Ended assigning rvalue...\n";
	return 0;
}
