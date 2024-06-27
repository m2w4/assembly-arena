/*
  ╔══════════════════════════════════════════════════════════╗
  ║                         util.h
  ║ defines misc. utility functions used all across the
  ║ virtual machine.
  ╚══════════════════════════════════════════════════════════╝
*/


#ifndef UTIL_H
#define UTIL_H
/* ╔═ base64_encode ═════════════════════════════════════════╗
 * ║
 * ║ Encodes & decodes a char array. self-explaining.
 * ║
 * ╚═════════════════════════════════════════════════════════╝
 */
char *base64_encode(const unsigned char *input, size_t input_len);
unsigned char *base64_decode(const char *input, size_t *output_len);

/* ╔═ hash ═════════════════════════════════════════╗
 * ║
 * ║ hashes an i8 array (normally a warrior) to
 * ║ a 32-bit hash for deterministic code
 * ║ placement and gameplay.
 * ║
 * ╚════════════════════════════════════════════════╝
 */
uint32_t hash( unsigned char *code, size_t code_n );

/* ╔═ check_collision ═══════════════════════════════╗
 * ║
 * ║ Calculates if two ranges are overlapping.
 * ║ Used by rng trial-and-error warrior placement.
 * ║
 * ║ parameters:
 * ║ - addresses: code start location
 * ║ - n1, n2: size of code fragments
 * ║
 * ║ returns:
 * ║ - 0 if not colliding, 1 when collide
 * ║
 * ╚════════════════════════════════════════════════╝
 */
uint32_t check_collision(int address1, int address2, size_t n1, size_t n2);
#endif
