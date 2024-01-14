
#ifndef SHA512_H_
#define SHA512_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

/* Feed a full block in */
#define SHA512_BLOCK_SIZE 128

/* SHA512 state. State is updated as data is fed in, and then the final
 * hash can be read out in slices.
 *
 * Data is fed in as a sequence of full blocks terminated by a single
 * partial block.
 */
struct sha512_state {
  uint64_t h[8];
  uint8_t partial[SHA512_BLOCK_SIZE];
  size_t len;
};

/* Set up a new context */
void sha512_init(struct sha512_state *s);

void sha512_add(struct sha512_state *s, const void *data, size_t len);

/* Fetch a slice of the hash result. */
#define SHA512_HASH_SIZE 64

void sha512_final(struct sha512_state *s, uint8_t *hash);

static inline void *sha512_final_get(struct sha512_state *s) {
  sha512_final(s, s->partial);
  return s->partial;
}

#endif
