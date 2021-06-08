#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h> 
#include <map>

using namespace std;

string trace_matrix[400][400];
string test_matrix[400][400];
int score_matrix[400][400];
vector<string> msa_names;
string master_top;
string master_side;
//first string is the name of the sequence, second string is the sequence itself
unordered_map<string, string> msa_name_to_seq;
// pair is a combo of sequence1 name + sequence2 name, int is the score of their alignment
unordered_map<string, int> msa_seq_to_score; 
// unordered_map<string, int> msa_name_to_score;
unordered_map<string, string> msa_master_name_to_seq;
vector< pair <string,int> > msa_name_to_score; 





int traceback_module(string traceback_seq_top, string traceback_seq_side, int top_trace, int side_trace, string side, string top) //int top_trace_str, int side_trace_str,
{

    int top_trace_str = top_trace -1;
    int side_trace_str = side_trace-1;
    string hold_side_seq = traceback_seq_side;
    string hold_top_seq = traceback_seq_top;

    //cout<<side_trace<<" "<<top_trace<<endl;
    // string indicator = trace_matrix[side_trace][top_trace];
    if(side_trace == 0 && top_trace == 0)
    {
        // cout<<"\n\n\n"; 

        reverse(traceback_seq_top.begin(),traceback_seq_top.end());
        reverse(traceback_seq_side.begin(),traceback_seq_side.end());
        // cout<<rev_top<<'\n'<<rev_side<<"\n\n";
        //cout<<traceback_seq_top<<"\n"<<traceback_seq_side<<"\n\n";
        master_top = traceback_seq_top;
        master_side = traceback_seq_side;
        //cout<<"==========================================================="<<endl;
        return 0;
    }
    else if(side_trace == 0)
    {
        //cout<<"HERE: "<<top.at(0)<<endl;
        //cout<<top<<endl;
        //cout<<"um? "<<top.substr(0,top_trace)<<endl;
        traceback_seq_top += top.substr(0,top_trace);
        for(int x = 0; x<=top_trace_str; x++)
        {
            traceback_seq_side += "_";
        }
        reverse(traceback_seq_top.begin(),traceback_seq_top.end());
        reverse(traceback_seq_side.begin(),traceback_seq_side.end());
        master_top = traceback_seq_top;
        master_side = traceback_seq_side;
        //cout<<traceback_seq_top<<"\n"<<traceback_seq_side<<"\n\n";
        //cout<<"==========================================================="<<endl;
        return 0;
        cout<<"nada\n";
    }
    else if(top_trace == 0)
    {
        //cout<<"HERE: "<<side.at(0)<<endl;
        //cout<<side<<endl;
        //cout<<"um? "<<side.substr(0,side_trace)<<endl;
        traceback_seq_side += side.substr(0,side_trace);
        for(int x = 0; x<=side_trace_str; x++)
        {
            traceback_seq_top += "_";
        }
        reverse(traceback_seq_top.begin(),traceback_seq_top.end());
        reverse(traceback_seq_side.begin(),traceback_seq_side.end());
        master_top = traceback_seq_top;
        master_side = traceback_seq_side;
        //cout<<traceback_seq_top<<"\n"<<traceback_seq_side<<"\n\n";
        //cout<<"==========================================================="<<endl;
        return 0;
        cout<<"nadda\n";
    }
    string indicator = trace_matrix[top_trace][side_trace];
    test_matrix[top_trace][side_trace] = "#";
    for(int count = 0; count <= 2; count++)
    {
        char check = indicator.at(count);
        //cout << "indicator " << indicator<<" "<<endl;
        // cout<<check<<count<<endl;
        // cout<<"top trace"<<top_trace<<" side trace"<<side_trace<<" "<<"top string"<<hold_top_seq.length()<<" side string"<<hold_side_seq.length()<<endl;
        switch(check)
        {
            case 'L': traceback_seq_top += '_';
                      traceback_seq_side += (side.at(side_trace_str)); //top_trace
                            // reverse(traceback_seq_top.begin(),traceback_seq_top.end());
                            // reverse(traceback_seq_side.begin(),traceback_seq_side.end());
                            // cout<<"\n\n"<<traceback_seq_top<<"\n"<<traceback_seq_side<<endl;
                            // reverse(traceback_seq_top.begin(),traceback_seq_top.end());
                            // reverse(traceback_seq_side.begin(),traceback_seq_side.end());
                      traceback_module(traceback_seq_top, traceback_seq_side, top_trace, side_trace-1, side, top); //top_trace_str, side_trace_str-1,
                      traceback_seq_top = hold_top_seq;
                      traceback_seq_side = hold_side_seq;
                      break;
            case 'U': traceback_seq_top+=(top.at(top_trace_str)); //side_trace
                      traceback_seq_side += '_';
                        // reverse(traceback_seq_top.begin(),traceback_seq_top.end());
                        // reverse(traceback_seq_side.begin(),traceback_seq_side.end());
                        // cout<<"\n\n"<<traceback_seq_top<<"\n"<<traceback_seq_side<<endl;
                        // reverse(traceback_seq_top.begin(),traceback_seq_top.end());
                        // reverse(traceback_seq_side.begin(),traceback_seq_side.end());
                      traceback_module(traceback_seq_top, traceback_seq_side, top_trace-1, side_trace, side, top); //top_trace_str-1, side_trace_str,
                      traceback_seq_top = hold_top_seq;
                      traceback_seq_side = hold_side_seq;
                      break;
            case 'V': traceback_seq_top+=(top.at(top_trace_str)); //top_trace
                      traceback_seq_side+=(side.at(side_trace_str)); //side_trace
                        // reverse(traceback_seq_top.begin(),traceback_seq_top.end());
                        // reverse(traceback_seq_side.begin(),traceback_seq_side.end());
                        // cout<<"\n\n"<<traceback_seq_top<<"\n"<<traceback_seq_side<<endl;
                        // reverse(traceback_seq_top.begin(),traceback_seq_top.end());
                        // reverse(traceback_seq_side.begin(),traceback_seq_side.end());
                      traceback_module(traceback_seq_top, traceback_seq_side, top_trace-1, side_trace-1, side, top); ///top_trace_str-1, side_trace_str-1,
                      traceback_seq_top = hold_top_seq;
                      traceback_seq_side = hold_side_seq;
                      break;
            case 'n': if(side_trace == 1)
                        {
                        traceback_seq_top+=(top.at(top_trace_str));
                        traceback_seq_side += '_';
                        }
            default: break;
        }
        // cout<<"OUT OF THE FOR LOOP FOR "<< check << "-------------------------------------------- \n";
    }
    // trace_matrix[top_trace][side_trace] = "###";
    //cout<<"this i guess???\n";
    return 0;
}

