using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

class Knot
{
    private Knot? _next;

    public Knot Next => _next ?? throw new Exception("don't access Next on head");

    public Knot(Knot? next)
    {
        _next = next;
        x = 0;
        y = 0;
    }

    public int x { get; set; }
    public int y { get; set; }

    public void FollowNext()
    {
        if (Next.y - y == -2)
        {
            y--;
            if (Next.x - x == 2)
            {
                x++;
            }
            else if (Next.x - x == -2)
            {
                x--;
            }
            else
            {
                x = Next.x;
            }
        }
        if (Next.y - y == 2)
        {
            y++;
            if (Next.x - x == 2)
            {
                x++;
            }
            else if (Next.x - x == -2)
            {
                x--;
            }
            else
            {
                x = Next.x;
            }
        }
        if (Next.x - x == -2)
        {
            x--;
            if (Next.y - y == 2)
            {
                y++;
            }
            else if (Next.y - y == -2)
            {
                y--;
            }
            else
            {
                y = Next.y;
            }
        }
        if (Next.x - x == 2)
        {
            x++;
            if (Next.y - y == 2)
            {
                y++;
            }
            else if (Next.y - y == -2)
            {
                y--;
            }
            else
            {
                y = Next.y;
            }
        }
    }
}

class Program
{
    static void Step(char dir, Knot head, Knot[] tail)
    {
        switch (dir)
        {
            case 'U':
                {
                    head.y--;
                    break;
                }

            case 'D':
                {
                    head.y++;
                    break;
                }
            case 'L':
                {
                    head.x--;
                    break;
                }
            case 'R':
                {
                    head.x++;
                    break;
                }
        }

        foreach (var knot in tail)
        {
            knot.FollowNext();
        }
    }

    static void Main()
    {
        var time = new Stopwatch();
        time.Start();

        var positions = new HashSet<(int, int)>();
        positions.Add((0, 0));

        var head = new Knot(null);
        var next = head;
        var tail = new Knot[9];
        for (var i = 0; i < 9; i++)
        {
            tail[i] = new Knot(next);
            next = tail[i];
        }

        foreach (var line in File.ReadLines("input"))
        {
            var split = line.Split(' ');
            var dir = split[0][0];
            var steps = Int32.Parse(split[1]);

            foreach (var _ in Enumerable.Range(0, steps))
            {
                Step(dir, head, tail);
                positions.Add((tail[8].x, tail[8].y));
            }
        }

        Console.WriteLine($"positions visited: {positions.Count()}");

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }
}