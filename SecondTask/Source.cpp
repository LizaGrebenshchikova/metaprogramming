#include <iostream>
#include <vector>
#include <list>

using namespace std;

// NullType
struct NullType { };

// TypeList
template <typename H, typename ...T>
struct TypeList
{
	typedef H H;
	typedef TypeList<T...> T;
};

template <typename H>
struct TypeList<H>
{
	typedef H H;
	typedef NullType T;
};

typedef TypeList<NullType> EmptyList;

// LinearHierarchy
template <typename Atomic, class Base>
class Unit : public Base 
{
	Atomic* value;
};

template <typename, template <typename, class> class>
class LinearHierarchy;

template <typename H, typename ...T, template <typename, class> class Unit>
class LinearHierarchy<TypeList<H, T...>, Unit> : Unit<H, LinearHierarchy<TypeList<T...>, Unit>> { };

template <template <typename, class> class Unit>
class LinearHierarchy<EmptyList, Unit> : public Unit<NullType, NullType> { };

// FibonacciHierarchy
template <unsigned char n>
struct Fibonacci 
{
	static const unsigned char number = Fibonacci<n - 1>::number + Fibonacci<n - 2>::number;
};

template <>
struct Fibonacci<0> 
{
	static const unsigned char number = 1;
};

template <>
struct Fibonacci<1> 
{
	static const unsigned char number = 1;
};

template <typename, template <typename, class> class, typename, unsigned char, unsigned char, unsigned char>
class HierarchyBranch;

template <typename H, typename ...T, template <typename, class> class Unit,
	typename ...AccT, unsigned char n, unsigned char CurSize, unsigned char Size>
class HierarchyBranch<TypeList<H, T...>, Unit, TypeList<AccT...>, n, CurSize, Size>
	: HierarchyBranch<TypeList<T...>, Unit, TypeList<H, AccT...>, n, CurSize + 1, Size> { };

template <typename H, typename ...T, template <typename, class> class Unit, 
	typename ...AccT, unsigned char n, unsigned char CurSize, unsigned char Size>
class HierarchyBranch<TypeList<H, T...>, Unit, TypeList<AccT...>, n, CurSize, Size>
	: public LinearHierarchy<TypeList<AccT...>, Unit>,
	  public HierarchyBranch<TypeList<H, T...>, Unit, EmptyList, n + 1, 0, Fibonacci<n + 1>::number + 1> { };

template <template <typename, class> class Unit, typename ...AccT, unsigned char n, unsigned char CurSize, unsigned char Size>
class HierarchyBranch<EmptyList, Unit, TypeList<AccT...>, n, CurSize, Size>
	: public LinearHierarchy<TypeList<AccT...>, Unit> { };

template <template <typename, class> class Unit, typename ...AccT, unsigned char n, unsigned char Size>
class HierarchyBranch<EmptyList, Unit, TypeList<AccT...>, n, Size, Size> 
	: public LinearHierarchy<TypeList<AccT...>, Unit> { };

template <typename List, template <typename, class> class Unit>
class FibonacciHierarchy : public HierarchyBranch<List, Unit, EmptyList, 0, 0, 4> { };

int main() {
	
	typedef 
	FibonacciHierarchy
	<
		TypeList<int, double, long long, long double>,
		Unit
	> 
	HierarchyTypeName;

	HierarchyTypeName hier;
}