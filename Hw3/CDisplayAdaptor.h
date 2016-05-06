/*
/
*/

#include "CSlicingTree.h"

#ifndef __CDisplayAdaptor_H__
#define __CDisplayAdaptor_H__

class CDisplayAdaptor
{
public:
	CDisplayAdaptor(CSlicingTree *slicingTree);
	// ~CDisplayAdaptor();

private:
	CSlicingTree *slicingTree;
};

#endif
