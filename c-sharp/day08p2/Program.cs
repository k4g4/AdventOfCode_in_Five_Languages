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
        var scores = new int[rows, columns];
        foreach (var row in Enumerable.Range(0, rows))
        {
            foreach (var column in Enumerable.Range(0, columns))
            {
                scores[row, column] = 1;
            }
        }

        Func<int?[]> newHeightDists = () =>
        {
            var heightDists = new int?[11]; // pos 10 is dist from edge
            heightDists[10] = 0;
            return heightDists;
        };

        var heightDists = newHeightDists();
        var visible = new HashSet<(int, int)>();

        Action<int, int> traverse = (row, column) =>
        {
            //search for nearest tree at this height or higher
            var height = trees[row][column];
            var nearestDist = heightDists[10];
            for (var i = height; i < 10; i++)
            {
                if (heightDists[i] != null)
                {
                    if (heightDists[i] < nearestDist)
                    {
                        nearestDist = heightDists[i];
                    }
                }
            }

            // increment all height distances
            for (var i = 0; i < 11; i++)
            {
                if (heightDists[i] != null)
                {
                    heightDists[i]++;
                }
            }

            heightDists[height] = 1; //reset this height

            scores[row, column] *= nearestDist ?? throw new Exception("unreachable");
        };

        // west to east
        foreach (var row in Enumerable.Range(0, rows))
        {
            heightDists = newHeightDists();
            foreach (var column in Enumerable.Range(0, columns))
            {
                traverse(row, column);
            }
        }

        // east to west
        foreach (var row in Enumerable.Range(0, rows))
        {
            heightDists = newHeightDists();
            foreach (var column in Enumerable.Range(0, columns).Reverse())
            {
                traverse(row, column);
            }
        }

        // north to south
        foreach (var column in Enumerable.Range(0, columns))
        {
            heightDists = newHeightDists();
            foreach (var row in Enumerable.Range(0, rows))
            {
                traverse(row, column);
            }
        }

        // south to north
        foreach (var column in Enumerable.Range(0, columns))
        {
            heightDists = newHeightDists();
            foreach (var row in Enumerable.Range(0, rows).Reverse())
            {
                traverse(row, column);
            }
        }

        var max = 0;
        foreach (var row in Enumerable.Range(0, rows))
        {
            foreach (var column in Enumerable.Range(0, columns))
            {
                max = Math.Max(scores[row, column], max);
            }
        }

        Console.WriteLine($"max score: {max}");

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}