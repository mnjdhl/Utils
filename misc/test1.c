#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void arr_point() {
char a[][10] = {{'a' , 'r', 'u', 'n', 'H', 'N', '\0'}, {'V', 'I', 'n', 'o', 'd', '\0'}, {'B', 'h', 'a', 'r', 'a', 't', '\0'}};
char **b;

	
	/*printf("\n %c %c %c \n", *a[0], *a[1], *a[2]);
	 *b = *a+1;
	printf("%c %c %c \n", *b[0], *b[1], *b[2]);
	*/
	int i = 0, j =0;
	for (i=0;i<3;i++)
	{
		for (j=0;j<6;j++)
		{

			printf("%c", *(*(a+i)+j));
		}
		printf("\n");
	}

	i = 0;
	/*
	while ((a+i) != NULL)
	{
		j = 0;
		while (*(*(a+i)+j) != '\0')
		{
			printf("%c", *(*(a+i)+j));
			j++;
		}
		i++;
	}
	*/
}

void binary_shift() {

	signed char x = 0x80;
	printf("\n %x  %x\n", x & 0xFF, 0xFF & (x >> 1));

}

void div_test() {
	int a, b, c;

		a = 510;
		b = 512;
		c = a/b;
		printf("\ndiv_test(): %d %d\n", c, a%b);
		if (a%b > 0)
			c++;
		printf("\ndiv_test()_2: %d %d\n", c, a%b);

}

bool mstrcmp(char *s1, char *s2) {

	while (*s1 != '\0' && *s2 != '\0')
	{
		if (*s1 != *s2)
			return false;
		s1++;
		s2++;
	}

	if (*s2 == '\0')
		return true;

	return false;
}

char *mstrcpy(char *s1, char *s2) {
char *hd;

	hd = s1;

	while (*s2 != '\0')
	{
		*s1 = *s2;
		s2++;
		s1++;
	}

	//Now copy the null char
	*s1 = *s2;

	return hd;
}

char *mstrncpy(char *s1, char *s2, int ln) {
char *hd;
int i;

	hd = s1;

	i = 0;
	while (*s2 != '\0' && i < ln)
	{
		*s1 = *s2;
		s2++;
		s1++;
		i++;
	}

	//Now copy the null char
	if (i < ln )
		*s1 = *s2;

	return hd;
}
char *mmemcpy(char *s1, char *s2, int ln) {
char *hd;
int i;

	hd = s1;

	i = 0;
	while (i < ln)
	{
		*(s1 + i) = *(s2 + i);
		i++;
	}

	return hd;
}

char *mmemmov(char *s1, char *s2, int ln) {
char *hd;
//int i;
char *temp;

	hd = s1;
	temp = (char *)malloc(sizeof(char)*ln);
	if (temp == NULL)
		return NULL;
	temp = mmemcpy(temp, s2, ln);
	s1 = mmemcpy(s1, temp, ln);

	free(temp);
	return s1;
}

char *mmemmov2(char *s1, char *s2, int ln) {
char *hd;
//int i;
char *temp;

	hd = s1;
	temp = (char *)malloc(sizeof(char)*ln);
	temp = mmemcpy(temp, s2, ln);
	s1 = mmemcpy(s1, temp, ln);

	return s1;
}


char *mstrstr(char *s1, char *s2) {


	while (*s1 != '\0')
	{
		if (mstrcmp(s1, s2))
			return s1;
		s1++;
	}

	return NULL;
}

char *mstrtok(char *str, char *dlim) {

	static char *nxtptr;
	char *sp, *fsp;
	int dlen, tlen;
	bool flag;
	char *tok;

	if (str != NULL) {
		sp = str;
	} else {
		if (nxtptr == NULL)
			return NULL;
		sp = nxtptr;
	}

	dlen = strlen(dlim);
	flag = false;
	tlen = 0;
	fsp = sp;
	while ( *sp != '\0') {
		
		if (strncmp(sp, dlim, dlen) == 0)
		{
			flag = true;
			break;
		}
		sp++;
		tlen++;
	}

	if (!flag)
	{
		if (fsp)
			tok = fsp;
		else
			tok = NULL;
		nxtptr = NULL;
		return tok;
	}

	/* tok = (char *)malloc(sizeof(char)*(tlen+1)); */

	tok = fsp;
	/* strncpy(tok, fsp, tlen); */
	tok[tlen] = '\0';
	nxtptr = sp + dlen;

	return tok;

}

void test_mstrtok() {

	//char *str = "Live in the present moment"; // This causes mstrtok() to crash, never use const string like this
	char str[30] = "Live in the present moment";
	char *tok;

	printf("\ntest_mstrtok:\n");
	tok = mstrtok(str, " ");
	while (tok != NULL) {


		printf("%s\n", tok);
		//free(tok);
		tok = mstrtok(NULL, " ");
	}

}

void test_strtok() {

	char str[30] = "Live in the present moment";
	//char *str = "Live in the present moment"; // This causes strtok() to crash
	char *tok;

	printf("\ntest_strtok:\n");
	tok = strtok(str, " ");
	while (tok != NULL) {


		printf("%s\n", tok);
		tok = strtok(NULL, " ");
	}

}

void test_cpy() {

	char str[30] = "Live in the present \0 moment";
	char str2[30];
	char str3[30];
	char str4[30];
	char str5[30];

	mstrcpy(str2, str);
	printf("\ntest_cpy():str2 after mstrcpy:%s", str2);
	mmemcpy(str3, str, 30);
	printf("\ntest_cpy():str3 after mmemcpy:%s", str3);
	mmemmov(str3+5, str3, 10);
	printf("\ntest_cpy():str3 after mmemmov:%s", str3);
	mstrncpy(str4, str, 25);
	printf("\ntest_cpy():str4 after mstrncpy:%s", str4);
	strncpy(str5, str, 25);
	printf("\ntest_cpy():str5 after strncpy:%s", str5);
	printf("\n");

}

void main() {

int a, b;

	a = 5;b=18;

	printf("\nSum = %d\n", a+b);

	binary_shift();
	arr_point();
	div_test();

	printf("\n Result = %s \n", mstrstr("helloworld", "noworld"));
	printf("\n Result = %s \n", mstrstr("helloworld", "world"));

	test_strtok();
	test_mstrtok();
	test_cpy();
}
