#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define ZERO_ID 48
#define LOWERCASE_SHIFT 87
#define CAPITAL_SHIFT 55
#define CAPITAL_A_ID 65
#define LOWERCASE_A_ID 97
#define R_ID 82

// Struktura przechowująca tablicę liczb typu unsigned long long int
struct Array {
	int size;
	int alloc_size;
	unsigned long long int *numbers;
};
typedef struct Array Array;

// Zmienne globalne.
static Array *dimensions, *prefix_product;
static char *visited;
static int k = 0;
static bool odd_visited = false;
static unsigned long long int labyrinth_size, visited_size = 0, start, goal;

// Funkcja zwracająca tablicę przechowującą kolejne wymiary labiryntu.
unsigned long long int * get_dimensions(void) {
	return dimensions->numbers;
}

// Funkcja zwracająca tablicę zawierającą iloczyny prefiksowe 
// tablicy dimensions.
unsigned long long int * get_prefix_product(void) {
	return prefix_product->numbers;
}

// Funkcja zwracająca numer pola startowego.
unsigned long long int get_start(void) {
	return start;
}

// Funkcja zwracająca numer pola końcowego.
unsigned long long int get_goal(void) {
	return goal;
}

// Funkcja zwracająca liczbę k - liczba wymiarów labiryntu.
int get_k(void) {
	return k;
}

// Funkcja sprawdzająca, czy iloczyn dwóch liczb przekracza SIZE_MAX
// (jeśli tak - zwraca true, jeżeli nie - zwraca false).
static bool check_overflow(unsigned long long int a, unsigned long long int b) {
	unsigned long long int * pointer = 
	(unsigned long long int*)malloc(sizeof(unsigned long long int));
	bool res =  __builtin_umulll_overflow (a, b, pointer);
	free(pointer);
	return res;
}

// Funkcja zwracająca wskażnik na nową tablicę, lub NULL, gdy alkokacja
// pamięci nie powiodła się.
static Array *new_array(void) {
	Array *arr = 0;
	arr = (Array *)malloc(sizeof(Array));
	if (!arr) {
		exit(0);
	}
	arr->size = 0;
	arr->alloc_size = 0;
	arr->numbers = 0;
	return arr;
}

// Funkcja usuwająca tablicę.
static void delete_array(Array *array) {
	if (!array)
		return;
	free(array->numbers);
	free(array);
}

// Funkcja czyszcząca zaalokowaną pamięć.
void free_mem(void) {
	free(visited);
	delete_array(dimensions);
	delete_array(prefix_product);
}

// Funkcja obsługująca zwracanie errorów.
void error(int n) {
	free_mem();
	fprintf(stderr, "ERROR %d\n", n);
	exit(1);
}

// Funkcja dodająca liczbę do tablicy.
static void add_number(Array *array, unsigned long long int number) {
    if(array->size == array->alloc_size) {
        int new_size = 2 * array->size;
        if (new_size == 0) new_size = 1;
        unsigned long long int *new_numbers = NULL;
        new_numbers = 
        realloc(array->numbers, new_size * sizeof(unsigned long long int));
        if (!new_numbers) {
			delete_array(array);
			error(0);
		}
		array->numbers = new_numbers;
		array->alloc_size = new_size;
    }
    array->numbers[array->size] = number;
    array->size++;
}

// Funkcja czytająca linijkę danych, zapisuje dane do struktury Array
// (wykorzystywana do wczytania pierwszej linii wejścia).
static Array * read_and_save_line(int line_id) {
	unsigned long long int number = 0;
	int started = 0, empty = 1, a;
	Array *array = NULL;
	array = new_array();
    while((a = getchar()) != '\n') {
		empty = 0;
		if (a == EOF) {
			free(array);
			error(line_id);
		}
        if (isspace(a)) {
            if(started == 1) {
                if (number == 0){
					free(array);
                    error(line_id);
                }
                add_number(array, number);
                started = 0;
                number = 0;
            }
        }
        else {
            if (!isdigit(a)) {
				free(array);
                error(line_id);
            }
            started = 1;
            int digit = a - ZERO_ID;
            if (check_overflow(number, 10) || number > SIZE_MAX - digit) {
				error(line_id);
			}
            number = number * 10 + digit;
        }
    }
    if(started == 1) {
        if(number == 0){
			free(array);
            error(line_id);
        }
        add_number(array, number);
    }
    if (empty == 0 && array->size == 0) {
		delete_array(array);
		error(line_id);
	}
	if (empty) {
		delete_array(array);
		return NULL;
	}
	return array;
}

