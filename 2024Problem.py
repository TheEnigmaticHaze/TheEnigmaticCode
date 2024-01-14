from functools import cache
from itertools import product
from memory_profiler import profile 

OPERATION_MAX = 6
NUMBERS = [2, 0, 2, 4]

POSSIBLE_CONCATENATIONS = [
  [2, 0, 2, 4],
  [20, 2, 4],
  [2, 0, 24]
]

INVALID_STACK = Exception("invalid stack")

to_find = set(range(1, 100 + 1))

OPERATIONS = {"id"  : 1, # identity/no-op
              "+"   : 2, # binary plus
              "-"   : 2, # binary minus
              "~"   : 1, # unary minus
              "*"   : 2, # multiply
              "/"   : 2, # divide
              "**"  : 2, # power
              "!"   : 1, # factorial
              "!!"  : 1, # double factorial
              "sqrt": 1} # square root

OP_LAMBDAS = {"id"  : (lambda v1     : v1                  ), # identity/no-op
              "+"   : (lambda v1, v2 : v1 + v2             ), # binary plus
              "-"   : (lambda v1, v2 : v1 - v2             ), # binary minus
              "~"   : (lambda v1     : -v1                 ), # unary minus
              "*"   : (lambda v1, v2 : v1 * v2             ), # multiply
              "/"   : (lambda v1, v2 : v1 / v2             ), # divide
              "**"  : (lambda v1, v2 : power(v1, v2)       ), # power
              "!"   : (lambda v1     : factorial(v1)       ), # factorial
              "!!"  : (lambda v1     : double_factorial(v1)), # double factorial
              "sqrt": (lambda v1     : v1 ** .5            )} # square root

def prefix_to_infix(stack):
  output_stack = []
  for element in reversed(stack):
    if element not in OPERATIONS.keys():
      output_stack.append(element)
    elif OPERATIONS[element] == 2:
      output_stack.append(f"({output_stack.pop()} {element} {output_stack.pop()})")
    elif element == "~":
      output_stack.append(f"-{output_stack.pop()}")
    elif element == "sqrt":
      output_stack.append(f"sqrt({output_stack.pop()})")
    elif element == "id":
      output_stack.append(f"{output_stack.pop()}")
    else:
      output_stack.append(f"({output_stack.pop()}{element})")
  return output_stack[0][1:-1]

@cache
def factorial(x : int) -> int:
  if x < 0 or x > 20:
    raise INVALID_STACK
  if x == 0:
    return 1
  return x * factorial(x - 1)

@cache
def double_factorial(x : int) -> int:
  if x < 0 or x > 20:
    raise INVALID_STACK
  if x == 0 or x == 1:
    return 1
  return x * double_factorial(x - 2)

@cache
def power(x : int, y : int) -> int:
  if y > 1024:
    raise INVALID_STACK
  if x > 20 and y > 20:
    raise INVALID_STACK
  return x ** y
  

def perform_operation(num_stack, operation):
  if operation not in OPERATIONS.keys():
    raise INVALID_STACK
  
  if len(num_stack) < OPERATIONS[operation]:
    raise INVALID_STACK

  value1 = num_stack.pop()

  if OPERATIONS[operation] == 1:
    num_stack.append(OP_LAMBDAS[operation](value1))
  else:
    num_stack.append(OP_LAMBDAS[operation](value1, num_stack.pop()))


def eval_stack(stack) -> int:
  num_stack = []

  while len(stack) != 0:
    value = stack.pop()
    if type(value) == int:
      num_stack.append(value)
    elif value in OPERATIONS.keys():
      perform_operation(num_stack, value)
    else:
      raise INVALID_STACK
  
  return num_stack[0]

def try_stack(stack : list):
  starting_stack = stack.copy()

  try:
    ans = eval_stack(stack)
    if ans in to_find:
      print(ans, prefix_to_infix(starting_stack))
      to_find.discard(ans)
      del starting_stack
  except KeyboardInterrupt:
    quit()
  except:
    del starting_stack
    return

def get_next_operation_list(operation_list):
  if "sqrt" in set(operation_list) and len(set(operation_list)) == 1:
    return []
  op_value = 0
  place_value = 0

  for op in reversed(operation_list):
    op_value += (10 ** place_value) * list(OPERATIONS.keys()).index(op)
    place_value += 1
  
  to_return = []
  op_value += 1
  for i in range(place_value):
    to_return.append(list(OPERATIONS.keys())[op_value % 10])
    op_value //= 10
  
  return tuple(reversed(to_return))

def generate_stack(numbers, current_op_list, grouping):
  number_index = 0
  group_index = 0
  operation_index = 0
  
  to_return = []
  while len(to_return) != len(numbers) + OPERATION_MAX:
    to_return.append(current_op_list[operation_index])
    operation_index += 1

    for i in range(grouping[group_index]):
      to_return.append(numbers[number_index])
      number_index += 1
    group_index += 1
  return to_return

@profile
def solve(numbers):
  current_op_list = ["id"] * OPERATION_MAX
  current_op_list = tuple(current_op_list)

  group_numbers = [x for x in product(range(len(numbers)), repeat=OPERATION_MAX) if sum(x) == len(numbers) and x[-1] != 0]

  while len(current_op_list) == OPERATION_MAX:
    current_op_list = get_next_operation_list(current_op_list)
    if sum(OPERATIONS[op] - 1 for op in current_op_list) + 1 != len(numbers):
      continue

    for grouping in group_numbers:
      stack = generate_stack(numbers, current_op_list, grouping)
      try_stack(stack)

for nums in POSSIBLE_CONCATENATIONS:
  solve(nums)
print(f"unfound numbers: {to_find}")