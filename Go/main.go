package main

import (
	"fmt"
	"os"
	"strconv"
)

func main() {
	iterations := 10
	if len(os.Args) > 1 {
		if n, err := strconv.Atoi(os.Args[1]); err == nil {
			iterations = n
		}
	}

	checksum := work(iterations)
	fmt.Printf("checksum = %d\n", checksum)
}

func work(iterations int) int64 {
	const arraySize = 5_000_000
	bigArray := make([]int, arraySize)
	forest := make([]*tree, iterations)
	var acc int64

	for i := 0; i < iterations; i++ {
		forest[i] = build(4 + ((i * 7) % 11))
		mutateArray(bigArray, i)
		acc += forest[i].sum()
	}
	return acc
}

type tree struct {
	left, right *tree
	payload     int
}

func build(depth int) *tree {
	t := &tree{payload: depth}
	if depth > 0 {
		t.left = build(depth - 1)
		t.right = build(depth - 1)
	}
	return t
}

func (t *tree) sum() int64 {
	if t == nil {
		return 0
	}
	return int64(t.payload) + t.left.sum() + t.right.sum()
}

func mutateArray(a []int, iterationSeed int) {
	len := len(a)
	step := 97
	i := 0
	for count := 0; count < len; count++ {
		a[i] += iterationSeed + i
		i = (i + step) % len
	}
}
