#!/usr/bin/env python
# -*- coding: utf-8 -*-

from random import randrange
import time
from copy import deepcopy

'''
生命游戏中，对于任意细胞，规则如下：
1． 如果一个细胞周围有3个细胞为生（一个细胞周围共有8个细胞），则该细胞为生（即该细胞若原先为死，则转为生，若原先为生，则保持不变） 。
2． 如果一个细胞周围有2个细胞为生，则该细胞的生死状态保持不变；
3． 在其它情况下，该细胞为死（即该细胞若原先为生，则转为死，若原先为死，则保持不变）
可以把最初的细胞结构定义为种子，当所有在种子中的细胞同时被以上规则处理后, 可以得到第一代细胞图。按规则继续处理当前的细胞图，可以得到下一代的细胞图，周而复始。
参考：http://zh.wikipedia.org/wiki/%E7%94%9F%E5%91%BD%E6%B8%B8%E6%88%8F
'''

def life_count_round(dyadicarray, row, col, width, height):
    def islife(row, col):
        if row >= 0 and row < height and col >= 0 and col < width:
            return dyadicarray[row][col]
        else:
            return 0
    return sum(islife(row+i, col+j) for i in [-1, 0, 1] for j in [-1, 0, 1] if i != 0 or j != 0)

def reproduct(dyadicarray, width, height):
    generation = deepcopy(dyadicarray)
    for row, rowitem in enumerate(generation):
        for col, colitem in enumerate(rowitem):
            c = life_count_round(generation, row, col, width, height)
            if c == 3:
                generation[row][col] = 1
            elif c != 2:
                generation[row][col] = 0
    return generation

def print_generation(dyadicarray):
    print;print;print;print;print;print;print;print;
    for row in dyadicarray:
        print ' '.join(['*' if col else ' ' for col in row])
    
if __name__ == '__main__':
    length = 20
    dyadicarray = [[randrange(0,2) for col in range(length)] for row in range(length)]
    print_generation(dyadicarray)
    while 1:
        time.sleep(1)
        dyadicarray = reproduct(dyadicarray, length, length)
        print_generation(dyadicarray)

