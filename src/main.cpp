#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <bitset>

// #include "xxx/xxx_string.h"
// #include "xxx/xxx_array.h"
// #include "xxx/xxx_i32array.h"
// #include "xxx/xxx_stack.h"
// #include "xxx/xxx_i32stack.h"
// #include "xxx/xxx_bitset.h"
// #include "xxx/xxx_i32heap.h"
// #include "xxx/xxx_i32sort.h"

using namespace std;

int main() {
    
    // printf("%d, %d", nums.size(), nums.capacity());
}

int palindromePartition(string s, int k) {
    int n = s.length();
    vector<vector<int>> f(n, vector<int>(n, 0));
    for (int i = n - 2; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            f[i][j] = f[i + 1][j - 1] + (s[i] != s[j]);
        }
    }
    for (int i = 0; i < n - 2; ++i) {
        if (!f[0][i]) {
            continue;
        }
        for (int j = i + 1; j < n - 1; ++j) {
            if (f[i + 1][j] && f[j + 1][n - 1]) {
                return true;
            }
        }
    }
    return false;
}

bool checkPartitioning(string s) {
    int n = s.length();
    vector<vector<bool>> f(n, vector<bool>(n, true));
    for (int i = n - 2; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            f[i][j] = f[i + 1][j - 1] && (s[i] == s[j]);
        }
    }
    vector<vector<bool>> dp(n, vector<bool>(3, false));
    dp[0][0] = true;
    for (int j = 1; j < n; ++j) {
        dp[j][0] = f[0][j];
        for (int k = 1; k < 3; ++k) {
            for (int i = k - 1; i < j; ++i) {
                if (f[i + 1][j] && dp[i][k - 1]) {
                    dp[j][k] = true;
                    break;
                }
            }
        }
    }
    return dp[n - 1][2];
}

int dfs(vector<vector<int>>& grid, int r, int c) {
    int m = grid.size();
    int n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] == 0) {
        return 0;
    }
    return grid[r][c]
        + dfs(grid, r - 1, c)
        + dfs(grid, r + 1, c)
        + dfs(grid, r, c - 1)
        + dfs(grid, r, c + 1);
}

int findMaxFish(vector<vector<int>>& grid) {
    int ret = 0;
    int m = grid.size();
    int n = grid[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 0) {
                continue;
            }
            int t = dfs(grid, i, j);
            if (ret < t) {
                ret = t;
            }
        }
    }
    return ret;
}
