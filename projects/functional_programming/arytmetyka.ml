
(* Zadanie "Arytmetyka przybliżonych wartości"*)
(* Autor : Maja Tkaczyk*)
(* Recenzent: Paweł Pilarski*)
type przedzial = float * float;;

type wartosc = przedzial * przedzial;;
(*  suma dwóch przedziałów: ((a, b), (c, d)) gdzie a<c jeżeli są rozłączne, lub ((a, d), (nan, nan)) lub ((nan, nan), (nan, nan)) w przeciwnym wypadku)  
*)

let stworz_przedzial a b : przedzial = ( a, b );;

let stworz_wartosc a b : wartosc = (a, b);;


let is_nan x =
  if ( classify_float x = FP_nan ) then true
  else false;;

let pusty = stworz_przedzial nan nan;;

(* sprawdza, czy przedzial jest pusty, czyli (nan, nan)*)
let czy_pusty ( ( a, b) : przedzial ) =
  if ( classify_float a = FP_nan || classify_float b = FP_nan ) then true
  else false;;


(*zwraca mniejszą z dwóch liczb z uwzględnieniem typu nan*)
let mini x y =
  if ( is_nan x ) then y
  else if ( is_nan y ) then x
  else min x y;;

(*zwraca większą z dwóch liczb z uwzględnieniem typu nan*)
let maxi x y =
  if ( is_nan x ) then y
  else if ( is_nan y ) then x
  else max x y;;

(*zmienia kolejność przedziałów, jeżeli się nie zgadza*)
let fix1 (( p1, p2 ): wartosc ) = 
  if( mini  ( fst p1 ) ( fst p2) ) = ( fst p2 ) then
    stworz_wartosc p2 p1
  else stworz_wartosc p1 p2;;

(*łączy dwa przedziały, jeżeli się przecinają*)
let fix2 (( p1, p2 ): wartosc ) = 
  if( czy_pusty p2 ) then stworz_wartosc p1 p2
  else if ( snd p1 ) >= ( fst p2 ) then stworz_wartosc ( stworz_przedzial (fst p1) (snd p2) ) pusty
  else stworz_wartosc p1 p2;; 

let fix w=
  fix2(fix1 w );;


let p_zero = stworz_przedzial 0.0 0.0;; 

let wartosc_dokladnosc x p = 
  let blad = abs_float( x *. p /. 100.00 ) 
  in
  stworz_wartosc ( stworz_przedzial  ( x -. blad)  ( x +. blad )) ( stworz_przedzial  nan nan );;

let wartosc_dokladna x =
  stworz_wartosc ( stworz_przedzial x x ) pusty;;

let wartosc_od_do x y = 
  stworz_wartosc ( stworz_przedzial x y ) pusty;;


(*minimum z przedziału*)
let min_przedzial ( ( x1, x2 ) : przedzial ) = mini x1 x2;;

(*maksimum z przedziału*)
let max_przedzial ( ( x1, x2 ) : przedzial ) = maxi x1 x2;;

(*minimum z wartości*)
let min_wartosc ( ( x1, x2 ) : wartosc ) = mini ( min_przedzial x1 ) ( min_przedzial x2 );;

(*maksimum z wartosci*)
let max_wartosc ( ( x1, x2 ) : wartosc ) = maxi ( max_przedzial x1 ) ( max_przedzial x2 );;



(* sprawdza, czy y nalezy do przedzialu *)
let in_przedzial ( ( x1, x2 ) : przedzial ) y = 
  if czy_pusty ( ( x1, x2 ) : przedzial ) then false
  else  if ( y >= x1 ) && ( y <= x2 ) then 
    true
  else 
    false ;;

(*sprawdza, czy x należy do wartości*)
let in_wartosc ( ( x1, x2 ) : wartosc ) y = 
  if ( in_przedzial x1 y) || (in_przedzial x2 y ) then true
  else false;;

(*zwraca średnią wartość *)
let sr_wartosc x = ( max_wartosc x +. min_wartosc x ) /. 2.0;;


(* zwraca granicę a/b, niektóre przypadki zdefiniowane na potrzeby prostszej implementacji*) 

