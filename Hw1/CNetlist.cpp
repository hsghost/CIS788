/*
/
*/

#include "CNetlist.h"

char CNetlistElement::getSymbol()
{
	return (this->symbol);
}

int CNetlistElement::getDrain()
{
	return (this->drain);
}

int CNetlistElement::getSource()
{
	return (this->source);
}

CNetlist::CNetlist()
{
	this->netlistQ = new std::queue<CNetlistElement*>();
}

CNetlist::~CNetlist()
{
	while (!this->netlistQ->empty())
	{
		delete this->netlistQ->front();
		this->netlistQ->front() = NULL;
	}

	delete this->netlistQ;
	this->netlistQ = NULL;
}

void CNetlist::pushQueue(CNetlistElement *netlistElement)
{
	this->netlistQ->push(netlistElement);
}

CNetlistElement *CNetlist::popQueue()
{
	CNetlistElement *front = this->netlistQ->front();
	this->netlistQ->pop();
	return (front);
}

bool CNetlist::emptyQueue()
{
	return (this->netlistQ->empty());
}
