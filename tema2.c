// Copyright 2018 Damian Stefania-Dorina 312CD
#include <stdio.h>
#include <string.h>

// verify if the string is a number and return the number
int number(char v[]) {
    int i = 0, nr = 0, l = strlen(v);
    // invalid number
    if (l > 1 && v[i] == '0') {
        return -1;
    }
    while (i < l) {
        // invalid number
        if (v[i] < '0' || v[i] > '9') {
            return -1;
        } else {
            nr = nr * 10 + v[i] - '0';
        }
        i++;
    }
    return nr;
}

// convert number into string
void make_string(int n, char string[]) {
    strcpy(string, "");
    if (n == 0) {
        string[0] = '0';
        string[1] = '\0';
    } else {
        while (n > 0) {
            strcpy(string + 1, string);
            string[0] = '0' + n % 10;
            n /= 10;
        }
    }
}

// return lenght of the longest word
int longest_word_length(char original[1000][1000], int original_line_count) {
    char word[100] = "", *p;
    int i;
    for (i = 0; i < original_line_count; i++) {
        p = strtok(original[i], " ");
        while (p != NULL) {
            if (strlen(p) > strlen(word)) {
                strcpy(word, p);
            }
            p = strtok(NULL, " ");
        }
    }
    return strlen(word);
}

// return length of the longest line
int longest_line_length(char text[1000][1000], int line_count) {
    int i, l, lmax = strlen(text[0]);
    for (i = 1; i < line_count; i++) {
        l = strlen(text[i]);
        if (l > lmax) {
            lmax = l;
        }
    }
    return lmax;
}

// validate Wrap
int validation_W(char operation[], int param_nr[10][4], int n, int min) {
    char *p = strtok(operation, " ");
    // first parameter
    if (p == NULL) {
        return 0;
    } else {
        param_nr[n][0] = number(p);
        if (param_nr[n][0] == -1) {
            return 0;
        } else {
            // second parameter
            p = strtok(NULL, " ");
            if (p == NULL) {
                if (param_nr[n][0] >= min) {
                    return 1;
                } else {
                    printf("Cannot wrap!\n");
                    return -1;
                }
            } else {
                return 0;
            }
        }
    }
}

// validate Center Text, Align left/ right and Justify
void validation_CLRJ(char operation[], int param_nr[10][4], int n, char *p,
                    int nr_param, int *ok) {
    // the format is correct at the moment
    if (*ok == 1) {
        param_nr[n][nr_param + 1] = number(p);
        // 2 parameters were already found
        if (nr_param == 2) {
            *ok = 0;
        } else {
            // the numeric parameter is not valid
            if (param_nr[n][nr_param + 1] == -1) {
                *ok = 0;
            } else {
                // find the next parameter
                p = strtok(NULL, " ");
                if (p != NULL) {
                    validation_CLRJ(operation, param_nr, n, p,
                                    nr_param + 1, ok);
                }
            }
        }
    }
}

// validate Paragraphs
void validation_P(char operation[], int param_nr[10][4], int n, char *p,
                    int nr_param, int *ok) {
    // the format is correct at the moment
    if (*ok == 1) {
        // discussion on the number of parameters already found
        switch (nr_param) {
            case 0:    param_nr[n][3] = number(p);
                    if (param_nr[n][3] == -1) {
                        *ok = 0;
                    }
                    break;
            case 1:
            case 2: param_nr[n][nr_param] = number(p);
                    if (param_nr[n][nr_param] == -1) {
                        *ok = 0;
                    }
                    break;
            default: *ok = 0;
        }

        if (*ok == 1) {
            // find next parameter
            p = strtok(NULL, " ");
            if (p != NULL) {
                validation_P(operation, param_nr, n, p, nr_param + 1, ok);
            }
        }
    }
}

