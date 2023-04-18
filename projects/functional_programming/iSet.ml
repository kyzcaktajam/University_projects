(*
Zadanie Modyfikacja Drzew
Autor: Maja Tkaczyk
Recenzent: Kamil Pilkiewicz
*)
type t = Empty | Node of t * (int * int) * t * int * int;;

let empty = Empty;;

let is_empty x =
  if x = empty then true
  else false;;

(*zwraca wysokość drzewa*)
let height x =
  match x with 
  | Node(_, _, _, _, h) -> h 
  | Empty -> 0;; 

(* zwraca min ((a+b), max_int)*)
let plus a b =
  if a + b < 0 then max_int
  else a+b;;
 
(*zwraca liczbę elemetów w poddrzewie*)
let ile_liczb x = 
  match x with
  | Node(_, (a, b), _, c, _) -> c
  | Empty -> 0;; 

(*zwraca (a-b+1), aby nie przekroczyło max_int*)
let roznica a b =
  if a>0 && b<0 then plus (plus a (-b)) 1
  else if a<0 && b>0 then plus (plus (-a) b) 1
  else plus (a-b) 1;;

let make l ((a, b) as k) r = Node (l, k, r, plus (plus (ile_liczb l) (ile_liczb r)) (roznica b a) , (max (height l) (height r)) + 1);;


let below x set =
  let rec helper set = 
    match set with
    | Empty -> 0
    | Node (l, (a, b) , r, _, _) -> 
        if a <= x && x<= b then
          plus (ile_liczb l) (roznica x a)
        else if x < a then helper l
        else plus (ile_liczb l) (plus (roznica b a) (helper r))
  in helper set;;


let bal l k r =
  let hl = height l in
  let hr = height r in
  if hl > hr + 2 then
    match l with
    | Node (ll, lk, lr, _, _) ->
        if height ll >= height lr then make ll lk (make lr k r)
        else
          (match lr with
           | Node (lrl, lrk, lrr, _, _) ->
               make (make ll lk lrl) lrk (make lrr k r)
           | Empty -> assert false)
    | Empty -> assert false
  else if hr > hl + 2 then
    match r with
    | Node (rl, rk, rr, _, _) ->
        if height rr >= height rl then make (make l k rl) rk rr
        else
          (match rl with
           | Node (rll, rlk, rlr, _, _) ->
               make (make l k rll) rlk (make rlr rk rr)
           | Empty -> assert false)
    | Empty -> assert false
  else make l k r


let mem x set =
  let rec loop set =
    match set with 
    | Empty -> false 
    | Node (l, (a, b), r, _, _) ->
        if a <= x && x <= b then
          true
        else if x < a then 
          loop l
        else
          loop r
  in loop set;;

let iter f s =
  let rec loop = function
    | Empty -> ()
    | Node (l, k, r, _, _) -> loop l; f k; loop r in
  loop s;;

let fold f set acc =
  let rec loop acc = function
    | Empty -> acc
    | Node (l, k, r, _, _) -> loop (f k (loop acc l)) r in
  loop acc set;;

(*zwraca true, jeżeli dwa przedziały są przecinające się lub sąsiednie*)
let czy_przeciecie (a, b) (c, d) =
  if (a <= c) then  
    if c <= (b+1) then true  
    else false
  else
  if a <= (d+1) then true
  else false ;;
  
(*zwraca najmniejszy możliwy początek przedziału w drzewie, który
  jest przecinający sie lub sąsiedni z przedziałem p
*)
let poczatek p set =
  let rec helper res s = 
    match s with 
    | Empty -> res 
    | Node(l, k, r, _, _) ->
        if czy_przeciecie p k then helper (min res (fst k) ) l
        else if (snd k) < (fst p) then helper res r
        else helper res l
  in helper (fst p) set;;

(*zwraca największy możliwy koniec przedziału w drzewie, który
  jest przecinający sie lub sąsiedni z przedziałem p
*)
let koniec p set =
  let rec helper res s = 
    match s with 
    | Empty -> res 
    | Node(l, k, r, _, _) ->
        if czy_przeciecie p k then helper (max res (snd k) ) r
        else if (snd k) < (fst p) then helper res r
        else helper res l
  in helper (snd p) set;;
  

(*zwraca sumę przedziałó w drzewie, które są przecinają się z przedziałem p*)
let suma_przecinajacych p set = 
  ((poczatek p set), (koniec p set));;


(*dodaje do drzewa przedział rozłączny z każdym z jego przedziałów*)
let rec dodaj_rozlaczny ((a, b) as p) set = 
  match set with 
  | Empty -> Node (empty, p, empty, (roznica b a), 0)
  | Node (l, k, r, _, _) ->
      if (fst k) > a then bal (dodaj_rozlaczny p l) k r 
      else bal l k (dodaj_rozlaczny p r);;


(*łączy dwa drzewa*)
let rec join s1 s2 =
  match (s1, s2) with
  | (s, Empty) -> s
  | (Empty, s) -> s
  | (Node (l1, k1, r1, h1, _), Node (l2, k2, r2, h2, _)) ->
      if h1 > h2 + 2 then
        bal l1 k1 (join r1 s2)
      else
        bal (join s1 l2) k2 r2;;

(*łączy dwa drzewa i przedział w jedno drzewo*)
let rec zlacz_w_drzewo l k r =
  match (l, r) with
  | (Empty, _) -> dodaj_rozlaczny k r
  | (_, Empty) -> dodaj_rozlaczny k l
  | ( Node (l1, k1, r1, _, h1), Node (l2, k2,r2, _, h2) ) -> 
      if h1 > h2 + 2 then bal l1 k1 (zlacz_w_drzewo r1 k r)
      else if h2 > h1 + 2 then bal (zlacz_w_drzewo l k l2) k2 r2 
      else make l k r
  
          
(* dzieli drzewo na dwa drzewa wzgledem wartosci x zawierającej się w przedziale w korzeniu*)
let podziel a b x l r =
  let lewe = if a = x then l else dodaj_rozlaczny (a, x - 1) l in
  let prawe = if b = x then r else dodaj_rozlaczny (x + 1, b) r in 
  (lewe, true, prawe);;


let split x set =
  let rec loop s =
    match s with 
    | Empty -> (Empty, false, Empty)
    | Node (l, ((a, b) as k), r, _, _) -> 
        if a <= x && x <= b then podziel a b x l r
        else if x < a then 
          let (lewe, czy, prawe) = loop l in (lewe, czy, zlacz_w_drzewo prawe k r)
        else 
          let (lewe, czy, prawe) = loop r in (zlacz_w_drzewo l k lewe, czy, prawe)
  in loop set;;

let remove (a, b) set =
  let (lewe, _, _) = split a set
  and (_, _, prawe) = split b set in
  join lewe prawe;;

let add x set =
  let suma = suma_przecinajacych x set in
  dodaj_rozlaczny suma (remove suma set);;

let elements s =
  let rec loop acc s =
    match s with
    | Node (l, k, r, _, _) -> loop (k::(loop acc r)) l
    | Empty -> acc
  in loop [] s;;


