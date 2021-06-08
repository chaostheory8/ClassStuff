#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <map>
#include <stdio.h> 
#include <stdlib.h> 
#include <bits/stdc++.h>  
#include <unistd.h>
#include <time.h>
#include <chrono> 


using namespace std;

vector<vector<int>> alignment;
vector<vector<string>> trace;
vector<vector<string>> bifurcation;
string data;
vector<string> sequences;
vector<string> output;
//vector<string> output2;
unordered_map<string, int> bp_vals; 
unordered_map<string, int> cbp_vals; 
vector<double> durations;

int pairing(char top_val, char side_val, char version)
{
	if(version == 'Y')
	{
		int result = 0;
		string pair = "";
		pair+=top_val;
		pair+=side_val;

		result = bp_vals[pair];
		return result;
	}
	else if(version == 'N')
	{
		int result = 0;
		string pair = "";
		pair+=top_val;
		pair+=side_val;

		result = cbp_vals[pair];
		return result;
	}
	else
	{
		cout<<"Error\n";
		return -1;
	}
	
}

//remember i = side and j = top
int compute_cell(int i, int j, char version)
{
	trace[i][j] = "";
	trace[i][j] = "";
	//alignment[i][j+1] = 9;
	int left = alignment[i][j-1];
	int down = alignment[i+1][j];
	int val = alignment[i+1][j-1];
	//check if complement
	char side_val = data.at(i);
	char top_val = data.at(j-1);
	//cout<<top_val<<" "<<side_val<<endl;

	int calc = pairing(top_val, side_val, version);
	if(calc != 0)
	{
		alignment[i][j] = val+calc;
		trace[i][j] += "/";
	}

	if(left >= down)
	{
		if(left >= alignment[i][j])
		{
			alignment[i][j] = left;
			trace[i][j] += "<";
		}
	}
	if (down >= left)
	{
		if(down >= alignment[i][j])
		{
			alignment[i][j] = down;
			trace[i][j] += "|";
		}
		
	}

	//bifurcation
	int track_val = 0;
	int track_i1;
	int track_i2;
	int track_j1;
	int track_j2;
	vector<string> options;
	for(int i_iter = i+1; i_iter<j-1;i_iter++)
	{
		int bif = alignment[i][i_iter]+alignment[i_iter][j];
		//cout<<bif<<endl;
		//cout<<" "<<alignment[i][i_iter]<<endl<<" "<<alignment[i_iter][j]<<endl;
		string bif_trace = "";
		if(bif>=track_val)
		{
			options.clear();
			track_val=bif;
			track_i1 = i;
			track_j1 = i_iter;
			track_i2 = i_iter;
			track_j2 = j;

			//string bif_trace = "";
			bif_trace+=to_string(track_i1);
			bif_trace+=",";
			bif_trace+=to_string(track_j1);
			bif_trace+=" ";
			bif_trace+=to_string(track_i2);
			bif_trace+=",";
			bif_trace+=to_string(track_j2);
			bif_trace+=" | ";
			options.push_back(bif_trace);
		}
		else if(bif==track_val)
		{
			//track_val=bif;
			track_i1 = i;
			track_j1 = i_iter;
			track_i2 = i_iter;
			track_j2 = j;

			//string bif_trace = "";
			bif_trace+=to_string(track_i1);
			bif_trace+=","; 	
			bif_trace+=to_string(track_j1);
			bif_trace+=" ";
			bif_trace+=to_string(track_i2);
			bif_trace+=",";
			bif_trace+=to_string(track_j2);
			bif_trace+=" | ";
			options.push_back(bif_trace);
		}

	}

	if(alignment[i][j]<=track_val && track_val > 0)
	{
		alignment[i][j] = track_val;
		trace[i][j] += "B";
		for(int x = 0; x<options.size(); x++)
		{
			bifurcation[i][j] += options.at(x);
		}
		//cout<<bifurcation[i][j]<<endl;
	}

	return 0;
}

