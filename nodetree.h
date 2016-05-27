//Code Written and Produced by Tim Chinenov

#ifndef _NODETREE_H_
#define _NODETREE_H_
#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <stack>
#include <algorithm>
#include <iostream>

class Node
{
public:
	Node(){
		index = 0;
	}
	~Node(){}
	std::list <Node*> pointers;//List of pointers to other nodes
	std::set<std::string> value;//Set of values that are in the node.
	//The node will be initialized with only a single name. After the
	//SCC algorithm is run the set will have multiple names to 
	//represent 'super nodes'

	int index;//Position of node in the graph vertex. 
	//^^still testing idea

};

//Graph contains the Nodes in the format of the inputed vector
//and functions that can be applied on this vector
class Graph{
public:
	//Graph contructor takes in a vector of node*

	Graph(std::vector<Node*> & loose)
	{
		TFTG = loose;
		//the graph object contructed takes the vector of nodes and 
		//creates a map(dictionary) which keeps track of the nodes
		//position in the vector. This will be important when trying to
		//quickly traverse the graph in the depthfirstsearch function 
		for(int i = 0; i < TFTG.size(); i++)
		{
			vectorposition.insert(std::make_pair<std::set<std::string>, int> ((*TFTG[i]).value, i));
		}
	}

	~Graph(){//destructor function. Cleans up memory nodes
		for(int i = 0; i < TFTG.size(); i++)
		{
			delete TFTG[i];
		}
	}
	
	//This is the main SCC function. After the function is run the nodes will be reorganized in a manner 
	//that removes possible cycles in the web of nodes. In removing the cycles, the nodes will
	//be ready to be reversed for the later iterative leaf removal function.
	void SCC_nodes()
	{
		std::vector<std::vector<Node*> > allComp; //vector to hold all the
		//found strongly connected componenets

		int TFTGsize = TFTG.size();//variable for size of node vector

		bool visited [TFTGsize];//This array keeps track of wether a node has been 
		//visited before as we do not want to return to previously
		//visisted nodes.

		for(int i = 0; i < TFTGsize; i++)//set all bools in visited to false
		{
			visited[i] = false;
		}

		std::stack<Node*> theStack;//Stack is necesseray to keep track of which
		//nodes have been passed and are waiting to be placed into a SCC
		int times = 0;//required counter
		int lowlink[TFTGsize];//array that holds finishing times of nodes 

		for(int i = 0; i < TFTGsize;i++)//fill all of lowlink with 0
		{
			lowlink[i] = 0;
		}
		//for loop goes through all the nodes in the vector, visiting them 
		//each one time
		for (int i = 0; i < TFTG.size(); i++)
		{
			if(!visited[i])
				{
					depthfirstsearch(i,times,theStack,visited,lowlink,allComp);
				}
		}//after this for loop finishes we have double vector containing Node pointers 
		//to SCC structures. Now that we have that data we have to compile the nodes
		//super nodes which will contain all the values of the original nodes and all
		//the pointers that were not included in the SCC structure.
		std::cout << "size: " <<  TFTG.size() << "\n"; 
		Consolidate_SCC(allComp);
/*
		for(int i = 0; i < allComp.size(); i++)
		{
			if (allComp[i].size() == 0) std::cout << "problem";
			for(int j = 0; j <allComp[i].size(); j++)
			{
				Node * x = ((allComp[i][j]));
				std::set<std::string> tmpset = (*x).value;
				std::set<std::string>::iterator sitr = tmpset.begin();
				std::string tmp = (*sitr);
				std::cout<< tmp << ", ";
			}
			std::cout<< "\n";
		}
*/
		std::cout << "size: " <<TFTG.size() << "\n";
		
	}
  	
  	//This function is used by the SCC_nodes function to run through the 
  	//nodes. This algorithm along with the original SCC algorithm are 
  	//based on Tarjan's strongly connected componenets algorithm
	void depthfirstsearch (const int & index ,int & times, std::stack<Node*> & stack,
	 bool (visited)[], int (lowlink)[], std::vector<std::vector<Node*> > & allComp)
	{
		Node* n = TFTG[index];
		times++;
		lowlink[index] = times;
		visited[index] = true;
		stack.push(n);///check later
		bool isCR = true;
		std::vector<Node*> comp;

		std::list<Node*>::iterator itr = ((*TFTG[index]).pointers.begin());

		for(;itr != ((*TFTG[index]).pointers.end()); itr++)
		{
			int pos = vectorposition[(**itr).value];
			if(!visited[pos])
				depthfirstsearch(pos,times,stack,visited,lowlink,allComp);
			if(lowlink[index] > lowlink[pos])
			{
				lowlink[index] = lowlink[pos];
				isCR = false;
			}
		}

		if (isCR) {
			while(true)
			{
				Node* x = stack.top();
				stack.pop();
				comp.push_back(x);
				lowlink[vectorposition[(*x).value]] = TFTG.size() + 10;
				if((*x).value.begin() == (*TFTG[index]).value.begin())
					break;
			}

		}
		if (comp.size() != 0)
			allComp.push_back(comp);
	}
 
