# from: 
# http://stackoverflow.com/questions/241327/python-snippet-to-remove-c-and-c-comments

import re

def comment_remover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return ""
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)


if '__main__' == __name__:
    test_str = r'''" */ /* SCC has been trained to know about strings /* */ */"!
"\"Double quotes embedded in strings, \\\" too\'!"
"And \
newlines in them"

"And escaped double quotes at the end of a string\""

aa '\\
n' OK
aa "\""
aa "\
\n"

This is followed by C++/C99 comment number 1.
// C++/C99 comment with \
continuation character \
on three source lines (this should not be seen with the -C fla
The C++/C99 comment number 1 has finished.

This is followed by C++/C99 comment number 2.
/\
/\
C++/C99 comment (this should not be seen with the -C flag)
The C++/C99 comment number 2 has finished.

This is followed by regular C comment number 1.
/\
*\
Regular
comment
*\
/
The regular C comment number 1 has finished.

/\
\/ This is not a C++/C99 comment!

This is followed by C++/C99 comment number 3.
/\
\
\
/ But this is a C++/C99 comment!
The C++/C99 comment number 3 has finished.

/\
\* This is not a C or C++ comment!

This is followed by regular C comment number 2.
/\
*/ This is a regular C comment *\
but this is just a routine continuation *\
and that was not the end either - but this is *\
\
/
The regular C comment number 2 has finished.

This is followed by regular C comment number 3.
/\
\
\
\
* C comment */'''
    s = comment_remover(test_str)
    print s

#    f = open('foo.c', 'w')
#    f.write(test_str)
#    f.close()
#    
#    import commands
#    s2 = commands.getoutput('cpp -fpreprocessed foo.c')
#    
#    commands.getoutput('rm -f foo.c')
#
#    f = open('python.c', 'w')
#    f.write(s)
#    f.close()
#
#    f = open('c.c', 'w')
#    f.write(s2)
#    f.close()
