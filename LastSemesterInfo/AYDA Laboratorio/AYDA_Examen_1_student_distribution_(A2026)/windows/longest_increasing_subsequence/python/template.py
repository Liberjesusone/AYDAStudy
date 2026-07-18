import sys

def longestIncreasingSubsequence(arr):
    return 0

if __name__ == '__main__':
    n = int(sys.stdin.readline().strip())
    arr = []
    for _ in range(n):
        arr.append(int(sys.stdin.readline().strip()))

    result = longestIncreasingSubsequence(arr)

    with open('solution.txt', 'w') as f:
        f.write(str(result) + '\n')