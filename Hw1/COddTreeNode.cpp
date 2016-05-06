/*
/
*/

#include <queue>
#include "COddTreeNode.h"

COddTreeNode::COddTreeNode()
{
	this->triType = Undetermined;
	this->symbol = '$';
	this->parent = NULL;
	for (int i = 0; i < MAX_CLAUSE; i++)
		this->children[i] = NULL;
	this->numChildren = 0;
	this->siblingIndex = 0;
	this->reversed = false;
	this->drain = 0;
	this->source = -1;
}

COddTreeNode::~COddTreeNode()
{
	if (this->parent != NULL)
	{
		for (int i = this->siblingIndex; i + 1 < MAX_CLAUSE && this->parent->children[i] != NULL; i++)
		{
			this->parent->children[i] = this->parent->children[i + 1];
			if (this->parent->children[i] != NULL)
				this->parent->children[i]->siblingIndex--;
		}
		this->parent->numChildren--;
		this->parent = NULL;
	}
	else
	{
		; // This is root, nothing to do now
	}
}

char COddTreeNode::getSymbol()
{
	return this->symbol;
}

void COddTreeNode::deleteNode()
{
	delete this;
}

void COddTreeNode::flipChildren()
{
	COddTreeNode *childrenTemp;
	for (int i = 0; i <= this->numChildren / 2; i++)
	{
		childrenTemp = this->children[i];
		this->children[i] = this->children[this->numChildren - 1 - i];
		this->children[this->numChildren - 1 - i] = childrenTemp;
	}
}

void COddTreeNode::sortChildren()
{
	std::queue<COddTreeNode *> psuedoQueue, realQueue, tophalfQueue, bottomhalfQueue;
	int zone = 0;

	for (int i = 0; i < this->numChildren; i++)
	{
		switch (this->children[i]->triType)
		{
		case Real:
			realQueue.push(this->children[i]);
			break;
		case BottomHalf:
			bottomhalfQueue.push(this->children[i]);
			break;
		case TopHalf:
			tophalfQueue.push(this->children[i]);
			break;
		case Psuedo:
			psuedoQueue.push(this->children[i]);
			break;
		default:
			break;
		}
	}

	int i = 0;
	while (i < this->numChildren)
	{
		if (!psuedoQueue.empty())
		{
			this->children[i] = psuedoQueue.front();
			psuedoQueue.pop();
			this->children[i]->siblingIndex = i;
		}
		else if (zone == 0)
		{
			zone++;
			continue;
		}
		else if (zone == 1)
		{
			zone++;
			if (!bottomhalfQueue.empty())
			{
				this->children[i] = bottomhalfQueue.front();
				bottomhalfQueue.pop();
				this->children[i]->siblingIndex = i;
			}
			else if (!tophalfQueue.empty())
			{
				this->children[i] = tophalfQueue.front();
				tophalfQueue.pop();
				this->children[i]->siblingIndex = i;
				this->children[i]->flipChildren();
			}
			else
				continue;
		}
		else if (zone == 2)
		{
			if (!realQueue.empty())
			{
				this->children[i] = realQueue.front();
				realQueue.pop();
				this->children[i]->siblingIndex = i;
			}
			else
			{
				zone++;
				continue;
			}
		}
		else if (zone == 3)
		{
			zone++;
			if (!tophalfQueue.empty())
			{
				this->children[i] = tophalfQueue.front();
				tophalfQueue.pop();
				this->children[i]->siblingIndex = i;
			}
			else if (!bottomhalfQueue.empty())
			{
				this->children[i] = bottomhalfQueue.front();
				bottomhalfQueue.pop();
				this->children[i]->siblingIndex = i;
				this->children[i]->flipChildren();
			}
			else
				continue;
		}
		else if (zone == 4)
		{
			if (this->children[i - 1]->triType == BottomHalf)
			{
				if (!tophalfQueue.empty())
				{
					this->children[i] = tophalfQueue.front();
					tophalfQueue.pop();
					this->children[i]->siblingIndex = i;
				}
				else if (!bottomhalfQueue.empty())
				{
					this->children[i] = bottomhalfQueue.front();
					bottomhalfQueue.pop();
					this->children[i]->siblingIndex = i;
					this->children[i]->flipChildren();
				}
				else
					continue;
			}
			else if (this->children[i - 1]->triType == TopHalf)
			{
				if (!bottomhalfQueue.empty())
				{
					this->children[i] = bottomhalfQueue.front();
					bottomhalfQueue.pop();
					this->children[i]->siblingIndex = i;
				}
				else if (!tophalfQueue.empty())
				{
					this->children[i] = tophalfQueue.front();
					tophalfQueue.pop();
					this->children[i]->siblingIndex = i;
					this->children[i]->flipChildren();
				}
				else
					continue;
			}
		}
		else
			;// throw some exception
		i++;
	}
}

