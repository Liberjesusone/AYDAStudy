import sys

sys.setrecursionlimit(2000)

class Employee:
    def __init__(self, rating, left_child, right_sibling):
        self.rating = rating
        self.left_child = left_child
        self.right_sibling = right_sibling

def planParty(tree):
    return 0.0

if __name__ == '__main__':
    lines = sys.stdin.read().split()
    if not lines:
        sys.exit()

    n = int(lines[0])
    tree = []
    idx = 1
    for _ in range(n):
        rating = float(lines[idx])
        lc = int(lines[idx+1])
        rs = int(lines[idx+2])
        tree.append(Employee(rating, lc, rs))
        idx += 3

    result = planParty(tree)

    with open('solution.txt', 'w') as f:
        f.write(f"{result:.2f}\n")