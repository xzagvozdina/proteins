#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
// #include <float.h>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>

using namespace std;

struct dot 
{
	int x;
	int y;
	int z;
};

const vector <dot> steps = { {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };

// void generate_walks(vector <dot> &path, int L, vector <vector <dot>> &cache)
// {
// 	if ( L == 0) cache.push_back(path);
// 	else
// 	{
// 		for(int i = 0; i < 6; i++)
// 		{
// 			dot dot_cur = path.back();
// 			// cout << dot_cur.x << " " << dot_cur.y << " " << dot_cur.z << endl;
// 			dot dot_new = {dot_cur.x + steps[i].x, dot_cur.y + steps[i].y, dot_cur.z + steps[i].z};
// 			// cout << dot_new.x << " " << dot_new.y << " " << dot_new.z << endl;
// 			vector <dot> pp = path;
// 			// for(int j = 0; j < path.size(); j++) cout << pp[j].x << " " << pp[j].y << " " << pp[j].z << endl;
// 			pp.push_back(dot_new);
// 			generate_walks(pp, L-1, cache);
// 		}
// 	}
// }
// size_t
void print_saws(vector <vector <dot>> &cache)
{
	for (int i = 0; i < cache.size(); i++)
	{
		for (int j = 0; j < cache[0].size(); j++) cout << "{" << cache[i][j].x << ", " << cache[i][j].y << ", " << cache[i][j].z << "}, ";
		cout << endl;
	}
}

void generate_saws(vector <dot> &path, int L, vector <vector <dot>> &cache)
{
	bool flag = true;
	if ( L == 0) cache.push_back(path);
	else
	{
		for(int i = 0; i < 6; i++)
		{
			dot dot_cur = path.back();
			// cout << dot_cur.x << " " << dot_cur.y << " " << dot_cur.z << endl;
			dot dot_new = {dot_cur.x + steps[i].x, dot_cur.y + steps[i].y, dot_cur.z + steps[i].z};
			// cout << dot_new.x << " " << dot_new.y << " " << dot_new.z << endl;
			flag = true;
			for (int i = 0; i < path.size(); i++)
				if ((path[i].x==dot_new.x) && (path[i].y==dot_new.y) && (path[i].z==dot_new.z)) 
				{
					flag = false;
					break;
				}

			if (flag) 
			{
				vector <dot> pp = path;
				pp.push_back(dot_new);
				generate_saws(pp, L-1, cache);
			}
		}
	}
}

void print_dot(dot point)
{
	cout << "{" << point.x << ", " << point.y << ", " << point.z << "}" << endl;
}

double dist3D_2(dot point1, dot point2={0, 0, 0})
{
	return (double)(pow(point1.x-point2.x, 2) + pow(point1.y-point2.y, 2) + pow(point1.z-point2.z, 2));
}

dot minus3D(dot point1, dot point2)
{
	return {point2.x-point1.x, point2.y-point1.y, point2.z-point1.z};
}

vector <int> topological_n(vector <dot> &path, int point_number)
{
	vector <int> neighs;
	dot point = path[point_number];
	for (int i = 0; i < path.size(); i++)
	{
		double d = dist3D_2(point, path[i]);
		if (d <= 1 && d >= 1e-3 && i - 1 != point_number && i + 1 != point_number)  
			neighs.push_back(i);
	}
	return neighs;
}

vector <vector <int>> HP_seqs(int L)
{
    vector <vector <int>> seqs;
    
	int flag, len;
	for (int i = 0; i < pow(2, L+1); i++)
	{
        vector <int> cur_seq;
        vector <int> rcur_seq;
		int cur_i = i;
		int cur_mod = 0;
		// cout << cur_i << endl;
		for (int j = 0; j < L+1; j++)
		{
			cur_mod = cur_i%2;
			cur_seq.insert(cur_seq.begin(), cur_mod);
			rcur_seq.push_back(cur_mod);
			cur_i = cur_i/2;
		}
		
		len = seqs.size();
		if (len <= 0) 
			seqs.push_back(cur_seq);
		else
		{
			for (int k = 0; k < len; k++)
			{
				// cout << k << " ";
				flag = 0;
				for (int l = 0; l < L+1; l++)
					if (seqs[k][l] == rcur_seq[l])
						flag++;
				if (flag >= L)
					break;
			}
			if (flag < L) 
				seqs.push_back(cur_seq);
		}
	}
	
    return seqs;
}

vector <vector <int>> Ising_seqs(int L)
{
	vector <vector <int>> seqs = HP_seqs(L);

	for (int i = 0; i < pow(2, L+1); i++)
		for (int j = 0; j < L+1; j++)
			if (seqs[i][j] == 0)
				seqs[i][j] = -1;

	return seqs;
}

int energy(vector <dot> &path, vector <int> &seq)
{
	int cnt = 0;

	for (int point_ind = 0; point_ind < path.size(); point_ind++)
	{
		vector <int> tops = topological_n(path, point_ind);
		
		for (int i = 0; i < tops.size(); i++)
			cnt += seq[point_ind] * seq[tops[i]];
	}

	return cnt/2;
}

double Phi(vector <int> &seq)
{
	int sum = 0;
	int len = seq.size();
	for (int i = 0; i < len; i++)
		sum += seq[i];

	return (double)(sum)/(double)(len);
}

void r_arithm(int L_min, int L_max)
{
	vector <dot> path = { {0, 0, 0} };
	vector <vector <dot>> cache;
	int cache_len;
	double r_2, rg_2;
	ofstream myfile;
  	myfile.open ("r_arithm.dat");
	for (int L = L_min; L < L_max; L++)
	{
		generate_saws(path, L+1, cache);
		// print_saws(cache);
		cache_len = cache.size();
		r_2 = 0.;
		rg_2 = 0.;

		for (int k = 0; k < cache_len; k++)
		{
			r_2 += dist3D_2(cache[k][L+1], cache[k][0]);
			for (int ind_i = 0; ind_i < L+2; ind_i++)
				for (int ind_j = 0; ind_j < L+2; ind_j++)
					rg_2 += dist3D_2(minus3D(cache[k][ind_j], cache[k][ind_i]));
			// print_dot(cache[i][0]);
		}
		r_2 /=  (double)(cache_len);
		rg_2 /=  2.*(double)(cache_len)*(double)(pow(L+2, 2));
		cout << "L = " << L+1 << ", r^2 = " << r_2 << ", rg^2 = " << rg_2 << endl;
		myfile << L+1 << " "  << r_2 << " " << rg_2 << endl;	

		cache.clear();
	} 
  	myfile.close();
}

void r_stat_HP(int L_min, int L_max)
{
	vector <dot> path = { {0, 0, 0} };
	vector <vector <dot>> cache;
	int cache_len;
	double r_2, rg_2;
	ofstream myfile;
  	myfile.open ("r_stat.dat");
	for (int L = L_min; L < L_max; L++)
	{
		generate_saws(path, L+1, cache);
		// print_saws(cache);
		cache_len = cache.size();
		r_2 = 0.;
		rg_2 = 0.;

		for (int k = 0; k < cache_len; k++)
		{
			r_2 += dist3D_2(cache[k][L+1], cache[k][0]);
			for (int ind_i = 0; ind_i < L+2; ind_i++)
				for (int ind_j = 0; ind_j < L+2; ind_j++)
					rg_2 += dist3D_2(minus3D(cache[k][ind_j], cache[k][ind_i]));
			// print_dot(cache[i][0]);
		}
		r_2 /=  (double)(cache_len);
		rg_2 /=  2.*(double)(cache_len)*(double)(pow(L+2, 2));
		cout << "L = " << L+1 << ", r^2 = " << r_2 << ", rg^2 = " << rg_2 << endl;
		myfile << L+1 << " "  << r_2 << " " << rg_2 << endl;	

		cache.clear();
	} 
  	myfile.close();
}

int main()
{
	// int L;
	// cout << "L = ";
	// cin >> L;
	// cin.get();
	
	
	r_arithm(0, 10);
	
	
	/* OUTPUT */
	// cout << "N SAWs = " << cache.size() << endl;
	// print_saws(cache);

	// vector <vector <int>> seqs = HP_seqs(3);
	// cout << seqs.size() << endl;
	// for (int i = 0; i < seqs.size(); i++)
	// {
	// 	for (int j = 0; j < 3+1; j++) 
	// 		cout << seqs[i][j] << " ";
	// 	cout << endl;
	// }

	return 0;
}
