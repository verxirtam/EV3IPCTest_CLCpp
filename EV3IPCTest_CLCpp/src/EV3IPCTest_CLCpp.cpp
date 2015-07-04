/*
 * EV3IPCTest_CLCpp.cpp
 *
 *  Created on: 2014/01/29
 *      Author: daisuke
 */

#include "TCPClient.h"
#include <iostream>


using namespace std;

int main(void)
{
	TCPClient cl("192.168.0.6",50000);
	string s;
	cl.ReceiveText(s);
	cout<<s<<endl;
}
