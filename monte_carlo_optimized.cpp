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
#include <chrono> 

using namespace std;

vector<string> msa_names;
vector<string> msa_names_alt;
vector<double> resample_times;
vector<double> resample_times_fhalf;
vector<double> resample_times_shalf;
vector<double> parse_times;
vector<double> cmd_times;
vector<double> rand_times;
string treestring;
unordered_map<string, string> msa_name_to_seq;
map<int, string> msa_int_to_seq;
void lcg(int seed, int mod, int multiplier, int increment,  vector<int>& rand_vect, int rand_total)
{
    srand (time(NULL));
    auto start_rand = chrono::high_resolution_clock::now(); 
    for(int x = 0; x < rand_total; x++)
    {
        rand_vect[x] = (rand()) % mod;
    }
    auto end_rand = chrono::high_resolution_clock::now(); 
    double duration_rand = chrono::duration_cast<chrono::nanoseconds>(end_rand - start_rand).count();  
    duration_rand *= 1e-9;
    rand_times.push_back(duration_rand);
}

int dnadist(string msa_name)
{
    int i;
    ofstream dnadist_file;
    dnadist_file.open ("input");
    dnadist_file << msa_name << "\n";
    dnadist_file << "dnadist_out\n";
   
    
    dnadist_file << "R\n";
    dnadist_file << "Y\n";

    dnadist_file.close();
    system("/usr/local/bin/phylip/./dnadist < input");
    system("mv outfile dnadist_out > /dev/null 2>&1");
    system("rm input");
    return 0;

}

int neighbor()
{
    int i;
    system("ls -al");
    cout<< ("The value returned was: %d.\n",i);
    ofstream neighbor_file;
    neighbor_file.open ("input");
    neighbor_file << "dnadist_out"<<"\n";
    neighbor_file << "Y\n";
    neighbor_file << "R\n";

    
   
    neighbor_file.close();
    system("/usr/local/bin/phylip/./neighbor < input > /dev/null 2>&1");
    i = system("rm dnadist_out");
    i = system("rm input");
    return 0;
}

int random_resample(int resample, string infile)
{
    auto start_resamp = chrono::high_resolution_clock::now(); 
    time_t start_resamp_A, end_resamp_A; 
    time_t start_resamp_B, end_resamp_B; 
    time(&start_resamp_A);
    //read aligned sequences to randomly resample
    ifstream myfile2 (infile); 
    int check = 0;
    int given_len;
    int resample_count=0;
    if (myfile2.is_open())
    {
        string line;
        string temp = "";
        string input = "";
        string name = "";
        string intersect_name;
        int looper = 0;
        while(getline(myfile2, line))
        {
            input = line;
            int lsize = line.size();
            temp = line.back();
            if(temp == "\r")
            {
                line.pop_back();
            }
            if(lsize > 0)
            {
            }
            else
            {
                looper = 0;
                check = 1;
            }
            if(check == 0)
            {
                given_len = lsize;
                int max_size = 0;
                string holder;
                intersect_name = line.substr(0,10);
                msa_names_alt.push_back(intersect_name);
                string::size_type comp = line.find(intersect_name);
                if (comp != string::npos) 
                {
                    input.erase(comp, (intersect_name).length());
                    msa_int_to_seq[looper] = input;
                    looper++;
                }
            }
            else if(check == 1 && resample == 1)
            {
                msa_int_to_seq[looper]+=input;
                looper++;
                
            }

            
            
        }
        myfile2.close();
    }
    else
	{
		cout << "Unable to open file\n";
		return 1;
	}
    time(&end_resamp_A); 
    double duration_resamp = double(end_resamp_A - start_resamp_A);
    resample_times_fhalf.push_back(duration_resamp);


    //resample
    time(&start_resamp_B);
    srand (time(NULL));
    int total_size = msa_int_to_seq[2].size();
    vector<int> rand_vect(total_size);

    int seed = rand()%total_size;
    int mod = total_size;
    int multiplier = rand()%total_size;
    int increment = rand()%total_size;

    lcg(seed, mod, multiplier, increment, rand_vect, total_size);

    int resample_iter = 0;
    map<int, string> resamp_int_to_seq = msa_int_to_seq;

    for(int col = 1; col<rand_vect.size(); col++)
    {
        for(int row = 1; row<=msa_names.size()+1; row++)
        {
            resamp_int_to_seq[row].at(resample_iter) =  msa_int_to_seq[row].at(rand_vect.at(col));
            
        }
        resample_iter++;
    }
    for(int row = 1; row<=msa_names.size()+1; row++)
    {
        resamp_int_to_seq[row]=msa_names_alt[(row)]+resamp_int_to_seq[row];
    }
    ofstream myfile_out ("resampled_alignment");
    myfile_out << " " << msa_names.size()+1 << "  " << total_size << "\n";
    int write_count = 0;
    while(write_count < resamp_int_to_seq[2].size())
    {
        for(pair<int, string> element : resamp_int_to_seq)
        {
            if((element.second).size() < 10)
            {

            }
            else
            {
                for(int write_lim = 0; write_lim<50; write_lim++)
                {   
                    
                    if(write_lim+write_count >= resamp_int_to_seq[2].size())
                    {
                        //hit the end
                        break;
                    }
                    myfile_out << (element.second).at(write_lim+write_count);
                }
                
                myfile_out << "\n";
            }
            
        }
        write_count+=50;
        myfile_out<<"\n";
    }
    myfile_out.close();
    time(&end_resamp_B);
    auto end_resamp = chrono::high_resolution_clock::now(); 
    duration_resamp = chrono::duration_cast<chrono::nanoseconds>(end_resamp - start_resamp).count();  
    duration_resamp *= 1e-9;
    resample_times.push_back(duration_resamp);
    duration_resamp = double(end_resamp_B - start_resamp_A);
    resample_times_shalf.push_back(duration_resamp);
    cout<<"done"<<endl;
    return 0; 
}

