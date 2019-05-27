// proteins.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <float.h>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <float.h>
#include <vector>

using namespace std;

struct dot 
{
	int x;
	int y;
	int z;
};

// void generate_saws(vector <vector <int>> path, int L, vector <vector <vector <int>>> cache)
// {
// 	vector <vector <int>> steps = { {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };

// 	if (L == 0)
// 	{
// 		cache.push_back(path);
// 	}
// 	else
// 	{
// 		for (int i = 0; i < 6; i++)
// 		{
// 			vector <int> cur_step = path.back();

// 			vector <int> new_step(3);
// 			for (int k = 0; k < 3; k++)
// 			{
// 				new_step[k] = cur_step[k] + steps[i][k];
// 			}
// 			bool trap = true;

// 			for (int j = 0; j < path.size(); j++)
// 			{
// 				if (new_step == path[j])
// 				{
// 					trap = false;
// 					break;
// 				}
// 			}

// 			if (trap)
// 			{
// 				vector <vector <int>> pp(path);
// 				pp.push_back(new_step);
// 				generate_saws(pp, L - 1, cache);
// 			}
// 		}
// 	}
// }

void generate_walks(vector <dot> path, int L, vector <vector <dot>> cache)
{
	vector <dot> steps = { {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };

	if (L == 0)
	{
		cout << "L == 0" << endl;
        cache.push_back(path);
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
		    cout << "i = " << i << endl;
			dot cur_step = path.back();

			dot new_step;
            new_step.x = cur_step.x + steps[i].x;
            new_step.y = cur_step.y + steps[i].y;
            new_step.z = cur_step.z + steps[i].z;
		    cout << "new_step" << endl;

			vector <dot> pp;
            pp = path;
		    cout << "pp" << endl;
            pp.push_back(new_step);
		    cout << "push_back" << endl;
            generate_walks(pp, L - 1, cache);
		    cout << "recursion" << endl;
		}
	}
}

int main()
{
	vector <dot> path = {{0, 0, 0}};
	vector <vector <dot>> cache = { {{}} };
	int L;

	cout << "L = ";
	cin >> L;
	cin.get();

    generate_walks(path, L, cache);
	cout << cache.size() << endl;


	// generate_saws(path, L, cache);

	// cout << path[0].size() << endl;

	/*for (int i = 0; i < cache.size(); i++)
	{
		cout << i << ": { ";
		for (int j = 0; j < cache[0].size(); j++)
		{
			cout << "{ ";
			for (int k = 0; k < 3; k++)
			{
				cout << cache[i][j][k] << " ";
			}
			cout << "} ";
		}
		cout << "};" << endl;
	}*/

	// cin.get();
	// cin.get();

	/*vector <vector <int>> steps = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };

	cout << steps.size() << endl;
	cin.get();*/
}