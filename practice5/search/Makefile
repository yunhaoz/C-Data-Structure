all:search-shell

search-shell:search-shell.cpp cmdhandler.o combiners.o handler.o md_parser.o searcheng.o searchui.o txt_parser.o util.o webpage.o
	g++ -g -Wall -std=c++11 search-shell.cpp cmdhandler.o combiners.o handler.o md_parser.o searcheng.o searchui.o txt_parser.o util.o webpage.o -o search-shell
cmdhandler.o:
	g++ -g -Wall -std=c++11 -c cmdhandler.cpp
combiners.o:
	g++ -g -Wall -std=c++11 -c combiners.cpp
handler.o:
	g++ -g -Wall -std=c++11 -c handler.cpp
md_parser.o:
	g++ -g -Wall -std=c++11 -c md_parser.cpp
searcheng.o:
	g++ -g -Wall -std=c++11 -c searcheng.cpp
searchui.o:
	g++ -g -Wall -std=c++11 -c searchui.cpp
txt_parser.o:
	g++ -g -Wall -std=c++11 -c txt_parser.cpp
util.o:
	g++ -g -Wall -std=c++11 -c util.cpp
webpage.o:
	g++ -g -Wall -std=c++11 -c webpage.cpp
clean:
	rm -f *.o search-shell

