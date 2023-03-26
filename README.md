# ICT-374-PROJECT
Current Progress: 30%


TODO:

BLOB
The shell built-in command pwd 
home directory

in its own pid whenever we use a file descriptor. 
slow system calls:
 int again = 1;
      char *linept;        // pointer to the line buffer

      while (again) {
           again = 0;
           linept = fgets(line, LENGTH_OF_LINE, stdin);
           if (linept == NULL) 
               if (errno == EINTR)
                    again = 1;        // signal interruption, read again
      }
      
 DONE:
 The shell built-in command pwd 
 Shell pipeline |