 	//function used to reogranized the SCC nodes into new nodes
	void Consolidate_SCC(std::vector<std::vector <Node*> > allComp) {
		std::vector<Node*> newvect;///all of this needs to be put into this new vector in the end
		//this function will do passes of consolidation. The first pass will cover SCC's and groups of
		//nodes. The second pass will cover single nodes.
		for(int i = 0; i < allComp.size(); i++)
		{
			if (allComp[i].size() < 2)//if the size of thegroup is less than 2 ignore it
				continue;
			Node* x = new Node();//create a new node
			std::set<std::string> used_pointers;//names of pointers are put into here
			//to avoid repitition
			for(int j = 0; j < allComp[i].size(); j++)
			{//for loop goes through each node in the SCC group
				(*x).value.insert(*((*allComp[i][j]).value.begin()));//we insert the name to
				//new nodes values
			}

			for(int j = 0; j < allComp[i].size(); j++)
			{
				for(std::list<Node*>::iterator k = (*allComp[i][j]).pointers.begin();
				 k != (*allComp[i][j]).pointers.end();k++)//This loop goes through each pointer of the
					//node and adds the pointer to the SCC node
				{
					std::string tmp = *((**k).value.begin());//takes the first name (only name) of the 
					//original node
					if(used_pointers.find(tmp) == used_pointers.end() 
					&& (*x).value.find(tmp) == (*x).value.end()) //checks to make sure that the
						//pointer is not pushed into the node more than once.
					{
						(*x).pointers.push_back(*k);//pushes back new pointer
						used_pointers.insert(tmp);//puts the value into used_pointers
					}
					else
						continue;
				}
			}
			newvect.push_back(x);//place this Node into the replacement vector
		}
		//second pass of the allComp vector.
		for(int i = 0; i < allComp.size(); i++)
		{
			if (allComp[i].size() != 1)//ignore any groups. We want to focus
				//on individuals
				continue;
			//Go through each pointer in the single node
			std::set<std::set<std::string> > used_pointers;
			for(std::list<Node*>::iterator j = (*allComp[i][0]).pointers.begin();
			 j != (*allComp[i][0]).pointers.end();j++)
			{
				
				std::string currentname = *((**j).value.begin());//the name of our targeted node
				for(int k = 0; k < newvect.size(); k++)//Go through the new
					//vector we created in order to find new clusters that have
					//same values as the values of the pointer in this Node.
					//we will switch the pointer from the old single node the cluster
					//as a whole
				{
					if ((*newvect[k]).value.find(currentname) != (*newvect[k]).value.end())
					{
						if(used_pointers.find((*newvect[k]).value) != used_pointers.end())
						{//if there is alread a pointer pointing to this value, do not
					     //push it back into the list.
						 j = (*allComp[i][0]).pointers.erase(j);
						}
						else
						{
							*j = newvect[k];
							used_pointers.insert((*newvect[k]).value);
						}
						
					}
				}
			}
			newvect.push_back(allComp[i][0]);			
		}

		TFTG = newvect;//Make current TFTG into the consilidated SCC
		// map has to be remade
		std::map<std::set<std::string>, int> replacementMap;
		for(int i = 0; i < TFTG.size(); i++)
		{
			replacementMap.insert(std::make_pair<std::set<std::string>, int> ((*TFTG[i]).value, i));
		}
		vectorposition = replacementMap;
		///////////Print for clarification
		for(int i = 0; i < TFTG.size(); i++)
		{	
			std::cout << "values\n";
			for(std::set<std::string>::iterator itr = (*TFTG[i]).value.begin();
				itr != (*TFTG[i]).value.end(); itr++)
			{
				std::cout << (*itr) << ", ";
			}
			std::cout << "\n" << "pointers\n";
			for(std::list<Node*>::iterator itr = (*TFTG[i]).pointers.begin();
				itr != (*TFTG[i]).pointers.end(); itr++)
			{
				for(std::set<std::string>::iterator itr2 = (**itr).value.begin();
				itr2 != (**itr).value.end(); itr2++)
				{
					std::cout << (*itr2) << ", ";
				}
				std::cout << "\n";
			}
		}
		///////////
	}