let granica a b p2 =
  match ( classify_float a, classify_float b) with 
  | ( FP_infinite, FP_infinite ) -> (a *. b ); 
  | ( FP_zero, FP_infinite ) -> 0.0
  | ( FP_zero, FP_normal ) -> 0.0 
  | ( FP_normal, FP_infinite ) -> 0.0
  | ( FP_infinite, FP_normal ) when ( a=infinity ) && (b > 0.0 ) -> infinity
  | ( FP_infinite, FP_normal ) when ( a=neg_infinity ) && (b < 0.0 ) -> infinity
  | ( FP_infinite, FP_normal ) -> neg_infinity 
  | ( FP_zero, FP_zero ) -> 0.0
  | ( _, FP_nan ) -> nan
  | ( FP_nan, _ ) -> nan
  | ( _, FP_zero )  when ( ( a < 0.0 ) && ( p2 = 2 ) ) -> infinity
  | ( _, FP_zero )  when ( ( a > 0.0 ) && ( p2 = 1 ) ) -> infinity
  | ( _, FP_zero )  -> neg_infinity 
  | ( FP_normal, FP_normal ) -> ( a /. b )
  | ( _, _ ) -> 0.0;; 

(* zwraca najmniejszą z 4 liczb*)
let mini_z_4 a b c d =
  mini ( mini a b ) ( mini c d );;

(* zwraca najwiekszą z 4 liczb*)
let maxi_z_4 a b c d =
  maxi ( maxi a b ) ( maxi c d );;

(* dzieli przedzial przez drugi przedział, który jest cały nieujemny/ niedodatni, zwraca przedzial*)
let dziel_czesc ( ( a, b ) : przedzial ) ( ( c, d ) : przedzial ) =
  if czy_pusty ( (c, d) : przedzial ) then pusty
  else 
    let g1 = mini_z_4 ( granica a c  1 ) ( granica a d  2 ) ( granica b c  1 ) ( granica b d  2 ) 
    and  g2 = maxi_z_4 ( granica a c  1 ) ( granica a d  2 ) ( granica b c  1 ) ( granica b d  2 )
    in stworz_przedzial g1 g2;;

(* dzieli przedzial na dwa przedzialy takie, że każdy ma tylko elementy nieujemne lub tylko elementy niedodatnie (lub "dokleja" przedział pusty)*)
let przetnij_przedzial ( ( a, b ) : przedzial ) = 
  if czy_pusty ( (a, b) : przedzial ) then stworz_wartosc pusty pusty
  else if ( a < 0.0 ) && ( b > 0.0 ) then stworz_wartosc (a, 0.0) (0.0, b)
  else stworz_wartosc (a, b) pusty;;

(* dzieli przedział przez przedział, zwraca wartość*)
let podziel_przedzialy ( ( a, b ) : przedzial ) ( ( x, y ) : przedzial ) =
  if ( ( x, y ) = pusty ) || ( ( x, y ) = p_zero ) then stworz_wartosc pusty pusty
  else 
    let ( ( l1, r1 ), ( l2, r2 ) ) = przetnij_przedzial( x, y )
    in fix ( stworz_wartosc ( dziel_czesc (a, b) ( l1, r1) ) ( dziel_czesc (a, b) ( l2, r2) ) ) ;; 


(* zwraca sumę dwóch przedziałów*)
let suma_2_przedzialow ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) =
  let p = stworz_przedzial a1 a2 and q = stworz_przedzial b1 b2
  in
  if czy_pusty p then stworz_wartosc q pusty 
  else if czy_pusty q then stworz_wartosc p pusty 
  else if ( in_przedzial p b1) || ( in_przedzial p b2 ) then 
    fix (stworz_wartosc ( ( mini a1 b1 ), ( maxi a2 b2 ) ) pusty )
  else fix (stworz_wartosc p q);;

