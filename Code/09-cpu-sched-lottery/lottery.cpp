// 随机生成 1-100 之间的数，根据范围决定是否中奖，通过大量模拟计算中奖概率
#include <iostream>
#include <random>
#include <iomanip>

using std::cout;
using std::endl;
using std::cin;

int main(void) {
    long long total_trials = 1000000; // 模拟总次数
    const int range_min = 1;          // 数字范围最小值
    const int range_max = 1000;       // 数字范围最大值
    const int winning_min = 1;        // 中奖范围最小值
    double winning_max = 10;          // 中奖范围最大值

    cout << "总模拟次数：";
    cin >> total_trials;

    cout << "设定中奖概率(支持一位小数)：";
    cin >> winning_max;
    winning_max = static_cast<int>(winning_max * 10.0); // 转换为整数范围

    // 初始化随机数生成器
    std::random_device rd;                                     // 获取随机数种子
    std::mt19937 gen(rd());                                    // 使用梅森旋转算法的随机数生成器
    std::uniform_int_distribution<> dis(range_min, range_max); // 均匀分布

    long long win_count = 0; // 中奖计数器

    int random_number;
    for (int i = 0; i < total_trials; ++i) {
        random_number = dis(gen); // 生成随机数

        if (random_number >= winning_min && random_number <= winning_max) { // 判断是否中奖
            ++win_count;
        }
    }

    cout << "实际中奖概率：" << static_cast<double>(win_count) / total_trials * 100 << "%" << endl;

    return 0;
}