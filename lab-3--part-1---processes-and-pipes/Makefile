#pipes_proc1: pipes_processes1.c
	#gcc pipes_processes1.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic  -o pipes_proc1
	
#pipes_proc2: pipes_processes2.c
	#gcc pipes_processes2.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic  -o pipes_proc2

#pipes_processes3: pipes_processes3.c
	#gcc pipes_processes3.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic  -o pipes_processes3




all: pipes_processes1 pipes_processes2 pipes_processes3

pipes_processes1:
	@echo "Compiling pipes_processes1.c"
	@gcc pipes_processes1.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic -o pipes_processes1

pipes_processes2:
	@echo "Compiling pipes_processes2.c"
	@gcc pipes_processes2.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic -o pipes_processes2

pipes_processes3:
	@echo "Compiling pipes_processes3.c"
	@gcc pipes_processes3.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic -o pipes_processes3

clean:
	@echo "Cleaning up..."
	@rm -f pipes_processes1 pipes_processes2 pipes_processes3