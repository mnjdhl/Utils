#include <stdio.h>
#include <stdlib.h>

typedef struct snode {
	int val;
    int len;
	struct snode *next;
} node;

typedef struct snode6 {
	long val1;
	long val2;
    int len;
	struct snode *next;
} node6;


void dec_test(int ver) {

    void *l;

    printf("\n Declaration testing .... ");
    if (ver == 6) {

        node6 *l1;
	    l1 = (node6 *)malloc(sizeof(node6));
	    if (l1 == NULL)
		    return;
        l1->val1 = 20;
        l1->val2 = 30;
        l1->len = sizeof(node6);
        l1->next = NULL;
        printf("\n Size = %d, len = %d\n", sizeof(l1), l1->len);
        l = (void *) l1;
        //free(l1);

    } else {

        node *l1;
	    l1 = (node *)malloc(sizeof(node));
	    if (l1 == NULL)
		    return;
        l1->val = 35;
        l1->len = sizeof(node);
        l1->next = NULL;
        printf("\n Size = %d, len = %d\n", sizeof(l1), l1->len);
        l = (void *) l1;
        //free(l1);

    }

     free(l);

}

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

	if (detect_loop(list))
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

	if (detect_loop(list))
		printf("\n(2) Loop detected...");
	else
		printf("\n(2) Loop not detected...");

	printf("\n");
}

void main() {

	node *list = NULL;
	
	for (int i=0;i<100;i++)
		list = list_insert(i, list);

	list_show(list);
	list = list_del(0, list);
	list = list_del(55, list);
	list = list_del(99, list);
	list_show(list);
	list = list_reverse(list);
	printf("\nAfter reversing the list....");
	list_show(list);

	test_loop();

    /* Declaration test... */
    dec_test(6);
}
