#pragma once
///////////////////////////////////////////////////////////////////////////////
// Project: Hoyo Client Game
//
// hy_function.h
// Created: 2012-8-6
// Author: Like Cheng
///////////////////////////////////////////////////////////////////////////////

//#include "hy_random.h"

class ad_function
{
public:

	static ad_function*			instance();
	void    init();
    void prepareBanner();
    
    void showBanner();
    
    void hideBanner();
    
    void showInterstitial();
    
    void incentivized();
    void prepareVideo();
    
    void showVideo();
private:

	static ad_function*			instance_p_;


};
