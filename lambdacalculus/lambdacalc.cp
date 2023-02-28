// Lambda Terms in C++ (since 2014)
// may have to compile with g++ using -std=c++14 option

#include<iostream>
using namespace std;

int main()
{
  auto I = [](auto x) {return x;};
  auto K = [](auto x){ return [x](auto y) { return x; }; };
  auto S = [](auto x){return [x](auto y){return [x,y](auto z){return x(z)(y(z));};};};

  auto Ki = [](int x){ return [x](int y) { return x; }; };
  //  auto K = [](int x){ return [x](int y) mutable { x++; return x; }; };  
  // [x] means "capture x" by value  [&x] would capture by reference


  cout << Ki(1)(2) << endl;
  auto K1 = Ki(1);
  auto K3 = Ki(3);
  cout << K1(2) << endl; // should print 1
  cout << K3(2) << endl; // should print 3

  auto SKI = S(K)(I);  // SKI reduces to I
  cout << SKI(4) << endl;

  // this term is not a part of *typed* lambda calculus:

  auto XX = [](auto x){return x(x); }; // this unfortunately compiled
  // under C++, even though it is not typable.  The people who added lambda
  // terms to C++ probably know this, but disallowing it may conflict with
  // other features of C++ that were added earlier.  Anybody can add features
  // to C++, not just people who know lambda calculus and type theory. 

  // We can only conclude that lambda terms that use "auto" types
  // represent untyped lambda terms.  That's not what auto should do.

  return 0;
}//main

/* It's easier to implement closures in C++ than in C because C++ already
  have a native notion of classes and constructor/destructor.

  The lambda term Ki (K for integers) in main,
  
    auto Ki = [](int x){ return [x](int y) { return x; }; };

  is compiled into something roughly the equivalent of

  class Kc
  {
     private:
        int xcopy; // internal copy of captured x as an instance var
     public: 
        Kc(int x) { xcopy = x; }
        int inner(int y) { return xcopy; }
  };

  The call to K1=K(1) creates an instance of the Kc class by passing
  1 to its constructor.  The call to K1(2) corresponds to a call
  to K1.inner(2);

  Essentially, a closure (a lambda term with free variables) is 
  equivalent to an instance of a class with instance variables.
   
  This point is worth repeating: CLOSURES ARE OBJECTS.

  Furthermore, in this program the object is directly allocated on
  the stack frame for main, and will be destroyed (by calling the
  default destructor of the class) when main exits.  

  However, if the captured value is itself a pointer (a free
  variable of type int*, for example), then these lambda terms
  can create additional problems for memory management (greater
  possibility of memory errors).
*/