// separate the first n non-whitespace characters
void separate_n_param(char operation[], int n) {
    int i = 0;
    char aux[30];
    while (operation[i] == ' ' && operation + i != NULL) {
        strcpy(aux, operation+1);
        strcpy(operation, aux);
    }
    strcpy(aux, operation +1);
    strcpy(operation + 2, aux);
    operation[1] = ' ';
    i++;

    while (operation[i] == ' ' && operation + i != NULL) {
        i++;
    }
    strcpy(aux, operation +i+1);
    strcpy(operation + i + 2, aux);
    operation[i + 1] = ' ';

    if (n == 3) {
        i++;
        while (operation[i] == ' ' && operation + i != NULL) {
            i++;
        }
        strcpy(aux, operation +i+1);
        strcpy(operation + i + 2, aux);
        operation[i + 1] = ' ';
    }
}

// validate Lists
void validation_I(char operation[], char param_c[10][4], int param_nr[10][4],
                 int n, char *p, int nr_param, int *ok) {
    // the format is correct at the moment
    if (*ok == 1) {
        // discussion on the number of parameters already found
        switch (nr_param) {
            case 0:
                    if (p[0] == 'n' || p[0] == 'a'
                        || p[0] == 'A' || p[0] == 'b') {
                        param_c[n][1] = p[0];
                    } else {
                        *ok = 0;
                    }
                    break;
            case 1:
                    if ((p[0] >= 'A' && p[0] <= 'z')
                        || (p[0] >= '0' && p[0] <= '9')) {
                        *ok = 0;
                    } else {
                        param_c[n][2] = p[0];
                    }
                    break;
            case 2:
            case 3: param_nr[n][nr_param - 1] = number(p);
                    // the numeric parameter in not valid
                    if (param_nr[n][nr_param - 1] == -1) {
                        *ok = 0;
                    }
                    break;
            default: *ok = 0;
        }
        if (*ok == 1) {
            // find next parameter
            p = strtok(NULL, " ");
            if (p == NULL) {
                if (nr_param < 1) {
                    *ok = 0;
                }
            } else {
                validation_I(operation, param_c, param_nr, n, p,
                            nr_param + 1, ok);
            }
        }
    }
}

// validate Ordered Lists
void validation_O(char operation[], char param_c[10][4], int param_nr[10][4],
                    int n, char *p, int nr_param, int *ok) {
    // the format is correct at the moment
    if (*ok == 1) {
        // discussion on the number of parameters already found
        switch (nr_param) {
            case 0:
                    if (p[0] == 'n' || p[0] == 'a'
                        || p[0] == 'A' || p[0] == 'b') {
                        param_c[n][1] = p[0];
                    } else {
                        *ok = 0;
                    }
                    break;
            case 1:
                    if ((p[0] >= 'A' && p[0] <= 'z')
                        || (p[0] >= '0' && p[0] <= '9')) {
                        *ok = 0;
                    } else {
                        param_c[n][2] = p[0];
                    }
                    break;
            case 2:
                    if (p[0] == 'a' || p[0] == 'z') {
                        param_c[n][3] = p[0];
                    } else {
                        *ok = 0;
                    }
                    break;
            case 3:
            case 4:
                    param_nr[n][nr_param - 2] = number(p);
                    // the numeric parameter is not valid
                    if (param_nr[n][nr_param - 2] == -1) {
                        *ok = 0;
                    }
                    break;
            default: *ok = 0;
        }
        if (*ok == 1) {
            // find next parameter
            p = strtok(NULL, " ");
            if (p == NULL) {
                if (nr_param < 2) {
                    *ok = 0;
                }
            } else {
                validation_O(operation, param_c, param_nr, n, p,
                            nr_param + 1, ok);
            }
        }
    }
}

