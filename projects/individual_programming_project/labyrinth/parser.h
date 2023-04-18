#ifndef PARSER_H
#define PARSER_H

// Funkcja zwracająca tablicę zawierającą rozmiary labiryntu
// w poszczególnych wymiarach.
unsigned long long int * get_dimensions(void);

// Funkcja zwracająca tablicę zawierającą iloczyny prefiksowe
// tablicy get_dimensions.
unsigned long long int * get_prefix_product(void);

// Funkcja zwracająca numer pola startowego w labiryncie.
unsigned long long int get_start(void);

// Funkcja zwracająca numer pola końcowego w labiryncie.
unsigned long long int get_goal(void);

// Funkcja zwracająca liczbę wymairów labiryntu.
int get_k(void);

// Funkcja zwalniająca zaalokowaną pamięć.
void free_mem(void);

// Funkcja obsługująca errory.
void error(int n);

// Funkcja ustawiająca x-te pole labiryntu jako odwiedzone.
void set_as_visited(unsigned long long int x);

// Funkcja zwracająca true, jeśli x-te pole labiryntu zostało już odwiedzone,
// lub jest w nim ściana, w przeciwnym przypadku zwraca false.
bool is_visited(unsigned long long int x);

// Funkcja wczytująca dane wejściowe.
void read_input(void);

#endif /* PARSER_H */
