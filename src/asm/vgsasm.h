#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgscpu_internal.h"

#define LOGV printf
#define A1 0
#define A2 1
#define A4 2
#define B1 3
#define B2 4
#define B4 5
#define C1 6
#define C2 7
#define C4 8
#define D1 9
#define D2 10
#define D4 11

#define M1 1
#define M2 2
#define M4 4

struct line_data {
    char error[1024];
    char* buffer;
    int number;
    char* token[8];
    int toknum;
    unsigned char op[16];
    int oplen;
};

char* load_file(const char* path);
void trimstring(char* src);
int check_GR(char* token);
int getHex(char c);
int getDec(char c);
int getOct(char c);
int getBin(char c);
int check_literal(char* token, unsigned int* result);
int check_address(char* token, unsigned int* result, int* m);

struct line_data* parse_lines(char* buf, int* line);
void remove_empty_line(struct line_data* line, int* len);
void parse_token(struct line_data* line, int len);
int parse_operation(struct line_data* line, int len);
int parse_push(struct line_data* line, int i);
int parse_pop(struct line_data* line, int i);
int parse_ld(struct line_data* line, int i);
int parse_st(struct line_data* line, int i);