// verify if the operation has the correct format
int validate_op_type(char operation[], int param_nr[10][4],
                    char param_c[10][4], int n, int min) {
    char *p, aux[30];
    strcpy(aux, operation + 1);
    int ok = 1;
    switch (operation[0]) {
            case 'w':
            case 'W':
                    param_c[n][0] = 'W';
                    ok = validation_W(aux, param_nr, n, min);
                    break;
            case 'c':
            case 'C':
                    p = strtok(operation + 1, " ");
                    param_c[n][0] = 'C';
                    if (p != NULL) {
                        validation_CLRJ(aux, param_nr, n, p, 0, &ok);
                    }
                    break;
            case 'l':
            case 'L':
                    p = strtok(operation + 1, " ");
                    param_c[n][0] = 'L';
                    if (p != NULL) {
                        validation_CLRJ(aux, param_nr, n, p, 0, &ok);
                    }
                    break;
            case 'r':
            case 'R':
                    p = strtok(aux, " ");
                    param_c[n][0] = 'R';
                    if (p != NULL) {
                        validation_CLRJ(aux, param_nr, n, p, 0, &ok);
                    }
                    break;
            case 'j':
            case 'J':
                    p = strtok(aux, " ");
                    param_c[n][0] = 'J';
                    if (p != NULL) {
                        validation_CLRJ(aux, param_nr, n, p, 0, &ok);
                    }
                    break;
            case 'p':
            case 'P':
                    param_c[n][0] = 'P';
                    p = strtok(aux, " ");
                    if (p != NULL) {
                        validation_P(aux, param_nr, n, p, 0, &ok);
                    } else {
                        ok = 0;
                    }
                    break;
            case 'i':
            case 'I':
                    param_c[n][0] = 'I';
                    separate_n_param(aux, 2);
                    if (operation + 1 != NULL) {
                        p = strtok(aux, " ");
                        validation_I(aux, param_c, param_nr,
                                    n, p, 0, &ok);
                    } else {
                        ok = 0;
                    }
                    break;
            case 'o':
            case 'O':
                    param_c[n][0] = 'O';
                    separate_n_param(aux, 3);
                    if (operation + 1 != NULL) {
                        p = strtok(aux, " ");
                        validation_O(aux, param_c, param_nr,
                                   n, p, 0, &ok);
                    } else {
                        ok = 0;
                    }
                    break;
            default: return 0;
    }
    return ok;
}

// verify if the operation is valid
int validate_operation(char operation[], int param_nr[10][4],
                        char param_c[10][4], int n, int min) {
    char aux[30];
    while (operation[0] == ' ' && operation != NULL) {
        strcpy(aux, operation +1);
        strcpy(operation, aux);
    }
    if (operation != NULL) {
        param_nr[n][1] = 0;
        param_nr[n][2] = 999;
        return validate_op_type(operation, param_nr, param_c, n, min);
    } else {
        // the operation is empty
        param_c[n][0] = ' ';
        return 2;
    }
}

// validate all operations
int validate_operations(char string[], int param_nr[10][4],
                        char param_c[10][4], int *n, int min) {
    char *p, aux[10][20];
    int ok = 1, i;
    *n = 0;
    p = strtok(string, ",");
    while (p != NULL && ok == 1) {
        if (strlen(p) > 20) {
            printf("Invalid operation!\n");
            ok = 0;
        } else {
            if (*n < 10) {
                strcpy(aux[*n], p);
                *n = *n + 1;
            } else {
                printf("Too many operations! "
                    "Only the first 10 will be applied.\n");
                break;
            }
        }
        p = strtok(NULL, ",");
    }

    for (i = 0; i < *n && ok == 1; i++) {
        ok = validate_operation(aux[i], param_nr, param_c, i, min);
        if (ok == 0) {
            printf("Invalid operation!\n");
        }
        if (ok == 1 && param_c[i][0] == 'W' && param_nr[i][0] < min) {
            printf("Cannot wrap!\n");
            ok = 0;
        }
        if (ok == 1 && param_nr[i][1] > param_nr[i][2]) {
            printf("Invalid operation!\n");
            ok = 0;
        }
        if (ok == 1 && param_nr[i][1] < 0) {
            ok = 0;
        }
    }
    return ok;
}

// eliminate trailing whitespaces
void no_trailing_whitespaces(char text[1000]) {
    int l;
    char aux[1000];
    l = strlen(text) - 1;
    if (text[l] == '\n') {
        l--;
    }
    while ((text[l] == ' ' || text[l] == '\t') && l >= 0) {
        strcpy(aux, text + l + 1);
        strcpy(text + l, aux);
        l--;
    }
}

