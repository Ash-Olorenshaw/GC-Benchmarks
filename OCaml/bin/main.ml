type tree = {
  mutable left : tree option;
  mutable right : tree option;
  payload : int;
}

let rec tree_build depth =
  let t = { left = None; right = None; payload = depth } in
  if depth > 0 then begin
    t.left <- Some (tree_build (depth - 1));
    t.right <- Some (tree_build (depth - 1))
  end;
  t

let rec tree_sum self =
  let l = match self.left with
    | Some s -> tree_sum s
    | None -> 0
  in
  let r = match self.right with
    | Some s -> tree_sum s
    | None -> 0
  in
  self.payload + l + r

let mutate_array a iteration_seed =
  let len = Array.length a in
  let step = 97 in
  let rec muta i count =
    if count < len then begin
      a.(i) <- a.(i) + (iteration_seed + i);
      muta ((i + step) mod len) (count + 1)
    end
  in
  muta 0 0

let work iterations =
  let array_size = 5_000_000 in
  let big_array = Array.make array_size 0 in
  let forest = Array.make iterations { left = None; right = None; payload = 0 } in
  let rec acc i count =
    if i < iterations then begin
      forest.(i) <- tree_build (4 + ((i * 7) mod 11));
      mutate_array big_array i;
      acc (i + 1) (count + tree_sum forest.(i))
    end
    else
      count
  in
  acc 0 0

let () =
  let iterations =
    if Array.length Sys.argv > 1 then
      int_of_string Sys.argv.(1)
    else
      10
  in
  let checksum = work iterations in
  (* Printf.printf "checksum = %d\n" checksum; *)
  ignore checksum

