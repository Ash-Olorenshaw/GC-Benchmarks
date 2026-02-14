using System.Diagnostics;

class Program {
    static int Main(string[] args) {
        int iterations = args.Length > 0 && int.TryParse(args[0], out int n) ? n : 10;
        long checksum = Work(iterations);
        // Console.WriteLine($"checksum = {checksum}");
        return 0;
    }

    static long Work(int iterations) {
        const int ArraySize = 5_000_000;
        int[] bigArray = new int[ArraySize];
        Tree[] forest = new Tree[iterations];
        long acc = 0;

        for (int i = 0; i < iterations; i++) {
            forest[i] = Tree.Build(4 + ((i * 7) % 11));
            MutateArray(bigArray, i);
            acc += forest[i].Sum();
        }
        return acc;
    }

    class Tree {
        public Tree? Left, Right;
        public int Payload;

        public static Tree Build(int depth) {
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
            a[i] += iterationSeed + i;
            i = (i + step) % len;
        }
    }
}
