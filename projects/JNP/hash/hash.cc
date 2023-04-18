// Authors: Filip Nogaj, Maja Tkaczyk

#include <iostream>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <set>
#include <assert.h>
#include "hash.h"

#ifdef __cplusplus
using hash_function_t = uint64_t (*)(uint64_t const *s, size_t c);
#else
typedef uint64_t (hash_function_t)(uint64_t const *s, size_t c);
#endif

std::unordered_map<unsigned long, std::pair<hash_function_t, std::unordered_map< uint64_t, std::set<std::vector<uint64_t> > > > > & hashTables(){
    static std::unordered_map<unsigned long, std::pair<hash_function_t, std::unordered_map< uint64_t, std::set<std::vector<uint64_t> > > > > ans;
    return ans;
}

std::unordered_map< uint64_t, std::set<std::vector<uint64_t> > > & empty(){
    static std::unordered_map< uint64_t, std::set<std::vector<uint64_t> > > ans;
    return ans;
}

std::unordered_map<uint64_t, size_t> & sizes(){
    static std::unordered_map<uint64_t, size_t> ans;
    return ans;
}

    
namespace jnp1 {   
	#ifdef NDEBUG
        const bool debug = false;
    #else
        const bool debug = true;
    #endif
    	 
	unsigned long counter = 0;

    unsigned long hash_create(hash_function_t hash_function) {
	    if (debug) {
			assert(hash_function != NULL);
		}
	    if (debug) {
			std::cerr << __func__ << "(" << &hash_function << ")\n";
		}
	    unsigned long id = counter;
	    hashTables()[id] = {hash_function, empty()};
	    counter++;
	    if (debug) {
			std::cerr << __func__ << ": hash table #" << id << " created\n";
		}
	    return id;
    }

    void hash_delete(unsigned long id) {
	    if (debug) {
			std::cerr << __func__ << "(" << id << ")\n" << __func__ << ": " <<"hash table #" << id;
		}
	    auto it = hashTables().find(id);
	    if (it == hashTables().end()) {
			if (debug) {
	        std::cerr << " does not exist\n";
		}
		    return;
	    }
	    hashTables().erase(it);
	    if (debug) {
	        std::cerr << " deleted\n";
		}
    }

    size_t hash_size(unsigned long id) {
	    if (debug) {
			std::cerr << __func__<< '(' << id << ")\n";
		}
	    auto it = hashTables().find(id);
	    if (it == hashTables().end()) {
			if (debug) {
				std::cerr << __func__<< ": hash table #" << id << " does not exist\n";
			}
			return 0;
		}
	    if (debug) {
			std::cerr << __func__<< ": hash table #" << id << " contains " << sizes()[id] << " element(s)\n";
		}
	    return sizes()[id];
    }

    void showSeq (uint64_t const * seq, size_t size) {
		if (!debug) {
			return;
		}
	    if (seq == NULL) {
		    std::cerr << "NULL";
		    return;
	    }
	    if (size == 0) {
		    std::cerr << '"' << '"';
		    return;
	    }
	    std::cerr << '"';
	    for (size_t i = 0; i < size - 1; ++i) {
		    std::cerr << seq[i] << ' ';
	    }
	    std::cerr << seq[size - 1] << '"';
    }

    bool hash_insert(unsigned long id, uint64_t const * seq, size_t size) {
		if (debug) {
	        std::cerr << __func__<< '(' << id << ", ";
	        showSeq(seq, size);
	        std::cerr << ", " << size << ")\n"; 
		}
	    if (seq == NULL) {
		    if (debug) {
				std::cerr << __func__ << ": invalid pointer (NULL)\n";
			}
		    if (size != 0) {
		        return false;
		    }
	    }
	    if (size == 0) {
		    if (debug) {
				std::cerr << __func__ << ": invalid size (0)\n";
			}
		    return false;
	    }
	    if (debug) {
			std::cerr << __func__ << ": hash table #" << id;
		}
	    auto it = hashTables().find(id);
	    if (it == hashTables().end()) {
		    if (debug) {
				std::cerr << " does not exist\n";
			}
		    return false;
	    }
	    if (debug) { 
		    std::cerr << ", sequence ";
	        showSeq(seq, size);
	    }
	    std::vector<uint64_t> newVec(size);
	    
	    for (size_t i = 0; i < size; ++i) {
		    newVec[i] = seq[i];
	    }
	
	    uint64_t hashVal = it->second.first(seq, size);
	
	    if (hashTables()[id].second[hashVal].find(newVec) == hashTables()[id].second[hashVal].end()) {
		    sizes()[id]++;
		    hashTables()[id].second[hashVal].insert(newVec);
		    if (debug) {
				std::cerr << " inserted\n";
			}
	    }
	    else {
		    if (debug) {
				std::cerr << " was present\n";
			}
		    return false;
	    }
	    return true;
    }

