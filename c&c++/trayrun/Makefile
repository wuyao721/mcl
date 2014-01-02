!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe

LINK=link.exe

LINK_LIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib

RSC=rc.exe

.c.obj::
	$(CPP) $(CPP_FLAGS) $<

.rc.res::
	$(RSC) $(RSC_FLAGS) $<

CPP_FLAGS=/nologo /MT /W3 /GX /O2 /DWIN32 /DNDEBUG /D_WINDOWS /D_MBCS /c /I..\shared\include
                       
LINK_FLAGS=$(LINK_LIBS) /nologo /machine:I386

RSC_FLAGS=/l 0x409 /dNDEBUG

TARGET=TrayRun.exe
LINK_OBJS=TrayIcon.obj trayrun.obj trayrun.res

all: $(TARGET)

clean:
	-@erase $(TARGET)
	-@erase $(LINK_OBJS)

$(TARGET): $(LINK_OBJS)
	$(LINK) $(LINK_FLAGS) $(LINK_OBJS) /out:$(TARGET)