// Funkcja cztająca linię danych będących kolejnymi współrzędnymi kostki
// labiryntu, zwraca liczbę będącą numerem tej kostki w labiryncie
// (wykorzystywana do wczytania drugiej i trzeciej linii).
static unsigned long long int read_line(int line_id) {
	unsigned long long int number = 0, result = 0;
	int started = 0, a, numbers_counter = 0;
    while((a = getchar()) != EOF) {
		if (a == '\n') {
			break;
		}
        else if (isspace(a)) {
            if(started == 1) {
                if (number == 0){
                    error(line_id);
                }
                numbers_counter++;
                if (numbers_counter > k || 
                number > dimensions->numbers[numbers_counter - 1]) {
			        error(line_id);
		        }
		        unsigned long long int pp = 1;
		        if (numbers_counter > 1)
		            pp = prefix_product->numbers[numbers_counter - 2];
                result += (number - 1) * pp;
                started = 0;
                number = 0;
            }
        }
        else {
            if (!isdigit(a)) {
                error(line_id);
            }
            started = 1;
            int digit = a - ZERO_ID;
            if (check_overflow(number, 10) || number > SIZE_MAX - digit) {
				error(line_id);
			}
            number = number * 10 + digit;
        }
    }
	if (a == EOF) {
		error(line_id);
	}
    if(started == 1) {
        if(number == 0){
            error(line_id);
        }
        numbers_counter++;
        if (numbers_counter > k || 
        number > dimensions->numbers[numbers_counter - 1]) {
			error(line_id);
		}
		unsigned long long int pp = 1;
		if (numbers_counter > 1) 
		    pp = prefix_product->numbers[numbers_counter - 2];
        result += (number - 1) * pp;
    }
    if (numbers_counter != k) {
		error(line_id);
	}
	return result;
}

// Funkcja zwracająca wartość cyfry o danym kodzie ASCII.
static char get_id (int x) {
	if (isdigit(x)) {
		return x - ZERO_ID;
	}
	else if (x >= LOWERCASE_A_ID) {
		return x - LOWERCASE_SHIFT;
	}
	else {
		return x - CAPITAL_SHIFT;
	}
}

// Funkcja wykrywająca błąd polegający na tym, że w liczbie opisującej położenie
// ścian labiryntu istnieje i-ty bit o wartości 1, gdzie i jest większe
// niż rozmiar labiryntu.
static void first_bit_error(unsigned long long int size) {
	if (visited[0] == 0)
	    return;
	unsigned long long int first_id = 0;    
	for (int i = 7; i >= 0; i --) {
		if (((1 << i) & (int)visited[0]) != 0) {
			first_id = i;
			break;
		}
	}
	first_id += (8 * (size - 1));
	if (odd_visited) {
	    first_id -= 4;
	}
	if (first_id >= labyrinth_size) {
	    error(4);
	}
}

// Funkcja przygotowująca tablicę odwiedzonych.
static void prepare_visited(unsigned long long int size) {
	visited_size = (labyrinth_size + 7) / 8;
	if (odd_visited) {
	    visited_size = (labyrinth_size + 11) / 8;
	}
	if (size > visited_size) {
		error(4);
	}
	first_bit_error(size);
	char *new_visited = NULL;
	new_visited = realloc(visited, visited_size);
	if (!new_visited) 
		error(0);
	visited = new_visited;
	unsigned long long int shift = visited_size - size;
	for (unsigned long long int i = visited_size - 1; i >= shift; i--) {
		visited[i] = visited[i - shift];
		if (i == 0)
		    break;
	}
	for (unsigned long long int  i = 0; i < shift; i++) {
		visited[i] = 0;
	}
}

// Funkcja wczytująca pojedyńczą liczbe.
static int read_number(void) {
    int started = 0, stop = 0, err = 0, exist = 0, a = 0;
    long long number = 0;
    while(stop == 0 && (a = getchar()) != '\n' && a != EOF) {
        if (isspace(a) == 0) {
            exist = 1;
            started = 1;
            if (!isdigit(a)) err = 1;
            int digit = a - ZERO_ID;
            number = number * 10 + digit;
            if (number >  UINT32_MAX) {
		        error(4);
	        }
        }
        else if (started == 1) {
		    stop = 1;
		}
    }
    if (err == 1 || exist == 0) {
		error(4);
	}
	if (number >  UINT32_MAX) {
		error(4);
	}
    return (int)number;
}

// Funkcja dodająca liczbę do tablicy odwiedzonych.
static void add_to_visited(int *sz, int *alloc_sz, char x) {
	if (*sz == *alloc_sz) {
		char *new_vis = realloc(visited, 2 * (*alloc_sz));
		if (!new_vis) {
			error(0);
		}
		visited = new_vis;
		(*alloc_sz) *= 2;
	}
	visited[*sz] = x;
	(*sz)++;
}

