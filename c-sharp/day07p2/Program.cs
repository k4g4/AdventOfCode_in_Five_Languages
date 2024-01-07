using System.Collections;
using System.Diagnostics;

abstract class Item
{
    public abstract int Size();
}

class FileItem : Item
{
    private int _size;

    public FileItem(int size) => _size = size;

    public override int Size() => _size;
}

class DirItem : Item
{
    private Dictionary<string, Item> _items;
    private DirItem? _parent;

    public DirItem(DirItem? parent)
    {
        _items = new Dictionary<string, Item>();
        _parent = parent;
    }

    public Dictionary<string, Item> Items
    {
        get => _items;
        set => _items = value;
    }

    public DirItem? Parent => _parent;

    public override int Size() => _items.Aggregate(0, (size, item) => size + item.Value.Size());
}

class Program
{
    private const int TOTAL_SIZE = 70_000_000;
    private const int FREE_NEEDED = 30_000_000;

    static void Main()
    {
        var time = new Stopwatch();
        time.Start();

        DirItem root = new DirItem(null);
        DirItem workingDir = root;
        foreach (var line in File.ReadLines("input"))
        {
            if (line.StartsWith("$ cd"))
            {
                var target = line.Substring(5);
                if (target == "/")
                {
                    workingDir = root;
                }
                else if (target == "..")
                {
                    workingDir = workingDir.Parent ?? throw new Exception("parent shouldn't be null");
                }
                else
                {
                    workingDir = (DirItem)workingDir.Items[target];
                }
            }
            else if (line != "$ ls")
            {
                if (line.StartsWith("dir"))
                {
                    workingDir.Items.TryAdd(line.Substring(4), new DirItem(workingDir));
                }
                else
                {
                    var split = line.Split(' ');
                    workingDir.Items.TryAdd(split[1], new FileItem(Int32.Parse(split[0])));
                }
            }
        }

        var unusedSize = TOTAL_SIZE - root.Size();
        var sizeNeeded = FREE_NEEDED - unusedSize;

        Console.WriteLine($"size of dir to delete: {SmallestSizeOver(root, sizeNeeded)}");

        time.Stop();
        Console.WriteLine($"elapsed: {time.Elapsed.TotalMilliseconds} ms");
    }

    static int SmallestSizeOver(DirItem dir, int sizeNeeded)
    {
        var smallestSize = TOTAL_SIZE;

        var dirSize = dir.Size();
        if (dirSize < smallestSize && dirSize >= sizeNeeded)
        {
            smallestSize = dirSize;
        }

        foreach (var item in dir.Items)
        {
            if (item.Value is DirItem)
            {
                dirSize = SmallestSizeOver(((DirItem)item.Value), sizeNeeded);
                if (dirSize < smallestSize && dirSize >= sizeNeeded)
                {
                    smallestSize = dirSize;
                }
            }
        }

        return smallestSize;
    }
}