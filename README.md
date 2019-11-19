# Server-CLient

  The project is a multi-processed and multi-threaded client-server shell that performs basic mathematical operations and runs applications while also keeping a list of all processes (active and inactive). The entire project is implanted using C and Linux APIs. I have implemented two files: One for client and one for server. The client file has code that deals with the client side of the project, which includes connecting to socket, reading from StdIn (User), writing on StdOut (Console / Terminal), and reading and writing on socket.
  The server code reads the client’s inputs from a socket and processes the input and returns the result. The server also has its own interactive interface, where the user can get information about all the clients and processes that are currently running or were running.
  In order to run the program, you must first compile the server file on a terminal. The file must be compiled with the -lpthread flag because it uses POSIX threads. Similar is the case with the client file. The client must be compile using -lpthread flag. But in order to execute the client object code (.out) file. IP and Port Number should also be passed as a command line argument.


Client Commands
  After Client is connecting to the server using the socket port. The client program will execute and prompt the user to enter a command. The list of commands includes:

	add 
  -	Writing the above command followed by list of number will compute and display its sum.
  
	sub 
  -	Writing the above command followed by list of number will compute and display its difference.
  
	mul
  -	Writing the above command followed by list of number will compute and display its Product.
  
	div
  -	Writing the above command followed by list of number will compute and display division.
  
	run 
  -	Writing the above command followed by the executable file name will execute the process.
  -	This command is case sensitive and could show errors if case is ignored.
  
	list 
  -	Writing the above command will displayed all active as well as inactive processes.
  
	killp
  -	Writing the above command followed by Process pid will forcefully terminate the process
  
	killn
  -	Writing the above command followed by Process Name will forcefully terminate the process.
  
	exit 
  -	Exit the client from running.


Server Commands
  The client program will execute and prompt the user to enter a command. The list of commands includes: 

	list client
  -	Shows the information about all currently connected client.
  -	The information includes client’s IP address, client’s Port and its starting time
  
	client count
  -	Displays the count of currently connected clients.
  
	kick
  -	Writing the above command followed by IP will disconnect the client. (If connected). 
  
	printall
  -	Writing the above command followed by a message will print the message on all client console / terminal.
  
	print 
  -	Writing the above command followed by an IP and a message will print the message on that particular client console / terminal.
  
	list process
  -	Shows all the processes (active and inactive) of all currently connected clients.
  
	print 
  -	Writing the above command followed by an IP and a message will print the message on that particular client console / terminal. 
