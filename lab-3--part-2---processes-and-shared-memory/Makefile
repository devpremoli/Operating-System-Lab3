all: shm_proc bank

shm_proc:
	@echo "Compiling shm_processes.c"
	@gcc shm_processes.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic -o shm_proc

bank:
	@echo "Compiling bank.c"
	@gcc bank.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic -o bank

clean:
	@echo "Cleaning up..."
	@rm -f shm_proc bank