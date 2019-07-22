# bash_shell
A very simple shell

Requirements
* Prompt the user to enter a command with the prompt "shellname" >
* Parse user input into a command and a list of arguments
* Launch a process from the command and pass it the list of arguments
* Wait for command to exit
* Display the pid of the finished process and its exit status
* Some commands are built into the shell instead of being forked and executed. You will also implement some of these commands.

* pwd - display the present working directory to stdout
* cd - Change the current working directory to the one specified by the user
* exit - The shell exits with code zero
* The shell must run repeatedly until the user types exit at the command line.

