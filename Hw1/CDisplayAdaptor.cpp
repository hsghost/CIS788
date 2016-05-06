/*
/
*/

#include <iostream>
#include <sstream>
#include <queue>
#include "CDisplayAdaptor.h"

CDisplayAdaptor::CDisplayAdaptor(std::string *sequence, CNetlist *netlist, std::string *netlistStr)
{
	this->gateSequence = sequence;
	this->netlist = netlist;
	this->netlistString = netlistStr;
}

CDisplayAdaptor::~CDisplayAdaptor()
{
	this->gateSequence = NULL;
	this->netlist = NULL;
	this->netlistString = NULL;
}

void CDisplayAdaptor::trimGateSequence()
{
	
	for (std::string::iterator it = this->gateSequence->begin(); it != gateSequence->end() && (*it >= '0' && *it <= '9'); )
		it = this->gateSequence->erase(it);
	for (std::string::reverse_iterator rit = this->gateSequence->rbegin(); rit != gateSequence->rend() && (*rit >= '0' && *rit <= '9'); ++rit)
		this->gateSequence->erase(--(rit.base()));

	
	std::queue<std::string::iterator> itQueue;
	for (std::string::iterator it = ++this->gateSequence->begin(); it != --this->gateSequence->end(); ++it)
	{
		if (*it >= '0' && *it <= '9')
		{
			if (*(it - 1) < '0' || *(it - 1) > '9')
				itQueue.push(it);
			if (*(it + 1) < '0' || *(it + 1) > '9')
				itQueue.push(it + 1);
		}
	}

	std::string::iterator iBegin, iEnd;
	while (!itQueue.empty())
	{
		iBegin = itQueue.front();
		itQueue.pop();
		iEnd = itQueue.front();
		itQueue.pop();

		this->gateSequence->replace(iBegin, iEnd, 1, '%');
	}
}

void CDisplayAdaptor::serializeNetlist()
{
	CNetlistElement *currentElement;
	std::string currentDrain;
	std::string currentSource;
	std::ostringstream convert;

	convert
		<< "\t" << "Symbol" << "\t" << "Drain " << "\t" << "Source" << std::endl
		<< "\t" << "------" << "\t" << "------" << "\t" << "------" << std::endl;

	while (!this->netlist->emptyQueue())
	{
		currentElement = this->netlist->popQueue();

		switch (currentElement->getDrain())
		{
		case 0:
			currentDrain = "Out";
			break;
		case -1:
			currentDrain = "GND";
			break;
		default:
			currentDrain = "N" + std::to_string(currentElement->getDrain());
			break;
		}

		switch (currentElement->getSource())
		{
		case 0:
			currentSource = "Out";
			break;
		case -1:
			currentSource = "GND";
			break;
		default:
			currentSource = "N" + std::to_string(currentElement->getSource());
		}

		convert 
			<< "\t" 
			<< currentElement->getSymbol() << "\t" 
			<< currentDrain << "\t"
			<< currentSource
			<< std::endl;

		delete currentElement;
	}

	*this->netlistString = convert.str();

	currentElement = NULL;
}
