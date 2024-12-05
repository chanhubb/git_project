#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h> // 추가된 헤더 파일

#define WIDTH 40
#define HEIGHT 10

int dino_pos = HEIGHT - 1;
int obstacle_pos = WIDTH - 1;
int score = 0;
int jump = 0;

void draw() {
    system("clear");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == dino_pos && j == 1) {
                printf("D");
            } else if (i == HEIGHT - 1) {
                printf("-");
            } else if (i == HEIGHT - 2 && j == obstacle_pos) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

void update() {
    if (jump > 0) {
        dino_pos--;
        jump--;
    } else if (dino_pos < HEIGHT - 1) {
        dino_pos++;
    }

    obstacle_pos--;
    if (obstacle_pos < 0) {
        obstacle_pos = WIDTH - 1;
        score++;
    }

    if (dino_pos == HEIGHT - 2 && obstacle_pos == 1) {
        printf("Game Over! Final Score: %d\n", score);
        exit(0);
    }
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    while (1) {
        if (kbhit()) {
            char c = getchar();
            if (c == ' ') {
                jump = 3;
            }
        }
        draw();
        update();
        usleep(100000);
    }
    return 0;
}