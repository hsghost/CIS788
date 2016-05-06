/*
/
*/

#include <iostream>
#include "COddTreeBuilder.h"

COddTreeBuilderException::COddTreeBuilderException(int e)
{
	this->eNum = e;
}

COddTreeBuilder::COddTreeBuilder(COddTree *tree)
{
	this->oddTree = tree;
	this->psuedoCount = '0';
	this->e = NULL;
	this->parStack = new std::stack < char > ;
}

COddTreeBuilder::~COddTreeBuilder()
{
	this->oddTree = NULL;
	delete this->e;
	this->e = NULL;
	delete this->parStack;
	this->parStack = NULL;
}

void COddTreeBuilder::buildOddTree(std::string *inputExp)
{
	try
	{
		COddTreeNode *current = this->oddTree->root;

		for (unsigned int i = 0; i < inputExp->length(); i++)
		{
			if (!(((*inputExp)[i] >= '(' && (*inputExp)[i] <= '+') || ((*inputExp)[i] >= 'A' && (*inputExp)[i] <= 'Z') || ((*inputExp)[i] == '~')))
				throw this->e = new COddTreeBuilderException(1);
			switch ((*inputExp)[i])
			{
			case '~':
				break;
			case '(':
				if (i > 0 && !(((*inputExp)[i - 1]) >= '*' && ((*inputExp)[i - 1]) <= '+' || ((*inputExp)[i - 1]) == '(' || ((*inputExp)[i - 1]) == '~'))
					throw this->e = new COddTreeBuilderException(2);
				this->parStack->push((*inputExp)[i]);
				if (current->numChildren > MAX_CLAUSE - 1)
					throw this->e = new COddTreeBuilderException(4);
				current->children[current->numChildren] = new COddTreeNode();
				current->children[current->numChildren]->parent = current;
				current->children[current->numChildren]->siblingIndex = current->numChildren;
				current = current->children[current->numChildren];
				break;
			case ')':
				if (i > 0 && !(((*inputExp)[i - 1]) >= 'A' && ((*inputExp)[i - 1]) <= 'Z' || ((*inputExp)[i - 1]) == ')'))
					throw this->e = new COddTreeBuilderException(2);
				if (!this->parStack->empty())
					this->parStack->pop();
				else
					throw this->e = new COddTreeBuilderException(3);
				if (current->numChildren > 5)
					throw this->e = new COddTreeBuilderException(4);
				if (current->numChildren % 2 == 0 && current->numChildren < MAX_CLAUSE)
				{
					if (++this->psuedoCount > '9')
						throw this->e = new COddTreeBuilderException(5);
					else
					{
						current->children[current->numChildren] = new COddTreeNode();
						current->children[current->numChildren]->parent = current;
						current->children[current->numChildren]->siblingIndex = current->numChildren;
						current->children[current->numChildren]->triType = Psuedo;
						current->children[current->numChildren]->topHalf = h_Psuedo;
						current->children[current->numChildren]->bottomHalf = h_Psuedo;
						current->children[current->numChildren]->symbol = this->psuedoCount;
						current->numChildren++;
					}
				}
				current = current->parent;
				current->numChildren++;
				break;
			case '+': case '*':
				if (i > 0 && !(((*inputExp)[i - 1]) >= 'A' && ((*inputExp)[i - 1]) <= 'Z' || ((*inputExp)[i - 1]) == ')'))
					throw this->e = new COddTreeBuilderException(2);
				current->symbol = (*inputExp)[i];
				break;
			default:
				if (i > 0 && (((*inputExp)[i - 1]) >= 'A' && ((*inputExp)[i - 1]) <= 'Z' || ((*inputExp)[i - 1]) == ')'))
					throw this->e = new COddTreeBuilderException(2);
				if (current->numChildren > MAX_CLAUSE - 1)
					throw this->e = new COddTreeBuilderException(4);
				current->children[current->numChildren] = new COddTreeNode();
				current->children[current->numChildren]->parent = current;
				current->children[current->numChildren]->siblingIndex = current->numChildren;
				current->children[current->numChildren]->triType = Real;
				current->children[current->numChildren]->topHalf = h_Real;
				current->children[current->numChildren]->bottomHalf = h_Real;
				current->children[current->numChildren]->symbol = (*inputExp)[i];
				current->numChildren++;
				break;
			}
		}
		if (!this->parStack->empty())
			throw this->e = new COddTreeBuilderException(3);
	}
	catch (COddTreeBuilderException *e)
	{
		std::cerr << "COddTreeBuilderException " << e->eNum << std::endl;
	}
	catch (...)
	{
		this->e->eNum = -1;
		std::cerr << "COddTreeBuilderException " << this->e->eNum << std::endl;
	}
}
