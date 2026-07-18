using System;
using System.Collections.Generic;
using System.IO;

class Solution {
    public static int longestIncreasingSubsequence(List<int> arr) {
        return 0;
    }

    static void Main(String[] args) {
        int n = Convert.ToInt32(Console.ReadLine()!.Trim());
        List<int> arr = new List<int>();

        for (int i = 0; i < n; ++i) {
            arr.Add(Convert.ToInt32(Console.ReadLine()!.Trim()));
        }

        int result = longestIncreasingSubsequence(arr);

        using (StreamWriter file = new StreamWriter("solution.txt")) {
            file.WriteLine(result);
        }
    }
}