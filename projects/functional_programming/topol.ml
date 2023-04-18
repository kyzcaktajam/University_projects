(*
Zadanie Sortowanie Topologiczne
Autor: Maja Tkaczyk
Recenzja: Emilia Dębicka
*)

open PMap

exception Cykliczne 

(*  sortowanie topologiczne *)
let topol lista_s = 
  let (l1, l2) = List.split lista_s in
  (*lista wszystkich wierzchołków występujących w początkowej liście (z powtórzeniami)*)
  let wierzcholki =ref (l1@(List.flatten l2) )in
  (*przypisanie wierzchołkom identyfikatorów będących kolejnymi liczbami naturalnymi (1, 2, 3, ..., n)*)
  (* mapy:
  id: wierzchołek -> identyfikator
  di: identyfikator -> wierzchołek
  *)
  let id = ref empty and di = ref empty in
  (*liczba różnych wierzchołków*)
  let n = ref 0 in
  (*jeśli wierzchołek jeszcze nie wystąpił, nadaje mu identyfikator i dodaje do mapy*)
  let dodaj x = 
    if not (mem x !id) then (
      n:= !n + 1;
      di := add !n x !di;
      id := add x !n !id;
    ) in 
  let nn = List.length !wierzcholki in
  for i = 1 to nn do (
    dodaj (List.hd !wierzcholki);
    wierzcholki := List.tl !wierzcholki;
  ) done; 
  
  (*tablice list reprezentujących sąsiadów każdego z wierzchołków (transp - dla grafu transponowanego)*)
  let sas = Array.make (!n + 1) (ref []) and transp = Array.make (!n + 1) (ref []) in
  (*pomocnicza kopia listy*)
  let lista_pom = ref lista_s in
  (*wypełnienie tablic sas i transp*)
  for i = 1 to (List.length lista_s) do 
    let (x, lst) = List.hd !lista_pom in
    lista_pom := List.tl !lista_pom;
    let xx = find x !id in
    let pom_lst = ref lst in
    for j = 1 to (List.length lst) do 
      let y = find (List.hd !pom_lst) !id in
      if y = xx then raise Cykliczne;
      pom_lst := List.tl !pom_lst;
      sas.(xx) <- (ref (y::!(sas.(xx)))); 
      transp.(y) <-( ref (xx::!(transp.(y))));
    done;
  done; 
  
  (*tablica odwiedzonych (0 - nieodwiedzony, 1 - odwiedzony przez dfs, 2 - odwiedzony przez dfs2)*)
  let odw = Array.make (!n + 1 ) 0 in
  (*lista, na której zostaną umieszczone wierzchołki w porządku postorder*)
  let postorder = ref [] in
  
  (*przejście po grafie*)
  let rec dfs x = 
    odw.(x) <- 1;
    let sasiedzi = sas.(x) in
    for i = 1 to (List.length !(sas.(x))) do 
      let y = List.hd !sasiedzi in
      sasiedzi := List.tl !sasiedzi;
      if odw.(y) = 0 then dfs y;
    done;
    postorder := (x::!postorder);
  in
  
  (*przejście po grafie transponowanym, sprawdzenie, czy istnieje cykl*)
  let rec dfs2 x = 
    odw.(x) <- 2;
    let sasiedzi = transp.(x) in
    for i = 1 to (List.length !(transp.(x))) do 
      let y = List.hd !sasiedzi in
      sasiedzi := List.tl !sasiedzi;
      if odw.(y) < 2 then raise Cykliczne;
    done; 
  in 
  
  for i = 1 to !n do 
    if odw.(i) = 0 then dfs i;
  done;
  
  let wynik = ref [] in 
  
  for i = 1 to !n do 
    let x = List.hd !postorder in
    postorder := List.tl !postorder;
    if odw.(x) < 2 then dfs2 x;
    wynik := (find x !di)::!wynik;
  done;
  List.rev !wynik;;