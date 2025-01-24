#Minishell

Update on 24.01.2025

<h2>Current Features</h2>

- Command line input processing with readline
- Token parsing and expansion
- Environment variable handling
- Basic command execution with execve (no built-in)
- Error handling
- Test suite for token processing (but not for command exec)

<h2>Token Processing Pipeline</h2>

![image](https://github.com/user-attachments/assets/66190664-e304-4659-8bc2-cb5deba6febd)

<h2>Implementation Details</h2>

<h3>Token Processing</h3>

- **Extraction**: Handles quoted strings, delimiters (<, >, |, <<, >>), and unquoted text
- **Type Assignment**: Identifies commands, arguments, operators based on context
- **Expansion**: Processes environment variables and removes unnecessary quotes

<h3>Command Execution</h3>

- Path resolution for commands
- Arguments preparation
- Child process creation with fork/execve
- Exit status handling

<h2>Next Steps</h2>

<h3>Built-in Commands</h3>

- Implement standard shell builtins (cd, echo, pwd, exit)
- Handle environment variable management (export, unset, env)

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
