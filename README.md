# ICT-374-PROJECT
Current Progress: 30%


TODO:

BLOB
1.The shell built-in command pwd 
2.home directory 
3.slow system calls:
 int again = 1;
      char *linept;        // pointer to the line buffer

      while (again) {
           again = 0;
           linept = fgets(line, LENGTH_OF_LINE, stdin);
           if (linept == NULL) 
               if (errno == EINTR)
                    again = 1;        // signal interruption, read again
      }
      
 4.Background job execution: Multi threading (pid child processes running executions while inputting other commands).
 5. The shell environment ( parent process ).
 6. The shell built-in command exit ( parent process ).
 7. Wildcard characters
 
 DONE:
 The shell built-in command pwd 
 Shell pipeline |
 Standard input and output redirections > and <