int nucleotide_scoring(string top, string side, int gap, int match, int mismatch, int extend)
{
        
    // string line;
    
    int top_length = top.length();
    int side_length = side.length();
    //init matrices
    //int score_matrix[top_length+1][side_length+1];
    
    // string trace_matrix[top_length+1][side_length+1];

    //int trace_matrix[top_length+1][side_length+1];
    for(int x=0;x<=top_length;x++)
    {
        for(int y=0;y<=side_length;y++)
        {
            score_matrix[x][y]=0;
            // cout<<"this bit"<<endl;
            trace_matrix[x][y]='n';
            test_matrix[x][y]="0";
        }
        
    }
    
    //init score matrix with gap data
    // cout<<"check matrix init\n";
    for(int init_score_row = 0; init_score_row<=top_length; init_score_row++)
    {
        score_matrix[init_score_row][0] = gap * init_score_row;
    }
    for(int init_score_col = 0; init_score_col<=side_length; init_score_col++)
    {
        score_matrix[0][init_score_col] = gap * init_score_col;
    }


    //fill the matrices
    // cout<<"check matrix fill\n";
    for(int top_iter = 1; top_iter<=top_length;top_iter++)
    {
        // cout<<"check matrix fill for loop 1\n";
        for(int side_iter = 1; side_iter<=side_length;side_iter++)
        {
            // cout<<"check matrix fill for loop 2"<<side_iter<<"\n"
            int check = 0;
            int vert;
            int up = score_matrix[top_iter-1][side_iter]+gap;
            if(top.at(top_iter-1)==side.at(side_iter-1))
            {
                vert = score_matrix[top_iter-1][side_iter-1]+match;
                check = 1;
            }
            else
            {
                vert = score_matrix[top_iter-1][side_iter-1]+mismatch;
            }
            int left = score_matrix[top_iter][side_iter-1]+gap;
            string back;
            // cout<<score_matrix[top_iter][side_iter]<<" ";
            if((up >= vert) && (up >= left))
            {
                // cout << up << " ";
                if(trace_matrix[top_iter-1][side_iter].at(0) == 'U')
                {
                    score_matrix[top_iter][side_iter] = score_matrix[top_iter-1][side_iter]+extend;
                }
                else
                {
                    score_matrix[top_iter][side_iter] = up;
                }   
            }
            else if ((vert >= up) && (vert >= left))
            {
                score_matrix[top_iter][side_iter] = vert;
                // cout << vert << " ";
            }
            else
            {
                if(trace_matrix[top_iter][side_iter-1].at(1) == 'L')
                {
                    score_matrix[top_iter][side_iter] = score_matrix[top_iter][side_iter-1]+extend;                 
                }
                else
                {
                score_matrix[top_iter][side_iter] = left;
                }
                // cout << left << " ";
            }
            // cout<<"made it to this bit"<<endl;
            // make traceback guide
            if(score_matrix[top_iter-1][side_iter] == score_matrix[top_iter][side_iter]-gap || score_matrix[top_iter-1][side_iter] == score_matrix[top_iter][side_iter]-extend)
            {
                back += "U";
            }
            else
            {
                back += "-";
            }
            if(score_matrix[top_iter][side_iter-1] == score_matrix[top_iter][side_iter]-gap || score_matrix[top_iter][side_iter-1] == score_matrix[top_iter][side_iter]-extend)
            {
                back += "L";
            }
            else
            {
                back += "-";
            }
            if(check == 1)
            {
                if(score_matrix[top_iter-1][side_iter-1] == score_matrix[top_iter][side_iter]-match)
                {
                    back += "V";
                }
                else
                {
                    back += "-";
                }
            }
            else
            {
                if(score_matrix[top_iter-1][side_iter-1] == score_matrix[top_iter][side_iter]-mismatch)
                {
                    back += "V";
                }
                else
                {
                    back += "-";
                }
            }
            if(back == "---")
            {
                
                if(score_matrix[top_iter-1][side_iter] == score_matrix[top_iter][side_iter]-gap-extend)
                {
                    back.at(0)='U';
                }
                if(score_matrix[top_iter][side_iter-1] == score_matrix[top_iter][side_iter]-gap-extend)
                {
                    back.at(1) = 'L';
                }
            }
            // cout<<back<<endl;
            
            // char* c = const_cast<char*>(back.c_str());   
                
            trace_matrix[top_iter][side_iter] = back;
        }
        // cout<<endl;
    }

        //print the matrices
    // cout<<"check matrix print"<<endl;
    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      // cout<< trace_matrix[x][y] << "   ";
    //      if(score_matrix[x][y] >= 0)
    //      {
    //          cout<<" "<<score_matrix[x][y];
    //      }
    //      else
    //      {
    //          cout<<score_matrix[x][y];
    //      }
    //  }
    //  cout<<endl;
    // }

    //trace matrix print
    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      cout<< trace_matrix[x][y] << "   ";
            
    //  }
    //  cout<<endl;
    // }
    
    string alignment_top = "";
    string alignment_side = "";

    // find start position value
    int start_point = score_matrix[top_length][side_length];
    for(int x = 0; x<=top.length(); x++)
    {
        if(start_point < score_matrix[x][side_length])
        {
            start_point = score_matrix[x][side_length];
        }
    }
    for(int x = 0; x<side.length(); x++)
    {
        if(start_point < score_matrix[top_length][x])
        {
            start_point = score_matrix[top_length][x];
        }
    }

    //execute traceback from start positions
    for(int x = 0; x<=top.length(); x++)
    {
        if(score_matrix[x][side_length] == start_point)
        {
            for(int y = 0; y<(top.length()-x);y++)
            {
                alignment_side+='_';
            }
            alignment_top = top.substr(x,top.length());
            //cout<<alignment_top<<" "<<alignment_side<<endl;
            traceback_module(alignment_top, alignment_side, x, side_length, side, top); //op_length-1, side_length-1, 
            alignment_top = "";
            alignment_side = "";
        }
    }
    for(int x = 0; x<side.length(); x++)
    {
        if(score_matrix[top_length][x] == start_point)
        {
            for(int y = 0; y<(side.length()-x);y++)
            {
                alignment_top+='_';
            }
            alignment_side = side.substr(x,side.length());
            //cout<<alignment_top<<" "<<alignment_side<<endl;
            traceback_module(alignment_top, alignment_side, top_length, x, side, top); //op_length-1, side_length-1, *
            alignment_top = "";
            alignment_side = "";
        }
    }
    //traceback_module(alignment_top, alignment_side, top_length, side_length, side, top); //op_length-1, side_length-1, 
    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      cout<<trace_matrix[x][y];
    //  }
    //  cout<<endl;
    // }
    
    //test_matrix
    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      cout<<test_matrix[x][y]<<" ";
    //      // cout<< trace_matrix[x][y] << "   ";
    //      // if(score_matrix[x][y] >= 0)
    //      // {
    //      //  // cout<<" "<<score_matrix[x][y];
    //      // }
    //      // else
    //      // {
    //      //  // cout<<score_matrix[x][y];
    //      // }
    //  }
    //  cout<<endl;
    // }
    return start_point;
}

