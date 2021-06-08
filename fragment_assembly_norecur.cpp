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

using namespace std;

vector<vector<int>> join_vals;
vector<vector<int>> prev_locs;
vector<string> sequences;
vector<string> names;
vector<string> results;
vector<string> true_results;
vector<int> prev_loc_master;
vector<double> defrag_averages;
vector<double> comp_averages;
vector<double> findmax_averages;
int minimum;
int master_size;

int find_max_seq_loc()
{
    auto start_findmax = chrono::high_resolution_clock::now(); 
    int nu_max = minimum;
    int temp = 0;
    int result;
    for(int seq_iter = 0; seq_iter<sequences.size(); seq_iter++)
	{
        for(int comp_iter = 0; comp_iter<sequences.size(); comp_iter++)
        {
            if(join_vals[seq_iter][comp_iter] >= nu_max && prev_loc_master[seq_iter] != 1 && prev_loc_master[comp_iter] != 1)
            {
                nu_max = join_vals[seq_iter][comp_iter];
                temp = 1;
                //join_vals[seq_iter][comp_iter] = -1;
            }
        }
    }
    auto end_findmax = chrono::high_resolution_clock::now(); 
    double duration_findmax = chrono::duration_cast<chrono::nanoseconds>(end_findmax - start_findmax).count();  
    duration_findmax *= 1e-9;
    findmax_averages.push_back(duration_findmax);
    if(temp == 0)
    {
        return 0;
    }
    return nu_max;
}

string defrag(int seq, int comp, string result)
{
    auto start_defrag = chrono::high_resolution_clock::now(); 
    string holder = result;
    int done = 0;
    while(done == 0)
    {
        int true_seq;
        int true_comp =0;
        int max = 0;
        //iterate through comp vector to find largest outgoing weight
        for(int comp_iter = 0; comp_iter < sequences.size(); comp_iter++)
        {
            if(join_vals[seq][comp_iter] > max && prev_loc_master[seq] != 1 && prev_loc_master[comp_iter] != 1)
            {
                max = join_vals[seq][comp_iter];
                true_comp = comp_iter;
            }
        }
        if(max < minimum)
        {
            //cout<<"dead end\n"<<endl;
            //cout<<true_comp<<endl;
            prev_loc_master.at(seq) = 1;
            done = 1;
            break;
        }

        //find loc of max
        for(int comp_iter = 0; comp_iter<sequences.size(); comp_iter++)
        {
            if(join_vals[seq][comp_iter] == max)
            {
                string left = result;
                left = left.substr(0,left.size()-max);
                string right = sequences.at(comp_iter);
                right = right.substr(max,right.size());
                
                string inter = sequences.at(comp_iter);
                inter = inter.substr(0,max);
                result = left.append(inter);
                result = result.append(right);
                //cout<<names.at(seq)<<", "<<names.at(comp_iter)<<" ("<<inter<<") :: ";
                
                //move on to next seq
                prev_loc_master.at(seq) = 1;
                prev_locs[seq][comp_iter] = 1;
                if(prev_loc_master.at(comp_iter) != 1)
                {
                    //cout<<"reset\n"<<max<<endl;
                    seq = comp_iter;
                    comp_iter = -1;
                    max = -2;
                    //break;
                }
                else
                {
                    //cout<<"loop\n";
                    done = 1;
                    break;
                }
                
            }
        }
    }
    if(holder == result)
    {
        prev_loc_master.at(seq) = 1;
    }
    auto end_defrag = chrono::high_resolution_clock::now(); 
    double duration_defrag = chrono::duration_cast<chrono::nanoseconds>(end_defrag - start_defrag).count();  
    duration_defrag *= 1e-9;
    defrag_averages.push_back(duration_defrag);
    return result;
}

string comp(string suffix, string prefix)
{
    auto start_comp = chrono::high_resolution_clock::now(); 
    int match = 0;
    int param = 1;
    string answer;
    while(match == 0)
    {
        //cout<<param<<endl;
        if(param > prefix.size() || param > suffix.size())
        {
            match = 1;
        }
        else
        {
            string suffix_frag = suffix.substr(suffix.size()-param);
            string prefix_frag = prefix.substr(0,suffix_frag.size());
            param+=1;
            if(suffix_frag == prefix_frag)
            {
                answer = suffix_frag;
            }
        }
        
        
    }
    auto end_comp = chrono::high_resolution_clock::now(); 
    double duration_comp = chrono::duration_cast<chrono::nanoseconds>(end_comp - start_comp).count();  
    duration_comp *= 1e-9;
    comp_averages.push_back(duration_comp);
    return answer;
}

