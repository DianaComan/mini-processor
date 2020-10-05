#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.c"

FILE *f1, *f2, *f3;
char ar[23][23];
char br[23][23];
char buf[256];
int checkdef;
int k, kk;
char *infile;
char *dir; char *dir2; char *dirwin, *dir2win;
char *outfile;
int stdo = 1;
int checkvar, checkvars;
char path[100];
char axx[23];
int rez, ign;
int inputccheck;
int  arcc;

/*functie de define*/
void deffunc(struct table *t, char *inn)
{
	char *p;

	memset(axx, 0, sizeof(axx));
	if (inn[strlen(inn)-1] == '\\')
		checkdef = 1;
	p = strtok(inn, " ");
	p = strtok(NULL, " ");
	memcpy(ar[k], p, (strlen(p)+1)*sizeof(char));
	if (checkdef == 1) {
		p = strtok(NULL, "\\");
		strcpy(axx, p);
		strcat(br[k], p);
	}
	if (checkdef == 0) {
		p = strtok(NULL, "\\");
		memcpy(br[k], p, (strlen(p)+1)*sizeof(char));
		insert(t, ar[k], br[k]); k++;
	}
}
/*define pe mai multe linii*/
void multidef(struct table *t, char *inn)
{
	char *p;

	if (inn[strlen(inn)-1] == '\\')  {
		checkdef = 1;
		p = strtok(inn, " \t");
		strcat(br[k], " ");
		strcat(br[k], p);
		p = strtok(NULL, "\\");
		strcat(br[k], " ");
		strcat(br[k], p);
	} else {
		checkdef = 0;
		p = strtok(inn, "\t");
		strcat(br[k], " ");
		strcat(br[k], inn);
		insert(t, ar[k], br[k]); k++;
	}
}
/*functie pt if/elif*/
void iffunc(struct table *t, char *inn)
{
	char *p, *aux, header[100];
	int bs, as;

	aux = (char *)malloc(sizeof(char)*(strlen(inn)+1));
	memcpy(aux, inn, (strlen(inn)+1)*sizeof(char));
	p = strtok(aux, " ");
	p = strtok(NULL, " ");
	memcpy(header, p, (strlen(p)+1)*sizeof(char));
	as = atoi(lookup(t, header));
	bs = atoi(header);
	if ((as != 0) || (bs != 0))
		rez = 1;
	else
		ign = 1;
	free(aux);
}

/*functie pt ifdef, ifndef*/
void checkfunc(struct table *t, char *inn)
{
	char *aux, *p, *rc, header[100];
	int v;

	aux = (char *)malloc(sizeof(char)*(strlen(inn)+1));
	memcpy(aux, inn, (strlen(inn)+1)*sizeof(char));
	p = strtok(aux, " ");
	p = strtok(NULL, " ");
	memcpy(header, p, (strlen(p)+1)*sizeof(char));
	rc = lookup(t, header);
	v = strcmp(rc, "null");
	if ((v != 0) && (checkvars == 1)) {
	} else
		if ((v == 0) && (checkvars == 0))
			rez = 1;
		else
			ign = 1;
	free(aux);
}
/*undef*/
void undeffunc(struct table *t, char *inn)
{
	char *aux, *p, header[100], *rc;
	int v;

	aux = (char *)malloc(sizeof(char)*(strlen(inn) + 1));
	memcpy(aux, inn, (strlen(inn)+1)*sizeof(char));
	p = strtok(aux, " ");
	p = strtok(NULL, " ");
	memcpy(header, p, (strlen(p)+1)*sizeof(char));
	rc = lookup(t, header);
	v = strcmp(rc, "null");
	if (v != 0)
		insert(t, header, "null");
	free(aux);
}
/*functie de replace a cuvintelor*/
char *replacec(const char *del, const char *cuv, const char *cuvN)
{
	int i, cnt = 0, l1, l2;
	char *result;

	l1 = strlen(cuvN); l2 = strlen(cuv);
	for (i = 0; del[i] != '\0'; i++) {
		if (strstr(&del[i], cuv) == &del[i]) {
			i = i + l2 - 1;
			cnt = cnt + 1;
		}
	}
	result = (char *)malloc(200);
	i = 0;
	while (*del) {
		if (strstr(del, cuv) == del) {
			strcpy(&result[i], cuvN);
			i += l1;
			del += l2;
		} else
			result[i++] = *del++;
	}
	result[i] = 0;
	return result;
}