// Funkcja ustawiająca x-te pole labiryntu jako odwiedzone.
void set_as_visited(unsigned long long int x) {
	if (x >= labyrinth_size)
	    return;
	if (odd_visited)
	    x += 4;
	unsigned long long int index = x / 8;
    int mod = x % 8;
    visited[visited_size - index - 1] |= (1 << mod); 
}

// Funkcja zwracająca true, jeżeli x-te pole labiryntu zostało już odwiedzone
// lub znajduje się w nim ściana, w przeciwnym przypadku zwraca false.
bool is_visited(unsigned long long int x) {
	if (odd_visited)
	    x += 4;
	int mod = x % 8;
	if ((visited[visited_size - x / 8 - 1] & (1 << mod)) != 0) {
		return true;
	}
	else {
		return false;
	}
}

// Funkcja wczytująca pierwszy typ (liczba szesnastkowa) drugiej linii danych.
static void read_type1(void) {
	int a = getchar();
	if (a != (int)'x') {
	    error(4);
	}
	visited = (char *)malloc(sizeof(char));
	visited[0] = 0;
	int vis_sz = 0, array_sz = 1, started = 0, number_end = 0, digit_cnt = 0;
	char previous = 0;
	bool exist = 0;
	
	while((a = getchar()) != EOF && a != '\n') {
		if (isxdigit(a)) {
			exist = true;
			if (number_end) {
				return error(4);
			}
			if (a == ZERO_ID && started == 0) {
		        continue;
			}
		    started = 1;
		    digit_cnt++;
		    if (digit_cnt % 2 == 0) {
				add_to_visited(&vis_sz, &array_sz, 16 * previous + get_id(a));
			}
			previous = get_id(a);
		}
		else if (isspace(a)) {
			number_end = 1;
		}
		else {
			error(4);
		}
	}
	if (a == '\n') {
	    if ((a = getchar()) != EOF)
	        error(5);
	}
	
	if (digit_cnt % 2 == 1) {
		odd_visited = true;
		add_to_visited(&vis_sz, &array_sz, 16 * previous);
	}
	if (vis_sz == 0 && !exist) {
		error(4);
	}
	prepare_visited(vis_sz);
}

// Funkcja wczytująca drugi typ czwartej linii danych.
static void read_type2(void) {
	int a, b, m, r, s_0;
    a = read_number();
    b = read_number();
    m = read_number();
    r = read_number();
    s_0 = read_number();
    if (m == 0) {
        error(4);
    }
    int get, new_line = 0;
    while ((get = getchar()) != EOF) {
		if (get == '\n') {
			new_line = 1;
		}
		else if (isspace(get)) {
			if (new_line) {
				error(4);
			}
		}
		else {
			error(4);
		}
	}
    visited_size = (labyrinth_size + 7) / 8;
    visited = calloc(visited_size, sizeof(char));
    if (!visited) {
		error(0);
	}
	unsigned long long int base = 4294967296, prev_s = s_0;
	for(int i = 1; i <= r; ++i) {
        int s_i = (a * prev_s + b) % m;
        int w_i = s_i % labyrinth_size;
        unsigned long long int id = w_i;
        while (id < labyrinth_size) {
            set_as_visited(id);
            id += base;
        }
        prev_s = s_i;
    }
}

// Funkcja wczytująca czwartą linię danych.
static void read_4th_line(void) {
	int a = 0;
	while ((a = getchar()) != EOF) {
		if (a == (int)'\n') {
			error(4);
		}
		else if (!isspace(a)){
			if (a == ZERO_ID) {
				read_type1();
				break;
			}
			else if (a == R_ID) {
				read_type2();
				break;
			}
			else {
				error(4);
			}
		}
	}
}

// Funkcja wczytująca dane wejściowe.
void read_input(void) {
	dimensions = read_and_save_line(1);
	if (!dimensions) {
		error(1);
	}
	k = dimensions->size;
	prefix_product = new_array();
	prefix_product->numbers =
	(unsigned long long int *)malloc(k * sizeof(unsigned long long int));
	if (!prefix_product->numbers) {
		error(0);
	}
	prefix_product->size = k;
	prefix_product->alloc_size = k;
	prefix_product->numbers[0] = dimensions->numbers[0];
	for(int i = 1; i < k; i++) {
		if (check_overflow(prefix_product->numbers[i - 1], 
		dimensions->numbers[i])) {
			error(0);
		}
		prefix_product->numbers[i] = 
		prefix_product->numbers[i - 1] * dimensions->numbers[i];
	}
	labyrinth_size = prefix_product->numbers[k - 1];
	start = read_line(2);
	goal = read_line(3);
	
	read_4th_line();
}