void COddTreeNode::calculateTriType()
{
	if (this->triType == Undetermined)
	{
		if (this->numChildren == 0)
			;//error
		else
		{
			// Arrangements
			this->sortChildren();

			this->topHalf = this->children[0]->topHalf;
			this->bottomHalf = this->children[this->numChildren - 1]->bottomHalf;
		}

		if (this->topHalf == h_Real && this->bottomHalf == h_Real)
			this->triType = Real;
		else if (this->topHalf == h_Psuedo && this->bottomHalf == h_Real)
			this->triType = BottomHalf;
		else if (this->topHalf == h_Real && this->bottomHalf == h_Psuedo)
			this->triType = TopHalf;
		else if (this->topHalf == h_Psuedo && this->bottomHalf == h_Psuedo)
			this->triType = Psuedo;
	}
}

void COddTreeNode::calculateDriection()
{
	if (this->symbol == '*')
	{
		this->children[0]->reversed = this->reversed;
		for (int i = 1; i <= this->numChildren / 2; i++)
		{
			this->children[2 * i - 1]->reversed = !this->reversed;
			this->children[2 * i]->reversed = this->reversed;
		}
	}
}

void COddTreeNode::calculateNet(int *netCount)
{
	switch (this->symbol)
	{
	case '*':
		for (int i = 0; i < this->numChildren; i++)
		{
			this->children[i]->drain = this->drain;
			this->children[i]->source = this->source;
		}
		break;
	case '+':
		if (this->reversed)
		{
			this->children[this->numChildren - 1]->drain = this->drain;
			this->children[this->numChildren - 1]->source = (this->children[this->numChildren - 1]->symbol >= '0' && this->children[this->numChildren - 1]->symbol <= '9') ? this->drain : ++*netCount;
			for (int i = this->numChildren - 2; i > 0; i--)
			{
				this->children[i]->drain = this->children[i + 1]->source;
				this->children[i]->source = ++*netCount;
			}
			this->children[0]->drain = (this->children[0]->symbol >= '0' && this->children[0]->symbol <= '9') ? this->source : this->children[1]->source;
			this->children[0]->source = this->source;
			if (this->children[1]->source != this->children[0]->drain)
				this->children[1]->source = this->children[0]->drain;
		}
		else
		{
			this->children[0]->drain = this->drain;
			this->children[0]->source = (this->children[0]->symbol >= '0' && this->children[0]->symbol <= '9') ? this->drain : ++*netCount;
			for (int i = 1; i < this->numChildren - 1; i++)
			{
				this->children[i]->drain = this->children[i - 1]->source;
				this->children[i]->source = ++*netCount;
			}
			this->children[this->numChildren - 1]->drain = (this->children[this->numChildren - 1]->symbol >= '0' && this->children[this->numChildren - 1]->symbol <= '9') ? this->source : this->children[this->numChildren - 2]->source;
			this->children[this->numChildren - 1]->source = this->source;
			if (this->children[this->numChildren - 2]->source != this->children[this->numChildren - 1]->drain)
				this->children[this->numChildren - 2]->source = this->children[this->numChildren - 1]->drain;
		}
		break;
	default:
		break;
	}
}

void COddTreeNode::getNetInfo(CNetlistElement *netlistElement)
{
	netlistElement->symbol = this->symbol;
	netlistElement->drain = this->drain;
	netlistElement->source = this->source;
}

