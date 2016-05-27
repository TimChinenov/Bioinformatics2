#include <iostream>     // std::cout
#include <algorithm>    // std::for_each
#include <vector>       // std::vector

class Node
{
public:
	Node() 
	Node(const int & init) : value(init) {}
	~Node();
	void addPointer(Node* & p){
		pointers.push_back(p);
	}
	int vector_position;
	std::list <Node*> pointers;
	int value;
	bool change;
};

void ReverseArcs(std::list<int> pointers, Node & origin){
	if(origin == NULL) return;
	for(std::list<int>::iterator i = pointers.begin(); i != pointers.end(); i++)
	{
		if((*i) != NULL || (i*) != origin || this.setChanged(false)){
			ReverseArcs((i*).pointers, origin);
		}
		n.remove(i);
		pointers.add(origin);
	}
	this.setChanged(true);
	
}