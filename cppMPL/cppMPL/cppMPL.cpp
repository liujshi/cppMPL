// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "boost/mpl/vector_c.hpp"
#include "boost/mpl/int.hpp"
#include "boost/mpl/plus.hpp"
#include "boost/static_assert.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/type_traits.hpp"
# include <boost/mpl/int.hpp>
#include <boost/mpl/placeholders.hpp>

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

template<typename X, typename Y>
struct replace_type_aux<X, X, Y>
{
	typedef Y type;
};

template<typename C, typename X, typename Y>
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
struct replace_type_aux<C[], X, Y>
{
	typedef typename replace_type<C, X, Y>::type type[];
};


template <typename C, typename X, typename Y, unsigned N>
struct replace_type_aux<C[N], X, Y> {
	typedef typename replace_type<C, X, Y>::type type[N];
};

//////////////////////////////////////////////////////////////////////////


namespace mpl = boost::mpl;


typedef	mpl::vector_c<int, 1, 0, 0, 0, 0, 0, 0> mass;
typedef	mpl::vector_c<int, 0, 1, 0, 0, 0, 0, 0> length;
typedef	mpl::vector_c<int, 0, 0, 1, 0, 0, 0, 0> time;
typedef	mpl::vector_c<int, 0, 0, 0, 1, 0, 0, 0> charge;
typedef	mpl::vector_c<int, 0, 0, 0, 0, 1, 0, 0> temperature;
typedef	mpl::vector_c<int, 0, 0, 0, 0, 0, 1, 0> intensity;
typedef	mpl::vector_c<int, 0, 0, 0, 0, 0, 0, 1> amount_of_substance;


typedef	mpl::vector_c<int, 0, 1, -1, 0, 0, 0, 0> velocity;
typedef	mpl::vector_c<int, 0, 1, -2, 0, 0, 0, 0> acceleration;
typedef	mpl::vector_c<int, 1, 1, -1, 0, 0, 0, 0> momentum;
typedef	mpl::vector_c<int, 1, 1, -2, 0, 0, 0, 0> force;

typedef	mpl::vector_c<int, 0, 0, 0, 0, 0, 0, 0> scalar;

template<class T, class Dimensions>
struct quantity
{
	explicit quantity(T x) :m_value(x) {};
	T value() const { return m_value; }
	
	template <class OtherDimensions>
	quantity(quantity<T, OtherDimensions> const& rhs)
		: m_value(rhs.value())
	{
		BOOST_STATIC_ASSERT((
			mpl::equal<Dimensions, OtherDimensions>::type::value
			));
	}
private:
	T m_value;
};

template<class T, class D>
quantity<T, D>
operator+(quantity<T, D> x, quantity<T, D> y)
{
	return quantity<T, D>(x.value() + y.value());
}

template<class T, class D>
quantity<T, D>
operator-(quantity<T, D> x, quantity<T, D> y)
{
	return quantity<T, D>(x.value() - y.value());
}

struct plus_f
{
	template <class T1, class T2>
	struct apply {
		typedef typename mpl::plus<T1, T2>::type type;
	};
};

template<class T, class D1, class D2>
quantity<T,
	typename mpl::transform<D1, D2, plus_f>::type 
>
operator*(quantity < T, D1> x, quantity < T, D2> y)
{
	typedef typename mpl::transform<D1, D2, plus_f>::type dim;
	return quantity<T, dim>(x.value() *y.value());
}

template <class UnaryMetaFunctionClass, class Arg>
struct apply1
	: UnaryMetaFunctionClass::template apply<Arg>
{};

template <class F,class X>
struct twice
	: apply1<F, typename apply1<F, X>::type>
{};

template<class _Ty>
struct add_pointer1
{	// add pointer
	typedef typename remove_reference<_Ty>::type *type;
};
struct add_pointer_f
{
	template <class T>
	struct apply :add_pointer1<T> {};
};

BOOST_STATIC_ASSERT((
	boost::is_same<twice<add_pointer_f, int>::type, int **>::value
	));
//////////////////////3.0//////////////////////////////////////////////
template<unsigned int N>
struct binary
{
	static const unsigned int digit = N % 10;
	BOOST_STATIC_ASSERT(digit == 0 || digit == 1);
	static const unsigned int value = digit + (binary<N / 10>::value * 2);
};
template<>
struct binary<0>
{
	static const unsigned int value = 0;
};

/////////////////////3.1/////////////////////////////////////////////////
using namespace mpl::placeholders;
typedef mpl::transform<mpl::vector_c<int, 1, 2, 3>, mpl::vector_c<int, 1, 1, 1>, mpl::plus<_1,_2> >::type mytype;

typedef mpl::vector_c<int, 2, 3, 4>::type expec;

BOOST_STATIC_ASSERT((
	mpl::equal<mytype, expec>::type::value
	));


////////////////////3.2//////////////////////////////////////////////////
#include "boost/mpl/multiplies.hpp"
typedef mpl::vector_c<int, 1, 2, 3>::type v1;
typedef mpl::transform<v1, mpl::multiplies<_1, _1>>::type v2;
typedef mpl::vector_c<int, 1, 4, 9>::type v3;
BOOST_STATIC_ASSERT((
	mpl::equal<v2, v3>::type::value
	));
typedef mpl::vector_c<int, 1, 2, 3>::type v11;
typedef mpl::transform<v1, mpl::multiplies<_, mpl::int_<3>>>::type v21;
typedef mpl::vector_c<int, 3, 6, 9>::type v31;
BOOST_STATIC_ASSERT((
	mpl::equal<v21, v31>::type::value
	));


///////////////////3.3////////////////////////////////////////////////////
#include <boost/type_traits/add_pointer.hpp>
BOOST_STATIC_ASSERT((
	boost::is_same<twice<add_pointer_f, twice<add_pointer_f, int>::type >::type, int ****>::value
	));

BOOST_STATIC_ASSERT((
	boost::is_same<twice<twice, twice<add_pointer_f, int>::type >::type, int ****>::value
	));
//BOOST_STATIC_ASSERT((
//	boost::is_same<twice<boost::add_pointer<_>, twice<boost::add_pointer<_>, int> >::type, int ****>::value
//	));

int main()
{
	/*typedef const int&  const_int_ref;
	cout << std::is_same<add_const_ref<int &>::value, int >::value << endl;*/

	//cout<<std::is_same<replace_type<int *[], int, float>::type, float *[]>::value << endl;

	auto p = quantity<int, length>(2);
	auto q = quantity<int, length>(3);
	auto r = quantity<int, mass>(3);
	p = q+p;
	cout << p.value() << endl;

	BOOST_STATIC_ASSERT((mpl::plus<mpl::int_<1>, mpl::int_<2> >::value == 3));

	auto a1 = quantity<float, mass>(5.0f);
	auto a2 = quantity<float, acceleration>(5.0f);
	quantity<float, force> c = a1*a2;
	cout<<(a1 * a2).value()<<endl;
	return 0;
}

