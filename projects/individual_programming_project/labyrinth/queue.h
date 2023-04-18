#ifndef QUEUE_H
#define QUEUE_H

// Funkcja dodająca do kolejki element o polach:
// value = x i distance = d.
// Zwraca true, jeżeli dodanie powiodło sie, w przeciwnym wypadku
// (gdy alokacja się nie udała) zwraca false;
bool queue_insert(unsigned long long int x, unsigned long long int d);

// Funkcja zwracająca true, gdy kolejka jest pusta i false w przeciwnym wypadku.
bool queue_is_empty(void);

// Funkcja usuwająca pierwszy (przedni) element kolejki.
void queue_pop(void);

// Funkcja zwracająca wartość pola value pierwszego elementu kolejki.
unsigned long long int queue_front_value(void);

// Funkcja zwracająca wartość pola distance pierwszego elementu kolejki.
unsigned long long int queue_front_distance(void);

// Funkcja usuwająca z kolejki wszystkie znajdujące się w niej elementy.
void free_queue(void);

#endif /* QUEUE_H */
