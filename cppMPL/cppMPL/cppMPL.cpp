// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//////////////////////////////2.0///////////////////////////////////
template<class T>
struct add_const_ref {
	typedef const T&  value;
};
template<class T>
struct add_const_ref<T &> {
	typedef T & value;
};
//////////////////////////////////////////////////////////////////////////


/////////////////////////////2.1//////////////////////////////////////////
template<typename C, typename X, typename Y>
struct replace_type;

template<typename C, typename X, typename Y>
struct replace_type_aux;


template<typename X, typename Y>
struct replace_type<X, X, Y>
{
	typedef Y type;
};

template<typename X,  typename Y>
struct replace_type_aux<X, X, Y>
{
	typedef Y type;
};

template<typename C,typename X, typename Y>
struct replace_type_aux
{
	typedef C type;
};


template<typename C, typename X, typename Y>
struct replace_type 
{
	typedef typename replace_type_aux<C, X, Y>::type type;
};

template<typename C, typename X, typename Y>
struct replace_type_aux<C const, X, Y>
{
	typedef typename replace_type<C, X, Y>::type const type;
};

template<typename C, typename X, typename Y>
struct replace_type_aux<C *, X, Y>
{
	typedef typename replace_type<C, X, Y>::type * type;
};

template<typename C, typename X, typename Y>
struct replace_type_aux<C [], X, Y>
{
	typedef typename replace_type<C, X, Y>::type type[];
};


template <typename C, typename X, typename Y, unsigned N>
struct replace_type_aux<C[N], X, Y> {
	typedef typename replace_type<C, X, Y>::type type[N];
};

//////////////////////////////////////////////////////////////////////////

int main()
{
	/*typedef const int&  const_int_ref;
	cout << std::is_same<add_const_ref<int &>::value, int >::value << endl;*/

	//cout<<std::is_same<replace_type<int *[], int, float>::type, float *[]>::value << endl;

	return 0;
}

