#include "mazewidget.h"
#include "ui_mazewidget.h"
mazeWidget::mazeWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::mazeWidget), map(new maze(20)), painting_switch(false), timing_switch(false)
    , keybord_switch(false), grade(0), time(0) {
    //TODO:状态栏
    ui->setupUi(this);
    ui->progressBar->setVisible(false);                 //初始隐藏进度条
    ui->end_btn->setEnabled(false);                     //设置终止按钮禁用
    ui->stop_ptn->setEnabled(false);                    //设置暂停按钮禁用
    ui->grade_value->setText("  ");                     //设置分数值显示为空
    ui->time_value->setText("  ");                      //设置时间值显示为空
    map->makemap();                                     //生成地图
    timer = new QTimer(this);                                           //初始化计时器
    connect(timer, &QTimer::timeout, this, &mazeWidget::time_update);   //链接时间更新信号与槽
    ui->plaque_time->setText("  ");
    ui->plaque_grade->setText("  ");
    ui->label->setText("  ");
}
mazeWidget::~mazeWidget() {
    delete ui;
    delete map;
    delete timer;
}
void mazeWidget::paintEvent(QPaintEvent*) {
    if(!painting_switch) return;
    QPainter painter(this);     //画笔对象
    //绘图逻辑：
    int perblock = (std::min(ui->frame->width(), ui->frame->height()) - 20) / (map->getside());
    int start_x = ui->frame->x() + (ui->frame->width() - (ui->frame->x() + (map->getside()) * perblock)) / 2;
    int strat_y = ui->frame->y() + (ui->frame->height() - (ui->frame->y() + (map->getside()) * perblock)) / 2;
    for(int i = 0; i < map->getlevel() * 2 + 1; i++) {
        for(int j = 0; j < map->getlevel() * 2 + 1; j++) {
            if(i == map->p_x && j == map->p_y) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::red));
            } else if(map->getmap()[i][j] == 7) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::yellow));
            } else if(map->getmap()[i][j] == 3 || map->getmap()[i][j] == 4) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::gray));
            } else if(map->getmap()[i][j] == 5) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::blue));
            } else if(map->getmap()[i][j] == 6) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::green));
            } else {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::black));
            }
        }
    }
}
void mazeWidget::keyPressEvent(QKeyEvent* event) {
    if(!keybord_switch) return;
    int x = map->p_x;
    int y = map->p_y;
    //键盘移动逻辑：
    if(event->key() == Qt::Key_I || event->key() == Qt::Key_W) {
        if((*map)[x][y - 1] == 3 || (*map)[x][y - 1] == 4 || (*map)[x][y - 1] == 5 || (*map)[x][y - 1] == 6 || (*map)[x][y - 1] == 7) {
            map->p_y--;
        }
    } else if(event->key() == Qt::Key_K || event->key() == Qt::Key_S) {
        if((*map)[x][y + 1] == 3 || (*map)[x][y + 1] == 4 || (*map)[x][y + 1] == 5 || (*map)[x][y + 1] == 6 || (*map)[x][y + 1] == 7) {
            map->p_y++;
        }
    } else if(event->key() == Qt::Key_J || event->key() == Qt::Key_A) {
        if((*map)[x - 1][y] == 3 || (*map)[x - 1][y] == 4 || (*map)[x - 1][y] == 5 || (*map)[x - 1][y] == 6 || (*map)[x - 1][y] == 7) {
            map->p_x--;
        }
    } else if(event->key() == Qt::Key_L || event->key() == Qt::Key_D) {
        if((*map)[x + 1][y] == 3 || (*map)[x + 1][y] == 4 || (*map)[x + 1][y] == 5 || (*map)[x + 1][y] == 6 || (*map)[x + 1][y] == 7) {
            map->p_x++;
        }
    }
    //经过路径
    if((*map)[map->p_x][map->p_y] != 5 && (*map)[map->p_x][map->p_y] != 6)(*map)[map->p_x][map->p_y] = 7;
    repaint();
    //到达终点
    if((*map)[map->p_x][map->p_y] == 6) {
        map->makemap();
        repaint();
        grade += pow(map->getlevel(), 2);
        ui->grade_value->setText(QString::number(grade));
    }
}
void mazeWidget::time_update() {
    if(time != 0) {
        //计时中
        time--;
        ui->time_value->setText(QString::number(time));
        ui->progressBar->setValue(time / 2);
    } else {
        timer->stop();                          //停止计时器
        ui->progressBar->setVisible(false);     //隐藏进度条
        keybord_switch = false;                 //设置键盘响应、
        painting_switch = false;                //绘图响应、
        timing_switch = false;                  //计时响应为关闭状态
        repaint();                              //清除画布
        ui->start_btn->setEnabled(true);        //|
        ui->time_value->setText(" ");           //|
        ui->grade_value->setText(" ");          //|
        ui->stop_ptn->setEnabled(false);        //|设置各按钮与标签状态
        ui->end_btn->setEnabled(false);         //|
        ui->setting_btn->setEnabled(true);      //|
        //提示
        QMessageBox outgrade(QMessageBox::NoIcon, "恭喜", "您得分:" + QString::number(grade), QMessageBox::Ok);
        outgrade.exec();
        //分数重置
        grade = 0;
    }
}
void mazeWidget::on_start_btn_clicked() {
    painting_switch = true;
    timing_switch = true;
    keybord_switch = true;
    time = 200;
    timer->start(1000);
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(100);
    repaint();
    ui->time_value->setText(QString::number(time));
    ui->grade_value->setText(QString::number(grade));
    ui->start_btn->setEnabled(false);
    ui->stop_ptn->setEnabled(true);
    ui->end_btn->setEnabled(true);
    ui->setting_btn->setEnabled(false);
    ui->plaque_time->setText("时间");
    ui->plaque_grade->setText("分数");
}
void mazeWidget::on_stop_ptn_clicked() {
    if(stop_switch) {
        timing_switch = false;
        keybord_switch = false;
        timer->stop();
        ui->stop_ptn->setText("继续");
        stop_switch = false;
    } else {
        timing_switch = true;
        keybord_switch = true;
        timer->start();
        ui->stop_ptn->setText("暂停");
        stop_switch = true;
    }
}
void mazeWidget::on_end_btn_clicked() {
    timing_switch = false;
    painting_switch = false;
    keybord_switch = false;
    stop_switch = false;
    timer->stop();
    time = 0;
    grade = 0;
    ui->plaque_time->setText("  ");
    ui->plaque_grade->setText("  ");
    ui->progressBar->setVisible(false);
    ui->grade_value->setText(" ");
    ui->time_value->setText(" ");
    ui->stop_ptn->setText("暂停");
    ui->stop_ptn->setEnabled(false);
    ui->end_btn->setEnabled(false);
    ui->start_btn->setEnabled(true);
    ui->setting_btn->setEnabled(true);
    map->rebuildmap();
    repaint();
}
void mazeWidget::on_rule_btn_clicked() {
    QMessageBox rule(QMessageBox::NoIcon, "规则", "计时200秒，根据迷宫等级与经过关卡记分。\n操作方式：WASD或者IJKL控制方向。", QMessageBox::Ok);
    rule.exec();
}
void mazeWidget::on_setting_btn_clicked() {
    QStringList difficultys;
    difficultys << tr("小朋友难度(5阶迷宫)") << tr("简单难度(10阶迷宫)") << tr("普通难度(20阶迷宫)") << tr("困难难度(40阶迷宫)");
    QString difficulty = QInputDialog::getItem(this, tr("选择难度"),
                                               tr("请选择一个条目"), difficultys, 0, false);
    if(difficulty == tr("小朋友难度(5阶迷宫)")) {
        delete map;
        map = new maze(5);
        map->makemap();
    } else if(difficulty == tr("简单难度(10阶迷宫)")) {
        delete map;
        map = new maze(10);
        map->makemap();
    } else if(difficulty == tr("普通难度(20阶迷宫)")) {
        delete map;
        map = new maze(20);
        map->makemap();
    } else if(difficulty == tr("困难难度(40阶迷宫)")) {
        delete map;
        map = new maze(40);
        map->makemap();
    }
}




