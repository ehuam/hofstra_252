#include<stdio.h>
#include<stdlib.h>

/*
(define f (lambda (x) (let ((y 0)) (begin (set! y (+ y x)) y))))

int f(int x) { int y =0; y=y+x; return y; }

(define f (let ((y 0)) (lambda (x) (begin (set! y (+ y x)) y))))


(define (makeacc)
 (let ((y 0)) 
   (lambda (x) 
     (begin (set! y (+ y x)) y))))
*/

void* makeacc()
{
  static int y = 0; // what's wrong with making it static?
  int f(int x) { y+= x; return y; }
  return &f; 
}

int main()
{
  int (*a)(int);
  int (*b)(int);
  a = (int (*)(int))makeacc();
  b = (int (*)(int))makeacc();
  printf("a %d\n",a(2));
  printf("a %d\n",a(2));
  printf("b %d\n",b(2));
  return 0;
}
