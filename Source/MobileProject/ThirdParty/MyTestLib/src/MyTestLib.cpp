// MyTestLib.cpp : Defines the functions for the static library.
//

#include "MyTestLib.h"

#include "pch.h"
#include "framework.h"

#include <stdexcept>

namespace my_test_lib
{
	double Math::Add(double a, double b)
	{
		return a + b;
	}
}
