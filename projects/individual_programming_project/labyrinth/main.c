#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "queue.h"
#include "parser.h"

// Funkcja szukająca najkrótszej ścieżki od startu do mety (algorytm BFS).
void solve() {
	unsigned long long int start = get_start(), goal = get_goal();
	if (is_visited(start) ) error(2);
    if (is_visited(goal) ) error(3);
    
    if (start == goal) {
		printf("0\n");
		free_mem();
		free_queue();
		return;
	}
    unsigned long long int answer = 0;
    unsigned long long int *dimensions = get_dimensions();
    unsigned long long int *prefix_product = get_prefix_product();
    int k = get_k();
    queue_insert(start, 0);
    while(!queue_is_empty()){
        unsigned long long int x = queue_front_value();
        unsigned long long int d = queue_front_distance();
        queue_pop();
        if(x == goal) {
            answer = d;
            break;
        }
        unsigned long long int curr = x;
        set_as_visited(x);
        unsigned long long int next;
        for(int i = k - 1; i >= 0 ; --i) {
			unsigned long long int pp = 1;
			if (i > 0) pp = prefix_product[i - 1];
            unsigned long long int kth_dim = x / pp;
            x = x % pp;
            if(kth_dim !=  0) {
                next = curr - pp;
                if (!is_visited(next)) {
					queue_insert(next, d + 1);
					set_as_visited(next);
				}
            }

            if(kth_dim != dimensions[i] - 1){
                next = curr + pp;
                if (!is_visited(next)) {
					queue_insert(next, d + 1);
					set_as_visited(next);
				}
            }
        }
    }
    if(answer == 0) {
        printf("NO WAY\n");
    }
    else {
        printf("%lld\n", answer);
    }
    free_mem();
    free_queue();
}

int main() {
	read_input();
	solve();
}
