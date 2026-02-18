public class Benchmark {
    public static void main(String[] args) {
        int iterations = args.length > 0 ? parseIntOrDefault(args[0], 10) : 10;
        long checksum = work(iterations);
        System.out.println("checksum = " + checksum);
    }

    private static int parseIntOrDefault(String s, int defaultValue) {
        try {
            return Integer.parseInt(s);
        } 
		catch (NumberFormatException e) {
            return defaultValue;
        }
    }

    private static long work(int iterations) {
        final int ArraySize = 5_000_000;
        int[] bigArray = new int[ArraySize];
        Tree[] forest = new Tree[iterations];
        long acc = 0;

        for (int i = 0; i < iterations; i++) {
            forest[i] = Tree.build(4 + ((i * 7) % 11));
            mutateArray(bigArray, i);
            acc += forest[i].sum();
        }
        return acc;
    }

    private static class Tree {
        Tree left, right;
        int payload;

        static Tree build(int depth) {
            Tree t = new Tree();
            t.payload = depth;
            if (depth > 0) {
                t.left = build(depth - 1);
                t.right = build(depth - 1);
            }
            return t;
        }

        long sum() {
            long l = left != null ? left.sum() : 0;
            long r = right != null ? right.sum() : 0;
            return payload + l + r;
        }
    }

    private static void mutateArray(int[] a, int iterationSeed) {
        int len = a.length;
        int step = 97;
        int i = 0;
        for (int count = 0; count < len; count++) {
            a[i] += iterationSeed + i;
            i = (i + step) % len;
        }
    }
}

