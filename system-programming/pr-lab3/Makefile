all: process-a process-b

process-a: src\process-a.c
	gcc -municode src\process-a.c -o build\process-a

process-b: src\process-b.c
	gcc -municode src\process-b.c -o build\process-b

clean:
	del build\process-a.exe
	del build\process-b.exe