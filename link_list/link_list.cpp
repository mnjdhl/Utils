#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>

using namespace std;

typedef struct snode {
	int val;
	struct snode *next;
} node;

node *list_insert(int v, node *l) {

	node *tmp, *tmp2;

	tmp = (node *)malloc(sizeof(node));
	if (tmp == NULL)
		return l;

	tmp->val = v;
	tmp->next = NULL;

	if (l == NULL)
		l = tmp;
	else {
		tmp2 = l;
		while (tmp2 != NULL) {
			if (tmp2->next == NULL)
				break;

			tmp2 = tmp2->next;
		}
		tmp2->next = tmp;
	}

	return l;
}

node *list_del(int v, node *l) {

	node *cur, *prv;

	prv = NULL;
	cur = l;
	while (cur != NULL) {
		if (cur->val == v)
			break;
		prv = cur;
		cur = cur->next;
	}

	if (prv == NULL)
		l = cur->next;
	else {
		prv->next = cur->next;
	}

	if (cur != NULL)
		free(cur);

	return l;
}

node *list_reverse(node *l) {

	node *cur, *prv, *nxt;

	cur = l;
	prv = NULL;
	while (cur != NULL) {
		nxt = cur->next;
		cur->next = prv;
		if (nxt == NULL)
			break;
		prv = cur;
		cur = nxt;
	}

	return cur;
}

/* 1st element becomes last and 2nd one becomes first */
node *list_rotate_left(node *l) {

	node *cur, *prv, *last;

	cur = l;
	if (cur == NULL || cur->next == NULL)
		return l;

	last = l;
	l = cur->next;
	prv = cur;
	cur = l->next;
	while (cur != NULL) {
		prv = cur;
		cur = cur->next;
	}

	prv->next = last;
	last->next = NULL;

	return l;
}

/* Last one becomes 1st element and 1st becomes 2nd element  like that*/
node *list_rotate_right(node *l) {

	node *cur, *prv, *last;

	cur = l;
	if (cur == NULL || cur->next == NULL)
		return l;

	prv = NULL;
	while (cur != NULL) {
		if (cur->next == NULL)
			break;
		prv = cur;
		cur = cur->next;
	}

	last = cur;
	prv->next = NULL;
	last->next = l;
	l = last;

	return l;
}
/* With double loop iteration and comparison */
node *detect_loop(node *l) {
	
	node *cur;
	node *dup = l;

	while (dup != NULL) {
		cur = dup->next;
		while (cur != NULL) {
			if (cur == dup)
				return cur;		

			cur = cur->next;
		}
		dup = dup->next;
	}


	return NULL;
}

/*Using map */
node *detect_loop2(node *l) {
	
	std::map<long, int> lmap;
	std::map<long, int>::iterator it;
	node *cur = l;


	while (cur != NULL) {
		//it = lmap.find((long)cur);
		//if (it != lmap.end())
		if (lmap.count((long)cur) > 0)
			return cur;		
		lmap[(long)cur] = 1;
		cur = cur->next;
	}

	return NULL;
}

void list_show(node *l) {

	node *tmp = l;

	printf("\nThe list is:");
	while (tmp != NULL) {
		printf("%d ", tmp->val);
		tmp = tmp->next;
	}

	printf("\n");

}

void test_loop() {

	node *list = NULL;
	node *p, *pp;
	int i;
	
	for (i=0;i<10;i++)
	{
		list = list_insert(i, list);
	}

	if (detect_loop2(list))
		printf("\n(1) Loop detected...");
	else
		printf("\n(1) Loop not detected...");


	//Now creating loop
	p = list;
	pp = NULL;
	i = 0;
	while (p) {
		pp = p;
		p = p->next;
		i++;

		if (i == 5) {
			pp->next = list;
			break;
		}
	}

	if (detect_loop2(list))
		printf("\n(2) Loop detected...");
	else
		printf("\n(2) Loop not detected...");

	printf("\n");
}

int main() {

	node *list = NULL;
	
	for (int i=0;i<100;i++)
		list = list_insert(i, list);

	list_show(list);
	list = list_del(0, list);
	list = list_del(55, list);
	list = list_del(99, list);
	cout <<endl<< "After deleting 0, 55 and 99:";
	list_show(list);
	list = list_rotate_left(list);
	cout <<endl<< "After left rotating ....:"; 
	list_show(list);
	list = list_reverse(list);
	cout <<endl<< "After reversing ....:";
	list_show(list);
	list = list_rotate_right(list);
	cout <<endl<< "After right rotating ....:";
	list_show(list);

	test_loop();

	return 0;
}
