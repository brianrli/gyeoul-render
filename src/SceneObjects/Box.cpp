#ifdef _WIN32
namespace std
{
	template <class _Tp>
	inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
	  return __b < __a ? __b : __a;
	}

	template <class _Tp>
	inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
	  return  __a < __b ? __b : __a;
	}
}
#endif


#include <cmath>
#include <assert.h>
#include <algorithm>

#include "Box.h"

using namespace std;

const double HUGE_DOUBLE = 1e100;

bool Box::intersectLocal( const ray& r, isect& i ) const
{
    // YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.

	return false;
}
