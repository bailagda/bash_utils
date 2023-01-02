#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pcre.h>

typedef struct flags {
    int e, i, v, c, l, n, h, s, f, o;
} s_list;

typedef struct support {
    int files_count;
    char *ptrn;
} t_list;

FILE *file_open(char *argv, int flag_s) {  //  открыть файл c паттерном
    FILE *fptr = fopen(argv, "r");
    if (fptr == NULL) {
        if (flag_s == 0) {
            fprintf(stderr, "grep: %s: No such file or directory\n", argv);
        }
        exit(1);
    } else {
        return fptr;
    }
}

void do_pcre_exec(pcre* pattern, int argc, char *argv, s_list *flags, t_list *support) {
    FILE *str = file_open(argv, flags->s);
    if (!str) {
        if (flags->s == 0) {
            fprintf(stderr, "Failed\n");
        }
    } else {
        char s[80] = {0};
        int ovector[30];
        int counter_str = 0;
        for (int i = 0; fgets(s, 80, str) != 0; i++) {
            int count = pcre_exec(pattern,  NULL, (char *) s, strlen((const char *)s), 0, 0, ovector, 30);
            if ((flags->v == 1) ? count == -1 : count == 1) {
                if (support->files_count > 1) {
                    if (flags->h == 0 && flags->c == 0 && flags->l == 0)
                        printf("%s:", argv);
                }
                if (flags->n == 1) {
                    printf("%d:", i+1);
                }
                if (flags->l == 1) {
                    printf("%s\n", argv);
                    break;
                }
                if (flags->o == 1 && flags->v == 0) {
                    printf("%s\n", support->ptrn);
                }
                if (flags->c == 1)
                    counter_str++;
                else if (flags->o == 0)
                    printf("%s", s);
            }
        }
        if (flags->c == 1) {
            if (support->files_count > 1) {
                printf("%s:%d\n", argv, counter_str);
            } else {
                printf("%d\n", counter_str);
            }
        }
        if (argc > 2) {
            if (s[strlen(s)-1] == EOF || s[strlen(s)-1] != '\n') {
                if (flags->i == 1 || flags->v == 1) {
                    printf("\n");
                }
            }
        }
        fclose(str);
    }
}

pcre *do_pcre_compile(char *pattern, s_list *flags) {  //  компиляция паттерна
    pcre *re = NULL;
    int options = 0;
    if (flags->i)
        options = PCRE_CASELESS;
    const char *error;
    int erroffset;
    re = pcre_compile(pattern, options, &error, &erroffset, NULL);
    return re;
}

void parse_files(int argc, char **argv, t_list *support) {
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {}
        else
            support->files_count++;
    }
    support->files_count -= 2;
}

void parse_flags(int argc, char **argv, s_list *flags, t_list *support) {
    int i;
    for (i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'e': flags->e = 1;
                        support->ptrn = argv[i+1];
                        break;
                    case 'i': flags->i = 1; break;
                    case 'v': flags->v = 1; break;
                    case 'c': flags->c = 1; break;
                    case 'l': flags->l = 1; break;
                    case 'n': flags->n = 1; break;
                    case 'h': flags->h = 1; break;
                    case 's': flags->s = 1; break;
                    case 'f': flags->f = 1;
                        support->ptrn = argv[i+1];
                        break;
                    case 'o': flags->o = 1; break;
                }
            }
        }
    }
}

char *find_pattern(int *pi, char **argv, t_list *support) {  //  найти паттерн (если нет флагов -еf)
    int i = 1;
    char *ptrn = NULL;
    for (i = 1; !ptrn; i++) {
        if (argv[i][0] != '-')
            ptrn = argv[i];
    }
    *pi = i;
    support->ptrn = ptrn;
    return (ptrn);
}

void check_argc(int argc, s_list *flags) {
    if (argc < 2 && flags->s == 0) {
        fprintf(stderr, "Usage: grep [OPTION]... PATTERNS [FILE]...\n");
        fprintf(stderr, "Try 'grep --help' for more information.\n");
        exit(1);
    }
}

int main(int argc, char **argv) {
    s_list flags = {0};
    t_list support = {0};
    pcre* pattern = 0;
    char *ptrn = NULL;
    int i = 0;
    check_argc(argc, &flags);
    parse_flags(argc, argv, &flags, &support);
    parse_files(argc, argv, &support);
    if (flags.f != 1 && flags.e != 1) {
        ptrn = find_pattern(&i, argv, &support);
        pattern = do_pcre_compile(ptrn, &flags);
    } else if (flags.e == 1) {
        i = 1;
        ptrn = support.ptrn;
        while (ptrn == argv[i] || argv[i][0] == '-') {
            i++;
        }
        pattern = do_pcre_compile(ptrn, &flags);
    } else if (flags.f == 1) {
        FILE *file_pattern = file_open(support.ptrn, flags.s);
        char s[1024];
        char *pattern_before_compile = (fgets(s, 80, file_pattern));
        pattern = do_pcre_compile((char*)pattern_before_compile, &flags);
    }
    if (pattern) {
        while (argv[i]) {
            if (argv[i] == support.ptrn ) {}
            else if (argv[i][0] != '-' && i != 0)
                do_pcre_exec(pattern, argc, argv[i], &flags, &support);
            i++;
        }
        pcre_free(pattern);
    }
    return 0;
}