string sanitize(string input, int check)
{
	int kill = 0;
	for(int x = 0; x<input.size(); x++)
	{
		char thing = input.at(x);
		switch(thing)
		{
			case 'A':
				//fine
				break;
			case 'U':
				//fine
				break;
			case 'G':
				//fine
				break;
			case 'C':
				//fine
				break;
			case 'T':
				kill = 1;
				input.at(x) = 'U';
				break;
			default:
				if(check == 1)
				{
					kill = 1;
				}
				input.erase(x,0);
		}
	}
	if(kill == 1 && check == 1)
	{
		return "";
	}
	else
	{
		return input;
	}
}

string traceback(int i, int j, string read, string dot_bracket, string report, string seq, char version)
{
	string temp;
	
	if(i >= j)
	{
		if(report == "")
		{
			output.push_back(dot_bracket);
			
			cout<<seq<<endl;
			cout<<dot_bracket<<"\n"<<read<<endl;
			cout<<"-------------------------------------------\n";
		}		
		return dot_bracket;
	}
	else
	{
	
		if(alignment[i][j] == alignment[i][j-1])
		{
			temp = read;
			temp += '<';
			traceback(i,j-1,temp,dot_bracket, report, seq, version);
			temp = "";
		}
		if(alignment[i+1][j] == alignment[i][j])
		{
			temp = read;
			temp += '|';
			traceback(i+1,j,temp,dot_bracket, report, seq, version);
			temp = "";
		}
		int match = pairing(data.at(i), data.at(j-1), version);
		if(alignment[i][j] == alignment[i+1][j-1]+match && match != 0)
		{
			temp = read;
			temp += '/';
			
			dot_bracket.replace(j-1,1,")");
			dot_bracket.replace(i,1,"(");
			traceback(i+1,j-1,temp,dot_bracket, report, seq, version);
			temp = "";
		}
		
		for(int iter = i+1; iter<j; iter++)
		{

			if(alignment[i][j] <= alignment[i][iter] + alignment[iter][j])
			{
				temp = read;
				temp += 'B';
				if(iter != j-1 && iter != i)
				{
					if((dot_bracket.at(i) == '(' || dot_bracket.at(iter) == ')') || (dot_bracket.at(iter) == '(' || dot_bracket.at(j-1) == ')'))
					{
						// cout<<"AAAAAAAA!\n";
						// cout<<i<<endl<<iter<<endl;
						// cout<<temp<<endl<<dot_bracket<<endl<<endl;
						//return "";
					}
					else
					{
						string concat;
						concat = traceback(i,iter,temp,dot_bracket, "1", seq, version);
						traceback(iter,j,temp,concat, report, seq, version);
					}										
				}
				temp = "";
				break;
			}
		}
		return dot_bracket;
	}
	//cout<<read<<endl<<dot_bracket<<endl;
}



