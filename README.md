# Write-String-With-Doubles

- First string is split in tokens, each token with the length of 8.
- Second check if the CPU is little endian or big endian.
- If CPU is little endian the tokens are reversed, else they are not.
- Tokens are converted to HEX.
- Using an Union tokens are converted to double.
- Next a memory block is allocated to store the tokens.
- Each token is copied in the newly created memory block.
- '\0' is appended at the end.
- Using puts function the memory block is displayed as a string.

## Observation
Function puts(char* str) writes the C string pointed by str to the standard output(stdout) and appends a newline character('\n').
The puts begins copying from the address specified(char* str) until it reaches the terminating null character('\0').
This terminating null-character is not copied to the stream.[source:http://www.cplusplus.com/reference/cstdio/puts/]
