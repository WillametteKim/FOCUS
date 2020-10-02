all: prong clean

prong: pro.o dbBan.o dbShow.o bittwistb_sql.o alarm.o
	gcc -o prong pro.o dbBan.o dbShow.o bittwistb_sql.o alarm.o -lpthread -lpcap -l sqlite3
pro.o:
	gcc -c pro.c

dbBan.o:
	gcc -c dbBan.c

dbShow.o:
	gcc -c dbShow.c

bittwistb_sql.o:
	gcc -c bittwistb_sql.c

alarm.o:
	gcc -c alarm.c

clean:
	rm -f pro.o dbBan.o dbShow.o bittwistb_sql.o alarm.o