(* zwraca wartość, będącą sumą wartości i przedziału ( jako sumę dwóch przedziałów, co ze względu na specyfikę przedziałów będących argumentami - działa)
Jeżeli wartosc ma 2 przedzialy, to dodany przedzial nie utworzy 3.*) 
let suma_zbiorow_przedzialu_i_wartosci ( ( ( ( a, b ) : przedzial ), ( ( c, d ) : przedzial ) ) : wartosc ) ( ( x, y ) : przedzial ) = 
  let p1 = stworz_przedzial a b and p2 = stworz_przedzial c d and q = stworz_przedzial x y
  in 
  if czy_pusty q then fix(stworz_wartosc p1 p2)
  else if czy_pusty p2 then suma_2_przedzialow p1 q 
  else if ( in_przedzial p1 x ) && ( in_przedzial p2 y ) then fix (stworz_wartosc ( mini c ( mini a x ), maxi d ( maxi b y ) ) pusty )
  else if ( in_przedzial p1 x )  then fix (stworz_wartosc ( ( mini a x ), ( maxi b y ) ) p2) 
  else fix( stworz_wartosc p1 ( ( mini c x ), ( maxi d y ) ));;



(* sumuje 2 wartosci *)
let zlacz_wartosci ( ( p1, p2 ) : wartosc )  ( ( p3, p4 ) : wartosc )  = 
  suma_zbiorow_przedzialu_i_wartosci( suma_zbiorow_przedzialu_i_wartosci ( ( p1, p2 ) : wartosc ) p3 ) p4;;

(*funkcja pomocnicza w procedurze sumowania 4 wartosci*)
let zlacz_4_wartosci a b c d = 
  zlacz_wartosci ( zlacz_wartosci (zlacz_wartosci a b) c) d ;;


(*sprawdza, czy przedzial przecina sie z wartoscia*)
let spr_przedzial ((a, b): przedzial) w =
  if (in_wartosc w a) || (in_wartosc w b) then true
  else false;;
  

let pusta : wartosc = (pusty, pusty);;

let czy_pusta w =
  if (czy_pusty (fst w) ) && (czy_pusty (snd w ))then true
  else false;;

(*sprawdza, czy mozna polaczyc 2 wartosci tak, zsumowaly sie do maksymalnie dwoch przedzialow*)
let do_zlaczenia ( ( p1, p2 ) : wartosc )  ( ( p3, p4 ) : wartosc )  = 
  if ( czy_pusta ( ( p1, p2 ) : wartosc ) ) || ( czy_pusta ( ( p3, p4 ) : wartosc ) ) then false
  else if (czy_pusty p1 && czy_pusty p2) || (czy_pusty p3 && czy_pusty p4) then true
  else if (czy_pusty p2) && ( spr_przedzial p1 ( ( p3, p4 ) : wartosc )) then true
  else if (czy_pusty p4) && ( spr_przedzial p3 ( ( p1, p2 ) : wartosc )) then true
  else if ( spr_przedzial p3 ( ( p1, p2 ) : wartosc ) ) && ( spr_przedzial p4 ( ( p1, p2 ) : wartosc ) ) then true
  else false;;
  

(*sprawdza, czy sa co najmniej 2 puste wartosci - wtedy z pozostalych mozna stworzyc wartosc*)
let czy_stop w1 w2 w3 w4 = 
  if czy_pusta w1 && czy_pusta w2 then true
  else if czy_pusta w1 && czy_pusta w3 then true
  else if czy_pusta w1 && czy_pusta w4 then true
  else if czy_pusta w2 && czy_pusta w3 then true
  else if czy_pusta w2 && czy_pusta w4 then true
  else if czy_pusta w3 && czy_pusta w4 then true
  else false;; 

(*sumuje 4 wartosci*)
let rec zlacz4 w1 w2 w3 w4 =
  if czy_stop w1 w2 w3 w4 then zlacz_4_wartosci w1 w2 w3 w4
  else if do_zlaczenia w1 w2 then zlacz4 (zlacz_wartosci w1 w2) pusta w3 w4
  else if do_zlaczenia w1 w3 then zlacz4 (zlacz_wartosci w1 w3 ) w2 pusta w4
  else if do_zlaczenia w1 w4 then zlacz4 (zlacz_wartosci w1 w4 ) w2 w3 pusta
  else if do_zlaczenia w2 w3 then zlacz4 w1 (zlacz_wartosci w2 w3 ) pusta w4
  else if do_zlaczenia w2 w4 then zlacz4 w1 (zlacz_wartosci w2 w4 ) w3 pusta 
  else zlacz4 w1 w2 (zlacz_wartosci w3 w4 ) pusta;;
      