int main()
{
    
    srand (time(NULL));
    string infile;
    string outfile;
    cout << "Please enter the file with the sequences:\n";
	cin >> infile;
    int i;
    int times;
    cout << "Please enter the number of times you want to bootstrap:\n";
	cin >> times;
    time_t start_main, end_main; 
    time(&start_main); 

    ifstream myfile (infile); 
    if (myfile.is_open())
    {
        string line;
        string temp;
        string input = "";
        string name = "";
        int check = 0;
        int seq_iter = 0;
        while(getline(myfile, line))
        {
            string line;
            string temp;
            string input = "";
            string name = "";
            int check = 0;
            int seq_iter = 0;
            while(getline(myfile, line))
            {
                if(check != 2)
                {
                    if(line.size()>0 && check == 1)
                    {
                        temp = line.erase(10);
                        cout<<temp<<endl;
                        msa_names.push_back(temp);
                    }
                    else
                    {
                        check++;
                    }        
                }
            }
            
        }

        
    }
    else
	{
		cout << "Unable to open file";
		return 1;
	}

    
    //do dnadist 
    dnadist(infile);
    //do neighbor 
    neighbor();

    treestring = "";
    ifstream treefile ("outtree");
    if(treefile.is_open())
    {
        string line;
        while(getline(treefile, line))
        {
            treestring += line;
        }
    }
    cout<<treestring<<endl<<endl;
    string treestring_f = treestring;
      
    vector<string> clade;
    vector<string> clade_cross;
    vector<int> bad_val;
    vector<int> total_match;

    for(int go = 0; go<treestring.size(); go++)
    {
        if(treestring.at(go) == ')')
        {
            for(int back = go; back>=0; back=back-1)
            {
                if(treestring.at(back) == '(')
                {
                    int ref = back;
                    if(find(bad_val.begin(), bad_val.end(), ref) != bad_val.end())
                    {
                        //already hit this '('
                    }
                    else
                    {
                        string pre_scrub = treestring.substr(back,(go-back+1));
                        string post_scrub;
                        int start = 0;
                        int stop = 0;
                        int go = 0;
                        for(int scrub = 0; scrub<pre_scrub.size(); scrub++)
                        {
                            char checker = pre_scrub.at(scrub);
                            if(checker == ':')
                            {
                                start = scrub;
                                go = 1;
                            }
                            else if(checker == ',' || checker == ')')
                            {
                                if(go == 1)
                                {
                                    pre_scrub.erase(start,(scrub-start));
                                    scrub=scrub-start;
                                    go = 0;
                                }
                                
                            }
                        }
                        cout<<endl;
                        clade.push_back(pre_scrub);
                    
                        total_match.push_back(0);
                        
                        bad_val.push_back(back);
                        back = -1;
                    }
                }
            }
        }
    }

    //do more resamplings N times
    time_t start_cmds, end_cmds; 
    for(int N = 0; N < times; N++)
    {
        auto start_cmds = chrono::high_resolution_clock::now(); 
        random_resample(1, infile);
        //do dnadist with resample
        dnadist("resampled_alignment");
        //do neighbor with dnadist
        neighbor();
        auto end_cmds = chrono::high_resolution_clock::now(); 
        double duration_cmds = chrono::duration_cast<chrono::nanoseconds>(end_cmds - start_cmds).count(); 
        duration_cmds *= 1e-9;
        cmd_times.push_back(duration_cmds);
        ifstream treefile ("outtree");
        if(treefile.is_open())
        {
            string line;
            while(getline(treefile, line))
            {
                treestring += line;
            }
        }
        
        auto start_parse = chrono::high_resolution_clock::now(); 
        //partse treestring for results
        for(int go = 0; go<treestring.size(); go++)
        {
            if(treestring.at(go) == ')')
            {
                for(int back = go; back>=0; back=back-1)
                {
                    if(treestring.at(back) == '(')
                    {
                        int ref = back;
                        if(find(bad_val.begin(), bad_val.end(), ref) != bad_val.end())
                        {
                            //already hit this '('
                        }
                        else
                        {
                            string pre_scrub = treestring.substr(back,(go-back+1));
                            string post_scrub;
                            int start = 0;
                            int stop = 0;
                            int go1 = 0;
                            for(int scrub = 0; scrub<pre_scrub.size(); scrub++)
                            {
                                char cross_ref = pre_scrub.at(scrub);
                                if(cross_ref == ':')
                                {
                                    start = scrub;
                                    go1 = 1;
                                }
                                else if(cross_ref == ',' || cross_ref == ')')
                                {
                                    if(go1 == 1)
                                    {
                                        pre_scrub.erase(start,(scrub-start));
                                        scrub=scrub-start;
                                        go1 = 0;
                                    }
                                    
                                }
                            }
                            cout<<endl;
                            clade_cross.push_back(pre_scrub);
                            
                            bad_val.push_back(back);
                            back = -1;
                        }
                    }
                }
            }       

        }
        //compare clade and clade cross
        for(int comp = 0; comp < clade.size(); comp++)
        {
            for(int comp2 = 0; comp2<clade_cross.size(); comp2++)
            {
                if(clade[comp] == clade_cross[comp2])
                {
                    total_match[comp] = total_match[comp]+1;
                    
                    comp2 = clade_cross.size();
                }
            }
        }
        auto end_parse = chrono::high_resolution_clock::now(); 
        double duration_parse = chrono::duration_cast<chrono::nanoseconds>(end_parse - start_parse).count(); 
        duration_parse *= 1e-9;
        parse_times.push_back(duration_parse);
        clade_cross.clear();
        bad_val.clear();

    }
    time(&end_main); 
    double duration_main = double(end_main - start_main); 
    string clade_val = clade.back();
    for(int final = total_match.size()-1; final>=0; final--)
    {
        cout<<"clade: " << clade[final]<< " \ntotal matches among bootstraps: "<< total_match[final] << "\nconfidance val: " << double(total_match[final])/double(times) << endl;
        size_t pos = clade_val.find(clade[final]);
        while( pos != string::npos)
        {
            string replace = clade[final]+":"+to_string(double(total_match[final])/double(times));
            clade_val.replace(pos, clade[final].size(), replace);
            pos = clade_val.find(clade[final], pos + replace.size());
        }
    }
    double average;
    cout<<clade_val<<endl<<endl;
    cout<<"total time from input to print: "<<duration_main<<" seconds"<<endl;
    cout.precision(6);
    cout<<"resample times\n";
    for(int res = 0; res<resample_times.size(); res++)
    {
        average = average + resample_times[res];
        cout<<resample_times[res]<<" ";
    }
    cout<<endl;
    cout<<endl<<"the total resample time was: "<<average<<" seconds."<<endl;
    average = average/resample_times.size();
    cout<<"the average resample time was: "<<average<<" seconds."<<endl;
    cout<<endl<<"phylip command execution times:\n";
    average = 0;
    for(int res = 0; res<cmd_times.size(); res++)
    {
        cout<<cmd_times[res]<<" ";
        average+=cmd_times[res];
    }
    cout<<"\nthe total cmd exec time was: "<<average<<" seconds."<<endl;

    cout<<endl<<"parsing output times:\n";
    average = 0;
    for(int res = 0; res<parse_times.size(); res++)
    {
        cout<<parse_times[res]<<" ";
        average+=parse_times[res];
    }
    cout<<"\nthe total parsing output times was: "<<average<<" seconds."<<endl<<endl;
    average = 0;
    cout<<"random number gen times:\n";
    for(int res = 0; res<rand_times.size(); res++)
    {
        cout<<rand_times[res]<<" ";
        average+=rand_times[res];
    }
    cout<<"\nthe total rand gen time was: "<<average<<" seconds."<<endl;

}