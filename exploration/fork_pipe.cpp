# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <sys/wait.h>

# include <vector>
# include <map>
# include <cstdlib>
# include <string>
# include <stdexcept>
# include <iostream>

# define READ_END	0
# define WRITE_END	1

// https://stackoverflow.com/questions/2605130/redirecting-exec-output-to-a-buffer-or-file

int main()
{
	int pipefd[2];
	pipe(pipefd);

	if (fork() == 0)
	{
		close(pipefd[READ_END]);	// close reading end in the child

		dup2(pipefd[WRITE_END], 1);  // send stdout to the pipe
		dup2(pipefd[WRITE_END], 2);  // send stderr to the pipe

		// close(pipefd[1]);	// this descriptor is no longer needed

		const char* cmd[] = {"ls", "-l", NULL};
		execve(cmd[0], const_cast<char* const *>(cmd), 0);
	}
	else
	{
		// parent

		char buffer[1024];

		close(pipefd[WRITE_END]);  // close the write end of the pipe in the parent

		while (read(pipefd[READ_END], buffer, sizeof(buffer)) != 0)
		{
			std::cout << buffer << std::endl;
		}
	}
}