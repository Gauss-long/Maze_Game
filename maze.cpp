#include "maze.h"
//生成基础地图（单元格）
void maze::base() {
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (i == 0 || j == 0 || i == level * 2 + 1 - 1 ||
                j == level * 2 + 1 - 1) {
                map[i][j] = -1;
            } else if (i % 2 != 0 && j % 2 != 0) {
                map[i][j] = 1;
            } else {
                map[i][j] = 0;
            }
        }
    }
}
//使点的周围设为待定（2）
void maze:: _2(int i, int j) {
    if (map[i - 1][j] == 0) {
        map[i - 1][j] = 2;
    }
    if (map[i + 1][j] == 0) {
        map[i + 1][j] = 2;
    }
    if (map[i][j - 1] == 0) {
        map[i][j - 1] = 2;
    }
    if (map[i][j + 1] == 0) {
        map[i][j + 1] = 2;
    }
}
//设定迷宫开始延伸的起点
void maze:: start() {
    map[start_x][start_y] = 5;
    _2(start_x, start_y);
}
//循环停止判定（是否存在未判定的区域）
bool maze:: judge() {
    bool flag = 0;
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (map[i][j] == 2) {
                flag = 1;
                return flag;
            }
        }
    }
    return flag;
}
//操作（如果相邻空单元（1）则打通（变为4），如果不相邻空单元则为墙壁（0））
void maze:: op(int i, int j) {
    if ((map[i - 1][j] == 3 || map[i - 1][j] == 5) && map[i + 1][j] == 1) {
        map[i][j] = 4;
        map[i + 1][j] = 3;
        _2(i + 1, j);
        start_x = i + 1;
        start_y = j;
    } else if ((map[i][j - 1] == 3 || map[i][j - 1] == 5) &&
               map[i][j + 1] == 1) {
        map[i][j] = 4;
        map[i][j + 1] = 3;
        _2(i, j + 1);
        start_x = i;
        start_y = j + 1;
    } else if ((map[i + 1][j] == 3 || map[i + 1][j] == 5) &&
               map[i - 1][j] == 1) {
        map[i][j] = 4;
        map[i - 1][j] = 3;
        _2(i - 1, j);
        start_x = i - 1;
        start_y = j;
    } else if ((map[i][j + 1] == 3 || map[i][j + 1] == 5) &&
               map[i][j - 1] == 1) {
        map[i][j] = 4;
        map[i][j - 1] = 3;
        _2(i, j - 1);
        start_x = i;
        start_y = j - 1;
    } else {
        map[i][j] = 0;
    }
}
//随机选择一个待定墙壁判断并操作
void maze:: random2() {
    int t = 0;
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (map[i][j] == 2) {
                t++;
            }
        }
    }
    int k = rand() % t + 1;
    t = 0;
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (map[i][j] == 2) {
                t++;
                if (t == k) {
                    op(i, j);
                    goto loopout;
                }
            }
        }
    }
loopout:
    if (!judge()) {
        map[start_x][start_y] = 6;
    }
}

//构造函数申请内存空间
maze:: maze(int in_level) : level(in_level) {
    map = new int* [level * 2 + 1];
    for (int i = 0; i < level * 2 + 1; i++) {
        map[i] = new int[level * 2 + 1];
    }
    start_x = 1, start_y = 1; //起点设置为(1,1)
}
maze::~maze() {
    for (int i = 0; i < level * 2 + 1; i++) {
        delete [] map[i];
    }
    delete [] map;
}
//获取地图
int maze:: getlevel() {
    return maze::level;
}
int** maze::getmap() {
    return map;
}
int maze::getside() {
    return level * 2 + 1;
}
//生成地图
void maze:: makemap() {
    p_x = start_x;
    p_y = start_y;
    base();
    start();
    int a = 0;
    while (judge()) {
        a++;
        random2();
        // if (a % 30 == 0) {
        //     printarr(map, level);
        //     system("PAUSE");
        // }
    }
}
int* maze::operator[](int index) {
    return map[index];
}
//重置地图
void maze::rebuildmap() {
    start_x = 1;
    start_y = 1;
    makemap();
}
