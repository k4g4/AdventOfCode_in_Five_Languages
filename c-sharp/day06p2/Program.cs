using System.Collections;
using System.Diagnostics;

class Program
{
    static void Main()
    {
        const int MARKER_LEN = 14;

        var time = new Stopwatch();
        time.Start();

        var input = File.ReadAllBytes("input");

        var buf = new Queue<byte>(MARKER_LEN + 1);
        for (int i = 0; i < input.Count(); i++)
        {
            buf.Enqueue(input[i]);
            if (buf.Count > MARKER_LEN)
            {
                buf.Dequeue();
            }
            if (buf.Distinct().Count() == MARKER_LEN)
            {
                Console.WriteLine($"marker at {i + 1}");
                break;
            }
        }

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}