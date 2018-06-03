#pragma once
#include <exception>

class MyException : public std::exception
{
    
	const char* what () const throw ()
    {
    	return "C++ Exception";
    }
};
