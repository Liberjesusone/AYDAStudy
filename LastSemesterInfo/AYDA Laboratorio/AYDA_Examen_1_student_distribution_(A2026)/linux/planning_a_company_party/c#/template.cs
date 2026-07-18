using System;
using System.Collections.Generic;
using System.IO;
using System.Globalization;

class Employee {
    public double rating;
    public int left_child;
    public int right_sibling;
}

class Solution {
    public static double planParty(List<Employee> tree) {
        return 0.0;
    }

    static void Main(String[] args) {
        string firstLine = Console.ReadLine()!;
        if (string.IsNullOrEmpty(firstLine)) return;

        int n = int.Parse(firstLine.Trim());
        List<Employee> tree = new List<Employee>();

        for (int i = 0; i < n; ++i) {
            string[] tokens = Console.ReadLine()!.Trim().Split(' ');
            Employee emp = new Employee();
            emp.rating = double.Parse(tokens[0], CultureInfo.InvariantCulture);
            emp.left_child = int.Parse(tokens[1]);
            emp.right_sibling = int.Parse(tokens[2]);
            tree.Add(emp);
        }

        double result = planParty(tree);

        using (StreamWriter file = new StreamWriter("solution.txt")) {
            file.WriteLine(result.ToString("F2", CultureInfo.InvariantCulture));
        }
    }
}