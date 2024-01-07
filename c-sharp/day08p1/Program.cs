using System.Collections;
using System.Diagnostics;

class Program
{
    static void Main()
    {
        var time = new Stopwatch();
        time.Start();

        foreach (var line in File.ReadLines("input"))
        {
            Console.WriteLine(line);
        }

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}