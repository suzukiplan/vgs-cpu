#include "vgsasm.h"

char* load_file(const char* path)
{
    FILE* fp;
    void* data;
    int size;

    fp = fopen(path, "rb");
    if (NULL == fp) {
        return NULL;
    }
    if (-1 == fseek(fp, 0, SEEK_END)) {
        fclose(fp);
        return NULL;
    }
    size = (int)ftell(fp);
    if (size < 1) {
        fclose(fp);
        return NULL;
    }
    if (-1 == fseek(fp, 0, SEEK_SET)) {
        fclose(fp);
        return NULL;
    }
    data = malloc(size + 1);
    if (NULL == data) {
        fclose(fp);
        return NULL;
    }
    ((char*)data)[size] = '\0';
    if (size != fread(data, 1, (size_t)size, fp)) {
        fclose(fp);
        free(data);
        return NULL;
    }
    fclose(fp);
    return data;
}

void trimstring(char* src)
{
    int i, j;
    int len;
    for (i = 0; ' ' == src[i]; i++)
        ;
    if (i) {
        for (j = 0; src[i] != '\0'; j++, i++) {
            src[j] = src[i];
        }
        src[j] = '\0';
    }
    for (len = (int)strlen(src) - 1; 0 <= len && ' ' == src[len]; len--) {
        src[len] = '\0';
    }
}

void back_space(char* ln)
{
    if (!*ln) return;
    do {
        *ln = *(ln + 1);
        ln++;
    } while (*(ln - 1));
}

int check_GR(char* token)
{
    if (0 == strcasecmp(token, "A")) {
        return A4;
    } else if (0 == strcasecmp(token, "AH")) {
        return A2;
    } else if (0 == strcasecmp(token, "AO")) {
        return A1;
    } else if (0 == strcasecmp(token, "B")) {
        return B4;
    } else if (0 == strcasecmp(token, "BH")) {
        return B2;
    } else if (0 == strcasecmp(token, "BO")) {
        return B1;
    } else if (0 == strcasecmp(token, "C")) {
        return C4;
    } else if (0 == strcasecmp(token, "CH")) {
        return C2;
    } else if (0 == strcasecmp(token, "CO")) {
        return C1;
    } else if (0 == strcasecmp(token, "D")) {
        return D4;
    } else if (0 == strcasecmp(token, "DH")) {
        return D2;
    } else if (0 == strcasecmp(token, "DO")) {
        return D1;
    } else {
        return -1;
    }
}

int getHex(char c)
{
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 11;
        case 'c':
        case 'C':
            return 12;
        case 'd':
        case 'D':
            return 13;
        case 'e':
        case 'E':
            return 14;
        case 'f':
        case 'F':
            return 15;
    }
    return -1;
}

int getDec(char c)
{
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
    }
    return -1;
}

int getOct(char c)
{
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
    }
    return -1;
}

int getBin(char c)
{
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
    }
    return -1;
}

int check_literal(char* token, unsigned int* result)
{
    int i, v;
    if ('$' == token[0]) {
        *result = 0;
        for (i = 1; token[i]; i++) {
            if (-1 == (v = getHex(token[i]))) {
                return -1;
            }
            *result <<= 4;
            *result |= v & 0xf;
        }
        return 0;
    } else if ('0' == token[0]) {
        *result = 0;
        for (i = 1; token[i]; i++) {
            if (-1 == (v = getOct(token[i]))) {
                return -1;
            }
            *result <<= 3;
            *result |= v & 0x7;
        }
        return 0;
    } else if ('B' == token[0]) {
        *result = 0;
        for (i = 1; token[i]; i++) {
            if (-1 == (v = getBin(token[i]))) {
                return -1;
            }
            *result <<= 1;
            *result |= v & 0x1;
        }
        return 0;
    } else if ('-' == token[0]) {
        *result = 0;
        for (i = 1; token[i]; i++) {
            if (-1 == (v = getDec(token[i]))) {
                return -1;
            }
            *result *= 10;
            *result += v % 10;
        }
        *result *= -1;
        return 0;
    } else if (-1 != getDec(token[0])) {
        *result = 0;
        for (i = 0; token[i]; i++) {
            if (-1 == (v = getDec(token[i]))) {
                return -1;
            }
            *result *= 10;
            *result += v % 10;
        }
        return 0;
    }
    return -1;
}

int check_address(char* token, unsigned int* result, int* m)
{
    char buf[1024];
    strcpy(buf, token);
    size_t len = strlen(buf);
    if (len < 3) return -1;
    if ('[' == buf[0] && ']' == buf[len - 1]) {
        buf[len - 1] = '\0';
        *m = M4;
        return check_literal(buf + 1, result);
    } else if ('[' == buf[0] && ']' == buf[len - 2] && ('H' == buf[len - 1] || 'h' == buf[len - 1])) {
        buf[len - 2] = '\0';
        *m = M2;
        return check_literal(buf + 1, result);
    } else if ('[' == buf[0] && ']' == buf[len - 2] && ('O' == buf[len - 1] || 'o' == buf[len - 1])) {
        buf[len - 2] = '\0';
        *m = M1;
        return check_literal(buf + 1, result);
    }
    return -1;
}
