/* CSC 123/252 Assignment: Scheme/Elm Style Functional Programming in GCC 

  In this assignment, unless otherwise noted for a specific program, 
  your MAY NOT USE ANY OF THE FOLLOWING:

  1. while-loops, for-loops, do-while loops, goto, or any other kind of loop:
   YOU CAN ONLY USE TAIL RECURSION.

  2. The assignment statement that changes the value of a declared variable:
  
     int x = 1; // this is fine and corresponds to (define x 1), (let ((x 1))..
     x = x+2;   // THIS IS NOT ALLOWED  (neither is x++, ++x, etc.)

  3. Pointer notation including *, & and -> (as well as pointer arithmetic), except
      when otherwise indicated.  (You can, however, call cons, car, cdr defined below).

  The idea is to stay as close as possible to pure lambda calculus in C.
  However, for this assignment you may use gcc (Gnu C) instead of ANSI C 
  (clang).  Gnu C enables a few more features, including the ability to define
  a function inside a function: the inner function is not visible outside
  the function in which it's defined.

  The only feature that you can use which is not admissible in lambda calculus
  is printf.  Furthermore, printf should only be done in main to test your
  program.

  You can declare a variable and give it a value, but you may not CHANGE it.
  The assignment statement is a MEMORY OPERATION and is not definable in pure
  lambda calculus.  All programs that run on real computers, however, must
  read from- and write to memory.  But at the programming language level,
  such operations can be hidden.  Think about where in a program you will
  typically see "x=x+1": probably inside a while loop.  But while loops can
  be replaced by tail-recursive calls.  The linked list datatype has already
  been defined, but you can't access its internal representation. You can 
  only use the high-level functions cons, car and cdr.

  You may not use any destructible (mutable) structure for this assignment.

  The purpose of this assignment is to understand the most important 
  differences between a highly abstract language and a relatively "low level"
  language.  You will see that (gnu) C is still capapble of creating powerful
  abstractions, much more so than you might think.  Ultimately, however,
  there will be fundamental limitations to C.

  There are some differences between C and Scheme/Elm that we have to
  "manage", in particular the fact that C is compiled and is
  statically typed, although it does not have type inference: you have
  to declare the type of variables.  BY DEFAULT, WE WILL ASSUME THAT
  THE TYPE OF VALUES IS int (sometimes unsigned int).  However, we
  will distinguish between ints and lists/vectors of ints, as well as
  functions that operate on ints (functions are also values that have
  a type in C).

  Be sure to use gcc (Gnu C).  Beaware that gcc on Macs may not be Gnu C.
  If you have a Mac and can't run the virtual machine, use Hofstra's Linux
  workstations.
  (you can ssh into them on port 5010, e.g ssh -p 5010 147.4.180.110-120).
*/

#include<stdio.h>
#include<stdlib.h>

#define NIL 0
#define BOOL int

typedef unsigned int uint;  // convenient name for unsigned int

// The following defines the TYPES OF FUNCTIONS
typedef int (*fun)(int);  /* fun is the type of functions int->int */
typedef int (*binop)(int,int); /* binary operation on ints */
typedef BOOL (*predicate)(int); /* predicates on integers */
typedef void (*action)(); /* simple action, no return value */
typedef void (*actionon)(int); /* act on an integer */

/* refer to programs written in class for more code and examples */

// Basic linked-list encapsulation
typedef struct cell* list;  // linked list pointer
struct cell
{
  int item;
  list next;
};

list cons(int x, list n) // creates struct on heap
{
  list newcell = (list)malloc(sizeof(struct cell));
  newcell->item = x;
  (*newcell).next = n;
  return newcell;
}
// list m = cons(2,cons(3,cons(5,cons(7,NIL))));
int car(list m) { return m->item; }
list cdr(list m) { return m->next; }
int cadr(list m) { return m->next->item; } // second item
list cddr(list m) { return m->next->next; }

//// IN YOUR CODE THESE ARE THE ONLY OPERATIONS YOU CAN CALL ON LISTS:
// cons, car, cdr and functions defined from them: you may NOT access
// the struct cell encapsulated by these functions (and you won't have to).
// You may not call malloc/calloc and you can't use any pointer notation.

