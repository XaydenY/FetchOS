#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <time.h>

// colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define DOG_COLOR "\033[33m" 
const char *dog[] = {
    "    / \\__         ",
    "   (    @\\___     ",
    "   /         O    ",
    "  /   (_____/     ",
    " /_____/   U      ",
    "                  ",
    "                  ",
};

#define DOG_LINES 7

void get_os(char *buf, size_t len) {
    FILE *f = fopen("/etc/os-release", "r");
    if (!f) { strncpy(buf, "FetchOS", len); return; }
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            char *start = strchr(line, '"');
            if (start) {
                start++;
                char *end = strchr(start, '"');
                if (end) *end = '\0';
                strncpy(buf, start, len);
                fclose(f);
                return;
            }
        }
    }
    fclose(f);
    strncpy(buf, "FetchOS", len);
}

void get_kernel(char *buf, size_t len) {
    struct utsname u;
    if (uname(&u) == 0) {
        strncpy(buf, u.release, len);
    } else {
        strncpy(buf, "Unknown", len);
    }
}

void get_uptime(char *buf, size_t len) {
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        long hours = si.uptime / 3600;
        long mins  = (si.uptime % 3600) / 60;
        snprintf(buf, len, "%ldh %ldm", hours, mins);
    } else {
        strncpy(buf, "Unknown", len);
    }
}


void get_cpu(char *buf, size_t len) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) { strncpy(buf, "Unknown", len); return; }
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *colon = strchr(line, ':');
            if (colon) {
                colon += 2; 
                colon[strcspn(colon, "\n")] = '\0';
                strncpy(buf, colon, len);
                fclose(f);
                return;
            }
        }
    }
    fclose(f);
    strncpy(buf, "Unknown", len);
}


void get_ram(char *buf, size_t len) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) { strncpy(buf, "Unknown", len); return; }
    long total = 0, available = 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "MemTotal:", 9) == 0)
            sscanf(line, "MemTotal: %ld kB", &total);
        if (strncmp(line, "MemAvailable:", 13) == 0)
            sscanf(line, "MemAvailable: %ld kB", &available);
    }
    fclose(f);
    long used = total - available;
    snprintf(buf, len, "%ldMB / %ldMB", used / 1024, total / 1024);
}

void get_packages(char *buf, size_t len) {
    FILE *f = popen("dpkg -l 2>/dev/null | grep -c '^ii'", "r");
    if (!f) { strncpy(buf, "Unknown", len); return; }
    int count = 0;
    fscanf(f, "%d", &count);
    pclose(f);
    snprintf(buf, len, "%d (dpkg)", count);
}


void get_shell(char *buf, size_t len) {
    char *shell = getenv("SHELL");
    if (shell) {

        char *name = strrchr(shell, '/');
        strncpy(buf, name ? name + 1 : shell, len);
    } else {
        strncpy(buf, "Unknown", len);
    }
}

void get_hostname(char *buf, size_t len) {
    gethostname(buf, len);
}


void get_username(char *buf, size_t len) {
    char *user = getenv("USER");
    if (user) strncpy(buf, user, len);
    else strncpy(buf, "goodboy", len);
}

int main() {
    char os[128], kernel[128], uptime[64], cpu[128];
    char ram[64], packages[64], shell[64];
    char hostname[64], username[64];

    get_os(os, sizeof(os));
    get_kernel(kernel, sizeof(kernel));
    get_uptime(uptime, sizeof(uptime));
    get_cpu(cpu, sizeof(cpu));
    get_ram(ram, sizeof(ram));
    get_packages(packages, sizeof(packages));
    get_shell(shell, sizeof(shell));
    get_hostname(hostname, sizeof(hostname));
    get_username(username, sizeof(username));


    char *labels[] = {
        "\xF0\x9F\x90\x95 Breed   ",  // 🐕
        "\xF0\x9F\xA6\xB4 Bones   ",  // 🦴
        "\xE2\x8F\xB0 Walkies ",      // ⏰
        "\xF0\x9F\x93\xA6 Treats  ",  // 📦
        "\xF0\x9F\x90\xBE Paws    ",  // 🐾
        "\xE2\x9A\xA1 Strength",      // ⚡
        "\xF0\x9F\x92\xA7 Drool   ",  // 💧
    };

    char *values[] = {
        os,
        kernel,
        uptime,
        packages,
        shell,
        cpu,
        ram,
    };


    printf("\n");
    printf(BOLD YELLOW "  %s" RESET BOLD WHITE "@" RESET BOLD YELLOW "%s\n" RESET, username, hostname);
    printf(CYAN "  ══════════════════════════════════════════\n" RESET);


    for (int i = 0; i < DOG_LINES; i++) {
        printf(DOG_COLOR "%s" RESET, dog[i]);
        if (i < 7) {
            printf(CYAN "%s" RESET ": %s\n", labels[i], values[i]);
        } else {
            printf("\n");
        }
    }


    printf("\n  ");
    const char *colors[] = {
        "\033[41m", "\033[42m", "\033[43m",
        "\033[44m", "\033[45m", "\033[46m",
        "\033[47m", "\033[0m"
    };
    for (int i = 0; i < 7; i++) {
        printf("%s   ", colors[i]);
    }
    printf(RESET "\n\n");
    printf(YELLOW "  🐶 Woof! Have a good day!\n\n" RESET);

    return 0;
}