// eliminate starting whitespaces
void no_starting_whitespaces(char text[1000]) {
    char aux[1000];
    while ((text[0] == ' ' || text[0] == '\t') && strlen(text) > 0) {
        strcpy(aux, text +  1);
        strcpy(text, aux);
    }
}

// complete the lines with the text in buffer while buffer is longer
// than needed
void complete_lines(char original[1000][1000], int original_line_count,
                    char result[1000][1000], int *result_line_count,
                    int max_length, char buffer[]) {
    int l = strlen(buffer), j;
    char aux[1000];
    while (l > max_length) {
        j = max_length;
        while (buffer[j] != ' ' && buffer[j] != '\t') {
            j--;
        }

        strncpy(aux, buffer, j + 1);
        aux[j] = '\n';
        aux[j + 1] = '\0';
        strcpy(result[*result_line_count], aux);
        no_trailing_whitespaces(result[*result_line_count]);
        *result_line_count += 1;

        if (strlen(aux) == strlen(buffer)) {
            buffer[0] = '\0';
        } else {
            strcpy(aux, buffer + strlen(aux));
            strcpy(buffer, aux);
        }
        l = strlen(buffer);
    }
}

// apply Wrap
void Wrap(char original[1000][1000], int original_line_count,
            char result[1000][1000], int *result_line_count, int max_length) {
    char buffer[2000] = "", line[1000];
    int i, l;
    *result_line_count = 0;
    for (i = 0; i < original_line_count; i++) {
        strcpy(line, original[i]);

        // we have an empty line
        if (strcmp(line, "") == 0 || strcmp(line, "\n") == 0) {
            // if buffer is not empty, complete result line
            // with buffer and then add the empty line
            if (strlen(buffer) > 0) {
                strcpy(result[*result_line_count], buffer);
                strcat(result[*result_line_count], "\n");
                *result_line_count += 1;
                buffer[0] = '\0';
            }
            strcpy(result[*result_line_count], "\n");
            no_trailing_whitespaces(result[*result_line_count]);
            *result_line_count += 1;
        } else {
            // concatenate lines
            if (strlen(buffer) > 0) {
                no_trailing_whitespaces(buffer);
                no_starting_whitespaces(line);
                l = strlen(line);
                if (line[l - 1] == '\n') {
                    line[l - 1] = '\0';
                }
                strcat(buffer, " ");
                strcat(buffer, line);
            } else {
                strcpy(buffer, line);
                l = strlen(buffer);
                if (buffer[l - 1] == '\n') {
                    buffer[l - 1] = '\0';
                }
            }
            complete_lines(original, original_line_count, result,
                            result_line_count, max_length, buffer);
        }
    }

    // complete result line with what is left in buffer
    if (strlen(buffer) > 0) {
        strcpy(result[*result_line_count], buffer);
        strcat(result[*result_line_count], "\n");
        no_trailing_whitespaces(result[*result_line_count]);
        *result_line_count += 1;
    }
}

// apply Center text
void Center_text(char text[1000][1000], int start_line,
                int end_line, int length) {
    int nr, i;
    char aux[1000];
    for (i = start_line; i <= end_line; i++) {
        if (strlen(text[i]) > 1) {
            // number of spaces needed to add
            nr = (length - strlen(text[i])) / 2 +
                (length - strlen(text[i])) % 2;
            strcpy(aux, text[i]);
            strcpy(text[i] + nr, aux);
            memset(text[i], ' ', nr);
        }
    }
}

// apply Align left
void Align_left(char text[1000][1000], int start_line, int end_line) {
    int i;
    for (i = start_line; i <= end_line; i++) {
        no_starting_whitespaces(text[i]);
    }
}

