#pragma once
///////////////////////////////////////////////////////////////////////////////
// Project: Hoyo Client Game
//
// hy_function.h
// Created: 2012-8-6
// Author: Like Cheng
///////////////////////////////////////////////////////////////////////////////

//#include "hy_random.h"

class hy_function
{
public:

	static hy_function*			instance();
	void    init();

	bool					splite_string_to_vec( const char* in_str, std::vector< std::string > &out_str, char sign = '|' );
	bool					splite_sint_to_vec( const char* in_str,std::vector< int > &out_int, char sign = '|' );
    int randomFrom(int n,int m);
private:

	static hy_function*			instance_p_;


};