int protein_scoring(string top, string side, int gap, int match, int mismatch, string protein_matrix, int extend)
{
    ifstream myfile2 (protein_matrix); //"pairwise_nucleotide_1.fasta"
    string pam_matrix[21][21];
    int check = 0;
    for(int i = 0; i < 21; i++)
    {
        for(int j = 0; j < 21; j++)
        {
            if(check == 0)
            {
                pam_matrix[i][j] = " ";
                check = 1;
            }
            else
            {
                // cout<<myfile2<<endl;
                myfile2 >> pam_matrix[i][j];

            }

        }

    }

    // cout << "The pam elements are : "<<endl;
    // for(int i = 0; i < 21; i++)
    // {
    //  for(int j = 0; j < 21; j++)
    //  {
    //      cout << pam_matrix[i][j]<<"  ";

    //  }
    //  cout<<endl;
    // }    
    // string line;
    
    int top_length = top.length();
    int side_length = side.length();

    //init matrices
    //int score_matrix[top_length+1][side_length+1];
    
    // string trace_matrix[top_length+1][side_length+1];

    //int trace_matrix[top_length+1][side_length+1];
    for(int x=0;x<=top_length;x++)
    {
        for(int y=0;y<=side_length;y++)
        {
            score_matrix[x][y]=0;
            // cout<<"this bit"<<endl;
            trace_matrix[x][y]='n';
            test_matrix[x][y]="0";
        }
        
    }
    
    //init score matrix with gap data
    // cout<<"check matrix init\n";
    for(int init_score_row = 0; init_score_row<=top_length; init_score_row++)
    {
        score_matrix[init_score_row][0] = gap * init_score_row;
    }
    for(int init_score_col = 0; init_score_col<=side_length; init_score_col++)
    {
        score_matrix[0][init_score_col] = gap * init_score_col;
    }

    //cout<<"upcheck"<<endl;

    //fill the matrices
    // cout<<"check matrix fill\n";
    int prot_val;
    for(int top_iter = 1; top_iter<=top_length;top_iter++)
    {
        int side_track;
        int top_track;
        // cout<<"check matrix fill for loop 1\n";
        //check scoring matrix loop
        for(int prot_iter_top = 0; prot_iter_top<21; prot_iter_top++)
        {
            //cout<<prot_iter_top<<endl;
            char temp1 = (pam_matrix[prot_iter_top][0]).at(0);
            if(temp1 == top.at(top_iter-1))
            {
                top_track = prot_iter_top;
            }
            //cout<<top.at(prot_iter_top)<<endl;

        }
        // cout << "post first"<<endl;
        for(int side_iter = 1; side_iter<=side_length;side_iter++)
        {
            //check scoring matrix loop
            //cout<<"pre second\n";
            for(int prot_iter_side = 0; prot_iter_side<21; prot_iter_side++)
            {
                //cout<<side_iter<<" "<<side.length()<<endl;
                char temp2 = (pam_matrix[0][prot_iter_side]).at(0);
                if(temp2 == side.at(side_iter-1))
                {
                    side_track = prot_iter_side;
                }
            }
            //cout<<"post second"<<endl;
            //cout<<top_track<<" "<<side_track<<endl;
            int prot_mod_val = stoi(pam_matrix[top_track][side_track]);
            match = prot_mod_val;
            mismatch = prot_mod_val;
            //cout<<match<<" "<<mismatch<<endl;
            // cout<<prot_mod_val<<endl;
            //cout<<"check matrix fill for loop 2 "<<side_iter<<"\n";
            int check = 0;
            int vert;
            int up = score_matrix[top_iter-1][side_iter]+gap;
            if(top.at(top_iter-1)==side.at(side_iter-1))
            {
                //cout<<"here?"<<endl;
                vert = score_matrix[top_iter-1][side_iter-1]+match;
                check = 1;
            }
            else
            {
                //cout<<"or here?"<<endl;
                vert = score_matrix[top_iter-1][side_iter-1]+mismatch;
                //cout<<"OR HERE??"<<endl;
            }
            //cout<<"top,side,tlen,slen"<<top_iter<<" "<<side_iter<<" "<<top.length()<<" "<<side.length()<<endl;
            int left = score_matrix[top_iter][side_iter-1]+gap;
            //cout<<"post this bit\n";
            string back;
            //cout<<score_matrix[top_iter][side_iter]<<" ";
            if((up >= vert) && (up >= left))
            {
                // cout << up << " ";
                if(trace_matrix[top_iter-1][side_iter].at(0) == 'U')
                {
                    score_matrix[top_iter][side_iter] = up+extend;
                }
                else
                {
                    score_matrix[top_iter][side_iter] = up;
                }   
                //score_matrix[top_iter][side_iter] = up;
            }
            else if ((vert >= up) && (vert >= left))
            {
                score_matrix[top_iter][side_iter] = vert;
                // cout << vert << " ";
            }
            else
            {
                if(trace_matrix[top_iter][side_iter-1].at(1) == 'L')
                {
                    score_matrix[top_iter][side_iter] = left+extend;                    
                }
                else
                {
                    score_matrix[top_iter][side_iter] = left;
                }
                //score_matrix[top_iter][side_iter] = left;
                // cout << left << " ";
            }
            // cout<<"made it to this bit"<<endl;
            // make traceback score
            if(score_matrix[top_iter-1][side_iter] == score_matrix[top_iter][side_iter]-gap)
            {
                back += "U";
            }
            else
            {
                back += "-";
            }
            if(score_matrix[top_iter][side_iter-1] == score_matrix[top_iter][side_iter]-gap)
            {
                back += "L";
            }
            else
            {
                back += "-";
            }
            if(check == 1)
            {
                if(score_matrix[top_iter-1][side_iter-1] == score_matrix[top_iter][side_iter]-match)
                {
                    back += "V";
                }
                else
                {
                    back += "-";
                }
            }
            else
            {
                if(score_matrix[top_iter-1][side_iter-1] == score_matrix[top_iter][side_iter]-mismatch)
                {
                    back += "V";
                }
                else
                {
                    back += "-";
                }
            }   
            if(back == "---")
            {
                
                if(score_matrix[top_iter-1][side_iter] == score_matrix[top_iter][side_iter]-gap-extend)
                {
                    back.at(0)='U';
                }
                if(score_matrix[top_iter][side_iter-1] == score_matrix[top_iter][side_iter]-gap-extend)
                {
                    back.at(1) = 'L';
                }
            }
            //cout<<back<<endl;
            
            // char* c = const_cast<char*>(back.c_str());   
            
                
            trace_matrix[top_iter][side_iter] = back;
            //cout<<"here?????\n";
        }
        // cout<<endl;
    }

        //print the matrices
    // cout<<"check matrix print"<<endl;
    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      // cout<< trace_matrix[x][y] << "   ";
    //      if(score_matrix[x][y] >= 0)
    //      {
    //          cout<<" "<<score_matrix[x][y];
    //      }
    //      else
    //      {
    //          cout<<score_matrix[x][y];
    //      }
    //  }
    //  cout<<endl;
    // }

    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      // cout<< trace_matrix[x][y] << "   ";
    //      if(score_matrix[x][y] >= 0)
    //      {
    //          cout<<" "<<score_matrix[x][y];
    //      }
    //      else
    //      {
    //          cout<<score_matrix[x][y];
    //      }
    //  }
    //  cout<<endl;
    // }

    int start_point = score_matrix[top_length][side_length];
    for(int x = 0; x<=top.length(); x++)
    {
        if(start_point < score_matrix[x][side_length])
        {
            start_point = score_matrix[x][side_length];
        }
    }
    for(int x = 0; x<side.length(); x++)
    {
        if(start_point < score_matrix[top_length][x])
        {
            start_point = score_matrix[top_length][x];
        }
    }
    
    //cout<<"here?"<<endl;
    string alignment_top = "";
    string alignment_side = "";
    for(int x = 0; x<=top.length(); x++)
    {
        if(score_matrix[x][side_length] == start_point)
        {
            for(int y = 0; y<(top.length()-x);y++)
            {
                alignment_side+='_';
            }
            alignment_top = top.substr(x,top.length());
            //cout<<alignment_top<<" "<<alignment_side<<endl;
            traceback_module(alignment_top, alignment_side, x, side_length, side, top); //op_length-1, side_length-1, 
            alignment_top = "";
            alignment_side = "";
        }
    }
    for(int x = 0; x<side.length(); x++)
    {
        if(score_matrix[top_length][x] == start_point)
        {
            for(int y = 0; y<(side.length()-x);y++)
            {
                alignment_top+='_';
            }
            alignment_side = side.substr(x,side.length());
            //cout<<alignment_top<<" "<<alignment_side<<endl;
            traceback_module(alignment_top, alignment_side, top_length, x, side, top); //op_length-1, side_length-1, *
            alignment_top = "";
            alignment_side = "";
        }
    }
    //traceback_module(alignment_top, alignment_side, top_length, side_length, side, top); //op_length-1, side_length-1, 
    //cout<<"shocking. it's the traceback module\n";
    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      cout<<trace_matrix[x][y];
    //  }
    //  cout<<endl;
    // }
    
    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      // cout<< trace_matrix[x][y] << "   ";
    //      if(score_matrix[x][y] >= 0)
    //      {
    //          cout<<" "<<score_matrix[x][y];
    //      }
    //      else
    //      {
    //          cout<<score_matrix[x][y];
    //      }
    //  }
    //  cout<<endl;
    // }

    // for(int x=0;x<=top_length;x++)
    // {
    //  for(int y=0;y<=side_length;y++)
    //  {
    //      cout<<test_matrix[x][y]<<" ";
    //      // cout<< trace_matrix[x][y] << "   ";
    //      // if(score_matrix[x][y] >= 0)
    //      // {
    //      //  // cout<<" "<<score_matrix[x][y];
    //      // }
    //      // else
    //      // {
    //      //  // cout<<score_matrix[x][y];
    //      // }
    //  }
    //  cout<<endl;
    // }
    return start_point;
}

