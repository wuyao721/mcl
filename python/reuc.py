# -*- coding:utf-8 -*-
# regular express for unicode

reuc = re.compile(u'[\u4e00-\u9fa5]+$')
def isUnicodeChinese(text):
    if not isinstance(text, unicode):
        return False
    if reuc.match(text):
        return True
    return False

if __name__ == '__main__':
    import re
    phanzi = re.compile(u'[\u4e00-\u9fa5]+$');
    print u'锦衣夜行', phanzi.match(u'锦衣夜行')
    print '锦衣夜行', phanzi.match('锦衣夜行')
    print 'uu锦衣夜行', phanzi.match('uu锦衣夜行')
    print u'uu锦衣夜行', phanzi.match(u'uu锦衣夜行')
    print u'锦衣u夜行', phanzi.match(u'锦衣u夜行')
    print u'abc', phanzi.match(u'babc')
    print 'abc', phanzi.match('babc')