(*pomocnicza funkcja mnożenia*)
let razy_pomoc a b =
  if ( a = 0.0 ) || ( b = 0.0 ) then 0.0
  else a *. b;;

(*najwieksza liczba jaką można uzyskać mnożąc dwa przedziały*)
let ogr_maks_razy ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) =
  maxi_z_4 ( razy_pomoc a1 b1 ) ( razy_pomoc a1 b2 ) ( razy_pomoc a2 b1 ) ( razy_pomoc a2 b2 );;

(*najmniejsza liczba jaką można uzyskać mnożąc dwa przedziały*)
let ogr_min_razy ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) =
  mini_z_4 ( razy_pomoc a1 b1 ) ( razy_pomoc a1 b2 ) ( razy_pomoc a2 b1 ) ( razy_pomoc a2 b2 );;

(*mnożenie przedziałów*)
let razy_przedzial ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) =
  if( czy_pusty ( ( a1, a2 ) : przedzial ) ) || ( czy_pusty ( ( b1, b2 ) : przedzial ) ) then pusty
  else stworz_przedzial ( ogr_min_razy ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) ) ( ogr_maks_razy ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ));;

    
(*dzielenie  wartości*)
let podzielic (  ( p1, p2 ) : wartosc ) (  ( p3, p4 ) : wartosc ) =
  zlacz4( podziel_przedzialy p1 p3 ) ( podziel_przedzialy p1 p4 ) ( podziel_przedzialy p2 p3 ) ( podziel_przedzialy p2 p4 );;


(*mnożenie wartości*)
let razy (  ( p1, p2 ) : wartosc ) (  ( p3, p4 ) : wartosc ) =
  zlacz4 (stworz_wartosc (razy_przedzial p1 p3) pusty) (stworz_wartosc (razy_przedzial p1 p4) pusty) (stworz_wartosc (razy_przedzial p2 p3) pusty) (stworz_wartosc (razy_przedzial p2 p4) pusty);;

(*dodaje przedziały*)
let plus_przedzial( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial) =
  ( ( a1 +. b1, a2 +. b2 ) : przedzial );;

(*dodaje wartosci*)
let plus (  ( p1, p2 ) : wartosc ) (  ( p3, p4 ) : wartosc ) =
  zlacz4 (stworz_wartosc (plus_przedzial p1 p3) pusty) (stworz_wartosc (plus_przedzial p1 p4) pusty) (stworz_wartosc (plus_przedzial p2 p3) pusty) (stworz_wartosc (plus_przedzial p2 p4) pusty);;

(*najmniejsza liczba jaką można uzyskać odejmując dwa przedziały*)
let ogr_min_minus ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) = 
  if ( czy_pusty ( ( a1, a2 ) : przedzial ) ) || ( czy_pusty ( ( b1, b2 ) : przedzial ) ) then nan 
  else if  ( is_nan ( a1 -. b2 ) ) then neg_infinity
  else ( a1 -. b2 );;

(*największa liczba jaką można uzyskać odejmując dwa przedziały*)
let ogr_maks_minus ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) = 
  if ( czy_pusty ( ( a1, a2 ) : przedzial ) ) || ( czy_pusty ( ( b1, b2 ) : przedzial ) ) then nan   
  else if ( is_nan( a2 -. b1) ) then infinity
  else ( a2 -. b1 );;

(*odejmuje przedzialy*)
let minus_przedzial ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) =
  stworz_przedzial ( ogr_min_minus ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) ) ( ogr_maks_minus ( ( a1, a2 ) : przedzial ) ( ( b1, b2 ) : przedzial ) );;


(*odejmuje wartosci*)
let minus (  ( p1, p2 ) : wartosc ) (  ( p3, p4 ) : wartosc ) =
  zlacz4 (stworz_wartosc (minus_przedzial p1 p3) pusty) (stworz_wartosc (minus_przedzial p1 p4) pusty) (stworz_wartosc (minus_przedzial p2 p3) pusty) (stworz_wartosc (minus_przedzial p2 p4)pusty);;