// apply Align right
void Align_right(char text[1000][1000], int start_line,
                int end_line, int length) {
    int i, nr, l;
    char aux[1000];
    for (i = start_line; i <= end_line; i++) {
        l = strlen(text[i]);
        if (l > 1) {
            // number of spaces needed to add
            nr = length - l;
            strcpy(aux, text[i]);
            strcpy(text[i]+ nr, aux);
            memset(text[i], ' ', nr);
        }
    }
}

// eliminate extra whitespaces
void elim_extra_spaces(char text[1000]) {
    int j, l;
    char *p, aux[1000];

    no_trailing_whitespaces(text);
    no_starting_whitespaces(text);

    // replace tabs with whitespaces
    for (j = 0; j < strlen(text); j++) {
        if (text[j] == '\t') {
            strcpy(aux, text + j + 1);
            text[j] = ' ';
            text[j + 1] = ' ';
            text[j + 2] = ' ';
            text[j + 3] = ' ';
            text[j + 4] = '\0';
            strcat(text, aux);
        }
    }

    // erase extra whitespaces
    p = strchr(text, ' ');
    while (p != NULL) {
        l = p - text;
        while (*(text + l + 1) == ' ' && l < strlen(text) - 1) {
            strcpy(aux, text + l + 2);
            strcpy(text + l + 1, aux);
        }
        p = strchr(p + 1, ' ');
    }
}

// add one whitespace between every 2 words
void add_spaces(char text[1000], int initial_nr_spaces) {
    char *p, aux[1000];
    int l;
    p = strchr(text, ' ');
    while (p != NULL) {
        l = p - text;
        strcpy(aux, p);
        strcpy(text + l + 1, aux);
        p = strchr(p + initial_nr_spaces + 1, ' ');
    }
}

// apply Justify
void Justify(char text[1000][1000], int line_count, int start_line,
            int end_line, int length) {
    int i, nr, l, nr_spaces_groups, nr_spaces_added, length_spaces_group;
    char aux[1000], *p;

    for (i = start_line; i <= end_line; i++) {
        no_starting_whitespaces(text[i]);
        // apply Justify if the line is not the last one
        // in the paragrapf and it is not empty
        if (i != line_count - 1 && strlen(text[i + 1]) > 1 &&
            strlen(text[i]) > 1) {
            no_starting_whitespaces(text[i]);
            elim_extra_spaces(text[i]);

            nr = length - strlen(text[i]);

            nr_spaces_groups = 0;
            p = strchr(text[i], ' ');
            while (p != NULL) {
                nr_spaces_groups++;
                p = strchr(p + 1, ' ');
            }

            nr_spaces_added = 0;
            length_spaces_group = 1;
            // add whitespaces in each whitespaces group
            while (nr_spaces_added + nr_spaces_groups <= nr) {
                add_spaces(text[i], length_spaces_group);
                length_spaces_group++;
                nr_spaces_added += nr_spaces_groups;
            }

            // add remaining whitespaces
            p = strchr(text[i], ' ');
            while (nr_spaces_added < nr) {
                l = p - text[i];
                strcpy(aux, p);
                strcpy(text[i] + l + 1, aux);
                nr_spaces_added++;
                p = strchr(p + length_spaces_group + 1, ' ');
            }
        } else {
            Align_left(text, i, i);
        }
    }
}

// apply Paragraphs
void Paragraphs(char text[1000][1000], int indent_length, int start_line,
                int end_line) {
    int i;
    char aux[1000];
    if (indent_length > 8) {
        indent_length = 8;
    }
    for (i = start_line; i <= end_line; i++) {
        // the first line in file
        if (i == 0) {
            no_starting_whitespaces(text[i]);
            strcpy(aux, text[i]);
            strcpy(text[i] + indent_length, aux);
            memset(text[i], ' ', indent_length);
        } else {
            // the first line in paragraph
            if (strcmp(text[i - 1], "\n") == 0) {
                no_starting_whitespaces(text[i]);
                strcpy(aux, text[i]);
                strcpy(text[i] + indent_length, aux);
                memset(text[i], ' ', indent_length);
            }
        }
        no_trailing_whitespaces(text[i]);
    }
}

