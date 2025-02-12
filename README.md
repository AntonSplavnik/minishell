#Minishell

Update on February 12, 2025

<h2>Current Features</h2>


- Command line input processing with readline
- Token parsing and expansion using linked list structure
- Complete environment variable handling (including $? expansion)
- Comprehensive command execution with path resolution and execve
- All required shell builtins implemented
- Error handling with appropriate exit status code

<h2>Token Processing Pipeline</h2>

Miro Board: https://miro.com/app/board/uXjVLIs9XcM=/

<h2>Implementation Details</h2>

<h3>Token Processing</h3>

- **Extraction**: Handles quoted strings, delimiters (<, >, |, <<, >>), and unquoted text
- **Type Assignment**: Identifies commands, arguments, operators based on context
- **Expansion**: Processes environment variables and removes unnecessary quotes
- **Heredoc**: Special handling for heredoc content with quoted/unquoted delimiter
=> All tokens are stored in a linked list.


Reason for choosing a linked list over AST:
- For the mandatory part, the pipe requirements are relatively straightforward : pipes operations are linear (cmd1 | cmd2 | cmd3), and each command's output feeds into the next command's input sequentially (the linear structure of the linked list makes it easy to traverse the commands sequentially).
- The project doesn't require complex operations like boolean operators (&& and ||) or parentheses grouping (only needed for the bonus part).
- AST would add complexity while not providing substantial benefits for the mandatory requirements

<h3>Environment Variable Management</h3>

- Complete environment initialization and management
- Support for variable assignments and modifications
- Proper handling of append operations (+=)
- Sorted output for export command

<h3>Command Execution</h3>

- If built-in, redirect towards built-in execution
- If not built-in, path resolution for commands
- Arguments preparation
- Child process creation with fork/execve
- Error handling and Exit status for various command execution scenarios

<h3>Error Handling</h3>


- Current exit status implementation:
  - 0: Success (command executed successfully)
  - 1: General errors (malloc failure, execution errors)
  - 2: Syntax errors (parsing errors)
  - 126: Command found but not executable
  - 127: Command not found

<h2>Next Steps</h2>

<h3>Redirections, Heredoc and Pipes</h3>

- Implement input/output redirections (<, >, >>)
- Add pipe support
- Handle multiple command execution
- Manage file descriptors properly

<h3>Signal Handling</h3>

- Implement signal handling (CTRL+C, CTRL+D, CTRL+)
- Implement correct exit status for signal handling
- Ensure proper behavior in both interactive and non-interactive modes

<h2>Testing</h2>

- Add comprehensive test suite for command exec
- Edge case validation
- Memory leak checks
- Find and test edge cases

- In order to test valgrind, run valgrind with the extra flag : valgrind --suppressions=suppressions.supp --leak-check=full --show-leak-kinds=all ./minishell