int msa_module(int gap, int match, int mismatch, int extend, int type_check)
{
    int name_tracker = 1;
    map<int, string> consensus;
    string consensus_seq;
    unordered_map<string, int> msa_temp_alignments_to_score;
    //for loop to compare sequence at x with all other sequences
    vector<string> blacklist;
    vector<int> master_scores;
    //unordered_map<string, int> consensus_holder;
    for(pair<string, string> element : msa_name_to_seq)
    {
        //cout<<"first loop: "<<x<<endl;
        string tracker = element.first;
        int score;
        blacklist.push_back (tracker);
        //sequence to iterate thru all other sequences
        for (pair<string, string> element2 : msa_name_to_seq)
        {
            //cout<<"second loop: "<<element.first<<" "<<element.second<<endl;
            int highscore;
            int skip = 0;
            //check against blacklist to see if alignment has already been done
            for(int y = 0; y<blacklist.size();y++)
            {
                // cout<<"checker loop: "<<blacklist.at(y)<<" "<<element.first<<endl;
                // cout<<blacklist.size()<<endl;
                if(blacklist.at(y) == element2.first)
                {
                    skip = 1;
                }
            }
            if(skip == 0)
            {
                if(type_check == 0)
                {
                    //do nucleotide things
                    highscore = nucleotide_scoring(msa_name_to_seq[tracker],element2.second, gap, match, mismatch, extend);
                    msa_temp_alignments_to_score[master_top] = highscore;
                    msa_temp_alignments_to_score[master_side] = highscore;
                    msa_master_name_to_seq[tracker] = master_top;
                    msa_master_name_to_seq[element2.second] = master_side;
                    msa_seq_to_score[msa_name_to_seq[tracker]] = highscore;
                    msa_seq_to_score[element2.second] = highscore;
                    msa_name_to_score.push_back(make_pair(tracker, highscore));
                    msa_name_to_score.push_back(make_pair(element2.first, highscore));
                    master_scores.push_back(highscore);
                    //cout<<highscore<<"\n"<<master_top<<"\n"<<master_side<<endl;
                    
                }
                else if(type_check == 1)
                {
                    //do protein things
                    highscore = protein_scoring(msa_name_to_seq[tracker],element2.second, gap, match, mismatch, "pam100.txt", extend);
                    msa_temp_alignments_to_score[master_top] = highscore;
                    msa_temp_alignments_to_score[master_side] = highscore;
                    msa_master_name_to_seq[tracker] = master_top;
                    msa_master_name_to_seq[element2.second] = master_side;
                    msa_seq_to_score[msa_name_to_seq[tracker]] = highscore;
                    msa_seq_to_score[element2.second] = highscore;
                    msa_name_to_score.push_back(make_pair(tracker, highscore));
                    msa_name_to_score.push_back(make_pair(element2.first, highscore));
                    master_scores.push_back(highscore);
                    //cout<<highscore<<"\n"<<master_top<<"\n"<<master_side<<endl;

                }
            }
            
        }
    }


    // sort(master_scores.begin(), master_scores.end()); 
    // reverse(master_scores.begin(),master_scores.end());

    //compare scores until final alignment produced
    while(msa_seq_to_score.size() > 0)
    {
        sort(master_scores.begin(), master_scores.end()); 
        reverse(master_scores.begin(),master_scores.end());
        //cout<<"NEW LOOP"<<endl;
        //make consensus sequence of sequences with given value
        int find_2 = 0;
        string sequence_1;
        string sequence_2;
        while(find_2 != 2)
        {
            //cout<<"assuming the issue is here? "<<msa_temp_alignments_to_score.size()<<endl;
            
            //cout<<"checking here: "<<find_2<<endl;
            //find 
            for (pair<string, int> element : msa_temp_alignments_to_score)
            {
                //cout<<"    "<<master_scores.at(0)<<" "<<element.first<<" "<<element.second<<endl;
                if(element.second == master_scores.at(0))
                {
                    //cout<<element.first<<" "<<element.second<<" "<<master_scores.at(0)<<endl;
                    //put the sequence with name in the consensus list
                    if(find_2 == 0)
                    {
                        sequence_1 = element.first;
                        find_2++;
                    }
                    else if(find_2 == 1)
                    {
                        sequence_2 = element.first;
                        find_2++;
                    }
                    //the second time it find the sequence this will make it quit out
                    
                }
            }
        }
        //cout<<"have the things"<<endl;
        //cout<<sequence_1<<endl<<sequence_2<<endl;
        
        //hunter killer function
        int bad_val = master_scores.at(0);
        vector<string> bad_names;
        for (auto& element : msa_name_to_score)
        {
            //cout<<element.first<<" "<<element.second<<endl;
            if(element.second == bad_val)
            {
                //name to seq
                msa_seq_to_score.erase(msa_name_to_seq[element.first]);
                msa_name_to_seq.erase(element.first);
                
                //seq to score
                bad_names.push_back(element.first);
                //cout<<"KILL: "<<element.first<<endl;

            }
        }
        //cout<<"hunter killer worked"<<endl;
        //check if consensus map is empty 
        if(consensus.empty()==1)
        {
            for(int x = 0; x<sequence_1.size();x++)
            {
                //cout<<"here in consensus putter loop init  "<<x<<endl;
                string temp = "?";
                //consensus[x] = "~"+sequence_1.at(x);
                temp +=sequence_1.at(x);
                temp +=sequence_2.at(x);
                consensus[x] = temp;
                //cout<<temp<<endl;
            }   
        }
        else
        {
            //add the things here
            int c_size = consensus.size();
            if(sequence_1.size() > c_size)
            {
                c_size = sequence_1.size() - 1 - c_size;

                for(int x = 0; x<c_size; x++)
                {
                    string temp = "?";
                    temp +=sequence_1.at(sequence_1.size()-1+x);
                    temp +=sequence_2.at(sequence_2.size()-1+x);
                    consensus[sequence_1.size()+x];
                }
            }
            else
            {
                for(int x = 0; x<consensus.size();x++)
                {
                    //cout<<"here in consensus putter loop  "<<x<<endl;
                    string temp = consensus[x];
                    //consensus[x] = "~"+sequence_1.at(x);
                    temp +=sequence_1.at(x);
                    temp +=sequence_2.at(x);
                    consensus[x] = temp;
                    //cout<<temp<<endl;
                }
            }
            
            // for(int x = 0; x<consensus.size();x++)
            // {
            //  //cout<<"here in consensus putter loop  "<<x<<endl;
            //  string temp = consensus[x];
            //  //consensus[x] = "~"+sequence_1.at(x);
            //  temp +=sequence_1.at(x);
            //  temp +=sequence_2.at(x);
            //  consensus[x] = temp;
            //  //cout<<temp<<endl;
            // }

        }
        //cout<<"made all things\n";
        
        //make consensus
        consensus_seq = "";
        for(int x = 0; x<sequence_1.size(); x++)
        {
            if(sequence_1.at(x) == sequence_2.at(x))
            {
                consensus_seq += sequence_1.at(x);
            }
            else
            {
                consensus_seq += "_";
            }
            
        }
        // for(pair<int, string> element : consensus)
        // {
            
        //  int breaker = 0;
        //  char check = (element.second).at(1);
        //  //count all the residues and resolve appropriately
        //  for(int x = 1;x<(element.second).size();x++)
        //  {
        //      if((element.second).at(x) != check)
        //      {
        //          breaker = 1;
        //      }
        //  }
        //  if(breaker == 1)
        //  {
        //      (element.second).at(0) = '_';
        //      consensus_seq += '_';
        //      //cout<<"element.second "<<element.second<<endl;
        //  }
        //  else
        //  {
        //      (element.second).at(0) = check;
        //      consensus_seq += check;
        //      //cout<<"element.second "<<element.second<<endl;
        //  }
            
        // }
        //cout<<"----->"<<consensus_seq<<endl;  
        msa_name_to_seq["consensus"+to_string(name_tracker)] = consensus_seq;

        //compare consensus to all other remaining sequences
        //reset msa_temp_alignments_to_score and name_to_score
        msa_name_to_score.clear();
        msa_temp_alignments_to_score.clear();
        master_scores.clear();
        blacklist.clear();
        //for(int x = 0; x<msa_names.size();x++)
        //{
            //cout<<"first loop: "<<x<<endl;
        string tracker = "consensus"+to_string(name_tracker);
        int score;
        blacklist.push_back (tracker);
        //cout<<"for loop bit"<<endl;
        //sequence to iterate thru all other sequences
        for (pair<string, string> element : msa_name_to_seq)
        {
            //cout<<"second loop: "<<element.first<<" "<<element.second<<endl;
            int highscore;
            int skip = 0;
            //check against blacklist to see if alignment has already been done
            for(int y = 0; y<blacklist.size();y++)
            {
                // cout<<"checker loop: "<<blacklist.at(y)<<" "<<element.first<<endl;
                // cout<<blacklist.size()<<endl;
                if(blacklist.at(y) == element.first)
                {
                    skip = 1;
                }
            }
            if(skip == 0)
            {
                if(type_check == 0)
                {
                    //do nucleotide things
                    highscore = nucleotide_scoring(msa_name_to_seq[tracker],element.second, gap, match, mismatch, extend);
                    msa_temp_alignments_to_score[master_top] = highscore;
                    msa_temp_alignments_to_score[master_side] = highscore;
                    // if(msa_temp_alignments_to_score[master_top] <= highscore)
                    // {
                    //  msa_temp_alignments_to_score[master_top] = highscore;
                    // }    
                    // if(msa_temp_alignments_to_score[master_side] <= highscore)
                    // {
                    //  msa_temp_alignments_to_score[master_side] = highscore;
                    // }
                    msa_master_name_to_seq[tracker] = master_top;
                    msa_master_name_to_seq[element.second] = master_side;
                    msa_seq_to_score[msa_name_to_seq[tracker]] = highscore;
                    msa_seq_to_score[element.second] = highscore;
                    msa_name_to_score.push_back(make_pair(tracker, highscore));
                    msa_name_to_score.push_back(make_pair(element.first, highscore));
                    master_scores.push_back(highscore);
                    //cout<<highscore<<"\n"<<master_top<<"\n"<<master_side<<endl;
                    
                }
                else if(type_check == 1)
                {
                    //do protein things
                    highscore = protein_scoring(msa_name_to_seq[tracker],element.second, gap, match, mismatch, "pam100.txt", extend);
                    msa_temp_alignments_to_score[master_top] = highscore;
                    msa_temp_alignments_to_score[master_side] = highscore;
                    // if(msa_temp_alignments_to_score[master_top] >= highscore)
                    // {
                    //  msa_temp_alignments_to_score[master_top] = highscore;
                    // }    
                    // if(msa_temp_alignments_to_score[master_side] >= highscore)
                    // {
                    //  msa_temp_alignments_to_score[master_side] = highscore;
                    // }
                    msa_master_name_to_seq[tracker] = master_top;
                    msa_master_name_to_seq[element.second] = master_side;
                    msa_seq_to_score[msa_name_to_seq[tracker]] = highscore;
                    msa_seq_to_score[element.second] = highscore;
                    msa_name_to_score.push_back(make_pair(tracker, highscore));
                    msa_name_to_score.push_back(make_pair(element.first, highscore));
                    master_scores.push_back(highscore);
                    //cout<<highscore<<"\n"<<master_top<<"\n"<<master_side<<endl;

                }
            }       
        }
        //master_scores.erase(master_scores.begin());
        //cout<<"end loop"<<endl;
    }   
    //print out consensus and sequences
    cout<<consensus_seq<<endl;
    cout<<consensus.size()<<" "<<consensus_seq.size()<<" "<<consensus[0].size()<<endl;
	int iter = 0;
    for(auto elem : consensus)
    {
		elem.second = elem.second+(consensus_seq.at(iter));
		cout<<elem.second<<endl;
		iter++;
    }
	for(auto elem : consensus)
    {
		
	}
	
    return 0;
}



