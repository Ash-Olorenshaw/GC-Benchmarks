import std.stdio;
import std.datetime.stopwatch : StopWatch, AutoStart;
import std.random : Random, uniform;
import std.conv : to;
import std.format : format;
import std.exception;

long work(int iterations) {
    enum int ArraySize = 5_000_000;
    int[] bigArray = new int[ArraySize];
    Tree[] forest = new Tree[iterations];
    long acc = 0;

    foreach (i; 0 .. iterations) {
        forest[i] = Tree.build(4 + ((i * 7) % 11));
        mutateArray(bigArray, i);
        acc += forest[i].sum();
    }
    return acc;
}

class Tree {
    Tree left, right;
    int payload;

    static Tree build(int depth) {
        auto t = new Tree();
        t.payload = depth;
        if (depth > 0) {
            t.left = build(depth - 1);
            t.right = build(depth - 1);
        }
        return t;
    }

    long sum() {
        long l = left ? left.sum() : 0;
        long r = right ? right.sum() : 0;
        return payload + l + r;
    }
}

void mutateArray(int[] a, int iterationSeed) {
    int len = cast(int) a.length;
    int step = 97;
    int i = 0;
    foreach (count; 0 .. len) {
        a[i] += iterationSeed + i;
        i = (i + step) % len;
    }
}

void main(string[] args) {
    int iterations = 10;
    if (args.length > 1)
        iterations = args[1].to!int.ifThrown(10);

    long checksum = work(iterations);
    writefln("checksum = %d", checksum);
}

