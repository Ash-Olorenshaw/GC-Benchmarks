open System
open System.Diagnostics

let Rng : Random = new Random 42

type Tree = {
    mutable Left : Option<Tree>
    mutable Right : Option<Tree>
    Payload : int
}

let rec TreeBuild(self : Tree) (depth : int) : Tree =
    let t : Tree = { Left = None; Right = None; Payload = depth }
    if depth > 0 then
        t.Left <- Some(TreeBuild t (depth - 1))
        t.Right <- Some(TreeBuild t (depth - 1))
    t

let TreeBuildTree (self : Tree) =
    TreeBuild self (Rng.Next(4, 15))

let rec TreeSum (self : Tree) =
    let l = 
        match self.Left with 
        | Some s -> TreeSum s
        | None -> 0
    let r = 
        match self.Right with 
        | Some s -> TreeSum s
        | None -> 0
    self.Payload + l + r


let MutateArray (a : int[]) (iterationSeed : int) =
    let len : int = a.Length
    let step : int = 97
    let rec muta (i : int) (count : int) = 
        if count < len then
            a[i] <- a[i] + (iterationSeed + i)
            muta ((i + step) % len) (count + 1)
        else 
            ()
    muta 0 0 

let Work (iterations : int) =
    let ArraySize : int = 5_000_000
    let bigArray : int[] = Array.zeroCreate ArraySize
    let forest : Tree[] = Array.zeroCreate iterations
    let rec acc i count = 
        if i < iterations then
            let t : Tree = { Left = None; Right = None; Payload = 0 }
            forest[i] <- TreeBuildTree t
            MutateArray bigArray i
            acc (i + 1) (count + TreeSum(forest[i]))
        else
            count
    acc 0 0

[<EntryPoint>]
let main args = 
    printfn "Starting F# benchmark..."
    let iterations : int = 
        if args.Length > 0 then 
            Int32.Parse(args[0])
        else 10

    let sw = Stopwatch.StartNew()
    let checksum = Work iterations
    sw.Stop()
    printfn $"{iterations} iterations in {sw.Elapsed.TotalSeconds:F2}s"
    printfn $"checksum = {checksum}"
    0