int main()
{
    string top, side;
    //string aligned_top, aligned_side;
    int match, mismatch, gap, extend, is_msa;
    int answer_loop = 0;
    while(answer_loop == 0)
    {
        string answer;
        cout << "Is this a single pairwise alignment (P) or a multiple sequence alignment (M)?"<<endl;
        cin >> answer;
        if(answer == "M")
        {
            is_msa = 1;
            answer_loop = 1;
        }
        else if(answer == "P")
        {
            is_msa = 0;
            answer_loop = 1;
        }
        else
        {
            cout<<"...repeat that?"<<endl;
        }
    }   
    cout << "what is the gap value?";
    cin >> gap;
    cout << "what is the match value?";
    cin >> match;
    cout << "what is the mismatch value?";
    cin >> mismatch;
    cout << "what is the extend gap value?";
    cin >> extend;
    int type_check = 0;
    answer_loop = 0;
    while(answer_loop == 0)
    {
        string answer;
        cout << "Are you aligning a protein sequence? (Y/N)"<<endl;
        cin >> answer;
        if(answer == "Y")
        {
            type_check = 1;
            answer_loop = 1;
        }
        else if(answer == "N")
        {
            answer_loop = 1;
        }
        else
        {
            cout<<"...repeat that?"<<endl;
        }
    }
        
    string file;    
    cout << "Please enter the file with the sequences:";
    cin >> file;
    //file = "MSA_sequences.txt";
    if(is_msa == 0)
    {
        ifstream myfile (file); //"pairwise_nucleotide_1.fasta"
        string temp ="";
        if (myfile.is_open())
        {
            string line;
            string temp;
            int check = 0;
            while(getline(myfile, line))
            {
                temp = line.back();
                if(temp == "\r")
                {
                    line.pop_back();
                }
                //line.pop_back();
                //cout<<line<<endl;
                if(line.at(0) == '>')
                {
                    check++;
                }
                else if(check == 1)
                {
                    top=top+line;
                    // cout<<top<<endl;
                    // cout<<check<<endl;

                }
                else if(check == 2)
                {
                    side=side+line;
                    // cout<<side<<endl;
                    // cout<<check<<endl;
                }
            }
            myfile.close();
            cout << top << endl;
            cout << side << endl;
        }
        else
        {
            cout << "Unable to open file";
            return 1;
        }

        if(type_check == 0)
        {
            nucleotide_scoring(top, side, gap, match, mismatch, extend);
        }
        else
        {
            protein_scoring(top, side, gap, match, mismatch, "100pam.txt", extend);
        }   
    }
    else
    {
        ifstream myfile (file); //"pairwise_nucleotide_1.fasta"
        string temp ="";
        if (myfile.is_open())
        {
            string line;
            string temp;
            string input = "";
            string name = "";
            int check = 0;
            while(getline(myfile, line))
            {
                temp = line.back();
                if(temp == "\r")
                {
                    line.pop_back();
                }
                if(line.at(0) == '>')
                {
                    name = line;
                    input = "";
                    msa_names.push_back(name);
                    msa_name_to_seq[name] = "";
                }
                else
                {
                    input+=line;
                    msa_name_to_seq[name] = input;
                }
                
            }
            myfile.close();
        }
        else
        {
            cout << "Unable to open file";
            return 1;
        }
        myfile.close();

        for (pair<string, string> element : msa_name_to_seq)
        {
            cout << element.first << " :: " << element.second << endl;
        }
        cout<<"=========================="<<endl;

        //do msa_module things
        msa_module(gap, match, mismatch, extend, type_check);
    }
    

    // if(type_check == 0)
    // {
    //  nucleotide_scoring(top, side, gap, match, mismatch, extend);
    // }
    // else
    // {
    //  protein_scoring(top, side, gap, match, mismatch, "100pam.txt", extend);
    // }    
}