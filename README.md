# ICT-374-PROJECT
Current Progress: 80%


TODO:

BLOB
<bd>1.The shell built-in command pwd</bd> 
2.home directory 
<bd>3.slow system calls:</bd>
 int again = 1;
      char *linept;        // pointer to the line buffer

      while (again) {
           again = 0;
           linept = fgets(line, LENGTH_OF_LINE, stdin);
           if (linept == NULL) 
               if (errno == EINTR)
                    again = 1;        // signal interruption, read again
      }
      
 <bd>4.Background job execution: Multi threading (pid child processes running executions while inputting other commands).</bd>
 <bd>5. The shell environment ( parent process ).</bd>
 <bd>6. The shell built-in command exit ( parent process ).</bd>
 <bd>7. Wildcard characters</bd>
 
 <bd>DONE:</bd>
 The shell built-in command pwd 
 Shell pipeline |
 Standard input and output redirections > and <.
