all: cpu-watcher/cpu-watcher.so vscope/vscope.so

cpu-watcher/cpu-watcher.so: cpu-watcher/cpu-watcher.c
	gcc -g -c -fPIC cpu-watcher/cpu-watcher.c -o cpu-watcher/cpu-watcher.o
	gcc -g -o cpu-watcher/cpu-watcher.so -shared cpu-watcher/cpu-watcher.o

vscope/vscope.so: vscope/vscope.c 
	gcc -g -c -fPIC vscope/vscope.c -o vscope/vscope.o
	gcc -g -o vscope/vscope.so -shared vscope/vscope.o -lusb

clean:
	rm *.o *.so
