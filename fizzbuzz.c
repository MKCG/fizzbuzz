#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define STDOUT_BUFFER_SIZE 65536
#define STDOUT_FD 1

static int increment_number(char * number, int number_length) {
    number[number_length - 1] = 0;

    for (int i = number_length - 2; i >= 0; i--) {
        if (number[i] < '9') {
            number[i]++;
            return number_length;
        }

        number[i] = '0';
    }

    for (int i = number_length; i > 0; i--) {
        number[i] = number[i - 1];
    }

    number[0] = '1';

    return number_length + 1;
}

static void loop() {
    char buffer[1 << 19] = "1\n2\nFizz\n4\nBuzz\nFizz\n7\n8\nFizz\nBuzz\n11\nFizz\n13\n14\nFizzBuzz\n16\n17\nFizz\n19\nBuzz\nFizz\n22\n23\nFizz\nBuzz\n26\nFizz\n28\n29\nFizzBuzz\n";
    uint64_t offset = strlen(buffer);

    char number[64] = "31";
    int number_length = 2;

    while (1) {
        #define BUFFER_WRITE_NUMBER()                                   \
            __builtin_memcpy(&buffer[offset], number, number_length);   \
            offset += number_length;                                    \
            buffer[offset++] = '\n';                                    \
            number[number_length - 1]++;

        #define BUFFER_WRITE_TWO_NUMBERS()                                                      \
            __builtin_memcpy(&buffer[offset], number, number_length - 1);                       \
            __builtin_memcpy(&buffer[offset + 1 + number_length], number, number_length - 1);   \
                                                                    \
            offset += number_length;                                \
            buffer[offset - 1] = number[number_length - 1];         \
            buffer[offset++] = '\n';                                \
            number[number_length - 1]++;                            \
                                                                    \
            offset += number_length;                                \
            buffer[offset - 1] = number[number_length -1];          \
            buffer[offset++] = '\n';                                \
            number[number_length - 1]++;

        #define BUFFER_WRITE_FIZZ()                         \
            __builtin_memcpy(&buffer[offset], "Fizz\n", 5); \
            offset += 5;                                    \
            number[number_length - 1]++;                    \

        #define BUFFER_WRITE_FIZZBUZZ()                         \
            __builtin_memcpy(&buffer[offset], "FizzBuzz\n", 9); \
            offset += 9;

        #define BUFFER_WRITE_FIZZ_THEN_BUZZ()                       \
            __builtin_memcpy(&buffer[offset], "Fizz\nBuzz\n", 10);  \
            offset += 10;

        #define BUFFER_WRITE_BUZZ_THEN_FIZZ()                       \
            __builtin_memcpy(&buffer[offset], "Buzz\nFizz\n", 10);  \
            offset += 10;

        #define BUFFER_WRITE_1_TO_10()      \
            BUFFER_WRITE_TWO_NUMBERS();     \
            BUFFER_WRITE_FIZZ();            \
            BUFFER_WRITE_NUMBER();          \
            BUFFER_WRITE_BUZZ_THEN_FIZZ();  \
            number[number_length - 1] += 2; \
            BUFFER_WRITE_TWO_NUMBERS();     \
            BUFFER_WRITE_FIZZ_THEN_BUZZ();  \
            number[number_length - 1]++;

        #define BUFFER_WRITE_11_TO_21()     \
            BUFFER_WRITE_NUMBER();          \
            BUFFER_WRITE_FIZZ();            \
            BUFFER_WRITE_TWO_NUMBERS();     \
            BUFFER_WRITE_FIZZBUZZ();        \
            number[number_length - 1]++;    \
            BUFFER_WRITE_TWO_NUMBERS();     \
            BUFFER_WRITE_FIZZ();            \
            BUFFER_WRITE_NUMBER();          \
            BUFFER_WRITE_BUZZ_THEN_FIZZ();

        #define BUFFER_WRITE_22_TO_30()     \
            BUFFER_WRITE_TWO_NUMBERS();     \
            BUFFER_WRITE_FIZZ_THEN_BUZZ();  \
            number[number_length - 1] += 2; \
            BUFFER_WRITE_NUMBER();          \
            BUFFER_WRITE_FIZZ();            \
            BUFFER_WRITE_TWO_NUMBERS();     \
            BUFFER_WRITE_FIZZBUZZ();

        #define BUFFER_WRITE_30()               \
            BUFFER_WRITE_1_TO_10();             \
            number[number_length - 1] = '1';    \
            number[number_length - 2]++;        \
                                                \
            BUFFER_WRITE_11_TO_21();            \
            number[number_length - 1] = '2';    \
            number[number_length - 2]++;        \
                                                \
            BUFFER_WRITE_22_TO_30();            \
            number[number_length - 1] = '1';    \
            number[number_length - 2]++;

        BUFFER_WRITE_30();
        BUFFER_WRITE_30();
        BUFFER_WRITE_1_TO_10();

        // > 99
        number_length = increment_number(number, number_length);
        number[number_length - 1] = '1';

        BUFFER_WRITE_11_TO_21();
        number[number_length - 1] = '2';
        number[number_length - 2]++;

        BUFFER_WRITE_22_TO_30();
        number[number_length - 1] = '1';
        number[number_length - 2]++;

        BUFFER_WRITE_30();
        BUFFER_WRITE_30();
        BUFFER_WRITE_1_TO_10();
        number[number_length - 1] = '1';
        number[number_length - 2]++;
        BUFFER_WRITE_11_TO_21();

        // > 99
        number_length = increment_number(number, number_length);
        number[number_length - 1] = '2';

        BUFFER_WRITE_22_TO_30();
        number[number_length - 1] = '1';
        number[number_length - 2]++;

        BUFFER_WRITE_30();
        BUFFER_WRITE_30();
        BUFFER_WRITE_1_TO_10();
        number[number_length - 1] = '1';
        number[number_length - 2]++;
        BUFFER_WRITE_11_TO_21();
        number[number_length - 1] = '2';
        number[number_length - 2]++;
        BUFFER_WRITE_22_TO_30();

        // > 99
        number_length = increment_number(number, number_length);

        BUFFER_WRITE_30();

        while (offset >= STDOUT_BUFFER_SIZE) {
            offset -= write(STDOUT_FD, buffer, STDOUT_BUFFER_SIZE);
        }
    }

    offset -= write(STDOUT_FD, buffer, offset);
}

int main() {
    loop();

    return 0;
}
