using System.Collections;
using System.Diagnostics;
using System.Text;

class Program
{
    static void Main()
    {
        var time = new Stopwatch();
        time.Start();

        var trees =
            new List<int[]>(
                File
                    .ReadLines("input")
                    .Select(line =>
                        Encoding
                            .ASCII
                            .GetBytes(line)
                            .Select(b => b - 0x30)
                            .ToArray<int>()));
        var (rows, columns) = (trees.Count(), trees[0].Count());
        var height = -1;
        var visible = new HashSet<(int, int)>();

        Action<int, int> traverse = (row, column) =>
        {
            if (trees[row][column] > height)
            {
                height = trees[row][column];
                visible.Add((row, column));
            }
        };

        // west to east
        foreach (var row in Enumerable.Range(0, rows))
        {
            height = -1;
            foreach (var column in Enumerable.Range(0, columns))
            {
                traverse(row, column);
            }
        }

        // east to west
        foreach (var row in Enumerable.Range(0, rows))
        {
            height = -1;
            foreach (var column in Enumerable.Range(0, columns).Reverse())
            {
                traverse(row, column);
            }
        }

        // north to south
        foreach (var column in Enumerable.Range(0, columns))
        {
            height = -1;
            foreach (var row in Enumerable.Range(0, rows))
            {
                traverse(row, column);
            }
        }

        // south to north
        foreach (var column in Enumerable.Range(0, columns))
        {
            height = -1;
            foreach (var row in Enumerable.Range(0, rows).Reverse())
            {
                traverse(row, column);
            }
        }

        Console.WriteLine($"total visible: {visible.Count()}");

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}