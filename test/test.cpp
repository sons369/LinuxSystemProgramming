#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

#define PI 3.14
#define CIRCLE(x) (x) * (x) * (3.14)

class polynomial
{
private:
	int total;
	int **nArr;
	vector<vector<int>> v;
	vector<int> num;

public:
	void LoadPolynomial(string &filename)
	{
		ifstream fs;
		char buf[1000];
		int i = 0;
		int flag = 0;

		fs.open(filename);
		if (fs.fail())
		{
			cerr << "file open error\n";
		}
		while (fs.getline(buf, 1000))
		{
			if (flag == 0)
			{
				total = atoi(buf);
				nArr = new int *[atoi(buf)];
				for (i = 0; i < atoi(buf); i++)
					nArr[i] = new int[2];
				i = 0;
				flag = 1;
			}
			else
			{
				nArr[i][0] = atoi(buf);
				nArr[i][1] = atoi(strrchr(buf, ' '));
				i++;
			}
		}
		fs.close();
	}
	void PrintPolynomial()
	{
		for (int i = 0; i < total - 1; i++)
		{
			for (int j = 0; j < total - 1; j++)
			{
				if (nArr[j][1] < nArr[j + 1][1])
				{
					int tmp1, tmp2;
					tmp1 = nArr[j][0];
					tmp2 = nArr[j][1];
					nArr[j][0] = nArr[j + 1][0];
					nArr[j][1] = nArr[j + 1][1];
					nArr[j + 1][0] = tmp1;
					nArr[j + 1][1] = tmp2;
				}
			}
		}
		for (int i = 0; i < total; i++)
		{
			cout << nArr[i][0] << " " << nArr[i][1] << endl;
		}
	}
	void Add(polynomial a, polynomial b)
	{
		for (int i = 0, j = 0; i < a.total, j < b.total;)
		{
			if (a.nArr[i][1] > b.nArr[j][1])
			{

				num.push_back(a.nArr[i][0]);
				num.push_back(a.nArr[i][1]);
				v.push_back(num);
				i++;
			}
			else if (a.nArr[i][1] < b.nArr[j][1])
			{

				num.push_back(a.nArr[j][0]);
				num.push_back(a.nArr[j][1]);
				v.push_back(num);
				j++;
			}
			else if (a.nArr[i][1] == b.nArr[j][1])
			{
				if ((a.nArr[i][0] + b.nArr[j][0]) != 0)
				{
					num.push_back(a.nArr[i][0] + b.nArr[j][0]);
					num.push_back(a.nArr[i][1]);
					v.push_back(num);
				}
				i++;
				j++;
			}
			if (!num.empty())
			{
				num.pop_back();
				num.pop_back();
			}
		}
		for (int i = 0; i < v.size(); i++)
		{
			for (int j = 0; j < v[i].size(); j++)
				cout << v[i][j] << " ";
			cout << "\n";
		}
	}
};

int main(void)
{
	polynomial p1, p2, p3;
	string str;
	getline(cin, str);
	p1.LoadPolynomial(str);
	p1.PrintPolynomial();
	getline(cin, str);
	p2.LoadPolynomial(str);
	p2.PrintPolynomial();
	p3.Add(p1, p2);
}