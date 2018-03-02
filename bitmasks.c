/*
*Brandon Corbett
*/

#include <stdio.h>
#include <limits.h>

void bit_print(int);
int pack(char, char, char, char);
char unpack(int, int);
unsigned getbits(unsigned, int, int);
unsigned setbits(unsigned x, int p, int n, unsigned y);
unsigned invertbits(unsigned x, int p, int n);

void bit_print(int a){
	int i;
	int n = sizeof(int) * CHAR_BIT;
	int mask = 1 << (n-1); // mask = 100...0
	
	for (i=1; i<=n; i++){
		putchar(((a & mask) == 0)? '0' : '1');
		a <<= 1;
		if (i % CHAR_BIT == 0 && i < n)
			putchar(' ');
	}
	putchar('\n');
}

int pack(char a, char b, char c, char d){
	int p=a;
	p = (p << CHAR_BIT) | b;
	p = (p << CHAR_BIT) | c;
	p = (p << CHAR_BIT) | d;
	return p;
}

char unpack(int p, int k){    // k=0, 1, 2, or 3
	int n = k * CHAR_BIT; // n = 0, 8, 16, 24
	unsigned mask = 255;  // mask = low-order byte
	
	mask <<= n;
	return ((p & mask) >> n);	
}

// getbits() extracts n bits from position p(start counting from the right-most bit) in x
unsigned getbits(unsigned x, int p, int n){
	unsigned temp = x >> (p+1-n);
	
	unsigned mask = 0;
	mask = ~mask;
	mask = mask << n;
	mask = ~mask;

	return temp & mask;
	// return (x >> (p+1-n)) & ~(~0<<n);
}

unsigned setbits(unsigned x, int p, int n, unsigned y){
	//initialize local variables
	unsigned temp;
	unsigned mask = 0;
	
	//bit manipulation goes here
	
	/*
	in test case, x=2004384122
	01110111 01111000 01111001 01111010
	
	in test case, y=1634952294
	01100001 01110011 01100100 01100110
	
	p=20, n=4
	currently, this is (20+1-4) = 17
	00000000 00000000 00000000 01100001
	*/
	temp = (p+1-n);
	
	/*
	mask = 0
	00000000 00000000 00000000 00000000

	first, let's flip the bits by using the bitwise compliment ~
	this will make mask = 
	11111111 11111111 11111111 11111111
	*/
	mask = ~mask;
	
	/*
	now lets shift the bits over n times
	in example, n=4 so this will push 4 ones off the left end and fill right end with zeros
	11111111 11111111 11111111 11110000
	*/
	mask = mask << n;

	/*
	now we will compliment the bits again to get 1's at the right end
	00000000 00000000 00000000 00001111
	*/
	mask = ~mask;

	/*
	now we can push the bits over temp times (p+1-n), or 17 times in example, to get them in place
	FYI: starts at rightmost bit and starts pushing, everything in front moves too
	00000000 00011110 00000000 00000000
	*/
	mask = mask << temp;
	
	/*
	since we are trying to manipulate bits in x, it currently lines up like this
	x    -> 01110111 01111000 01111001 01111010
	mask -> 00000000 00011110 00000000 00000000
	
	so, the bits we are trying to manipulate are 1100 in x
	let's compliment mask again and then bitwise AND x and mask to make the 1100 all zeros and store in
	another temporary variable called "manip"
	*/
	mask = ~mask;
	unsigned manip = x & mask;
	
	/*
	x     -> 01110111 01111000 01111001 01111010 &
        mask  -> 11111111 11100001 11111111 11111111
	
	manip -> 01110111 01100000 01111001 01111010
	                     ^^^^
	*/

	/*
	now we need the rightmost n bits of y
	y = 01100001 01110011 01100100 01100110
	                                   ^^^^
	we need to set mask back to 0 first
	*/

	mask = 0;

	/*
	let's compliment mask and then shift over n
	*/

	mask = ~mask;
	mask = mask << n;

	/*
	mask is currently 11111111 11111111 11111111 11110000
	let's setup another variable called "endgame" and set to 0 to get our final manipulated unsigned int
	let's compliment mask again and then bitwise AND to get 0110 in rightmost bits

	endgame -> 00000000 00000000 00000000 00000000
	y       -> 01100001 01110011 01100100 01100110
	mask    -> 00000000 00000000 00000000 00001111
	*/
	unsigned endgame = 0;
	mask = ~mask;
	endgame = y & mask;
	
	/*
	endgame -> 00000000 00000000 00000000 00000110

	so endgame now contains the n bits we want at the rightmost n bits
	so now we need to move them into the position that we want to change on x
	*/
	
	endgame = endgame << temp;
	
	/*
	so now all we need to do is bitwise OR manip with endgame and we will effectively change
	n bits and x position p+1-n to n right bits of y
	manip   -> 01110111 01100000 01111001 01111010 | 
	endgame -> 00000000 00001100 00000000 00000000
	
	endgame -> 01110111 01101100 01111001 01111010
	*/
	
	endgame = manip | endgame;

	//return temp;
	//return mask;
	//return manip;
	return endgame;
	//return temp & mask;
}

