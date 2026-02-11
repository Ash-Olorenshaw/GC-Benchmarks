using System.Diagnostics;

class Program {
    static Random Rng = new Random(42);

    static int Main(string[] args) {
	System.Console.Write("Starting C# benchmark...\n");
        int iterations = args.Length > 0 && int.TryParse(args[0], out int n) ? n : 10;
        var sw = Stopwatch.StartNew();
        long checksum = Work(iterations);
        sw.Stop();
        Console.WriteLine($"{iterations} iterations in {sw.Elapsed.TotalSeconds:F2}s");
        Console.WriteLine($"checksum = {checksum}");
        return 0;
    }

    static long Work(int iterations) {
        const int ArraySize = 5_000_000;
        int[] bigArray = new int[ArraySize];
        Tree[] forest = new Tree[iterations];
        long acc = 0;

        for (int i = 0; i < iterations; i++) {
            forest[i] = Tree.BuildTree();
            MutateArray(bigArray, i);
            acc += forest[i].Sum();
        }
        return acc;
    }

    class Tree {
        public Tree? Left, Right;
        public int Payload;

        public static Tree BuildTree() {
            return Build(Rng.Next(4, 15));
        }

        private static Tree Build(int depth) {
            var t = new Tree { Payload = depth };
            if (depth > 0) {
                t.Left = Build(depth - 1);
                t.Right = Build(depth - 1);
            }
            return t;
        }

        public long Sum() {
            long l = Left?.Sum() ?? 0;
            long r = Right?.Sum() ?? 0;
            return Payload + l + r;
        }
    }

    static void MutateArray(int[] a, int iterationSeed) {
        int len = a.Length;
        int step = 97;
        int i = 0;
        for (int count = 0; count < len; count++) {
            // a[i] ^= iterationSeed + i;
            a[i] += iterationSeed + i;
            i = (i + step) % len;
        }
    }
}
