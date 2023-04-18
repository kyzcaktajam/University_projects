(* Zadanie Drzewa Lewicowe
   Autor: Maja Tkaczyk
   Recenzent: Marcin Żołek
*)

type 'a queue = Null | Node of 'a * int * 'a queue * 'a queue;;
(* typ Node (a, len, l, r), gdzie: 
a - wartość w węźle,
len - długość najdłuższej ścieżki biegnącej w głąb drzewa po prawych poddrzewach,
l, r - odpowiednio lewe i prawe poddrzewo *)

exception Empty;;

let empty = Null;;

let is_empty q = 
  if q = Null then
    true
  else
    false
;;

let new_queue a l q1 q2 = 
  Node (a, l, q1, q2)
;;

let _len q = 
  (* zwraca długość najdłuższej ścieżki biegnącej w głąb drzewa po prawych poddrzewach *)
  match q with
  | Null -> 0
  | Node (_, l, _, _) -> l
;;

let rec join d1 d2 =
  match (d1, d2) with 
  | (Null, _) -> d2
  | (_, Null) -> d1
  | (Node (a1, len1, l1, r1), Node (a2, len2, l2, r2)) ->
      if (a1 > a2) then
        join d2 d1
      else
        let s1 = l1 and s2 = join d2 r1 in
        
        if (_len s1 >= _len s2) then 
            new_queue a1 ((_len s2) + 1) s1 s2
        else 
            new_queue a1 ((_len s1) + 1) s2 s1
;;

let add e q = 
  join (new_queue e 0 empty empty) q
;;

let delete_min q =
  match q with
  | Null -> raise Empty
  | Node (a, len, l, r) -> (a, join l r)
;;