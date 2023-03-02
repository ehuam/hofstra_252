# Why did Python switch from dynamic to static scoping?  Here's a
# simple example that might explain it.

from math import pi,e

def f():
   print "the base of natural log is", e

def circumference(radius): 
    return pi*2*radius        # which pi is this? some frat?

def static():
   P = [2,4,6,8,10]
   pi = 0 # statically scoped local variable, indexes array P
   while pi<len(P):
       print "circumference with radius",P[pi],"is",circumference(P[pi])
       pi += 1
# works just fine ... why?


# procedure that simulates the effect of dynamic scoping under static scoping
def dynamic():
   global e,pi
   savede,savedpi = e,pi
   e = 3000000000000000
   pi = input("what would you like pi to be? ")
   f()
   print "circumference of circle of radius 10 is", circumference(10)
   e,pi = savede,savedpi  # restore original values
# myprocedure

f()
static()
dynamic()

