#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"


static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to encode a string to base64
char *base64_encode(const unsigned char *input, size_t input_len) {
    size_t output_len = 4 * ((input_len + 2) / 3); // Calculate the length of the output
    char *output = malloc(output_len + 1); // +1 for null-terminator

    if (output == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0, j = 0; i < input_len;) {
        uint32_t octet_a = i < input_len ? input[i++] : 0;
        uint32_t octet_b = i < input_len ? input[i++] : 0;
        uint32_t octet_c = i < input_len ? input[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        output[j++] = base64_chars[(triple >> 3 * 6) & 0x3F];
        output[j++] = base64_chars[(triple >> 2 * 6) & 0x3F];
        output[j++] = base64_chars[(triple >> 1 * 6) & 0x3F];
        output[j++] = base64_chars[(triple >> 0 * 6) & 0x3F];
    }

    // Pad with '=' characters if needed
    for (size_t i = 0; i < (3 - input_len % 3) % 3; i++) {
        output[output_len - 1 - i] = '=';
    }

    output[output_len] = '\0'; // Null-terminate the output
    return output;
}

unsigned char* base64_decode(const char* input, size_t* output_length) {
    size_t input_length = strlen(input);
    size_t decoded_length = 0;

    // Calculate the size of the decoded data
    for (size_t i = 0; i < input_length; i++) {
        if (input[i] == '=' || strchr(base64_chars, input[i]) != NULL) {
            decoded_length++;
        }
    }

    // Ensure the input is a valid base64 string
    if (decoded_length % 4 != 0) {
        fprintf(stderr, "Invalid base64 input\n");
        return NULL;
    }

    decoded_length = (decoded_length * 3) / 4;

    // Allocate memory for the decoded data
    unsigned char* decoded_data = (unsigned char*)malloc(decoded_length);
    if (decoded_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;
    int buffer = 0;
    int bits_remaining = 0;

    // Decode the base64 string
    while (i < input_length) {
        char ch = input[i++];
        if (ch == '=') {
            break; // Padding, end of the input
        }

        // Find the index of the character in the base64 character set
        const char* p = strchr(base64_chars, ch);
        if (p == NULL) {
            fprintf(stderr, "Invalid base64 character: %c\n", ch);
            free(decoded_data);
            return NULL;
        }

        int index = p - base64_chars;

        // Append the bits to the buffer
        buffer = (buffer << 6) | index;
        bits_remaining += 6;

        // Output 8 bits at a time from the buffer
        if (bits_remaining >= 8) {
            bits_remaining -= 8;
            decoded_data[j++] = (buffer >> bits_remaining) & 0xFF;
        }
    }

    *output_length = decoded_length;
    return decoded_data;
}

uint32_t hash( unsigned char *code, size_t code_n )
{
    uint32_t hash = 0xFFFFFFFF;

    // Process four characters at a time
    for (size_t i = 0; i < code_n-code_n%4; i += 4) {
        hash ^= ( *(uint32_t*)(code + i) );
    }
    // Process remaining characters
    for (size_t i = code_n / 4 * 4; i < code_n; ++i) {
        hash ^= (code[i]);
        //hash <<= 8;
    }
    return hash;
}

uint32_t check_collision(int address1, int address2, size_t n1, size_t n2)
{
    // Calculate the end addresses of the objects
    int endAddress1 = address1 + n1;
    int endAddress2 = address2 + n2;

    // Check for collision
    if (address1 >= address2 && address1 < endAddress2) {
        // Object 1 overlaps with the end of Object 2
        return 1; // Collide
    } else if (address2 >= address1 && address2 < endAddress1) {
        // Object 2 overlaps with the end of Object 1
        return 1; // Collide
    } else {
        // No collision
        return 0;
    }
}