	void cleanup() //this function cleans up incorrect pointers
	{
		for(int i = 0; i < TFTG.size(); ++i){
			std::list<*Node>::iterator itr;
			for(itr = (*TFTG[i]).pointers.begin(); itr != (*TFTG[i]).pointers.end(); itr++)
			{
				5
			}
		}
	}
	//This function creates a empty set of nodes to store node values
	//and a NULL Node pointer. The function continues by going through 
	//each node in TFTG and passing it into Transpose_Graph2, along with the 
	//Null pointer and set of passed nodes 
	void Transpose()
	{
		//create set of values. This set starts empty and
		// will keep track of nodes that are visited by the 
		//transpose function 
		std::set<std::set<std::string> > nodes_passed;
		Node* p  = NULL;
		for(int i = 0; i < TFTG.size(); i++)
		{
			Transpose_Graph2(p,TFTG[i],nodes_passed);
		}
	}
	void Transpose_Graph2(Node* & Prev, Node* & current, std::set<std::set<std::string> > & visited)
	{
		///Testing seg fault
		for(std::set<std::string>::iterator itr = (*current).value.begin();
			itr != (*current).value.end(); itr++)
		{
			std::cerr << (*itr) << ", ";
		}
		std::cerr << "\n";
		//
		if(current == NULL) return;//either something is wrong or we've
		//reached the end of a node;
		std::cerr << "I work 1\n";
		if(Prev == NULL)//starting case, Prev will be null first run.
		{
			visited.insert((*current).value); //put the current value into the visited array
			for(std::list<Node*>::iterator itr = (*current).pointers.begin();
				itr != (*current).pointers.end(); itr++)
			{//recursively continue through the current nodes pointers.
				Transpose_Graph2(current, *itr, visited);
			}
		}
		else if(visited.find((*Prev).value) != visited.end())//in the case that we have already visited the prev node
		{
			for (std::list<Node*>::iterator itr = (*Prev).pointers.begin(); itr != (*Prev).pointers.end(); itr++)
			{//this for loop runs through all the previous nodes pointers to see if any
				//of them point to the current node
				if (*itr == current)//if they do...
				{//flip the pointer around by
					itr = (*Prev).pointers.erase(itr);//deleting thr current pointer
					Node* x  = Prev;//creating a new pointer that points to Prev
					(*current).pointers.push_back(x);//putting this new pointer into the current node
					visited.insert((*current).value); //insert the current node into the visited set
					return;//go back an iteration
				}
			}
		}
		std::cerr << "I work 3\n";
		//at the end make sure to register that we visited the current value;
		if (Prev != NULL)//make this check to avoid placing the value in visited twice
			{
				visited.insert((*current).value);
			}
		std::cerr << "I work 4\n";
		/*for(std::list<Node*>::iterator itr = (*current).pointers.begin();
			itr != (*current).pointers.end(); itr++)
		{//again, transpose through each pointer in the current node again.
			Transpose_Graph2(current, *itr , visited);
		}
		
		for (std::list<Node*>::iterator itr = (*Prev).pointers.begin();
				itr != (*Prev).pointers.end(); itr++)
		{std::cerr << "I work 5\n";
			if (*itr == current)
				//this for loop runs through all the previous nodes pointers to see if any
				//of them point to the current node
			{
				itr = (*Prev).pointers.erase(itr);
				Node* x  = Prev;
				(*current).pointers.push_back(x);
				return;
			}
		}*/
		std::cerr << "I work 6\n";
	}

private:
	//Graph contains a vector of all the Node sturcutes.
	std::vector<Node*> TFTG;
	//^^still testing whether to keep as Node or Node*

	//a map which gives quick access of what vector position a certain
	//node is in in std::vector<Node> TFTG.
	std::map<std::set<std::string>, int> vectorposition;
	//^^ this idea is in testing but still seems promising

};



#endif


/*
	void Transpose_Graph(){
		std::set<std::string> complete;
		for(int i = 0; i < TFTG.size(); i++)
		{
			for(std::list<Node*>::iterator j = TFTG[i].pointers.begin; j != TFTG[i].pointers.end(); j++)
			{	
				std::string name = (j*).value; ///name of the node the pointer is pointing to
				if (complete.find(name) == complete.end()) continue;//if it is completed go on to the next pointer
				int position = vectorposition.find(name);

				assert(i != position);//make sure pointer isn't pointing at itself
				Node* temp = TFTG[i];
				TFTG[position].addPointer(temp);
				j = TFTG[i].pointers.erase(j);
				--j;
			}
			complete.insert(TFTG[i].value);
		}
	}
*/
