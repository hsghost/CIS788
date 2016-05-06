/*
/
*/

#include "CModulelist.h"

CModule::CModule(int moduleID, double a, double r, double s, bool rotatable) :
moduleID(moduleID), a(a), r(r), s(s), rotatable(rotatable)
{
	;
}

unsigned CModule::getModuleID()
{
	return (this->moduleID);
}

double CModule::getA()
{
	return (this->a);
}

double CModule::getR()
{
	return (this->r);
}

double CModule::getS()
{
	return (this->s);
}

double CModule::getWidth()
{
	return (this->width);
}

double CModule::getHeight()
{
	return (this->height);
}

bool CModule::getRotatable()
{
	return (this->rotatable);
}

CModulelist::CModulelist() :
numModules(0), p(0), q(0)
{
	;
}

unsigned CModulelist::getNumModules()
{
	return (this->numModules);
}

double CModulelist::getP()
{
	return (this->p);
}

double CModulelist::getQ()
{
	return (this->q);
}

const std::vector<CModule*> &CModulelist::getModules()
{
	return (this->modules);
}
