#include <stdio.h>
#include <string.h>

typedef struct {
    int e, b, n, s, t, v;
} s_list;

void parse_flags(int argc, char **argv, s_list *flags) {
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[1][0] == '-') {
            i = 1;
            while (argv[1][i] != '\0') {
                switch (argv[1][i]) {
                    case 'e':
                    case 'E': flags->e = 1; break;
                    case 'b': flags->b = 1; break;
                    case 'n': flags->n = 1; break;
                    case 's': flags->s = 1; break;
                    case 't':
                    case 'T': flags->t = 1; break;
                    case 'v': flags->v = 1; break;
                }
                ++i;
            }
        }
    }
    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            if (strcmp("--number-nonblank", argv[i]) == 0) {
                flags->b = 1;
            } else if ((strcmp("--number", argv[i]) == 0)) {
                flags->n = 1;
            } else if ((strcmp("--squeeze-blank", argv[i]) == 0)) {
                flags->s = 1;
            } else {
                fprintf(stderr,
                    "cat: illegal option -- %c\nusage: cat [-benstuv] [file "
                    "...]\n",
                    argv[i][1]);
            }
        }
     }
}

char print_nonprint_ch(char ch) {
    if (ch != 10 && ch != 127) {
        ch += 64;
        printf("^");
    } else if (ch == 127) {
        ch -= 64;
        printf("^");
    }
    return ch;
}

int write_file(char **argv, s_list *flags, int i) {
    FILE *fptr;
    char ch;
    fptr = fopen(argv[i], "r");
    if (fptr == NULL) {
        fprintf(stderr, "Error! opening file\n");
        return (1);
    }
    int n = 1;
    char ch_2 = '\n';
    while ((ch = fgetc(fptr)) != EOF) {
        if (flags->s == 1 && ch_2 == '\n') {
            while (ch_2 == '\n' && ch == '\n') {
                ch = fgetc(fptr);
                ch_2 = ch;
                if (ch != '\n') {
                    if (flags->n == 1) {
                        printf("%6d\t\n", n);
                        n++;
                    } else {
                        printf("\n");
                    }
                }
            }
            ch_2 = '\n';
        }
        if (flags->b == 1 && ch_2 == '\n') {
            flags->n = 0;
            if (ch != '\n') {
                printf("%6d\t", n++);
            }
        }
        if (flags->n == 1 && ch_2 == '\n') {
            printf("%6d\t", n);
            n++;
        }
        if (flags->t == 1) {
            if ((ch >= 0 && ch <= 31 && ch != 10) || ch == 127)
                ch = print_nonprint_ch(ch);
            flags->v = 0;
        }
        if (flags->e == 1) {
            if ((ch >= 0 && ch <= 31 && ch != 9) || ch == 127) {
                if ((ch != 10 && ch != 127) || ch == 127)
                    ch = print_nonprint_ch(ch);
                else
                    printf("$");
            }
            flags->v = 0;
        }
        if (flags->v == 1) {
            if ((ch >= 0 && ch <= 31 && ch != 9) || ch == 127)
                ch = print_nonprint_ch(ch);
        }
        printf("%c", ch);
        ch_2 = ch;
    }
    fclose(fptr);
    return (0);
}

int main(int argc, char **argv) {
    s_list flags = {0};
    parse_flags(argc, argv, &flags);
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-')
            write_file(argv, &flags, i);
    }
    return 0;
}