int main()
{
    string answer;
	cout << "What is the input file?"<<endl;
	cin >> answer;

	cout << "What is the minimum number of bases to match by?"<<endl;
	cin >> minimum;

    if(minimum <= 0)
    {
        cout<<"...I'm gonna go ahead and default the minimum to 1 for you, wise guy/gal."<<endl;
        minimum = 1;
    }

    auto start_total = chrono::high_resolution_clock::now(); 


    //read input
    ifstream myfile (answer); 
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
                    name = line.substr(2);
                    names.push_back(name);
                    seq_iter++;
                    input = "";              

                
                }
                else
                {
                    input+=line;
                    // if(minimum >= input.size())
                    // {
                    //     cout<<"Error, minimum overlap value greater than contig length. Defaulting minimum overlap to 1.\n";
                    // }
                    sequences.push_back(input);
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

    //load sequences
    for(int seq_iter = 0; seq_iter<sequences.size(); seq_iter++)
	{
        string data = sequences.at(seq_iter);
        vector<int> vect;
        join_vals.push_back(vect);
        prev_locs.push_back(vect);
        for(int seq_iter2 = 0; seq_iter2<sequences.size(); seq_iter2++)
        {
            join_vals[seq_iter].push_back(0);
            prev_locs[seq_iter].push_back(0);
        }
    }

    auto start_comp = chrono::high_resolution_clock::now(); 
    //do comparisons
    int true_max = 0;
    for(int seq_iter = 0; seq_iter<sequences.size(); seq_iter++)
    {
        //cout<<"The end of sequence " << names.at(seq_iter) << " matches with the beginning of: "<<endl;
        int max = minimum;
        for(int comp_iter = 0; comp_iter<sequences.size(); comp_iter++)
        {
            if(comp_iter != seq_iter)
            {
                //do comparison bit here
                int temp = 0;
                string answer = comp(sequences.at(seq_iter),sequences.at(comp_iter));
                // if(answer != "")
                // {
                //     cout<< "    Sequence " << names.at(comp_iter) << ", " << answer << " (weight: " << answer.size() << ")" << ", "<<endl;
                // }
                temp = answer.size();
                if(temp >= max)
                {
                    join_vals[seq_iter][comp_iter] = temp;
                    max = temp;
                    if(max >= true_max)
                    {
                        true_max = max;
                    }
                }
            }
            else
            {
                join_vals[seq_iter][comp_iter] = -1;
            }
            
        }
        //cout<<endl;
    }
    auto end_comp = chrono::high_resolution_clock::now(); 

    //make alignments
    auto start_align = chrono::high_resolution_clock::now(); 
    for(int loc_int = 0; loc_int < sequences.size(); loc_int++)
    {
        prev_loc_master.push_back(0);
    }
    results.push_back("");
    int finished = 0;
    int ref = 0;
    while(finished == 0)
    {
        //determine if all sequences have been hit
        ref = 0;
        for(int x = 0; x<prev_loc_master.size(); x++)
        {
            //cout<<"["<<names.at(x)<< ": " << prev_loc_master.at(x)<<" ]";
            if(prev_loc_master.at(x) == 0)
            {
                ref = 1;
            }
        }
        //cout<<ref<<endl;
        if(ref == 0 || true_max < minimum)
        {
            //assemble final fragments, either have hit 0 or there are no more fragments to assemble
            //cout<<"done\n";
            finished = 1;
        }
        else
        {
            //havent hit a 0 and haven't hit all sequences
            //cout<<true_max<<endl;
            for(int seq_iter = 0; seq_iter<sequences.size(); seq_iter++)
            {
                for(int comp_iter = 0; comp_iter<sequences.size(); comp_iter++)
                {
                    if(join_vals[seq_iter][comp_iter] == true_max && prev_loc_master[seq_iter] == 0)
                    {
                        string frag = defrag(seq_iter, comp_iter, sequences.at(seq_iter));
                        if(frag != sequences.at(seq_iter) && frag != results.at(results.size()-1))
                        {
                            //cout<<frag<<endl<<endl;
                            //cout<<endl;
                            //cout<<"---------------\n";
                            results.push_back(frag);
                        }
                        else if(frag != results.at(results.size()-1))
                        {
                            //cout<< names.at(seq_iter)<< endl;
                            results.push_back(frag);
                            //cout<<frag<<endl<<endl;
                            //cout<<"---------------\n";
                        }
                        prev_locs[seq_iter][comp_iter] = 1;
                        true_max = find_max_seq_loc(); 
                        seq_iter = 0;
                        comp_iter = 0;
                                                
                    }
                }
            }
        }
        //reset if not done
    }
    auto end_align = chrono::high_resolution_clock::now(); 


    results.erase(results.begin());
    cout<<results.size()<<endl;
    
    //cout<<endl<<ult<<endl;
    int count = 1;
    for(int f = 0; f<results.size(); f++)
    {
        cout<<"Contig "<<count<<": "<<results.at(f)<<endl;
        count++;
    }

    auto end_total = chrono::high_resolution_clock::now(); 
    double duration_total = chrono::duration_cast<chrono::nanoseconds>(end_total - start_total).count();  
    duration_total *= 1e-9;
    cout<<"the total runtime was " << duration_total << " seconds.\n";

    double duration_comp = chrono::duration_cast<chrono::nanoseconds>(end_comp - start_comp).count();  
    duration_comp *= 1e-9;
    cout<<"the runtime for fragment overlap calculation was " << duration_comp << " seconds.\n";

    double duration_align = chrono::duration_cast<chrono::nanoseconds>(end_align - start_align).count();  
    duration_align *= 1e-9;
    cout<<"the runtime for total contig assembly was " << duration_align << " seconds.\n";

    double average;
    for(int x = 0; x<defrag_averages.size(); x++)
    {
        average+=defrag_averages[x];
    }
    average *= 1e-9;
    cout<<"the runtime for the defrag function was " << average << " seconds.\n";

    average = 0;
    for(int x = 0; x<comp_averages.size(); x++)
    {
        average+=comp_averages[x];
    }
    average *= 1e-9;
    cout<<"the runtime for the comparison function was " << average << " seconds.\n";

    average = 0;
    for(int x = 0; x<findmax_averages.size(); x++)
    {
        average+=findmax_averages[x];
    }
    average *= 1e-9;
    cout<<"the runtime for the max-finding function was " << average << " seconds.\n";


}