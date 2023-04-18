(**
   Zadanie Przelewanka
   Autor: Maja Tkaczyk
   Recenzent: Florian Ficek
*) 

let przelewanka tab = 
  let odw = Hashtbl.create 1000000  (*Hashtablica przechowujaca informacje, czy dany stan został już rozpatrzony*)
  and kolejka = Queue.create ()
  and wynik = ref (-1) 
  and n = Array.length tab 
  in
  
  if n = 0 then 0
  else 
  
    (*funkcja obliczjąca nwd dwóch liczb*)
    let rec nwd a b =
      if a = 0 then b 
      else nwd (b mod a) a
    in
  
    let nwd_pojemnosci = Array.fold_left (fun ac (x, _) -> nwd ac x) 0 tab 
    in
    if nwd_pojemnosci = 0 then 
      if Array.for_all (fun(_, a) -> a=0) tab then 0
      else -1
    else 
    (*sprawdzenie warunków koniecznych:
    -docelowa ilość wody w każdej szklance musi być podzielna przez nwd wszystkich pojemnosci
    -przynajmniej jedna szklanka musi być pełna lub pusta
    *)
    let spr_nwd = Array.exists (fun (a, b) -> (b mod nwd_pojemnosci) <> 0 ) tab
    and spr_kon = Array.exists (fun (a, b) -> (b=0 || a=b)) tab
    in 
    if spr_nwd || not spr_kon then -1
    
    else 
      let pojemnosc = Array.init n (fun x -> fst tab.(x))
      and cel = Array.init n (fun x -> snd tab.(x)) in 
  
      (*stan początkowy - wszystkie szklanki puste*)
      let pocz = Array.create n 0 in
      
      (*funkcja generująca wszystkie możliwe ruchy - backtracking*)
      let krok (stan, ile) =
        if stan = cel then ile
        else if Hashtbl.mem odw stan = true then -1
        else (
          Hashtbl.add odw stan true; 
      
          (*napełnienie każdej ze szklanek do pełna*)
          for i=0 to (n-1) do
            let pom = Array.copy stan in
            pom.(i) <- pojemnosc.(i);
            Queue.push (pom, ile+1) kolejka;
          done;
      
          (*całkowite opróżnienie każdej ze szklanek*)
          for i=0 to (n-1) do
            let pom = Array.copy stan in
            pom.(i) <- 0;
            Queue.push (pom, ile+1) kolejka;
          done;
  
          (*wszystkie możliwości przelania z jednej szklanki do drugiej*)
          for i=0 to (n-1) do
            for j=0 to (n-1) do 
              if i <> j && stan.(i) <> 0 && stan.(j) <> pojemnosc.(j) then (
                let pom = Array.copy stan 
                and przelane = min stan.(i) (pojemnosc.(j) - stan.(j)) in
                pom.(i) <- pom.(i) - przelane;
                pom.(j) <- pom.(j) + przelane;
                Queue.push (pom, ile+1) kolejka;
              )
            done
          done;
          (-1)
        ) 
      in 
      
      Queue.push (pocz, 0) kolejka;
      while (!wynik = (-1) && not (Queue.is_empty kolejka)) do
        let x = Queue.pop kolejka in 
        wynik := krok x; 
      done;
      !wynik;;
  
  