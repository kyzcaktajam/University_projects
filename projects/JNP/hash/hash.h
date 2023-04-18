
#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <iostream>
#include <iterator>
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <assert.h>
namespace jnp1 {
extern "C" {
using hash_function_t = uint64_t (*)(uint64_t const *s, size_t c);
#else
typedef uint64_t (hash_function_t)(uint64_t const *s, size_t c);
#endif

/*
 * Tworzy tablicÄ haszujÄcÄ i zwraca jej identyfikator. Parametr
 * hash_function jest wskaĹşnikiem na funkcjÄ haszujÄcÄ, ktĂłra daje w wyniku
 * liczbÄ uint64_t i ma kolejno parametry uint64_t const * oraz size_t.
 */
unsigned long hash_create(hash_function_t hash_function);

/*
 * Usuwa tablicÄ haszujÄcÄ o identyfikatorze id, o ile ona istnieje.
 * W przeciwnym przypadku nic nie robi.
 */
void hash_delete(unsigned long id);

/*
 * Daje liczbÄ ciÄgĂłw przechowywanych w tablicy haszujÄcej
 * o identyfikatorze id lub 0, jeĹli taka tablica nie istnieje.
 */
size_t hash_size(unsigned long id);

/*
 * Wstawia do tablicy haszujÄcej o identyfikatorze id ciÄg liczb
 * caĹkowitych seq o dĹugoĹci size. Wynikiem jest informacja, czy operacja
 * siÄ powiodĹa. Operacja siÄ nie powiedzie, jeĹli nie ma takiej tablicy
 * haszujÄcej, jeĹli tablica haszujÄca zawiera juĹź taki ciÄg, jeĹli
 * parametr seq ma wartoĹÄ NULL lub parametr size ma wartoĹÄ 0.
 */
bool hash_insert(unsigned long id, uint64_t const *seq, size_t size);

/*
 * Usuwa z tablicy haszujÄcej o identyfikatorze id ciÄg liczb caĹkowitych
 * seq o dĹugoĹci size. Wynikiem jest informacja, czy operacja siÄ
 * powiodĹa. Operacja siÄ nie powiedzie, jeĹli nie ma takiej tablicy
 * haszujÄcej, jeĹli tablica haszujÄca nie zawiera takiego ciÄgu,
 * jeĹli parametr seq ma wartoĹÄ NULL lub parametr size ma wartoĹÄ 0.
 */
bool hash_remove(unsigned long id, uint64_t const *seq, size_t size);

/*
 * JeĹli tablica haszujÄca o identyfikatorze id istnieje i nie jest pusta,
 * to usuwa z niej wszystkie elementy. W przeciwnym przypadku nic nie robi.
 */
void hash_clear(unsigned long id);

/*
 * Daje wynik true, jeĹli istnieje tablica haszujÄca o identyfikatorze id
 * i zawiera ona ciÄg liczb caĹkowitych seq o dĹugoĹci size. Daje wynik
 * false w przeciwnym przypadku oraz gdy parametr seq ma wartoĹÄ NULL lub
 * parametr size ma wartoĹÄ 0.
 */
bool hash_test(unsigned long id, uint64_t const *seq, size_t size);

#ifdef __cplusplus
} // extern "C"
} // namespace jnp1
#endif

#endif /* HASH_H */
