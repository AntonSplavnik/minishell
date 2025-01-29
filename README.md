#Minishell

Update on 24.01.2025

<h2>Current Features</h2>

- Command line input processing with readline
- Token parsing and expansion (with a linked list)
- Environment variable handling
- Basic command execution with execve
- Standard shell builtins (cd, echo, pwd, exit, env)
- Error handling
- Test suite for token processing (but not for command exec)

<h2>Token Processing Pipeline</h2>

![image](https://github.com/user-attachments/assets/66190664-e304-4659-8bc2-cb5deba6febd)

<h2>Implementation Details</h2>

<h3>Token Processing</h3>

- **Extraction**: Handles quoted strings, delimiters (<, >, |, <<, >>), and unquoted text
- **Type Assignment**: Identifies commands, arguments, operators based on context
- **Expansion**: Processes environment variables and removes unnecessary quotes
=> All tokens are stored in a linked list.
Reason for choosing a linked list over AST:
- For the mandatory part, the pipe requirements are relatively straightforward : pipes operations are linear (cmd1 | cmd2 | cmd3), and each command's output feeds into the next command's input sequentially (the linear structure of the linked list makes it easy to traverse the commands sequentially).
- The project doesn't require complex operations like boolean operators (&& and ||) or parentheses grouping (only needed for the bonus part).
- AST would add complexity while not providing substantial benefits for the mandatory requirements

<h3>Command Execution</h3>

- If built-in, redirect towards built-in execution (echo, pwd, cd, exit implemented)
- If not built-in, path resolution for commands
- Arguments preparation
- Child process creation with fork/execve
- Exit status handling (to double check - ticket #42)

<h2>Next Steps</h2>

<h3>Built-in Commands</h3>

- Handle environment variable management (export, unset)

<h3>Pipeline Implementation</h3>

- Add pipe operator (|) support
- Manage multiple command execution
- Handle input/output redirection between processes

<h3>Signal Handling</h3>

- Implement CTRL+C, CTRL+D handling

<h3>Edge Cases</h3>

- Find and test edge cases

<h2>Testing</h2>

- Add comprehensive test suite for command exec
- Edge case validation
- Memory leak checks
