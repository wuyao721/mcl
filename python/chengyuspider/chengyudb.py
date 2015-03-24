# -*- coding: utf-8 -*-

import sqlite3

dbfile = 'chengyu.dat'

sql_create_chengyu = '''CREATE TABLE IF NOT EXISTS chengyu (
    `name`                 VARCHAR(32) PRIMARY KEY,
    `spell`                VARCHAR(32),
    `description`          VARCHAR(256),
    `source`               VARCHAR(256),
    `example`              VARCHAR(256)
)'''

sql_add_chengyu = ("REPLACE INTO chengyu "
           "(name, spell, description, source, example) "
           "VALUES (?, ?, ?, ?, ?)")

def modify_table(sql, args):
    conn = sqlite3.connect(dbfile)
    c = conn.cursor()
    try:
        c.execute(sql, args)
        conn.commit()
    finally:
        conn.close()

def add_chengyu(name, spell, description, source, example):
    modify_table(sql_add_chengyu, (name, spell, description, source, example))

if __name__ == '__main__':
    conn = sqlite3.connect(dbfile)
    c = conn.cursor()
    c.execute(sql_create_chengyu)
    conn.commit()
    conn.close()
    add_chengyu('lksjdf', 'lksjdf', 'sldfkj', 'source', 'example')
    pass
