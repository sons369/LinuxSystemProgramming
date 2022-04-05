#include <stdio.h>
#include <stdlib.h>

char buf[17];
char v[3] = {'a', 'b'};

void dfs(int n, int depth)
{
	if (depth == n)
	{
		for (int i = 0; i < n; i++)
			printf("%c ", buf[i]);
		printf("\n");
		return;
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			buf[depth] = v[i];
			dfs(n, depth + 1);
		}
	}
}

int main()
{
	dfs(16, 0);
	exit(0);
}