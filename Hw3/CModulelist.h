/*
/
*/

#include <vector>

#ifndef __CModulelist_H__
#define __CModulelist_H__

class CModule
{
	friend class CSlicingTreeNode;
	friend class CCornerpoint;
	friend class CModulelistFileParser;

public:
	CModule(int moduleID, double a, double r, double s, bool rotatable);
	// ~CModule();

private:
	unsigned moduleID;
	double a, r, s;
	double width, height;
	bool rotatable;
	std::tuple<double, double> *optimalShape;

public:
	unsigned getModuleID();
	double getA();
	double getR();
	double getS();
	double getWidth();
	double getHeight();
	bool getRotatable();
};

class CModulelist
{
	friend class CModulelistFileParser;

public:
	CModulelist();
	// ~CModulelist();

private:
	unsigned numModules;
	double p, q;
	std::vector<CModule*> modules;

public:
	unsigned getNumModules();
	double getP();
	double getQ();
	const std::vector<CModule*> &getModules();
};

#endif