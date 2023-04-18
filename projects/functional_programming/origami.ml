(*
Zadanie Origami
Autor: Maja Tkaczyk
Recenzent: Adam Greloch
*)
type point = float * float
             
type kartka = point -> int 
  
(*stała niedokładności*)
let eps = 1e-9
  
let prostokat ((x1, y1) : point ) ((x2, y2): point) = 
  fun ((a, b) : point) ->
    if (a >= x1 -. eps) && (a -. eps <= x2 ) && (b >= y1 -. eps) && (b -. eps <= y2) then 1
    else 0;;

(*zwraca kwadrat odległości dwóch punktów*)
let odl ((x1, y1) : point ) ((x2, y2): point ) =
  (x1 -. x2) *. (x1 -. x2) +. (y1 -. y2) *. (y1 -. y2);;

let kolko p r =
  fun pkt ->
    if sqrt (odl p pkt) -.eps <= r then 1
    else 0;;

(*sprawdza polozenie punktu (x3, y3) względem prostej przechodzącej przez punkty (x1, y1) i (x2, y2)
-1 - po lewej, 0 - na prostej, 1 - po prawej
*)
let polozenie ((x1, y1) : point) ((x2, y2) : point) ((x3, y3) : point) =
  (*iloczyn wektorowy*)
  let wektorowy = (x3 -. x2) *. (y2 -. y1) -. (y3 -. y2) *. (x2 -. x1) in
  if wektorowy > -.eps && wektorowy < eps then 0
  else if wektorowy > 0.0 then 1
  else  -1 

(*zwraca punkt, będący odbiciem symetrycznym punktu (x, y) względem prostej przechodzącej przez punkty (x1, y1) i (x2, y2)*)
let odbicie ((x1, y1) : point) ((x2, y2) : point) ((x, y) : point) = 
  if x1 = x2 then (2.0 *. x1 -. x, y)
  else if y1 = y2 then (x, 2.0 *. y1 -. y) 
  else
    (*obliczam współczynniki w równaniach:
    y = ax + b - prosta przechodząca przez punkty (x1, y1) i (x2, y2)
    y = cx + d - prosta prostopadła do wyżej wymienionej i przechodząca przez (x, y)*)
    let a = (y1 -. y2) /. (x1 -. x2) in 
    let b = y2 -. a *. x2 in 
    let c = (x2 -. x1) /. (y1 -. y2) in
    let d =  y -. c *. x in
		(*	współrzędne (p, q) punktu przecięcia tych prostych  *)
    let p = (d -. b) /. (a -. c) in
    let q = p *. a +. b in 
    (2. *. p -. x, 2. *. q -. y);; 
     

let zloz a b k =
  fun p ->
    let pol = polozenie a b p in
    if pol = 0 then k p 
    else if pol = -1 then k p + k (odbicie a b p) 
    else 0;;

let skladaj l k = 
  let f k (a, b) = 
    zloz a b k in
  List.fold_left f k l;;
    
  