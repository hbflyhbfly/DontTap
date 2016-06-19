///////////////////////////////////////////////////////////////////////////////
// Project: Hoyo Client Game
//
// hy_function.cpp
// Created: 2012-8-6
// Author: Like Cheng
///////////////////////////////////////////////////////////////////////////////
#include "hy_function.h"
//#include <iconv.h>
#include <fstream>
#include <stdio.h>
#include <iostream>



static hy_function* hy_function_instance_p_ = NULL;
//////////////////////////////////////////////////////////////////////////////////
//hy_function::form_code
//////////////////////////////////////////////////////////////////////////////////

void  hy_function::init()
{

}

hy_function* hy_function::instance()
{
    if (hy_function_instance_p_ == NULL){
        hy_function_instance_p_ = new hy_function;
        hy_function_instance_p_->init();
    }
    return hy_function_instance_p_;
}

//hy_function::splite_string_to_vec
//////////////////////////////////////////////////////////////////////////////////
bool hy_function::splite_string_to_vec( const char* in_str, std::vector< std::string > &out_str, char s )
{
	std::string::size_type index, last, last_index;

	std::string ori_str, add_char, add_endchar;

	ori_str = in_str;

	index = last = last_index = 0;

	out_str.clear();

	if( ori_str.empty() || !ori_str.compare("") )
		return false;
	std::string sign;
	sign = s;

	bool is_flag = true;
	bool is_end_char = false;

	index = ori_str.find_first_of( sign, 0 );
	last_index = ori_str.find_last_of( sign );

	if( !index || index > ori_str.max_size() )
	{
		out_str.push_back( ori_str );
		return true;
	}

	while( is_flag )
	{
		index = ori_str.find_first_of( sign, last );

		if( index == last_index && last_index > 0 )
		{
			add_endchar = ori_str.substr( last_index + 1, ori_str.length() - last_index );
			is_end_char = true;
		}
		if( index > ori_str.length() || index <= last )
		{
			break;
		}
		add_char = ori_str.substr( last, index - last );

		out_str.push_back( add_char );
		if( is_end_char )
			out_str.push_back( add_endchar );

		last = index + 1;	
	}

	return true;
}
////////////////////////////////////////////////////////////
//hy_function::splite_sint_to_vec
////////////////////////////////////////////////////////////
bool hy_function::splite_sint_to_vec( const char* in_str,std::vector< int > &out_int, char sign )
{
	std::string str = in_str;
	out_int.clear();
	if( str.empty() || !str.compare("") || !str.compare("\0") )
		return false;
	std::vector< std::string > get_str;get_str.clear();
	splite_string_to_vec( str.c_str(), get_str, sign );

	for( std::string::size_type i = 0; i < get_str.size(); i++ )
	{
		out_int.push_back( (int)::atoi( get_str.at( i ).c_str() ) );
	}
	return true;
}

int hy_function::randomFrom(int n,int m){
    int x = (rand()%(m-n+1))+n;
    return x;
}

////////////////////////////////////////////////////////////
//hy_function::get_str_pos
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//hy_function::assembly_vec_by_string
////////////////////////////////////////////////////////////
