#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/utime.h>

#define PROGRAM_NAME "boop"
#define PROGRAM_VERSION "0.1.1"

typedef enum { TRUE, FALSE } HelpFlag;

static int check_exists;

static int remove_file;

static int change_adate;

static int change_mdate;

static char *timestamp;

void print_usage(HelpFlag hf_val) {
    printf("usage: %s [ARGUMENT] FILE ...\n", PROGRAM_NAME);

    if (hf_val == TRUE) {
        printf("\n\t-c\n\t\tcheck if file exist\n");
        printf("\n\t-r\n\t\tremove if file exist\n");
        printf("\n\t-a\n\t\tchange the file access time\n");
        printf("\n\t-m\n\t\tchange the file modification time\n");
        printf("\n\t-t STAMP\n\t\tuse timestamp in format YYYYMMDDhhmm\n");
        printf("\n\t--help\n\t\tshow this help message and exit\n");
        printf("\n\t--version\n\t\tshow version information and exit\n");
    }
}

void boop(char *filename) {
    if (remove_file) {
        int err = remove(filename);

        if (err) {
            print_usage(TRUE);
            exit(EXIT_FAILURE);
        }

        return;
    }

    FILE *fp;
    if (check_exists || (change_adate && !change_mdate)) {
        // open in read to not change modification date
        fp = fopen(filename, "r");
    }
    else {
        fp = fopen(filename, "w");
    }

    if (fp) {
        if (check_exists) {
            printf("file %s does exist\n", filename);
            return;
        }

        if (timestamp) {
            time_t epoch_a;
            time_t epoch_m;
            struct utimbuf ubuf;
            struct tm tm_a = {0};
            struct tm tm_m = {0};

            char year[5] = {0};
            char month[3] = {0};
            char day[3] = {0};
            char hour[3] = {0};
            char minute[3] = {0};

            strncpy(year, timestamp, 4);
            strncpy(month, (timestamp + 4), 2);
            strncpy(day, (timestamp + 6), 2);
            strncpy(hour, (timestamp + 8), 2);
            strncpy(minute, (timestamp + 10), 2);

            tm_a.tm_year = tm_m.tm_year = atoi(year) - 1900;
            tm_a.tm_mon = tm_m.tm_mon = atoi(month) - 1;
            tm_a.tm_mday = tm_m.tm_mday = atoi(day);
            tm_a.tm_hour = tm_m.tm_hour = atoi(hour);
            tm_a.tm_min = tm_m.tm_min = atoi(minute);

            epoch_a = mktime(&tm_a);
            epoch_m = mktime(&tm_m);

            ubuf.actime = epoch_a;
            ubuf.modtime = epoch_m;

            utime(filename, &ubuf);
        }
    }
    else {
        if (check_exists) {
            printf("file %s does NOT exist\n", filename);
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(FALSE);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        if (strcmpi("--help", *(argv + 1)) == 0) {
            print_usage(TRUE);
            exit(EXIT_SUCCESS);
        }
        else if (strcmpi("--version", *(argv + 1)) == 0) {
            printf("%s version %s", PROGRAM_NAME, PROGRAM_VERSION);
            exit(EXIT_SUCCESS);
        }
    }

    int i;
    for (i = 1; i < argc; i++) {
        // assume after first argument not starting with '-' is a filename
        if (*(*(argv + i)) != '-') {
            break;
        }

        if (strcmpi("-c", *(argv + i)) == 0) {
            check_exists = 1;
        }
        else if (strcmpi("-r", *(argv + i)) == 0) {
            remove_file = 1;
        }
        else if (strcmpi("-a", *(argv + i)) == 0) {
            change_adate = 1;
        }
        else if (strcmpi("-m", *(argv + i)) == 0) {
            change_mdate = 1;
        }
        else if (strcmpi("-t", *(argv + i)) == 0) {
            i++;
            // if argument after '-t' is another flag exit
            if (*(*(argv + i)) == '-') {
                print_usage(TRUE);
                exit(EXIT_FAILURE);
            }

            timestamp = (char *)malloc(sizeof(argv[i]));
            timestamp = argv[i];
        }
        else {
            print_usage(TRUE);
            exit(EXIT_FAILURE);
        }
    }

    for (; i < argc; i++) {
        boop(*(argv + i));
    }

    free(timestamp);

    return EXIT_SUCCESS;
}
