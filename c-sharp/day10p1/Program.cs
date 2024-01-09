using System;
using System.Diagnostics;

class Program
{
    static void Main()
    {
        var time = new Stopwatch();
        time.Start();

        var instructions = new List<int>();

        foreach (var line in File.ReadLines("input"))
        {
            if (line == "noop")
            {
                instructions.Add(0);
            }
            else
            {
                instructions.Add(0);
                instructions.Add(Int32.Parse(line.Split(' ')[1]));
            }
        }

        var (i, register, total) = (0, 1, 0);
        foreach (var cycle in new int[] { 19, 40, 40, 40, 40, 40 })
        {
            foreach (var _ in Enumerable.Range(0, cycle))
            {
                register += instructions[i++];
            }

            total += (i + 1) * register;
        }

        Console.WriteLine($"total: {total}");

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}