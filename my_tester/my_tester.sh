#!/bin/bash
# my_tester.sh
#
# This tester script runs each command from the list through bash and through your
# minishell. The bash output is used as the expected output. Since your minishell
# always prints its prompt (e.g. "minishell$ "), we filter those lines out before comparing.
#
# Note:
#   • Interactive commands (e.g. here-documents or those expecting keypresses) might
#     not work as expected in non-interactive mode.
#   • The timeout is set to 2 seconds to prevent hanging on commands.
#   • Adjust the prompt filter (currently looking for lines that consist solely of
#     "minishell$ " possibly with trailing spaces) if your prompt differs.
#
# Path to your minishell binary:
MINISHELL=./minishell

# Create a temporary directory if needed:
TMP_DIR="./minishell_test_tmp"
mkdir -p "$TMP_DIR"

# Define the test commands.
inputs=(
  ""
  "cat infile | grep \"hello\" > /nonexistentdir/outfile"
  "cat < file1.txt | tee file2.txt | tee file3.txt > final.txt"
  "echo, ls, whoami, pwd"
  "env"
  "echo \"This is a test\" | tr ' ' '\n' | sort | uniq"
  "echo test > out.txt"
  "   "
  "exit"
  "echo \$user"
  "echo \"test 1\" | echo \"test 2\""
  "\$?"
  "cd ../../../../../.."
  "cd"
  "cd .. cool swag"
  "cd Eyooooo"
  "cd \$PWD hi"
  "echo \"\$ \$\""
  "echo \"\$ \$ \$ \$ \$HOME \$\""
  "echo a c b  | sort"
  "echo"
  "echo \$NonExistingVar"
  "echo \$PATH"
  "echo -n -n -nnnn -nnnnm"
  "echo -n -nnn hello -n"
  "echo \"front\$HOMEback\""
  "env"
  "env Weeiirrddd"
  "src"
  "src/"
  "LICENSE"
  "abc"
  "./LICENSE"
  "./src"
  "./src_what"
  "./abc"
  "cat bla"
  "export a=42 then export a=43 | echo \$a"
  "export GHOST=123 | env | grep GHOST"
  "export \"\" test=a"
  "export 42=42"
  "export 1 = 2 45 7 7"
  "export test3=\$HOME"
  "export ==value"
  "\"\""
  "\$"
  ",,"
  "\$NOTKNOW"
  "echo '\"abc\"'"
  "echo \"\" bonjour"
  "cat | cat | cat | ls"
  "cat Makefile | grep pr | head -n 5 | cd file_not_exit"
  "cat Makefile | grep pr | head -n 5 | hello"
  "ls | exit"
  "> test | echo blabla"
  "exit > hoi.txt"
  "env | grep OLDPWD"
  "echo -e \"hello world\nthis is a test\nhello again\nbye\" > infile"
  "cd .. > derp.txt"
  "cat - [ PRESS CTRL + C ]"
  "cat [ PRESS CTRL + \\ ]"
  "cat [ PRESS CTRL + D ]"
  "PRESS CTRL D"
  "> file"
  "cat -e > test1 < test2"
  "cat < unknown"
  "echo 2 > out1 >> out2"
  "echo 2 >> out1 > out2"
  "echo test > file test1"
  "Non_exist_cmd > salut"
  "echo \"echo first > out1\n echo second >>out1 >out2\""
  "pwd home"
  "./Makefile"
  "./Makefiles"
  "./src"
  "./anything"
  "./src/environment/environment.c"
  "echo \"\$tests\"\"Makefile\""
  "echo \"\$tests\"Makefile"
  "echo \"\$tests\" \"Makefile\""
  "export \$var (\$var does not exists)"
  "|"
  "()"
  "(("
  "echo bonjour > > out"
  "echo bonjour > \$test w/ test=\"o1 o2\""
  "echo bonjour > \$test w/ test=\"o1 o2\""
  "echo bonjour >>> test"
  "echo bonjour | |"
  "unset ="
  "unset PWD"
  "cat wfeh | cat csijdsji | cat nfwir"
  "cat <<END\n> first, second \$ \$HOME \$UNKNOWN\n> third\n> END"
  "echo << END | ls\n> \$HOME\n> END"
  "cat << END1 <<END2\n> first\n> interrupt by Ctrl-D\n> second"
  "cat << END1 << END2\n> first\n> interrupt by Ctrl-C"
  "cat<<end"
  "cat <<()"
)

fail=0
total=${#inputs[@]}

echo "Starting dynamic expected output tests..."
echo "Total tests: $total"
echo "-------------------------------------"

# For each test command, run in bash to get expected output and in minishell to get result.
for i in "${!inputs[@]}"; do
    cmd="${inputs[$i]}"

    # Run the command in bash.
    expected=$(echo -e "$cmd" | bash 2>&1)

    # Run the command in minishell with a 2-second timeout.
    raw_result=$(echo -e "$cmd" | timeout 2 "$MINISHELL" 2>&1)

    # Filter out lines that consist solely of the prompt.
    # (Assuming your prompt is "minishell$ " exactly.)
    filtered_result=$(echo -e "$raw_result" | sed '/^minishell\$ *$/d')

    echo "Test $((i+1)):"
    echo "Command: [$cmd]"
    echo "Bash output (expected):"
    echo "---------------------"
    echo -e "$expected"
    echo "---------------------"
    echo "Minishell raw output:"
    echo "---------------------"
    echo -e "$raw_result"
    echo "---------------------"
    echo "Minishell filtered output:"
    echo "---------------------"
    echo -e "$filtered_result"
    echo "---------------------"

    if [ "$filtered_result" == "$expected" ]; then
        echo "Result: PASS"
    else
        echo "Result: FAIL"
        fail=$((fail+1))
    fi
    echo "-------------------------------------"
done

echo "Tests completed: $((total-fail)) passed, $fail failed."
rm -rf "$TMP_DIR"
