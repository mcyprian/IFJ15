/**
 * @file ial.c
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Built functions in IFJ2015 language (find)
 */

#include "ial.h"

int find(TDynamic_buffer *b, index_t string, TDynamic_buffer *c, index_t substring)
{
	/* creating fail */

	char *search = load_token(c, substring);
	char *s = load_token(b, string);
	int search_lenght = (int)strlen(search);

	int k,r;
	if (search_lenght == 0)
		return 0;

	int *fail = malloc((search_lenght) * sizeof(int));
	if (!fail)	// need to use sth from error module
	 	return -2;	// return INTERNAL_ERROR

	fail[0] = 0;
	for (k = 1; k < search_lenght; k++)
	{
		r = fail[k-1];
		while (r > 0 && search[r-1] != search[k])
			r = fail[r-1];

		fail[k+1] = r + 1;
	}

	/* find */
	int tInd = 1, pInd = 1;
	int s_lenght = (int)strlen(s);

	while (tInd <= s_lenght && pInd <= search_lenght)
	{
		if (pInd == 0 || s[tInd-1] == search[pInd-1])
		{
			tInd++;
			pInd++;
		}
		else
			pInd = fail[pInd - 1];
	}

	free(fail);
	fail = NULL;

	if (pInd > search_lenght)
		return tInd - search_lenght - 1;
	else
		return -1;
}

index_t sort(TDynamic_buffer *b, index_t string)
{
	save_token(b);	// whatever is in b, puts '\0' after that
	HeapSort(load_token(b, string));

	return string;
}

void SiftDown(char *a, int left, int right)
{
	int i,j,tmp;
	bool cont;
	i = left;
	j = 2 * i;
	tmp = a[i];
	cont = (j <= right);

	while (cont)
	{
		if (j < right)
		{
			if (a[j] < a[j + 1])
				j++;
		}

		if (tmp >= a[j])
			cont = false;
		else
		{
			a[i] = a[j];
			i = j;
			j = 2 * i;
			cont = (j <= right);
		}
	}

	a[i] = tmp;
}

void HeapSort(char *a)
{
	int i,left,right,tmp;
	int n = (int)strlen(a) - 1;

	left = n / 2 ;
	right = n ;
	for (i = left; i >= 0; i--)
		SiftDown(a, i, right);

	for (right = n ; right >= 1; right--)
	{
		tmp = a[0];
		a[0] = a[right];
		a[right] = tmp;
		SiftDown(a, 0, right - 1);
	}
}