    bool hash_remove(unsigned long id, uint64_t const * seq, size_t size) {
	    if (debug) {
			std::cerr << __func__ << '(' << id << ", ";
	        showSeq(seq, size);
	        std::cerr << ", " << size << ")\n";
	    }
	    if (seq == NULL) {
		    if (debug) {
				std::cerr << __func__ << ": invalid pointer (NULL)\n";
			}
		    if (size != 0) {
		        return false;
			}
	    }
	    if (size == 0) {
		    if (debug) {
				std::cerr << __func__ << ": invalid size (0)\n";
			}
		    return false;
	    }
	    if (debug) {
			std::cerr << __func__ << ": hash table #" << id;
		}
	    auto it = hashTables().find(id);
	    if (it == hashTables().end()) {
		    if (debug) {
				std::cerr << " does not exist\n";
			}
		    return false;
	    }
	    if (debug) {
			std::cerr << ", sequence ";
	        showSeq(seq, size);
	    }
	
	    std::vector<uint64_t> newVec(size);
	
	    for (size_t i = 0; i < size; ++i) {
		    newVec[i] = seq[i];
	    }
	
	    uint64_t hashVal = it->second.first(seq, size);
	
	    if (hashTables()[id].second[hashVal].find(newVec) != hashTables()[id].second[hashVal].end()) {
		    hashTables()[id].second[hashVal].erase(newVec);
		    sizes()[id]--;
		    if (debug) {
				std::cerr << " removed\n";
			}
	    }
	    else {
		    if (debug) {
				std::cerr << " was not present\n";
			}
		    return false;
	    }
	
	    return true;
    }

    void hash_clear(unsigned long id) {
	    if (debug) {
			std::cerr << __func__ << '(' << id << ")\n" << __func__ << ": hash table #" << id;
		}
	    auto it = hashTables().find(id);
	    if (it == hashTables().end()) {
		    if (debug) { 
				std::cerr << " does not exist\n";
			}
		    return;
	    }
	    if (sizes()[id] == 0) {
		    if (debug) {
				std::cerr << " was empty\n";
			}
		    return;
	    }
	    if (debug) {
			std::cerr << " cleared\n";
		}
	    hashTables()[id].second = empty();
	    sizes()[id] = 0;
    }

    bool hash_test(unsigned long id, uint64_t const * seq, size_t size) {
	    if (debug) {
			std::cerr << __func__ << '(' << id << ", ";
	        showSeq(seq, size);
	        std::cerr << ", " << size << ")\n";
	    }
	    if (seq == NULL) {
		    if (debug) {
				std::cerr << __func__ << ": invalid pointer (NULL)\n";
			}
		    if (size != 0) {
				return false;
			}
	    }
	    if (size == 0) {
		    if (debug) {
				std::cerr << __func__ << ": invalid size (0)\n";
			}
		    return false;
	    }
	   if (debug) {
		    std::cerr << __func__ << ": hash table #" << id;
		}
	    auto it = hashTables().find(id);
	    if (it == hashTables().end()) {
		    if (debug) {
				std::cerr << " does not exist\n";
			}
		    return false;
	    }
	    if (debug) {
			std::cerr << ", sequence ";
	        showSeq(seq, size);
		}
		
		std::vector<uint64_t> newVec(size);
	
	    for (size_t i = 0; i < size; ++i) {
		    newVec[i] = seq[i];
	    }
	
	    uint64_t hashVal = it->second.first(seq, size);
	
	    if (hashTables()[id].second[hashVal].find(newVec) != hashTables()[id].second[hashVal].end()) {
		    if (debug) {
				std::cerr << " is present\n";
			}
		    return true;
	    }
	    if (debug) {
			std::cerr << " is not present\n";
		}
	    return false;
    }
}
