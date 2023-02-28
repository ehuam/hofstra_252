-- elm init
-- elm reactor
-- localhost:8000  (127.0.0.1:8000)

module Main exposing (..)

-- Press buttons to increment and decrement a counter.
--
-- Read how it works:
--   https://guide.elm-lang.org/architecture/buttons.html
--

import Browser
import Html exposing (Html, button, div, text)
import Html.Events exposing (onClick)

-- MAIN
main =
  Browser.sandbox { init = init, update = update, view = view }

-- MODEL
type alias Model = List Int

--init : Model  -- I commented this out to show it's optional
init = [0]
  
-- UPDATE
type Msg
  = Increment
  | Decrement | Square | Power2 | Reset | Back


-- function to extract value from Maybe
getlist : Maybe Model -> Model
getlist m =
  case m of
     Just n -> n
     Nothing -> [0]
     
getint : Maybe Int -> Int
getint x =
  case x of
    Just n -> n
    Nothing -> 0

car m = getint (List.head m)
cdr m = getlist (List.tail m)

--update : Msg -> Model -> Model
update msg model =
  let current = car model in   --  getint (List.head model) in
  let tail = cdr model in      --  getlist (List.tail model) in
   case msg of
     Increment ->  (current+1)::current::tail
     Decrement ->  (current-1)::current::tail
     Square -> (current^2)::current::tail
     Power2 -> (2^current)::current::tail
     Reset -> [0]
     Back -> if List.length model>1 then tail else [0] --[0]


-- VIEW
--view : Model -> Html Msg
view model =
  div []
    [ button [ onClick Decrement ] [ text "-" ]
    , div [] [ text (String.fromInt (getint (List.head model))) ]
    , button [ onClick Increment ] [ text "+" ]
    , div [] []    
    , button [ onClick Square ] [ text "^2"]
    , button [ onClick Power2 ] [ text "2^"]
    , div [] []    
    , button [ onClick Back ] [ text "Back"]
    , button [ onClick Reset ] [ text "Reset"]    
    ]

