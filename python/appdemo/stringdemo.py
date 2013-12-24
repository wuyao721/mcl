
# string demo
s = ' one two three '
print s.split()   # ==> ['one', 'two', 'three']
print s.strip()   # ==> 'one two three'
print s.lstrip()  # ==> 'one two three '
print s.rstrip()  # ==> ' one two three'
' '.join(['one', 'two', 'three']) # ==> 'one two three'
'one two three'.upper() # ==> 'ONE TWO THREE'
'ONE TWO THREE'.lower() # ==> 'one two three'

# path demo 
# os.path (part of platform depend)
import os
print os.path.join('C:\\Windows', 'system32') # ==> 'C:\Windows\system32'
print os.path.exists('C:\\Windows\\system32') # ==> True
print os.path.exists('C:/Windows/system32')   # ==> True
print os.path.isdir('C:\\Windows\\system32')  # ==> True
print os.path.isfile('C:\\Windows\\system32') # ==> False
print os.path.splitext('C:\\Windows\\system32\\arp.exe') # ==> ('C:\\Windows\\system32\\arp', '.exe')
print os.path.splitdrive('C:\\Windows\\system32\\drivers') # ==> ('C:', '\\Windows\\system32\\drivers')
print os.path.getsize('C:\\Windows\\system32\\drivers\\etc\\hosts') # ==> 14351
print os.path.dirname('C:\\Windows\\system32\\drivers\\etc') # ==> C:\Windows\system32\drivers
print os.path.dirname('C:\\Windows\\system32\\drivers\\etc\\') # ==> C:\Windows\system32\drivers\etc
def walkdirdemo(path):
    def walk(arg, dirname, fnames):
        for fname in fnames:
            p = os.path.join(dirname, fname)
            if os.path.isdir(p):
                print p
    os.path.walk(path, walk, None)
walkdirdemo('C:\\windows\\system32\\drivers\\') # ==> see above
# C:\windows\system32\drivers\disdn
# C:\windows\system32\drivers\etc
# C:\windows\system32\drivers\umdf

