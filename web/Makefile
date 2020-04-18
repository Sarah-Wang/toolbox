all:
	gcc -o tcp_cli tcp_cli.c
	gcc -o tcp_srv tcp_srv.c
	gcc -o tcp_cli_thd tcp_cli_thd.c -lpthread
	gcc -o tcp_srv_thd tcp_srv_thd.c -lpthread

clean:
	rm -rf tcp_cli tcp_srv
	rm -rf tcp_cli_thd tcp_srv_thd
