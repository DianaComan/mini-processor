all: bulid

build: so-cpp.obj
	link /nologo /Feso-cpp.exe so-cpp.obj

so-cpp.obj: tema.c
	cl /nologo /W3 /MD /Foso-cpp.obj /c tema.c

clean:
	del *.obj *.exe