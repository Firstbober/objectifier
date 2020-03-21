#ifndef UTHASH_STRUCTS_H
#define UTHASH_STRUCTS_H
#include <stddef.h>
#include <stdint.h>

/*
Copyright (c) 2003-2018, Troy D. Hanson http://troydhanson.github.com/uthash/
All rights reserved.
*/
typedef struct UT_hash_bucket {
  struct UT_hash_handle *hh_head;
  unsigned count;

  /* expand_mult is normally set to 0. In this situation, the max chain length
   * threshold is enforced at its default value, HASH_BKT_CAPACITY_THRESH. (If
   * the bucket's chain exceeds this length, bucket expansion is triggered).
   * However, setting expand_mult to a non-zero value delays bucket expansion
   * (that would be triggered by additions to this particular bucket)
   * until its chain length reaches a *multiple* of HASH_BKT_CAPACITY_THRESH.
   * (The multiplier is simply expand_mult+1). The whole idea of this
   * multiplier is to reduce bucket expansions, since they are expensive, in
   * situations where we know that a particular bucket tends to be overused.
   * It is better to let its chain length grow to a longer yet-still-bounded
   * value, than to do an O(n) bucket expansion too often.
   */
  unsigned expand_mult;

} UT_hash_bucket;

/*
Copyright (c) 2003-2018, Troy D. Hanson http://troydhanson.github.com/uthash/
All rights reserved.
*/
typedef struct UT_hash_table {
  UT_hash_bucket *buckets;
  unsigned num_buckets, log2_num_buckets;
  unsigned num_items;
  struct UT_hash_handle *tail; /* tail hh in app order, for fast append    */
  ptrdiff_t hho; /* hash handle offset (byte pos of hash handle in element */

  /* in an ideal situation (all buckets used equally), no bucket would have
   * more than ceil(#items/#buckets) items. that's the ideal chain length. */
  unsigned ideal_chain_maxlen;

  /* nonideal_items is the number of items in the hash whose chain position
   * exceeds the ideal chain maxlen. these items pay the penalty for an uneven
   * hash distribution; reaching them in a chain traversal takes >ideal steps */
  unsigned nonideal_items;

  /* ineffective expands occur when a bucket doubling was performed, but
   * afterward, more than half the items in the hash had nonideal chain
   * positions. If this happens on two consecutive expansions we inhibit any
   * further expansion, as it's not helping; this happens when the hash
   * function isn't a good fit for the key domain. When expansion is inhibited
   * the hash will still work, albeit no longer in constant time. */
  unsigned ineff_expands, noexpand;

  uint32_t signature; /* used only to find hash tables in external analysis */
#ifdef HASH_BLOOM
  uint32_t bloom_sig; /* used only to test bloom exists in external analysis */
  uint8_t *bloom_bv;
  uint8_t bloom_nbits;
#endif

} UT_hash_table;

/*
Copyright (c) 2003-2018, Troy D. Hanson http://troydhanson.github.com/uthash/
All rights reserved.
*/
typedef struct UT_hash_handle {
  struct UT_hash_table *tbl;
  void *prev;                     /* prev element in app order      */
  void *next;                     /* next element in app order      */
  struct UT_hash_handle *hh_prev; /* previous hh in bucket order    */
  struct UT_hash_handle *hh_next; /* next hh in bucket order        */
  void *key;                      /* ptr to enclosing struct's key  */
  unsigned keylen;                /* enclosing struct's key len     */
  unsigned hashv;                 /* result of hash-fcn(key)        */
} UT_hash_handle;

#endif /* UTHASH_STRUCTS_H */
