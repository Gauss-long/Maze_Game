#ifndef MAZE_H
#define MAZE_H
#include <iostream>
/**************************************************
 * -1:边界 0:墙壁
 * 1:空单元(初始化后未判定的单元)
 * 2:待定墙壁(新建已定单元时建立)
 * 3:已定单元(确定迷宫树已延伸到的单元)
 * 4:打通墙壁(延伸操作)
 * 5.起点
 * 6.终点
 * 7.已经过路径
 * **************************************************/
class maze {
private:
    int level;  //迷宫阶数
    int** map;  //地图存储空间
    int start_x, start_y;   //起终点
    //生成基础地图（单元格）
    void base();
    //使点的周围设为待定（2）
    void _2(int i, int j);
    //设定迷宫开始延伸的起点
    void start() ;
    //循环停止判定（是否存在未判定的区域）
    bool judge();
    //操作（如果相邻空单元（1）则打通（变为4），如果不相邻空单元则为墙壁（0））
    void op(int i, int j);
    //随机选择一个待定墙壁判断并操作
    void random2();
public:
    //构造函数申请内存空间
    maze(int in_level);
    ~maze();
    //获取地图
    int getlevel() ;
    int** getmap();
    int getside();
    //生成地图
    void makemap();
    int p_x, p_y; //当前位置
    //重置地图
    void rebuildmap();
    int* operator[](int index);
};
#endif // MAZE_H
