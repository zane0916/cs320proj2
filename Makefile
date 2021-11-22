all:	cachesim

cachesim:	Cache.o
	g++ Cache.o -o cache-sim

Cache.o:	Cache.cpp
	g++ -c Cache.cpp

clean:
	rm -f *.o *.txt cache-sim