int main()
{
	bp_vals["AA"] = 0;
	bp_vals["AC"] = 1;
	bp_vals["AG"] = 0;
	bp_vals["AU"] = 2;

	bp_vals["CA"] = 1;
	bp_vals["CC"] = 0;
	bp_vals["CG"] = 2;
	bp_vals["CU"] = 0;

	bp_vals["GA"] = 0;
	bp_vals["GC"] = 2;
	bp_vals["GG"] = 0;
	bp_vals["GU"] = 1;

	bp_vals["UA"] = 2;
	bp_vals["UC"] = 0;
	bp_vals["UG"] = 1;
	bp_vals["UU"] = 0;

	cbp_vals["AU"] = 1;
	cbp_vals["CG"] = 1;
	cbp_vals["GC"] = 1;
	cbp_vals["UA"] = 1;

    string answer;
	cout << "What is the input file?"<<endl;
	cin >> answer;

	int checkin;
	cout << "Would you like to attempt to repair invalid nucleotides (0), or reject sequences with errors (1)?"<<endl;
	cin >> checkin;

	char version;
	cout << "Would you like to include non-canonical base pairings? (Y/N)"<<endl;
	cin >> version;

	int min_gap = 0;
	// cout << "What is the minimal number of enclosed positions? (Select between 0 and 5)"<<endl;
	// cin >> min_gap;

    //read in file w/ two sequences
    ifstream myfile (answer); //"pairwise_nucleotide_1.fasta"
	string temp ="";
	if (myfile.is_open())
    {
        cout<<"open\n";
        string line;
        string temp;
        string input = "";
        string name = "";
        int check = 0;
        int seq_iter = 0;
        while(getline(myfile, line))
        {
            temp = line.back();
            if(temp == "\r")
            {
                line.pop_back();
            }
            if(line.size() > 0)
            {
                if(line.at(0) == '>')
                {
                    name = line;
                    seq_iter++;
                    input = "";              

                
                }
                else
                {
                    input+=line;
                    sequences.push_back(input);
					cout<<input<<endl;
                }
            }
            
            
        }
    }
    else
	{
		cout << "Unable to open file";
		return 1;
	}
	myfile.close();

	for(int seq_iter = 0; seq_iter<sequences.size(); seq_iter++)
	{
		//auto start_resamp = chrono::high_resolution_clock::now(); 
    	time_t start_time, end_time;
		time(&start_time); 
		data = sequences.at(seq_iter);
		string seq = data;
		seq = sanitize(seq, checkin);
		//init 2d vector
		int length = data.size();
		string top = " "+data;
		string side = data;
		//alignment.resize(length+1);
		//trace.resize(length+1);
		for(int set = 0; set<length+1; set++)
		{
			vector<int> a_vect;
			vector<string> t_vect;
			//alignment[set].resize(length);
			//trace[set].resize(length);
			alignment.push_back(a_vect);
			bifurcation.push_back(t_vect);
			trace.push_back(t_vect);
		}
		for(int set = 0; set<length+1; set++)
		{
			for(int set2 = 0; set2<length+1; set2++)
			{
				alignment[set].push_back(0);
				bifurcation[set].push_back("#");
				trace[set].push_back("#");
			}
			//cout<<endl;
		}
		//cout<<"done"<<endl;

		for(int col = 0; col<length+1; col++)
		{
			if(col+1 <= length)
			{
				//cout<<col<<" "<<col+1<<endl;
				alignment[col][col] = 0;
				alignment[col][col+1] = 0;
				for(int gap = 1; gap<=min_gap;gap++)
				{
					alignment[col][col+1+gap] = 0;
				}
				bifurcation[col][col] = "0";
				bifurcation[col][col+1] = "0";
				for(int gap = 1; gap<=min_gap;gap++)
				{
					bifurcation[col][col+1+gap] = "0";
				}
				//bifurcation[col][col+2] = "0";
				trace[col][col] = "0";
				trace[col][col+1] = "0";
				for(int gap = 1; gap<=min_gap;gap++)
				{
					trace[col][col+1+gap] = "0";
				}
			}
		}
		cout<<"made matrices\n";

		//iterate through thing
		int super = 1;
		for(int col = 2+min_gap; col<length+1; col++)
		{
			int adj = 0;
			for(int diag_iter = 0; diag_iter<length; diag_iter++)
			{
				if(!(col+adj>length))
				{
					//diag_iter;
					//col+adj;
					//here is where (i,j) is calculated
					compute_cell(diag_iter, (col+adj), version);
					//alignment[diag_iter][col+adj] = super;
					super++;
					adj++;
				}
			}
			//cout<<">"<<data.size()<<endl;
			//cout<<"------------------------------\n"<<super<<endl;
			
		}
		cout<<"cell computed\n";

		//traceback
		string dot_bracket;
		for(int x = 0; x<data.size(); x++)
		{
			dot_bracket+=".";
		}
		
		string read = "";
		
		if(seq != "")
		{
			cout<<"into traceback\n";
			traceback(0,data.size(),read,dot_bracket, "", seq, version);
			//output2.clear();
			cout<<"out of traceback\n";
			trace.clear();
			alignment.clear();
			bifurcation.clear();
		}
		else
		{
			cout<<"Sequence "<<seq_iter+1<<" had invalid characters."<<endl;
		}
		cout<<"done with cycle "<<seq_iter+1<<endl;
		time(&end_time);
		double duration = double(end_time - start_time);
		durations.push_back(duration);
		output.clear();
	}

    for(int y = 0; y<durations.size();y++)
	{
		cout<<"Sequence " << y+1 << " Total time: " << durations.at(y) << endl;
	}
	//cout<<data.size()<< " " << data.size()+1;
	//cout<<trace[0][9]<<endl;
	
	// trace.clear();
	// alignment.clear();
}

