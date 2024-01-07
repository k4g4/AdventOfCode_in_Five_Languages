using System.Collections;
using System.Diagnostics;

class Program
{
    static void Main()
    {
        var time = new Stopwatch();
        time.Start();

        var input = File.ReadAllBytes("input");

        var buf = new Queue<byte>(5);
        for (int i = 0; i < input.Count(); i++)
        {
            buf.Enqueue(input[i]);
            if (buf.Count > 4)
            {
                buf.Dequeue();
            }
            if (buf.Distinct().Count() == 4)
            {
                Console.WriteLine($"marker at {i + 1}");
                break;
            }
        }

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}