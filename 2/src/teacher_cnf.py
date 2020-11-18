import math
import pycosat
from typing import List
from pathlib import Path


TeacherSubjects = List[int]
CNF = List[List[int]]


def teacher_cnf(teacher_subjects: List[TeacherSubjects], num_subjects: int, subset_size: int):
    variables = {}
    inv_variables = {}
    num_teachers = len(teacher_subjects)
    subject_teachers = [[] for s in range(num_subjects)]
    for i, subjects in enumerate(teacher_subjects):
        for subject in subjects:
            subject_teachers[subject].append(i)
    unique_bitstring_size = math.ceil(math.log2(num_teachers))
    teacher_bitstring = {}
    bitstring_matrix = []
    extra_variables = []
    next_var = 1
    for teacher in range(num_teachers):
        teacher_bitstring[teacher] = [bool(int(x)) for x in ('{:0' + str(unique_bitstring_size) + 'b}').format(teacher)]
        inv_variables[next_var] = teacher
        variables[teacher] = next_var
        next_var += 1
    for g in range(subset_size):
        bitstring = []
        for bit in range(unique_bitstring_size):
            bitstring.append(next_var)
            next_var += 1
        bitstring_matrix.append(bitstring)
        extra = []
        for i in range(num_teachers):
            extra.append(next_var)
            next_var += 1
        extra_variables.append(extra)
    cnf: CNF = []
    for subject in range(num_subjects):
        cnf.append([variables[t] for t in subject_teachers[subject]])
    for teacher in range(num_teachers):
        teacher_bitstring_clause = [-variables[teacher]]
        for g in range(subset_size):
            teacher_bitstring_clause.append(extra_variables[g][teacher])
            for bit in range(unique_bitstring_size):
                phi = bitstring_matrix[g][bit] if teacher_bitstring[teacher][bit] else -bitstring_matrix[g][bit]
                cnf.append([-extra_variables[g][teacher], phi])
        cnf.append(teacher_bitstring_clause)
    return cnf, next_var, inv_variables


def solve_teacher_cnf(cnf: CNF, inv_variables):
    solution = pycosat.solve(cnf)
    if solution in ('UNSAT', 'UNKNOWN'):
        return solution
    teacher_subset = []
    for var in solution:
        if var in inv_variables:
            teacher = inv_variables[var]
            teacher_subset.append(teacher)
    return teacher_subset


def write_dimacs(path: Path, cnf: CNF, num_variables: int):
    lines = [f'p cnf {num_variables} {len(cnf)}']
    for clause in cnf:
        lines.append(f'{" ".join(map(str, clause))} 0')
    with path.open('w') as f:
        f.write('\n'.join(lines))


def parse_data(data: Path):
    with data.open('r') as f:
        lines = f.read().splitlines()
        num_teachers, num_subjects, subset_size = map(int, lines.pop(0).split())
        teacher_subjects = list(map(lambda l: list(map(int, l.split())), lines))
    return teacher_subjects, num_subjects, subset_size


def main():
    data = Path(input('Path to input file:\n'))
    teacher_subjects, num_subjects, subset_size = parse_data(data)
    cnf, num_variables, inv_variables = teacher_cnf(teacher_subjects, num_subjects, subset_size)
    write_dimacs(data.with_suffix('.dimacs'), cnf, num_variables)
    print('Solution:', solve_teacher_cnf(cnf, inv_variables))


if __name__ == '__main__':
    main()
