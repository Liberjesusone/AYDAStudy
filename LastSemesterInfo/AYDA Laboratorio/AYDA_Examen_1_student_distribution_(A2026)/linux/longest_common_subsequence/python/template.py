import sys

def longestCommonSubsequence(a, b):
    return []

if __name__ == '__main__':
    input_data = sys.stdin.read().split()
    if not input_data:
        sys.exit()

    n = int(input_data[0])
    m = int(input_data[1])

    a = [int(x) for x in input_data[2 : 2+n]]
    b = [int(x) for x in input_data[2+n : 2+n+m]]

    result = longestCommonSubsequence(a, b)

    with open('solution.txt', 'w') as f:
        f.write(' '.join(map(str, result)) + '\n')