// apply Lists
void Lists(char text[1000][1000], char list_type, char special_character,
            int start_line, int end_line) {
    int i, nr = 1, l;
    char aux[1000], numbering[4];
    Align_left(text, start_line, end_line);

    for (i = start_line; i <= end_line; i++) {
        switch (list_type) {
            case 'b':
                    aux[0] = special_character;
                    aux[1] = ' ';
                    aux[2] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
            case 'n':
                    make_string(nr, numbering);
                    strcpy(aux, numbering);
                    l = strlen(aux);
                    aux[l] = special_character;
                    aux[l + 1] = ' ';
                    aux[l + 2] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
            case 'a':
                    aux[0] = 'a' + nr - 1;
                    aux[1] = special_character;
                    aux[2] = ' ';
                    aux[3] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
            case 'A':
                    aux[0] = 'A' + nr - 1;
                    aux[1] = special_character;
                    aux[2] = ' ';
                    aux[3] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
        }
        nr++;
    }
}

// swap lines
void swap(char text[1000][1000], int i, int j) {
    char aux[1000];
    strcpy(aux, text[i]);
    strcpy(text[i], text[j]);
    strcpy(text[j], aux);
}

// find pivot for alphabetically ordered lists
int pivot(char text[1000][1000], int start, int end) {
    int i, j;
    char x[1000];
    i = start + 1;
    j = end;
    strcpy(x, text[start]);
    while (i <= j) {
        if (strcmp(text[i], x) <= 0) {
            i++;
        }
        if (strcmp(text[j], x) > 0) {
            j--;
        }
        if (strcmp(text[i], x) > 0 && strcmp(text[j], x) <= 0 && i < j) {
            swap(text, i, j);
            i++;
            j--;
        }
    }
    swap(text, start, i - 1);
    return i - 1;
}

// sort lines for alphabetically ordered lists
void quicksort(char text[1000][1000], int start, int end) {
    int p;
    p = pivot(text, start, end);
    if (start < p - 1) {
        quicksort(text, start, p - 1);
    }
    if (p + 1 < end) {
        quicksort(text, p + 1, end);
    }
}

// find pivot for reversed alphabetically ordered lists
int reverse_pivot(char text[1000][1000], int start, int end) {
    int i, j;
    char x[1000];
    i = start + 1;
    j = end;
    strcpy(x, text[start]);
    while (i <= j) {
        if (strcmp(text[i], x) >= 0) {
            i++;
        }
        if (strcmp(text[j], x) < 0) {
            j--;
        }
        if (strcmp(text[i], x) < 0 && strcmp(text[j], x) >= 0 && i < j) {
            swap(text, i, j);
            i++;
            j--;
        }
    }
    swap(text, start, i - 1);
    return i - 1;
}

// sort lines for reversed alphabetically ordered lists
void reverse_quicksort(char text[1000][1000], int start, int end) {
    int p;
    p = reverse_pivot(text, start, end);
    if (start < p - 1) {
        reverse_quicksort(text, start, p - 1);
    }
    if (p + 1 < end) {
        reverse_quicksort(text, p + 1, end);
    }
}

// apply Ordered lists
void Ordered_lists(char text[1000][1000], char list_type,
                    char special_character, char ordering,
                    int start_line, int end_line) {
    int i, nr = 1, l;
    char aux[1000], numbering[4];

    // sort specified lines
    switch (ordering) {
        case 'a':
                quicksort(text, start_line, end_line);
                break;
        case 'z':
                reverse_quicksort(text, start_line, end_line);
                break;
    }

    Align_left(text, start_line, end_line);

    for (i = start_line; i <= end_line; i++) {
        switch (list_type) {
            case 'b':
                    aux[0] = special_character;
                    aux[1] = ' ';
                    aux[2] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
            case 'n':
                    make_string(nr, numbering);
                    strcpy(aux, numbering);
                    l = strlen(aux);
                    aux[l] = special_character;
                    aux[l + 1] = ' ';
                    aux[l + 2] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
            case 'a':
                    aux[0] = 'a' + nr - 1;
                    aux[1] = special_character;
                    aux[2] = ' ';
                    aux[3] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
            case 'A':
                    aux[0] = 'A' + nr - 1;
                    aux[1] = special_character;
                    aux[2] = ' ';
                    aux[3] = '\0';
                    strcat(aux, text[i]);
                    strcpy(text[i], aux);
                    break;
        }
        nr++;
    }
}

