using System.Diagnostics;

class Knot
{
    public Knot()
    {
        x = 0;
        y = 0;
    }

    public int x { get; set; }
    public int y { get; set; }

    public bool Touching(Knot other) => Math.Abs(x - other.x) < 2 && Math.Abs(y - other.y) < 2;
}

class Program
{
    static void Step(char dir, Knot head, Knot tail, HashSet<(int, int)> positions)
    {
        switch (dir)
        {
            case 'U':
                {
                    head.y--;
                    if (!head.Touching(tail))
                    {
                        tail.x = head.x;
                        tail.y = head.y + 1;
                    }
                    break;
                }

            case 'D':
                {
                    head.y++;
                    if (!head.Touching(tail))
                    {
                        tail.x = head.x;
                        tail.y = head.y - 1;
                    }
                    break;
                }
            case 'L':
                {
                    head.x--;
                    if (!head.Touching(tail))
                    {
                        tail.x = head.x + 1;
                        tail.y = head.y;
                    }
                    break;
                }
            case 'R':
                {
                    head.x++;
                    if (!head.Touching(tail))
                    {
                        tail.x = head.x - 1;
                        tail.y = head.y;
                    }
                    break;
                }
        }

        positions.Add((tail.x, tail.y));
    }

    static void Main()
    {
        var time = new Stopwatch();
        time.Start();

        var positions = new HashSet<(int, int)>();
        positions.Add((0, 0));
        var (head, tail) = (new Knot(), new Knot());
        foreach (var line in File.ReadLines("input"))
        {
            var split = line.Split(' ');
            char dir = split[0][0];
            int steps = Int32.Parse(split[1]);
            foreach (var _ in Enumerable.Range(0, steps))
            {
                Step(dir, head, tail, positions);
            }
        }

        Console.WriteLine($"positions visited: {positions.Count()}");

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}