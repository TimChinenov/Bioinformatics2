/*Program is designed to run a vertex sort algorithm 
to organize something TF's and TG's

Written by Tim Chinenov*/
#include "nodetree.h"
#include <cassert>
#include <iostream>
#include <fstream>

//Function Prototypes
void Organize_Input(std::ifstream & in_str, std::vector<Node> & TFTG);
void Output(std::vector<Node*> & answer,  std::ofstream & out_str);

//This function will be used to ogranzie inputed data/text/etc...
void Organize_Input(std::ifstream & in_str, std::vector<Node*> & TFTG)
{
  std::string TF;//String holds the TF
  std::string TG;//string holds the TG
  std::string pTF;
  int pnumb;
  while(in_str >> TF)//start by taking in the next TF
  {
    in_str >> TG;//Then take in the TG
    if(TF == "TF" || TG == "TG")//IF the inputed text is the first line
      //then ignore it and go onto the next line
    {
      continue;
    }
    //std::cout << TF << " " << TG << "\n";
    //for loop goes through vector of existing nodes.
    //if existing node is found then a new pointer is added
    //to that existing node. Otherwise a new node is created.
    bool foundTF = false;
    for(int i = 0; i < TFTG.size(); ++i)
    {
      //if the node is found, then the TG has to be found as well.
      if((*TFTG[i]).value.find(TF) != (*TFTG[i]).value.end())
      { 
        foundTF = true;
        bool foundTG = false;
        for(int j = 0; j < TFTG.size(); ++j)
        {
          //Two conditions can come out, if the node is found
          //then add a pointer to the TG which points to the TF.
          if((*TFTG[j]).value.find(TG) != (*TFTG[j]).value.end())//node is found
          {
            foundTG = true;
            Node* n = TFTG[j];
            (*TFTG[i]).pointers.push_back(n);
            break;
          }
        }
        //If the TG is not found then, a new node with the TG
        //has to be created.
        if(!foundTG)
        {
          Node* n = new Node;
          (*n).value.insert(TG);
          (*TFTG[i]).pointers.push_back(n);
          TFTG.push_back(n);
        }
        break;
      }
    }
    //If the TF is not found in the vector, then it has to be created.
    if(!foundTF)
    {
      Node* n = new Node;
      (*n).value.insert(TF);
      TFTG.push_back(n);
      //algorithm for finding the TG needs to be repeated
      bool foundTG = false;
      for(int j = 0; j < TFTG.size(); ++j)
      {
        //Two conditions can come out, if the node is found
        //then add a pointer to the TF which points to the TG.
        if((*TFTG[j]).value.find(TG) != (*TFTG[j]).value.end())
        {
          Node* sn = TFTG[j];
          (*n).pointers.push_back(sn);
          foundTG = true;
          break;
        } 
        //If the TG is not found then, a new node with the TG
        //has to be created.
      }
      if(!foundTG)
      {
        Node* sn = new Node;
        (*sn).value.insert(TG);
        (*n).pointers.push_back(sn);
        TFTG.push_back(sn);
      }
    }
  }
}

void Output(std::vector<Node*> & answer, std::ofstream & out_str)
{
  for(int i = 0; i < answer.size(); i++)
  {
    out_str << "TF of Node\n";
    for(std::set<std::string>::iterator j = (*answer[i]).value.begin(); j !=(*answer[i]).value.end(); j++)
    {
      out_str << (*j) << "\n";
    }
    out_str << "TG of Node\n";
    for(std::list<Node*>::iterator j = (*answer[i]).pointers.begin(); j !=(*answer[i]).pointers.end(); j++)
    {
      for(std::set<std::string>::iterator k = (*(*j)).value.begin(); k !=(*(*j)).value.end(); k++)
      {
        out_str << (*k) << "\n";
      }
    }
  }
}

//all user requested actions will be performed here...
int main(int argc, char* argv[]){
	assert(argc = 3);

	std::ifstream in_str(argv[1]);
	if (!in_str.good()) 
	{
    	std::cerr << "Can't open " << argv[1] << " to read.\n";
    	return 1; 
  }
  std::ofstream out_str(argv[2]);
  if (!out_str.good()) 
  {
    std::cerr << "Can't open " << argv[2] << " to write.\n";
    return 1; 
  }
    std::vector<Node*> TFTG;
    Organize_Input(in_str, TFTG);
    Output(TFTG,out_str);
    
    Graph Graph1 (TFTG);
    Graph1.SCC_nodes();
    std::cout << "\n\n\n";
   // Graph1.Transpose();
}

   
