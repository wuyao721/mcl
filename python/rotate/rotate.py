# -*- coding:utf-8 -*-
import random

prize_arr = [
    { 'id': 0, 'min': 5,   'max': 40,  'prize': '工费五折',     'v': 90 },
    { 'id': 1, 'min': 50,  'max': 85,  'prize': '三十元代金券', 'v': 1 },
    { 'id': 2, 'min': 95,  'max': 125, 'prize': '再来一次吧',   'v': 1 },
    { 'id': 3, 'min': 135, 'max': 170, 'prize': '谢谢参与',     'v': 1 },
    { 'id': 4, 'min': 180, 'max': 215, 'prize': '一千元代金券', 'v': 1 },
    { 'id': 5, 'min': 225, 'max': 260, 'prize': '三百元代金券', 'v': 1 },
    { 'id': 6, 'min': 270, 'max': 305, 'prize': '一百元代金券', 'v': 5 },
    { 'id': 7, 'min': 315, 'max': 350, 'prize': '三十元代金券', 'v': 5 },
]

def rotate():
    s = sum(map(lambda x: x['v'], prize_arr))
    theone = None
    for p in prize_arr:
        r = random.randint(1, s)
        if p['v'] >= r:
            theone = p
            break
        s = s - p['v']
    return theone

if '__main__' == __name__:
    print rotate()
