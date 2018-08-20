#ifndef CHEW_HASHTABLE_H
#define CHEW_HASHTABLE_H

namespace chew{

template <bool long_vector_support>
struct HashTableType {
  typedef IntegerVector type;
};

template <>
struct HashTableType<true> {
  typedef NumericVector type;
};

template <bool long_vector_support, typename Vector>
class HashData_int{
public:

  inline size_t hash(R_xlen_t indx){
    int xi = x[indx];
    if (xi == NA_INTEGER) return 0;
    return scatter((unsigned int) xi, d);
  }

  inline int equal(R_xlen_t i, R_xlen_t j){
    if (i < 0 || j < 0) return 0;
    return x[i] == x[j];
  }

  inline int lookup(SEXP table, SEXP x, R_xlen_t indx, HashData *d)	\
  {								                                            \
    int *h = HTDATA_INT(d);					                       \
    hlen i = HASHFUN(x, indx, d);				                  \
    while (h[i] != NIL) {					                         \
      if (EQLFUN(table, h[i], x, indx))			             \
        return h[i] >= 0 ? h[i] + 1 : d->nomatch;	     \
      i = (i + 1) % d->M;					                         \
    }							                                           \
    return d->nomatch;					                            \
  }


private:

  inline size_t scatter(unsigned int key){
    return 3141592653U * key >> (32 - K);
  }

  int K;
  size_t M;
  R_xlen_t nmax;
  bool isLong;
  HashTableType<long_vector_support>::type HashTable;
  int nomatch;
  Vector x;
};

};

#endif
