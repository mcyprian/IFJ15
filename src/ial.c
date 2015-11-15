/**
 * @file ial.c
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Built functions in IFJ2015 language (find)
 */

#include <ial.h>

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