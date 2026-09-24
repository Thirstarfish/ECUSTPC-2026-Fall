"""Compile and regression-test checker.cpp; Python standard library only."""
import argparse
from dataclasses import dataclass
import math
from pathlib import Path
import random
import subprocess
import tempfile
import time

S = 10**13
LIMIT = 10**18
LOW = (S - 10000)**2
HIGH = (S + 10000)**2
OK, WA, PE, FAIL = 0, 1, 2, 3
PROBLEM = Path(__file__).resolve().parents[1]


def render(points):
    return ''.join(f'{x} {y}\n' for x, y in points)


def required(n):
    return max(0, ((n**n).bit_length() - 1)//2 - n)


def oracle(points):
    """Independent exact Python-integer oracle for small geometric cases."""
    if any(abs(c) > LIMIT for p in points for c in p):
        return WA
    units = 0
    for i, (x, y) in enumerate(points):
        for u, v in points[:i]:
            d = (x-u)**2 + (y-v)**2
            if d < 10**18:
                return WA
            units += LOW <= d <= HIGH
    return OK if units >= required(len(points)) else WA


@dataclass
class Case:
    name: str
    sizes: list
    output: str
    expected: int
    answer: str


def cases():
    result = []

    def geometry(name, points, expected):
        assert oracle(points) == expected, name
        n = len(points)
        answer = render([(i*S, 0) for i in range(n)])
        result.append(Case(name, [n], render(points), expected, answer))

    geometry('sample', [(0, 0), (S, 0), (S//2, 8660254038000)], OK)
    geometry('coordinate_extremes', [(-LIMIT, -LIMIT), (LIMIT, LIMIT)], OK)
    geometry('duplicate', [(0, 0), (0, 0)], WA)
    geometry('coordinate_too_large', [(0, 0), (LIMIT+1, 0)], WA)
    geometry('coordinate_too_small', [(0, 0), (0, -LIMIT-1)], WA)
    tail = [(0, i*S) for i in [10, 20, 30, 40]]
    for delta in [-1, 0, 1]:
        geometry(f'minimum_{delta:+}', [(4*S, 0), (4*S+10**9+delta, 0)], WA if delta < 0 else OK)
        geometry(f'unit_lower_{delta:+}', [(8*S, 0), (9*S-10000+delta, 0)]+tail, WA if delta < 0 else OK)
        geometry(f'unit_upper_{delta:+}', [(7*S, 0), (8*S+10000+delta, 0)]+tail, WA if delta > 0 else OK)
    geometry('minimum_diagonal_exact', [(0, 0), (600000000, 800000000)], OK)
    geometry('minimum_diagonal_outside', [(0, 0), (600000000, 799999999)], WA)
    geometry('unit_diagonal_exact', [(0, 0), (3*S//5, 4*S//5)]+tail, OK)
    # n=8 requires exactly 4 pairs; isolates off-by-one and unordered-pair counting.
    geometry('exactly_required_pairs', [(i*S, 0) for i in range(5)]+tail[:3], OK)
    geometry('one_pair_short', [(i*S, 0) for i in range(4)]+tail, WA)

    valid = f'0 0\n{S} 0\n'
    for name, output, verdict in [
        ('decimal_token', '0 0\n1.0 0\n', PE),
        ('exponent_token', '0 0\n1e13 0\n', PE),
        ('nan_token', '0 0\nnan 0\n', PE),
        ('integer_overflow', '0 0\n'+str(2**63)+' 0\n', PE),
        ('long_token', '0 0\n'+'9'*100+' 0\n', PE),
        ('empty_output', '', PE),
        ('missing_coordinate', '0 0\n1\n', PE),
        ('extra_token', valid+'0\n', WA),
        ('trailing_whitespace', valid+' \t\n', OK),
    ]:
        result.append(Case(name, [2], output, verdict, valid))
    for name, answer in [
        ('jury_duplicate', '0 0\n0 0\n'),
        ('jury_malformed', '0 0\nx 0\n'),
        ('jury_missing', ''),
        ('jury_extra', valid+'0\n'),
    ]:
        result.append(Case(name, [2], valid, FAIL, answer))
    result.append(Case('second_case_invalid', [2, 2], valid+'0 0\n0 0\n', WA, valid*2))
    result.append(Case('second_case_missing', [2, 2], valid, PE, valid*2))
    result.append(Case('maximum_T', [2]*100, valid*100, OK, valid*100))

    rng = random.Random(20260925)
    for i in range(30):
        center = [10**9, S-10000, S+10000][i % 3]
        offset = rng.randint(-2, 2)
        x, y = rng.randint(-9000, 9000)*S, rng.randint(-9000, 9000)*S
        points = [(x, y), (x+center+offset, y)]
        if i % 3:
            points += [(x, y+j*S) for j in [10, 20, 30, 40]]
        geometry(f'oracle_{i:02}', points, oracle(points))

    # Generate a large answer, and independently certify enough hypercube edges.
    points = []
    vx = [math.sqrt(1-(0.0002*(1 << b))**2) for b in range(13)]
    for i in range(5000):
        x = round(sum(vx[b] for b in range(13) if i >> b & 1)*S)
        points.append((x, i*2000000000))
    edges = 0
    for i, (x, y) in enumerate(points):
        assert abs(x) <= LIMIT and abs(y) <= LIMIT
        for b in range(13):
            if i >> b & 1:
                u, v = points[i ^ (1 << b)]
                assert LOW <= (x-u)**2+(y-v)**2 <= HIGH
                edges += 1
    # Distinct y coordinates differ by at least 2*10^9, proving minimum separation.
    assert edges >= required(5000) == 25719
    large = render(points)
    result.append(Case('maximum_n', [5000], large, OK, large))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--checker', type=Path, help='Existing checker executable; otherwise compile with g++.')
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--export', type=Path, help='Save .in/.out/.ans and expected-verdict files.')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='unit-distance-checker-') as temp:
        work = Path(temp)
        checker = args.checker.resolve() if args.checker else work/'checker.exe'
        if not args.checker:
            subprocess.run([args.cxx, '-std=c++17', '-O2', str(PROBLEM/'checker.cpp'), '-o', str(checker)], check=True)
        all_cases = cases()
        failures = []
        for case in all_cases:
            folder = args.export.resolve()/case.name if args.export else work/'case'
            folder.mkdir(parents=True, exist_ok=True)
            inp, out, ans = [folder/name for name in ['input.in', 'participant.out', 'jury.ans']]
            inp.write_text(str(len(case.sizes))+'\n'+'\n'.join(map(str, case.sizes))+'\n')
            out.write_text(case.output)
            ans.write_text(case.answer)
            start = time.perf_counter()
            run = subprocess.run([str(checker), str(inp), str(out), str(ans)], capture_output=True, text=True, timeout=15)
            passed = run.returncode == case.expected
            print(f'{"PASS" if passed else "FAIL"} {case.name}: expected={case.expected}, actual={run.returncode}, {time.perf_counter()-start:.3f}s')
            if args.export:
                (folder/'expected.txt').write_text(f'{case.expected}\n')
            if not passed:
                failures.append(case.name)
                print(run.stdout+run.stderr)
        print(f'{len(all_cases)-len(failures)}/{len(all_cases)} passed')
        return bool(failures)


if __name__ == '__main__':
    raise SystemExit(main())
