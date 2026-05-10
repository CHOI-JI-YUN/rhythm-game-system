#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <cwchar>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

#define UP_KEY 72
#define DOWN_KEY 80
#define LEFT_KEY 75
#define RIGHT_KEY 77
#define SPACE_KEY 32

struct Note {
    int x;
    int type;      // 0: 위, 1: 아래, 2: 왼쪽, 3: 오른쪽, 4: 스페이스
    bool isActive;
};

struct BeatNote {
    int timeMs;     // 음악 시작 후 몇 ms에 타격해야 하는지
    int type;       // 0:위, 1:아래, 2:왼쪽, 3:오른쪽, 4:스페이스
    bool spawned;
};

void gotoxy(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearText(int x, int y, int len) {
    gotoxy(x, y);
    for (int i = 0; i < len; i++) cout << ' ';
}

void printWideAt(int x, int y, const wchar_t* text, int color = 15) {
    gotoxy(x, y);
    setColor(color);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    WriteConsoleW(hOut, text, (DWORD)wcslen(text), &written, NULL);

    setColor(15);
}

void showStartUI() {
    system("cls");

    gotoxy(20, 3);
    setColor(11);
    cout << "==============================================";

    gotoxy(28, 4);
    cout << "       MUSIC TOKTOK GAME";

    gotoxy(20, 5);
    cout << "==============================================";

    setColor(15);
    gotoxy(24, 8);
    cout << "   방향키 + SPACE로 노트를 맞추세요";

    setColor(10);
    gotoxy(28, 11);
    cout << "      <- [LEFT]";

    setColor(14);
    gotoxy(28, 12);
    cout << "      ^  [UP]";

    setColor(13);
    gotoxy(28, 13);
    cout << "      v  [DOWN]";

    setColor(11);
    gotoxy(28, 14);
    cout << "      -> [RIGHT]";

    setColor(9);
    gotoxy(28, 15);
    cout << "      SP [SPACE BAR]";

    setColor(15);
    gotoxy(22, 19);
    cout << "   아무 키나 누르면 게임을 시작합니다...";
    _getch();

    system("cls");
}

int selectDifficulty() {
    while (true) {
        system("cls");

        setColor(11);
        gotoxy(24, 4);  cout << "========================================";
        gotoxy(30, 5);  cout << "     SELECT DIFFICULTY";
        gotoxy(24, 6);  cout << "========================================";

        setColor(10);
        gotoxy(34, 10); cout << "    1. EASY";

        setColor(14);
        gotoxy(34, 12); cout << "    2. NORMAL";

        setColor(12);
        gotoxy(34, 14); cout << "    3. HARD";

        setColor(15);
        gotoxy(20, 19); cout << "   플레이할 난이도 번호(1~3)를 눌러주세요.";

        int key = _getch();

        if (key == '1') return 1;
        if (key == '2') return 2;
        if (key == '3') return 3;
    }
}

void showStageClearScreen(int stage) {
    system("cls");

    setColor(11);
    gotoxy(28, 6);  cout << "========================================";
    gotoxy(32, 7);  cout << "        STAGE " << stage << " CLEAR!";
    gotoxy(28, 8);  cout << "========================================";

    setColor(14);
    gotoxy(24, 13); cout << " 아무 키나 누르면 다음 스테이지로 이동합니다...";
    _getch();
}

void showFinalClearScreen(int totalScore, int maxCombo) {
    for (int frame = 0; frame < 25; frame++) {
        system("cls");

        setColor(11);
        gotoxy(28, 3);  cout << "========================================";
        gotoxy(33, 4);  cout << "          FINAL CLEAR!";
        gotoxy(28, 5);  cout << "========================================";

        setColor(15);
        gotoxy(34, 8);  cout << "TOTAL SCORE : " << totalScore;
        gotoxy(34, 10); cout << "MAX COMBO   : " << maxCombo;

        setColor(14);
        gotoxy(24, 14); cout << "        Congratulations! All stages cleared!";
        gotoxy(30, 16); cout << "      Fantastic Rhythm Master!";

        setColor(12 + (frame % 4));
        gotoxy(15, 7);  cout << "   * ";
        gotoxy(14, 8);  cout << " * | * ";
        gotoxy(13, 9);  cout << "* |  *";
        gotoxy(14, 10); cout << " * | * ";
        gotoxy(15, 11); cout << "   * ";

        setColor(9 + (frame % 5));
        gotoxy(70, 6);  cout << "   * ";
        gotoxy(69, 7);  cout << " * \\ * ";
        gotoxy(68, 8);  cout << "* --O--*";
        gotoxy(69, 9);  cout << " * / * ";
        gotoxy(70, 10); cout << "   * ";

        setColor(10 + (frame % 5));
        gotoxy(45, 18); cout << "   * ";
        gotoxy(44, 19); cout << " * | * ";
        gotoxy(43, 20); cout << "*--*--*";
        gotoxy(44, 21); cout << " * | * ";
        gotoxy(45, 22); cout << "   * ";

        Sleep(180);
    }

    system("cls");

    setColor(11);
    gotoxy(28, 4);  cout << "========================================";
    gotoxy(33, 5);  cout << "             FINAL CLEAR!";
    gotoxy(28, 6);  cout << "========================================";

    setColor(15);
    gotoxy(34, 10); cout << "TOTAL SCORE : " << totalScore;
    gotoxy(34, 12); cout << "MAX COMBO   : " << maxCombo;

    setColor(14);
    gotoxy(26, 16); cout << "  축하합니다! 모든 스테이지를 클리어했습니다!";
    gotoxy(29, 18); cout << "            게임 종료!";
    gotoxy(28, 22); cout << "   아무 키나 누르면 종료합니다...";
    _getch();
}

bool showGameOverScreen(int totalScore, int maxCombo, int stage)
{
    int frame = 0;

    while (true)
    {
        system("cls");

        if (frame % 2 == 0) setColor(12);
        else setColor(8);

        gotoxy(28, 5);  cout << "========================================";
        gotoxy(38, 6);  cout << "    GAME OVER";
        gotoxy(28, 7);  cout << "========================================";

        setColor(15);
        gotoxy(34, 11); cout << "      FAILED STAGE: " << stage;
        gotoxy(34, 13); cout << "      TOTAL SCORE : " << totalScore;
        gotoxy(34, 15); cout << "      MAX COMBO   : " << maxCombo;

        if (frame % 2 == 0) setColor(12);
        else setColor(7);

        gotoxy(30, 18); cout << "            MISSION FAILED";
        gotoxy(26, 20); cout << "            R : RESTART    Q : EXIT";

        setColor((frame % 2 == 0) ? 12 : 14);
        int offset = (frame % 2 == 0) ? 0 : 1;

        gotoxy(12 + offset, 9);  cout << "XX      XX";
        gotoxy(14 + offset, 10); cout << "XX    XX";
        gotoxy(16 + offset, 11); cout << "XX  XX";
        gotoxy(18 + offset, 12); cout << "XXXX";
        gotoxy(16 + offset, 13); cout << "XX  XX";
        gotoxy(14 + offset, 14); cout << "XX    XX";
        gotoxy(12 + offset, 15); cout << "XX      XX";

        gotoxy(73 + offset, 9);  cout << "  XXXXXX  ";
        gotoxy(73 + offset, 10); cout << " XX    XX ";
        gotoxy(73 + offset, 11); cout << "XX      XX";
        gotoxy(73 + offset, 12); cout << "XX      XX";
        gotoxy(73 + offset, 13); cout << "XX      XX";
        gotoxy(73 + offset, 14); cout << " XX    XX ";
        gotoxy(73 + offset, 15); cout << "  XXXXXX  ";

        setColor(4 + (frame % 2));
        gotoxy(42, 24); cout << "!!!";

        if (_kbhit())
        {
            int key = _getch();
            if (key == 'r' || key == 'R') return true;
            if (key == 'q' || key == 'Q') return false;
        }

        frame++;
        Sleep(120);
    }
}

void drawHeader()
{
    setColor(11);
    gotoxy(2, 1);  cout << "================ MUSIC TOKTOK ================";
    gotoxy(2, 10); cout << "------------------------------------------------------------";
}

void drawTargetLine(int targetX, int noteY) {
    setColor(11);
    gotoxy(targetX - 3, noteY - 1); cout << "=======";
    gotoxy(targetX - 1, noteY);     cout << "[ ]";
    gotoxy(targetX - 3, noteY + 1); cout << "=======";

    setColor(14);
    gotoxy(targetX - 10, noteY); cout << "HIT ->";
}

void drawPlayer() {
    setColor(10);
    gotoxy(2, 23);  cout << "PLAYER: ('u')o";
    gotoxy(12, 24); cout << "| ";
    gotoxy(11, 25); cout << "/|\\";
    gotoxy(11, 26); cout << "/ \\";
}

void drawDynamicUI(int totalScore, int stageScore, int combo, int maxCombo, int life, int speed, int stage, int targetScore, const string& modeName) {
    setColor(15);

    gotoxy(2, 3);  cout << "MODE        : " << modeName << "      ";
    gotoxy(2, 4);  cout << "TOTAL SCORE : " << totalScore << "      ";
    gotoxy(2, 5);  cout << "STAGE SCORE : " << stageScore << "      ";
    gotoxy(2, 6);  cout << "TARGET      : " << targetScore << "      ";
    gotoxy(2, 7);  cout << "COMBO       : " << combo << "      ";
    gotoxy(2, 8);  cout << "MAX COMBO   : " << maxCombo << "      ";

    gotoxy(2, 9);  cout << "LIFE        : ";
    clearText(16, 9, 30);
    for (int i = 0; i < life; i++) {
        printWideAt(16 + i * 2, 9, L"♥", 12);
    }

    gotoxy(38, 3); cout << "STAGE : " << stage << "    ";
    gotoxy(38, 4); cout << "SPEED : " << speed << " ms    ";

    setColor(8);
    gotoxy(2, 28);
    cout << "Controls : Arrow Keys + Space";
}

void drawJudgeNearTarget(const string& judgeText, int targetX, int noteY) {
    clearText(targetX + 2, noteY - 3, 12);
    gotoxy(targetX + 2, noteY - 3);

    if (judgeText == "PERFECT") setColor(10);
    else if (judgeText == "GOOD") setColor(14);
    else if (judgeText == "MISS") setColor(12);
    else setColor(15);

    cout << judgeText;
    setColor(15);
}

void drawSingleNote(const Note& note, int noteY) {
    if (!note.isActive) return;

    // ★ 노트를 3칸짜리 블록으로 크게 만들기 위해 x-1 위치부터 그립니다.
    gotoxy(note.x - 1, noteY);

    switch (note.type) {
    case 0: setColor(14); cout << "^"; break;
    case 1: setColor(13); cout << "v"; break;
    case 2: setColor(10); cout << "<"; break;
    case 3: setColor(11); cout << ">"; break;
    case 4: setColor(9);  cout << "SP"; break; // 스페이스바
    }
}

void eraseSingleNote(const Note& note, int noteY) {
    // ★ 커진 노트 크기에 맞춰 3칸을 공백으로 지웁니다.
    gotoxy(note.x - 1, noteY);
    cout << "   ";
}

void drawNotes(const vector<Note>& notes, int noteY) {
    for (const auto& note : notes) {
        drawSingleNote(note, noteY);
    }
}

void removeInactiveNotes(vector<Note>& notes) {
    notes.erase(
        remove_if(notes.begin(), notes.end(),
            [](const Note& n) { return !n.isActive; }),
        notes.end()
    );
}

void showReadyGo(int x, int y) {
    setColor(14);
    gotoxy(x, y - 1); cout << "=================";
    gotoxy(x, y);     cout << " R E A D Y . . . ";
    gotoxy(x, y + 1); cout << "=================";
    Sleep(2000);

    setColor(10);
    gotoxy(x, y - 1); cout << "=================";
    gotoxy(x, y);     cout << "    G O ! ! !    ";
    gotoxy(x, y + 1); cout << "=================";
    Sleep(2000);

    clearText(x, y - 1, 17);
    clearText(x, y, 17);
    clearText(x, y + 1, 17);
}

// 1. EASY 전용 맵
vector<BeatNote> getBaseBeatMap() {
    return {
        { 1000, 2, false }, { 1500, 0, false }, { 2000, 3, false }, { 2500, 1, false },
        { 3000, 2, false }, { 3500, 3, false }, { 4000, 0, false }, { 4500, 4, false },
        { 5200, 2, false }, { 5700, 0, false }, { 6200, 3, false }, { 6700, 1, false },
        { 7200, 2, false }, { 7700, 3, false }, { 8200, 0, false }, { 8700, 4, false },
        { 9400, 0, false }, { 9900, 1, false }, { 10400, 2, false }, { 10900, 3, false },
        { 11400, 0, false }, { 11900, 1, false }, { 12400, 2, false }, { 12900, 4, false },
        { 13600, 2, false }, { 14100, 3, false }, { 14600, 0, false }, { 15100, 1, false },
        { 15600, 2, false }, { 16100, 3, false }, { 16600, 4, false }, { 17100, 0, false },
        { 17800, 2, false }, { 18100, 0, false }, { 18400, 3, false }, { 18700, 1, false },
        { 19000, 2, false }, { 19300, 0, false }, { 19600, 3, false }, { 19900, 4, false },
        { 20400, 0, false }, { 20900, 2, false }, { 21400, 1, false }, { 21900, 3, false },
        { 22400, 0, false }, { 22900, 2, false }, { 23400, 1, false }, { 23900, 4, false },
        { 24600, 2, false }, { 25100, 3, false }, { 25600, 0, false }, { 26100, 1, false },
        { 26600, 2, false }, { 27100, 3, false }, { 27600, 0, false }, { 28100, 4, false },
        { 28800, 0, false }, { 29300, 1, false }, { 29800, 2, false }, { 30300, 3, false },
        { 30800, 0, false }, { 31300, 1, false }, { 31800, 2, false }, { 32300, 4, false }
    };
}

// 2. NORMAL 전용 맵
vector<BeatNote> getNormalBeatMap() {
    return {
        { 1000, 2, false }, { 1500, 3, false }, { 2000, 0, false }, { 2500, 1, false },
        { 3000, 4, false }, { 3250, 2, false }, { 3500, 3, false }, { 4000, 0, false },
        { 4500, 1, false }, { 5000, 4, false }, { 5250, 0, false }, { 5500, 1, false },
        { 6000, 2, false }, { 6500, 3, false }, { 7000, 4, false }, { 7500, 2, false },
        { 8000, 0, false }, { 8250, 3, false }, { 8500, 1, false }, { 9000, 4, false },
        { 9500, 2, false }, { 10000, 3, false }, { 10250, 0, false }, { 10500, 1, false },
        { 11000, 4, false }, { 11500, 2, false }, { 12000, 3, false }, { 12500, 0, false },
        { 13000, 1, false }, { 13250, 2, false }, { 13500, 4, false }, { 14000, 3, false },
        { 14500, 0, false }, { 15000, 1, false }, { 15500, 2, false }, { 15750, 3, false },
        { 16000, 4, false }, { 16500, 0, false }, { 17000, 1, false }, { 17500, 2, false },
        { 18000, 4, false }, { 18250, 0, false }, { 18500, 3, false }, { 19000, 1, false },
        { 19500, 2, false }, { 20000, 4, false }, { 20500, 0, false }, { 21000, 1, false },
        { 21500, 2, false }, { 21750, 3, false }, { 22000, 4, false }, { 22500, 0, false },
        { 23000, 1, false }, { 23500, 2, false }, { 24000, 3, false }, { 24500, 4, false },
        { 25000, 0, false }, { 25250, 1, false }, { 25500, 2, false }, { 26000, 4, false }
    };
}

// 3. HARD 전용 맵
vector<BeatNote> getHardBeatMap() {
    return {
        { 800, 2, false }, { 1100, 3, false }, { 1400, 0, false }, { 1700, 1, false },
        { 2000, 4, false }, { 2200, 2, false }, { 2400, 3, false }, { 2700, 0, false },
        { 3000, 1, false }, { 3200, 4, false }, { 3400, 2, false }, { 3600, 3, false },
        { 4000, 4, false }, { 4200, 0, false }, { 4400, 1, false }, { 4700, 2, false },
        { 5000, 3, false }, { 5200, 4, false }, { 5500, 0, false }, { 5700, 1, false },
        { 6000, 2, false }, { 6200, 3, false }, { 6400, 4, false }, { 6800, 0, false },
        { 7000, 1, false }, { 7300, 2, false }, { 7600, 3, false }, { 7800, 4, false },
        { 8100, 0, false }, { 8300, 1, false }, { 8600, 2, false }, { 8800, 3, false },
        { 9100, 4, false }, { 9400, 2, false }, { 9600, 3, false }, { 9900, 0, false },
        { 10200, 1, false }, { 10400, 4, false }, { 10600, 2, false }, { 10800, 3, false },
        { 11200, 0, false }, { 11400, 1, false }, { 11700, 4, false }, { 12000, 2, false },
        { 12300, 3, false }, { 12500, 0, false }, { 12800, 1, false }, { 13100, 4, false },
        { 13300, 2, false }, { 13500, 3, false }, { 13800, 0, false }, { 14000, 1, false },
        { 14300, 4, false }, { 14600, 2, false }, { 14800, 3, false }, { 15200, 0, false },
        { 15400, 1, false }, { 15700, 4, false }, { 16000, 2, false }, { 16200, 3, false },
        { 16500, 0, false }, { 16700, 1, false }, { 17000, 4, false }, { 17300, 2, false },
        { 17500, 3, false }, { 17800, 0, false }, { 18000, 1, false }, { 18300, 4, false },
        { 18600, 2, false }, { 18800, 3, false }, { 19100, 0, false }, { 19300, 1, false },
        { 19600, 4, false }, { 19900, 2, false }, { 20100, 3, false }, { 20400, 0, false },
        { 20600, 1, false }, { 20900, 4, false }, { 21200, 2, false }, { 21400, 3, false },
        { 21700, 0, false }, { 21900, 1, false }, { 22200, 4, false }, { 22500, 2, false },
        { 22700, 3, false }, { 23000, 0, false }, { 23200, 1, false }, { 23500, 4, false },
        { 23800, 2, false }, { 24000, 3, false }, { 24300, 0, false }, { 24500, 1, false },
        { 24800, 4, false }, { 25100, 2, false }, { 25300, 3, false }, { 25600, 0, false }
    };
}

vector<BeatNote> getBeatMapForStage(int difficulty, int stage) {
    vector<BeatNote> beatMap;
    double rate = 1.0;

    if (difficulty == 1) {
        beatMap = getBaseBeatMap();
        if (stage == 1) rate = 0.93;
        else if (stage == 2) rate = 1.03;
        else rate = 1.13;
    }
    else if (difficulty == 2) {
        beatMap = getNormalBeatMap();
        if (stage == 1) rate = 0.95;
        else if (stage == 2) rate = 1.00;
        else rate = 1.05;
    }
    else if (difficulty == 3) {
        beatMap = getHardBeatMap();
        if (stage == 1) rate = 0.95;
        else if (stage == 2) rate = 1.05;
        else rate = 1.15;
    }

    for (auto& beat : beatMap) {
        beat.timeMs = static_cast<int>(beat.timeMs / rate);
        beat.spawned = false;
    }
    return beatMap;
}

void playStageMusic(int difficulty, int stage) {
    PlaySound(NULL, 0, 0);

    if (difficulty == 1) {
        if (stage == 1) PlaySound(TEXT("stage1 0.93x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else if (stage == 2) PlaySound(TEXT("stage1 1.03x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else PlaySound(TEXT("stage1 1.13x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    else if (difficulty == 2) {
        if (stage == 1) PlaySound(TEXT("stage2 0.95x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else if (stage == 2) PlaySound(TEXT("stage2 1x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else PlaySound(TEXT("stage2 1.05x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    else if (difficulty == 3) {
        if (stage == 1) PlaySound(TEXT("stage3 0.95x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else if (stage == 2) PlaySound(TEXT("stage3 1.05x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        else PlaySound(TEXT("stage3 1.15x.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
}

int getStageSpeed(int difficulty, int stage) {
    if (difficulty == 1) {
        int baseSpeed = 110;
        if (stage == 1) return baseSpeed;
        if (stage == 2) return max(45, baseSpeed - 5);
        return max(40, baseSpeed - 10);
    }
    else if (difficulty == 2) {
        if (stage == 1) return 105;
        if (stage == 2) return 100;
        return 95;
    }
    else if (difficulty == 3) {
        if (stage == 1) return 90;
        if (stage == 2) return 85;
        return 80;
    }
    return 110;
}

int getInitialLife(int difficulty) {
    if (difficulty == 1) return 3;
    if (difficulty == 2) return 5;
    if (difficulty == 3) return 5;
    return 3;
}

void setTargetScores(int difficulty, int targetScores[4]) {
    targetScores[0] = 0;
    if (difficulty == 1) {
        targetScores[1] = 1500;
        targetScores[2] = 2000;
        targetScores[3] = 3000;
    }
    else if (difficulty == 2) {
        targetScores[1] = 1800;
        targetScores[2] = 2300;
        targetScores[3] = 3000;
    }
    else if (difficulty == 3) {
        targetScores[1] = 2500;
        targetScores[2] = 3500;
        targetScores[3] = 4500;
    }
}

string getModeName(int difficulty) {
    if (difficulty == 1) return "EASY";
    if (difficulty == 2) return "NORMAL";
    if (difficulty == 3) return "HARD";
    return "UNKNOWN";
}

int main() {
    srand((unsigned)time(NULL));
    system("mode con cols=100 lines=30");
    system("title Music TokTok Console");
    hideCursor();

    while (true) {
        showStartUI();
        int difficulty = selectDifficulty();
        system("cls");

        int totalScore = 0;
        int stageScore = 0;
        int combo = 0;
        int maxCombo = 0;

        int life = getInitialLife(difficulty);
        bool isGameOver = false;

        int stage = 1;
        const int maxStage = 3;

        int targetScores[4] = { 0 };
        setTargetScores(difficulty, targetScores);

        vector<Note> notes;
        vector<BeatNote> beatMap = getBeatMapForStage(difficulty, stage);

        const int targetX = 12;
        const int noteY = 16;

        int speed = getStageSpeed(difficulty, stage);

        DWORD lastTime = GetTickCount();
        string judgeText = "";
        string modeName = getModeName(difficulty);

        drawHeader();
        drawTargetLine(targetX, noteY);
        drawPlayer();
        drawDynamicUI(totalScore, stageScore, combo, maxCombo, life, speed, stage, targetScores[stage], modeName);

        showReadyGo(targetX + 2, noteY - 3);

        playStageMusic(difficulty, stage);
        DWORD musicStartMs = GetTickCount();
        int travelTimeMs = (60 - targetX) * speed;

        while (!isGameOver) {
            DWORD currentTime = GetTickCount();
            int elapsedMs = (int)(currentTime - musicStartMs);

            if (_kbhit()) {
                int key = _getch();

                if (key == 224) key = _getch();

                int inputType = -1;
                if (key == UP_KEY) inputType = 0;
                else if (key == DOWN_KEY) inputType = 1;
                else if (key == LEFT_KEY) inputType = 2;
                else if (key == RIGHT_KEY) inputType = 3;
                else if (key == SPACE_KEY) inputType = 4;

                // 방향키나 스페이스바가 눌렸다면?
                if (inputType != -1) {

                    // ★ [가장 확실한 비상 수단] 윈도우 기본 사운드칩을 튕겨서 무조건 소리를 냅니다! 
                    Beep(1200, 30);

                    bool hit = false;
                    int bestIndex = -1;
                    int bestDist = 999;

                    for (int i = 0; i < (int)notes.size(); i++) {
                        if (!notes[i].isActive) continue;
                        if (notes[i].type != inputType) continue;

                        int dist = abs(notes[i].x - targetX);
                        if (dist < bestDist) {
                            bestDist = dist;
                            bestIndex = i;
                        }
                    }

                    if (bestIndex != -1) {
                        if (bestDist <= 1) {
                            eraseSingleNote(notes[bestIndex], noteY);
                            notes[bestIndex].isActive = false;

                            int addScore = 150 + (combo * 5);
                            totalScore += addScore;
                            stageScore += addScore;
                            combo++;
                            if (combo > maxCombo) maxCombo = combo;
                            judgeText = "PERFECT";
                            hit = true;
                        }
                        else if (bestDist <= 4) {
                            eraseSingleNote(notes[bestIndex], noteY);
                            notes[bestIndex].isActive = false;

                            int addScore = 100 + (combo * 3);
                            totalScore += addScore;
                            stageScore += addScore;
                            combo++;
                            if (combo > maxCombo) maxCombo = combo;
                            judgeText = "GOOD";
                            hit = true;
                        }
                    }

                    if (!hit) {
                        combo = 0;
                        life--;
                        judgeText = "MISS";
                    }

                    drawDynamicUI(totalScore, stageScore, combo, maxCombo, life, speed, stage, targetScores[stage], modeName);
                    drawJudgeNearTarget(judgeText, targetX, noteY);
                    drawTargetLine(targetX, noteY);
                }
            }

            if (currentTime - lastTime >= (DWORD)speed) {
                lastTime = currentTime;

                for (auto& beat : beatMap) {
                    if (!beat.spawned && elapsedMs >= beat.timeMs - travelTimeMs) {
                        Note newNote;
                        int timeLeft = beat.timeMs - elapsedMs;

                        newNote.x = targetX + (timeLeft / speed);

                        if (newNote.x >= targetX) {
                            if (newNote.x > 60) newNote.x = 60;
                            newNote.type = beat.type;
                            newNote.isActive = true;
                            notes.push_back(newNote);
                        }
                        beat.spawned = true;
                    }
                }

                for (auto& note : notes) {
                    if (!note.isActive) continue;
                    eraseSingleNote(note, noteY);
                }

                for (auto& note : notes) {
                    if (!note.isActive) continue;

                    note.x--;

                    if (note.x < targetX - 4) {
                        note.isActive = false;
                        combo = 0;
                        life--;
                        judgeText = "MISS";
                    }
                }

                removeInactiveNotes(notes);

                drawDynamicUI(totalScore, stageScore, combo, maxCombo, life, speed, stage, targetScores[stage], modeName);
                drawJudgeNearTarget(judgeText, targetX, noteY);
                drawNotes(notes, noteY);
                drawTargetLine(targetX, noteY);

                if (stageScore >= targetScores[stage]) {
                    if (stage < maxStage) {
                        PlaySound(NULL, 0, 0);
                        showStageClearScreen(stage);

                        stage++;
                        stageScore = 0;
                        combo = 0;
                        notes.clear();
                        judgeText = "";

                        beatMap = getBeatMapForStage(difficulty, stage);
                        speed = getStageSpeed(difficulty, stage);

                        system("cls");
                        drawHeader();
                        drawTargetLine(targetX, noteY);
                        drawPlayer();
                        drawDynamicUI(totalScore, stageScore, combo, maxCombo, life, speed, stage, targetScores[stage], modeName);

                        showReadyGo(targetX + 2, noteY - 3);

                        playStageMusic(difficulty, stage);
                        musicStartMs = GetTickCount();
                        travelTimeMs = (60 - targetX) * speed;

                        lastTime = GetTickCount();
                    }
                    else {
                        PlaySound(NULL, 0, 0);
                        showFinalClearScreen(totalScore, maxCombo);
                        return 0;
                    }
                }

                if (life <= 0) {
                    isGameOver = true;
                }
            }
        }

        PlaySound(NULL, 0, 0);
        bool restart = showGameOverScreen(totalScore, maxCombo, stage);
        if (!restart) break;
    }

    return 0;
}