fn main()
{
  let mut to_be_sorted: Vec<i32> = vec![12, 2, 0, 5, 13, 10];

  print!("unsorted list: ");
  print_list(&to_be_sorted);

  bubble_sort(&mut to_be_sorted);

  print!("sorted list: ");
  print_list(&to_be_sorted);
}

fn is_list_sorted(list:&Vec<i32>) -> bool
{
  let length = list.len();

  for x in 0..length-1
  {
    if list[x] > list[x+1]
    {
      return false;
    }
  }

  return true;
}

fn print_list(list:&Vec<i32>)
{
  for x in list.iter()
  {
    print!("{} ", x);
  }

  println!();
}

fn bubble_sort(to_be_sorted:&mut Vec<i32>)
{
  while !is_list_sorted(&to_be_sorted)
  {
    for x in 0..to_be_sorted.len()-1
    {
      if to_be_sorted[x] > to_be_sorted[x+1]
      {
        let carry = to_be_sorted[x];
        to_be_sorted[x] = to_be_sorted[x+1];
        to_be_sorted[x+1] = carry;
      }
    }
  }
}