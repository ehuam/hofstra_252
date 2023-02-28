module Main exposing (..)
import List 
import Html exposing(text, div)
-----------------------------------  

-- Differences between Elm and Python(ish) languages
-- 1. Elm is strongly typed but uses type inference
-- 2. Elm is very much pure lambda calculus

-- function definition.  the following are equivalent:

f1 = \x->\y->x+y   -- \x-> is lambda x.
f2 x y = x+y    -- f1,f2,f3 are all Curried Functions
f3 x = \y->x+y
--however , use the f2 form when defining recursive function.

-- but none of the functions above are equivalent to:

g1(x,y) = x+y  -- Uncurried function

-- Currying:  f1,f2,f3 are "Curried" functions after the logician
-- Haskell Curry.  All lambda abstractions are over one argument.  But
-- being a realistic programming language, Elm has built-in pairs, with
-- syntax (a,b)

-- converting: uncurried to curried and back

curry g x y = g(x,y)
uncurry f (x,y) = f x y

-- no mutable vars and no loops!

-- worst example of recursion:
fib1 n = if n<3 then 1 else fib1(n-1)+fib1(n-2)

-- second worst example of recursion:
fact1 n = if n<2 then 1 else n*fact1(n-1)

-- The following will produce a rare runtime error in elm
--factmil = fact1 1000000  -- stack overflow

---- tail recursive version of factorial function:

fact2 n ax = if n<2 then ax else fact2 (n-1) (n*ax)

-- must call fact2 with 1 as initial value of ax

-- tail-recursive Fibonacci
----- encapsulate one procedure inside another using let
fib m =
  let fib2 n a b = if n<2 then b else fib2 (n-1) b (a+b)
  in
  fib2 m 0 1
-- see main for call/output

-- working with linked lists (without Maybe monad):
forced_car defaultval m =
   case m of
     [] -> defaultval
     (a::_) -> a


----- extracting a value from a "Maybe monad" requires a defaultvalue:

extract monad default_value =
  case monad of
    Just x -> x
    Nothing -> default_value

---- versions of car/cdr for numerical lists with NaN as default value:
car m = extract (List.head m) (0/0)
cdr m = extract (List.tail m) []
isnil m = 0==List.length m

{- alternative versions
cdr m =
   case m of   -- this is called "pattern matching"
     [] -> []
     (a::b) -> b

-- need to define a new 'car' for each type of defaultval:
car m = forced_car (0/0) m
-}
primes = [2,3,5,7,11]

-- sum of a list of numbers, tail-recursive:
sumlist m =
  let sum mm ax = if (isnil mm) then ax else sum (cdr mm) (ax+(car mm))
  in
  sum m 0

-- example of higher-order functions:
forall predicate m =
  (isnil m) || ((predicate (car m)) && (forall predicate (cdr m)))

exists predicate m =
  not(forall (\x->not(predicate x)) m)

reduce func left_identity mm = -- assumes func is left-associative
  let red m ax = if (isnil m) then ax else red (cdr m) (func ax (car m))
  in
  red mm left_identity

-- product of all numbers in a list (contrast with sumlist):
productlist m = reduce (\x->\y->x*y) 1 m


-- right-associative fold, but not tail-recursive:
fold func right_id m =
  if (isnil m) then right_id else (func (car m) (fold func right_id (cdr m)))

{- UNFORTUNATELY Elm choked on this: -}
-- challenge: define right-associative fold tail-recursively
-- by building a continuation function
foldright func id mm =
  -- accumulator ax is itself a function
  let mcf m ax = if (isnil m) then ax else mcf (cdr m) (\x->ax (func (car m) x))
  in
  let continuation = mcf mm (\x->x) in (continuation id)

---------------- FOR BUILDING OUTPUT WEB PAGE BY MAIN -----------------
str n = String.fromInt n
tostr n = Debug.toString(n)
build_output inl outl = -- this is a "tail-recursive" function
  case inl of
    [] -> div [] (List.reverse outl)
    a::b -> build_output b (div [] [text a]::outl)
--output = build_output ["hello","there!"] []

main = 
 build_output
  [
     "OUTPUT OF BASIC ELM PROGRAM:",
     "(f2 3 4): " ++ str(f2 3 4),
     "f2 5: " ++ tostr(f2 5),
     "(curry g1 8 2):  " ++ tostr(curry g1 8 2),
     "(uncurry f1 (3,5):  " ++ tostr(uncurry f1 (3,5)),
     "0/0 is not technically a runtime error, it's a " ++ tostr(0/0),
     "tail recursive (fact2 5 1): " ++ str(fact2 5 1),
     "tail recursive Fibonacci with inner function (fib 10): "++str(fib 10),
     ".",
     "(car primes): " ++ tostr(car primes),
     "(car (cdr (cdr primes)): " ++ tostr(car (cdr (cdr primes))),
     "(cdr []): " ++ tostr(cdr []),
     "(car []): " ++ tostr(car []),
     "sumlist primes:  " ++ tostr(sumlist primes),
     "all primes are positive: "++tostr(forall (\x->x>0) primes),
     "there exists an even prime: "++
        tostr(exists (\x->0==modBy 2 (truncate x)) primes),
     "productlist primes:  " ++ tostr(productlist primes),
     "largest value in list: "++
        tostr(reduce (\x->\y->if x>y then x else y) (-1/0) [4,5,1,8,7]),
     "left-reduce (-) 10 [5,3,2]: "++tostr(reduce (-) 10 [5,3,2]),
     ".",
     "rare runtime error: (fact1 5):  ++ str(fact1 1000000)",
     "--------"
  ] []
