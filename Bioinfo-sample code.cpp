//The following code is section of the nodetree.h code
//and shows a section of the Graph class. Due to research 
//reasons I am not able to show the entire code but have 
//included functions that required much personal studying, such 
//as Tarjan's algorithm for SCC, depthfirst seearch algorithms,
// and consolidation of algorithm to create super nodes!
//Keep in mind, this class is part of active research and is
//still a work in progress, yet it is the most complex code
//I have worked on to date and wanted to present it to the company.


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
		std::cout << "size: " <<TFTG.size() << "\n";
		
	}
  	
  	//This function is used by the SCC_nodes function to run through the 
  	//nodes. This algorithm along with the original SCC algorithm are 
  	//based on Tarjan's strongle connected componenets algorithm
	void depthfirstsearch (const int & index ,int & times, std::stack<Node*> & stack,
	 bool (visited)[], int (lowlink)[], std::vector<std::vector<Node*> > & allComp)
	{
		Node* n = TFTG[index];
		times++;
		lowlink[index] = times;
		visited[index] = true;
		stack.push(n);
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
		///////////
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
