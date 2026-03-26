# ME 5: DP

Alice is transmitting numbers from 0 to 19 (inclusive) encoded as a comma-delimited string such as `4,19,20,...`. Unfortunately, a man-in-the-middle is listening to the plaintext and deleting every comma to cause mischief. Now the string looks like `41920...`. Bob is on the other end, and wants to reconstruct Alice's original string. As an intermediate step, he wants to know how many strings there could have been. He only knows that Alice never encodes a single digit as two digits. (i.e., she never writes 09 to mean 9). The string is at least 1 character, and at most 1 million.

To help Bob, define the function
```c
int n_encodings(const char* s, int len);
```

Which returns the total number of different strings could have given rise to the one with commas deleted. For example: `n_encodings("41910", 5) == 4`, because "4,1,9,1,0"; "4,1,9,10", "4,19,1,0", and "4,19,10" are the valid strings.

Your solution must run in $O(\mathrm{len})$ time. -75 pts if not. No outside functions permitted.

Hint: consider if we had the solution for n_encodings(s, len - 1) and n_encodings(s, len - 2). Now imagine we see a new character. If it can be merged with its previous character, that gives us two sets of encodings to consider. 

# Key

```c
int dp[1000000]; // could technically be a single value
                 // but the DP is easier to understand with a table

int n_encodings(const char* s, int len) {
    dp[0] = 1;

    return dp[len - 1];
}
```
