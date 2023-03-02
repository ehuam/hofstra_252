// simulating dynamic scoping in C:

// C is statically scoped but it's easy to simulate most of dynamic scoping
// once you understand what dynamic scoping is doing.  Dynamic scoping
// just assigns a temporary value to a global variable.

#include<stdio.h>

void f()  // some function that writes to stdout
{
  printf("I'll show you the password now but don't save it to a file...");
  printf("The password is xyzzy : memorize it and close this window now!");
}


void g() // simulates dynamic scoping of stdout
{
  // start of simulated dynamic binding scope
  FILE *savedstdout = stdout;  // stack stdout
  stdout = fopen("passwordinfo.txt","w");  // open file for writing

  // body of dynamic scope
  f();  // call function, stdout now changed
  fclose(stdout);

  // end of simulated dynamic scope
  stdout = savedstdout;
}

int main()
{
  g();
  return 0;
}

/* in contrast, if C was natively dynamically scoped, we'd just have to
write:

void g()
{
   FILE *stdout = fopen("passwordinfo.txt","w");  // stdout is re-declared
   f();
}

and indeed in Perl:

   local *STDOUT;

would suffice, because 'local' uses dynamic scoping, as oppossed to 'my',
which uses static scoping in Perl.

With dynamic scoping, when f() refers to stdout it will look it up in
its runtime environment, aka on its RUNTIME STACK.  But C is
statically scoped, so f() will still lookup stdout in the environment
in which it was defined.  We can simulate the effect of dynamic
scoping by temporarily changing the variable, and later restoring it to
its original value. Our simulation technique only works because the
variable stdout is visible inside g().  If f referred to a non-local
(but not global) variable that's not visible inside g(), then g() will
of course not be able to change its value.  Thus our simulation
technique is only guaranteed to work for global variables, but that's
exactly when you would need to use dynamic scoping - to temporarily
change the value of variables that are already global in scope.
*/




