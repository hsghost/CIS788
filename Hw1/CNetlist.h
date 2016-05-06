/*
/
*/

#include <queue>

#ifndef __CNetlist_H__
#define __CNetlist_H__

class CNetlistElement
{
	friend class CNetlist;
	friend class COddTreeNode;

private:
	char symbol;
	int drain;
	int source;

public:
	char getSymbol();
	int getDrain();
	int getSource();
};

class CNetlist
{
public:
	CNetlist();
	~CNetlist();

private:
	std::queue<CNetlistElement*> *netlistQ;

public:
	void pushQueue(CNetlistElement *netlistElement);
	CNetlistElement *popQueue();
	bool emptyQueue();
};

#endif