/*functie care cauta sa vada daca exista cuvinte de schimbat*/
void editar(struct table *t, char inn[256])
{
	char *delim = " []{}<>=+-*/%!&|^.,:;().";
	char  *token, *aux, *repl, *result;
	int ok = 1;

	aux = (char *) malloc(sizeof(char) * strlen(inn) + 1);
	
		repl = (char *) malloc(sizeof(char) * strlen(inn) + 1);
		strcpy(repl, inn);
		strcpy(aux, inn);
		token = strtok(aux, delim);
		while (token != NULL) {
			if ((lookup(t, token)) != "null") {
				result = replacec(repl, token,
						lookup(t, token));
				strcpy(repl, result);
				ok = 0;
			}
			token = strtok(NULL, delim);
		}
		if (ok)
			fprintf(f2, "%s\n", inn);
		else
			fprintf(f2, "%s\n", repl);
		free(repl);
		free(aux);
}
/*functie pt liniile fara #*/
void mainfunc(struct table *t, char inn[256])
{
	char au[256]; int oke = 1; char *p, *p1;
	char token[10][256]; int index = 0; char *line, *part;
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\.";

	if (strlen(inn) == 0)
		oke = 0;
	if (ign == 1)
		oke = 0;
	if (oke == 1) {
		strcpy(au, inn);
		editar(t, au);
	}
	ign = 0;
}
/*parsare a liniilor*/
void editt(struct table *t, char inn[256])
{
	char *aux, *p, header[100];
	char la[256];
	int checkmain = 1; int checkif = 1;

	if (checkdef == 1) {
		multidef(t, inn); checkmain = 0;
	}
	if (strstr(inn, "#")) {
		checkmain = 0;
		if (strstr(inn, "define") != NULL)
			deffunc(t, inn);
		if (strstr(inn, "elif")) {
			if (rez == 0)
				iffunc(t, inn);
			else
				ign = 1;
			checkif = 0;
		}
		if (strstr(inn, "else"))
			if (rez == 1)
				ign = 1;
			else
				ign = 0;
		if (strstr(inn, "endif")) {
			ign = 0;  rez = 0; checkif = 0;
		}
		if (strstr(inn, "undef"))
			undeffunc(t, inn);
		if (strstr(inn, "ifdef")) {
			checkvars = 1; checkif = 0;
			checkfunc(t, inn);
		}
		if (strstr(inn, "ifndef")) {
			checkvars = 0; checkif = 0;
			checkfunc(t, inn);
		}
		if (strstr(inn, "if"))
			if (checkif == 1)
				iffunc(t, inn);
		if (strstr(inn, "include")) {
			aux = (char *) malloc(sizeof(char)*
				(strlen(inn)+1));

			memcpy(aux, inn, (strlen(inn)+1)*sizeof(char));
			p = strtok(aux, " ");

			p = strtok(NULL, " ");
			memmove(p, p+1, strlen(p));
			p[strlen(p) - 1] = 0;
			header[100];

			memcpy(header, p, (strlen(p)+1)*sizeof(char));
			f3 = fopen(header, "r");
			if (kk == 1) {
				if (f3 == NULL) {
					strcat(path, dir);
					strcat(path, header);
					f3 = fopen(path, "r");
				}
			}
			while (fgets(buf, 256, f3)) {
				buf[strlen(buf)-2] = 0;
				editt(t, buf);
				fprintf(f2, "%s\n", buf);
			}
			free(aux);
		}
	}

	strcpy(la, inn);
	if (checkmain == 1)
		mainfunc(t, la);
}

int main(int argc, char *argv[])
{
	int i = 1;
	char *p;
	char *c;
	char *al;
	int ii;
	struct table *t;
	int kkk = 0;
	size_t len; int std = 1;
	int checkbad = 1;
	char aux[200];
	t = createTable(50);
	if (t == NULL) {
		free(t);
		exit(12);
	}
	/*parametrii*/
	if (argc > 1) {
		while (i<argc) {
			if ((argv[i][0] == '-') && (argv[i][1] == 'D')) {
				if (strlen(argv[i]) == 2) {
					i++;
					c = argv[i];
					i++;
				}
				else {
					c = argv[i];
					len = strlen(c);
					memmove(c, c+2, len - 1);
					i++;
				}
				memcpy(aux, c, (strlen(c)+1)*sizeof(char));
				p = strchr(aux, '=');
				if (p) {
					int index = p - aux;
					char aux1[200];
					memcpy(aux1, aux, index*sizeof(char));
					aux1[index] = 0;
					memmove(aux, aux+index+1, strlen(aux) - index);
					memcpy(ar[k], aux1, (strlen(aux1)+1)*sizeof(char)); 
					memcpy(br[k], aux, (strlen(aux)+1)*sizeof(char));
					insert(t, ar[k], br[k]); k++;
				} else {
				memcpy(ar[k], aux, (strlen(aux)+1)*sizeof(char)); 
					memcpy(br[k], "0", (strlen("0")+1)*sizeof(char));
					insert(t, ar[k], br[k]); k++;
				}
			} else {
			if (argv[i][0] != '-') {
				if (inputccheck == 0) {
				infile = argv[i];
				i++; inputccheck = 1;
				std = 0;
				}
				else {
				outfile = argv[i];
				i++;
				stdo = 0;
				}
			} else {
			if ((argv[i][0] == '-') && (argv[i][1] == 'I')) {
				i++;
				if (kk==0) {
					dir = argv[i]; kk=1;
				} else {
					dir2 = argv[i]; kk=2;
				}
				i++;
			} else {
			if ((argv[i][0] == '-') && (argv[i][1] == 'o')) {
				i++;
				outfile = argv[i];
				i++;
				stdo = 0;
			}
			else {
			if (argv[i][0] == '-') {
				 free(t); return -1;
			}
			}
			}
			}
			}
		}
	}
	if (stdo == 1)
		f2 = stdout;
	else
		f2 = fopen(outfile, "w");
	if (f2 != NULL) {
		/*citire*/
		if (std == 1)
			f1 = stdin;
		else
			f1 = fopen(infile, "r");
		if (f1 != NULL) {
			while (fgets(buf, 256, f1)) {
				buf[strlen(buf)-1] = 0;
				editt(t, buf);
			}
			fclose(f1);
		} else {
			free(t);
			if (stdo == 0)
				fclose(f2);
			return -1;
		}
	} else {
		free(t); return -1;
	}
	if (stdo == 0)
		if (f2)
			fclose(f2);
	return 1;
}