// apply the operation
void apply_operation(char original[1000][1000], int original_line_count,
                    char result[1000][1000], int *result_line_count,
                    int param_nr[10][4], char param_c[10][4], int i) {
    char aux[1000];
    int j, line_max_length = longest_line_length(result, *result_line_count);

    for (j = 0; j < *result_line_count; j++) {
        no_trailing_whitespaces(result[j]);
    }

    switch (param_c[i][0]) {
        case 'W':
                for (j = 0; j < *result_line_count; j++) {
                    strcpy(aux, result[j]);
                    strcpy(original[j], aux);
                }
                original_line_count = *result_line_count;
                Wrap(original, original_line_count, result, result_line_count,
                    param_nr[i][0]);
                break;
        case 'C':
                Center_text(result, param_nr[i][1], param_nr[i][2],
                            line_max_length);
                break;
        case 'L':
                Align_left(result, param_nr[i][1], param_nr[i][2]);
                break;
        case 'R':
                Align_right(result, param_nr[i][1], param_nr[i][2],
                            line_max_length);
                break;
        case 'J':
                Justify(result, *result_line_count, param_nr[i][1],
                        param_nr[i][2], line_max_length);
                break;
        case 'P':
                Paragraphs(result, param_nr[i][3], param_nr[i][1],
                           param_nr[i][2]);
                break;
        case 'I':
                Lists(result, param_c[i][1], param_c[i][2], param_nr[i][1],
                        param_nr[i][2]);
                break;
        case 'O':
                Ordered_lists(result, param_c[i][1], param_c[i][2],
                            param_c[i][3], param_nr[i][1], param_nr[i][2]);
                break;
    }
}

int main(int argc, char *argv[]) {
    char buf[1000], original[1000][1000], result[1000][1000],
    param_c[10][4];
    int original_line_count = 0, result_line_count = 0, i, j,
    nr_operations = 0, param_nr[10][4], valid, longest_word;

    if (argc != 4) {
        fprintf(stderr,
            "Usage: %s operations_string input_file output_file\n", argv[0]);
        return -1;
    }

      FILE *input_file = fopen(argv[2], "r");
    if (input_file == NULL) {
        fprintf(stderr, "File \"%s\" not found\n", argv[2]);
        return -2;
    }

    // Read data in file line by line
     while (fgets(buf, 1000, input_file)) {
        strcpy(original[original_line_count], buf);
        strcpy(result[original_line_count], original[original_line_count]);
        original_line_count++;
    }
    result_line_count = original_line_count;

    fclose(input_file);

    FILE *output_file = fopen(argv[3], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Could not open or create file \"%s\"\n", argv[3]);
        return -2;
    }

    longest_word = longest_word_length(original, original_line_count);
    valid = validate_operations(argv[1], param_nr, param_c,
                                &nr_operations, longest_word);
    
    if (valid == 1) {
        for (i = 0; i < nr_operations; i++) {
            if (param_nr[i][2] > result_line_count - 1) {
                param_nr[i][2] = result_line_count - 1;
            }

            apply_operation(original, original_line_count, result,
                            &result_line_count, param_nr, param_c, i);
            for (j = 0; j < result_line_count; j++) {
                no_trailing_whitespaces(result[j]);
            }
        }
    } else {
        // the operation is empty ("")
        if (valid == 2) {
            for (j = 0; j < result_line_count; j++) {
                no_trailing_whitespaces(result[j]);
            }
        }
    }
    

    for (i = 0; i < result_line_count; i++) {
        fputs(result[i], output_file);
    }

    fclose(output_file);
    return 0;
}