// sample functions on lists (similar to lecture examples).
list reverse(list m)
{
  // need a stack, initially null
  list irev(list m, list stack)
  {
    if (m==NIL) return stack; else return irev(cdr(m), cons(car(m),stack));
  }
  return irev(m,NIL);
}

list map(fun f, list m)
{
  list imap(list m, list stack)
  {
    if (m==NIL) return stack;
    else return imap(cdr(m), cons(f(car(m)),stack));
  }
  return reverse(imap(m,NIL));
}

/* in scheme
(define (reduce bop id m)
  (define (inner m ax) (if (null? m) ax (inner (cdr m) (bop ax (car m)))))
  (if (null? m) id (inner (cdr m) (car m))))
(reduce (lambda (x y) (+ x y)) 0 '(3 4 5 1))
*/
// the id is the "left-identity"
int reduce(binop bop, int id, list m)
{
  int inner(list m, int ax) {
    if (m==NIL) return ax; else return inner(cdr(m),bop(ax,car(m)));
  }
  if (m==NULL) return id; else return inner(cdr(m),car(m));
}

/* there-exists:
(define (exists p m)   
  (if (null? m) #f 
     (if (p (car m)) #t (exists p (cdr m)))))
*/
BOOL exists(predicate p, list m)
{
  return m!=NIL && (p(car(m)) || exists(p,cdr(m)));
}// is this tail recursive? TRICK QUESTION

BOOL forall(predicate p, list m)
{
  BOOL notp(int x) { return !p(x); }
  return !exists(notp,m);  //forall x.P(x) == !exists x.!P(x)
}