unsigned invertbits(unsigned x, int p, int n){
	unsigned temp = p+1-n;
	unsigned mask = 0;

	/*
	x    -> 01110111 01111000 01111001 01111010
	mask -> 00000000 00000000 00000000 00000000
	
	we are taking the bit at position p in x through position n in x and flipping the bits
	so, in example x we are taking:
	x    -> 01110111 01111000 01111001 01111010
	                     ^^^^ ^^^^^

	x    -> 01110111 01110111 10000001 01111010

	first let's compliment mask and then shift left n times
	*/
	
	mask = ~mask;
	mask = mask << n;
	
	/*
	compliment the bits in mask again to get 1's 
	*/

	mask = ~mask;

	/*
	next, move the complimented bits over to the p+1-n position (temp)
	this will move the least significant bit over p+1-n times (ex: 19+1-9=11) 
	and the 1's in front will move to the p position
	*/

	mask = mask << temp;

	/*
	so now we create unsigned "manip" to store the manipulated number
	so in order to replace bits p thru n in x, we take the bitwise XOR of x and mask

	x     -> 01110111 01111000 01111001 01111010 ^	
	mask  -> 00000000 00001111 11111000 00000000

	manip -> 01110111 01110111 10000001 01111010
	*/
	
	unsigned manip = 0;
	manip = x ^ mask;
	
	//return mask;
	return manip;
	//return temp;
}

int main(){
	int x = 19;

	printf("The binary rep. of %d is:\n", x);
	bit_print(x);

	int p=pack('w', 'x', 'y', 'z');
	printf("\n'w', 'x', 'y', and 'z' packed together is equal to %d. Its binary rep. is:\n", p);
	bit_print(p);
	printf("calling unpack(p, 0) to extract the byte # 0 from the right:\n");
	bit_print(unpack(p, 0));
	printf("calling unpack(p, 1) to extract the byte # 1 from the right:\n");
	bit_print(unpack(p, 1));
	printf("calling unpack(p, 2) to extract the byte # 2 from the right:\n");
	bit_print(unpack(p, 2));
	printf("calling unpack(p, 3) to extract the byte # 3 from the right:\n");
	bit_print(unpack(p, 3));

	unsigned result = getbits(p, 20, 7);
	printf("\ncalling getbits(p, 20, 7) to extract 7 bits from bit # 20 returns %d:\n", result);
	bit_print(result);
	
	//setbits
	x = 2004384122;
	int y = 1634952294;
	p = 20;
	int n = 4;

	result = setbits(x, p, n, y);
	printf("\ncalling setbits(x, p, n, y) returns %d:\n", result);
	bit_print(result);

	//invertbits
	p = 19;
	n = 9;
	result = invertbits(x, p, n);
	printf("\ncalling invertbits(x, p, n) returns %d:\n", result);
        bit_print(result);	

	return 0;
}

