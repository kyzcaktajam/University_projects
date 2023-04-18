/**
 * University of Warsaw
 * JNP 2022/2023, task 3
 *
 * Authors :
 * Maja Tkaczyk mt438772
 * Tsimafei Lukashevich tl439668
 */

#ifndef MONEYBAG_H
#define MONEYBAG_H

#include <iostream>
#include <string>
#include <stdbool.h>
    
class Moneybag {
public:
    typedef uint64_t coin_number_t;
private:
    coin_number_t livre;
    coin_number_t solidus;
    coin_number_t denier;    
public: 
    constexpr Moneybag(coin_number_t l, coin_number_t s, coin_number_t d) 
    	: livre(l), solidus(s), denier(d){}
        
    constexpr Moneybag(const Moneybag &m) 
        : livre(m.livre), solidus(m.solidus), denier(m.denier){}

    constexpr int livre_number() const {
        return livre;
    }
    constexpr int solidus_number() const {
        return solidus;
    }
    constexpr int denier_number() const {
        return denier;
    }
    
    explicit operator bool() const {
        if (livre == 0 && denier == 0 && solidus == 0) {
            return false;
        }
        else {
            return true;
        }
    } 
    
    constexpr Moneybag &operator =(const Moneybag &m) {
        livre = m.livre;
        solidus = m.solidus;
        denier = m.denier;
        return *this;
    }
    
    constexpr Moneybag operator+(const Moneybag &m) const {
        return Moneybag(safe_sum(livre, m.livre), 
                        safe_sum(solidus, m.solidus),
                        safe_sum(denier, m.denier));
    }
    
    constexpr Moneybag& operator +=(const Moneybag &m){
        livre = safe_sum(livre, m.livre);
        solidus = safe_sum(solidus, m.solidus);
        denier = safe_sum(denier, m.denier);
        return *this;
    }
    
    constexpr Moneybag operator-(const Moneybag &m) const {
        return Moneybag(safe_dif(livre, m.livre), 
                        safe_dif(solidus, m.solidus), 
                        safe_dif(denier, m.denier));
    }
    
    constexpr Moneybag& operator -=(const Moneybag &m){
        livre = safe_dif(livre, m.livre);
        solidus = safe_dif(solidus, m.solidus);
        denier = safe_dif(denier, m.denier);
        return *this;
    }
    
    constexpr Moneybag operator*(const uint64_t c) const {
        return Moneybag(safe_prod(livre, c),
                        safe_prod(solidus, c), 
                        safe_prod(denier, c));
    }
    
    
    constexpr Moneybag& operator *=(const coin_number_t c){
        livre = safe_prod(livre, c);
        solidus = safe_prod(solidus, c);
        denier *= safe_prod(denier, c);
        return *this;
    }
    
    constexpr bool operator==(const Moneybag &m) const{
        if (livre == m.livre && denier == m.denier && solidus == m.solidus) {
            return true;
        }
        else {
            return false;
        }
    }
    
    constexpr std::partial_ordering operator <=>(Moneybag const &m) const {
        if (livre == m.livre && denier == m.denier && solidus == m.solidus) {
          return std::partial_ordering::equivalent;
        }    
        else if (livre >= m.livre && denier >= m.denier && solidus >= m.solidus) {
          return std::partial_ordering::greater;
        }
        else if (livre <= m.livre && denier <= m.denier && solidus <= m.solidus) {
          return std::partial_ordering::less;
        }
        else {
            return std::partial_ordering::unordered;
        }
    }
    
    std::string toString() const {
        std::string res = "(" +
                std::to_string(livre) +
                    (livre == 1 ? " livr, " : " livres, ") +
                std::to_string(solidus) +
                    (solidus == 1 ? " solidus, " : " soliduses, ") +
                std::to_string(denier) +
                    (denier == 1 ? " denier)" : " deniers)");
        return res;
    }
    
    static constexpr coin_number_t safe_sum(const coin_number_t a, const coin_number_t b) {
        coin_number_t sum = a + b;
        if (sum < std::max(a, b)) {
            throw std::out_of_range("");
        }
        return sum;
    }
    
    static constexpr coin_number_t safe_dif(const coin_number_t a, const coin_number_t b) {
        if (b > a) {
            throw std::out_of_range("");
        }
        return a - b;
    }
    
    static constexpr coin_number_t safe_prod(const coin_number_t a, const coin_number_t b) {
        if (a == 0 || b == 0) {
            return 0;
        }

        coin_number_t limit = UINT64_MAX;
        if (limit / b < a) {
            throw std::out_of_range("");
        }
        return a * b;
    }
};

inline std::ostream &operator <<(std::ostream &os, Moneybag const &m) {
    return os << m.toString();
}

constexpr Moneybag Livre{1, 0, 0};
constexpr Moneybag Solidus{0, 1, 0};
constexpr Moneybag Denier{0, 0, 1};

constexpr Moneybag operator*(const uint64_t c, const Moneybag m) {
    return Moneybag(m.livre_number() * c,  m.solidus_number() * c, m.denier_number() * c);
}

class Value {
private:
    Moneybag::coin_number_t val;
public:
    constexpr Value(): val(0){}
    
    constexpr Value(uint64_t v): val(v){}

    constexpr Value(const Moneybag &m)
     : val(Moneybag::safe_sum (
                Moneybag::safe_sum (
                      Moneybag::safe_prod (240, m.livre_number()),
                      Moneybag::safe_prod (12, m.solidus_number())
                      ),
                m.denier_number()
                )
       ){}
    
    constexpr Value &operator =(const Value &v) {
        val = v.val;
        return *this;
    }
    
    constexpr std::strong_ordering operator <=>(Value const &v) const {
        if (val > v.val) {
          return std::strong_ordering::greater;
        }
        else if (val < v.val) {
            return std::strong_ordering::less;
        }
        else {
            return std::strong_ordering::equal;
        }
    }
    
    constexpr std::strong_ordering operator <=>(uint64_t v) const {
        if (val > v) {
          return std::strong_ordering::greater;
        }
        else if (val < v) {
            return std::strong_ordering::less;
        }
        else {
            return std::strong_ordering::equal;
        }
    }
    
    constexpr bool operator==(const Value &v) const{
        if (val == v.val) {
            return true;
        }
        else {
            return false;
        }
    }
    
    constexpr bool operator==(uint64_t v) {
        if (val == v) {
            return true;
        }
        else {
            return false;
        }
    }
    
    explicit operator std::string() {
        return std::to_string(val);
    }
};

#endif /* MONEYBAG_H */
