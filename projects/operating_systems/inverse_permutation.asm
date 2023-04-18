global inverse_permutation

; Argumenty funkcji inverse_permutation:
; rdi - n
; rsi - wskaźnik do p
inverse_permutation:
      cmp       rdi, 0                              ; Sprawdzenie, czy n jest dodatnie.
      je        .checking_failed
      mov       rdx, 2147483648                     ; Sprawdzenie, czy n nie przekracza INT_MAX + 1.
      cmp       rdi, rdx
      je        .checking_failed
      mov       rdx, 0
.checking_range:                                    ; Sprawdzenie, czy wszystkie wartości w tablicy należą do przedziału <0, n - 1>.
      cmp       DWORD [rsi + 4 * rdx], 0            ; Indeks komórki sprawdzanej w danej iteracji jest przechowywany w rejestrze rdx.
      jl        .checking_failed
      cmp       DWORD [rsi + 4 * rdx], edi
      jnl       .checking_failed
      inc       rdx
      cmp       rdx, rdi
      jl        .checking_range
      mov       rdx, 0                              ; Rejestr rdx przechowuje i - numer iteracji (aktualnie sprawdzanej komórki tablicy).
.checking_duplicates:                               ; Sprawdzenie, czy wartości w tablicy są parami różne.
      mov       ecx, DWORD [rsi + 4 * rdx]          ; Przepisanie wartości w i-tej komórce tablicy do rejestru ecx.
      cmp       ecx, 0                              ; Jeżeli wartość w i-tej komórce jest ujemna(została wcześniej zanegowana, aby odnotować wystąpienie wartości i),
      jl        .fix_neg                            ; odzyskujemy jej początkową wartość poprzez ponowną negację bitową.
.back_from_fix_neg:
      cmp       DWORD [rsi + 4 * rcx], 0            ; Sprawdzenie, czy wartość p[i] pojawiła się już w tablicy. Miało to miejsce, jeżeli p[p[i]] ma ujemną wartość.
      jl        .fix_after_fail                     ; Jeśli tak, skok warunkowy do sekcji, która przywraca początkowe wartości komórkom w tablicy i kończy funkcje.
      not       DWORD [rsi + 4 * rcx]               ; Negacja bitowa wartości p[p[i]], co pozwala na odnotowanie, że wartości p[i] pojawiła się w tablicy.
      inc       rdx
      cmp       rdx, rdi
      jl        .checking_duplicates
      mov       rdx, 0
      jmp       .inverse_permutation
.fix_neg:
      not       ecx
      jmp       .back_from_fix_neg
.inverse_permutation:                               ; Sekcja odwracająca permutację.
      cmp       DWORD [rsi + 4 * rdx], 0            ; Jeżeli p[i] jest dodatnie, cykl do którego należy został już odwrócony.
      jnl       .end_iter_ip
      not       DWORD [rsi + 4 * rdx]
      mov       rcx, rdx                            ; Rejestr rcx przechowuje indeks aktualnie przetwarzanej komórki tablicy.
      mov       r9d, DWORD [rsi + 4 * rdx]          ; Rejestr r9d przechowuje początkową wartość aktualnie przetwarzanej komórki tablicy.
.inverse_cycle:                                     ; Sekcja odwracająca cykl permutacji.
      mov       r8d, DWORD [rsi + 4 * r9]           ; Przepisanie wartośi komórki, do której zaraz zostanie przepisana wartość rejestru ecx.
      not       r8d                                 ; Negacja bitowa, aby przywrócić faktyczną wartość.
      mov       DWORD [rsi + 4 * r9], ecx           ; Przepisanie do komórki jej wartości po odwróceniu.
      mov       ecx, r9d                            ; Przepisanie indeksu komórki, do której zostało wykonane przepisanie wartości.
      mov       r9d, r8d                            ; Przepisanie początkowej wartości komórki, do której przepisano nową wartość.
      cmp       ecx, edx                            ; Przetworzenie kolejnej komórki cyklu, jeżeli nie jest ona jego początkiem.
      jne        .inverse_cycle
.end_iter_ip:
      inc       rdx
      cmp       rdx, rdi
      jl        .inverse_permutation
.inverse_done:                                      ; Jeżeli sprawdzenie poprawności tablicy i odwrócenie permutacji przebiegło pomyślnie,
      mov       rax, 1                              ; przepisujemy do rejestru rax wynik funkcji (true) i kończymy wykonywanie funkcji.
      ret
.fix_after_fail:                                    ; Segment przywracający tablicę do stanu początkowego, jeżeli zostały wykryte duplikaty.
      mov       rdx, 0
.fix_after_fail_loop:
      cmp       DWORD [rsi + 4 * rdx], 0            ; Jeżeli jakaś komórka tablicy ma wartość ujemną (została wcześniej wykonana negacja bitowa),
      jnl       .end_iter_fail                      ; negujemy ją ponownie, aby przywrócić wartość początkową.
      not       DWORD [rsi + 4 * rdx]
.end_iter_fail:
      inc       rdx
      cmp       rdx, rdi
      jl        .fix_after_fail_loop
.checking_failed:                                   ; Jeżeli podczas sprawdzania poprawności tablicy zostały wykryte nieprawidłowości,
      mov       rax, 0                              ; przepisujemy wynik funkcji(false) do rejestru rax i kończymy wykonywanie funkcji.
      ret
