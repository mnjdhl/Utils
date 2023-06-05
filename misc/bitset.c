#include <stdio.h>


int bitset1(int bits, int pos) {

	int base = 1, res;

	base = base << (pos-1);

	res = bits | base;

	return res;
}

int bitset0(int bits, int pos) {

	int base = 1, res;

	base = base << (pos-1);
	base = ~base;

	res = bits & base;

	return res;
}


int main() {

	int nb = bitset0(15, 4);

	printf("\n Bitset 0 of 1111 i.e. 15 : %d", nb); 
	printf("\n Bitset 1 of 1101 i.e. %d : %d\n", nb, bitset1(nb, 4)); 

}