/*        ***************  ASSIGNMENT PROBLEMS:  *****************

For each problem, write out the program in C and demonstrate how it's called
in main.  I will give you some code in scheme.  Linked lists in Elm 
works a bit differently because of how error handling is done.

1. write the equivalent of the following scheme function, which "doubles up"
each value of a list.

(define (doubleup m)
   (if (null? m) m (cons (car m) (cons (car m) (doubleup (cdr m))))))

for example, (doubleup '(1 2 3)) returns '(1 1 2 2 3 3)).  This function
is recursive but not tail-recursive.  ALL FUNCTIONS AFTER #1 must be tail-recursive.

RECREATE THIS FUNCTION IN C, RESPECTING THE STRUCTURE OF THIS PROGRAM.

1b. Write a tail-recursive version of the doubleup function: wrap the tail
   recursive function inside an outer function.  You may also want to copy
   over the reverse function from above.

2. define a new type:
   typedef void (*actionon)(int);

   Then define a "foreach" loop construct that works as follows:

   void output(int x) { printf("%d ",x); }
   foreach(m,output); // would print every value in m

   It is possible to use map for printing, but that would be a bit
   wasteful because, unlike map, foreach doesn't need to return anything.  In
   Scheme you can define it tail-recursively with

   (define (foreach m f)   ; arguments are list m and function f
      (if (not (null? m)) (begin (f (car m)) (foreach (cdr m) f))))

   The signature (header) of your foreach function should be

   void foreach(list m, actionon f);
   or
   void foreach(list m, void (*f)(int));  // without typedef

3. Write a function to print every value in a list using the foreach loop
   you created above for problem 2.  If you're not sure of the above, write a
   tail recursive function from scratch first.

4. write a higher-order function 'howmany' that takes a predicate
   as an argument and returns how many values in a list satisfy the
   given property.  In scheme, the function should behave as follows:

   (howmany (lambda (x) (< x 0)) (cons 2 (cons -3 (cons 4 (cons -1 ())))))
   should return 2 because there are two negative numbers in the list.

   Here's how you'd write the function non-tail recursively in scheme:
   (define (homany p m) 
      (if (null? m) 0 (+ (if (p (car m)) 1 0) (homany p (cdr m)))))
  ;(if (p (car m)) 1 0) evaluates to 1 or 0, which is added to (howmany..)
  
   YOUR SOLUTION IN C MUST BE TAIL RECURSIVE.

   hint: for the (inner) tail-recursive function, the counter should be 
   passed in as an extra argument.

   challenge: write the function in both C and Scheme

5. write a higher order function 'filter' that takes a predicate p and list
   m as an arguments and returns a list with just those values in m that
   satisfies the predicate.  For example, (in scheme syntax)

   (filter (lambda (x) (< x 0)) (cons 2 (cons -3 (cons 4 (cons -1 ())))))

   should return a list '(-3 -1), which are the values in the original list
   that are negative.  You can also return '(-1 -3) (cons(-1,cons(-3,NIL))):
   the order in the filtered list is not important.
 
6. write a function that takes two lists as arguments and returns their
   intersection: a list that contains all values found in both lists
   (ordering and duplicates do not matter).  For example, given

   list m = cons(2,cons(4,cons(6,NIL)));
   list n = cons(2,cons(5,cons(6,cons(8,NIL))));
   list mn = intersection(m,n); // should return list cons(2,cons(6,NIL))  

   hint: for the tail-recursive function the intersection to be built (mn)
   should be an extra argument.

7. write a function 'sublist' so that sublist(m,n) returns true if every
   value in m is also found in n (ordering and duplicates don't matter),
   and returns false othewise.  

7b (challenge). Write the sublist function using only the forall and exists
   functions.  That is, do not write your own recursive function: use only calls 
   to forall and/or exists.

hint: M is a sublist of N if forall x in M there exists a y in N such that x==y


8. The following function applies a 'binop' to the values of a list:
int left_reduce(list m, binop op) // assumes m is not empty
{
  int iter(list m, int ax)
  {
    if (m==NIL) return ax; else return iter(cdr(m),op(ax,car(m)));
  }
  return iter(cdr(m),car(m)); // will crash if there's no car
}
For example, given
int subtract(int x, int y) { return x-y; }
list m = cons(5,cons(3,cons(2,NIL)));
left_reduce(m,subtract) will return (5-3)-2 = 0.

This function will always assume that binop is left-associative and only works for
non-empty lists.

Write a tail-recursive version of this function that assumes that binop is right-associative.  
Hint: here's one that works, but is not tail-recursive
int fold(list m, binop op)
{
   if (cdr(m)==NIL) return car(m); else return op(car(m),fold(cdr(m),op));
}
fold(m,subtract) will return 5-(3-2) = 4.

Submit all problems with main in one file.
*/

int main()
{
  list m = cons(2,cons(3,cons(5,cons(7,cons(11,NIL)))));
  // ... demonstrate all your functions ...

list doubleup(list m)
  {
    if (m == NIL) { return m;}
    list newlist = cons ( car(m), cons ( car (m), 
                          doubleup(cdr(m))  ));
  }
//printf("%d %d", car(doubleup(m)), car(cdr(doubleup(m))));

list doubleupT(list m){

  list doubleup(list m, list stack){
    if( m == NIL) return stack;
    else{
      return doubleup(cons(car(m), cons(car(m), cdr(m))), NIL );
    }
  }
  return doubleup(m,NIL);
}
printf("%d %d", car(doubleupT(m)), car(cdr(doubleupT(m))));
/*
//2 
typedef void (*actionon)(int);

void output(int x) 
{
  printf("%d ", x);
}

void foreach(list m, actionon f){
  if( m == NIL) return output(NIL);
  else{
    f(car(m));
    return foreach(cdr(m), f);
  }
  
}


//3
void printE(list m){
  return foreach(m, output);
}

//4
int howmany(predicate p, list m){
  return foreach(m, predicate p);

}

//5
list filter (predicate p, list m){
  if(m == NIL) return cons(NIL, list);
  list filtered = return foreach(m, filter);
  return filtered;
}

//6 list intersection(list m, list n)

//7
list sublist(list m, list n){
// return true if every value in m is also found in n
// false otherwise

return forall(car(m) == car(n), )

}
*/


  return 0;
}//main
