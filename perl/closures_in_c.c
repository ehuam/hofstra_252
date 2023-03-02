#include<stdio.h>
#include<stdlib.h>

// Can we have closures and stateful functions in C?  To What extent?

int x = 0;

int f(int y) {x += y; return x; }
// clearly f returns a different value each time: but there's only one global x

void* newaccumulator()
{
  static int x = 0;
  int f(int y) {x += y; return x; }
  return &f;
}

int main()
{
  double (*accum1)(double); // declares accum1 to be of functional type
  double (*accum2)(double); // declares accum1 to be of functional type
  accum1 = (double (*)(double))newaccumulator();
  printf("%d\n", accum1(3));
  printf("%d\n", accum1(3));
  accum2 = (double (*)(double))newaccumulator();
  printf("%d\n", accum2(3));
  printf("%d\n", accum1(3));
  return 0;
}


// Note: this program is slightly different from the one in the video: the
// type cast of the void* returned by newaccumulator to type double(*)(double)
// (pointer to function that takes a double and returns double) actually
// got rid of the segmentation fault, but of course still did not produce
// the effects of a closure.
