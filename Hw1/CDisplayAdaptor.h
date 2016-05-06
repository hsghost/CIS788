/*
/
*/

#ifndef __CDisplayAdaptor_H__
#define __CDisplayAdaptor_H__

#include <string>
#include "CNetlist.h"

class CDisplayAdaptor
{
public:
	CDisplayAdaptor(std::string *sequence, CNetlist *netlist, std::string *netlistStr);
	~CDisplayAdaptor();

private:
	std::string *gateSequence;
	CNetlist *netlist;
	std::string *netlistString;

public:
	void trimGateSequence();
	void serializeNetlist();
};

#endif
