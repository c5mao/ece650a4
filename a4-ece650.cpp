#include <iostream>
#include <vector>
#include <string.h>
#include <bits/stdc++.h> 
#include <regex>
#include "minisat/core/Solver.h"

using namespace std; 
/*****************************************************
Global Variable
******************************************************/
#define NO_ERROR         0
#define INVALID_INPUT    1
#define INPUT_DONE       2

typedef struct edge{
	int src;
	int dst;
}edge_t;



regex re("<.*?>");
regex num(R"(\d+)");
sregex_iterator reg_end;
// no. of vertices 
int v = 1; 
vector<edge_t> edge_obj;
/*****************************************************
Function Prototype
******************************************************/ 
//Function to parse the input
int parse_line(string line){
    int error=NO_ERROR;
    unsigned int i;
    //for debug
    //cout << line[0] << endl;
    if (line[0]=='V'){
    	// Find position of ':' using find() 
        int pos = line.find(" "); 
        // Copy substring after pos 
        string value = line.substr(pos + 1); 
        v = stoi(value);
        edge_obj.clear();
        error = NO_ERROR;
    }else if (line[0]=='E'){
    	try {
    	    // Find position of ':' using find() 
            int pos = line.find(" "); 
            // Copy substring after pos 
            string str = line.substr(pos + 1); 
            regex_token_iterator<string::iterator> rend;
            sregex_iterator it(str.begin(), str.end(), re);
            int vertices[2]={0,0};
            for (; it != reg_end; ++it) {
            	i=0;
            	string s=it->str();
            	sregex_iterator iter(s.begin(), s.end(), num);
                while(iter != reg_end){
                	if (i >= 2){
                		return INVALID_INPUT;
        	        }
                	vertices[i]= stoi((*iter)[0]);
                	if(vertices[i] >= v){
                		return INVALID_INPUT;
                	}
                    i++;
                    ++iter;
                }
                edge_t sub_edge;
                sub_edge.src=vertices[0];
                sub_edge.dst=vertices[1];
                edge_obj.push_back(sub_edge);
                cout << "<" << edge_obj[edge_obj.size()-1].src << "," << edge_obj[edge_obj.size()-1].dst << ">" << endl;
            }             
        } catch (...) {
            // Syntax error in the regular expression
            cout << "Error: unexpect errors" << endl;
            error = INVALID_INPUT;
        }
        error = INPUT_DONE;
    }else{
    	error = INVALID_INPUT;
    }

    return error;

}

int reduction(int vertex, vector<edge_t> edge){
	unsigned int i,j,p,q;
	unsigned int n=vertex;
	unsigned int k;
	int *x=NULL;
	unsigned int no_of_clauses;

	for(i=0;i<edge.size();i++){
		cout << "<" << edge[i].src << "," << edge[i].dst << ">";
	}
	cout << endl;

    //loop k from 1 to n to find the minimal vertex cover
	for(k=1;k<=n;k++){
		delete [] x;
		x=new int[n*k];
		for (i=0;i<n*k;i++){
			x[i]=(int)i+1;
			//cout << x[i];
		}
		cout << endl;
		no_of_clauses=k+edge.size()+n*k*(n+k-2)/2;
		cout << "p cnf " << n*k << " " << no_of_clauses << endl;
		//first set of clauses
		for (j=0;j<k;j++){
			for (i=0;i<n;i++){
			    cout << x[i+j*n] << " ";
			}
            cout << "0" << endl;
        }
	    //second set of clauses
	    for (i=0;i<n;i++){
	    	for (p=0;p<k;p++){
	    		for (q=p+1;q<k;q++){
	    			cout << (-1)*x[i+p*n] << " " << (-1)*x[i+q*n] << " 0" << endl;
	    		}
	    	}
	    }
	    //third set of clauses
	    for (j=0;j<k;j++){
	    	for (p=0;p<n;p++){
	    		for (q=p+1;q<n;q++){
	    			cout << (-1)*x[p+j*n] << " " << (-1)*x[q+j*n] << " 0" << endl;
	    		}
	    	}
	    }
	    //fourth set of clauses
	    for (p=0;p<edge_obj.size();p++){
	    	for (q=0;q<k;q++){
	    		cout << x[edge_obj[p].src+q*n] << " " << x[edge_obj[p].dst+q*n] << " ";
	    	}
	    	cout << "0" << endl;
	    }

	    int vc_value=4;
	    int vc_v;
	    if (vc_value>0){
	    	vc_v=(vc_value-1)%n+1;
	    }
	    cout << vc_v << endl;
       
	}




    return 0;
}

int main(){
	string line;
	int error=NO_ERROR;

	using Minisat::mkLit;
    using Minisat::lbool;

    Minisat::Solver solver;
    // Create variables
    auto A = solver.newVar();
    auto B = solver.newVar();
    auto C = solver.newVar();
    
    // Create the clauses
    solver.addClause( mkLit(A),  mkLit(B),  mkLit(C));
    solver.addClause(~mkLit(A),  mkLit(B),  mkLit(C));
    solver.addClause( mkLit(A), ~mkLit(B),  mkLit(C));
    solver.addClause( mkLit(A),  mkLit(B), ~mkLit(C));
    
    // Check for solution and retrieve model if found
    auto sat = solver.solve();
    if (sat) {
        std::clog << "SAT\n"
                  << "Model found:\n";
        std::clog << "A := " << (solver.modelValue(A) == l_True) << '\n';
        std::clog << "B := " << (solver.modelValue(B) == l_True) << '\n';
        std::clog << "C := " << (solver.modelValue(C) == l_True) << '\n';
    } else {
        std::clog << "UNSAT\n";
        return 1;
    }

	while(1){
	    try{
   	        getline(cin, line);
            if(!cin){
               if(cin.eof())
                 break;             
            }
            error = parse_line(line);
            if (error == NO_ERROR){
        	    cout << "Error: Parse Successful" << endl;
            }else if (error == INVALID_INPUT){
        	    cout << "Error: Invalid Command" << endl;
            }else if (error == INPUT_DONE){
        	    cout << "Error: Input Done, Do Vertex Cover" << endl;
        	    reduction(v, edge_obj);
            }
	    }catch(...){
	    	cout << "Error: unexpected error" << endl;
	    }
	}
	return 0;

}
