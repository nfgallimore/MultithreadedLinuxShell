To compile type "make" in the terminal to run the makefile.
You can use the shell by running ./shell
The following commands work:

 C file1 file2 Copy; create file2, copy all bytes of file1 to file2 without deleting file1.
 D file Delete the named file.
 E comment Echo; display comment on screen followed by a new line
 H Help; display the user manual
 L List the contents of the current directory.
 M file Make; create the named text file by launching a text editor.
 P file Print; display the contents of the named file on screen.
 Q Quit the shell.
 S Surf the web by launching a browser as a background process.
 W Wipe; clear the screen.
 X program Execute the named program.


functions contained in shell:

int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
int main(int argc, char **);

execlp used:

http://man7.org/linux/man-pages/man3/exec.3.html
