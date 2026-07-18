using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

class Solution {
    public static List<int> longestCommonSubsequence(List<int> a, List<int> b) {
        return new List<int>();
    }

    static void Main(String[] args) {
        string[] tokens = Console.ReadLine()!.Split(' ');
        int n = int.Parse(tokens[0]);
        int m = int.Parse(tokens[1]);

        List<int> a = Console.ReadLine()!.Split(' ').Select(int.Parse).ToList();
        List<int> b = Console.ReadLine()!.Split(' ').Select(int.Parse).ToList();

        List<int> result = longestCommonSubsequence(a, b);

        using (StreamWriter file = new StreamWriter("solution.txt")) {
            file.WriteLine(string.Join(" ", result));
        }
    }
}